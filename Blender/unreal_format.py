import bpy
import bpy_extras
from bpy.props import StringProperty, BoolProperty, PointerProperty, EnumProperty, FloatProperty, FloatVectorProperty, CollectionProperty
from bpy.types import Scene

import struct
import os
import io
import time
import gzip
from mathutils import Vector, Matrix, Quaternion, Euler
from math import *

from pip._internal import main as pipmain
pipmain(['install', 'zstd'])
import zstd

# ---------- ADDON ---------- #

bl_info = {
    "name": "Unreal Format (.uemodel / .ueanim / .ueworld)",
    "author": "Half",
    "version": (1, 0, 0),
    "blender": (3, 1, 0),
    "location": "View3D > Sidebar > Unreal Format",
    "category": "Import",
}

class UFPanel(bpy.types.Panel):
    bl_category = "Unreal Format"
    bl_label = "Unreal Format"
    bl_region_type = 'UI'
    bl_space_type = 'VIEW_3D'
    
    def draw(self, context):
        UFPanel.draw_model_options(self, context)
        self.layout.row().operator("uf.import_uemodel", icon='MESH_DATA')
        UFPanel.draw_anim_options(self, context)
        self.layout.row().operator("uf.import_ueanim", icon='ANIM')
       
    @staticmethod
    def draw_model_options(self, context):
        layout = self.layout

        box = layout.box()
        box.label(text="Model Options", icon="OUTLINER_OB_MESH")
        box.row().prop(bpy.context.scene.uf_settings, "reorient_bones")
        box.row().prop(bpy.context.scene.uf_settings, "bone_length")
        box.row().prop(bpy.context.scene.uf_settings, "scale")
        
    @staticmethod
    def draw_anim_options(self, context):
        layout = self.layout

        box = layout.box()
        box.label(text="Anim Options", icon="ACTION")
        box.row().prop(bpy.context.scene.uf_settings, "rotation_only")
        
class UFImportUEModel(bpy.types.Operator, bpy_extras.io_utils.ImportHelper):
    bl_idname = "uf.import_uemodel"
    bl_label = "Import Model"
    bl_context = 'scene'
    
    filename_ext = ".uemodel"
    filter_glob: StringProperty(default="*.uemodel", options={'HIDDEN'}, maxlen=255)
    files: CollectionProperty(type=bpy.types.OperatorFileListElement, options={'HIDDEN', 'SKIP_SAVE'})
    directory: StringProperty(subtype='DIR_PATH')
    
    def execute(self, context):
        for file in self.files:
            import_file(os.path.join(self.directory, file.name))
        return {'FINISHED'}

    def draw(self, context):
       UFPanel.draw_model_options(self, context)
       
class UFImportUEAnim(bpy.types.Operator, bpy_extras.io_utils.ImportHelper):
    bl_idname = "uf.import_ueanim"
    bl_label = "Import Animation"
    bl_context = 'scene'
    
    filename_ext = ".ueanim"
    filter_glob: StringProperty(default="*.ueanim", options={'HIDDEN'}, maxlen=255)
    files: CollectionProperty(type=bpy.types.OperatorFileListElement, options={'HIDDEN', 'SKIP_SAVE'})
    directory: StringProperty(subtype='DIR_PATH')
    
    def execute(self, context):
        for file in self.files:
            import_file(os.path.join(self.directory, file.name))
        return {'FINISHED'}

    def draw(self, context):
       UFPanel.draw_anim_options(self, context)
    
class UFSettings(bpy.types.PropertyGroup):
    scale: FloatProperty(name="Scale", default=0.01, min = 0.01)
    bone_length: FloatProperty(name="Bone Length", default=5, min = 0.1)
    reorient_bones: BoolProperty(name="Reorient Bones", default=False)
    rotation_only: BoolProperty(name="Rotation Only", default=False)
    
def draw_import_menu(self, context):
    self.layout.operator(UFImportUEModel.bl_idname, text = "Unreal Model (.uemodel)")
    self.layout.operator(UFImportUEAnim.bl_idname, text = "Unreal Animation (.ueanim)")

operators = [UFPanel, UFImportUEModel, UFImportUEAnim, UFSettings]

def register():
    for operator in operators:
        bpy.utils.register_class(operator)
        
    Scene.uf_settings = PointerProperty(type=UFSettings)
    bpy.types.TOPBAR_MT_file_import.append(draw_import_menu)

def unregister():
    for operator in operators:
        bpy.utils.unregister_class(operator)
        
    del Scene.uf_settings
    bpy.types.TOPBAR_MT_file_import.remove(draw_import_menu)

if __name__ == "__main__":
    register()
    
# ---------- IMPORT CLASSES ---------- #

def bytes_to_str(in_bytes):
    return in_bytes.rstrip(b'\x00').decode()

class FArchiveReader:
    data = None
    size = 0

    def __init__(self, data):
        self.data = io.BytesIO(data)
        self.size = len(self.data.read())
        self.data.seek(0)

    def __enter__(self):
        self.size = len(self.data.read())
        self.data.seek(0)
        return self

    def __exit__(self, type, value, traceback):
        self.data.close()

    def eof(self):
        return self.data.tell() >= self.size

    def read(self, size: int):
        return self.data.read(size)

    def read_to_end(self):
        return self.data.read(self.size - self.data.tell())

    def read_bool(self):
        return struct.unpack("?", self.data.read(1))[0]

    def read_string(self, size: int):
        string =  self.data.read(size)
        return bytes_to_str(string)

    def read_fstring(self):
        size, = struct.unpack("i", self.data.read(4))
        string = self.data.read(size)
        return bytes_to_str(string)

    def read_int(self):
        return struct.unpack("i", self.data.read(4))[0]

    def read_int_vector(self, size: int):
        return struct.unpack(str(size) + "I", self.data.read(size*4))

    def read_short(self):
        return struct.unpack("h", self.data.read(2))[0]

    def read_byte(self):
        return struct.unpack("c", self.data.read(1))[0]

    def read_float(self):
        return struct.unpack("f", self.data.read(4))[0]

    def read_float_vector(self, size: int):
        return struct.unpack(str(size) + "f", self.data.read(size*4))

    def read_byte(self):
        return struct.unpack("c", self.data.read(1))

    def read_byte_vector(self, size: int):
        return struct.unpack(str(size) + "B", self.data.read(size))

    def skip(self, size: int):
        self.data.read(size)
        
    def read_bulk_array(self, predicate):
        count = self.read_int()
        return self.read_array(count, predicate)

    def read_array(self, count, predicate):
        array = []
        for counter in range(count):
            array.append(predicate(self))
        return array
    
    

class UEModel:
    vertices = []
    indices = []
    normals = []
    tangents = []
    colors = []
    uvs = []
    materials = []
    morphs = []
    weights = []
    bones = []
    sockets = []
    
class Material:
    material_name = ""
    first_index = -1
    num_faces = -1

    def __init__(self, ar: FArchiveReader):
        self.material_name = ar.read_fstring()
        self.first_index = ar.read_int()
        self.num_faces = ar.read_int()

class Bone:
    name = ""
    parent_index = -1
    position = []
    rotation = []

    def __init__(self, ar: FArchiveReader):
        self.name = ar.read_fstring()
        self.parent_index = ar.read_int()
        self.position = [pos * bpy.context.scene.uf_settings.scale for pos in ar.read_float_vector(3)]
        self.rotation = ar.read_float_vector(4)

class Weight:
    bone_index = -1
    vertex_index = -1
    weight = -1

    def __init__(self, ar: FArchiveReader):
        self.bone_index = ar.read_short()
        self.vertex_index = ar.read_int()
        self.weight = ar.read_float()

class MorphTarget:
    name = ""
    deltas = []

    def __init__(self, ar: FArchiveReader):
        self.name = ar.read_fstring()

        self.deltas = ar.read_bulk_array(lambda ar: MorphTargetData(ar))

class MorphTargetData:
    position = []
    normals = []
    vertex_index = -1

    def __init__(self, ar: FArchiveReader):
        self.position = [pos * bpy.context.scene.uf_settings.scale for pos in ar.read_float_vector(3)]
        self.normals = ar.read_float_vector(3)
        self.vertex_index = ar.read_int()

class Socket:
    name = ""
    parent_name = ""
    position = []
    rotation = []
    scale = []

    def __init__(self, ar: FArchiveReader):
        self.name = ar.read_fstring()
        self.parent_name = ar.read_fstring()
        self.position = [pos * bpy.context.scene.uf_settings.scale for pos in ar.read_float_vector(3)]
        self.rotation = ar.read_float_vector(4)
        self.scale = ar.read_float_vector(3)
        
class UEAnim:
    num_frames = 0
    frames_per_second = 0
    tracks = []
    
class Track:
    name = ""
    keys = []

    def __init__(self, ar: FArchiveReader):
        self.name = ar.read_fstring()
        self.keys = ar.read_bulk_array(lambda ar: BoneKey(ar))
        
class BoneKey:
    position = []
    rotation = []
    scale = []

    def __init__(self, ar: FArchiveReader):
        self.position = [pos * bpy.context.scene.uf_settings.scale for pos in ar.read_float_vector(3)]
        self.rotation = ar.read_float_vector(4)
        self.scale = ar.read_float_vector(3)
        
# ---------- IMPORT FUNCTIONS ---------- #

MAGIC = "UNREALFORMAT"
MODEL_IDENTIFIER = "UEMODEL"
ANIM_IDENTIFIER = "UEANIM"

def import_file(path: str):
    with open(path, 'rb') as file:
        import_data(file.read())

def import_data(data, link_model: bool = True):
    with FArchiveReader(data) as ar:
        magic = ar.read_string(len(MAGIC))
        if magic != MAGIC:
            return

        identifier = ar.read_fstring()
        file_version = ar.read_byte()
        object_name = ar.read_fstring()
        print(f"Importing {object_name}")

        read_archive = ar
        is_compressed = ar.read_bool()
        if is_compressed:
            compression_type = ar.read_fstring()
            uncompressed_size = ar.read_int()
            compressed_size = ar.read_int()
            
            print(f"Compressed with {compression_type}")

            if compression_type == "GZIP":
                read_archive = FArchiveReader(gzip.decompress(ar.read_to_end()))
            elif compression_type == "ZSTD":
                read_archive = FArchiveReader(zstd.ZSTD_uncompress(ar.read_to_end()))
            else:
                print(f"Unknown Compression Type: {compression_type}")
                return


        if identifier == MODEL_IDENTIFIER:
            return import_uemodel_data(read_archive, object_name, link_model)
        elif identifier == ANIM_IDENTIFIER:
            return import_ueanim_data(read_archive, object_name)
        
def import_uemodel_data(ar: FArchiveReader, name: str, link: bool):
    data = UEModel()

    while not ar.eof():
        header_name = ar.read_fstring()
        array_size = ar.read_int()
        byte_size = ar.read_int()

        if header_name == "VERTICES":
            data.vertices = ar.read_array(array_size, lambda ar: [vert * bpy.context.scene.uf_settings.scale for vert in ar.read_float_vector(3)])
        elif header_name == "INDICES":
            data.indices = ar.read_array(int(array_size / 3), lambda ar: ar.read_int_vector(3))
        elif header_name == "NORMALS":
            data.normals = ar.read_array(array_size, lambda ar: ar.read_float_vector(3))
        elif header_name == "TANGENTS":
            data.tangents = ar.read_array(array_size, lambda ar: ar.read_float_vector(3))
        elif header_name == "VERTEXCOLORS":
            data.colors = ar.read_array(array_size, lambda ar: ar.read_byte_vector(4))
        elif header_name == "TEXCOORDS":
            data.uvs = ar.read_array(array_size, lambda ar: ar.read_array(ar.read_int(), lambda ar: ar.read_float_vector(2)))
        elif header_name == "MATERIALS":
            data.materials = ar.read_array(array_size, lambda ar: Material(ar))
        elif header_name == "WEIGHTS":
            data.weights = ar.read_array(array_size, lambda ar: Weight(ar))
        elif header_name == "BONES":
            data.bones = ar.read_array(array_size, lambda ar: Bone(ar))
        elif header_name == "MORPHTARGETS":
            data.morphs = ar.read_array(array_size, lambda ar: MorphTarget(ar))
        elif header_name == "SOCKETS":
            data.sockets = ar.read_array(array_size, lambda ar: Socket(ar))
        else:
            ar.skip(byte_size)

    # geometry
    mesh_data = bpy.data.meshes.new(name)
    mesh_data.from_pydata(data.vertices, [], data.indices)

    mesh_object = bpy.data.objects.new(name, mesh_data)
    return_object = mesh_object
    if link:
        bpy.context.scene.collection.objects.link(mesh_object)

    # normals
    if len(data.normals) > 0:
        mesh_data.polygons.foreach_set("use_smooth", [True] * len(mesh_data.polygons))
        mesh_data.normals_split_custom_set_from_vertices(data.normals)
        mesh_data.use_auto_smooth = True

    # weights
    if len(data.weights) > 0 and len(data.bones) > 0:
        for weight in data.weights:
            bone_name = data.bones[weight.bone_index].name
            vertex_group = mesh_object.vertex_groups.get(bone_name)
            if not vertex_group:
                vertex_group = mesh_object.vertex_groups.new(name = bone_name)
            vertex_group.add([weight.vertex_index], weight.weight, 'ADD')

    # morph targets
    if len(data.morphs) > 0:
        default_key = mesh_object.shape_key_add(from_mix=False)
        default_key.name = "Default"
        default_key.interpolation = 'KEY_LINEAR'

        for morph in data.morphs:
            key = mesh_object.shape_key_add(from_mix=False)
            key.name = morph.name
            key.interpolation = 'KEY_LINEAR'

            for delta in morph.deltas:
                key.data[delta.vertex_index].co += Vector(delta.position)
                
    # vertex colors
    if len(data.colors) > 0:
        vertex_color = mesh_data.vertex_colors.new(name="COL0", do_init=False)
        for polygon in mesh_data.polygons:
            for vertex_index, loop_index in zip(polygon.vertices, polygon.loop_indices):
                color = data.colors[vertex_index]

    # texture coordinates
    if len(data.uvs) > 0:
        for index, uvs in enumerate(data.uvs):
            uv_layer = mesh_data.uv_layers.new(name="UV" + str(index))
            for polygon in mesh_data.polygons:
                for vertex_index, loop_index in zip(polygon.vertices, polygon.loop_indices):
                    uv_layer.data[loop_index].uv = uvs[vertex_index]

    # materials
    if len(data.materials) > 0:
        for material in data.materials:
            if material.material_name == "":
                mesh_data.materials.append(None)
                continue
            
            mat = bpy.data.materials.get(material.material_name)
            if mat is None:
                mat = bpy.data.materials.new(name=material.material_name)
            mesh_data.materials.append(mat)

    # skeleton
    if len(data.bones) > 0 or len(data.sockets) > 0:
        armature_data = bpy.data.armatures.new(name=name)
        armature_data.display_type = 'STICK'

        armature_object = bpy.data.objects.new(name + "_Skeleton", armature_data)
        armature_object.show_in_front = True
        return_object = armature_object

        if link:
            bpy.context.scene.collection.objects.link(armature_object)
        bpy.context.view_layer.objects.active = armature_object
        armature_object.select_set(True)

        mesh_object.parent = armature_object
                
    if len(data.bones) > 0:
        # create bones
        bpy.ops.object.mode_set(mode='EDIT')
        edit_bones = armature_data.edit_bones
        for bone in data.bones:
            bone_pos = Vector(bone.position)
            bone_rot = Quaternion((bone.rotation[3], bone.rotation[0], bone.rotation[1], bone.rotation[2])) # xyzw -> wxyz

            edit_bone = edit_bones.new(bone.name)
            edit_bone.length = bpy.context.scene.uf_settings.bone_length * bpy.context.scene.uf_settings.scale

            bone_matrix = Matrix.Translation(bone_pos) @ bone_rot.to_matrix().to_4x4()

            if bone.parent_index >= 0:
                parent_bone = edit_bones.get(data.bones[bone.parent_index].name)
                edit_bone.parent = parent_bone
                bone_matrix = parent_bone.matrix @ bone_matrix
    
            edit_bone.matrix = bone_matrix
            
        bpy.ops.object.mode_set(mode='OBJECT')

        # armature modifier
        armature_modifier = mesh_object.modifiers.new(armature_object.name, type='ARMATURE')
        armature_modifier.show_expanded = False
        armature_modifier.use_vertex_groups = True
        armature_modifier.object = armature_object

        # bone colors
        bpy.ops.object.mode_set(mode='POSE')
        leaf_group = armature_object.pose.bone_groups.new(name='Leaf Bones')
        leaf_group.color_set = 'THEME03'

        unused_group = armature_object.pose.bone_groups.new(name='Weightless Bones')
        unused_group.color_set = 'THEME10'

        for bone in armature_object.pose.bones:
            if len(bone.children) == 0:
                bone.bone_group = leaf_group

            if mesh_object.vertex_groups.get(bone.name) is None:
                bone.bone_group = unused_group

        bpy.ops.object.mode_set(mode='OBJECT')

    # sockets
    if len(data.sockets) > 0:
        # create sockets
        bpy.ops.object.mode_set(mode='EDIT')
        for socket in data.sockets:
            socket_bone = edit_bones.new(socket.name)
            parent_bone = edit_bones.get(socket.parent_name)
            socket_bone.parent = parent_bone
            socket_bone.length = bpy.context.scene.uf_settings.bone_length * bpy.context.scene.uf_settings.scale
            socket_bone.matrix = parent_bone.matrix @ Matrix.Translation(socket.position) @ Quaternion((socket.rotation[3], socket.rotation[0], socket.rotation[1], socket.rotation[2])).to_matrix().to_4x4() # xyzw -> wxyz
        bpy.ops.object.mode_set(mode='POSE')

        # socket colors
        socket_group = armature_object.pose.bone_groups.new(name='Sockets')
        socket_group.color_set = 'THEME05'
        for socket in data.sockets:
            armature_object.pose.bones.get(socket.name).bone_group = socket_group
        bpy.ops.object.mode_set(mode='OBJECT')

    return return_object

def import_ueanim_data(ar: FArchiveReader, name: str):
    data = UEAnim()
    
    data.num_frames = ar.read_int()
    data.frames_per_second = ar.read_float()

    while not ar.eof():
        header_name = ar.read_fstring()
        array_size = ar.read_int()
        byte_size = ar.read_int()

        if header_name == "TRACKS":
            data.tracks = ar.read_array(array_size, lambda ar: Track(ar))
        else:
            ar.skip(byte_size)
    
import bpy
from mathutils import Vector


def bytes_to_str(in_bytes: bytes) -> str:
    return in_bytes.rstrip(b"\x00").decode()


def make_axis_vector(vec_in: Vector) -> Vector:
    vec_out = Vector()
    x, y, z = vec_in

    if abs(x) > abs(y):
        if abs(x) > abs(z):
            vec_out.x = 1 if x >= 0 else -1
        else:
            vec_out.z = 1 if z >= 0 else -1
    elif abs(y) > abs(z):
        vec_out.y = 1 if y >= 0 else -1
    else:
        vec_out.z = 1 if z >= 0 else -1

    return vec_out


def get_case_insensitive(source, string: str):
    for item in source:
        if item.name.lower() == string.lower():
            return item


def get_active_armature():
    obj = bpy.context.object

    if obj is None:
        return None

    if obj.type == "ARMATURE":
        return obj
    elif obj.type == "MESH":
        for modifier in obj.modifiers:  # type: ignore
            if modifier.type == "ARMATURE":
                return modifier.object

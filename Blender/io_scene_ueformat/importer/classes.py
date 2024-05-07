import io
import struct
from collections.abc import Callable
from dataclasses import dataclass, field
from enum import IntEnum, auto
from types import TracebackType
from typing import BinaryIO, TypeVar

import numpy as np
from mathutils import Quaternion, Vector

from io_scene_ueformat.importer.utils import bytes_to_str
from io_scene_ueformat.logging import Log

R = TypeVar("R")


class FArchiveReader:
    def __init__(self, data: bytes) -> None:
        self.data: BinaryIO = io.BytesIO(data)
        self.size = len(data)
        self.data.seek(0)

    def __enter__(self) -> "FArchiveReader":
        self.data.seek(0)
        return self

    def __exit__(
        self,
        exc_type: type[BaseException] | None,
        exc_value: BaseException | None,
        traceback: TracebackType | None,
    ) -> None:
        self.data.close()

    def eof(self) -> bool:
        return self.data.tell() >= self.size

    def read(self, size: int) -> bytes:
        return self.data.read(size)

    def read_to_end(self) -> bytes:
        return self.data.read(self.size - self.data.tell())

    def read_bool(self) -> bool:
        return struct.unpack("?", self.data.read(1))[0]

    def read_string(self, size: int) -> str:
        string = self.data.read(size)
        return bytes_to_str(string)

    def read_fstring(self) -> str:
        (size,) = struct.unpack("i", self.data.read(4))
        string = self.data.read(size)
        return bytes_to_str(string)

    def read_int(self) -> int:
        return struct.unpack("i", self.data.read(4))[0]

    def read_int_vector(self, size: int) -> tuple[int, ...]:
        return struct.unpack(str(size) + "I", self.data.read(size * 4))

    def read_short(self) -> int:
        return struct.unpack("h", self.data.read(2))[0]

    def read_byte(self) -> bytes:
        return struct.unpack("c", self.data.read(1))[0]

    def read_float(self) -> float:
        return struct.unpack("f", self.data.read(4))[0]

    def read_float_vector(self, size: int) -> tuple[float, ...]:
        return struct.unpack(str(size) + "f", self.data.read(size * 4))

    def read_byte_vector(self, size: int) -> tuple[int, ...]:
        return struct.unpack(str(size) + "B", self.data.read(size))

    def skip(self, size: int) -> None:
        self.data.seek(size, 1)

    def read_bulk_array(self, predicate: Callable[["FArchiveReader"], R]) -> list[R]:
        count = self.read_int()
        return self.read_array(count, predicate)

    def read_array(
        self,
        count: int,
        predicate: Callable[["FArchiveReader"], R],
    ) -> list[R]:
        return [predicate(self) for _ in range(count)]

    def chunk(self, size: int) -> "FArchiveReader":
        return FArchiveReader(self.read(size))


MAGIC = "UEFORMAT"
MODEL_IDENTIFIER = "UEMODEL"
ANIM_IDENTIFIER = "UEANIM"


class EUEFormatVersion(IntEnum):
    BeforeCustomVersionWasAdded = 0
    SerializeBinormalSign = 1
    AddMultipleVertexColors = 2
    AddConvexCollisionGeom = 3
    LevelOfDetailFormatRestructure = 4
    SerializeVirtualBones = 5

    VersionPlusOne = auto()
    LatestVersion = VersionPlusOne - 1


@dataclass(slots=True)
class UEModel:
    lods: list = field(default_factory=list)
    collisions: list = field(default_factory=list)
    skeleton: "UEModelSkeleton | None" = None
    # physics = None


class UEModelLOD:
    name = ""
    vertices = []
    indices = []
    normals = []
    tangents = []
    colors = []
    uvs = []
    materials = []
    morphs = []
    weights = []

    def __init__(
        self,
        ar: FArchiveReader | None = None,
        name: str = "",
        scale: float = 1.0,
    ) -> None:
        if ar is None:
            return

        self.name = name
        while not ar.eof():
            header_name = ar.read_fstring()
            array_size = ar.read_int()
            byte_size = ar.read_int()

            pos = ar.data.tell()
            if header_name == "VERTICES":
                flattened = ar.read_float_vector(array_size * 3)
                self.vertices = (np.array(flattened) * scale).reshape(array_size, 3)
            elif header_name == "INDICES":
                self.indices = np.array(
                    ar.read_int_vector(array_size),
                    dtype=np.int32,
                ).reshape(array_size // 3, 3)
            elif header_name == "NORMALS":
                flattened = np.array(
                    ar.read_float_vector(array_size * 4),
                )  # W XYZ # TODO: change to XYZ W
                self.normals = flattened.reshape(-1, 4)[:, 1:]
            elif header_name == "TANGENTS":
                ar.skip(array_size * 3 * 3)
            elif header_name == "VERTEXCOLORS":
                self.colors = []
                for i in range(array_size):
                    self.colors.append(VertexColor.read(ar))
            elif header_name == "TEXCOORDS":
                self.uvs = []
                for i in range(array_size):
                    count = ar.read_int()
                    self.uvs.append(
                        np.array(ar.read_float_vector(count * 2)).reshape(count, 2),
                    )
            elif header_name == "MATERIALS":
                self.materials = ar.read_array(array_size, lambda ar: Material(ar))
            elif header_name == "WEIGHTS":
                self.weights = ar.read_array(array_size, lambda ar: Weight(ar))
            elif header_name == "MORPHTARGETS":
                self.morphs = ar.read_array(
                    array_size,
                    lambda ar: MorphTarget(ar, scale),
                )
            else:
                Log.warn(f"Unknown Mesh Data: {header_name}")
                ar.skip(byte_size)
            ar.data.seek(pos + byte_size, 0)


class UEModelSkeleton:
    bones = []
    sockets = []
    virtual_bones = []

    def __init__(self, ar: FArchiveReader = None, scale=1.0):
        if ar is None:
            return

        while not ar.eof():
            header_name = ar.read_fstring()
            array_size = ar.read_int()
            byte_size = ar.read_int()

            pos = ar.data.tell()
            if header_name == "BONES":
                self.bones = ar.read_array(array_size, lambda ar: Bone(ar, scale))
            elif header_name == "SOCKETS":
                self.sockets = ar.read_array(array_size, lambda ar: Socket(ar, scale))
            elif header_name == "VIRTUALBONES":
                self.virtual_bones = ar.read_array(
                    array_size,
                    lambda ar: VirtualBone(ar),
                )
            else:
                Log.warn(f"Unknown Skeleton Data: {header_name}")
                ar.skip(byte_size)
            ar.data.seek(pos + byte_size, 0)


"""class UEModelPhysics:
    bodies = []

    def __init__(self, ar: FArchiveReader, scale):

        while not ar.eof():
            header_name = ar.read_fstring()
            array_size = ar.read_int()
            byte_size = ar.read_int()

            pos = ar.data.tell()
            if header_name == "BODIES":
                self.bodies = ar.read_array(array_size, lambda ar: BodySetup(ar, scale))
            else:
                Log.warn(f"Unknown Skeleton Data: {header_name}")
                ar.skip(byte_size)
            ar.data.seek(pos + byte_size, 0)

class EPhysicsType(IntEnum):
    PhysType_Default = 0
    PhysType_Kinematic = 1
    PhysType_Simulated = 2


class BodySetup:
    bone_name = ""
    physics_type = EPhysicsType.PhysType_Default
    
    sphere_elems = []
    box_elems = []
    capsule_elems = []
    tapered_capsule_elems = []
    convex_elems = []
    
    def __init__(self, ar: FArchiveReader, scale):
        self.bone_name = ar.read_fstring()
        self.physics_type = EPhysicsType(int.from_bytes(ar.read_byte(), byteorder="big"))
        
        self.sphere_elems = ar.read_bulk_array(lambda ar: SphereCollision(ar, scale))
        self.box_elems = ar.read_bulk_array(lambda ar: BoxCollision(ar, scale))
        self.capsule_elems = ar.read_bulk_array(lambda ar: CapsuleCollision(ar, scale))
        self.tapered_capsule_elems = ar.read_bulk_array(lambda ar: TaperedCapsuleCollision(ar, scale))
        self.convex_elems = ar.read_bulk_array(lambda ar: ConvexCollision(ar, scale))
        
class SphereCollision:
    name = ""
    center = []
    radius = 0

    def __init__(self, ar: FArchiveReader, scale):
        self.name = ar.read_fstring()
        self.center = [pos * scale for pos in ar.read_float_vector(3)]
        self.radius = ar.read_float()

class BoxCollision:
    name = ""
    center = []
    rotation = []
    x = 0
    y = 0
    z = 0

    def __init__(self, ar: FArchiveReader, scale):
        self.name = ar.read_fstring()
        self.center = [pos * scale for pos in ar.read_float_vector(3)]
        self.rotation = ar.read_float_vector(3)
        self.x = ar.read_float()
        self.y = ar.read_float()
        self.z = ar.read_float()

class CapsuleCollision:
    name = ""
    center = []
    rotation = []
    radius = 0
    length = 0

    def __init__(self, ar: FArchiveReader, scale):
        self.name = ar.read_fstring()
        self.center = [pos * scale for pos in ar.read_float_vector(3)]
        self.rotation = ar.read_float_vector(3)
        self.radius = ar.read_float()
        self.length = ar.read_float()

class TaperedCapsuleCollision:
    name = ""
    center = []
    rotation = []
    radius0 = 0
    radius1 = 0
    length = 0

    def __init__(self, ar: FArchiveReader, scale):
        self.name = ar.read_fstring()
        self.center = [pos * scale for pos in ar.read_float_vector(3)]
        self.rotation = ar.read_float_vector(3)
        self.radius0 = ar.read_float()
        self.radius1 = ar.read_float()
        self.length = ar.read_float()"""


class ConvexCollision:
    name = ""
    vertices = []
    indices = []

    def __init__(self, ar: FArchiveReader, scale):
        self.name = ar.read_fstring()

        vertices_count = ar.read_int()
        vertices_flattened = ar.read_float_vector(vertices_count * 3)
        self.vertices = (np.array(vertices_flattened) * scale).reshape(
            vertices_count,
            3,
        )

        indices_count = ar.read_int()
        self.indices = np.array(
            ar.read_int_vector(indices_count),
            dtype=np.int32,
        ).reshape(indices_count // 3, 3)


class VertexColor:
    name = ""
    data = []

    def __init__(self, name, data):
        self.name = name
        self.data = data

    @classmethod
    def read(cls, ar: FArchiveReader):
        name = ar.read_fstring()
        count = ar.read_int()
        data = (
            np.array(ar.read_byte_vector(count * 4)).reshape(count, 4) / 255
        ).astype(np.float32)

        return cls(name, data)


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

    def __init__(self, ar: FArchiveReader, scale):
        self.name = ar.read_fstring()
        self.parent_index = ar.read_int()
        self.position = [pos * scale for pos in ar.read_float_vector(3)]
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

    def __init__(self, ar: FArchiveReader, scale: float):
        self.name = ar.read_fstring()

        self.deltas = ar.read_bulk_array(lambda ar: MorphTargetData(ar, scale))


class MorphTargetData:
    position = []
    normals = []
    vertex_index = -1

    def __init__(self, ar: FArchiveReader, scale: float):
        self.position = [pos * scale for pos in ar.read_float_vector(3)]
        self.normals = ar.read_float_vector(3)
        self.vertex_index = ar.read_int()


class Socket:
    name = ""
    parent_name = ""
    position = []
    rotation = []
    scale = []

    def __init__(self, ar: FArchiveReader, scale):
        self.name = ar.read_fstring()
        self.parent_name = ar.read_fstring()
        self.position = [pos * scale for pos in ar.read_float_vector(3)]
        self.rotation = ar.read_float_vector(4)
        self.scale = ar.read_float_vector(3)


class VirtualBone:
    source_name = ""
    target_name = ""
    virtual_name = ""

    def __init__(self, ar: FArchiveReader):
        self.source_name = ar.read_fstring()
        self.target_name = ar.read_fstring()
        self.virtual_name = ar.read_fstring()


class UEAnim:
    num_frames = 0
    frames_per_second = 0
    tracks = []
    curves = []


class Curve:
    name = ""
    keys = []

    def __init__(self, ar: FArchiveReader):
        self.name = ar.read_fstring()
        self.keys = ar.read_bulk_array(lambda ar: FloatKey(ar))


class Track:
    name = ""
    position_keys = []
    rotation_keys = []
    scale_keys = []

    def __init__(self, ar: FArchiveReader, scale):
        self.name = ar.read_fstring()
        self.position_keys = ar.read_bulk_array(lambda ar: VectorKey(ar, scale))
        self.rotation_keys = ar.read_bulk_array(lambda ar: QuatKey(ar))
        self.scale_keys = ar.read_bulk_array(lambda ar: VectorKey(ar))


class AnimKey:
    frame = -1

    def __init__(self, ar: FArchiveReader):
        self.frame = ar.read_int()


class VectorKey(AnimKey):
    value = []

    def __init__(self, ar: FArchiveReader, multiplier=1):
        super().__init__(ar)
        self.value = [float * multiplier for float in ar.read_float_vector(3)]

    def get_vector(self) -> Vector:
        return Vector(self.value)


class QuatKey(AnimKey):
    value = []

    def __init__(self, ar: FArchiveReader):
        super().__init__(ar)
        self.value = ar.read_float_vector(4)

    def get_quat(self) -> Quaternion:
        return Quaternion((self.value[3], self.value[0], self.value[1], self.value[2]))


class FloatKey(AnimKey):
    value = 0.0

    def __init__(self, ar: FArchiveReader):
        super().__init__(ar)
        self.value = ar.read_float()

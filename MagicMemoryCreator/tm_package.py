import struct
from pathlib import Path


TM_FILES = {
    b"/TM/tmctrl.prx": Path("TM/tmctrl.prx"),
    b"/TM/DCARK/reboot.prx": Path("TM/DCARK/reboot.prx"),
    b"/TM/DCARK/tmhlpr.prx": Path("TM/DCARK/tmhlpr.prx"),
}


def extract_tm_package(package):
    files = {}
    with open(package, "rb") as source:
        source.seek(0, 2)
        total = source.tell()
        source.seek(0)
        header = source.read(4)
        if len(header) != 4:
            raise ValueError("Invalid TM.PKG header")
        count, = struct.unpack("<I", header)
        if count > (total - 4) // 5:
            raise ValueError("Invalid TM.PKG file count")
        for _ in range(count):
            header = source.read(5)
            if len(header) != 5:
                raise ValueError("Truncated TM.PKG entry")
            size, length = struct.unpack("<IB", header)
            name = source.read(length)
            if not length or len(name) != length or b"\0" in name:
                raise ValueError("Invalid TM.PKG filename")
            if size > total - source.tell():
                raise ValueError("Truncated TM.PKG file")
            if name in TM_FILES:
                if not size or name in files:
                    raise ValueError("Invalid or duplicate DC module in TM.PKG")
                files[name] = source.read(size)
                if len(files[name]) != size:
                    raise ValueError("Truncated TM.PKG file")
            else:
                source.seek(size, 1)
    if files.keys() != TM_FILES.keys():
        raise ValueError("TM.PKG is missing required DC modules")
    for name, data in files.items():
        destination = TM_FILES[name]
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_bytes(data)

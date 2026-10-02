#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import sys
import struct
import unittest

import npcolony
import npcolony_py.xp


class XpTest(unittest.TestCase):

    def _module(
        self,
        libraries=(b"KERNEL32.dll",),
        machine=0x14C,
        magic=0x10B,
        os_version=(5, 0),
        subsystem_version=(5, 0),
        address=0x1000,
    ):
        names = b""
        descriptors = b""
        names_address = 0x1000 + 20 * (len(libraries) + 1)
        for library in libraries:
            descriptors += struct.pack("<12xI4x", names_address + len(names))
            names += library + b"\0"
        section = descriptors + struct.pack("<20x") + names
        data = struct.pack(
            "<2s58xI4sHHIIIHH", b"MZ", 64, b"PE\0\0", machine, 1, 0, 0, 0, 224, 0x2102
        )
        data += struct.pack(
            "<H38xHH4xHH40xI8xII112x",
            magic,
            os_version[0],
            os_version[1],
            subsystem_version[0],
            subsystem_version[1],
            16,
            address,
            len(section),
        )
        data += struct.pack(
            "<8sIIII16x", b".idata", len(section), 0x1000, len(section), len(data) + 40
        )
        return data + section

    def test_check_module(self):
        libraries = npcolony_py.xp.check_module(
            self._module(libraries=(b"KERNEL32.dll", b"WINSPOOL.DRV", b"python27.dll"))
        )
        self.assertEqual(libraries, ["KERNEL32.dll", "WINSPOOL.DRV", "python27.dll"])

        libraries = npcolony_py.xp.check_module(self._module(libraries=()))
        self.assertEqual(libraries, [])

        libraries = npcolony_py.xp.check_module(self._module(address=0))
        self.assertEqual(libraries, [])

    def test_check_module_invalid(self):
        data = self._module()
        for invalid in (
            b"",
            b"MZ",
            b"ZM" + data[2:],
            data[:64] + b"NE\0\0" + data[68:],
            data[:60] + struct.pack("<I", 0xFFFFFFF0) + data[64:],
            data[:100],
            data[:350],
            data[:-10],
            data[:-1],
            self._module(address=0x2000),
            self._module(address=0x0FFF),
        ):
            self.assertRaises(ValueError, lambda: npcolony_py.xp.check_module(invalid))

    def test_check_module_machine(self):
        for machine, magic in (
            (0x8664, 0x20B),
            (0xAA64, 0x20B),
            (0x1C4, 0x10B),
            (0x14C, 0x20B),
        ):
            self.assertRaises(
                ValueError,
                lambda: npcolony_py.xp.check_module(
                    self._module(machine=machine, magic=magic)
                ),
            )

    def test_check_module_versions(self):
        for os_version, subsystem_version in (
            ((4, 0), (4, 0)),
            ((5, 0), (5, 1)),
            ((5, 1), (5, 0)),
            ((5, 1), (5, 1)),
        ):
            libraries = npcolony_py.xp.check_module(
                self._module(os_version=os_version, subsystem_version=subsystem_version)
            )
            self.assertEqual(libraries, ["KERNEL32.dll"])

        for os_version, subsystem_version in (
            ((5, 2), (5, 0)),
            ((5, 0), (5, 2)),
            ((6, 0), (6, 0)),
            ((10, 0), (5, 1)),
        ):
            self.assertRaises(
                ValueError,
                lambda: npcolony_py.xp.check_module(
                    self._module(
                        os_version=os_version, subsystem_version=subsystem_version
                    )
                ),
            )

    def test_check_module_libraries(self):
        libraries = npcolony_py.xp.check_module(
            self._module(libraries=(b"kernel32.DLL", b"MSVCR100.dll", b"Python34.dll"))
        )
        self.assertEqual(libraries, ["kernel32.DLL", "MSVCR100.dll", "Python34.dll"])

        for library in (
            b"VCRUNTIME140.dll",
            b"api-ms-win-crt-runtime-l1-1-0.dll",
            b"python38.dll",
            b"bcrypt.dll",
            b"kernel32",
            b"k\xe9rnel32.dll",
            b"",
        ):
            self.assertRaises(
                ValueError,
                lambda: npcolony_py.xp.check_module(
                    self._module(libraries=(b"KERNEL32.dll", library))
                ),
            )

    @unittest.skipIf(os.name != "nt", "windows xp modules are only built on windows")
    def test_check_module_npcolony(self):
        if struct.calcsize("P") == 8 or sys.version_info >= (3, 5):
            if os.environ.get("NPCOLONY_TEST_XP", None):
                self.fail("the npcolony module is not built for windows xp")
            self.skipTest("requires a 32 bit python 2.7 or 3.4")
        with open(npcolony.__file__, "rb") as file:
            libraries = npcolony_py.xp.check_module(file.read())
        self.assertEqual(
            "python%d%d.dll" % sys.version_info[:2]
            in [library.lower() for library in libraries],
            True,
        )

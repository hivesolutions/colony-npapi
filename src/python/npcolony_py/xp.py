#!/usr/bin/python
# -*- coding: utf-8 -*-

# Hive Colony Framework
# Copyright (c) 2008-2020 Hive Solutions Lda.
#
# This file is part of Hive Colony Framework
#
# Hive Colony Framework is free software: you can redistribute it and/or modify
# it under the terms of the Apache License as published by the Apache
# Foundation, either version 2.0 of the License, or (at your option) any
# later version.
#
# Hive Colony Framework is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# Apache License for more details.
#
# You should have received a copy of the Apache License along with
# Hive Colony Framework If not, see <http://www.apache.org/licenses/>.

__author__ = "João Magalhães <joamag@hive.pt>"
""" The author(s) of the module """

__copyright__ = "Copyright (c) 2008-2020 Hive Solutions Lda."
""" The copyright for the module """

__license__ = "Apache License, Version 2.0"
""" The license for the module """

import struct

XP_VERSION = (5, 1)
""" The version of windows xp, the most recent version of
windows (operative system and subsystem) that a module is
allowed to require in order to be loaded by windows xp """

XP_MACHINE = 0x14C
""" The machine (architecture) of the modules that are
loaded by windows xp, the 32 bit one (x86) """

XP_LIBRARIES = (
    "kernel32.dll",
    "user32.dll",
    "gdi32.dll",
    "winspool.drv",
    "comdlg32.dll",
    "msvcr90.dll",
    "msvcr100.dll",
    "python27.dll",
    "python34.dll",
)
""" The libraries that a module is allowed to import, the ones
of windows xp that are used by the module and the ones of the
versions of python (and of their runtimes) that run on it """


def check_module(data):
    """
    Verifies that the provided windows module (eg: the pyd file
    of the npcolony module) is able to be loaded by windows xp,
    meaning that it's a 32 bit module that requires no version of
    windows more recent than windows xp and that imports only the
    libraries available in it, raising an exception otherwise.

    :type data: String
    :param data: The contents of the windows module (portable
    executable) that is going to be verified.
    :rtype: List
    :return: The names of the libraries imported by the module,
    in the order in which they are imported.
    """

    # verifies that the data starts with the signature of the dos header,
    # the first header of every windows module, as it's the one that
    # contains the offset of the (portable executable) header of the module
    if not data[:2] == b"MZ":
        raise ValueError("Invalid module, it must be a windows module")

    # reads and verifies the values of the headers of the module, in case
    # any of them is not contained in the data (eg: truncated module) or
    # is not valid (eg: name of a library that is not ascii) the module is
    # considered invalid, as it can't be loaded
    try:
        # retrieves the offset of the header of the module and verifies
        # its signature, making sure that it's a portable executable
        offset = struct.unpack_from("<I", data, 0x3C)[0]
        if not data[offset : offset + 4] == b"PE\0\0":
            raise ValueError("Invalid module, it must be a windows module")

        # verifies that the module is a 32 bit (x86) one, the only one
        # loaded by windows xp, according to both the machine of the module
        # and the format of its optional header (the 32 bit one)
        machine, sections_count = struct.unpack_from("<HH", data, offset + 4)
        optional_size = struct.unpack_from("<H", data, offset + 20)[0]
        optional = offset + 24
        magic = struct.unpack_from("<H", data, optional)[0]
        if not machine == XP_MACHINE or not magic == 0x10B:
            raise ValueError("Invalid module, it must be a 32 bit module")

        # verifies that neither the version of the operative system nor
        # the version of the subsystem required by the module is more
        # recent than windows xp, as otherwise it's not loaded
        for version_offset in (40, 48):
            version = struct.unpack_from("<HH", data, optional + version_offset)
            if version > XP_VERSION:
                raise ValueError("Invalid module, it requires windows %d.%d" % version)

        # retrieves the address, the size and the offset (in the data) of
        # each of the sections of the module, so that the addresses of the
        # imports (relative to the loaded module) can be converted into
        # offsets in the data, using the section that contains them
        sections = [
            struct.unpack_from("<III", data, optional + optional_size + index * 40 + 12)
            for index in range(sections_count)
        ]

        def _offset(address):
            for section_address, section_size, section_offset in sections:
                if section_address <= address < section_address + section_size:
                    return address - section_address + section_offset
            raise ValueError("Invalid module, it must be a windows module")

        # iterates over the descriptors of the imports of the module (in
        # case there's any) until the empty one that ends them is found,
        # reading the (null terminated) name of each imported library and
        # verifying that it's one of the libraries available in windows xp
        libraries = []
        address = struct.unpack_from("<I", data, optional + 104)[0]
        while address:
            name = struct.unpack_from("<I", data, _offset(address) + 12)[0]
            if not name:
                break
            start = _offset(name)
            end = data.find(b"\0", start)
            if end == -1:
                raise ValueError("Invalid module, it must be a windows module")
            library = data[start:end].decode("ascii")
            if not library.lower() in XP_LIBRARIES:
                raise ValueError("Invalid module, it imports '%s'" % library)
            libraries.append(library)
            address += 20
    except (struct.error, UnicodeDecodeError):
        raise ValueError("Invalid module, it must be a windows module")

    # returns the names of the libraries imported by the module, as
    # the module is able to be loaded by windows xp
    return libraries

#!/usr/bin/python
# -*- coding: utf-8 -*-

# Hive Colony Framework
# Copyright (c) 2008-2016 Hive Solutions Lda.
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

__version__ = "1.0.0"
""" The version of the module """

__revision__ = "$LastChangedRevision$"
""" The revision number of the module """

__date__ = "$LastChangedDate$"
""" The last change date of the module """

__copyright__ = "Copyright (c) 2008-2016 Hive Solutions Lda."
""" The copyright for the module """

__license__ = "Apache License, Version 2.0"
""" The license for the module """

import os
import setuptools

module = setuptools.Extension(
    "npcolony",
    define_macros = [
        ("MAJOR_VERSION", "1"),
        ('MINOR_VERSION', '0')
    ],
    include_dirs = [
        ".",
        "src/colony_npapi/",
        "/usr/local/include"
    ],
    libraries = ["cups"],
    library_dirs = ["/usr/local/lib"],
    extra_compile_args = [] if os.name in ("nt",) else [
        "-std=c99",
        "-pedantic",
        "-finline-functions",
        "-Wall",
        "-Wno-long-long",
        "-Wno-variadic-macros",
        "-Wno-strict-aliasing",
        "-Wno-strict-prototypes",
        "-DNO_CONFIG_H",
        "-DCOLONY_PLATFORM_UNIX"
    ],
    sources = [
        "src/colony_npapi/stdafx.c",
        "src/colony_npapi/encoding/base_64.c",
        "src/colony_npapi/plugin/base.c",
        "src/colony_npapi/plugin/python.c",
        "src/colony_npapi/plugin/util.c",
        "src/colony_npapi/print/print_unix.c",
        "src/colony_npapi/print/print_win32.c",
        "src/colony_npapi/system/gui_unix.c",
        "src/colony_npapi/system/gui_win32.c"
    ]
)

setuptools.setup(
    name = "npcolony",
    version = "0.1.0",
    author = "Hive Solutions Lda.",
    author_email = "development@hive.pt",
    description = "Colony Framework",
    license = "Apache License, Version 2.0",
    keywords = "colony npapi native",
    url = "http://colony_npapi.hive.pt",
    zip_safe = False,
    ext_modules = [module],
    classifiers = [
        "Development Status :: 3 - Alpha",
        "Topic :: Utilities",
        "License :: OSI Approved :: Apache Software License",
        "Operating System :: OS Independent",
        "Programming Language :: Python",
        "Programming Language :: Python :: 2.6",
        "Programming Language :: Python :: 2.7",
        "Programming Language :: Python :: 3.0",
        "Programming Language :: Python :: 3.1",
        "Programming Language :: Python :: 3.2",
        "Programming Language :: Python :: 3.3",
        "Programming Language :: Python :: 3.4"
    ]
)

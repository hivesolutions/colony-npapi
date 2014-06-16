# Hive Colony Framework
# Copyright (C) 2008-2014 Hive Solutions Lda.
#
# This file is part of Hive Colony Framework.
#
# Hive Colony Framework is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Hive Colony Framework is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Hive Colony Framework. If not, see <http://www.gnu.org/licenses/>.

# __author__    = João Magalhães <joamag@hive.pt>
# __version__   = 1.0.0
# __revision__  = $LastChangedRevision$
# __date__      = $LastChangedDate$
# __copyright__ = Copyright (c) 2008-2014 Hive Solutions Lda.
# __license__   = GNU General Public License (GPL), Version 3

include $(top_srcdir)/Common.am

lib_LTLIBRARIES = libnpcolony.la

libnpcolony_la_CFLAGS = $(AM_CFLAGS)
libnpcolony_la_CXXFLAGS = $(AM_CXXFLAGS)

libnpcolony_la_SOURCES = colony_npapi/stdafx.c \
colony_npapi/encoding/base_64.c \
colony_npapi/plugin/base.c \
colony_npapi/plugin/util.c \
colony_npapi/print/print_unix.c \
colony_npapi/print/print_win32.c \
colony_npapi/system/gui_unix.c \
colony_npapi/system/gui_win32.c \
colony_npapi_python/main.c

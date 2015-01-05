# Hive Colony Framework
# Copyright (C) 2008-2015 Hive Solutions Lda.
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
# __copyright__ = Copyright (c) 2008-2015 Hive Solutions Lda.
# __license__   = GNU General Public License (GPL), Version 3

include $(top_srcdir)/Common.am

lib_LTLIBRARIES = libnpcolony.la

libnpcolony_la_CFLAGS = $(AM_CFLAGS)
libnpcolony_la_CXXFLAGS = $(AM_CXXFLAGS)

libnpcolony_la_SOURCES = stdafx.c \
encoding/base_64.c \
plugin/base.c \
plugin/python.c \
plugin/util.c \
print/print_unix.c \
print/print_win32.c \
system/gui_unix.c \
system/gui_win32.c

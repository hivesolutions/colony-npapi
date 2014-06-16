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

# defines the version values
m4_define([colony_npapi_major_version], [1])
m4_define([colony_npapi_minor_version], [0])
m4_define([colony_npapi_micro_version], [0])
m4_define([colony_npapi_version], m4_format(%s.%s.%s, colony_npapi_major_version, colony_npapi_minor_version, colony_npapi_micro_version))
m4_define([colony_npapi_makefiles], [Makefile m4/Makefile src/Makefile])

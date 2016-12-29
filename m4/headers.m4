# Hive Colony Framework
# Copyright (c) 2008-2017 Hive Solutions Lda.
#
# This file is part of Hive Colony Framework.
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
# Hive Colony Framework. If not, see <http://www.apache.org/licenses/>.

# __author__    = João Magalhães <joamag@hive.pt>
# __version__   = 1.0.0
# __revision__  = $LastChangedRevision$
# __date__      = $LastChangedDate$
# __copyright__ = Copyright (c) 2008-2017 Hive Solutions Lda.
# __license__   = Apache License, Version 2.0

# checks for header files
AC_CHECK_HEADERS([stdlib.h assert.h])

# checks for specific header files
AC_CHECK_HEADERS([cups/cups.h], [], [AC_MSG_ERROR([cups headers are required])])
AC_CHECK_HEADERS([python2.6/Python.h], [have_python=true], [have_python=false])
AC_CHECK_HEADERS([python2.7/Python.h], [have_python=true], [have_python=false])

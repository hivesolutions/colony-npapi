# Hive Colony Framework
# Copyright (c) 2008-2018 Hive Solutions Lda.
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
# __copyright__ = Copyright (c) 2008-2018 Hive Solutions Lda.
# __license__   = Apache License, Version 2.0

# sets the default values for the various library
# control variables
have_cups=true
have_python=true
have_python_26=true
have_python_27=true

# checks for libraries
AC_CHECK_LIB([cups], [main], [], [AC_MSG_ERROR([cups library is required])])
AC_CHECK_LIB([python2.6], [main], [], [have_python_26=false])
AC_CHECK_LIB([python2.7], [main], [], [have_python_27=false])

if test "$have_python_26" = true || test "$have_python_27" = true; then
    have_python=true
    AC_DEFINE(HAVE_LIBPYTHON, true, [Define to 1 if python lib is present])
else
    have_python=false
fi

# library variables activation
AM_CONDITIONAL(LINK_CUPS, [test "$have_cups" != "false"])

/*
 Hive Colony Framework
 Copyright (C) 2008 Hive Solutions Lda.

 This file is part of Hive Colony Framework.

 Hive Colony Framework is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Hive Colony Framework is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Hive Colony Framework. If not, see <http://www.gnu.org/licenses/>.

 __author__    = João Magalhães <joamag@hive.pt>
 __version__   = 1.0.0
 __revision__  = $LastChangedRevision$
 __date__      = $LastChangedDate$
 __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
 __license__   = GNU General Public License (GPL), Version 3
*/

#include "stdafx.h"

#include "util.h"

void logmsg(const char *msg) {
    /* allocates space for a file reference */
    FILE *file;

    /* opens the logging file for wirting (appending)
    this is the file to be used for logging  */
    fopen_s(&file, "\\npcolony.log", "a");

    /* in case the file was not correclty
    opened, avoid any operation */
    if(file == NULL) {
        /* returns immediately */
        return;
    }

    /* prints the message into file (printing)
    and then closes it immediately */
    fputs(msg, file);
    fclose(file);
}

/*
 Hive Colony Framework
 Copyright (C) 2008-2012 Hive Solutions Lda.

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
 __copyright__ = Copyright (c) 2008-2012 Hive Solutions Lda.
 __license__   = GNU General Public License (GPL), Version 3
*/

#pragma once

#ifdef COLONY_PLATFORM_UNIX

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Prints a document using the default printer or in case the show
 * dialog option is set using the printer selected in the printing
 * dialog.
 *
 * The provided data buffer must be encoded using the binie binary
 * file specification, that specifies a series of basic printing
 * primitive routines.
 *
 * @param show_dialog If the printing dialog should be display for
 * selection of the printer.
 * @param data The buffer of data encoded in binie format describing
 * the document to be printed.
 * @return The result of the printing processm if success a value
 * greater than zero should be returned.
 */
COLONY_EXPORT_PREFIX int print(bool show_dialog, char *data);

#ifdef __cplusplus
}
#endif

#endif

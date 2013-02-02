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

#include "stdafx.h"

#ifdef COLONY_PLATFORM_UNIX

#include "print_unix.h"

#ifdef __cplusplus
extern "C" {
#endif

const char *pformat() {
    return NPCOLONY_PDF;
}

void pdevices(struct device_t **devices_p, size_t *devices_c) {
    /* allocates space for the various values to be used
    in the listing of the devices of the current system */
    size_t index;
    struct device_t *device;
    struct device_t *devices;
    cups_dest_t *dest = NULL;
    cups_dest_t *dests = NULL;
    int num_dests = cupsGetDests(&dests);

    /* allocates the buffer to hold the various printing devices
    existing in the current system (to be returned) */
    devices = (struct device_t *) malloc(sizeof(struct device_t) * num_dests);

    /* iterates over the complete set of destinies to create
    the associated device structure and populate it with the
    values that describe the device */
    for(index = 0; index < num_dests; index++) {
        device = &devices[index];
        dest = &dests[index];
        memcpy(device->name, dest->name, strlen(dest->name));
        device->name_s = strlen(dest->name);
        device->is_default = (char) dest->is_default;
    }

    /* releases the memory used for the listing
    of the various destinations */
    cupsFreeDests(num_dests, dests);

    /* updates the devices pointer and the number of devices
    that have been created (output variables) */
    *devices_p = devices;
    *devices_c = num_dests;
}

int print(bool show_dialog, char *data, size_t size) {
    /* allocates space for the various variables that
    are going to be used for the print operation and
    then retrieves the various available destinies */
    size_t index;
    char file_path[NPCOLONY_PATH_SIZE];
    int num_options = 0;
    cups_dest_t *dest = NULL;
    cups_dest_t *dests = NULL;
    cups_option_t *options = NULL;
    int num_dests = cupsGetDests(&dests);

    /* iterates over the complete set of destinies to try
    to find the one that is considered the default one */
    for(index = 0; index < num_dests; index++) {
        dest = &dests[index];
        if(dest->is_default == 0) { continue; }
        break;
    }

    /* copies the base (file) template to the file path and
    uses it it to create the final path to the temporary path
    then verifies it has been correctly opened */
    strncpy(file_path, NPCOLONY_TEMPLATE, strlen(NPCOLONY_TEMPLATE) + 1);
    int fd = mkstemp(file_path);
    if(fd < 0) { return -1; }

    /* writes the read contents from the pdf into the created
    temporary file and in case the result of the write operation
    is not the expected returns in error */
    size_t result = write(fd, data, size);
    if(result == -1) { return -1; }

    /* creates the buffer that will contain the various
    files that are meant to be printed */
    char *files[1] = {
        file_path
    };

    /* sends the print job to the target printer and received
    the associated job identifier to be used */
    cupsPrintFiles(
        dest->name,
        1,
        (const char **) files,
        "Colony Gateway",
        num_options,
        options
    );

    /* releases the memory used for the listing
    of the various destinations */
    cupsFreeDests(num_dests, dests);

    /* unlinks the created temporary file so that
    it's abel to be removed from the file system */
    unlink(file_path);

    /* returns with no error */
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif

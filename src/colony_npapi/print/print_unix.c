/*
 Hive Colony Framework
 Copyright (c) 2008-2020 Hive Solutions Lda.

 This file is part of Hive Colony Framework.

 Hive Colony Framework is free software: you can redistribute it and/or modify
 it under the terms of the Apache License as published by the Apache
 Foundation, either version 2.0 of the License, or (at your option) any
 later version.

 Hive Colony Framework is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 Apache License for more details.

 You should have received a copy of the Apache License along with
 Hive Colony Framework. If not, see <http://www.apache.org/licenses/>.

 __author__    = João Magalhães <joamag@hive.pt>
 __copyright__ = Copyright (c) 2008-2020 Hive Solutions Lda.
 __license__   = Apache License, Version 2.0
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
    const char *ppd_path;
    FILE *ppd_file;
    ppd_file_t *ppd;
    ppd_option_t *page_size_o;
    ppd_size_t *page_size;
    cups_dest_t *dest = NULL;
    cups_dest_t *dests = NULL;
    int num_dests = cupsGetDests(&dests);

    /* allocates the buffer to hold the various printing devices
    existing in the current system (to be returned) */
    devices = (struct device_t *) malloc(sizeof(struct device_t) * num_dests);
    memset(devices, 0, sizeof(struct device_t) * num_dests);

    /* iterates over the complete set of destinies to create
    the associated device structure and populate it with the
    values that describe the device */
    for(index = 0; index < num_dests; index++) {
        /* retrieves the references to the current
        device structure and to the current dest value */
        device = &devices[index];
        dest = &dests[index];

        /* retrieves the ppd file for the current device
        and opens its file, then reads the various values
        required to be read from it */
        ppd_path = cupsGetPPD(dest->name);
        if(ppd_path == NULL) { ppd_file = NULL; }
        else { ppd_file = fopen(ppd_path, "rb"); }
        if(ppd_file == NULL) { ppd = NULL; }
        else { ppd = ppdOpen(ppd_file); }
        if(ppd == NULL) { page_size_o = NULL; }
        else { page_size_o = ppdFindOption(ppd, "PageSize"); }

        /* populates the various device values according to the
        the various definitions of the device */
        memcpy(device->name, dest->name, strlen(dest->name));
        device->name_s = strlen(dest->name);
        device->is_default = (char) dest->is_default;
        if(page_size_o) {
            page_size = ppdPageSize(
                ppd,
                page_size_o->defchoice
            );
            memcpy(
                device->media,
                page_size_o->defchoice,
                strlen(page_size_o->defchoice)
            );
            device->media_s = strlen(page_size_o->defchoice);
        }

        /* in case the default page size is valid populates the
        dimensions of the media and its imageable area (printable
        box), both measured in points from the bottom left corner */
        if(page_size_o && page_size) {
            device->width = page_size->width;
            device->length = page_size->length;
            device->left = page_size->left;
            device->bottom = page_size->bottom;
            device->right = page_size->right;
            device->top = page_size->top;
        }

        /* in case the device accepts custom paper sizes populates their
        range and their (hardware) margins, all measured in points, so
        that it's possible to know if the size of a document is accepted
        by the device, as windows does for its custom paper sizes */
        if(ppd && ppd->variable_sizes) {
            device->custom = 1;
            memcpy(device->custom_min, ppd->custom_min, sizeof(device->custom_min));
            memcpy(device->custom_max, ppd->custom_max, sizeof(device->custom_max));
            memcpy(device->custom_margins, ppd->custom_margins, sizeof(device->custom_margins));
        }

        /* closes the ppd reference object, as it's not going
        to be used anymore (avoids memory leaks) */
        if(ppd != NULL) { ppdClose(ppd); }

        /* closes the temporary PPD file and then unlinks it
        so that it's correctly removed from the current system */
        if(ppd_file != NULL) { fclose(ppd_file); }
        if(ppd_path != NULL) { unlink(ppd_path); }
    }

    /* releases the memory used for the listing
    of the various destinations */
    cupsFreeDests(num_dests, dests);

    /* updates the devices' pointer and the number of devices
    that have been created (output variables) */
    *devices_p = devices;
    *devices_c = num_dests;
}

int print(
    bool show_dialog,
    struct job_t *config,
    char *data,
    size_t size
) {
    return print_printer(show_dialog, NULL, config, data, size);
}

int print_printer(
    bool show_dialog,
    char *printer,
    struct job_t *config,
    char *data,
    size_t size
) {
    /* allocates space for the various variables that
    are going to be used for the print operation */
    size_t index;
    int fd;
    int job_id;
    int num_dests;
    ssize_t result;
    FILE *file;
    char file_path[NPCOLONY_PATH_SIZE];
    const char *title = "Colony Gateway";
    int num_options = 0;
    cups_dest_t *dest = NULL;
    cups_dest_t *dests = NULL;
    cups_option_t *options = NULL;

    /* in case an output path is defined for the job the document
    is written to that path instead of being printed, mirroring
    the print to file behaviour of the windows implementation */
    if(config != NULL && config->output_path != NULL) {
        file = fopen(config->output_path, "wb");
        if(file == NULL) { return -1; }
        result = (ssize_t) fwrite(data, sizeof(char), size, file);
        if(fclose(file) != 0) { return -1; }
        if(result != (ssize_t) size) { return -1; }
        return 0;
    }

    /* retrieves the various available destinies and then tries
    to find the requested printer, in case no printer (or the
    default one) is requested the default destiny is used, or
    the single destiny of the system when none is the default */
    num_dests = cupsGetDests(&dests);
    if(printer == NULL || printer[0] == '\0' || !strcmp(printer, "default")) {
        for(index = 0; index < (size_t) num_dests; index++) {
            if(dests[index].is_default == 0) { continue; }
            dest = &dests[index];
            break;
        }
        if(dest == NULL && num_dests == 1) { dest = &dests[0]; }
    } else {
        dest = cupsGetDest(printer, NULL, num_dests, dests);
    }

    /* in case no destiny was found for the requested printer
    the destinies are released and the operation returns in
    error (avoids the access to an invalid destiny) */
    if(dest == NULL) {
        cupsFreeDests(num_dests, dests);
        return -1;
    }

    /* builds the options of the print job from the configuration
    of the job, so that the title, the media and the scaling of the
    document are explicitly set instead of guessed by the system */
    if(config != NULL) {
        if(config->title != NULL) { title = config->title; }
        if(config->media != NULL) {
            num_options = cupsAddOption("media", config->media, num_options, &options);
        }
        if(config->scaling != NULL) {
            num_options = cupsAddOption("print-scaling", config->scaling, num_options, &options);
        }
    }

    /* copies the base (file) template to the file path and
    uses it to create the final path to the temporary path
    then verifies it has been correctly opened */
    strncpy(file_path, NPCOLONY_TEMPLATE, strlen(NPCOLONY_TEMPLATE) + 1);
    fd = mkstemp(file_path);
    if(fd < 0) {
        cupsFreeOptions(num_options, options);
        cupsFreeDests(num_dests, dests);
        return -1;
    }

    /* writes the read contents from the pdf into the created
    temporary file and closes it, in case the result of the write
    (or close) operation is not the expected returns in error */
    result = write(fd, data, size);
    if(close(fd) != 0) { result = -1; }
    if(result != (ssize_t) size) {
        unlink(file_path);
        cupsFreeOptions(num_options, options);
        cupsFreeDests(num_dests, dests);
        return -1;
    }

    /* creates the buffer that will contain the various
    files that are meant to be printed */
    char *files[1] = {
        file_path
    };

    /* sends the print job to the target printer and received
    the associated job identifier to be used */
    job_id = cupsPrintFiles(
        dest->name,
        1,
        (const char **) files,
        title,
        num_options,
        options
    );

    /* releases the memory used for the options and for
    the listing of the various destinations */
    cupsFreeOptions(num_options, options);
    cupsFreeDests(num_dests, dests);

    /* unlinks the created temporary file so that
    it's able to be removed from the file system */
    unlink(file_path);

    /* returns the identifier of the job in case it has been
    created, otherwise (invalid identifier) returns in error */
    return job_id > 0 ? job_id : -1;
}

#ifdef __cplusplus
}
#endif

#endif

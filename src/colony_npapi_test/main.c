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

#define HELLO_WORLD_B64 "SGVsbG8gV29ybGQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQAAAAEAAABAAQAAAA\
AAAAAAAABDYWxpYnJpAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACQAAAAMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAwAAABIZWxsbyBXb3JsZAA="

void print_devices() {
    /* allocates memory for the various internal structure
    that are going to be used to print device information */
    size_t index;
    size_t devices_s;
    struct device_t *device;
    struct device_t *devices;

    /* retrieves the complete set of available printing
    devices and then iterates over them to print their
    currently set values and settings */
    pdevices(&devices, &devices_s);
    for(index = 0; index < devices_s; index++) {
        device = &devices[index];
        printf("%s\n", device->name);
    }

    /* releases the memory that was allocated for the
    device structures sequence (avoids memory leak) */
    free(devices);
}

void print_hello() {
    /* allocates space for the decoded data buffer and for
    the storage of the length (size) of it */
    char *data;
    size_t data_length;

    /* decodes the data value from the base 64 encoding
    and then uses it to print the data */
    decode_base64(
        (unsigned char *) HELLO_WORLD_B64,
        strlen(HELLO_WORLD_B64),
        (unsigned char **) &data, &data_length
    );
    print(FALSE, NULL, data, data_length);

    /* releases the decoded buffer (avoids memory leak)
    and then returns in success */
    _free_base64((unsigned char *) data);
}

void print_base64(char *file_path) {
    /* allocates space for both the "resolved" data
    buffer and for the buffer that will hold the file
    contents */
    char *data;
    unsigned char *buffer;

    /* allocates space for the value of the size of the
    file and the data buffer */
    size_t file_size;
    size_t data_length;

    /* allocates space for the pointer to the file object
    to be used in the reading */
    FILE *file;

    /* opens the target file for read operations in
    the binary form */
    fopen_s(&file, file_path, "rb");

    /* seeks to the final position in the file and
    checks the current offset (this value should be
    the size of the file in bytes) */
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    /* allocates enough space for the buffer to hold
    the complete set of contents of the file */
    buffer = (unsigned char *) malloc(file_size);

    /* reads the complete set of contents from the file
    and then sends then decodes them as base 64 */
    fread(buffer, sizeof(char), file_size, file);
    decode_base64(buffer, file_size, (unsigned char **) &data, &data_length);
    print(FALSE, NULL, data, data_length);

    /* releases the decoded buffer (avoids memory leak)
    and then returns in success */
    _free_base64((unsigned char *) data);

    /* releases the buffer holding the contents of the just
    read file (avoids leaking of memory) */
    free(buffer);
}

int main(int argc, char *argv[]) {
    print_devices();
    print_hello();
    return 0;
}

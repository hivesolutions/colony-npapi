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

#ifdef COLONY_PYTHON

#include "python.h"

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

static PyObject *get_format(PyObject *self, PyObject *args) {
    return PyUnicode_FromString(pformat());
}

static PyObject *get_devices(PyObject *self, PyObject *args) {
    /* allocates memory for the various internal structure
    that are going to be used to retrieve device information */
    size_t index;
    size_t devices_s;
    PyObject *element;
    PyObject *item;
    PyObject *custom;
    struct device_t *device;
    struct device_t *devices;
    PyObject *result = PyList_New(0);

    /* retrieves the complete set of available printing
    devices and then iterates over them to convert their
    internal structure into dictionaries to be returned, the
    strings are decoded using the encoding of the devices (with
    invalid characters replaced) and each value (and dictionary)
    reference is released once it's owned by its container */
    pdevices(&devices, &devices_s);
    for(index = 0; index < devices_s; index++) {
        device = &devices[index];
        element = PyDict_New();
#if PY_MAJOR_VERSION >= 3
        item = PyUnicode_Decode(
            device->name,
            device->name_s,
            DEVICE_ENCODING,
            "replace"
        );
#else
        item = PyString_Decode(
            device->name,
            device->name_s,
            DEVICE_ENCODING,
            "replace"
        );
#endif
        PyDict_SetItemString(element, "name", item);
        Py_DECREF(item);
        item = PyBool_FromLong((long) device->is_default);
        PyDict_SetItemString(element, "is_default", item);
        Py_DECREF(item);
#if PY_MAJOR_VERSION >= 3
        item = PyUnicode_Decode(
            device->media,
            device->media_s,
            DEVICE_ENCODING,
            "replace"
        );
#else
        item = PyString_Decode(
            device->media,
            device->media_s,
            DEVICE_ENCODING,
            "replace"
        );
#endif
        PyDict_SetItemString(element, "media", item);
        Py_DECREF(item);
        item = PyFloat_FromDouble((double) device->width);
        PyDict_SetItemString(element, "width", item);
        Py_DECREF(item);
        item = PyFloat_FromDouble((double) device->length);
        PyDict_SetItemString(element, "length", item);
        Py_DECREF(item);
        item = PyFloat_FromDouble((double) device->left);
        PyDict_SetItemString(element, "left", item);
        Py_DECREF(item);
        item = PyFloat_FromDouble((double) device->bottom);
        PyDict_SetItemString(element, "bottom", item);
        Py_DECREF(item);
        item = PyFloat_FromDouble((double) device->right);
        PyDict_SetItemString(element, "right", item);
        Py_DECREF(item);
        item = PyFloat_FromDouble((double) device->top);
        PyDict_SetItemString(element, "top", item);
        Py_DECREF(item);

        /* the custom paper sizes accepted by the device are described by
        their range and their margins (in points), the distances to the
        edges of the page and not the coordinates of its imageable box (as
        the ones of the default media), with an invalid value for the
        devices that don't accept custom paper sizes and for every device
        on windows, where they're not reported (the size of the document
        is given to the driver as a custom paper size instead) */
        if(device->custom) {
            custom = PyDict_New();
            item = PyFloat_FromDouble((double) device->custom_min[0]);
            PyDict_SetItemString(custom, "min_width", item);
            Py_DECREF(item);
            item = PyFloat_FromDouble((double) device->custom_min[1]);
            PyDict_SetItemString(custom, "min_length", item);
            Py_DECREF(item);
            item = PyFloat_FromDouble((double) device->custom_max[0]);
            PyDict_SetItemString(custom, "max_width", item);
            Py_DECREF(item);
            item = PyFloat_FromDouble((double) device->custom_max[1]);
            PyDict_SetItemString(custom, "max_length", item);
            Py_DECREF(item);
            item = PyFloat_FromDouble((double) device->custom_margins[0]);
            PyDict_SetItemString(custom, "margin_left", item);
            Py_DECREF(item);
            item = PyFloat_FromDouble((double) device->custom_margins[1]);
            PyDict_SetItemString(custom, "margin_bottom", item);
            Py_DECREF(item);
            item = PyFloat_FromDouble((double) device->custom_margins[2]);
            PyDict_SetItemString(custom, "margin_right", item);
            Py_DECREF(item);
            item = PyFloat_FromDouble((double) device->custom_margins[3]);
            PyDict_SetItemString(custom, "margin_top", item);
            Py_DECREF(item);
        } else {
            custom = Py_None;
            Py_INCREF(custom);
        }
        PyDict_SetItemString(element, "custom", custom);
        Py_DECREF(custom);

        PyList_Append(result, element);
        Py_DECREF(element);
    }

    /* releases the memory that was allocated for the
    device structures sequence (avoids memory leak) */
    free(devices);

    /* returns the list that has been constructed for the
    values that are going to be returned to the caller */
    return result;
}

static PyObject *print_devices(PyObject *self, PyObject *args) {
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

    /* returns an invalid value to the caller method/function
    as this function should not return anything */
    Py_RETURN_NONE;
}

static PyObject *print_hello(PyObject *self, PyObject *args) {
    /* allocates space for the decoded data buffer and for
    the storage of the length (size) of it */
    char *data;
    size_t data_length;
    int result;

    /* decodes the data value from the base 64 encoding
    and then uses it to print the data */
    decode_base64(
        (unsigned char *) HELLO_WORLD_B64,
        strlen(HELLO_WORLD_B64),
        (unsigned char **) &data,
        &data_length
    );
    result = print(FALSE, NULL, data, data_length);

    /* releases the decoded buffer (avoids memory leak)
    and then returns in success */
    _free_base64((unsigned char *) data);

    /* in case the print operation failed raises an exception
    so that the caller is notified about the problem */
    if(result < 0) {
        PyErr_SetString(PyExc_IOError, "Problem printing document");
        return NULL;
    }

    /* returns an invalid value to the caller method/function
    as this function should not return anything */
    Py_RETURN_NONE;
}

static PyObject *print_base64(PyObject *self, PyObject *args) {
    /* allocates space for the decoded data buffer and for
    the storage of the length (size) of it */
    char *data;
    char *input;
    size_t data_length;
    int result;

    /* tries to parse the provided sequence of arguments
    as a single string value that is going to be used as
    the input value for the printing of the page */
    if(PyArg_ParseTuple(args, "s", &input) == FALSE) {
        return NULL;
    }

    /* decodes the data value from the base 64 encoding, raising an
    exception in case it's not valid, and then uses it to print the data */
    if(decode_base64(
        (unsigned char *) input,
        strlen(input),
        (unsigned char **) &data,
        &data_length
    ) != 0) {
        PyErr_SetString(PyExc_ValueError, "Invalid data, it must be base 64 encoded");
        return NULL;
    }
    result = print(FALSE, NULL, data, data_length);

    /* releases the decoded buffer (avoids memory leak)
    and then returns in success */
    _free_base64((unsigned char *) data);

    /* in case the print operation failed raises an exception
    so that the caller is notified about the problem */
    if(result < 0) {
        PyErr_SetString(PyExc_IOError, "Problem printing document");
        return NULL;
    }

    /* returns an invalid value to the caller method/function
    as this function should not return anything */
    Py_RETURN_NONE;
}

static char *_get_option(PyObject *options, const char *name, PyObject *encoded) {
    /* retrieves the value of the option with the provided name
    and converts it into an (UTF-8) string, in case the option is
    not set or its value is not a string no value is returned */
    char *result = NULL;
    PyObject *value = PyDict_GetItemString(options, name);
    if(value == NULL) { return NULL; }
#if PY_MAJOR_VERSION >= 3
    if(PyUnicode_Check(value)) {
        result = (char *) PyUnicode_AsUTF8(value);
    }
#else
    /* unicode values are explicitly encoded as UTF-8 and the encoded
    string is kept in the provided list, so that it remains valid
    until the print operation is completed */
    if(PyUnicode_Check(value)) {
        value = PyUnicode_AsUTF8String(value);
        if(value != NULL) {
            if(PyList_Append(encoded, value) == 0) {
                result = PyString_AsString(value);
            }
            Py_DECREF(value);
        }
    } else if(PyString_Check(value)) {
        result = PyString_AsString(value);
    }
#endif

    /* clears any error resulting from a failed conversion, as
    an invalid value is considered to be an option that is not set */
    if(result == NULL) { PyErr_Clear(); }
    return result;
}

static PyObject *print_printer_base64(PyObject *self, PyObject *args, PyObject *kwargs) {
    /* allocates space for the decoded data buffer and for
    the storage of the length (size) of it */
    char *data;
    char *printer;
    char *input;
    size_t data_length;
    int result;
    PyObject *value;
    PyObject *options = NULL;
    PyObject *encoded = NULL;
    struct job_t job = {NULL, 0};
    static char *kwlist[] = {"printer", "data", "options", NULL};

    /* tries to parse the provided sequence of arguments
    as a single string value that is going to be used as
    the input value for the printing of the page */
    if(PyArg_ParseTupleAndKeywords(
        args,
        kwargs,
        "ss|O!",
        kwlist,
        &printer,
        &input,
        &PyDict_Type,
        &options
    ) == FALSE) {
        return NULL;
    }

    // in case options were set then we can build the job
    // options to be used in the print operation
    if(options != NULL) {
        encoded = PyList_New(0);
        if(encoded == NULL) { return NULL; }
        job.output_path = _get_option(options, "output_path", encoded);
        job.title = _get_option(options, "title", encoded);
        job.media = _get_option(options, "media", encoded);
        job.scaling = _get_option(options, "scaling", encoded);

        /* in case an output path is set but it's not possible to use it
        (eg: invalid type) the operation fails, as the document must not
        be printed when it's meant to be written to a file */
        value = PyDict_GetItemString(options, "output_path");
        if(job.output_path == NULL && value != NULL && value != Py_None) {
            Py_DECREF(encoded);
            PyErr_SetString(PyExc_TypeError, "Invalid output path, it must be a string");
            return NULL;
        }
    }

    /* decodes the data value from the base 64 encoding, raising an
    exception in case it's not valid, and then uses it to print the data */
    if(decode_base64(
        (unsigned char *) input,
        strlen(input),
        (unsigned char **) &data,
        &data_length
    ) != 0) {
        Py_XDECREF(encoded);
        PyErr_SetString(PyExc_ValueError, "Invalid data, it must be base 64 encoded");
        return NULL;
    }
    result = print_printer(FALSE, printer, &job, data, data_length);

    /* releases the decoded buffer and the encoded values of
    the options (avoids memory leaks) and then returns in success */
    _free_base64((unsigned char *) data);
    Py_XDECREF(encoded);

    /* in case the print operation failed raises an exception
    so that the caller is notified about the problem */
    if(result < 0) {
        PyErr_Format(PyExc_IOError, "Problem printing document in '%s'", printer);
        return NULL;
    }

    /* returns the result of the print operation to the caller
    method/function, the identifier of the job when available */
    return PyLong_FromLong((long) result);
}

static PyMethodDef colony_functions[] = {
    {"get_format", get_format, METH_NOARGS, "Retrieves the format supported by the system."},
    {"get_devices", get_devices, METH_NOARGS, "Retrieves the complete set of devices."},
    {"print_devices", print_devices, METH_NOARGS, "Prints the complete set of devices to stdout."},
    {"print_hello", print_hello, METH_NOARGS, "Prints an hello message to default printer."},
    {"print_base64", print_base64, METH_VARARGS, "Prints a Base64 based sequence of data to default printer."},
    {"print_printer_base64", (PyCFunction) print_printer_base64, METH_VARARGS | METH_KEYWORDS, "Prints a Base64 based sequence of data in a specific printer with optional options."},
    {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION >= 3
    struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "npcolony",
        "Colony Gateway",
        -1,
        colony_functions,
        NULL,
        NULL,
        NULL,
        NULL,
    };
#endif

#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit_npcolony() {
    PyObject *colony_module = PyModule_Create(&moduledef);
    if(colony_module == NULL) { return NULL; }
    PyModule_AddStringConstant(colony_module, "VERSION", NPCOLONY_VERSION);
    return colony_module;
}
#else
PyMODINIT_FUNC initnpcolony() {
    PyObject *colony_module = Py_InitModule("npcolony", colony_functions);
    if(colony_module == NULL) { return; }
    PyModule_AddStringConstant(colony_module, "VERSION", NPCOLONY_VERSION);
}
#endif

#endif

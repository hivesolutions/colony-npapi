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

#pragma once

#include "../print/print.h"
#include "../encoding/encoding.h"
#include "util.h"

/**
 * The number of methods (smbols) to be exposed
 * by the gateway to the javascript interface.
 */
#define METHODS_COUNT 6

bool has_method(NPObject* obj, NPIdentifier method_name);
bool invoke_default(NPObject *obj, const NPVariant *args, uint32_t arg_count, NPVariant *result);
bool invoke_status(NPObject *obj, const NPVariant *args, uint32_t arg_count, NPVariant *result);
bool invoke_version(NPObject *obj, const NPVariant *args, uint32_t arg_count, NPVariant *result);
bool invoke_callback(NPObject *obj, const NPVariant *args, uint32_t arg_count, NPVariant *result);
bool invoke_alert(NPObject *obj, const NPVariant *args, uint32_t arg_count, NPVariant *result);
bool invoke_print(NPObject *obj, const NPVariant *args, uint32_t arg_count, NPVariant *result);
bool invoke(NPObject* obj, NPIdentifier method_name, const NPVariant *args, uint32_t arg_count, NPVariant *result);
bool has_property(NPObject *obj, NPIdentifier property_name);
bool get_property(NPObject *obj, NPIdentifier property_name, NPVariant *result);

NPError nevv(NPMIMEType plugin_type, NPP instance, uint16 mode, int16 argc, char *argn[], char *argv[], NPSavedData *saved);
NPError destroy(NPP instance, NPSavedData **save);
NPError get_value(NPP instance, NPPVariable variable, void *value);
NPError handle_event(NPP instance, void *ev);
NPError set_window(NPP instance, NPWindow *window);

static NPClass ref_object = {
    NP_CLASS_STRUCT_VERSION,
    NULL,
    NULL,
    NULL,
    has_method,
    invoke,
    invoke_default,
    has_property,
    get_property,
    NULL,
    NULL,
};

static char *methods[METHODS_COUNT] = {
    "status",
    "version",
    "foo",
    "callback",
    "alert",
    "print"
};

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

#include "base.h"

bool has_method(NPObject* obj, NPIdentifier method_name) {
    /* logs the function call */
    log("npcolony: has_method\n");

    /* retrieves the correct string for the method name
    as a normal comparision structure */
    char *name = npnfuncs->utf8fromidentifier(method_name);

    /* iterates over all the methods in the static structure
    to compare them against the requesed method */
    for(size_t index = 0; index < METHODS_COUNT; index++) {
        /* in case the current method is the sames as the
        requested on returns valid */
        if(!strcmp(name, methods[index])) { return true; }
    }

    /* by default returns method not found */
    return false;
}

bool invoke_default(NPObject *obj, const NPVariant *args, uint32_t arg_count, NPVariant *result) {
    /* logs the function call and sets the result as
    the default magic value (answer to the universe) */
    log("npcolony: invoke_default\n");
    result->type = NPVariantType_Int32;
    result->value.intValue = 42;
    return true;
}

bool invoke_status(NPObject *obj, const NPVariant *args, uint32_t arg_count, NPVariant *result) {
    result->type = NPVariantType_Bool;
    result->value.boolValue = true;
    return true;
}

bool invoke_version(NPObject *obj, const NPVariant *args, uint32_t arg_count, NPVariant *result) {
    /* allocates static space for the hello message and
    then allocates npapi space for it */
    const char version[] = "1.0.11";
    char *version_message = (char *) npnfuncs->memalloc(strlen(version));

    /* copes the version value into the javascript owned
    version message */
    memcpy(version_message, version, strlen(version));

    /* allocates space for the version string and populates
    it with the just copied version message and the length of it */
    NPString version_string;
    SET_UTF8_CHARACTERS(version_string, version_message);
    SET_UTF8_LENGTH(version_string, strlen(version));

    /* sets the version string value in the return value */
    result->type = NPVariantType_String;
    result->value.stringValue = version_string;

    /* returns in success */
    return true;
}

bool invoke_callback(NPObject *obj, const NPVariant *args, uint32_t arg_count, NPVariant *result) {
	/* validates that the number of arguments is one and that
	the provided argument is an object (callback function) */
	if(arg_count != 1 || args[0].type != NPVariantType_Object) {
		return true;
	}

	/* allocates space for the result of the callback call */
	bool result_c;

    /* allocates space for both the parameter and the return
    value variant values */
    static NPVariant parameter;
    static NPVariant return_value;

    /* allocates static space for the hello message and
    then allocates npapi space for it */
    const char hello[] = "Hello World";
    char *message = (char *) npnfuncs->memalloc(strlen(hello));

    /* copies the hello message into the allocated message
    and then converts it into a variant value */
    memcpy(message, hello, strlen(hello));
    STRINGN_TO_NPVARIANT(message, strlen(hello), parameter);

    /* invokes the callback function and then returns the value of the
    invoke default function */
    result_c = npnfuncs->invokeDefault(
	    inst,
		NPVARIANT_TO_OBJECT(args[0]),
		&parameter,
		1,
		&return_value
	);
	if(!result_c) { return true; }

	/* sets the result of the call as null
	and returns the function with a valid value */
	result->type = NPVariantType_Null;
    return true;
}

#ifdef COLONY_PLATFORM_WIN32

bool invoke_alert(NPObject *obj, const NPVariant *args, uint32_t arg_count, NPVariant *result) {
    /* retrieves the first argument as an utf8 string */
    struct _NPString message_string = NPVARIANT_TO_STRING(args[0]);

    /* allocates space for both the title and the message
    of the alert box then converts them from the base string
    encoded utf8 values to the win32 api unicode representation */
    wchar_t *title = new wchar_t[6];
    wchar_t *message = new wchar_t[message_string.utf8length + 1];
    MultiByteToWideChar(CP_UTF8, NULL, "Alert", -1, title, 6);
    size_t count = MultiByteToWideChar(
        CP_UTF8,
		NULL,
		message_string.utf8characters,
		message_string.utf8length,
		message,
		message_string.utf8length
	);
    message[count] = '\0';

    /* creates the alert box with the "just" converted title
    and message values (both encoded in unicode) */
    int return_value = MessageBoxW(
        NULL,
        message,
        title,
        MB_ICONINFORMATION | MB_OK
    );

    /* releases the title and the message, then returns
    the execution control to the caller */
    delete message;
    delete title;
    return true;
}

#endif

#ifdef COLONY_PLATFORM_UNIX

bool invoke_alert(NPObject *obj, const NPVariant *args, uint32_t arg_count, NPVariant *result) {
    return true;
}

#endif

bool invoke_print(NPObject *obj, const NPVariant *args, uint32_t arg_count, NPVariant *result) {
    /* retrieves both the show dialog and the data string
    values to be used in the printing operation */
    bool show_dialog = NPVARIANT_TO_BOOLEAN(args[0]);
    struct _NPString data_string = NPVARIANT_TO_STRING(args[1]);

    /* allocates space for the decoded data buffer and for
    the storage of the length (size) of it */
    char *data;
    size_t data_length;

    /* decodes the data value from the base 64 encoding
    and then uses it to print the data */
    decode_base64(
		(unsigned char *) GET_UTF8_CHARACTERS(data_string),
		GET_UTF8_LENGTH(data_string),
		(unsigned char **) &data,
		&data_length
	);
    print(show_dialog, (char *) data);

    /* releases the decoded buffer (avoids memory leak)
    and then returns in success */
    free(data);
    return true;
}

bool invoke(NPObject* obj, NPIdentifier method_name, const NPVariant *args, uint32_t arg_count, NPVariant *result) {
    log("npcolony: invoke\n");
    char *name = npnfuncs->utf8fromidentifier(method_name);

    if(name) {
        if(!strcmp(name, "status")) {
           /* returns the result of the status invoking */
            return invoke_status(obj, args, arg_count, result);
        } else if(!strcmp(name, "version")) {
           /* returns the result of the version invoking */
            return invoke_version(obj, args, arg_count, result);
        } else if(!strcmp(name, "foo")) {
            /* returns the result of the default invoking */
            return invoke_default(obj, args, arg_count, result);
        } else if(!strcmp(name, "callback")) {
            /* returns the result of the callback invoking */
            return invoke_callback(obj, args, arg_count, result);
        } else if(!strcmp(name, "alert")) {
            /* returns the result of the alert invoking */
            return invoke_alert(obj, args, arg_count, result);
        } else if(!strcmp(name, "print")) {
            /* returns the result of the print invoking */
            return invoke_print(obj, args, arg_count, result);
        }
    }

    /* in case the control reaches this area an exception
    must have occured (invalid name) sets it in the environment */
    npnfuncs->setexception(obj, "exception during invocation");

    return false;
}

bool has_property(NPObject *obj, NPIdentifier property_name) {
    log("npcolony: has_property\n");
    return false;
}

bool get_property(NPObject *obj, NPIdentifier property_name, NPVariant *result) {
    log("npcolony: get_property\n");
    return false;
}

NPError nevv(NPMIMEType plugin_type, NPP instance, uint16 mode, int16 argc, char *argn[], char *argv[], NPSavedData *saved) {
    log("npcolony: new\n");
    inst = instance;
    return NPERR_NO_ERROR;
}

NPError destroy(NPP instance, NPSavedData **save) {
    /* in case the shared object is defined
    releases it from memory (avoids leaking) */
    if(so) { npnfuncs->releaseobject(so); }

    /* unsets the shared object reference, so that no more
    usage of it may be done without null pointing*/
    so = NULL;

    /* returns with no error */
    return NPERR_NO_ERROR;
}

NPError get_value(NPP instance, NPPVariable variable, void *value) {
    inst = instance;

    switch(variable) {
        case NPPVpluginNameString:
            *((char **)value) = "ColonyGatewayPlugin";
            break;

        case NPPVpluginDescriptionString:
            *((char **)value) = "<a href=\"http://getcolony.com/\">Colony Gateway</a> plugin.";
            break;

        case NPPVpluginScriptableNPObject:
            if(!so) { so = npnfuncs->createobject(instance, &ref_object); }
            npnfuncs->retainobject(so);
            * (NPObject **) value = so;
            break;

        case NPPVpluginNeedsXEmbed:
            *((PRBool *) value) = PR_FALSE;
            break;

        default:
            return NPERR_GENERIC_ERROR;
    }

    return NPERR_NO_ERROR;
}

/* expected by Safari on Darwin */
NPError handle_event(NPP instance, void *ev) {
    log("npcolony: handle_event\n");
    inst = instance;
    return NPERR_NO_ERROR;
}

/* expected by Opera */
NPError set_window(NPP instance, NPWindow *window) {
    log("npcolony: set_window\n");
    inst = instance;
    return NPERR_NO_ERROR;
}

#ifdef __cplusplus
extern "C" {
#endif

NPError OSCALL NP_GetEntryPoints(NPPluginFuncs *nppfuncs) {
    nppfuncs->version = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;
    nppfuncs->newp = nevv;
    nppfuncs->destroy = destroy;
    nppfuncs->getvalue = get_value;
    nppfuncs->event = handle_event;
    nppfuncs->setwindow = set_window;

    return NPERR_NO_ERROR;
}

NPError OSCALL NP_Initialize(NPNetscapeFuncs *npnf) {
    /* in case the functions table is not valid,
    must return in error */
    if(npnf == NULL) {
        /* returns in error (invalid function table) */
        return NPERR_INVALID_FUNCTABLE_ERROR;
    }

    /* checks if there are incompatible version for
    the plugin interface */
    if(HIBYTE(npnf->version) > NP_VERSION_MAJOR) {
        /* returns in error (incompatbile version) */
        return NPERR_INCOMPATIBLE_VERSION_ERROR;
    }

    /* save the functions in a global variable
    and returns in no error */
    npnfuncs = npnf;
    return NPERR_NO_ERROR;
}

NPError OSCALL NP_Shutdown() {
    return NPERR_NO_ERROR;
}

char *NP_GetMIMEDescription() {
    return "application/x-colony-gateway:.colony:gateway@getcolony.com";
}

NPError OSCALL NP_GetValue(void *npp, NPPVariable variable, void *value) {
    inst = (NPP) npp;
    return get_value((NPP) npp, variable, value);
}

#ifdef __cplusplus
}
#endif

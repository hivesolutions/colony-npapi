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

#include "base.h"

bool hasMethod(NPObject* obj, NPIdentifier methodName) {
    /* logs the function call */
    log("npcolony: hasMethod\n");
    return true;
}

bool invokeDefault(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
    /* logs the function call and sets the result as
    the default magic value (answer to the universe) */
    log("npcolony: invokeDefault\n");
    result->type = NPVariantType_Int32;
    result->value.intValue = 42;
    return true;
}

bool invoke(NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result) {
    log("npcolony: invoke\n");
    int error = 1;
    char *name = npnfuncs->utf8fromidentifier(methodName);

    if(name) {
        if(!strcmp(name, "foo")) {
            /* returns the result of the default invoking */
            return invokeDefault(obj, args, argCount, result);
        } else if(!strcmp(name, "callback")) {
            if(argCount == 1 && args[0].type == NPVariantType_Object) {
                /* allocates space for both the parameter and the return
                value variant values */
                static NPVariant parameter;
                static NPVariant returnValue;

                /* allocates static space for the hello message and
                then allocates npapi space for it */
                const char hello[] = "Hello World";
                char *message = (char *) npnfuncs->memalloc(strlen(hello));

                /* copies the hello message into the allocated message
                and then converts it into a variant value */
                memcpy(message, hello, strlen(hello));
                STRINGN_TO_NPVARIANT(message, strlen(hello), parameter);

                /* invokes the callback fnction and then returns the value of the
                invoke default function */
                if(npnfuncs->invokeDefault(inst, NPVARIANT_TO_OBJECT(args[0]), &parameter, 1, &returnValue)) {
                    /* returns the result of the default invoking */
                    return invokeDefault(obj, args, argCount, result);
                }
            }
        } else if(!strcmp(name, "alert")) {
            /* retrieves the first argument as an utf8 string */
            struct _NPString messageString = NPVARIANT_TO_STRING(args[0]);

            /* allocates space for both the title and the message
            of the alert box then converts them from the base string
            encoded utf8 values to the win32 api unicode representation */
            wchar_t *title = new wchar_t[6];
            wchar_t *message = new wchar_t[messageString.utf8length + 1];
            MultiByteToWideChar(CP_UTF8, NULL, "Alert", -1, title, 6);
            size_t count = MultiByteToWideChar(CP_UTF8, NULL, messageString.utf8characters, messageString.utf8length, message, messageString.utf8length);
            message[count] = '\0';

            /* creates the alert box with the "just" converted title
            and message values (both encoded in unicode) */
            int returnValue = MessageBoxW(
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
        } else if(!strcmp(name, "print")) {
            /* retrieves both the show dialog and the data string
            values to be used in the printing operation */
            bool showDialog = NPVARIANT_TO_BOOLEAN(args[0]);
            struct _NPString dataString = NPVARIANT_TO_STRING(args[1]);

            /* allocates space for the decoded data buffer and for
            the storage of the length (size) of it */
            char *data;
            size_t dataLength;

            /* decodes the data value from the base 64 encoding
            and then uses it to print the data */
            decodeBase64((unsigned char *) dataString.utf8characters, dataString.utf8length, (unsigned char **) &data, &dataLength);
            print(showDialog, (char *) data);

            /* releases the decoded buffer (avoids memory leak)
            and then returns in success */
            free(data);
            return true;
        }
    }

    /* in case the control reaches this area an exception
    must have occured (invalid name) sets it in the environment */
    npnfuncs->setexception(obj, "exception during invocation");

    return false;
}

bool hasProperty(NPObject *obj, NPIdentifier propertyName) {
    log("npcolony: hasProperty\n");
    return false;
}

bool getProperty(NPObject *obj, NPIdentifier propertyName, NPVariant *result) {
    log("npcolony: getProperty\n");
    return false;
}

NPError nevv(NPMIMEType pluginType, NPP instance, uint16 mode, int16 argc, char *argn[], char *argv[], NPSavedData *saved) {
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

NPError getValue(NPP instance, NPPVariable variable, void *value) {
    inst = instance;

    switch(variable) {
        case NPPVpluginNameString:
            *((char **)value) = "ColonyGatewayPlugin";
            break;

        case NPPVpluginDescriptionString:
            *((char **)value) = "<a href=\"http://getcolony.com/\">Colony Gateway</a> plugin.";
            break;

        case NPPVpluginScriptableNPObject:
            if(!so) { so = npnfuncs->createobject(instance, &npcRefObject); }
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
NPError handleEvent(NPP instance, void *ev) {
    log("npcolony: handleEvent\n");
    inst = instance;
    return NPERR_NO_ERROR;
}

/* expected by Opera */
NPError setWindow(NPP instance, NPWindow* pNPWindow) {
    log("npcolony: setWindow\n");
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
    nppfuncs->getvalue = getValue;
    nppfuncs->event = handleEvent;
    nppfuncs->setwindow = setWindow;

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
    return getValue((NPP) npp, variable, value);
}

#ifdef __cplusplus
}
#endif

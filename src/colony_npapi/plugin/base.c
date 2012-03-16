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

#include <Windows.h>
#include <winspool.h>
#include <Commdlg.h>

#define BUFFER_SIZE 1024

typedef struct ElementHeader_t {
	unsigned int type;
	unsigned int length;
} ElementHeade;

typedef struct Position_t {
	int x;
	int y;
} Position;

typedef struct Element_t {
	struct ElementHeader_t header;
	void *contents;
} Element;

typedef struct TextElementHeader_t {
	struct ElementHeader_t header;
	struct Position_t position;
    char font[256];
    unsigned int fontSize;
	unsigned int textAlign;
	unsigned int textWeight;
	unsigned int textItalic;
    unsigned int length;
} TextElementHeader;

typedef struct TextElement_t {
	struct TextElementHeader_t header;
	char *text;
} TextElement;

typedef struct DocumentHeader_t {
	char title[256];
	unsigned int elementCount;
} DocumentHeader;

typedef struct Document_t {
	struct DocumentHeader_t header;
	char *elements;
} Document;

HDC getDefaultPrinter() {
	/* allocates a new buffer in the stack
	and then set a long variable with the size
	of it to be used in the printer call */
	char buffer[BUFFER_SIZE];
	unsigned long size = BUFFER_SIZE;

	/* retrieves the default printer and then
	and then uses it to create the apropriate context */
	GetDefaultPrinter(buffer, &size);
	HDC hPrinterDC = CreateDC("WINSPOOL\0", buffer, NULL, NULL);

	/* retrieves the just created context */
	return hPrinterDC;
}

BOOL showPrintDialog(PRINTDLG *printDialogPointer) {
	/* populates the print dialog structure
	with the appropriate values */
	printDialogPointer->lStructSize = sizeof(PRINTDLG);
	printDialogPointer->hwndOwner = NULL;
	printDialogPointer->hDevMode = NULL;
	printDialogPointer->hDevNames = NULL;
	printDialogPointer->Flags = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC;
	printDialogPointer->nCopies = 1;
	printDialogPointer->nFromPage = 0xffff;
	printDialogPointer->nToPage	= 0xffff;
	printDialogPointer->nMinPage = 1;
	printDialogPointer->nMaxPage = 0xffff;

	/* shows the print dialog and then retrieves
	the result of the execution of it, returning it
	to the caller function */
	BOOL result = PrintDlg(printDialogPointer);
	return result;
}

int print(bool showDialog) {
	/* reserves space for the printing context to be
	used in the current operation */
	HDC context;

	/* in case the printer dialog is meant to be shown
	the proper show dialog function must be called*/
	if(showDialog) {
		/* allocates and resets the print dialog structure
		according to the windows rules */
		PRINTDLG printDialog;
		ZeroMemory(&printDialog, sizeof(PRINTDLG));
		BOOL result = showPrintDialog(&printDialog);
		if(!result) { return -1; }
		context = printDialog.hDC;
	}
	/* otherwise the default printer is retrieved */
	else {
		/* retrieves the default printer as the
		the default context for printing */
		context = getDefaultPrinter();
	}

	/* declares a document information structure
	and populate it */
	DOCINFO documentInformation;
	documentInformation.cbSize = sizeof(DOCINFO);
	documentInformation.lpszDocName	= "Glowdot Port Scanner";
	documentInformation.lpszOutput = NULL;
	documentInformation.fwType = 0;






	/* contructs the document information and prints
	it on finishing it (print on close document) */
	StartDoc (context, &documentInformation);
	StartPage(context);

	SetMapMode(context, MM_TWIPS);


/*
	HFONT font = CreateFont(
        48,
		0,
		0,
		0,
		FW_DONTCARE,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY,
		VARIABLE_PITCH,
		"Calibri"
	);
	SelectObject(context, font);
	TextOut(context, 10, 10, "Hello World", lstrlen("Hello World"));*/


	/* POR O ITEM DE FONT SCALE */


	FILE *file = fopen("c:/tobias.binie", "rb");

	fseek(file, 0, SEEK_END);
	size_t sz = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *buffer = (char *) malloc(sz);
	fread(buffer, sizeof(char), sz, file);

	size_t a = sizeof(struct DocumentHeader_t);

	struct DocumentHeader_t *documentHeader = (struct DocumentHeader_t *) buffer;
    struct ElementHeader_t *elementHeader = (struct ElementHeader_t *) (buffer + sizeof(struct DocumentHeader_t));

	for(size_t index = 0; index < documentHeader->elementCount; index++) {
		unsigned short elementType = elementHeader->type;
		unsigned int elementLength = elementHeader->length;

		switch(elementType) {
			case 1:
				struct TextElementHeader_t *textElementHeader = (struct TextElementHeader_t *) elementHeader;
				char *text = (char *) textElementHeader + sizeof(struct TextElementHeader_t);

				printf(text);

				int weight = FW_DONTCARE;

				if(textElementHeader->textWeight > 0) { weight = FW_BOLD; }

				HFONT font = CreateFont(
					textElementHeader->fontSize * 20,
					0,
					0,
					0,
					weight,
					textElementHeader->textItalic,
					FALSE,
					FALSE,
					DEFAULT_CHARSET,
					OUT_OUTLINE_PRECIS,
					CLIP_DEFAULT_PRECIS,
					CLEARTYPE_QUALITY,
					VARIABLE_PITCH,
					textElementHeader->font
				);
				SelectObject(context, font);
				TextOut(context, textElementHeader->position.x, textElementHeader->position.y, text, lstrlen(text));

				break;
		}

		elementHeader = (struct ElementHeader_t *) ((char *) elementHeader + sizeof(struct ElementHeader_t) + elementLength);
	}


	EndPage(context);
	EndDoc(context);

	/* deletes the print context (avoids leaking ofmemory) */
	DeleteDC(context);

	/* returns with no error */
	return 0;
}





bool hasMethod(NPObject* obj, NPIdentifier methodName) {
    /* logs the function call */
    logmsg("npsimple: hasMethod\n");
    return true;
}

bool invokeDefault(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	/* logs the function call and sets the result as
	the default magic value (answer to the universe) */
    logmsg("npsimple: invokeDefault\n");
    result->type = NPVariantType_Int32;
    result->value.intValue = 42;
    return true;
}

bool invoke(NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result) {
    logmsg("npsimple: invoke\n");
    int error = 1;
    char *name = npnfuncs->utf8fromidentifier(methodName);

    if(name) {
        if(!strcmp(name, "foo")) {
            logmsg("npsimple: invoke foo\n");
            return invokeDefault(obj, args, argCount, result);
        } else if(!strcmp(name, "callback")) {
            if(argCount == 1 && args[0].type == NPVariantType_Object) {
                static NPVariant v, r;
                const char kHello[] = "Hello World";
                char *txt = (char *) npnfuncs->memalloc(strlen(kHello));

                logmsg("npsimple: invoke callback function\n");
                memcpy(txt, kHello, strlen(kHello));

                STRINGN_TO_NPVARIANT(txt, strlen(kHello), v);
                /* INT32_TO_NPVARIANT(42, v); */

                if(npnfuncs->invokeDefault(inst, NPVARIANT_TO_OBJECT(args[0]), &v, 1, &r)) {
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
			wchar_t *message = new wchar_t[messageString.utf8length];
			MultiByteToWideChar(CP_UTF8, NULL, "Alert", -1, title, 6);
			MultiByteToWideChar(CP_UTF8, NULL, messageString.utf8characters, -1, message, messageString.utf8length);

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
			bool showDialog = NPVARIANT_TO_BOOLEAN(args[0]);
			print(showDialog);
			return true;
		}
    }

    /* in case the control reaches this area an exception
    must have occured (invalid name) sets it in the environment */
    npnfuncs->setexception(obj, "exception during invocation");

    return false;
}

bool hasProperty(NPObject *obj, NPIdentifier propertyName) {
    logmsg("npsimple: hasProperty\n");
    return false;
}

bool getProperty(NPObject *obj, NPIdentifier propertyName, NPVariant *result) {
    logmsg("npsimple: getProperty\n");
    return false;
}

NPError nevv(NPMIMEType pluginType, NPP instance, uint16 mode, int16 argc, char *argn[], char *argv[], NPSavedData *saved) {
    inst = instance;
    logmsg("npsimple: new\n");
    return NPERR_NO_ERROR;
}

NPError destroy(NPP instance, NPSavedData **save) {
	/* in case the shared object is defined
	releases it from memory (avoids leaking) */
    if(so) { npnfuncs->releaseobject(so); }

    /* unsets the shared object reference, so that no more
    usage of it may be done without null pointing*/
    so = NULL;

    logmsg("npsimple: destroy\n");
    return NPERR_NO_ERROR;
}

NPError getValue(NPP instance, NPPVariable variable, void *value) {
    inst = instance;

    switch(variable) {
        case NPPVpluginNameString:
            logmsg("npsimple: getvalue - name string\n");
            *((char **)value) = "AplixFooPlugin";
            break;

        case NPPVpluginDescriptionString:
            logmsg("npsimple: getvalue - description string\n");
            *((char **)value) = "<a href=\"http://www.aplix.co.jp/\">AplixFooPlugin</a> plugin.";
            break;

        case NPPVpluginScriptableNPObject:
            logmsg("npsimple: getvalue - scriptable object\n");
            if(!so)
                so = npnfuncs->createobject(instance, &npcRefObject);
            npnfuncs->retainobject(so);
            *(NPObject **)value = so;
            break;

        case NPPVpluginNeedsXEmbed:
            logmsg("npsimple: getvalue - xembed\n");
            *((PRBool *)value) = PR_FALSE;
            break;

        default:
            logmsg("npsimple: getvalue - default\n");
            return NPERR_GENERIC_ERROR;
    }

    return NPERR_NO_ERROR;
}

/* expected by Safari on Darwin */
NPError handleEvent(NPP instance, void *ev) {
    inst = instance;
    logmsg("npsimple: handleEvent\n");
    return NPERR_NO_ERROR;
}

/* expected by Opera */
NPError setWindow(NPP instance, NPWindow* pNPWindow) {
    inst = instance;
    logmsg("npsimple: setWindow\n");
    return NPERR_NO_ERROR;
}

#ifdef __cplusplus
extern "C" {
#endif

NPError OSCALL NP_GetEntryPoints(NPPluginFuncs *nppfuncs) {
    logmsg("npsimple: NP_GetEntryPoints\n");
    nppfuncs->version       = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;
    nppfuncs->newp          = nevv;
    nppfuncs->destroy       = destroy;
    nppfuncs->getvalue      = getValue;
    nppfuncs->event         = handleEvent;
    nppfuncs->setwindow     = setWindow;

    return NPERR_NO_ERROR;
}

NPError OSCALL NP_Initialize(NPNetscapeFuncs *npnf) {
    logmsg("npsimple: NP_Initialize\n");

    if(npnf == NULL) {
        return NPERR_INVALID_FUNCTABLE_ERROR;
    }

    /* checks if there are incompatible version for
    the plugin interface */
    if(HIBYTE(npnf->version) > NP_VERSION_MAJOR) {
        /* returns in error (incompatbile version) */
        return NPERR_INCOMPATIBLE_VERSION_ERROR;
    }

    npnfuncs = npnf;

    /* returns in no error */
    return NPERR_NO_ERROR;
}

NPError OSCALL NP_Shutdown() {
    logmsg("npsimple: NP_Shutdown\n");
    return NPERR_NO_ERROR;
}

char *NP_GetMIMEDescription() {
    logmsg("npsimple: NP_GetMIMEDescription\n");
    return "application/x-vnd-aplix-foo:.foo:anselm@aplix.co.jp";
}

/* needs to be present for WebKit based browsers */
NPError OSCALL NP_GetValue(void *npp, NPPVariable variable, void *value) {
    inst = (NPP) npp;
    return getValue((NPP) npp, variable, value);
}

#ifdef __cplusplus
}
#endif

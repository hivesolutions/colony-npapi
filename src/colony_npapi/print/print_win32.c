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

#include "print_win32.h"

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
    printDialogPointer->nToPage = 0xffff;
    printDialogPointer->nMinPage = 1;
    printDialogPointer->nMaxPage = 0xffff;

    /* shows the print dialog and then retrieves
    the result of the execution of it, returning it
    to the caller function */
    BOOL result = PrintDlg(printDialogPointer);
    return result;
}

int print(bool showDialog, char *data) {
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

    /* allocates space for the buffer to be used during
    the parsing stage */
    char *buffer = NULL;

    /* in case the data buffer was sent, we should be
    reading the data from there */
    if(data) {
        /* sets the data pointer as the buffer to be
        used for the parsing */
        buffer = data;
    }
    /* otherwise the data must be read from the local
    file system */
    else {
        FILE *file;
        fopen_s(&file, "default.binie", "rb");

        /* retrieves the size of the file by seeking
        to the end of it an then retrieving the offset */
        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        fseek(file, 0, SEEK_SET);

        /* allocates space for the buffer to hold the binie
        document file and then reads it into it, closing the
        file afterwards */
        buffer = (char *) malloc(size);
        fread(buffer, sizeof(char), size, file);
        fclose(file);
    }

    /* casts the initial part of the buffer into a document
    header element */
    struct DocumentHeader_t *documentHeader = (struct DocumentHeader_t *) buffer;

    /* declares a document information structure
    and populate it */
    DOCINFO documentInformation;
    documentInformation.cbSize = sizeof(DOCINFO);
    documentInformation.lpszDocName = documentHeader->title;
    documentInformation.lpszOutput = NULL;
    documentInformation.fwType = 0;

    /* contructs the document information and prints
    it on finishing it (print on close document) */
    StartDoc(context, &documentInformation);
    StartPage(context);

    /* sets the map mode of the document to twips */
    SetMapMode(context, MM_TWIPS);

    /* creates a new (drawing) pen for the document to
    be used in the drawing process of it */
    HANDLE pen = CreatePen(0, FONT_SCALE_FACTOR, 0);
    SelectObject(context, pen);

    /* retrieves the initial document element  header */
    struct ElementHeader_t *elementHeader = (struct ElementHeader_t *) (buffer + sizeof(struct DocumentHeader_t));

    /* retrieves the horizontal and vertical resolution and pixel
    density capabilities */
    int horizontalResolution = GetDeviceCaps(context, HORZRES);
    int verticalResolution = GetDeviceCaps(context, VERTRES);
    int verticalSize = GetDeviceCaps(context, VERTSIZE);
    int pixelDensity = GetDeviceCaps(context, LOGPIXELSY);

    /* start the current page value at the initial value */
    int currentPage = 0;

    /* iterates over the element count in the document to
    process it and generate the correct print instructions */
    for(size_t index = 0; index < documentHeader->elementCount; index++) {
        /* retrieves the type and length of the element */
        unsigned short elementType = elementHeader->type;
        unsigned int elementLength = elementHeader->length;

        /* allocates space fot the various elements to be used
        along the switch instruction */
        SIZE textSize;
        RECT clipBox;
        HFONT font;
        int result;
        int textX;
        int textY;
        int textYBottom;
        double textYBottomMillimeter;
        int weight;
        char *text;
        char *image;
        wchar_t *textUnicode;
        struct TextElementHeader_t *textElementHeader;
        struct ImageElementHeader_t *imageElementHeader;
        char directoryBuffer[1024];
        char path[MAX_PATH];
        int imageX;
        int imageY;
        int imageYBottom;
        double imageYBottomMillimeter;
        int previousMode;
        double divisor;
        double multiplier;
        HDC imageContext;
        HBITMAP handleImage;
        HANDLE handleImageNew;
        FILE *imageFile;
        BITMAP bitmap;
        float scaledWidth;
        float scaledHeight;
        int newPage;
        double pageSizeTwips;

        /* switches over the element type to generate the
        appropriate print instructions */
        switch(elementType) {
            case 1:
                /* "casts" the element header as text element header an retrieves
                the text part from it, then sets the default text weight as normal */
                textElementHeader = (struct TextElementHeader_t *) elementHeader;
                text = (char *) textElementHeader + sizeof(struct TextElementHeader_t);
                weight = FW_DONTCARE;

                /* in case the text weight is greater than zero it's
                considered to be bold sized */
                if(textElementHeader->textWeight > 0) { weight = FW_BOLD; }

                /* creates the correct front to display the current text
                instruction, and then selects it */
                font = CreateFont(
                    textElementHeader->fontSize * FONT_SCALE_FACTOR,
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

                /* converts the text into the appropriate windows unicode
                representation (may represent all charset) */
                textUnicode = new wchar_t[lstrlen(text) + 1];
                result = MultiByteToWideChar(CP_UTF8, NULL, text, -1, textUnicode, lstrlen(text) + 1);

                /* retrieves the extension (size) of the text for the current
                font using the current settings */
                GetTextExtentPointW(context, textUnicode, lstrlenW(textUnicode), &textSize);
                GetClipBox(context, &clipBox);

                /* calculates the text initial x position (deducting the margins)
                and using the current font scale factor */
                textX = (textElementHeader->marginLeft - textElementHeader->marginRight) * FONT_SCALE_FACTOR;

                /* in case the text align is left */
                if(textElementHeader->textAlign == LEFT_TEXT_ALIGN_VALUE) {
                    textX += 0;
                }
                /* in case the text align is right */
                else if(textElementHeader->textAlign == RIGHT_TEXT_ALIGN_VALUE) {
                    textX += clipBox.right - textSize.cx;
                }
                /* in case the text align is left */
                else if(textElementHeader->textAlign == CENTER_TEXT_ALIGN_VALUE) {
                    textX += clipBox.right / 2 - textSize.cx / 2;
                }

                /* sets the text y as the current position context y */
                textY = textElementHeader->position.y;

                /* calculates the y position for the bottom position of the
                text and then converts it into a milimiter type */
                textYBottom = textY - textSize.cy;
                textYBottomMillimeter = (double) textYBottom / TWIPS_PER_INCH * -1.0 * MM_PER_INCH;

                /* uses the bottom position of the text in milimiters and
                divides (integer division) it over the page size to check
                the current page number (index) */
                newPage = (int) (textYBottomMillimeter / verticalSize);

                /* checks if there is a new page for writing, in case
                there is a new page must be "constructed" */
                if(newPage != currentPage) {
                    /* ends the current page and starts a new
                    on (page break operation) */
                    EndPage(context);
                    StartPage(context);

                    /* updates the current page variable with
                    the new page value */
                    currentPage = newPage;
                }

                /* calculates the size of the page size in twips units
                and uses it to re-calculate the text y position, taking
                into account the already "used" pages (modulus) */
                pageSizeTwips = (verticalSize / MM_PER_INCH * TWIPS_PER_INCH);
                textY += (int) ((double) newPage * pageSizeTwips);

				/* resets the text y position in case the value is greater
				than the maximum zero value, otherwise uses the "normal" text
				y position value (default case) */
				textY = textY > 0 ? 0 : textY;

                /* outputs the text to the current drawing context */
                TextOutW(context, textX, textY, textUnicode, lstrlenW(textUnicode));

                /* releases the unicode representation of the text */
                delete textUnicode;

                /* breaks the switch */
                break;

            case 2:
                /* "casts" the element header as image element header an retrieves
                the image part from it */
                imageElementHeader = (struct ImageElementHeader_t *) elementHeader;
                image = (char *) imageElementHeader + sizeof(struct ImageElementHeader_t);

                /* retrieves the temporary path (directory) and then uses it to
                generate a temporary path for out image file */
                GetTempPath(1024, directoryBuffer);
                GetTempFileName(directoryBuffer, "default", 0, path);

                /* opens the image file for binary writing, writes the image contents
                to it and then closes the file*/
                fopen_s(&imageFile, path, "wb");
                fwrite(image, sizeof(char), imageElementHeader->length, imageFile);
                fclose(imageFile);

                /* loads the (bitmap) image from the file and creates the appropriate in
                memory image handler */
                handleImageNew = LoadImage(0, (LPCTSTR) path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
                imageContext = CreateCompatibleDC(NULL);
                handleImage = SelectBitmap(imageContext, handleImageNew);
                GetObject(handleImageNew, sizeof(bitmap), &bitmap);

                /* removes the temporary image file (it's no longer reuired)`*/
                remove(path);

                /* calculates the pixel divisor (resizing for text mode) and
                calculates the multipler for the image size */
                divisor = TWIPS_PER_INCH / pixelDensity;
                multiplier = (double) IMAGE_SCALE_FACTOR / divisor;

                /* in case the text align is left */
                if(imageElementHeader->textAlign == LEFT_TEXT_ALIGN_VALUE) {
                    imageX = 0;
                }
                /* in case the text align is right */
                else if(imageElementHeader->textAlign == RIGHT_TEXT_ALIGN_VALUE) {
                    imageX = horizontalResolution - bitmap.bmWidth;
                }
                /* in case the text align is left */
                else if(imageElementHeader->textAlign == CENTER_TEXT_ALIGN_VALUE) {
                    imageX = (int) ((float) horizontalResolution / 2.0) - (int) ((float) bitmap.bmWidth * (float) multiplier / 2.0);
                }

                /* calculates the scaled with and height taking into account the
                "just" calculated multiplier value */
                scaledWidth = (float) bitmap.bmWidth * (float) multiplier;
                scaledHeight = (float) bitmap.bmHeight * (float) multiplier;

                /* calculates the y position for the bottom position of the
                image and then converts it into a milimiter type */
                imageYBottom = imageElementHeader->position.y + (int) (scaledHeight * divisor);
                imageYBottomMillimeter = (double) imageYBottom / TWIPS_PER_INCH * -1.0 * MM_PER_INCH;

                /* uses the bottom position of the image in milimiters and
                divides (integer division) it over the page size to check
                the current page number (index) */
                newPage = (int) (imageYBottomMillimeter / verticalSize);

                /* checks if there is a new page for writing, in case
                there is a new page must be "constructed" */
                if(newPage != currentPage) {
                    /* ends the current page and starts a new
                    on (page break operation) */
                    EndPage(context);
                    StartPage(context);

                    /* updates the current page variable with
                    the new page value */
                    currentPage = newPage;
                }

                /* calculates the size of the page size in twips units
                and uses it to re-calculate the image y position, taking
                into account the already "used" pages (modulus) */
                pageSizeTwips = (verticalSize / MM_PER_INCH * TWIPS_PER_INCH);
                imageY = (int) ((double) imageElementHeader->position.y + ((double) newPage * pageSizeTwips));

				/* resets the image y position in case the value is greater
				than the maximum zero value, otherwise uses the "normal" image
				y position value (default case) */
				imageY = imageY > 0 ? 0 : imageY;

                /* sets the image y as the current position context y using
                the divisor for text mode scale */
                imageY = (int) ((double) imageY / divisor) * -1;

                /* switches the map mode to text (pixel oriented) and writes
                the image into the current context, then switches back to the
                previous map mode */
                previousMode = GetMapMode(context);
                SetMapMode(context, MM_TEXT);
                StretchBlt(context, imageX, imageY, (int) scaledWidth, (int) scaledHeight, imageContext, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
                SetMapMode(context, previousMode);

                /* selectes the bitmap for the context and then deletes the
                "just" generated drawing context */
                SelectBitmap(imageContext, handleImage);
                DeleteDC(imageContext);

                /* breaks the switch */
                break;
        }

        /* retrieves te next element header, taking into account the size
        of the current element (this is the increment delta to be used) */
        elementHeader = (struct ElementHeader_t *) ((char *) elementHeader + sizeof(struct ElementHeader_t) + elementLength);
    }

    /* ends the current page and the document for the
    current context */
    EndPage(context);
    EndDoc(context);

    /* deletes the print context (avoids leaking ofmemory) */
    DeleteDC(context);

    /* releases the buffer that contains the binie document,
    only in case the buffer was created from the default file */
    if(!data) { free(buffer); }

    /* returns with no error */
    return 0;
}

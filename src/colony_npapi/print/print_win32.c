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

#include "print_win32.h"

HDC get_default_printer() {
    /* allocates a new buffer in the stack
    and then set a long variable with the size
    of it to be used in the printer call */
    char buffer[BUFFER_SIZE];
    unsigned long size = BUFFER_SIZE;

    /* retrieves the default printer and then
    and then uses it to create the apropriate context */
    GetDefaultPrinter(buffer, &size);
    HDC handle_printer = CreateDC("WINSPOOL\0", buffer, NULL, NULL);

    /* retrieves the just created context */
    return handle_printer;
}

BOOL show_print_dialog(PRINTDLG *print_dialog_pointer) {
    /* populates the print dialog structure
    with the appropriate values */
    print_dialog_pointer->lStructSize = sizeof(PRINTDLG);
    print_dialog_pointer->hwndOwner = NULL;
    print_dialog_pointer->hDevMode = NULL;
    print_dialog_pointer->hDevNames = NULL;
    print_dialog_pointer->Flags = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC;
    print_dialog_pointer->nCopies = 1;
    print_dialog_pointer->nFromPage = 0xffff;
    print_dialog_pointer->nToPage = 0xffff;
    print_dialog_pointer->nMinPage = 1;
    print_dialog_pointer->nMaxPage = 0xffff;

    /* shows the print dialog and then retrieves
    the result of the execution of it, returning it
    to the caller function */
    BOOL result = PrintDlg(print_dialog_pointer);
    return result;
}

int print(bool show_dialog, char *data) {
    /* reserves space for the printing context to be
    used in the current operation */
    HDC context;

    /* in case the printer dialog is meant to be shown
    the proper show dialog function must be called*/
    if(show_dialog) {
        /* allocates and resets the print dialog structure
        according to the windows rules */
        PRINTDLG print_dialog;
        ZeroMemory(&print_dialog, sizeof(PRINTDLG));
        BOOL result = show_print_dialog(&print_dialog);
        if(!result) { return -1; }
        context = print_dialog.hDC;
    }
    /* otherwise the default printer is retrieved */
    else {
        /* retrieves the default printer as the
        the default context for printing */
        context = get_default_printer();
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
		/* creates the file object reference and opens
		the target (default) file */
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
    struct document_header_t *document_header = (struct document_header_t *) buffer;

    /* declares a document information structure
    and populate it */
    DOCINFO document_information;
    document_information.cbSize = sizeof(DOCINFO);
    document_information.lpszDocName = document_header->title;
    document_information.lpszOutput = NULL;
    document_information.fwType = 0;

    /* contructs the document information and prints
    it on finishing it (print on close document) */
    StartDoc(context, &document_information);
    StartPage(context);

    /* sets the map mode of the document to twips */
    SetMapMode(context, MM_TWIPS);

    /* creates a new (drawing) pen for the document to
    be used in the drawing process of it */
    HANDLE pen = CreatePen(0, FONT_SCALE_FACTOR, 0);
    SelectObject(context, pen);

    /* retrieves the initial document element  header */
    struct element_header_t *element_header = (struct element_header_t *) (buffer + sizeof(struct document_header_t));

    /* retrieves the horizontal and vertical resolution and pixel
    density capabilities */
    int horizontal_resolution = GetDeviceCaps(context, HORZRES);
    int vertical_resolution = GetDeviceCaps(context, VERTRES);
    int vertical_size = GetDeviceCaps(context, VERTSIZE);
    int pixel_density = GetDeviceCaps(context, LOGPIXELSY);

    /* start the current page value at the initial value */
    int current_page = 0;

    /* iterates over the element count in the document to
    process it and generate the correct print instructions */
    for(size_t index = 0; index < document_header->element_count; index++) {
        /* retrieves the type and length of the element */
        unsigned short element_type = element_header->type;
        unsigned int element_length = element_header->length;

        /* allocates space for the various elements to be used
        along the switch instruction */
        SIZE text_size;
        RECT clip_box;
        HFONT font;
        int result;
        int text_x;
        int text_y;
        int text_y_bottom;
        double text_y_bottom_millimeter;
        int weight;
        char *text;
        char *image;
        wchar_t *text_unicode;
        struct text_element_header_t *text_element_header;
        struct image_element_header_t *image_element_header;
        char directory_buffer[1024];
        char path[MAX_PATH];
        int image_x;
        int image_y;
        int image_y_bottom;
        double image_y_bottom_millimeter;
        int previous_mode;
        double divisor;
        double multiplier;
        HDC image_context;
        HBITMAP handle_image;
        HANDLE handle_image_new;
        FILE *image_file;
        BITMAP bitmap;
        float scaled_width;
        float scaled_height;
        int new_page;
        double page_size_twips;

        /* switches over the element type to generate the
        appropriate print instructions */
        switch(element_type) {
            case TEXT_VALUE:
                /* "casts" the element header as text element header an retrieves
                the text part from it, then sets the default text weight as normal */
                text_element_header = (struct text_element_header_t *) element_header;
                text = (char *) text_element_header + sizeof(struct text_element_header_t);
                weight = FW_DONTCARE;

                /* in case the text weight is greater than zero it's
                considered to be bold sized */
                if(text_element_header->text_weight > 0) { weight = FW_BOLD; }

                /* creates the correct front to display the current text
                instruction, and then selects it */
                font = CreateFont(
                    text_element_header->font_size * FONT_SCALE_FACTOR,
                    0,
                    0,
                    0,
                    weight,
                    text_element_header->text_italic,
                    FALSE,
                    FALSE,
                    DEFAULT_CHARSET,
                    OUT_OUTLINE_PRECIS,
                    CLIP_DEFAULT_PRECIS,
                    CLEARTYPE_QUALITY,
                    VARIABLE_PITCH,
                    text_element_header->font
                );
                SelectObject(context, font);

                /* converts the text into the appropriate windows unicode
                representation (may represent all charset) */
                text_unicode = new wchar_t[lstrlen(text) + 1];
                result = MultiByteToWideChar(CP_UTF8, NULL, text, -1, text_unicode, lstrlen(text) + 1);

                /* retrieves the extension (size) of the text for the current
                font using the current settings */
                GetTextExtentPointW(context, text_unicode, lstrlenW(text_unicode), &text_size);
                GetClipBox(context, &clip_box);

                /* calculates the text initial x position (deducting the margins)
                and using the current font scale factor */
                text_x = (text_element_header->margin_left - text_element_header->margin_right) * FONT_SCALE_FACTOR;

                /* in case the text align is left */
                if(text_element_header->text_align == LEFT_TEXT_ALIGN_VALUE) {
                    text_x += 0;
                }
                /* in case the text align is right */
                else if(text_element_header->text_align == RIGHT_TEXT_ALIGN_VALUE) {
                    text_x += clip_box.right - text_size.cx;
                }
                /* in case the text align is left */
                else if(text_element_header->text_align == CENTER_TEXT_ALIGN_VALUE) {
                    text_x += clip_box.right / 2 - text_size.cx / 2;
                }

                /* sets the text y as the current position context y */
                text_y = text_element_header->position.y;

                /* calculates the y position for the bottom position of the
                text and then converts it into a milimiter type */
                text_y_bottom = text_y - text_size.cy;
                text_y_bottom_millimeter = (double) text_y_bottom / TWIPS_PER_INCH * -1.0 * MM_PER_INCH;

                /* uses the bottom position of the text in milimiters and
                divides (integer division) it over the page size to check
                the current page number (index) */
                new_page = (int) (text_y_bottom_millimeter / vertical_size);

                /* checks if there is a new page for writing, in case
                there is a new page must be "constructed" */
                if(new_page != current_page) {
                    /* ends the current page and starts a new
                    on (page break operation) */
                    EndPage(context);
                    StartPage(context);

                    /* updates the current page variable with
                    the new page value */
                    current_page = new_page;
                }

                /* calculates the size of the page size in twips units
                and uses it to re-calculate the text y position, taking
                into account the already "used" pages (modulus) */
                page_size_twips = (vertical_size / MM_PER_INCH * TWIPS_PER_INCH);
                text_y += (int) ((double) new_page * page_size_twips);

                /* resets the text y position in case the value is greater
                than the maximum zero value, otherwise uses the "normal" text
                y position value (default case) */
                text_y = text_y > 0 ? 0 : text_y;

                /* outputs the text to the current drawing context */
                TextOutW(context, text_x, text_y, text_unicode, lstrlenW(text_unicode));

                /* releases the unicode representation of the text */
                delete text_unicode;

                /* breaks the switch */
                break;

            case IMAGE_VALUE:
                /* "casts" the element header as image element header an retrieves
                the image part from it */
                image_element_header = (struct image_element_header_t *) element_header;
                image = (char *) image_element_header + sizeof(struct image_element_header_t);

                /* retrieves the temporary path (directory) and then uses it to
                generate a temporary path for out image file */
                GetTempPath(1024, directory_buffer);
                GetTempFileName(directory_buffer, "default", 0, path);

                /* opens the image file for binary writing, writes the image contents
                to it and then closes the file*/
                fopen_s(&image_file, path, "wb");
                fwrite(image, sizeof(char), image_element_header->length, image_file);
                fclose(image_file);

                /* loads the (bitmap) image from the file and creates the appropriate in
                memory image handler */
                handle_image_new = LoadImage(0, (LPCTSTR) path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
                image_context = CreateCompatibleDC(NULL);
                handle_image = SelectBitmap(image_context, handle_image_new);
                GetObject(handle_image_new, sizeof(bitmap), &bitmap);

                /* removes the temporary image file (it's no longer reuired)`*/
                remove(path);

                /* calculates the pixel divisor (resizing for text mode) and
                calculates the multipler for the image size */
                divisor = TWIPS_PER_INCH / pixel_density;
                multiplier = (double) IMAGE_SCALE_FACTOR / divisor;

                /* in case the text align is left */
                if(image_element_header->text_align == LEFT_TEXT_ALIGN_VALUE) {
                    image_x = 0;
                }
                /* in case the text align is right */
                else if(image_element_header->text_align == RIGHT_TEXT_ALIGN_VALUE) {
                    image_x = horizontal_resolution - bitmap.bmWidth;
                }
                /* in case the text align is left */
                else if(image_element_header->text_align == CENTER_TEXT_ALIGN_VALUE) {
                    image_x = (int) ((float) horizontal_resolution / 2.0) - (int) ((float) bitmap.bmWidth * (float) multiplier / 2.0);
                }

                /* calculates the scaled with and height taking into account the
                "just" calculated multiplier value */
                scaled_width = (float) bitmap.bmWidth * (float) multiplier;
                scaled_height = (float) bitmap.bmHeight * (float) multiplier;

                /* calculates the y position for the bottom position of the
                image and then converts it into a milimiter type */
                image_y_bottom = image_element_header->position.y + (int) (scaled_height * divisor);
                image_y_bottom_millimeter = (double) image_y_bottom / TWIPS_PER_INCH * -1.0 * MM_PER_INCH;

                /* uses the bottom position of the image in milimiters and
                divides (integer division) it over the page size to check
                the current page number (index) */
                new_page = (int) (image_y_bottom_millimeter / vertical_size);

                /* checks if there is a new page for writing, in case
                there is a new page must be "constructed" */
                if(new_page != current_page) {
                    /* ends the current page and starts a new
                    on (page break operation) */
                    EndPage(context);
                    StartPage(context);

                    /* updates the current page variable with
                    the new page value */
                    current_page = new_page;
                }

                /* calculates the size of the page size in twips units
                and uses it to re-calculate the image y position, taking
                into account the already "used" pages (modulus) */
                page_size_twips = (vertical_size / MM_PER_INCH * TWIPS_PER_INCH);
                image_y = (int) ((double) image_element_header->position.y + ((double) new_page * page_size_twips));

                /* resets the image y position in case the value is greater
                than the maximum zero value, otherwise uses the "normal" image
                y position value (default case) */
                image_y = image_y > 0 ? 0 : image_y;

                /* sets the image y as the current position context y using
                the divisor for text mode scale */
                image_y = (int) ((double) image_y / divisor) * -1;

                /* switches the map mode to text (pixel oriented) and writes
                the image into the current context, then switches back to the
                previous map mode */
                previous_mode = GetMapMode(context);
                SetMapMode(context, MM_TEXT);
                StretchBlt(context, image_x, image_y, (int) scaled_width, (int) scaled_height, image_context, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
                SetMapMode(context, previous_mode);

                /* selectes the bitmap for the context and then deletes the
                "just" generated drawing context */
                SelectBitmap(image_context, handle_image);
                DeleteDC(image_context);

                /* breaks the switch */
                break;
        }

        /* retrieves te next element header, taking into account the size
        of the current element (this is the increment delta to be used) */
        element_header = (struct element_header_t *) ((char *) element_header + sizeof(struct element_header_t) + element_length);
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

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

#pragma once

/**
 * The default size to be used in the creation
 * of internal string buffers.
 */
#define BUFFER_SIZE 1024

/**
 * The coeficient for conversion between
 * millimeter and inch.
 */
#define MM_PER_INCH 25.4

/**
 * The coeficient for conversion between
 * twips and inch.
 */
#define TWIPS_PER_INCH 1440.0

/**
 * The scale factor to be used to resize the
 * fonts into the actual size to be used in
 * the document (for printing).
 */
#define FONT_SCALE_FACTOR 20

/**
 * The scale factor to be used when resizing
 * the image to display it in the oducment.
 */
#define IMAGE_SCALE_FACTOR 10

/**
 * The various element type values to be used
 * for the processing.
 */
#define TEXT_VALUE 1
#define IMAGE_VALUE 2

/**
 * The various alignment values to be used in
 * the display of the text.
 */
#define LEFT_TEXT_ALIGN_VALUE 1
#define RIGHT_TEXT_ALIGN_VALUE 2
#define CENTER_TEXT_ALIGN_VALUE 3

/**
 * Structure defining a position int a two axis
 * based system.
 */
typedef struct position_t {
    int x;
    int y;
} position;

/**
 * Structure describing the header of an element
 * of the binie file.
 */
typedef struct element_header_t {
    unsigned int type;
    unsigned int length;
} element_header;

/**
 * Structure describing an element in the binie
 * file.
 */
typedef struct element_t {
    struct element_header_t header;
    void *contents;
} element;

/**
 * Structure describing the header of a text element
 * of the binie file.
 */
typedef struct text_element_header_t {
    struct element_header_t header;
    struct position_t position;
    char font[256];
    unsigned int font_size;
    unsigned int text_align;
    unsigned int text_weight;
    unsigned int text_italic;
    unsigned int margin_left;
    unsigned int margin_right;
    unsigned int position_x;
    unsigned int position_y;
    unsigned int block_width;
    unsigned int block_height;
    unsigned int length;
} text_element_header;

/**
 * Structure describing a text element in the binie
 * file.
 */
typedef struct text_element_t {
    struct text_element_header_t header;
    char *text;
} text_element;

/**
 * Structure describing the header of an image element
 * of the binie file.
 */
typedef struct image_element_header_t {
    struct element_header_t header;
    struct position_t position;
    unsigned int text_align;
    unsigned int position_x;
    unsigned int position_y;
    unsigned int block_width;
    unsigned int block_height;
    unsigned int length;
} image_element_header;

/**
 * Structure describing an image element in the binie
 * file.
 */
typedef struct image_element_t {
    struct text_element_header_t header;
    char *data;
} image_element;

/**
 * Structure describing the header of the binie document.
 */
typedef struct document_header_t {
    char title[256];
    unsigned int width;
    unsigned int height;
    unsigned int element_count;
} document_header;

/**
 * Structure describing the binie document.
 */
typedef struct document_t {
    struct document_header_t header;
    char *elements;
} document;

/**
 * Retrieves the context handle to the default printer
 * This function is useful for situations where no print
 * dialog is wanted (fast printing).
 *
 * @param width Optional parameter that allows the retrieved
 * context to be configured with the desired width (in decimal
 * parts of millimiter).
 * @param height Optional parameter that allows the retrieved
 * context to be configured with the desired height (in decimal
 * parts of millimiter).
 * @return The handle for the the drawing context of the
 * default printer.
 */
COLONY_EXPORT_PREFIX HDC get_default_printer(int width, int height);

/**
 * Shows the printing dialog for printer selction, the return
 * value reflects the success of the calling.
 *
 * @param print_dialog_pointer A pointer to the print dialog
 * structure describing the configuration for the dialog box.
 * @return A boolean with the success result of the print dialog.
 */
COLONY_EXPORT_PREFIX BOOL show_print_dialog(PRINTDLG *print_dialog_pointer);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Prints a document using the default printer or in case the show
 * dialog option is set using the printer selected in the printing
 * dialog.
 *
 * The provided data buffer must be encoded using the binie sbinary
 * file specification, that specifies a series of basic printing
 * primitive routines.
 *
 * @param show_dialog If the printing dialog should be display for
 * selection of the printer.
 * @param data The buffer of data encoded in binie format describing
 * the document to be printed.
 * @return The result of the printing processm if success a value
 * greater than zero should be returned.
 */
COLONY_EXPORT_PREFIX int print(bool show_dialog, char *data);

#ifdef __cplusplus
}
#endif

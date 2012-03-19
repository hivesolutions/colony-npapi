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

/**
 * The default size to be used in the creation
 * of internal string buffers.
 */
#define BUFFER_SIZE 1024

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
typedef struct Position_t {
	int x;
	int y;
} Position;

/**
 * Structure describing the header of an element
 * of the binie file.
 */
typedef struct ElementHeader_t {
	unsigned int type;
	unsigned int length;
} ElementHeader;

/**
 * Structure describing an element in the binie
 * file.
 */
typedef struct Element_t {
	struct ElementHeader_t header;
	void *contents;
} Element;

/**
 * Structure describing the header of a text element
 * of the binie file.
 */
typedef struct TextElementHeader_t {
	struct ElementHeader_t header;
	struct Position_t position;
    char font[256];
    unsigned int fontSize;
	unsigned int textAlign;
	unsigned int textWeight;
	unsigned int textItalic;
	unsigned int marginLeft;
	unsigned int marginRight;
    unsigned int length;
} TextElementHeader;

/**
 * Structure describing a text element in the binie
 * file.
 */
typedef struct TextElement_t {
	struct TextElementHeader_t header;
	char *text;
} TextElement;

/**
 * Structure describing the header of an image element
 * of the binie file.
 */
typedef struct ImageElementHeader_t {
	struct ElementHeader_t header;
	struct Position_t position;
	unsigned int textAlign;
    unsigned int length;
} ImageElementHeader;

/**
 * Structure describing an image element in the binie
 * file.
 */
typedef struct ImageElement_t {
	struct TextElementHeader_t header;
	char *data;
} ImageElement;

/**
 * Structure describing the header of the binie document.
 */
typedef struct DocumentHeader_t {
	char title[256];
	unsigned int elementCount;
} DocumentHeader;

/**
 * Structure describing the binie document.
 */
typedef struct Document_t {
	struct DocumentHeader_t header;
	char *elements;
} Document;

HDC getDefaultPrinter();
BOOL showPrintDialog(PRINTDLG *printDialogPointer);
int print(bool showDialog, char *data);

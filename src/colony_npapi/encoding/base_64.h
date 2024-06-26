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

#pragma once

/**
 * The padding character for base 64.
 */
#define PADDING_BASE_64 '='

/**
 * Encodes into base 64 the buffer with the given length into a new buffer
 * to be created.
 *
 * @param buffer The buffer to be encoded into base 64.
 * @param buffer_length The length of the buffer to be encoded into base64.
 * @param encoded_buffer_pointer The pointer to the created encoded buffer.
 * @param encoded_buffer_length_pointer The length of the created encoded buffer.
 * @return The execution status.
 */
COLONY_EXPORT_PREFIX int encode_base64(
    unsigned char *buffer,
    size_t buffer_length,
    unsigned char **encoded_buffer_pointer,
    size_t *encoded_buffer_length_pointer
);

/**
 * Decodes from base 64 the encoded buffer with the given length into a new (decoded) buffer
 * to be created.
 *
 * @param encoded_buffer The encoded buffer to be decoded from base 64.
 * @param encoded_buffer_length The length of the encoded buffer to be decoded from base64.
 * @param decoded_buffer_pointer The pointer to the created (decoder) buffer.
 * @param decoded_buffer_length_pointer The length of the created (decoder) buffer.
 * @return The execution status.
 */
COLONY_EXPORT_PREFIX int decode_base64(
    unsigned char *encoded_buffer,
    size_t encoded_buffer_length,
    unsigned char **decoded_buffer_pointer,
    size_t *decoded_buffer_length_pointer
);

/**
 * Calculates the encoded buffer length from the
 * given (decoded) buffer length.
 *
 * @param buffer_length The length of the (decoded) buffer.
 * @return The length of the encoded buffer.
 */
COLONY_EXPORT_PREFIX size_t calculate_encoded_buffer_length_base64(size_t buffer_length);

/**
 * Calculates the decoded buffer length from the
 * given encoded buffer length.
 *
 * @param encoded_buffer_length The length of the encoded buffer.
 * @param padding_count The ammount of padding in the base 64 encoded string.
 * @return The length of the decoded buffer.
 */
COLONY_EXPORT_PREFIX size_t calculate_decoded_buffer_length_base64(
    size_t encoded_buffer_length,
    size_t padding_count
);

/**
 * Encodes the given buffer into base64.
 * This function is for internal use.
 *
 * @param buffer The buffer to be encoded.
 * @param buffer_length The length of the buffer to be encoded.
 * @param encoded_buffer The target encoded buffer.
 * @param encoded_buffer_length The target encoded buffer length.
 * @return The execution status.
 */
COLONY_EXPORT_PREFIX int _encode_base64(
    unsigned char *buffer, size_t buffer_length,
    unsigned char *encoded_buffer, size_t encoded_buffer_length
);

/**
 * Decodes the given encoded buffer from base64.
 * This function is for internal use.
 *
 * @param encoded_buffer The encoded buffer to be decoded.
 * @param encoded_buffer_length The length of the encoded buffer to be decoded.
 * @param buffer The target buffer.
 * @param buffer_length The target buffer length.
 * @param padding_count The ammount of padding in the base 64 encoded string.
 * @return The execution status.
 */
COLONY_EXPORT_PREFIX int _decode_base64(
    unsigned char *encoded_buffer,
    size_t encoded_buffer_length,
    unsigned char *buffer, size_t buffer_length,
    size_t padding_count
);

/**
 * Allocates a new encoded buffer.
 * This function is for internal use.
 *
 * @param buffer_length The size of the buffer to be encoded.
 * @param encoded_buffer_pointer The pointer to the encoded buffer to be created.
 * @param encoded_buffer_length_pointer The length of the encoded buffer to be created.
 * @return The execution status.
 */
COLONY_EXPORT_PREFIX int _allocate_encoded_buffer(
    size_t buffer_length,
    unsigned char **encoded_buffer_pointer,
    size_t *encoded_buffer_length_pointer
);

/**
 * Allocates a new decoded buffer.
 * This function is for internal use.
 *
 * @param encoded_buffer_length The (encoded) buffer to be decoded.
 * @param decoded_buffer_pointer The pointer to the decoded buffer to be created.
 * @param decoded_buffer_length_pointer The length of the decoded buffer to be created.
 * @param padding_count The ammount of padding to be used.
 * @return The execution status.
 */
COLONY_EXPORT_PREFIX int _allocate_decoded_buffer(
    size_t encoded_buffer_length,
    unsigned char **decoded_buffer_pointer,
    size_t *decoded_buffer_length_pointer,
    size_t padding_count
);

/**
 * Retrieves the padding count for the given encoded buffer
 * and using the given encoded buffer length.
 *
 * @param encoded_buffer The encoded buffer to measure the padding.
 * @param encoded_buffer_length The lenght of the encoded buffer.
 * @return The padding count for the given encoded buffer.
 */
COLONY_EXPORT_PREFIX unsigned int _get_padding_count(
    unsigned char *encoded_buffer,
    size_t encoded_buffer_length
);

/**
 * Looks up the given value in the reverse
 * base 64 table and returns the value.
 *
 * @param value The value to obtain the reverse value.
 * @return The reverse value (result).
 */
COLONY_EXPORT_PREFIX unsigned char _lookup_base64(unsigned char value);

/**
 * Looks up the given value in the reverse
 * base 64 table and returns the value.
 * This method is much faster than the normal
 * implementation
 *
 * @param value The value to obtain the reverse value.
 * @return The reverse value (result).
 */
COLONY_EXPORT_PREFIX unsigned char _lookup_fast_base64(unsigned char value);

/**
 * Releases the created buffers resulting from
 * then encoding or decoding from and into base 64.
 *
 * @param pointer The pointer to the buffer allocated
 * by the base 64 structure.
 */
COLONY_EXPORT_PREFIX void _free_base64(unsigned char *pointer);

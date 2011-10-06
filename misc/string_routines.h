/* string_routines.h
 *
 * Copyright (c) 2011 SITIS <info@sitis-mobile.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details. 
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301  USA.
 */

#ifndef __STRING_ROUTINES_H__
#define __STRING_ROUTINES_H__

#include <stddef.h>

void hex_string_to_hex_bin(char *str, char *hex_buf, int hex_buf_size);
void hex_bin_to_hex_string(char *hex_buf, int hex_buf_size, char *str, int str_size);
void hex_string_to_hex_bin_unicode(wchar_t *str, char *hex_buf, int hex_buf_size);
//void hex_bin_to_hex_string_unicode(char *hex_buf, int hex_buf_size, char *str, int str_size);

#endif
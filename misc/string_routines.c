/* string_routines.c
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

#include "misc/string_routines.h"
#include <memory.h>
#include <string.h>
#include <ctype.h>

void hex_string_to_hex_bin(char *str, char *hex_buf, int hex_buf_size)
{
	int str_i, hex_buf_i;
	char hi, lo;
	char *pstr = NULL, *phex_buf = NULL;

	memset(hex_buf, 0, hex_buf_size);
	str_i = strlen(str) - 1;
	pstr = str + str_i;
	hex_buf_i = hex_buf_size - 1;
	phex_buf = hex_buf + hex_buf_i;
	for (; str_i >= 0; str_i-= 2)
	{
		lo = *pstr--;
		hi = '0';
		if (str_i - 1 >= 0)
			hi = *pstr--;
		if (isdigit(lo) && isdigit(hi))
			*phex_buf-- = ((hi - '0') << 4 ) | ((lo - '0') & 0x0f);
	}
}
void hex_bin_to_hex_string(char *hex_buf, int hex_buf_size, char *str, int str_size)
{
	int str_i, hex_buf_i;
	char hi, lo, first = 1;
	char *pstr = NULL, *phex_buf = NULL;

	memset(str, 0, str_size);
	pstr = str;
	hex_buf_i = 0;
	phex_buf = hex_buf;
	while (*phex_buf == 0 && hex_buf_i++ < hex_buf_size)
		phex_buf++;

	str_i = 0;
	for (; hex_buf_i < hex_buf_size && (str_i - 1 < str_size); str_i++, hex_buf_i++)
	{
		lo = *phex_buf++;

		hi = (lo >> 4) & 0x0f;
		lo = lo & 0x0f;
		if (first == 0 || hi > 0)
		{
			*pstr++ = hi + '0';
			str_i++;
			first = 0;
		}
		*pstr++ = lo + '0';
	}
}

void hex_string_to_hex_bin_unicode(wchar_t *str, char *hex_buf, int hex_buf_size)
{
	int str_i, hex_buf_i;
	wchar_t hi, lo;
	wchar_t *pstr = NULL;
	char *phex_buf = NULL;

	memset(hex_buf, 0, hex_buf_size);
	str_i = wcslen(str) - 1;
	pstr = str + str_i;
	hex_buf_i = hex_buf_size - 1;
	phex_buf = hex_buf + hex_buf_i;
	for (; str_i >= 0; str_i-= 2)
	{
		lo = *pstr--;
		hi = '0';
		if (str_i - 1 >= 0)
			hi = *pstr--;
		if (iswxdigit(lo) && iswxdigit(hi))
			*phex_buf-- = ((hi - '0') << 4 ) | ((lo - '0') & 0x0f);
	}
}

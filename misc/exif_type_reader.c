/* exif_type_reader.c
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

#include "exif_type_reader.h"
#include <stdlib.h>
#include <memory.h>

void exif_mem_read(void *data, ExifByte *exist, ExifContent *content, ExifTag tag, size_t max_size)
{
	ExifEntry *entry = NULL;
	entry = exif_content_get_entry(content, tag);
	if (entry != NULL)
		memcpy(data, entry->data, MIN(entry->size, max_size));
	if (exist != NULL)
		*exist = entry != NULL;
}
void exif_short_read(ExifShort *data, ExifByte *exist, ExifContent *content, ExifTag tag, ExifByteOrder order)
{
	ExifEntry *entry = NULL;
	entry = exif_content_get_entry(content, tag);
	if (entry != NULL)
		*data = exif_get_short(entry->data, order);
	if (exist != NULL)
		*exist = entry != NULL;
}
void exif_long_read(ExifLong *data, ExifByte *exist, ExifContent *content, ExifTag tag, ExifByteOrder order)
{
	ExifEntry *entry = NULL;
	entry = exif_content_get_entry(content, tag);
	if (entry != NULL)
		*data = exif_get_long(entry->data, order);
	if (exist != NULL)
		*exist = entry != NULL;
}
void exif_srational_read(ExifSRational *data, ExifByte *exist, ExifContent *content, ExifTag tag, ExifByteOrder order)
{
	ExifEntry *entry = NULL;
	entry = exif_content_get_entry(content, tag);
	if (entry != NULL)
		*data = exif_get_srational(entry->data, order);
	if (exist != NULL)
		*exist = entry != NULL;
}
void exif_rational_read(ExifRational *data, ExifByte *exist, ExifContent *content, ExifTag tag, ExifByteOrder order)
{
	ExifEntry *entry = NULL;
	entry = exif_content_get_entry(content, tag);
	if (entry != NULL)
		*data = exif_get_rational(entry->data, order);
	if (exist != NULL)
		*exist = entry != NULL;
}
void exif_rational_to_double_read(double *dec, ExifByte *exist, ExifContent *content, ExifTag tag, ExifByteOrder order)
{
	ExifRational rational;
	ExifEntry *entry = NULL;
	entry = exif_content_get_entry(content, tag);
	if (exist != NULL)
		*exist = 0;
	if (entry) 	
	{
		rational = exif_get_rational(entry->data, order);
		if (rational.denominator != 0)
		{
			*dec = (double)rational.numerator / rational.denominator;
			if (exist != NULL)
				*exist = 1;
		}
	}
}
void exif_rational_dms_to_double_degrees_read(double *dec, ExifByte *exist, ExifContent *content, ExifTag tag, ExifByteOrder order)
{
	ExifRational Degrees, Minutes, Seconds;
	ExifEntry *entry = NULL;
	entry = exif_content_get_entry(content, tag);
	if (exist != NULL)
		*exist = 0;
	if (entry) 	
	{
		Degrees = exif_get_rational(entry->data, order);
		Minutes = exif_get_rational(entry->data + 0x08, order);
		Seconds = exif_get_rational(entry->data + 0x10, order);
		if (Degrees.denominator != 0 && Minutes.denominator != 0 && Seconds.denominator != 0)
		{
			*dec = decimal(
				(int)((double)Degrees.numerator / Degrees.denominator),
				(int)((double)Minutes.numerator / Minutes.denominator),
				(double)Seconds.numerator / Seconds.denominator);
			if (exist != NULL)
				*exist = 1;
		}
	}
}
void exif_rational_hms_to_int_seconds_read(int *dec, ExifByte *exist, ExifContent *content, ExifTag tag, ExifByteOrder order)
{
	ExifRational Degrees, Minutes, Seconds;
	ExifEntry *entry = NULL;
	entry = exif_content_get_entry(content, tag);
	if (exist != NULL)
		*exist = 0;
	if (entry) 	
	{
		Degrees = exif_get_rational(entry->data, order);
		Minutes = exif_get_rational(entry->data + 0x08, order);
		Seconds = exif_get_rational(entry->data + 0x10, order);
		if (Degrees.denominator != 0 && Minutes.denominator != 0 && Seconds.denominator != 0)
		{
			*dec = (int)
				(((double)Degrees.numerator / Degrees.denominator) * 3600 +
				((double)Minutes.numerator / Minutes.denominator) * 60 + 
				((double)Seconds.numerator / Seconds.denominator));
			if (exist != NULL)
				*exist = 1;
		}
	}
}
ExifShort *read_unicode_string_from_exif(ExifContent *content, ExifTag stag, ExifShort *str, size_t max_lenght)
{
	ExifEntry *entry = NULL;
	ExifShort size;
	entry = exif_content_get_entry (content, (ExifTag)stag);
	if (entry)
	{
		size = MIN(entry->size, max_lenght * sizeof(*str));
		memcpy(str, entry->data, size);
		if (size < max_lenght)
			str[size] = 0; // null terminated
		else
			str[size - 1] = 0; // null terminated
	}
	return str;
}
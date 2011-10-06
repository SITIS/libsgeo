/* exif_type_writer.c
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

#include "exif_type_writer.h"
#include <stdlib.h>
#include "misc/sgeo-tag.h"
#include <memory.h>
#include <string.h>

void exif_mem_write(ExifContent *content, ExifTag tag, ExifFormat format, void *data, size_t size, ExifByte *exist)
{
	ExifEntry *entry = NULL;
	if (exist != NULL && *exist == 0)
		return;

	entry = exif_content_get_entry(content, tag);
	if (entry == NULL)
		entry = create_tag_in_content(content, tag, size, format);
	memcpy(entry->data, data, size);
}

void exif_short_write(ExifContent *content, ExifTag tag, ExifShort *data, ExifByteOrder order, ExifByte *exist)
{
	ExifEntry *entry = NULL;
	if (exist != NULL && *exist == 0)
		return;

	entry = exif_content_get_entry(content, tag);
	if (entry == NULL)
		entry = create_tag_in_content(content, tag, 1, EXIF_FORMAT_SHORT);
	exif_set_short(entry->data, order, *data);
}

void exif_long_write(ExifContent *content, ExifTag tag, ExifLong *data, ExifByteOrder order, ExifByte *exist)
{
	ExifEntry *entry = NULL;
	if (exist != NULL && *exist == 0)
		return;

	entry = exif_content_get_entry(content, tag);
	if (entry == NULL)
		entry = create_tag_in_content(content, tag, 1, EXIF_FORMAT_LONG);
	exif_set_long(entry->data, order, *data);
}

void exif_rational_write(ExifContent *content, ExifTag tag, ExifRational *data, ExifByteOrder order, ExifByte *exist)
{
	ExifEntry *entry = NULL;
	if (exist != NULL && *exist == 0)
		return;

	entry = exif_content_get_entry(content, tag);
	if (entry == NULL)
		entry = create_tag_in_content(content, tag, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, *data);
}
void exif_srational_write(ExifContent *content, ExifTag tag, ExifSRational *data, ExifByteOrder order, ExifByte *exist)
{
	ExifEntry *entry = NULL;
	if (exist != NULL && *exist == 0)
		return;

	entry = exif_content_get_entry(content, tag);
	if (entry == NULL)
		entry = create_tag_in_content(content, tag, 1, EXIF_FORMAT_SRATIONAL);
	exif_set_srational(entry->data, order, *data);
}
void exif_double_to_rational_write(ExifContent *content, ExifTag tag, double *data, ExifByteOrder order, ExifByte *exist)
{
	ExifEntry *entry = NULL;
	if (exist != NULL && *exist == 0)
		return;

	entry = exif_content_get_entry(content, tag);
	if (entry == NULL)
		entry = create_tag_in_content(content, tag, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, DoubleToRational(*data));
}
void exif_double_degrees_to_rational_dms_write(ExifContent *content, ExifTag tag, double *data, ExifByteOrder order, ExifByte *exist)

{
	ExifRational deg, min, sec;
	ExifEntry *entry = NULL;
	if (exist != NULL && *exist == 0)
		return;

	entry = exif_content_get_entry(content, tag);
	if (entry == NULL)
		entry = create_tag_in_content(content, tag, 3, EXIF_FORMAT_RATIONAL);

	deg = DoubleToRational( degrees(*data) );
	min = DoubleToRational( minutes(*data) );
	sec = DoubleToRational( seconds(*data) );

	exif_set_rational(entry->data, order, deg);
	exif_set_rational(entry->data + 0x08, order, min);
	exif_set_rational(entry->data + 0x10, order, sec);
}
void exif_int_seconds_to_rational_hms_write(ExifContent *content, ExifTag tag, int *data, ExifByteOrder order, ExifByte *exist)
{
	ExifRational time_hours, time_minutes, time_seconds;
	int nHour, nMin;
	ExifEntry *entry = NULL;
	if (exist != NULL && *exist == 0)
		return;

	entry = exif_content_get_entry(content, tag);
	if (entry == NULL)
		entry = create_tag_in_content(content, tag, 3, EXIF_FORMAT_RATIONAL);

	nHour = *data / 3600;
	time_hours = DoubleToRational(nHour);
	nMin = (*data - nHour * 3600) / 60;
	time_minutes = DoubleToRational(nMin);
	time_seconds = DoubleToRational(*data - nHour * 3600 - nMin * 60);

	exif_set_rational(entry->data, order, time_hours);
	exif_set_rational(entry->data + 0x08, order, time_minutes);
	exif_set_rational(entry->data + 0x10, order, time_seconds);
}

void write_unicode_string_to_exif(ExifContent *content, ExifTag stag, ExifShort *str)
{
	size_t str_length_in_bytes;
	ExifEntry *entry = NULL;
	if (str != NULL)
	{
		str_length_in_bytes = wcslen((wchar_t* )str) * sizeof(*str);
		if (str_length_in_bytes == 0)
			str_length_in_bytes++;
		entry = exif_content_get_entry(content, (ExifTag)stag);
		if (!entry)
			entry = create_tag_in_content(content, (ExifTag)stag, str_length_in_bytes, EXIF_FORMAT_BYTE);
		memcpy(entry->data, str, str_length_in_bytes);
	}
}

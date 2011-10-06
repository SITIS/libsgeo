/* exif_type_reader.h
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

#ifndef __EXIF_TYPE_READER_H__
#define __EXIF_TYPE_READER_H__

#include "libexif/exif-content.h"

void exif_mem_read(void *data, ExifByte *exist, ExifContent *content, ExifTag tag, size_t max_size);
void exif_short_read(ExifShort *data, ExifByte *exist, ExifContent *content, ExifTag tag, ExifByteOrder order);
void exif_long_read(ExifLong *data, ExifByte *exist, ExifContent *content, ExifTag tag, ExifByteOrder order);
void exif_srational_read(ExifSRational *data, ExifByte *exist, ExifContent *content, ExifTag tag, ExifByteOrder order);
void exif_rational_read(ExifRational *data, ExifByte *exist, ExifContent *content, ExifTag tag, ExifByteOrder order);
// read ExifRational to double
void exif_rational_to_double_read(double *dec, ExifByte *exist, ExifContent *content, ExifTag tag, ExifByteOrder order);
// read three ExifRational to double (in degrees)
void exif_rational_dms_to_double_degrees_read(double *dec, ExifByte *exist, ExifContent *content, ExifTag tag, ExifByteOrder order);
// read three ExifRational to int (in seconds)
void exif_rational_hms_to_int_seconds_read(int *dec, ExifByte *exist, ExifContent *content, ExifTag tag, ExifByteOrder order);
ExifShort *read_unicode_string_from_exif(ExifContent *content, ExifTag stag, ExifShort *str, size_t max_lenght);

#endif
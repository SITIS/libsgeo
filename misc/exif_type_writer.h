/* exif_type_writer.h
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

#ifndef __EXIF_TYPE_WRITER_H__
#define __EXIF_TYPE_WRITER_H__

#include "libexif/exif-content.h"

void exif_mem_write(ExifContent *content, ExifTag tag, ExifFormat format, void *data, size_t size, ExifByte *exist);
void exif_short_write(ExifContent *content, ExifTag tag, ExifShort *data, ExifByteOrder order, ExifByte *exist);
void exif_long_write(ExifContent *content, ExifTag tag, ExifLong *data, ExifByteOrder order, ExifByte *exist);
void exif_rational_write(ExifContent *content, ExifTag tag, ExifRational *data, ExifByteOrder order, ExifByte *exist);
void exif_srational_write(ExifContent *content, ExifTag tag, ExifSRational *data, ExifByteOrder order, ExifByte *exist);
// write double to ExifRational
void exif_double_to_rational_write(ExifContent *content, ExifTag tag, double *data, ExifByteOrder order, ExifByte *exist);
// write double (in degrees) to three ExifRational
void exif_double_degrees_to_rational_dms_write(ExifContent *content, ExifTag tag, double *data, ExifByteOrder order, ExifByte *exist);
// write int (in seconds) to three ExifRational
void exif_int_seconds_to_rational_dms_write(ExifContent *content, ExifTag tag, int *data, ExifByteOrder order, ExifByte *exist);
void write_unicode_string_to_exif(ExifContent *content, ExifTag stag, ExifShort *str);
void exif_int_seconds_to_rational_hms_write(ExifContent *content, ExifTag tag, int *data, ExifByteOrder order, ExifByte *exist);

#endif
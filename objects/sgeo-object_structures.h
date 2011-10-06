/* sgeo-object_structures.h
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

#ifndef _SGEO_OBJECT_STRUCTURES_H_
#define _SGEO_OBJECT_STRUCTURES_H_

#include "libexif/exif-utils.h"

#define DataBlockMagic "DataBlockObjects"

#pragma pack(push, 1)
typedef struct _DataBlockHeader
{
	ExifByte magic[16];
	ExifLong number;
	ExifLong next_number;
	ExifShort first_object_offset;
} DataBlockHeader, *lpDataBlockHeader;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _DataUnitHeader
{
	ExifByte id[20];
	ExifShort size;
} DataUnitHeader, *lpDataUnitHeader;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _DataRun
{
	ExifByte *p;
	ExifLong size;
} DataRun, *lpDataRun;
#pragma pack(pop)

extern DataUnitHeader data_unit_header_null;

#endif
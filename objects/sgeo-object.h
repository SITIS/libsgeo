/* sgeo-object.h
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

#ifndef _SGEO_OBJECT_H_
#define _SGEO_OBJECT_H_

#pragma pack(push, 1)
typedef struct _SGeoObject
{
	ExifByte		UID[20]; // uniqe identificator object
	ExifShort		Type;// object type
	utf16			Name[80];// object name
	ExifShort		GlobalAttr;// attribute of globality
	utf16			Tag[10][30];// 10 tags
	ExifByte		CoordinateSystem;// 1
	ExifSRational	Latitude;// latitude
	ExifRational	LatitudeAccuracy;// latitude accuracy
	ExifSRational	Longitude;// longitude
	ExifRational	LongitudeAccuracy;// longitude accuracy
	ExifSRational	Altitude;// altitude
	ExifRational	AltitudeAccuracy;// altitude accuracy
	char			reserve[128];// reserve for derived objects
	ExifLong		DataSize;
	ExifByte		*Data;
	ExifLong		DataHash;
} SGeoObject;
#pragma pack(pop)
#endif
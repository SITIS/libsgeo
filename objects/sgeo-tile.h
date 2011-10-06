/* sgeo-tile.h
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

#ifndef _SGEO_TILE_H_
#define _SGEO_TILE_H_

#include "sgeo-object.h"

typedef struct _SGeoTile SGeoTile;

struct _SGeoTile
{
	// as _SGeoObject
	ExifByte		UID[20]; // uniqe identificator object
	ExifShort		Type;// object type
	utf16			Name[80];// object name
	ExifShort		GlobalAttr;// attribute of globality
	utf16			Tag[10][60];// 10 tags
	ExifByte		CoordinateSystem;// 1
	ExifSRational	Latitude;// latitude
	ExifRational	LatitudeAccuracy;// latitude accuracy
	ExifSRational	Longitude;// longitude
	ExifRational	LongitudeAccuracy;// longitude accuracy
	ExifSRational	Altitude;// altitude
	ExifRational	AltitudeAccuracy;// altitude accuracy
	//_as _SGeoObject
	
	// 128 byte data
	ExifSRational	LongitudeLeft;	// 8
	ExifSRational	LatitudeTop;	// 8
	ExifSRational	LongitudeRight; // 8
	ExifSRational	LatitudeBottom; // 8
	ExifByte		LogicUnitType;	// 1
	ExifRational	LogicWidth;		// 4
	ExifRational	LogicHeight;	// 4
	ExifByte		PhysicUnitType;	// 1
	ExifRational	PhysicWidth;	// 8
	ExifRational	PhysicHeight;	// 8
	ExifSRational	PhysicLevel;	// 8
	ExifRational	PhysicArea;		// 8
	ExifRational	PhysicScale;	// 8
	ExifByte		ProjectionType; // 1
	ExifByte		ImageType;      // 1


	char reserve[128-76];// reserve for derived objects
	//_128 byte data

	// as _SGeoObject
	ExifLong DataSize;
	ExifByte *Data;
	ExifLong DataHash;
	//_as _SGeoObject
};

#endif
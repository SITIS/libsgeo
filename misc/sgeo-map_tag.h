/* sgeo-map_tag.h
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

#ifndef _SGEO_MAP_TAG_H_
#define _SGEO_MAP_TAG_H_

#include "libexif/exif-utils.h"
#include "libexif/exif-loader.h"
#include "misc/list_exif_loader_struct.h"

typedef enum
{ 
	JPG, PNG, BMP 
} PictureFormat;

typedef enum 
{
	WGS60,
	WGS66,
	WGS72,
	WGS84,
	WGS84Spherical,
	GRS67,
	GRS80,
	NWGL10,
	IERS96,
	SGS85
} CoordinateSystemType;

typedef enum 
{
	LatitudeLongitude,
	Mercator,
	TransverseMercator,
	Stereographic,
	LambertConformalConic,
	ConicalEquidistant,
	MillerCylindrical,
	CylindricalEqualArea
} ProjectionType;

typedef struct
{
	// map version
	ExifByte		VersionID[4];
	// identificator map, useed in application
	ExifLong ID;

	// rectangle coordinas
	ExifSRational TopLeftLongitude;
	ExifSRational TopLeftLatitude;
	ExifSRational TopRightLongitude;
	ExifSRational TopRightLatitude;
	ExifSRational BottomLeftLongitude;
	ExifSRational BottomLeftLatitude;
	ExifSRational BottomRightLongitude;
	ExifSRational BottomRightLatitude;

	// coordinate system type
	// 0 - WGS60,
	// 1 - WGS66,
	// 2 - WGS72,
	// 3 - WGS84,
	// 4 - WGS84Spherical,
	// 5 - GRS67,
	// 6 - GRS80,
	// 7 - NWGL10,
	// 8 - IERS96,
	// 9 - SGS85
	ExifLong CoordinateSystem;
	// projection type
	// 0 - Latitude / Longitude
    // 1 - Mercator
    // 2 - Transverse Mercator (Gauss - Kruger)
    // 3 - Stereographic
    // 4 - Lambert Conformal Conic
    // 5 - Conical equidistant
    // 6 - Miller cylindrical
    // 7 - The cylindrical equal area
	ExifLong Projection;
	// image format
	// 0 - JPG
	// 1 - PNG
	// 2 - BMP 
	ExifLong Format;
	// size data
	ExifShort DataSize;
	// image data
	void *Data;
} MapTags;

typedef struct ListMapTags_tag
{
	struct MapTags *map_tags;
	struct ListMapTags_tag *prev;
	struct ListMapTags_tag *next;
} ListMapTags;
void free_ListExifLoaderItem(ListExifLoader *item);
void free_ListExifLoader(ListExifLoader *list_exif_loader);
int size_ListExifLoader(ListExifLoader *list_exif_loader);
#endif //_SGEO_MAP_TAG_H_
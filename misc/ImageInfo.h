/* ImageInfo.h
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

#ifndef ImageInfoH
#define ImageInfoH

#include <misc/sgeo-tag.h>

#define MAX_COMMENT 2000

#pragma pack(push, 1)
struct ImageInfo
{
	char		CameraMake[32];
	char		CameraModel[40];
	char		DateTimeOriginal[20]; // Date and time of original data generation
	char		DateTimeDigitized[20]; // Date and time of digital data generation
	ExifShort	Orientation;
	ExifRational	FocalLength;
	utf16		Comments[MAX_COMMENT]; // first element - BOM mark for utf16
	size_t		CommentsLength;

	
	ExifByte	ExistGPSDateStamp;
	char		GPSDateStamp[11];		// UTC date and time
	ExifByte	ExistGPSTimeStamp;
	int			GPSTimeStamp; // in seconds
	
	char		GPSLatitudeRef[2];
	ExifByte	ExistGPSLatitude;
	double		GPSLatitude;			// must be >= 0
	
	char		GPSLongitudeRef[2];
	ExifByte	ExistGPSLongitude;
	double		GPSLongitude;		// must be >= 0
	
	ExifByte	GPSAltitudeRef;
	ExifByte	ExistGPSAltitude;
	double		GPSAltitude;			// must be >= 0
	
	char		GPSImgDirectionRef[2];
	ExifByte	ExistGPSImgDirection;
	double		GPSImgDirection;
	
#ifdef _SGEO_FULL
	ExifLong    ImageWidth;
	ExifLong    ImageHeight;
#endif

	// SGEO
	SGeoTags SGeo;
}; 
#pragma pack(pop)

#endif
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

#include <misc/sgeotags.h>

#define MAX_COMMENT 2000

typedef unsigned short utf16;

struct ImageInfo
{
	char CameraMake[32];
	char CameraModel[40];
	char DateTime[20]; 		// DateTimeOriginal tag
	ExifShort Orientation;
	ExifRational FocalLength;
	utf16 Comments[MAX_COMMENT]; // first element - BOM mark for utf16
	size_t CommentsLength;

	
	char GPSDateStamp[11];		// UTC date and time
	int GPSTimeStamp;
	
	char GPSLatitudeRef[2];
	double GPSLatitude;			// must be >= 0
	
	char GPSLongitudeRef[2];
	double GPSLongitude;		// must be >= 0
	
	ExifByte GPSAltitudeRef;
	double GPSAltitude;			// must be >= 0
	
	char GPSImgDirectionRef[2];
	double GPSImgDirection;
	
	// SGEO
	SGeoTags SGeo;
}; 

#endif
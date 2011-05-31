/* sgeotags.h
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

#ifndef _SGEO_TAG_H_
#define _SGEO_TAG_H_

#include <libexif/exif-data.h>
#include <libexif/exif-format.h>

typedef enum {
	SGEO_TAG_VERSIONID = 50001,		// Byte x4
	// Sensor data
	SGEO_TAG_LONGITUDE, 			
	SGEO_TAG_LONGACCURACY,			
	SGEO_TAG_LATITUDE,				
	SGEO_TAG_LATACCURACY,			
	SGEO_TAG_ALTITUDE,				
	SGEO_TAG_ALTACCURACY,			
	SGEO_TAG_AZIMUTH,				
	SGEO_TAG_AZIMUTHACCURACY,		
	SGEO_TAG_PITCH,					
	SGEO_TAG_PITCHACCURACY,			
	SGEO_TAG_ROLL,					
	SGEO_TAG_ROLLACCURACY,			
	SGEO_TAG_HVIEWANGLE,		//			- Horizontal angle of view
	SGEO_TAG_HVIEWANGLEACCURACY,	
	SGEO_TAG_VVIEWANGLE,		//			- Vertical angle of view
	SGEO_TAG_VVIEWANGLEACCURACY,	
	SGEO_TAG_SATCOUNT,			//			- Number of satellites

	// User data
	SGEO_TAG_USER_LONGITUDE,		
	SGEO_TAG_USER_LONGACCURACY, 	
	SGEO_TAG_USER_LATITUDE,			
	SGEO_TAG_USER_LATACCURACY,		
	SGEO_TAG_USER_ALTITUDE,			
	SGEO_TAG_USER_ALTACCURACY,		
	SGEO_TAG_USER_EARTH_LEVEL,		
	SGEO_TAG_USER_AZIMUTH,			
	SGEO_TAG_USER_AZIMUTHACCURACY,	
	SGEO_TAG_USER_PITCH,			
	SGEO_TAG_USER_PITCHACCURACY,	
	SGEO_TAG_USER_ROLL,				
	SGEO_TAG_USER_ROLLACCURACY, 	
	SGEO_TAG_USER_HVIEWANGLE,		
	SGEO_TAG_USER_HVIEWANGLEACCURACY,
	SGEO_TAG_USER_VVIEWANGLE,		
	SGEO_TAG_USER_VVIEWANGLEACCURACY,

	// Resulted data
	SGEO_TAG_RESULT_LONGITUDE,		
	SGEO_TAG_RESULT_LONGACCURACY,	
	SGEO_TAG_RESULT_LATITUDE,		
	SGEO_TAG_RESULT_LATACCURACY,	
	SGEO_TAG_RESULT_ALTITUDE,		
	SGEO_TAG_RESULT_ALTACCURACY,	
	SGEO_TAG_RESULT_AZIMUTH,		
	SGEO_TAG_RESULT_AZIMUTHACCURACY,
	SGEO_TAG_RESULT_PITCH,			
	SGEO_TAG_RESULT_PITCHACCURACY,	
	SGEO_TAG_RESULT_ROLL,			
	SGEO_TAG_RESULT_ROLLACCURACY,	
	SGEO_TAG_RESULT_HVIEWANGLE, 	
	SGEO_TAG_RESULT_HVIEWANGLEACCURACY,
	SGEO_TAG_RESULT_VVIEWANGLE,		
	SGEO_TAG_RESULT_VVIEWANGLEACCURACY
} SGeoTag;

typedef struct _SGeoTags SGeoTags;

struct _SGeoTags
{
	ExifByte		VersionID[4];
	ExifSRational	Longitude;
	ExifRational	LongitudeAccuracy;
	ExifSRational	Latitude;
	ExifRational	LatitudeAccuracy;
	ExifSRational	Altitude;
	ExifRational	AltitudeAccuracy;
	ExifSRational	Azimuth;
	ExifRational	AzimuthAccuracy;
	ExifSRational	Pitch;
	ExifRational	PitchAccuracy;
	ExifSRational	Roll;
	ExifRational	RollAccuracy;
	ExifRational	HViewAngle;
	ExifRational	HViewAngleAccuracy;
	ExifRational	VViewAngle;
	ExifRational	VViewAngleAccuracy;
	ExifByte		SatCount;

	ExifSRational	UserLongitude;
	ExifRational	UserLongitudeAccuracy;
	ExifSRational	UserLatitude;
	ExifRational	UserLatitudeAccuracy;
	ExifSRational	UserAltitude;
	ExifRational	UserAltitudeAccuracy;
	ExifSRational   UserEarthLevel;
	ExifSRational	UserAzimuth;
	ExifRational	UserAzimuthAccuracy;
	ExifSRational	UserPitch;
	ExifRational	UserPitchAccuracy;
	ExifSRational	UserRoll;
	ExifRational	UserRollAccuracy;
	ExifRational	UserHViewAngle;
	ExifRational	UserHViewAngleAccuracy;
	ExifRational	UserVViewAngle;
	ExifRational	UserVViewAngleAccuracy;

	ExifSRational	ResultLongitude;
	ExifRational	ResultLongitudeAccuracy;
	ExifSRational	ResultLatitude;
	ExifRational	ResultLatitudeAccuracy;
	ExifSRational	ResultAltitude;
	ExifRational	ResultAltitudeAccuracy;
	ExifSRational	ResultAzimuth;
	ExifRational	ResultAzimuthAccuracy;
	ExifSRational	ResultPitch;
	ExifRational	ResultPitchAccuracy;
	ExifSRational	ResultRoll;
	ExifRational	ResultRollAccuracy;
	ExifRational	ResultHViewAngle;
	ExifRational	ResultHViewAngleAccuracy;
	ExifRational	ResultVViewAngle;
	ExifRational	ResultVViewAngleAccuracy;
};

SGeoTags read_sgeo_tags(ExifData *exif, ExifIfd ifd);
void write_sgeo_tags(ExifData *exif, ExifIfd ifd, SGeoTags * collection);

ExifEntry *create_tag(ExifData *exif, ExifIfd ifd, ExifTag tag, size_t len, ExifFormat exifFormat);

#endif //_SGEO_TAG_H_
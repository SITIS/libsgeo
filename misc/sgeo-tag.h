/* sgeo-tag.h
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
#include "libjpeg/jpeg-data.h"
#include "misc/sgeo-map_tag.h"
#include "objects/sgeo-object.h"

#define SGEO_VERSION 7

typedef enum {
	SGEO_TAG_MAP_VERSION = 1,
	SGEO_TAG_MAP_ID,
	SGEO_TAG_MAP_TOP_LEFT_LONGITUDE,
	SGEO_TAG_MAP_TOP_LEFT_LATITUDE,
	SGEO_TAG_MAP_TOP_RIGHT_LONGITUDE,
	SGEO_TAG_MAP_TOP_RIGHT_LATITUDE,
	SGEO_TAG_MAP_BOTTOM_LEFT_LONGITUDE,
	SGEO_TAG_MAP_BOTTOM_LEFT_LATITUDE,
	SGEO_TAG_MAP_BOTTOM_RIGHT_LONGITUDE,
	SGEO_TAG_MAP_BOTTOM_RIGHT_LATITUDE,
	SGEO_TAG_MAP_COORDINATE_SYSTEM,
	SGEO_TAG_MAP_PROJECTION,
	SGEO_TAG_MAP_THUMBNAIL_FORMAT,
	SGEO_TAG_MAP_THUMBNAIL,

	// fields ID encrypt table
	SGEO_TAG_ENCRYPT_VERSION = 1024,
	SGEO_TAG_ENCRYPT_TYPE,
	SGEO_TAG_ENCRYPT_ALGORITHM_VERSION,
	SGEO_TAG_ENCRYPT_PUBLIC_KEY,
	SGEO_TAG_ENCRYPT_TABLE,
	SGEO_TAG_ENCRYPT_FILE_NAME,
	SGEO_TAG_ENCRYPT_FILE_SIZE,

	// object
	SGEO_TAG_OBJECT = 2048,
	SGEO_TAG_OBJECT_TABLE,
	SGEO_TAG_OBJECT_ID,
	SGEO_TAG_OBJECT_NAME,
	SGEO_TAG_OBJECT_GID,
	SGEO_TAG_OBJECT_DATA,

	// information about device
	SGEO_TAG_DEVICE_NAME = 3072,
	SGEO_TAG_DEVICE_IMEI,
	SGEO_TAG_DEVICE_NUMBER,
	SGEO_TAG_DEVICE_OS,
	SGEO_TAG_DEVICE_OS_VERSION,
	SGEO_TAG_DEVICE_VERSION,
	SGEO_TAG_DEVICE_DATE_TIME_MEASURE,

	SGEO_TAG_PROGRAM_NAME = 3584,
	SGEO_TAG_PROGRAM_VERSION,
	SGEO_TAG_PROGRAM_USER_NAME,

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
	SGEO_TAG_RESULT_VVIEWANGLEACCURACY,

	// Sighting
	SGEO_TAG_SIGHTING,

	// global time
	SGEO_TAG_GLOBAL_TIME,

	//GNSS type
	SGEO_TAG_GNSS_TYPE,

	SGEO_TAG_USER_DEVICE_NAME,
	SGEO_TAG_USER_PROGRAM_NAME,		
	SGEO_TAG_USER_USER_NAME
} SGeoTag;


#ifdef _SGEO_FULL
#pragma pack(push, 1)
typedef struct
{
	ExifSRational Angle;
	// bit 0 - Orient (0 - VERTICAL, 1 - HORIZONTAL)
	// bit 1 - bit 7 - unused
	ExifByte Flags;
} SightingTags;
#pragma pack(pop)
#endif

typedef struct _SGeoTagsEncrypt SGeoTagsEncrypt;
#pragma pack(push, 1)
struct _SGeoTagsEncrypt
{
	ExifByte		VersionID[16];

	ExifLong		Type;
	ExifByte		AlgorithmVersion[16];
	ExifByte		PublicKey[128];

	utf16			FileName[32];
	ExifLong		FileSize;
	// exif
	ExifByte		ExistDateTimeOriginal;
	char			DateTimeOriginal[20]; // Date and time of original data generation

	// sensor data
	ExifByte		ExistLongitude;
	ExifSRational	Longitude;

	ExifByte		ExistLongitudeAccuracy;
	ExifRational	LongitudeAccuracy;

	ExifByte		ExistLatitude;
	ExifSRational	Latitude;

	ExifByte		ExistLatitudeAccuracy;
	ExifRational	LatitudeAccuracy;

	ExifByte		ExistAltitude;
	ExifSRational	Altitude;

	ExifByte		ExistAltitudeAccuracy;
	ExifRational	AltitudeAccuracy;

	ExifByte		ExistAzimuth;
	ExifSRational	Azimuth;

	ExifByte		ExistAzimuthAccuracy;
	ExifRational	AzimuthAccuracy;

	ExifByte		ExistPitch;
	ExifSRational	Pitch;

	ExifByte		ExistPitchAccuracy;
	ExifRational	PitchAccuracy;

	ExifByte		ExistRoll;
	ExifSRational	Roll;

	ExifByte		ExistRollAccuracy;
	ExifRational	RollAccuracy;

	ExifByte		ExistHViewAngle;
	ExifRational	HViewAngle;

	ExifByte		ExistHViewAngleAccuracy;
	ExifRational	HViewAngleAccuracy;

	ExifByte		ExistVViewAngle;
	ExifRational	VViewAngle;

	ExifByte		ExistVViewAngleAccuracy;
	ExifRational	VViewAngleAccuracy;

	ExifByte		ExistSatCount;
	ExifByte		SatCount;

	ExifByte		ExistGlobalTime;
	ExifByte		GlobalTime[20];

	ExifLong		GNSSType;

	// strings utf16, terminated two zero bytes
	ExifShort		DeviceName[32];
	ExifShort		DeviceIMEI[32];
	ExifShort		DeviceNumber[32];
	ExifShort		DeviceOS[32];
	ExifShort		DeviceOSVersion[32];
	ExifShort		DeviceVersion[32];
	ExifByte		DeviceDateTimeMeasure[20];

	ExifShort		ProgramName[32];
	ExifShort		ProgramVersion[32];
	ExifShort		ProgramUserName[32];
};
#pragma pack(pop)

typedef struct _SGeoTags SGeoTags;
#pragma pack(push, 1)
struct _SGeoTags
{
	ExifByte		VersionID[4];

	//sensor
	ExifByte		ExistLongitude;
	ExifSRational	Longitude;

	ExifByte		ExistLongitudeAccuracy;
	ExifRational	LongitudeAccuracy;

	ExifByte		ExistLatitude;
	ExifSRational	Latitude;

	ExifByte		ExistLatitudeAccuracy;
	ExifRational	LatitudeAccuracy;

	ExifByte		ExistAltitude;
	ExifSRational	Altitude;

	ExifByte		ExistAltitudeAccuracy;
	ExifRational	AltitudeAccuracy;

	ExifByte		ExistAzimuth;
	ExifSRational	Azimuth;

	ExifByte		ExistAzimuthAccuracy;
	ExifRational	AzimuthAccuracy;

	ExifByte		ExistPitch;
	ExifSRational	Pitch;

	ExifByte		ExistPitchAccuracy;
	ExifRational	PitchAccuracy;

	ExifByte		ExistRoll;
	ExifSRational	Roll;

	ExifByte		ExistRollAccuracy;
	ExifRational	RollAccuracy;

	ExifByte		ExistHViewAngle;
	ExifRational	HViewAngle;

	ExifByte		ExistHViewAngleAccuracy;
	ExifRational	HViewAngleAccuracy;

	ExifByte		ExistVViewAngle;
	ExifRational	VViewAngle;

	ExifByte		ExistVViewAngleAccuracy;
	ExifRational	VViewAngleAccuracy;

	ExifByte		ExistSatCount;
	ExifByte		SatCount;

	ExifByte		ExistGlobalTime;
	ExifByte		GlobalTime[20];

	//user
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
	ExifShort		UserDeviceName[32];
	ExifShort		UserProgramName[32];
	ExifShort		UserUserName[32];

	//result
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

	ExifLong		GNSSType;
	// strings utf16, terminated two zero bytes
	ExifShort		DeviceName[32];
	ExifShort		DeviceIMEI[32];
	ExifShort		DeviceNumber[32];
	ExifShort		DeviceOS[32];
	ExifShort		DeviceOSVersion[32];
	ExifShort		DeviceVersion[32];
	ExifByte		DeviceDateTimeMeasure[20];

	ExifShort		ProgramName[32];
	ExifShort		ProgramVersion[32];
	ExifShort		ProgramUserName[32];


	// object
	//ExifLong		ObjectType;
	//ExifLong		ObjectDataSize;
	//ExifByte		*ObjectData;

#ifdef _SGEO_FULL
	ExifShort UserSightingCount;// sighting count
	SightingTags *UserSightingData;// pointer sighting data
#endif

	// map is a specific object and stored in another marker app2
 //   ExifShort StaticMapCount;// maps count
	//MapTags *StaticMapData;// maps data

	SGeoTagsEncrypt SGeoEncrypt;
	//SGeoObject ObjectsTable[60];// max 39 marshals must be less 64KB
	ExifLong ObjectsTableCount;
	SGeoObject *ObjectsTable;
};
#pragma pack(pop)

SGeoTags read_sgeo_tags(ExifContent *content, ExifByteOrder order);
MapTags *read_sgeo_tags_map(ExifData **exif_array, ExifIfd ifd, ExifShort map_count);
void write_sgeo_tags(ExifData *exif, ExifIfd ifd, SGeoTags * collection);
void write_sgeo_version(unsigned char *data, ExifByteOrder order);
void add_sgeo_app2(JPEGData *jdata, SGeoTags *sgeo_tags, ExifData *exif);
int is_sgeo_app2(unsigned char *data);

ExifEntry *create_tag_in_content(ExifContent *content, ExifTag tag, size_t len, ExifFormat exifFormat);
ExifEntry *create_tag(ExifData *exif, ExifIfd ifd, ExifTag tag, size_t len, ExifFormat exifFormat);
#endif //_SGEO_TAG_H_
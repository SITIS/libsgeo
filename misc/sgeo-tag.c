/* sgeo-tag.c
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

#include "sgeo-tag.h"
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include "misc/sgeo-map_tag.h"
#include "misc/exif_type_reader.h"
#include "misc/exif_type_writer.h"

// very simply data encrypt
unsigned char xorkey[] = {0x1D, 0x6E, 0x44, 0x89, 0x2A, 0x34, 0xED, 0x29, 0x17, 0xE5, 0xF4, 0x28, 0xCF, 0x33, 0xFB, 0xB7};
int get_xorkey_length()
{
	int xorkey_length = sizeof(xorkey);
	while (xorkey_length > 0 && xorkey[xorkey_length - 1] == 0)
		xorkey_length--;
	return xorkey_length;
}
void xorCryptData(unsigned char *data, int size)
{
	//int j;
	int xorkey_length = get_xorkey_length();
	unsigned char *p = data;
	unsigned char *pend = data + size;

	//while (p < pend)
	//{
	//	for (j = 0; j < xorkey_length && p + j < pend; j++)
	//		p[j] ^= xorkey[j];
	//	p += xorkey_length;
	//}
}

void write_content_binary(unsigned char *data, ExifContent *content, ExifByteOrder order, unsigned int offset)
{
	unsigned int i = 0;
	unsigned int next_data = 0;
	ExifEntry *entry = NULL;
	unsigned int s = 0;
	unsigned char *d = data;

	exif_set_short(d, order, content->count);
	d += 2;
	next_data = 2 + offset + content->count * 12;// + 2;
	for (i = 0; i < content->count; i++)
	{
		entry = content->entries[i];
		if (entry != NULL)
		{
			exif_set_short(d + 0x00, order, entry->tag);
			exif_set_short(d + 0x02, order, entry->format);
			exif_set_long (d + 0x04, order, entry->components);
			s = exif_format_get_size(entry->format) * entry->components;
			if (s > 4)
			{
				exif_set_long (d + 0x08, order, next_data + 2*0);// начало смещения данных после APP2 (2 байта)
				memcpy(data + next_data - offset, entry->data, s);
				next_data += s;
			}
			else
			{
				if (entry->tag == SGEO_TAG_VERSIONID)
					write_sgeo_version(d + 0x08, order);
				else
					memcpy(d + 0x08, entry->data, s);
			}
			d = d + 0x0c;
		}
	}
}

void generateDecryptData(SGeoTagsEncrypt *collection, unsigned char *data, unsigned int size, ExifByteOrder order)
{
	ExifEntry *entry = NULL;
	ExifData *exif = NULL;
	ExifContent *content = NULL;
	
	// create new exif data
	exif = exif_data_new_mem (exif_mem_new_default());
	exif->priv->order = order;

	// fill exif fields
	exif_data_load_data_content(exif, EXIF_IFD_ENCRYPT, data, size, 0, 0);
	content = exif->ifd[EXIF_IFD_ENCRYPT];

	// excrypt version
	exif_mem_read(collection->VersionID, NULL, content, SGEO_TAG_ENCRYPT_VERSION, sizeof(collection->VersionID));
	// FileName
	read_unicode_string_from_exif(content, SGEO_TAG_ENCRYPT_FILE_NAME, collection->FileName, sizeof(collection->FileName) / sizeof(*collection->FileName));
	// FileSize
	exif_long_read(&collection->FileSize, NULL, content, SGEO_TAG_ENCRYPT_FILE_SIZE, order);
	// datetime
	exif_mem_read(collection->DateTimeOriginal, &collection->ExistDateTimeOriginal, content, EXIF_TAG_DATE_TIME_ORIGINAL, sizeof(collection->DateTimeOriginal));
	// Longitude
	exif_srational_read(&collection->Longitude, &collection->ExistLongitude, content, SGEO_TAG_LONGITUDE, order);
	// LongAccuracy
	exif_rational_read(&collection->LongitudeAccuracy, &collection->ExistLongitudeAccuracy, content, SGEO_TAG_LONGACCURACY, order);
	// Latitude
	exif_srational_read(&collection->Latitude, &collection->ExistLatitude, content, SGEO_TAG_LATITUDE, order);
	// LatAccuracy
	exif_rational_read(&collection->LatitudeAccuracy, &collection->ExistLatitudeAccuracy, content, SGEO_TAG_LATACCURACY, order);
	// Altitude
	exif_srational_read(&collection->Altitude, &collection->ExistAltitude, content, SGEO_TAG_ALTITUDE, order);
	// AltAccuracy
	exif_rational_read(&collection->AltitudeAccuracy, &collection->ExistAltitudeAccuracy, content, SGEO_TAG_ALTACCURACY, order);
	// Azimuth
	exif_srational_read(&collection->Altitude, &collection->ExistAltitude, content, SGEO_TAG_ALTITUDE, order);
	entry = exif_content_get_entry (content, (ExifTag)(SGEO_TAG_AZIMUTH));
	if (entry) collection->Azimuth = exif_get_srational(entry->data, order);
	// AzimuthAccuracy
	exif_rational_read(&collection->AzimuthAccuracy, &collection->ExistAzimuthAccuracy, content, SGEO_TAG_AZIMUTHACCURACY, order);
	// Pitch
	exif_srational_read(&collection->Pitch, &collection->ExistPitch, content, SGEO_TAG_PITCH, order);
	// PitchAccuracy
	exif_rational_read(&collection->PitchAccuracy, &collection->ExistPitchAccuracy, content, SGEO_TAG_PITCHACCURACY, order);
	// Roll
	exif_srational_read(&collection->Roll, &collection->ExistRoll, content, SGEO_TAG_ROLL, order);
	// RollAccuracy
	exif_rational_read(&collection->RollAccuracy, &collection->ExistRollAccuracy, content, SGEO_TAG_ROLLACCURACY, order);
	// HViewAngle
	exif_rational_read(&collection->HViewAngle, &collection->ExistHViewAngle, content, SGEO_TAG_HVIEWANGLE, order);
	// HViewAngleAccuracy
	exif_rational_read(&collection->HViewAngleAccuracy, &collection->ExistHViewAngleAccuracy, content, SGEO_TAG_HVIEWANGLEACCURACY, order);
	// VViewAngle
	exif_rational_read(&collection->VViewAngle, &collection->ExistVViewAngle, content, SGEO_TAG_VVIEWANGLE, order);
	// VViewAngleAccuracy
	exif_rational_read(&collection->VViewAngleAccuracy, &collection->ExistVViewAngleAccuracy, content, SGEO_TAG_VVIEWANGLEACCURACY, order);
	// SatCount
	exif_mem_read(&collection->SatCount, NULL, content, SGEO_TAG_SATCOUNT, sizeof(collection->SatCount));
	// global time
	exif_mem_read(&collection->GlobalTime, &collection->ExistGlobalTime, content, SGEO_TAG_GLOBAL_TIME, sizeof(collection->GlobalTime));

	// GNSSType
	exif_long_read(&collection->GNSSType, NULL, content, SGEO_TAG_GNSS_TYPE, order);

	// DeviceName
	read_unicode_string_from_exif(content, SGEO_TAG_DEVICE_NAME, collection->DeviceName, sizeof(collection->DeviceName) / sizeof(*collection->DeviceName));
	// DeviceIMEI
	read_unicode_string_from_exif(content, SGEO_TAG_DEVICE_IMEI, collection->DeviceIMEI, sizeof(collection->DeviceIMEI) / sizeof(*collection->DeviceIMEI));
	// DeviceNumber
	read_unicode_string_from_exif(content, SGEO_TAG_DEVICE_NUMBER, collection->DeviceNumber, sizeof(collection->DeviceNumber) / sizeof(*collection->DeviceNumber));
	// DeviceOS
	read_unicode_string_from_exif(content, SGEO_TAG_DEVICE_OS, collection->DeviceOS, sizeof(collection->DeviceOS) / sizeof(*collection->DeviceOS));
	// DeviceOSVersion
	read_unicode_string_from_exif(content, SGEO_TAG_DEVICE_OS_VERSION, collection->DeviceOSVersion, sizeof(collection->DeviceOSVersion) / sizeof(*collection->DeviceOSVersion));
	// DeviceVersion
	read_unicode_string_from_exif(content, SGEO_TAG_DEVICE_VERSION, collection->DeviceVersion, sizeof(collection->DeviceVersion) / sizeof(*collection->DeviceVersion));
	// DeviceDateTimeMeasure
	exif_mem_read(&collection->DeviceDateTimeMeasure, NULL, content, SGEO_TAG_DEVICE_DATE_TIME_MEASURE, sizeof(collection->DeviceDateTimeMeasure));

	// ProgramVersion
	read_unicode_string_from_exif(content, SGEO_TAG_PROGRAM_VERSION, collection->ProgramVersion, sizeof(collection->ProgramVersion) / sizeof(*collection->ProgramVersion));
	// ProgramName
	read_unicode_string_from_exif(content, SGEO_TAG_PROGRAM_NAME, collection->ProgramName, sizeof(collection->ProgramName) / sizeof(*collection->ProgramName));
	// ProgramUserName
	read_unicode_string_from_exif(content, SGEO_TAG_PROGRAM_USER_NAME, collection->ProgramUserName, sizeof(collection->ProgramUserName) / sizeof(*collection->ProgramUserName));

	exif_data_unref(exif);
}
static unsigned char GaloisLfsr(unsigned char lfsr)
{
	// http://kazus.ru/forums/showthread.php?t=19138
	return (lfsr >> 1) ^ (0 - (lfsr & 1u) & 0xb8u);
}
void generateEncryptData(unsigned char **data, unsigned int *size, SGeoTagsEncrypt *collection, ExifByteOrder order)
{
	unsigned int i = 0;
	unsigned int s = 0;
	unsigned int next_data = 0;
	ExifEntry *entry = NULL;
	ExifContent *content = NULL;
	unsigned char *d = NULL, n;
	ExifData *exif = NULL;
	int size_sgeo_bytes = 0;
	ExifIfd ifd = EXIF_IFD_ENCRYPT;

	srand((unsigned int)time(NULL));

	*data = NULL;
	*size = 0;

	exif = exif_data_new_mem (exif_mem_new_default());
	exif->priv->order = order;
	content = exif->ifd[EXIF_IFD_ENCRYPT];

	// VersionID
	exif_mem_write(content, SGEO_TAG_ENCRYPT_VERSION, EXIF_FORMAT_BYTE, collection->VersionID, sizeof(collection->VersionID), NULL);
	// FileName
	write_unicode_string_to_exif(content, SGEO_TAG_ENCRYPT_FILE_NAME, collection->FileName);
	// FileSize
	exif_long_write(content, SGEO_TAG_ENCRYPT_FILE_SIZE, &collection->FileSize, order, NULL);
	// DateTimeOriginal
	exif_mem_write(content, EXIF_TAG_DATE_TIME_ORIGINAL, EXIF_FORMAT_ASCII, collection->DateTimeOriginal, sizeof(collection->DateTimeOriginal), &collection->ExistDateTimeOriginal);
	// Longitude
	exif_srational_write(content, SGEO_TAG_LONGITUDE, &collection->Longitude, order, &collection->ExistLongitude);
	// LongAccuracy
	exif_rational_write(content, SGEO_TAG_LONGACCURACY, &collection->LongitudeAccuracy, order, &collection->ExistLongitudeAccuracy);
	// Latitude
	exif_srational_write(content, SGEO_TAG_LATITUDE, &collection->Latitude, order, &collection->ExistLatitude);
	// LatAccuracy
	exif_rational_write(content, SGEO_TAG_LATACCURACY, &collection->LatitudeAccuracy, order, &collection->ExistLatitudeAccuracy);
	// Altitude
	exif_srational_write(content, SGEO_TAG_ALTITUDE, &collection->Altitude, order, &collection->ExistAltitude);
	// AltAccuracy
	exif_rational_write(content, SGEO_TAG_ALTACCURACY, &collection->AltitudeAccuracy, order, &collection->ExistAltitudeAccuracy);
	// Azimuth
	exif_srational_write(content, SGEO_TAG_AZIMUTH, &collection->Azimuth, order, &collection->ExistAzimuth);
	// AzimuthAccuracy
	exif_rational_write(content, SGEO_TAG_AZIMUTHACCURACY, &collection->AzimuthAccuracy, order, &collection->ExistAzimuthAccuracy);
	// Pitch
	exif_srational_write(content, SGEO_TAG_PITCH, &collection->Pitch, order, &collection->ExistPitch);
	// PitchAccuracy
	exif_rational_write(content, SGEO_TAG_PITCHACCURACY, &collection->PitchAccuracy, order, &collection->ExistPitchAccuracy);
	// Roll
	exif_srational_write(content, SGEO_TAG_ROLL, &collection->Roll, order, &collection->ExistRoll);
	// RollAccuracy
	exif_rational_write(content, SGEO_TAG_ROLLACCURACY, &collection->RollAccuracy, order, &collection->ExistRollAccuracy);
	// HViewAngle
	exif_rational_write(content, SGEO_TAG_HVIEWANGLE, &collection->HViewAngle, order, &collection->ExistHViewAngle);
	// HViewAngleAccuracy
	exif_rational_write(content, SGEO_TAG_HVIEWANGLEACCURACY, &collection->HViewAngleAccuracy, order, &collection->ExistHViewAngleAccuracy);
	// VViewAngle
	exif_rational_write(content, SGEO_TAG_VVIEWANGLE, &collection->VViewAngle, order, &collection->ExistVViewAngle);
	// VViewAngleAccuracy
	exif_rational_write(content, SGEO_TAG_VVIEWANGLEACCURACY, &collection->VViewAngleAccuracy, order, &collection->ExistVViewAngleAccuracy);
	// SatCount
	exif_mem_write(content, SGEO_TAG_SATCOUNT, EXIF_FORMAT_BYTE, &collection->SatCount, sizeof(collection->SatCount), NULL);
	// global time
	exif_mem_write(content, SGEO_TAG_GLOBAL_TIME, EXIF_FORMAT_BYTE, collection->GlobalTime, sizeof(collection->GlobalTime), NULL);

	// GNSSType
	exif_long_write(content, SGEO_TAG_GNSS_TYPE, &collection->GNSSType, order, NULL);

	// DeviceName
	write_unicode_string_to_exif(content, SGEO_TAG_DEVICE_NAME, collection->DeviceName);
	// DeviceIMEI
	write_unicode_string_to_exif(content, SGEO_TAG_DEVICE_IMEI, collection->DeviceIMEI);
	// DeviceNumber
	write_unicode_string_to_exif(content, SGEO_TAG_DEVICE_NUMBER, collection->DeviceNumber);
	// DeviceOS
	write_unicode_string_to_exif(content, SGEO_TAG_DEVICE_OS, collection->DeviceOS);
	// DeviceOSVersion
	write_unicode_string_to_exif(content, SGEO_TAG_DEVICE_OS_VERSION, collection->DeviceOSVersion);
	// DeviceVersion
	write_unicode_string_to_exif(content, SGEO_TAG_DEVICE_VERSION, collection->DeviceVersion);
	// DeviceDateTimeMeasure
	exif_mem_write(content, SGEO_TAG_DEVICE_DATE_TIME_MEASURE, EXIF_FORMAT_BYTE, collection->DeviceDateTimeMeasure, sizeof(collection->DeviceDateTimeMeasure), NULL);

	// ProgramVersion
	write_unicode_string_to_exif(content, SGEO_TAG_PROGRAM_VERSION, collection->ProgramVersion);
	// ProgramName
	write_unicode_string_to_exif(content, SGEO_TAG_PROGRAM_NAME, collection->ProgramName);
	// ProgramUserName
	write_unicode_string_to_exif(content, SGEO_TAG_PROGRAM_USER_NAME, collection->ProgramUserName);
	
	size_sgeo_bytes = 2 + content->count * 12;
	for (i = 0; i < content->count; i++)
	{
		entry = content->entries[i];
		if (entry != NULL)
		{
			s = exif_format_get_size(entry->format) * entry->components;
			if (s > 4)
				size_sgeo_bytes += s;
			//	s += 12;// +tag size
			//else
			//	s = 12;
		}
	}
	if (size_sgeo_bytes > 0)
	{
		*data = (unsigned char* )calloc((size_t)size_sgeo_bytes, 1);
		if (*data != NULL)
		{
			write_content_binary(*data, content, order, 0);
			s = 0;
			if (size_sgeo_bytes < 1024)
				s = 1024 - size_sgeo_bytes;
			s += 2390;// white noice
			*size = size_sgeo_bytes + s;
			*data = (unsigned char* )realloc(*data, (size_t)(size_sgeo_bytes + s));
			n = rand() % 255;
			for (i = 0; i < s; i++)
			{
				n = GaloisLfsr(n);
				(*data)[size_sgeo_bytes + i] = n;
			}
		}
	}
	exif_data_unref(exif);
}


SGeoTags read_sgeo_tags(ExifContent *content, ExifByteOrder order)
{
	int i = 0;
	unsigned char *buf = NULL;
	SGeoTags collect = {0};
	ExifEntry *entry = NULL;
	//ExifContent *content = exif->ifd[EXIF_IFD_SGEO];
	SGeoTags *collection = NULL;
	
	collection = &collect;

	// Longitude
	exif_srational_read(&collection->Longitude, &collection->ExistLongitude, content, SGEO_TAG_LONGITUDE, order);
	// LongAccuracy
	exif_rational_read(&collection->LongitudeAccuracy, &collection->ExistLongitudeAccuracy, content, SGEO_TAG_LONGACCURACY, order);
	// Latitude
	exif_srational_read(&collection->Latitude, &collection->ExistLatitude, content, SGEO_TAG_LATITUDE, order);
	// LatAccuracy
	exif_rational_read(&collection->LatitudeAccuracy, &collection->ExistLatitudeAccuracy, content, SGEO_TAG_LATACCURACY, order);
	// Altitude
	exif_srational_read(&collection->Altitude, &collection->ExistAltitude, content, SGEO_TAG_ALTITUDE, order);
	// AltAccuracy
	exif_rational_read(&collection->AltitudeAccuracy, &collection->ExistAltitudeAccuracy, content, SGEO_TAG_ALTACCURACY, order);
	// Azimuth
	exif_srational_read(&collection->Azimuth, &collection->ExistAzimuth, content, SGEO_TAG_AZIMUTH, order);
	// AzimuthAccuracy
	exif_rational_read(&collection->AzimuthAccuracy, &collection->ExistAzimuthAccuracy, content, SGEO_TAG_AZIMUTHACCURACY, order);
	// Pitch
	exif_srational_read(&collection->Pitch, &collection->ExistPitch, content, SGEO_TAG_PITCH, order);
	// PitchAccuracy
	exif_rational_read(&collection->PitchAccuracy, &collection->ExistPitchAccuracy, content, SGEO_TAG_PITCHACCURACY, order);
	// Roll
	exif_srational_read(&collection->Roll, &collection->ExistRoll, content, SGEO_TAG_ROLL, order);
	// RollAccuracy
	exif_rational_read(&collection->RollAccuracy, &collection->ExistRollAccuracy, content, SGEO_TAG_ROLLACCURACY, order);
	// HViewAngle
	exif_rational_read(&collection->HViewAngle, &collection->ExistHViewAngle, content, SGEO_TAG_HVIEWANGLE, order);
	// HViewAngleAccuracy
	exif_rational_read(&collection->HViewAngleAccuracy, &collection->ExistHViewAngleAccuracy, content, SGEO_TAG_HVIEWANGLEACCURACY, order);
	// VViewAngle
	exif_rational_read(&collection->VViewAngle, &collection->ExistVViewAngle, content, SGEO_TAG_VVIEWANGLE, order);
	// VViewAngleAccuracy
	exif_rational_read(&collection->VViewAngleAccuracy, &collection->ExistVViewAngleAccuracy, content, SGEO_TAG_VVIEWANGLEACCURACY, order);
	// SatCount
	exif_mem_read(&collection->SatCount, &collection->ExistSatCount, content, SGEO_TAG_SATCOUNT, sizeof(collection->SatCount));
	// global time
	exif_mem_read(collection->GlobalTime, &collection->ExistGlobalTime, content, SGEO_TAG_GLOBAL_TIME, sizeof(collection->GlobalTime));

	// UserLongitude
	exif_srational_read(&collection->UserLongitude, NULL, content, SGEO_TAG_USER_LONGITUDE, order);
	// UserLongAccuracy
	exif_rational_read(&collection->UserLongitudeAccuracy, NULL, content, SGEO_TAG_USER_LONGACCURACY, order);
	// UserLatitude
	exif_srational_read(&collection->UserLatitude, NULL, content, SGEO_TAG_USER_LATITUDE, order);
	// UserLatAccuracy
	exif_rational_read(&collection->UserLatitudeAccuracy, NULL, content, SGEO_TAG_USER_LATACCURACY, order);
	// UserAltitude
	exif_srational_read(&collection->UserAltitude, NULL, content, SGEO_TAG_USER_ALTITUDE, order);
	// UserAltAccuracy
	exif_rational_read(&collection->UserAltitudeAccuracy, NULL, content, SGEO_TAG_USER_ALTACCURACY, order);
	// UserEarthLavel
	exif_srational_read(&collection->UserEarthLevel, NULL, content, SGEO_TAG_USER_EARTH_LEVEL, order);
	// UserAzimuth
	exif_srational_read(&collection->UserAzimuth, NULL, content, SGEO_TAG_USER_AZIMUTH, order);
	// UserAzimuthAccuracy
	exif_rational_read(&collection->UserAzimuthAccuracy, NULL, content, SGEO_TAG_USER_AZIMUTHACCURACY, order);
	// UserPitch
	exif_srational_read(&collection->UserPitch, NULL, content, SGEO_TAG_USER_PITCH, order);
	// UserPitchAccuracy
	exif_rational_read(&collection->UserPitchAccuracy, NULL, content, SGEO_TAG_USER_PITCHACCURACY, order);
	// UserRoll
	exif_srational_read(&collection->UserRoll, NULL, content, SGEO_TAG_USER_ROLL, order);
	// UserRollAccuracy
	exif_rational_read(&collection->UserRollAccuracy, NULL, content, SGEO_TAG_USER_ROLLACCURACY, order);
	// UserHViewAngle
	exif_rational_read(&collection->UserHViewAngle, NULL, content, SGEO_TAG_USER_HVIEWANGLE, order);
	// UserHViewAngleAccuracy
	exif_rational_read(&collection->UserHViewAngleAccuracy, NULL, content, SGEO_TAG_USER_HVIEWANGLEACCURACY, order);
	// UserVViewAngle
	exif_rational_read(&collection->UserVViewAngle, NULL, content, SGEO_TAG_USER_VVIEWANGLE, order);
	// UserVViewAngleAccuracy
	exif_rational_read(&collection->UserVViewAngleAccuracy, NULL, content, SGEO_TAG_USER_VVIEWANGLEACCURACY, order);
	// UserDeviceName
	read_unicode_string_from_exif(content, SGEO_TAG_USER_DEVICE_NAME, collection->UserDeviceName, sizeof(collection->UserDeviceName) / sizeof(*collection->UserDeviceName));
	// UserProgramName
	read_unicode_string_from_exif(content, SGEO_TAG_USER_PROGRAM_NAME, collection->UserProgramName, sizeof(collection->UserProgramName) / sizeof(*collection->UserProgramName));
	// UserUserName
	read_unicode_string_from_exif(content, SGEO_TAG_USER_USER_NAME, collection->UserUserName, sizeof(collection->UserUserName) / sizeof(*collection->UserUserName));
	


	// ResultLongitude
	exif_srational_read(&collection->ResultLongitude, NULL, content, SGEO_TAG_RESULT_LONGITUDE, order);
	// ResultLongAccuracy
	exif_rational_read(&collection->ResultLongitudeAccuracy, NULL, content, SGEO_TAG_RESULT_LONGACCURACY, order);
	// ResultLatitude
	exif_srational_read(&collection->ResultLatitude, NULL, content, SGEO_TAG_RESULT_LATITUDE, order);
	// ResultLatAccuracy
	exif_rational_read(&collection->ResultLatitudeAccuracy, NULL, content, SGEO_TAG_RESULT_LATACCURACY, order);
	// ResultAltitude
	exif_srational_read(&collection->ResultAltitude, NULL, content, SGEO_TAG_RESULT_ALTITUDE, order);
	// ResultAltAccuracy
	exif_rational_read(&collection->ResultAltitudeAccuracy, NULL, content, SGEO_TAG_RESULT_ALTACCURACY, order);
	// ResultAzimuth
	exif_srational_read(&collection->ResultAzimuth, NULL, content, SGEO_TAG_RESULT_AZIMUTH, order);
	// ResultAzimuthAccuracy
	exif_rational_read(&collection->ResultAzimuthAccuracy, NULL, content, SGEO_TAG_RESULT_AZIMUTHACCURACY, order);
	// ResultPitch
	exif_srational_read(&collection->ResultPitch, NULL, content, SGEO_TAG_RESULT_PITCH, order);
	// ResultPitchAccuracy
	exif_rational_read(&collection->ResultPitchAccuracy, NULL, content, SGEO_TAG_RESULT_PITCHACCURACY, order);
	// ResultRoll
	exif_srational_read(&collection->ResultRoll, NULL, content, SGEO_TAG_RESULT_ROLL, order);
	// ResultRollAccuracy
	exif_rational_read(&collection->ResultRollAccuracy, NULL, content, SGEO_TAG_RESULT_ROLLACCURACY, order);
	// ResultHViewAngle
	exif_rational_read(&collection->ResultHViewAngle, NULL, content, SGEO_TAG_RESULT_HVIEWANGLE, order);
	// ResultHViewAngleAccuracy
	exif_rational_read(&collection->ResultHViewAngleAccuracy, NULL, content, SGEO_TAG_RESULT_HVIEWANGLEACCURACY, order);
	// ResultVViewAngle
	exif_rational_read(&collection->ResultVViewAngle, NULL, content, SGEO_TAG_RESULT_VVIEWANGLE, order);
	// ResultVViewAngleAccuracy
	exif_rational_read(&collection->ResultVViewAngleAccuracy, NULL, content, SGEO_TAG_RESULT_VVIEWANGLEACCURACY, order);

	// GNSSType
	exif_long_read(&collection->GNSSType, &collection->ExistVViewAngle, content, SGEO_TAG_GNSS_TYPE, order);

	// DeviceName
	read_unicode_string_from_exif(content, SGEO_TAG_DEVICE_NAME, collection->DeviceName, sizeof(collection->DeviceName) / sizeof(*collection->DeviceName));
	// DeviceIMEI
	read_unicode_string_from_exif(content, SGEO_TAG_DEVICE_IMEI, collection->DeviceIMEI, sizeof(collection->DeviceIMEI) / sizeof(*collection->DeviceIMEI));
	// DeviceNumber
	read_unicode_string_from_exif(content, SGEO_TAG_DEVICE_NUMBER, collection->DeviceNumber, sizeof(collection->DeviceNumber) / sizeof(*collection->DeviceNumber));
	// DeviceOS
	read_unicode_string_from_exif(content, SGEO_TAG_DEVICE_OS, collection->DeviceOS, sizeof(collection->DeviceOS) / sizeof(*collection->DeviceOS));
	// DeviceOSVersion
	read_unicode_string_from_exif(content, SGEO_TAG_DEVICE_OS_VERSION, collection->DeviceOSVersion, sizeof(collection->DeviceOSVersion) / sizeof(*collection->DeviceOSVersion));
	// DeviceVersion
	read_unicode_string_from_exif(content, SGEO_TAG_DEVICE_VERSION, collection->DeviceVersion, sizeof(collection->DeviceVersion) / sizeof(*collection->DeviceVersion));
	// DeviceDateTimeMeasure
	exif_mem_read(collection->DeviceDateTimeMeasure, NULL, content, SGEO_TAG_DEVICE_DATE_TIME_MEASURE, sizeof(collection->DeviceDateTimeMeasure));

	// ProgramVersion
	read_unicode_string_from_exif(content, SGEO_TAG_PROGRAM_VERSION, collection->ProgramVersion, sizeof(collection->ProgramVersion) / sizeof(*collection->ProgramVersion));
	// ProgramName
	read_unicode_string_from_exif(content, SGEO_TAG_PROGRAM_NAME, collection->ProgramName, sizeof(collection->ProgramName) / sizeof(*collection->ProgramName));
	// ProgramUserName
	read_unicode_string_from_exif(content, SGEO_TAG_PROGRAM_USER_NAME, collection->ProgramUserName, sizeof(collection->ProgramUserName) / sizeof(*collection->ProgramUserName));

	// ObjectType
	//entry = exif_content_get_entry (content, (ExifTag)(SGEO_TAG_OBJECT_TYPE));
	//if (entry) collection.ObjectType = *entry->data;

	// object data
	//entry = exif_content_get_entry (content, (ExifTag)(SGEO_TAG_OBJECT_DATA));
	//if (entry)
	//{
	//	collection->ObjectDataSize = 0;
	//	collection->ObjectData = (ExifByte* )malloc(entry->size);
	//	if (collection->ObjectData)
	//	{
	//		collection->ObjectDataSize = entry->size;
	//		memcpy(collection->ObjectData, exif->data, collection->ObjectDataSize);
	//	}
	//}

	// encrypted table
	entry = exif_content_get_entry (content, (ExifTag)(SGEO_TAG_ENCRYPT_TABLE));
	if (entry)
	{
		exif_long_read(&collection->SGeoEncrypt.Type, NULL, content, SGEO_TAG_ENCRYPT_TYPE, order);
		exif_mem_read(collection->SGeoEncrypt.AlgorithmVersion, NULL, content, SGEO_TAG_ENCRYPT_ALGORITHM_VERSION, sizeof(collection->SGeoEncrypt.AlgorithmVersion));
		exif_mem_read(collection->SGeoEncrypt.PublicKey, NULL, content, SGEO_TAG_ENCRYPT_PUBLIC_KEY, sizeof(collection->SGeoEncrypt.PublicKey));

		xorCryptData(entry->data, entry->size);
		generateDecryptData(&collection->SGeoEncrypt, entry->data, entry->size, order);
	}

#ifdef _SGEO_FULL
	// Sighting
	collection->UserSightingCount = 0;
	entry = exif_content_get_entry (content, (ExifTag)(SGEO_TAG_SIGHTING));
	if (entry)
	{
		collection->UserSightingCount = entry->size / sizeof(SightingTags);
		collection->UserSightingData = (SightingTags* )malloc(sizeof(*collection->UserSightingData) * collection->UserSightingCount);
		buf = entry->data;
		for (i = 0; i < collection->UserSightingCount; i++)
		{
			collection->UserSightingData[i].Angle = exif_get_srational(buf, order);
			buf += sizeof(ExifSRational);
			collection->UserSightingData[i].Flags = *buf;
			buf++;
		}
	}
#endif

	// read objects
	collection->ObjectsTableCount = 60;
	collection->ObjectsTable = (SGeoObject *)calloc(collection->ObjectsTableCount, sizeof(*collection->ObjectsTable));
	exif_mem_read(collection->ObjectsTable, NULL, content, (ExifTag)SGEO_TAG_OBJECT_TABLE, sizeof(*collection->ObjectsTable) * collection->ObjectsTableCount);

	return *collection;
}
MapTags *read_sgeo_tags_map(ExifData **exif_array, ExifIfd ifd, ExifShort map_count)
{
	int i;
	MapTags *map_data = NULL;
	ExifData *exif = NULL;
	ExifEntry *entry;
	ExifByteOrder order;
	SGeoTag stag;
	ExifSRational *srational = NULL;
	ExifLong *elong = NULL;
	
	order = exif_data_get_byte_order(exif);
	map_data = (MapTags* )calloc(map_count, sizeof(*map_data));
	for (i = 0; i < map_count; i++)
	{
		exif = exif_array[i];

		entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_MAP_VERSION));
		if (entry)
			memcpy(map_data[i].VersionID, entry->data, sizeof(map_data[i].VersionID));

		entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_MAP_ID));
		if (entry)
			map_data[i].ID = exif_get_long(entry->data, order);

		srational = &map_data[i].TopLeftLongitude;
		for (stag = SGEO_TAG_MAP_TOP_LEFT_LONGITUDE; stag <= SGEO_TAG_MAP_BOTTOM_RIGHT_LATITUDE; stag++)
		{
			entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(stag));
			if (entry)
				*srational = exif_get_srational(entry->data, order);
			srational++;
		}
		elong = &map_data[i].CoordinateSystem;
		for (stag = SGEO_TAG_MAP_COORDINATE_SYSTEM; stag <= SGEO_TAG_MAP_THUMBNAIL_FORMAT; stag++)
		{
			entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(stag));
			if (entry)
				*elong = exif_get_long(entry->data, order);
			elong++;
		}
		entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_MAP_THUMBNAIL));
		if (entry)
		{
			map_data[i].Data = (void *)malloc(entry->size);
			map_data[i].DataSize = entry->size;
			memcpy(map_data[i].Data, entry->data, entry->size);
		}
	}
	return map_data;
}

ExifEntry *create_tag_in_content(ExifContent *content, ExifTag tag, size_t len, ExifFormat exifFormat)
{
	void *buf;
	ExifEntry *entry;
	size_t size;
	
	/* Create a memory allocator to manage this ExifEntry */
	ExifMem *mem = exif_mem_new_default();
	if (!mem) return NULL;
	
	/* Create a new ExifEntry using our allocator */
	entry = exif_entry_new_mem (mem);
	if (entry)
	{
		/* Allocate memory to use for holding the tag data */
		size = len * exif_format_get_size(exifFormat);
		
		buf = exif_mem_alloc(mem, size);
		if (buf)
		{
			/* Fill in the entry */
			entry->data = (unsigned char*)buf;
			entry->size = size;
			entry->tag = tag;
			entry->components = len;
			entry->format = exifFormat;

			/* Attach the ExifEntry to an IFD */
			exif_content_add_entry (content, entry);
		}
		
		exif_entry_unref(entry);
	}

	exif_mem_unref(mem);

	return entry;
}
ExifEntry *create_tag(ExifData *exif, ExifIfd ifd, ExifTag tag, size_t len, ExifFormat exifFormat)
{
	return create_tag_in_content(exif->ifd[ifd], tag, len, exifFormat);
}

void write_sgeo_tags(ExifData *exif, ExifIfd ifd, SGeoTags * collection)
{
	unsigned int i = 0;
	unsigned char *buf = NULL;
	ExifEntry *entry = NULL;
	ExifByteOrder order;
	ExifContent *content = NULL;

	order = exif_data_get_byte_order(exif);
	content = exif->ifd[ifd];

	// VersionID
	exif_mem_write(content, SGEO_TAG_VERSIONID, EXIF_FORMAT_BYTE, collection->VersionID, sizeof(collection->VersionID), NULL);
	// Longitude
	exif_srational_write(content, SGEO_TAG_LONGITUDE, &collection->Longitude, order, &collection->ExistLongitude);
	// LongAccuracy
	exif_rational_write(content, SGEO_TAG_LONGACCURACY, &collection->LongitudeAccuracy, order, &collection->ExistLongitudeAccuracy);
	// Latitude
	exif_srational_write(content, SGEO_TAG_LATITUDE, &collection->Latitude, order, &collection->ExistLatitude);
	// LatAccuracy
	exif_rational_write(content, SGEO_TAG_LATACCURACY, &collection->LatitudeAccuracy, order, &collection->ExistLatitudeAccuracy);
	// Altitude
	exif_srational_write(content, SGEO_TAG_ALTITUDE, &collection->Altitude, order, &collection->ExistAltitude);
	// AltAccuracy
	exif_rational_write(content, SGEO_TAG_ALTACCURACY, &collection->AltitudeAccuracy, order, &collection->ExistAltitudeAccuracy);
	// Azimuth
	exif_srational_write(content, SGEO_TAG_AZIMUTH, &collection->Azimuth, order, &collection->ExistAzimuth);
	// AzimuthAccuracy
	exif_rational_write(content, SGEO_TAG_AZIMUTHACCURACY, &collection->AzimuthAccuracy, order, &collection->ExistAzimuthAccuracy);
	// Pitch
	exif_srational_write(content, SGEO_TAG_PITCH, &collection->Pitch, order, &collection->ExistPitch);
	// PitchAccuracy
	exif_rational_write(content, SGEO_TAG_PITCHACCURACY, &collection->PitchAccuracy, order, &collection->ExistPitchAccuracy);
	// Roll
	exif_srational_write(content, SGEO_TAG_ROLL, &collection->Roll, order, &collection->ExistRoll);
	// RollAccuracy
	exif_rational_write(content, SGEO_TAG_ROLLACCURACY, &collection->RollAccuracy, order, &collection->ExistRollAccuracy);
	// HViewAngle
	exif_rational_write(content, SGEO_TAG_HVIEWANGLE, &collection->HViewAngle, order, &collection->ExistHViewAngle);
	// HViewAngleAccuracy
	exif_rational_write(content, SGEO_TAG_HVIEWANGLEACCURACY, &collection->HViewAngleAccuracy, order, &collection->ExistHViewAngleAccuracy);
	// VViewAngle
	exif_rational_write(content, SGEO_TAG_VVIEWANGLE, &collection->VViewAngle, order, &collection->ExistVViewAngle);
	// VViewAngleAccuracy
	exif_rational_write(content, SGEO_TAG_VVIEWANGLEACCURACY, &collection->VViewAngleAccuracy, order, &collection->ExistVViewAngleAccuracy);
	// SatCount
	exif_mem_write(content, SGEO_TAG_SATCOUNT, EXIF_FORMAT_BYTE, &collection->SatCount, sizeof(collection->SatCount), NULL);
	// global time
	exif_mem_write(content, SGEO_TAG_GLOBAL_TIME, EXIF_FORMAT_BYTE, collection->GlobalTime, sizeof(collection->GlobalTime), NULL);

	// UserLongitude
	exif_srational_write(content, SGEO_TAG_USER_LONGITUDE, &collection->UserLongitude, order, NULL);
	// UserLongAccuracy
	exif_rational_write(content, SGEO_TAG_USER_LONGACCURACY, &collection->UserLongitudeAccuracy, order, NULL);
	// UserLatitude
	exif_srational_write(content, SGEO_TAG_USER_LATITUDE, &collection->UserLatitude, order, NULL);
	// UserLatAccuracy
	exif_rational_write(content, SGEO_TAG_USER_LATACCURACY, &collection->UserLatitudeAccuracy, order, NULL);
	// UserAltitude
	exif_srational_write(content, SGEO_TAG_USER_ALTITUDE, &collection->UserAltitude, order, NULL);
	// UserAltAccuracy
	exif_rational_write(content, SGEO_TAG_USER_ALTACCURACY, &collection->UserAltitudeAccuracy, order, NULL);
	// UserEarthLavel
	exif_srational_write(content, SGEO_TAG_USER_EARTH_LEVEL, &collection->UserEarthLevel, order, NULL);
	// UserAzimuth	
	exif_srational_write(content, SGEO_TAG_USER_AZIMUTH, &collection->UserAzimuth, order, NULL);
	// UserAzimuthAccuracy
	exif_rational_write(content, SGEO_TAG_USER_AZIMUTHACCURACY, &collection->UserAzimuthAccuracy, order, NULL);
	// UserPitch
	exif_srational_write(content, SGEO_TAG_USER_PITCH, &collection->UserPitch, order, NULL);
	// UserPitchAccuracy
	exif_rational_write(content, SGEO_TAG_USER_PITCHACCURACY, &collection->UserPitchAccuracy, order, NULL);
	// UserRoll
	exif_srational_write(content, SGEO_TAG_USER_ROLL, &collection->UserRoll, order, NULL);
	// UserRollAccuracy
	exif_rational_write(content, SGEO_TAG_USER_ROLLACCURACY, &collection->UserRollAccuracy, order, NULL);
	// UserHViewAngle
	exif_rational_write(content, SGEO_TAG_USER_HVIEWANGLE, &collection->UserHViewAngle, order, NULL);
	// UserHViewAngleAccuracy
	exif_rational_write(content, SGEO_TAG_USER_HVIEWANGLEACCURACY, &collection->UserHViewAngleAccuracy, order, NULL);
	// UserVViewAngle
	exif_rational_write(content, SGEO_TAG_USER_VVIEWANGLE, &collection->UserVViewAngle, order, NULL);
	// UserVViewAngleAccuracy
	exif_rational_write(content, SGEO_TAG_USER_VVIEWANGLEACCURACY, &collection->UserVViewAngleAccuracy, order, NULL);
	// UserDeviceName
	write_unicode_string_to_exif(content, SGEO_TAG_USER_DEVICE_NAME, collection->UserDeviceName);
	// UserProgramName
	write_unicode_string_to_exif(content, SGEO_TAG_USER_PROGRAM_NAME, collection->UserProgramName);
	// UserUserName
	write_unicode_string_to_exif(content, SGEO_TAG_USER_USER_NAME, collection->UserUserName);

	// ResultLongitude
	exif_srational_write(content, SGEO_TAG_RESULT_LONGITUDE, &collection->ResultLongitude, order, NULL);
	// ResultLongAccuracy
	exif_rational_write(content, SGEO_TAG_RESULT_LONGACCURACY, &collection->ResultLongitudeAccuracy, order, NULL);
	// ResultLatitude
	exif_srational_write(content, SGEO_TAG_RESULT_LATITUDE, &collection->ResultLatitude, order, NULL);
	// ResultLatAccuracy
	exif_rational_write(content, SGEO_TAG_RESULT_LATACCURACY, &collection->ResultLatitudeAccuracy, order, NULL);
	// ResultAltitude
	exif_srational_write(content, SGEO_TAG_RESULT_ALTITUDE, &collection->ResultAltitude, order, NULL);
	// ResultAltAccuracy
	exif_rational_write(content, SGEO_TAG_RESULT_ALTACCURACY, &collection->ResultAltitudeAccuracy, order, NULL);
	// ResultAzimuth
	exif_srational_write(content, SGEO_TAG_RESULT_AZIMUTH, &collection->ResultAzimuth, order, NULL);
	// ResultAzimuthAccuracy
	exif_rational_write(content, SGEO_TAG_RESULT_AZIMUTHACCURACY, &collection->ResultAzimuthAccuracy, order, NULL);
	// ResultPitch
	exif_srational_write(content, SGEO_TAG_RESULT_PITCH, &collection->ResultPitch, order, NULL);
	// ResultPitchAccuracy
	exif_rational_write(content, SGEO_TAG_RESULT_PITCHACCURACY, &collection->ResultPitchAccuracy, order, NULL);
	// ResultRoll
	exif_srational_write(content, SGEO_TAG_RESULT_ROLL, &collection->ResultRoll, order, NULL);
	// ResultRollAccuracy
	exif_rational_write(content, SGEO_TAG_RESULT_ROLLACCURACY, &collection->ResultRollAccuracy, order, NULL);
	// ResultHViewAngle
	exif_rational_write(content, SGEO_TAG_RESULT_HVIEWANGLE, &collection->ResultHViewAngle, order, NULL);
	// ResultHViewAngleAccuracy
	exif_rational_write(content, SGEO_TAG_RESULT_HVIEWANGLEACCURACY, &collection->ResultHViewAngleAccuracy, order, NULL);
	// ResultVViewAngle
	exif_rational_write(content, SGEO_TAG_RESULT_VVIEWANGLE, &collection->ResultVViewAngle, order, NULL);
	// ResultVViewAngleAccuracy
	exif_rational_write(content, SGEO_TAG_RESULT_VVIEWANGLEACCURACY, &collection->ResultVViewAngleAccuracy, order, NULL);

	// GNSSType
	exif_long_write(content, SGEO_TAG_GNSS_TYPE, &collection->GNSSType, order, NULL);

	// DeviceName
	write_unicode_string_to_exif(content, SGEO_TAG_DEVICE_NAME, collection->DeviceName);
	// DeviceIMEI
	write_unicode_string_to_exif(content, SGEO_TAG_DEVICE_IMEI, collection->DeviceIMEI);
	// DeviceNumber
	write_unicode_string_to_exif(content, SGEO_TAG_DEVICE_NUMBER, collection->DeviceNumber);
	// DeviceOS
	write_unicode_string_to_exif(content, SGEO_TAG_DEVICE_OS, collection->DeviceOS);
	// DeviceOSVersion
	write_unicode_string_to_exif(content, SGEO_TAG_DEVICE_OS_VERSION, collection->DeviceOSVersion);
	// DeviceVersion
	write_unicode_string_to_exif(content, SGEO_TAG_DEVICE_VERSION, collection->DeviceVersion);
	// DeviceDateTimeMeasure
	exif_mem_write(content, SGEO_TAG_DEVICE_DATE_TIME_MEASURE, EXIF_FORMAT_BYTE, collection->DeviceDateTimeMeasure, sizeof(collection->DeviceDateTimeMeasure), NULL);

	// ProgramName
	write_unicode_string_to_exif(content, SGEO_TAG_PROGRAM_NAME, collection->ProgramName);
	// ProgramVersion
	write_unicode_string_to_exif(content, SGEO_TAG_PROGRAM_VERSION, collection->ProgramVersion);
	// ProgramUserName
	write_unicode_string_to_exif(content, SGEO_TAG_PROGRAM_USER_NAME, collection->ProgramUserName);

	// DeviceName
	write_unicode_string_to_exif(content, SGEO_TAG_USER_DEVICE_NAME, collection->UserDeviceName);
	// ProgramName
	write_unicode_string_to_exif(content, SGEO_TAG_USER_PROGRAM_NAME, collection->UserProgramName);
	// ProgramUserName
	write_unicode_string_to_exif(content, SGEO_TAG_USER_USER_NAME, collection->UserUserName);


	// encrypted table
	generateEncryptData(&buf, &i, &collection->SGeoEncrypt, order);
	if (i > 0)
	{
		exif_long_write(content, SGEO_TAG_ENCRYPT_TYPE, &collection->SGeoEncrypt.Type, order, NULL);
		exif_mem_write(content, SGEO_TAG_ENCRYPT_ALGORITHM_VERSION, EXIF_FORMAT_BYTE, collection->SGeoEncrypt.AlgorithmVersion, sizeof(collection->SGeoEncrypt.AlgorithmVersion), NULL);
		exif_mem_write(content, SGEO_TAG_ENCRYPT_PUBLIC_KEY, EXIF_FORMAT_BYTE, collection->SGeoEncrypt.PublicKey, sizeof(collection->SGeoEncrypt.PublicKey), NULL);

		entry = exif_content_get_entry(content, (ExifTag)SGEO_TAG_ENCRYPT_TABLE);
		if (!entry)
			entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_ENCRYPT_TABLE, i, EXIF_FORMAT_BYTE);
		memcpy(entry->data, buf, i);
		xorCryptData(entry->data, i);
		free(buf);
	}
#ifdef _SGEO_FULL
	// Sighting
	entry = exif_content_get_entry(content, (ExifTag)SGEO_TAG_SIGHTING);
	if (!entry && collection->UserSightingCount > 0)
	{
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_SIGHTING, sizeof(*collection->UserSightingData) * collection->UserSightingCount, EXIF_FORMAT_BYTE);
		buf = entry->data;
		for (i = 0; i < collection->UserSightingCount; i++)
		{
			exif_set_srational(buf, order, collection->UserSightingData[i].Angle);
			buf += sizeof(ExifSRational);
			*buf = collection->UserSightingData[i].Flags;
			buf++;
		}
	}
#endif
	if (collection->ObjectsTable != NULL && collection->ObjectsTableCount > 0)
		exif_mem_write(content, (ExifTag)SGEO_TAG_OBJECT_TABLE, EXIF_FORMAT_BYTE, collection->ObjectsTable, sizeof(*collection->ObjectsTable) * collection->ObjectsTableCount, NULL);
}


void write_sgeo_version(unsigned char *data, ExifByteOrder order)
{
	memcpy(data + 0x00, "SG", 2);
	exif_set_short(data + 0x02, order, SGEO_VERSION);
}
int is_sgeo_app2(unsigned char *data)
{
	int res = 1;
	unsigned char *d = data;
	res &= !memcmp(d, "FPXR", 5);
	d += 5;
	if (res)
	{
		res &= !(d[0] & 0xff);
		d++;
	}
	if (res)
	{
		d += 12;
		res &= !memcmp(d, "SG", 2);
	}
	return res;
}
void write_sgeo_app2(JPEGSection *section, ExifData *exif)
{
	int size_sgeo_bytes = 10;
	unsigned int i = 0;
	unsigned int s = 0;
	unsigned int next_data = 0;
	ExifEntry *entry = NULL;
	ExifContent *sgeo_content = NULL;
	unsigned char *d = NULL;

	sgeo_content = exif->ifd[EXIF_IFD_SGEO];
	size_sgeo_bytes += sgeo_content->count * 12;
	for (i = 0; i < sgeo_content->count; i++)
	{
		entry = sgeo_content->entries[i];
		if (entry != NULL)
		{
			s = exif_format_get_size(entry->format) * entry->components;
			if (s > 4)
				size_sgeo_bytes += s;
				//s += 12;// +tag size
			//else
			//	s = 12;
		}
	}

	section->content._generic.data = (unsigned char* )calloc((size_t)size_sgeo_bytes, 1);
	if (section->content._generic.data == NULL)
		return;

	section->marker = JPEG_MARKER_APP2;
	section->content._generic.size = size_sgeo_bytes;

	d = section->content._generic.data;
	memcpy(d, "FPXR\0\0\0\0", 8);
	d = d + 8;

	write_content_binary(d, sgeo_content, exif->priv->order,
		8 + // "FPXR\0\0\0\0"
		2); // size APP2
}
void add_sgeo_app2(JPEGData *jdata, SGeoTags *sgeo_tags, ExifData *exif)
{
	int number = 2;
	int i;
	JPEGSection *section = NULL;
	JPEGSection temp;
	int section_for_sgeo_need = 1;// + (int)sgeo_tags->StaticMapCount;
	int index_insert = 0;
	int is_insert_map = 0;
	ExifData *exif_sgeo = NULL;
	ExifEntry *entry_version_id = NULL;// store SGEO_TAG_VERSIONID for SGEO Map
	ExifByteOrder order;

	order = exif_data_get_byte_order(exif);
	
	if (jdata == NULL)
		return;

	// find and exclude all SGEO in APP2
	for (i = 0; i < (int)jdata->count; i++)
	{
		section = &jdata->sections[i];
		if (section->marker == JPEG_MARKER_APP2)
		{
			// detecting SGEO by data
			if (is_sgeo_app2(section->content._generic.data))
			{
				jpeg_data_exclude_section(jdata, section);
				i = -1;
			}
		}
	}
	section = NULL;
	for (i = 0; i < (int)jdata->count; i++)
	{
		section = &jdata->sections[i];
		if (section->marker == JPEG_MARKER_APP1)
		{
			index_insert = i + 1;
			break;
		}
	}
	while (section_for_sgeo_need--)
	{
		// add section
		jpeg_data_append_section(jdata);

		// move section to index_insert
		i = jdata->count - 1;
		temp = jdata->sections[i];
		for (; i > index_insert; i--)
			jdata->sections[i] = jdata->sections[i - 1];
		jdata->sections[i] = temp;

		exif_sgeo = exif_data_new_mem (exif->priv->mem);
		exif_sgeo->priv->order = exif->priv->order;

		// write exif_sgeo
		if (is_insert_map)
		{
			// add enrty with SGEO version
			//if (entry_version_id != NULL)
			//	exif_content_add_entry (exif_sgeo->ifd[EXIF_IFD_SGEO], entry_version_id);
			//write_sgeo_tags_map(exif_sgeo, EXIF_IFD_SGEO, &sgeo_tags->StaticMapData[is_insert_map - 1]);
		}
		else
		{
			write_sgeo_tags(exif_sgeo, EXIF_IFD_SGEO, sgeo_tags);
			
			entry_version_id = exif_content_get_entry(exif_sgeo->ifd[EXIF_IFD_SGEO], (ExifTag)SGEO_TAG_VERSIONID);
			entry_version_id->priv->ref_count++;

		}
		// write data
		write_sgeo_app2(&jdata->sections[i], exif_sgeo);
		exif_data_unref(exif_sgeo);
		exif_sgeo = NULL;
		entry_version_id->parent = NULL;

		// increase index
		index_insert++;
		is_insert_map++;
	}
	exif_entry_unref (entry_version_id);
}

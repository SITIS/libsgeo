/* sgeotags.c
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

#include "sgeotags.h"
#include <string.h>
#include <libexif/exif-data.h>

SGeoTags read_sgeo_tags(ExifData *exif, ExifIfd ifd)
{
	SGeoTags collection = {0};
	ExifEntry *entry;
	ExifByteOrder order;

	order = exif_data_get_byte_order(exif);
	
	// Longitude
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_LONGITUDE));
	if (entry) collection.Longitude = exif_get_srational(entry->data, order);

	// LongAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_LONGACCURACY));
	if (entry) collection.LongitudeAccuracy = exif_get_rational(entry->data, order);

	// Latitude
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_LATITUDE));
	if (entry) collection.Latitude = exif_get_srational(entry->data, order);

	// LatAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_LATACCURACY));
	if (entry) collection.LatitudeAccuracy = exif_get_rational(entry->data, order);

	// Altitude
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_ALTITUDE));
	if (entry) collection.Altitude = exif_get_srational(entry->data, order);

	// AltAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_ALTACCURACY));
	if (entry) collection.AltitudeAccuracy = exif_get_rational(entry->data, order);

	// Azimuth
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_AZIMUTH));
	if (entry) collection.Azimuth = exif_get_srational(entry->data, order);

	// AzimuthAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_AZIMUTHACCURACY));
	if (entry) collection.AzimuthAccuracy = exif_get_rational(entry->data, order);

	// Pitch
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_PITCH));
	if (entry) collection.Pitch = exif_get_srational(entry->data, order);

	// PitchAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_PITCHACCURACY));
	if (entry) collection.PitchAccuracy = exif_get_rational(entry->data, order);

	// Roll
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_ROLL));
	if (entry) collection.Roll = exif_get_srational(entry->data, order);

	// RollAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_ROLLACCURACY));
	if (entry) collection.RollAccuracy = exif_get_rational(entry->data, order);

	// HViewAngle
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_HVIEWANGLE));
	if (entry) collection.HViewAngle = exif_get_rational(entry->data, order);

	// HViewAngleAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_HVIEWANGLEACCURACY));
	if (entry) collection.HViewAngleAccuracy = exif_get_rational(entry->data, order);

	// VViewAngle
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_VVIEWANGLE));
	if (entry) collection.VViewAngle = exif_get_rational(entry->data, order);

	// VViewAngleAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_VVIEWANGLEACCURACY));
	if (entry) collection.VViewAngleAccuracy = exif_get_rational(entry->data, order);

	// SatCount
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_SATCOUNT));
	if (entry)
		collection.SatCount = entry->data[0];

	
	// UserLongitude
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_USER_LONGITUDE));
	if (entry) collection.UserLongitude = exif_get_srational(entry->data, order);

	// UserLongAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_USER_LONGACCURACY));
	if (entry) collection.UserLongitudeAccuracy = exif_get_rational(entry->data, order);

	// UserLatitude
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_USER_LATITUDE));
	if (entry) collection.UserLatitude = exif_get_srational(entry->data, order);

	// UserLatAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_USER_LATACCURACY));
	if (entry) collection.UserLatitudeAccuracy = exif_get_rational(entry->data, order);

	// UserAltitude
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_USER_ALTITUDE));
	if (entry) collection.UserAltitude = exif_get_srational(entry->data, order);

	// UserAltAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_USER_ALTACCURACY));
	if (entry) collection.UserAltitudeAccuracy = exif_get_rational(entry->data, order);
	
	// UserEarthLavel
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_USER_EARTH_LEVEL));
	if (entry) collection.UserEarthLevel = exif_get_srational(entry->data, order);
	
	// UserAzimuth
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_USER_AZIMUTH));
	if (entry) collection.UserAzimuth = exif_get_srational(entry->data, order);

	// UserAzimuthAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_USER_AZIMUTHACCURACY));
	if (entry) collection.UserAzimuthAccuracy = exif_get_rational(entry->data, order);

	// UserPitch
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_USER_PITCH));
	if (entry) collection.UserPitch = exif_get_srational(entry->data, order);

	// UserPitchAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_USER_PITCHACCURACY));
	if (entry) collection.UserPitchAccuracy = exif_get_rational(entry->data, order);

	// UserRoll
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_USER_ROLL));
	if (entry) collection.UserRoll = exif_get_srational(entry->data, order);

	// UserRollAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_USER_ROLLACCURACY));
	if (entry) collection.UserRollAccuracy = exif_get_rational(entry->data, order);

	// UserHViewAngle
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_USER_HVIEWANGLE));
	if (entry) collection.UserHViewAngle = exif_get_rational(entry->data, order);

	// UserHViewAngleAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_USER_HVIEWANGLEACCURACY));
	if (entry) collection.UserHViewAngleAccuracy = exif_get_rational(entry->data, order);

	// UserVViewAngle
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_USER_VVIEWANGLE));
	if (entry) collection.UserVViewAngle = exif_get_rational(entry->data, order);

	// UserVViewAngleAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_USER_VVIEWANGLEACCURACY));
	if (entry) collection.UserVViewAngleAccuracy = exif_get_rational(entry->data, order);

	
	// ResultLongitude
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_RESULT_LONGITUDE));
	if (entry) collection.ResultLongitude = exif_get_srational(entry->data, order);

	// ResultLongAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_RESULT_LONGACCURACY));
	if (entry) collection.ResultLongitudeAccuracy = exif_get_rational(entry->data, order);

	// ResultLatitude
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_RESULT_LATITUDE));
	if (entry) collection.ResultLatitude = exif_get_srational(entry->data, order);

	// ResultLatAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_RESULT_LATACCURACY));
	if (entry) collection.ResultLatitudeAccuracy = exif_get_rational(entry->data, order);

	// ResultAltitude
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_RESULT_ALTITUDE));
	if (entry) collection.ResultAltitude = exif_get_srational(entry->data, order);

	// ResultAltAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_RESULT_ALTACCURACY));
	if (entry) collection.ResultAltitudeAccuracy = exif_get_rational(entry->data, order);

	// ResultAzimuth
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_RESULT_AZIMUTH));
	if (entry) collection.ResultAzimuth = exif_get_srational(entry->data, order);

	// ResultAzimuthAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_RESULT_AZIMUTHACCURACY));
	if (entry) collection.ResultAzimuthAccuracy = exif_get_rational(entry->data, order);

	// ResultPitch
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_RESULT_PITCH));
	if (entry) collection.ResultPitch = exif_get_srational(entry->data, order);

	// ResultPitchAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_RESULT_PITCHACCURACY));
	if (entry) collection.ResultPitchAccuracy = exif_get_rational(entry->data, order);

	// ResultRoll
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_RESULT_ROLL));
	if (entry) collection.ResultRoll = exif_get_srational(entry->data, order);

	// ResultRollAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_RESULT_ROLLACCURACY));
	if (entry) collection.ResultRollAccuracy = exif_get_rational(entry->data, order);

	// ResultHViewAngle
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_RESULT_HVIEWANGLE));
	if (entry) collection.ResultHViewAngle = exif_get_rational(entry->data, order);

	// ResultHViewAngleAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_RESULT_HVIEWANGLEACCURACY));
	if (entry) collection.ResultHViewAngleAccuracy = exif_get_rational(entry->data, order);

	// ResultVViewAngle
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_RESULT_VVIEWANGLE));
	if (entry) collection.ResultVViewAngle = exif_get_rational(entry->data, order);

	// ResultVViewAngleAccuracy
	entry = exif_content_get_entry (exif->ifd[ifd], (ExifTag)(SGEO_TAG_RESULT_VVIEWANGLEACCURACY));
	if (entry) collection.ResultVViewAngleAccuracy = exif_get_rational(entry->data, order);

	return collection;
}

ExifEntry *create_tag(ExifData *exif, ExifIfd ifd, ExifTag tag, size_t len, ExifFormat exifFormat)
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
			exif_content_add_entry (exif->ifd[ifd], entry);
		}
		
		exif_entry_unref(entry);
	}

	exif_mem_unref(mem);

	return entry;
}

void write_sgeo_tags(ExifData *exif,ExifIfd ifd, SGeoTags * collection)
{
	ExifEntry *entry;
	ExifByteOrder order;

	order = exif_data_get_byte_order(exif);
	
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_VERSIONID);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_VERSIONID, 4, EXIF_FORMAT_BYTE);
	memcpy(entry->data, collection->VersionID, 4);
	
	// Longitude
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_LONGITUDE);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_LONGITUDE, 1, EXIF_FORMAT_SRATIONAL);
	exif_set_srational(entry->data, order, collection->Longitude);

	// LongAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_LONGACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_LONGACCURACY, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->LongitudeAccuracy);
	
	// Latitude
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_LATITUDE);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_LATITUDE, 1, EXIF_FORMAT_SRATIONAL);
	exif_set_srational(entry->data, order, collection->Latitude);
	
	// LatAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_LATACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_LATACCURACY, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->LatitudeAccuracy);
	
	// Altitude
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_ALTITUDE);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_ALTITUDE, 1, EXIF_FORMAT_SRATIONAL);
	exif_set_srational(entry->data, order, collection->Altitude);
	
	// AltAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_ALTACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_ALTACCURACY, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->AltitudeAccuracy);
	
	// Azimuth
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_AZIMUTH);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_AZIMUTH, 1, EXIF_FORMAT_SRATIONAL);
	exif_set_srational(entry->data, order, collection->Azimuth);

	// AzimuthAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_AZIMUTHACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_AZIMUTHACCURACY, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->AzimuthAccuracy);

	// Pitch
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_PITCH);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_PITCH, 1, EXIF_FORMAT_SRATIONAL);
	exif_set_srational(entry->data, order, collection->Pitch);

	// PitchAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_PITCHACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_PITCHACCURACY, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->PitchAccuracy);

	// Roll
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_ROLL);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_ROLL, 1, EXIF_FORMAT_SRATIONAL);
	exif_set_srational(entry->data, order, collection->Roll);

	// RollAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_ROLLACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_ROLLACCURACY, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->RollAccuracy);

	// HViewAngle
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_HVIEWANGLE);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_HVIEWANGLE, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->HViewAngle);

	// HViewAngleAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_HVIEWANGLEACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_HVIEWANGLEACCURACY, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->HViewAngleAccuracy);

	// VViewAngle
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_VVIEWANGLE);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_VVIEWANGLE, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->VViewAngle);

	// VViewAngleAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_VVIEWANGLEACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_VVIEWANGLEACCURACY, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->VViewAngleAccuracy);

	// SatCount
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_SATCOUNT);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_SATCOUNT, 1, EXIF_FORMAT_BYTE);	
	entry->data[0] = collection->SatCount;


	// UserLongitude
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_USER_LONGITUDE);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_USER_LONGITUDE, 1, EXIF_FORMAT_SRATIONAL);
	exif_set_srational(entry->data, order, collection->UserLongitude);

	// UserLongAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_USER_LONGACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_USER_LONGACCURACY, 1, EXIF_FORMAT_RATIONAL);
	if (entry) exif_set_rational(entry->data, order, collection->UserLongitudeAccuracy);
	
	// UserLatitude
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_USER_LATITUDE);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_USER_LATITUDE, 1, EXIF_FORMAT_SRATIONAL);
	if (entry) exif_set_srational(entry->data, order, collection->UserLatitude);
	
	// UserLatAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_USER_LATACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_USER_LATACCURACY, 1, EXIF_FORMAT_RATIONAL);
	if (entry) exif_set_rational(entry->data, order, collection->UserLatitudeAccuracy);
	
	// UserAltitude
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_USER_ALTITUDE);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_USER_ALTITUDE, 1, EXIF_FORMAT_SRATIONAL);
	if (entry) exif_set_srational(entry->data, order, collection->UserAltitude);
	
	// UserAltAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_USER_ALTACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_USER_ALTACCURACY, 1, EXIF_FORMAT_RATIONAL);
	if (entry) exif_set_rational(entry->data, order, collection->UserAltitudeAccuracy);
	
	// UserEarthLavel
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_USER_EARTH_LEVEL);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_USER_EARTH_LEVEL, 1, EXIF_FORMAT_SRATIONAL);
	if (entry) exif_set_srational(entry->data, order, collection->UserEarthLevel);
	
	// UserAzimuth	
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_USER_AZIMUTH);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_USER_AZIMUTH, 1, EXIF_FORMAT_SRATIONAL);
	exif_set_srational(entry->data, order, collection->UserAzimuth);
	
	// UserAzimuthAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_USER_AZIMUTHACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_USER_AZIMUTHACCURACY, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->UserAzimuthAccuracy);

	// UserPitch
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_USER_PITCH);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_USER_PITCH, 1, EXIF_FORMAT_SRATIONAL);
	exif_set_srational(entry->data, order, collection->UserPitch);

	// UserPitchAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_USER_PITCHACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_USER_PITCHACCURACY, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->UserPitchAccuracy);

	// UserRoll
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_USER_ROLL);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_USER_ROLL, 1, EXIF_FORMAT_SRATIONAL);
	exif_set_srational(entry->data, order, collection->UserRoll);

	// UserRollAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_USER_ROLLACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_USER_ROLLACCURACY, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->UserRollAccuracy);

	// UserHViewAngle
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_USER_HVIEWANGLE);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_USER_HVIEWANGLE, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->UserHViewAngle);

	// UserHViewAngleAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_USER_HVIEWANGLEACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_USER_HVIEWANGLEACCURACY, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->UserHViewAngleAccuracy);

	// UserVViewAngle
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_USER_VVIEWANGLE);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_USER_VVIEWANGLE, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->UserVViewAngle);

	// UserVViewAngleAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_USER_VVIEWANGLEACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_USER_VVIEWANGLEACCURACY, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->UserVViewAngleAccuracy);


	// ResultLongitude
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_RESULT_LONGITUDE);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_RESULT_LONGITUDE, 1, EXIF_FORMAT_SRATIONAL);
	exif_set_srational(entry->data, order, collection->ResultLongitude);

	// ResultLongAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_RESULT_LONGACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_RESULT_LONGACCURACY, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->ResultLongitudeAccuracy);
	
	// ResultLatitude
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_RESULT_LATITUDE);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_RESULT_LATITUDE, 1, EXIF_FORMAT_SRATIONAL);
	exif_set_srational(entry->data, order, collection->ResultLatitude);
	
	// ResultLatAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_RESULT_LATACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_RESULT_LATACCURACY, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->ResultLatitudeAccuracy);
	
	// ResultAltitude
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_RESULT_ALTITUDE);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_RESULT_ALTITUDE, 1, EXIF_FORMAT_SRATIONAL);
	exif_set_srational(entry->data, order, collection->ResultAltitude);
	
	// ResultAltAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_RESULT_ALTACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_RESULT_ALTACCURACY, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->ResultAltitudeAccuracy);
	
	// ResultAzimuth
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_RESULT_AZIMUTH);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_RESULT_AZIMUTH, 1, EXIF_FORMAT_SRATIONAL);
	exif_set_srational(entry->data, order, collection->ResultAzimuth);

	// ResultAzimuthAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_RESULT_AZIMUTHACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_RESULT_AZIMUTHACCURACY, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->ResultAzimuthAccuracy);

	// ResultPitch
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_RESULT_PITCH);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_RESULT_PITCH, 1, EXIF_FORMAT_SRATIONAL);
	exif_set_srational(entry->data, order, collection->ResultPitch);

	// ResultPitchAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_RESULT_PITCHACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_RESULT_PITCHACCURACY, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->ResultPitchAccuracy);

	// ResultRoll
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_RESULT_ROLL);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_RESULT_ROLL, 1, EXIF_FORMAT_SRATIONAL);
	exif_set_srational(entry->data, order, collection->ResultRoll);

	// ResultRollAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_RESULT_ROLLACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_RESULT_ROLLACCURACY, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->ResultRollAccuracy);

	// ResultHViewAngle
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_RESULT_HVIEWANGLE);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_RESULT_HVIEWANGLE, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->ResultHViewAngle);

	// ResultHViewAngleAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_RESULT_HVIEWANGLEACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_RESULT_HVIEWANGLEACCURACY, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->ResultHViewAngleAccuracy);

	// ResultVViewAngle
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_RESULT_VVIEWANGLE);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_RESULT_VVIEWANGLE, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->ResultVViewAngle);

	// ResultVViewAngleAccuracy
	entry = exif_content_get_entry(exif->ifd[ifd], (ExifTag)SGEO_TAG_RESULT_VVIEWANGLEACCURACY);
	if (!entry)
		entry = create_tag(exif, ifd, (ExifTag)SGEO_TAG_RESULT_VVIEWANGLEACCURACY, 1, EXIF_FORMAT_RATIONAL);
	exif_set_rational(entry->data, order, collection->ResultVViewAngleAccuracy);
}

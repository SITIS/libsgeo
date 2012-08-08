/* ExifLibImpl.c
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
 
#include "ExifLibImpl.h"
#include "objects/sgeo-object_reader.h"
#include "objects/sgeo-object_writer.h"
#include "misc/checksum.h"
#include "misc/exif_type_reader.h"
#include "misc/exif_type_writer.h"
#include "libexif/exif-tag.h"
#include "misc/string_routines.h"
#include "misc/exif-error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

void ReadObjectsData(SGeoObject *objects_table, size_t objects_table_count, ExifData **exif_array, size_t exif_array_count)
{
	size_t table_it;
	for (table_it = 0; table_it < objects_table_count; table_it++)
	{
		objects_table[table_it].Data = getObjectData(&objects_table[table_it], exif_array, exif_array_count);
#ifdef _DEBUG
		if (objects_table[table_it].Data == NULL && objects_table[table_it].DataSize > 0)
		{
			assert(!(objects_table[table_it].Data == NULL && objects_table[table_it].DataSize > 0));
			objects_table[table_it].Data = NULL;
		}
#endif
		if (objects_table[table_it].Data == NULL && objects_table[table_it].DataSize > 0)
			objects_table[table_it].DataSize = 0; // recovery record
		objects_table[table_it].DataHash ^= Crc32(objects_table[table_it].Data, objects_table[table_it].DataSize);
	}
}
struct ImageInfo ReadImageInfo (const char* filepath)
{
	ExifData **exif_array = NULL;
	ExifData *exif = NULL;
	ExifEntry *entry;
	ExifByteOrder order;
	struct ImageInfo imageInfo = {0};
	ExifShort sgeo_version = 0;
	int i = 0;
	int count = 0;

	size_t length, size_char;

	exif_data_new_from_file(filepath, &exif_array, &count);
	exif = exif_array[0];
	if (!exif)	
	{
		return imageInfo;
	}

	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_0], (ExifTag)EXIF_TAG_MAKE);
    if (entry)
	{
		length = entry->size;
		if (length > sizeof(imageInfo.CameraMake))
			length = sizeof(imageInfo.CameraMake);
		memcpy(imageInfo.CameraMake, entry->data, length);
	}

	order = exif_data_get_byte_order (exif);

	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_0], (ExifTag)EXIF_TAG_MODEL);
    if (entry)
	{
		length = entry->size;
		if (length > sizeof(imageInfo.CameraModel))
			length = sizeof(imageInfo.CameraModel);
		memcpy(imageInfo.CameraModel, entry->data, length);
	}
	
	exif_mem_read(imageInfo.DateTimeOriginal, NULL, exif->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_ORIGINAL, sizeof(imageInfo.DateTimeOriginal));
	exif_mem_read(imageInfo.DateTimeDigitized, NULL, exif->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_DIGITIZED, sizeof(imageInfo.DateTimeDigitized));
	exif_short_read(&imageInfo.Orientation, NULL, exif->ifd[EXIF_IFD_0], EXIF_TAG_ORIENTATION, order);
	exif_rational_read(&imageInfo.FocalLength, NULL, exif->ifd[EXIF_IFD_EXIF], EXIF_TAG_FOCAL_LENGTH, order);
	// User exif comments
	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_EXIF], (ExifTag)EXIF_TAG_USER_COMMENT);
    if (entry && entry->size > 8)
	{
		size_char = 0;
		imageInfo.CommentsLength = 0;

		if (memcmp(entry->data, "UNICODE\0", 8) == 0)
			size_char = sizeof(utf16);
		else if (memcmp(entry->data, "ASCII\0\0\0", 8) == 0)
			size_char = 1;

		if (size_char != 0)
		{
			length = (entry->size - 8) / size_char;
			if (length > MAX_COMMENT) length = MAX_COMMENT;

			if (size_char > 1)// for ASCII set comment length zero
			{
				imageInfo.CommentsLength = length - 1;
				memcpy(imageInfo.Comments, entry->data + 8 + 2, length * size_char); // 2 - size BOM
			}
			else
				memcpy(imageInfo.Comments, entry->data + 8, length * size_char);
		}
	}
	
	// GPSDateStamp
	exif_mem_read(imageInfo.GPSDateStamp, &imageInfo.ExistGPSDateStamp, exif->ifd[EXIF_IFD_GPS], EXIF_TAG_GPS_DATE_STAMP, sizeof(imageInfo.GPSDateStamp));
	// GPSTimeStamp
	exif_rational_hms_to_int_seconds_read(&imageInfo.GPSTimeStamp, &imageInfo.ExistGPSTimeStamp, exif->ifd[EXIF_IFD_GPS], EXIF_TAG_GPS_TIME_STAMP, order);
	// SGEOVersionID
	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_SGEO], (ExifTag)SGEO_TAG_VERSIONID);
	if (entry && memcmp(entry->data, "SG", 2) == 0)
	{
		sgeo_version = exif_get_short(entry->data + 0x02, exif->priv->order);

		if (sgeo_version >= 4)
		{
			imageInfo.SGeo = read_sgeo_tags(exif->ifd[EXIF_IFD_SGEO], exif->priv->order);

			ReadObjectsData(imageInfo.SGeo.ObjectsTable, imageInfo.SGeo.ObjectsTableCount, exif_array, count);

			length = MIN(sizeof(imageInfo.SGeo.VersionID), entry->size);
			memcpy(imageInfo.SGeo.VersionID, entry->data, length);

			//imageInfo.SGeo.StaticMapCount = (ExifShort)count - 1;
			//imageInfo.SGeo.StaticMapData = read_sgeo_tags_map(exif_array + 1, EXIF_IFD_SGEO, imageInfo.SGeo.StaticMapCount);
		}
	}
	// GPSLatitudeRef
	exif_mem_read(imageInfo.GPSLatitudeRef, NULL, exif->ifd[EXIF_IFD_GPS], EXIF_TAG_GPS_LATITUDE_REF, sizeof(imageInfo.GPSLatitudeRef));
	// GPSLatitude
	exif_rational_dms_to_double_degrees_read(&imageInfo.GPSLatitude, &imageInfo.ExistGPSLatitude, exif->ifd[EXIF_IFD_GPS], EXIF_TAG_GPS_LATITUDE, order);
	// GPSLongitudeRef
	exif_mem_read(imageInfo.GPSLongitudeRef, NULL, exif->ifd[EXIF_IFD_GPS], EXIF_TAG_GPS_LONGITUDE_REF, sizeof(imageInfo.GPSLongitudeRef));
	// GPSLongitude
	exif_rational_dms_to_double_degrees_read(&imageInfo.GPSLongitude, &imageInfo.ExistGPSLongitude, exif->ifd[EXIF_IFD_GPS], EXIF_TAG_GPS_LONGITUDE, order);
	// GPSAltitudeRef
	exif_mem_read(&imageInfo.GPSAltitudeRef, NULL, exif->ifd[EXIF_IFD_GPS], EXIF_TAG_GPS_ALTITUDE_REF, sizeof(imageInfo.GPSAltitudeRef));
	// GPSAltitude
	exif_rational_to_double_read(&imageInfo.GPSAltitude, &imageInfo.ExistGPSAltitude, exif->ifd[EXIF_IFD_GPS], EXIF_TAG_GPS_ALTITUDE, order);
	// GPSImgDirectionRef
	exif_mem_read(imageInfo.GPSImgDirectionRef, NULL, exif->ifd[EXIF_IFD_GPS], EXIF_TAG_GPS_IMG_DIRECTION_REF, sizeof(imageInfo.GPSImgDirectionRef));
	// GPSImgDirection
	imageInfo.GPSImgDirection = -1;
	exif_rational_to_double_read(&imageInfo.GPSImgDirection, &imageInfo.ExistGPSImgDirection, exif->ifd[EXIF_IFD_GPS], EXIF_TAG_GPS_IMG_DIRECTION, order);

#ifdef _SGEO_FULL
	// image width
	exif_long_read(&imageInfo.ImageWidth, NULL, exif->ifd[EXIF_IFD_EXIF], EXIF_TAG_PIXEL_X_DIMENSION, order);
	// image height
	exif_long_read(&imageInfo.ImageHeight, NULL, exif->ifd[EXIF_IFD_EXIF], EXIF_TAG_PIXEL_Y_DIMENSION, order);
#endif


	for (i = 0; i < count; i++)
		exif_data_unref(exif_array[i]);
	return imageInfo;
}

void push_object_data_pointer(SGeoObject *objects_table, size_t objects_table_count, size_t *data_pointers)
{
	size_t i;
	for (i = 0; i < objects_table_count; i++)
	{
		*data_pointers = (size_t)objects_table->Data;
		data_pointers++;
		objects_table++;
	}
}
void pop_object_data_pointer(SGeoObject *objects_table, size_t objects_table_count, size_t *data_pointers)
{
	size_t i;
	for (i = 0; i < objects_table_count; i++)
	{
		objects_table->Data = (ExifByte *)*data_pointers;
		data_pointers++;
		objects_table++;
	}
}
JPEGSection *WriteObjectsData(SGeoObject *objects_table, size_t objects_table_count, ExifLong *sections_count)
{
	size_t table_it;
	JPEGSection *sections = NULL;// sections for object data
	*sections_count = 0;
	for (table_it = 0; table_it < objects_table_count; table_it++)
	{
		objects_table[table_it].DataHash = Crc32(objects_table[table_it].Data, objects_table[table_it].DataSize);
		createDataBlocskObjects(&objects_table[table_it], &sections, sections_count);
	}
	return sections;
}
void insertObjectTableBlocks(JPEGData *jdata, JPEGSection *sections, ExifLong sections_count)
{
	size_t sections_it;
	unsigned char *p = NULL;
	// insert nre sections after APP2 SGEO
	for (sections_it = 0; sections_it < jdata->count; sections_it++)
	{
		p = jdata->sections[sections_it].content._generic.data;
		if (p != NULL && is_sgeo_app2(p))
		{
			jdata->sections = (JPEGSection *)realloc(jdata->sections, sizeof(*jdata->sections) * (jdata->count + sections_count));
			memmove(jdata->sections + (sections_it + 1 + sections_count), jdata->sections + (sections_it + 1), sizeof(*jdata->sections) * (jdata->count - (sections_it + 1)));
			memcpy(jdata->sections + (sections_it + 1), sections, sizeof(*jdata->sections) * sections_count);
			jdata->count += sections_count;
			break;
		}
	}
}
void createObjectsUID(SGeoTags *sgeo)
{
	size_t i, j;
	size_t imei_uid_size = 20 - 8;
	char *buf = NULL;
	SGeoObject null_object = { 0 };
	SGeoObject *object = NULL;
	
	buf = (char* )malloc(imei_uid_size);

	for (i = 0; i < sgeo->ObjectsTableCount; i++)
	{
		object = &sgeo->ObjectsTable[i];

		if (memcmp(object, &null_object, sizeof(null_object) == 0))
			continue;

		for (j = 0; j < imei_uid_size; j++)
			if (object->UID[j] != 0)
				break;
		// if UID not created
		if (j == imei_uid_size)
			hex_string_to_hex_bin_unicode((wchar_t* )sgeo->DeviceIMEI, (char* )object->UID, imei_uid_size);// creation UID
	}
	free(buf);
}
int checkObjectsUID(SGeoTags *sgeo)
{
	size_t i, j;
	SGeoObject null_object = { 0 };
	SGeoObject *object = NULL;
	char *uids = NULL;
	int uids_count = 0;
	uids = (char* )malloc(sgeo->ObjectsTableCount * 20);// 20 - size UID
	for (i = 0; i < sgeo->ObjectsTableCount; i++)
	{
		object = &sgeo->ObjectsTable[i];
		if (memcmp(object, &null_object, sizeof(null_object) == 0))
			continue;

		for (j = 0; j < uids_count; j++)
			if (memcmp(object->UID, uids + uids_count * 20, sizeof(object->UID) == 0))
			{
				free(uids);
				return 1;
			}
		memcpy(uids + uids_count * 20, object->UID, sizeof(object->UID));
		uids_count++;
	}
	free(uids);
	return 0;
}
void WriteImageInfo (const char* input_file, const char* output_file, struct ImageInfo* imageInfo)
{
	ExifContent *econtent;
	ExifData **exif_array = NULL;
	ExifData *exif = NULL;
	ExifEntry *entry;
	ExifByteOrder order;
	JPEGData *jdata;
	JPEGSection *data_table_sections = NULL;
	ExifLong sections_count = 0;
	size_t data_pointers[60];
	size_t size, total;
	int i = 0;
	int count = 0;
	int uids_ok = 0;

	exif_data_new_from_file(input_file, &exif_array, &count);
	exif = exif_array[0];
	if (!exif)
	{
		exif = exif_data_new();
	}
	
	order = exif_data_get_byte_order(exif);
	
	// DateTimeOriginal
	exif_mem_write(exif->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_ORIGINAL, EXIF_FORMAT_ASCII, imageInfo->DateTimeOriginal, sizeof(imageInfo->DateTimeOriginal), NULL);
	exif_mem_write(exif->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_DIGITIZED, EXIF_FORMAT_ASCII, imageInfo->DateTimeDigitized, sizeof(imageInfo->DateTimeDigitized), NULL);
	// Orientation
	exif_short_write(exif->ifd[EXIF_IFD_0], EXIF_TAG_ORIENTATION, &imageInfo->Orientation, order, NULL);
	// User comments
	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_EXIF], EXIF_TAG_USER_COMMENT);

	size = imageInfo->CommentsLength * sizeof(utf16);
	total = 8 + size;
	
	if (entry)
	{
		free(entry->data);
		entry->components = total;
		entry->size = total;
		entry->data = (unsigned char* )malloc(total);
	}
	else if (size > 0)
		entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_USER_COMMENT, total, EXIF_FORMAT_UNDEFINED);
	
	if (entry)
	{
		memcpy(entry->data, "UNICODE\0", 8);
		memcpy(entry->data + 8, imageInfo->Comments, size);
	}
	
	if (imageInfo->ExistGPSLatitude != 0 && imageInfo->ExistGPSLongitude != 0)
	{
		// GPS Latitude
		exif_mem_write(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_LATITUDE_REF, EXIF_FORMAT_ASCII, imageInfo->GPSLatitudeRef, sizeof(imageInfo->GPSLatitudeRef), NULL);
		exif_double_degrees_to_rational_dms_write(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_LATITUDE, &imageInfo->GPSLatitude, order, &imageInfo->ExistGPSLatitude);
		// GPS Longitude
		exif_mem_write(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_LONGITUDE_REF, EXIF_FORMAT_ASCII, imageInfo->GPSLongitudeRef, sizeof(imageInfo->GPSLongitudeRef), NULL);
		exif_double_degrees_to_rational_dms_write(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_LONGITUDE, &imageInfo->GPSLongitude, order, &imageInfo->ExistGPSLongitude);
		// GPS Altitude
		exif_mem_write(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_ALTITUDE_REF, EXIF_FORMAT_BYTE, &imageInfo->GPSAltitudeRef, sizeof(imageInfo->GPSAltitudeRef), &imageInfo->ExistGPSAltitude);
		exif_double_to_rational_write(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_ALTITUDE, &imageInfo->GPSAltitude, order, &imageInfo->ExistGPSAltitude);
		// GPS ImgDirection
		exif_mem_write(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_IMG_DIRECTION_REF, EXIF_FORMAT_ASCII, imageInfo->GPSImgDirectionRef, sizeof(imageInfo->GPSImgDirectionRef), &imageInfo->ExistGPSImgDirection);
		exif_double_to_rational_write(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_IMG_DIRECTION, &imageInfo->GPSImgDirection, order, &imageInfo->ExistGPSImgDirection);
		// GPS TimeStamp
		exif_int_seconds_to_rational_hms_write(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_TIME_STAMP, &imageInfo->GPSTimeStamp, order, &imageInfo->ExistGPSTimeStamp);
		// GPS DateStamp
		exif_mem_write(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_DATE_STAMP, EXIF_FORMAT_ASCII, imageInfo->GPSDateStamp, sizeof(imageInfo->GPSDateStamp), &imageInfo->ExistGPSDateStamp);
	}
	createObjectsUID(&imageInfo->SGeo);
	uids_ok = checkObjectsUID(&imageInfo->SGeo);
	if (uids_ok == 0)
	{
		push_object_data_pointer(imageInfo->SGeo.ObjectsTable, imageInfo->SGeo.ObjectsTableCount, data_pointers);
		data_table_sections = WriteObjectsData(imageInfo->SGeo.ObjectsTable, imageInfo->SGeo.ObjectsTableCount, &sections_count);
		jdata = jpeg_data_new_from_file(input_file);
		add_sgeo_app2(jdata, &imageInfo->SGeo, exif);
		pop_object_data_pointer(imageInfo->SGeo.ObjectsTable, imageInfo->SGeo.ObjectsTableCount, data_pointers);
		if (data_table_sections != NULL)
			insertObjectTableBlocks(jdata, data_table_sections, sections_count);
		else
			*LIB_ERROR_CODE = ERROR_OBJECT_TABLE_INSERT;

		// remove SGEO from APP1
		econtent = exif->ifd[EXIF_IFD_SGEO];
		if (econtent != NULL)
		{
			exif_content_unref (econtent);
			exif->ifd[EXIF_IFD_SGEO] = NULL;
		}

		jpeg_data_set_exif_data (jdata, exif);
		jpeg_data_save_file (jdata, output_file);

		jpeg_data_unref (jdata);
		for (i = 0; i < count; i++)
			exif_data_unref(exif_array[i]);
	}
	else
		*LIB_ERROR_CODE = ERROR_OBJECT_UID_NOT_UNIQE;
}

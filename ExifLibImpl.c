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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "libjpeg/jpeg-data.h"

#define LONG_MAX_ 0x7fffffff
#define LONG_MIN_ (-0x7fffffff - 1)

struct ImageInfo ReadImageInfo (const char* filepath)
{
	ExifData *exif;
	ExifEntry *entry;
	ExifByteOrder order;
	struct ImageInfo imageInfo = {0};

	ExifRational Degrees, Minutes, Seconds;
	int deg, min, hour;
	double sec;
	size_t length;

	exif = exif_data_new_from_file(filepath);
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
	
	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_EXIF], (ExifTag)EXIF_TAG_DATE_TIME_ORIGINAL);
    if (entry)
        memcpy(imageInfo.DateTime, entry->data, sizeof(imageInfo.DateTime));
	
	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_0], (ExifTag)EXIF_TAG_ORIENTATION);
    if (entry) 
		imageInfo.Orientation = exif_get_short(entry->data, order);
	
	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_EXIF], (ExifTag)EXIF_TAG_FOCAL_LENGTH);
    if (entry) 	
		imageInfo.FocalLength = exif_get_rational(entry->data,order);
	
	// User comments
	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_EXIF], (ExifTag)EXIF_TAG_USER_COMMENT);
    if (entry && entry->size > 8)
	{
		if (memcmp(entry->data, "UNICODE\0", 8) == 0)
		{
			length = (entry->size - 8) / sizeof(utf16);
			if (length > MAX_COMMENT) length = MAX_COMMENT;

			imageInfo.CommentsLength = length;
			memcpy(imageInfo.Comments, entry->data + 8, length * sizeof(utf16));
		}
	}
	
	// GPSDateStamp
	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_DATE_STAMP);
    if (entry)
		memcpy(imageInfo.GPSDateStamp, entry->data, sizeof(imageInfo.GPSDateStamp));
	
	// GPSTimeStamp
	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_TIME_STAMP);
    if (entry) 
	{
		Degrees = exif_get_rational(entry->data, order);
		Minutes = exif_get_rational(entry->data + 0x08, order);
		Seconds = exif_get_rational(entry->data + 0x10, order);
		hour = Degrees.numerator / Degrees.denominator;
		min = Minutes.numerator / Minutes.denominator;
		sec = (double)Seconds.numerator / Seconds.denominator;
		imageInfo.GPSTimeStamp = hour * 3600 + min * 60 + sec;
	}
	
	// SGEOVersionID
	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_SGEO], (ExifTag)SGEO_TAG_VERSIONID);
	if (entry && memcmp(entry->data, "SG\0\4", 4) == 0)
	{
		imageInfo.SGeo = read_sgeo_tags(exif, EXIF_IFD_SGEO);
		memcpy(imageInfo.SGeo.VersionID, entry->data, 4);
	}
	
	// GPSLatitudeRef
	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_LATITUDE_REF);
	if (entry)
		memcpy(imageInfo.GPSLatitudeRef, entry->data, sizeof(imageInfo.GPSLatitudeRef));

	// GPSLatitude
	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_LATITUDE);
	if (entry) 	
	{
		Degrees = exif_get_rational(entry->data, order);
		Minutes = exif_get_rational(entry->data + 0x08, order);
		Seconds = exif_get_rational(entry->data + 0x10, order);
		
		if (Degrees.denominator != 0 && Minutes.denominator != 0 && Seconds.denominator != 0)
		{
			deg = Degrees.numerator / Degrees.denominator;
			min = Minutes.numerator / Minutes.denominator;
			sec = (double)Seconds.numerator / Seconds.denominator;
			imageInfo.GPSLatitude = decimal(deg, min, sec);
		}
	}
	
	// GPSLongitudeRef
	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_LONGITUDE_REF);
	if (entry) 	
		memcpy(imageInfo.GPSLongitudeRef, entry->data, sizeof(imageInfo.GPSLongitudeRef));
	
	// GPSLongitude
	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_LONGITUDE);
	if (entry) 
	{
		Degrees = exif_get_rational(entry->data, order);
		Minutes = exif_get_rational(entry->data + 0x08, order);
		Seconds = exif_get_rational(entry->data + 0x10, order);
		
		if (Degrees.denominator != 0 && Minutes.denominator != 0 && Seconds.denominator != 0)
		{
			deg = Degrees.numerator / Degrees.denominator;
			min = Minutes.numerator / Minutes.denominator;
			sec = (double)Seconds.numerator / Seconds.denominator;
			imageInfo.GPSLongitude = decimal(deg, min, sec);
		}
	}
	
	// GPSAltitudeRef
	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_ALTITUDE_REF);
	if (entry) 	
		imageInfo.GPSAltitudeRef = entry->data[0];
	
	// GPSAltitude
	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_ALTITUDE);
	if (entry)
	{
		Degrees = exif_get_rational(entry->data, order);
		if (Degrees.denominator != 0)
			imageInfo.GPSAltitude = (double)Degrees.numerator / Degrees.denominator;
	}
	
	// GPSImgDirectionRef
	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_IMG_DIRECTION_REF);
	if (entry) 	
		memcpy(imageInfo.GPSImgDirectionRef, entry->data, sizeof(imageInfo.GPSImgDirectionRef));
	
	// GPSImgDirection
	imageInfo.GPSImgDirection = -1;
	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_IMG_DIRECTION);
	if (entry)
	{
		Degrees = exif_get_rational(entry->data, order);
		if (Degrees.denominator != 0)
			imageInfo.GPSImgDirection = (double)Degrees.numerator / Degrees.denominator;
	}
	
	exif_data_unref(exif);
	return imageInfo;
}
				  
void WriteImageInfo (const char* input_file, const char* output_file, struct ImageInfo* imageInfo)
{
	ExifData *exif;
	ExifEntry *entry;
	ExifByteOrder order;
	JPEGData *jdata;
	size_t size, total;
	
	ExifRational deg, min, sec;
	ExifRational time_hours, time_seconds, time_minutes;
	double dvalue;
	
	exif = exif_data_new_from_file(input_file);
	if (!exif)
	{
		exif = exif_data_new();
	}
	
	order = exif_data_get_byte_order(exif);
	
	// DateTimeOriginal
    entry = exif_content_get_entry(exif->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_ORIGINAL);
    if (!entry)
	{
        entry = create_tag(exif, (ExifTag)EXIF_IFD_EXIF, EXIF_TAG_DATE_TIME_ORIGINAL, 20, EXIF_FORMAT_ASCII);
		memcpy(entry->data, imageInfo->DateTime, 20);
	}
	
	// Orientation
	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_0], (ExifTag) EXIF_TAG_ORIENTATION);
	if (!entry)
		entry = create_tag(exif, EXIF_IFD_0, (ExifTag) EXIF_TAG_ORIENTATION, 1, EXIF_FORMAT_SHORT);
	exif_set_short(entry->data, order, imageInfo->Orientation);
	
	// User comments
	entry = exif_content_get_entry(exif->ifd[EXIF_IFD_EXIF], EXIF_TAG_USER_COMMENT);

	size = imageInfo->CommentsLength * sizeof(utf16);
	total = 8 + size;
	
	if (entry)
	{
		free(entry->data);
		entry->components = total;
		entry->size = total;
		entry->data = malloc(total);
	}
	else if (size > 0)
		entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_USER_COMMENT, total, EXIF_FORMAT_UNDEFINED);
	
	if (entry)
	{
		memcpy(entry->data, "UNICODE\0", 8);
		memcpy(entry->data + 8, imageInfo->Comments, size);
	}
	
	if (imageInfo->GPSLatitude != 0 || imageInfo->GPSLongitude != 0)
	{
		// GPS LatitudeRef
		entry = exif_content_get_entry(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_LATITUDE_REF);
		if (!entry)
			entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_LATITUDE_REF, 2, EXIF_FORMAT_ASCII);	
		memcpy(entry->data, imageInfo->GPSLatitudeRef, 2);
		
		// GPS Latitude
		entry = exif_content_get_entry(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_LATITUDE);
		if (!entry)
			entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_LATITUDE, 3, EXIF_FORMAT_RATIONAL);
		
		dvalue = imageInfo->GPSLatitude;
		deg = DoubleToRational( degrees(dvalue) );
		min = DoubleToRational( minutes(dvalue) );
		sec = DoubleToRational( seconds(dvalue) );
		
		exif_set_rational(entry->data, order, deg);
		exif_set_rational(entry->data + 0x08, order, min);
		exif_set_rational(entry->data + 0x10, order, sec);

		// GPS LongitudeRef
		entry = exif_content_get_entry(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_LONGITUDE_REF);
		if (!entry)
			entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_LONGITUDE_REF, 2, EXIF_FORMAT_ASCII);	
		memcpy(entry->data, imageInfo->GPSLongitudeRef, 2);
		
		// GPS Longitude
		entry = exif_content_get_entry(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_LONGITUDE);
		if (!entry)
			entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_LONGITUDE, 3, EXIF_FORMAT_RATIONAL);
		
		dvalue = imageInfo->GPSLongitude;
		deg = DoubleToRational( degrees(dvalue) );
		min = DoubleToRational( minutes(dvalue) );
		sec = DoubleToRational( seconds(dvalue) );
		
		exif_set_rational(entry->data, order, deg);
		exif_set_rational(entry->data + 0x08, order, min);
		exif_set_rational(entry->data + 0x10, order, sec);

		// GPS AltitudeRef
		entry = exif_content_get_entry(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_ALTITUDE_REF);
		if (!entry)
			entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_ALTITUDE_REF, 1, EXIF_FORMAT_BYTE);	
		entry->data[0] = imageInfo->GPSAltitudeRef;

		// GPS Altitude
		entry = exif_content_get_entry(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_ALTITUDE);
		if (!entry)
			entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_ALTITUDE, 1, EXIF_FORMAT_RATIONAL);	
		exif_set_rational(entry->data, order, DoubleToRational(imageInfo->GPSAltitude));

		// GPS ImgDirectionRef
		entry = exif_content_get_entry(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_IMG_DIRECTION_REF);
		if (!entry)
			entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_IMG_DIRECTION_REF, 2, EXIF_FORMAT_ASCII);	
		memcpy(entry->data, imageInfo->GPSImgDirectionRef, 2);

		// GPS ImgDirection
		entry = exif_content_get_entry(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_IMG_DIRECTION);
		if (!entry)
			entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_IMG_DIRECTION, 1, EXIF_FORMAT_RATIONAL);	
		exif_set_rational(entry->data, order, DoubleToRational(imageInfo->GPSImgDirection));
		
		//GPS TimeStamp
		entry = exif_content_get_entry(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_TIME_STAMP);
		if (!entry)
			entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_TIME_STAMP, 3, EXIF_FORMAT_RATIONAL);
		dvalue = imageInfo->GPSTimeStamp;
		int hour = dvalue / 3600;
		time_hours = DoubleToRational(hour);
		int min = (dvalue - hour * 3600) / 60;
		time_minutes = DoubleToRational(min);
		time_seconds = DoubleToRational(dvalue - hour * 3600 - min * 60);
			
		exif_set_rational(entry->data, order, time_hours);
		exif_set_rational(entry->data + 0x08, order, time_minutes);
		exif_set_rational(entry->data + 0x10, order, time_seconds);
		
		// GPS DateStamp
		entry = exif_content_get_entry(exif->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_DATE_STAMP);
		if (!entry)
			entry = create_tag(exif, (ExifTag)EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_DATE_STAMP, 11, EXIF_FORMAT_ASCII);
		memcpy(entry->data, imageInfo->GPSDateStamp, 11);
	}

	write_sgeo_tags(exif, EXIF_IFD_SGEO, &imageInfo->SGeo);

	jdata = jpeg_data_new_from_file(input_file);
	jpeg_data_set_exif_data (jdata, exif);
	jpeg_data_save_file (jdata, output_file);

	jpeg_data_unref (jdata);
	exif_data_unref(exif);
}

ExifRational DoubleToRational(double x)
{
	const double eps = 1.0e-8;
	
	ExifRational rational = {0};
	
	long intPart,a,prevNum,num,prevDen,den,tmp;
	double z,approxAns;
	
	if (x == 0)
	{
		rational.numerator = 0;
		rational.denominator = 1;
		return rational;
	}
	
	if (fabs(x) > (double) LONG_MAX_ || fabs(x) < (1.0f /  LONG_MAX_))
	{
		rational.numerator = 0;
		rational.denominator = 0;
		return rational;
	}
	
	if (x < 0.0) x = -x;
	
	intPart = (long) x;
	z = x - intPart;
	
	if (z != 0)
	{
		z = 1.0 / z;
		a = (long) z;
		z = z - a;
		prevNum = 0;
		num = 1;
		prevDen = 1;
		den = a;
		approxAns = ((double) den * intPart + num) / den;
		while (fabs((x - approxAns) / x) >= eps)
		{
			z = 1.0 / z;
			a = (long) z;
			z = z - a;
			// deal with too-big numbers:
			if ((double) a * num + prevNum > LONG_MAX_ || (double) a * den + prevDen > LONG_MAX_)
				break;
			tmp = a * num + prevNum;
			prevNum = num;
			num = tmp;
			tmp = a * den + prevDen;
			prevDen = den;
			den = tmp;
			approxAns = ((double) den * intPart + num) / den;
		}
		rational.numerator = (den * intPart + num);
		rational.denominator = den;
	}
	else
	{
		rational.numerator = intPart;
		rational.denominator = 1;
	}
	
	return rational;
}

ExifSRational DoubleToSRational(double x)
{
	ExifRational r = DoubleToRational(x);
	ExifSRational s;
	int sign = x >= 0 ? 1 : -1;
	s.numerator = sign * r.numerator;
	s.denominator = r.denominator;
	
	return s;
}

int degrees(double c)
{
	return (int) fabs(c);
}

int minutes(double c)
{
	double degs = fabs(c);
	return (int)((degs - floor(degs)) * 60);
}

double seconds(double c)
{
	return (fabs(c) - degrees(c) - (double)minutes(c) / 60) * 3600;		
}

double decimal(int degrees, int min, double sec)
{
	return (degrees + (double)min / 60 + sec / 3600);
}
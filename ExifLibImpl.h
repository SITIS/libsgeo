/* ExifLibImpl.h
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
 
#ifndef _EXIF_LIB_IMPL_H_
#define _EXIF_LIB_IMPL_H_

#include "misc/ImageInfo.h"

struct ImageInfo ReadImageInfo(const char* filepath);
void WriteImageInfo(const char* input_file, const char* output_file, struct ImageInfo* imginfo);

ExifRational DoubleToRational(double x);
ExifSRational DoubleToSRational(double x);

int degrees(double c);
int minutes(double c);
double seconds(double c);
double decimal(int degrees, int min, double sec);

#endif //_EXIF_LIB_IMPL_H_
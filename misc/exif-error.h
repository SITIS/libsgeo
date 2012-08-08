/* exif-error.h
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
 
#ifndef _EXIF_ERROR_H_
#define _EXIF_ERROR_H_

enum
{
	ERROR_OBJECT_UID_NOT_UNIQE = 1,
	ERROR_FILE_NOT_OPEN,
	ERROR_READING_APP2_SGEO,
	ERROR_OBJECT_TABLE_INSERT
} WriteErrors;

extern int *LIB_ERROR_CODE;

#endif //_EXIF_ERROR_H_
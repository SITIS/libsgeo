/* thumbnail_reader.c
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

#include "thumbnail_reader.h"
#include <stddef.h>
#include "libexif/exif-content.h"
#include "libexif/exif-loader.h"
#include <memory.h>
#include <malloc.h>
#include "misc/exif_type_reader.h"

void read_thumbnail(const char *fileName, void **thimbnail, int *size, ExifShort *Orientation)
{
	ExifLoader *loader = NULL;
	ExifData *exif_data = NULL;
	ExifContent *tn = NULL;
	ExifEntry *entry = NULL;
	int i = 0, j = 0;

	*thimbnail = NULL;
	*size = 0;
	*Orientation = -1;

	// allocate memory for loader
	loader = exif_loader_new();
	// read 1024 bytes and fill loader
	exif_loader_write_file (loader, fileName);
	exif_data = exif_loader_get_data (loader);
	if (exif_data != NULL && exif_data->priv != NULL)
	{
		exif_short_read(Orientation, NULL, exif_data->ifd[EXIF_IFD_0], EXIF_TAG_ORIENTATION, exif_data->priv->order);

		if (exif_data->data != NULL && exif_data->size > 0)
		{
			*size = exif_data->size;
			*thimbnail = malloc(*size);
			memcpy(*thimbnail, exif_data->data, *size);
		}
	}
	// memory free
	exif_loader_unref (loader);

}

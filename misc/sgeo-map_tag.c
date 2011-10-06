/* sgeo-map_tag.c
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
#include "sgeo-map_tag.h"
#include "libexif/exif-loader.h"
#include <stdlib.h>

void free_ListExifLoaderItem(ListExifLoader *item)
{
	if (item->prev != NULL && item->next != NULL)
	{
		item->prev->next = item->next;
		item->next->prev = item->prev;
	}
	else if (item->prev != NULL)
	{
		item->prev->next = NULL;
	}
	else if (item->next != NULL)
	{
		item->next->prev = NULL;
	}
	exif_loader_unref (item->exif_loader);
	free(item);
}
void free_ListExifLoader(ListExifLoader *list_exif_loader)
{
	ListExifLoader *first = list_exif_loader;
	if (first == NULL)
		return;
	// find first item
	while (first->prev != NULL)
		first = first->prev;
	// free all
	while (first->next != NULL)
		free_ListExifLoaderItem(first->next);
	free_ListExifLoaderItem(first);
}
int size_ListExifLoader(ListExifLoader *list_exif_loader)
{
	int size = 0;
	ListExifLoader *first = list_exif_loader;
	if (first != NULL)
	{
		size = 1;
		// find first item
		while (first->prev != NULL)
			first = first->prev;
		while (first->next != NULL)
		{
			size++;
			first = first->next;
		}
	}
	return size;
}
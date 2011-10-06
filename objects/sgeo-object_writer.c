/* sgeo-object_writer.c
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

#include "sgeo-object_writer.h"
#include "objects/sgeo-object_structures.h"
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

void appendDataBlock(JPEGSection **sections, ExifLong *sections_count);
lpDataUnitHeader getUnusedData(lpDataBlockHeader data_block_header);


JPEGSection* getLastSection(JPEGSection **sections, ExifLong *sections_count)
{
	JPEGSection *last_section = NULL;
	if (*sections == NULL)
		appendDataBlock(sections, sections_count);

	last_section = &(*sections)[*sections_count - 1];
	
	return last_section;
}
ExifShort getDataBlockFreeSpace(JPEGSection *section)
{
	ExifShort free_space = 0;
	lpDataBlockHeader data_block_header = NULL;
	lpDataUnitHeader data_unit_header = NULL;

	if (section == NULL)
		return free_space;

	data_block_header = (lpDataBlockHeader)section->content._generic.data;
	if (section->marker == JPEG_MARKER_APP2 &&
		memcmp(data_block_header->magic, DataBlockMagic, 16) == 0
		)
	{
		data_unit_header = getUnusedData(data_block_header);
		if (data_unit_header != NULL)
			free_space = 0xfffd - ((size_t)data_unit_header - (size_t)data_block_header);
	}
	return free_space;
}
void appendDataBlock(JPEGSection **sections, ExifLong *sections_count)
{
	lpDataBlockHeader data_block_header = NULL;
	lpDataUnitHeader data_unit_header = NULL;
	JPEGSection *last_section = NULL;

	(*sections_count)++;
	*sections = (JPEGSection *)realloc(*sections, sizeof(**sections) * *sections_count);
	last_section = &(*sections)[*sections_count - 1];

	last_section->marker = JPEG_MARKER_APP2;
	last_section->content._generic.size = sizeof(*data_block_header) + sizeof(data_unit_header->id); // 2 - size id object;
	last_section->content._generic.data = (unsigned char *)malloc(last_section->content._generic.size);

	data_block_header = (lpDataBlockHeader)last_section->content._generic.data;
	memcpy(data_block_header->magic, DataBlockMagic, 16);
	data_block_header->number = *sections_count;
	data_block_header->next_number = 0;
	data_block_header->first_object_offset = (ExifShort)sizeof(*data_block_header);

	data_unit_header =(lpDataUnitHeader)((unsigned char *)data_block_header + data_block_header->first_object_offset);
	memset(data_unit_header->id, 0, sizeof(data_unit_header->id));

	if (*sections_count > 1)
		((lpDataBlockHeader)((*sections)[*sections_count - 2].content._generic.data))->next_number = data_block_header->number;
}
lpDataUnitHeader getUnusedData(lpDataBlockHeader data_block_header)
{
	lpDataUnitHeader unused_data_unit = NULL;
	lpDataUnitHeader data_unit_header = NULL;
	data_unit_header = (lpDataUnitHeader)((unsigned char *)data_block_header + data_block_header->first_object_offset);
	while (memcmp(data_unit_header->id, data_unit_header_null.id, sizeof(data_unit_header_null.id)) != 0 &&
		((size_t)data_unit_header - (size_t)data_block_header) < 0xfffd)
		data_unit_header = (lpDataUnitHeader)((unsigned char *)data_unit_header + data_unit_header->size);

	if (memcmp(data_unit_header->id, data_unit_header_null.id, sizeof(data_unit_header_null.id)) == 0 &&
		((size_t)data_unit_header - (size_t)data_block_header) <= 0xfffd)
		unused_data_unit = data_unit_header;
	return unused_data_unit;
}
void createDataBlocskObjects(SGeoObject *sgeo_object, JPEGSection **sections, ExifLong *sections_count)
{
	JPEGSection *last_section = NULL;
	ExifShort free_space;
	lpDataBlockHeader data_block_header = NULL;
	lpDataUnitHeader data_unit_header = NULL;
	ExifByte *p = sgeo_object->Data;
	ExifShort data_write_size;
	ExifLong data_size = sgeo_object->DataSize;
	size_t new_size;
	int is_Data_change = 0;
	int duh_id_size, duh_size_size;

	if (memcmp(sgeo_object->UID, data_unit_header_null.id, sizeof(data_unit_header_null.id)) == 0)
	{
		assert("last 8 bytes UID must not be all zero values");
		return;
	}

	duh_id_size = sizeof(data_unit_header->id);
	duh_size_size = sizeof(data_unit_header->size);

	if (p == NULL || data_size == 0)
		return;


	while (data_size != 0)
	{
		last_section = getLastSection(sections, sections_count);
		free_space = getDataBlockFreeSpace(last_section);
		if (free_space <= duh_id_size)
		{
			appendDataBlock(sections, sections_count);
			last_section = getLastSection(sections, sections_count);

			free_space = getDataBlockFreeSpace(last_section);
			if (free_space <= duh_id_size)
			{
				assert("error creation data block");
				return;
			}
		}
		if (free_space > 20)
		{
			data_block_header = (lpDataBlockHeader)last_section->content._generic.data;
			if (last_section->marker == JPEG_MARKER_APP2 &&
				memcmp(data_block_header->magic, DataBlockMagic, 16) == 0
				)
			{
				if (free_space < data_size + duh_id_size + duh_size_size)
					data_write_size = free_space;
				else
					data_write_size = data_size + duh_id_size + duh_size_size;

				if (data_size < (ExifLong)(data_write_size - (duh_id_size + duh_size_size)))
				{
					assert("error calculation block memory size");
					return;
				}
				new_size = last_section->content._generic.size + data_write_size - duh_id_size;
				if (new_size < 0xfffe - duh_id_size)
					new_size += duh_id_size;
				if (new_size > 0xffff)
				{
					assert("size section overload (max=0xffff)");
					return;
				}

				last_section->content._generic.data = (unsigned char *)realloc(last_section->content._generic.data, new_size);
				data_block_header = (lpDataBlockHeader)((unsigned char *)last_section->content._generic.data);
				data_unit_header = (lpDataUnitHeader)getUnusedData(data_block_header);
				memcpy(data_unit_header->id, sgeo_object->UID, sizeof(data_unit_header->id));
				data_unit_header->size = data_write_size;

				// + (duh_id_size + duh_size_size)
				memcpy(last_section->content._generic.data + last_section->content._generic.size + duh_size_size, p, data_write_size - (duh_id_size + duh_size_size));
				if (is_Data_change == 0)
				{
					sgeo_object->Data = (ExifByte *)data_block_header->number; // is corrupts data, need stored Data pointer before call createDataBlocskObjects
					is_Data_change = 1;
				}
				last_section->content._generic.size = new_size;
				data_size = data_size - (data_write_size - (duh_id_size + duh_size_size));
				if (data_size != 0)
					p = p + (data_write_size - (duh_id_size + duh_size_size));
				if (new_size < 0xfffe - duh_id_size)
				{
					data_unit_header = (lpDataUnitHeader)((unsigned char *)data_unit_header + data_unit_header->size);
					memset(data_unit_header->id, 0, sizeof(data_unit_header->id));
				}
			}
			else
			{
				assert("section error");
				return;
			}
		}
	}
}

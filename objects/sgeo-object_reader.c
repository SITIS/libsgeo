/* sgeo-object_reader.c
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

#include "sgeo-object_reader.h"
#include "objects/sgeo-object_structures.h"
#include <stdlib.h>
#include <assert.h>
#include <memory.h>

ExifData* findDataBlockByNumber(ExifLong ref_on_data_block, ExifData **exif_array, size_t exif_array_count)
{
	size_t i;
	lpDataBlockHeader pDataBlock = NULL;

	for (i = 0; i < exif_array_count; i++)
	{
		pDataBlock = (lpDataBlockHeader)exif_array[i]->data;
		if (pDataBlock != NULL &&
			memcmp(pDataBlock->magic, DataBlockMagic, 16) == 0 &&
			ref_on_data_block == pDataBlock->number)
		{
			return exif_array[i];
		}
	}

	return NULL;
}

lpDataUnitHeader getDataUnit(lpDataBlockHeader pdata_block_header, ExifByte id[20])
{
	lpDataUnitHeader unit = NULL;
	if (memcmp(pdata_block_header, DataBlockMagic, 16) != 0)
		return NULL;
	unit = (lpDataUnitHeader)((unsigned char *)pdata_block_header + pdata_block_header->first_object_offset);
	while (memcmp(unit->id, id, sizeof(id)) != 0 &&
		memcmp(unit->id, data_unit_header_null.id, sizeof(data_unit_header_null.id)) != 0 && 
		   ((unsigned char *)unit - (unsigned char *)pdata_block_header < 0xfffd) // maximum 0xfffd
		  )
	{
		unit = (lpDataUnitHeader)((unsigned char *)unit + unit->size);
	}
	return memcmp(unit->id, data_unit_header_null.id, sizeof(data_unit_header_null.id)) == 0 ? NULL : unit;
}

lpDataRun getDataRun(SGeoObject *sgeo_object, ExifData **exif_array, size_t exif_array_count)
{
	ExifData *exif = NULL;
	ExifLong ref_on_data_block = (ExifLong)sgeo_object->Data;
	ExifLong data_size, readed_bytes;
	lpDataUnitHeader data_unit = NULL;
	lpDataRun data_run = NULL, pdata_run = NULL;
	ExifLong data_run_count = 0;
	lpDataBlockHeader data_block = NULL;

	data_run_count++;
	data_run = (lpDataRun)calloc(data_run_count, sizeof(*data_run));
	pdata_run = data_run;

	if (sgeo_object->UID != 0 && ref_on_data_block != 0)
	{
		data_size = sgeo_object->DataSize;
		readed_bytes = 0;


		while (readed_bytes < data_size && ref_on_data_block != 0)
		{
			exif = findDataBlockByNumber(ref_on_data_block, exif_array, exif_array_count);
			data_block = (lpDataBlockHeader)exif->data;
			data_unit = getDataUnit(data_block, sgeo_object->UID);
			if (data_unit != NULL)
			{
				pdata_run->p = (unsigned char *)data_unit + sizeof(*data_unit);
				pdata_run->size = data_unit->size - sizeof(*data_unit);
				readed_bytes += pdata_run->size;

				data_run_count++;
				data_run = (lpDataRun)realloc(data_run, sizeof(*data_run) * data_run_count);
				pdata_run = data_run + (data_run_count - 1);
				memset(pdata_run, 0, sizeof(*pdata_run)); // last data run have pointer NULL and size zero
			}
			ref_on_data_block = data_block->next_number;
		}
	}

	return data_run;
}

ExifByte* getObjectData(SGeoObject *sgeo_object, ExifData **exif_array, size_t exif_array_count)
{
	ExifByte *data = NULL,
	         *p_data = NULL;
	lpDataRun data_run = NULL,
	          p_data_run = NULL;
	ExifLong size = 0;

	data_run = getDataRun(sgeo_object, exif_array, exif_array_count);
	p_data_run = data_run;
	while (p_data_run->p != NULL)
	{
		size += p_data_run->size;
		p_data_run++;
	}

	if (size > 0)
	{
		if (size == sgeo_object->DataSize)
		{
			data = (ExifByte* )malloc(size);
			p_data = data;
			p_data_run = data_run;
			while (p_data_run->p != NULL)
			{
				memcpy(p_data, p_data_run->p, p_data_run->size);
				p_data += p_data_run->size;
				p_data_run++;
			}
		}
		else
			assert(size != sgeo_object->DataSize);
	}
	free(data_run);
	return data;
}

/* Copyright 2003-2012, Stephen Fryatt (info@stevefryatt.org.uk)
 *
 * This file is part of SFLib:
 *
 *   http://www.stevefryatt.org.uk/software/
 *
 * Licensed under the EUPL, Version 1.2 only (the "Licence");
 * You may not use this work except in compliance with the
 * Licence.
 *
 * You may obtain a copy of the Licence at:
 *
 *   http://joinup.ec.europa.eu/software/page/eupl
 *
 * Unless required by applicable law or agreed to in
 * writing, software distributed under the Licence is
 * distributed on an "AS IS" basis, WITHOUT WARRANTIES
 * OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * See the Licence for the specific language governing
 * permissions and limitations under the Licence.
 */

/**
 * \file: heap.c
 *
 * Flexlib-based static heap implementation, providing malloc-like calls
 * on an OS_Heap managed heap inside the first block of a flex heap.
 */

#include <stdlib.h>
#include <string.h>

/* Acorn C header files. */

#include "flex.h"

/* OS-Lib header files. */

#include "oslib/os.h"
#include "oslib/osheap.h"
#include "oslib/messagetrans.h"
#include "oslib/wimp.h"

/* SF-Lib header files. */

#include "heap.h"
#include "errors.h"

#include "debug.h"


#define HEAP_GRANULARITY 1024							/**< The size of standard allocations from flex.			*/
#define HEAP_BLOCK_OHEAD 16							/**< The amount of memory requuired by OS_Heap to manage a heap block.	*/


static byte	*heap_anchor = NULL;
static int	heap_block_size = HEAP_GRANULARITY;


/* Initialise the heap.  Flex must have been initialised via flex_init() before
 * this is called.
 *
 * This is an external interface, documented in heap.h
 */

osbool heap_initialise(void)
{
	os_error	*error;

	flex_alloc((flex_ptr) &heap_anchor, heap_block_size);

	error = xosheap_initialise(heap_anchor, heap_block_size);
	if (error != NULL);
		error_report_program(error);

	return TRUE;
}


/* Allocate a block of memory from the heap.
 *
 * This is an external interface, documented in heap.h
 */

void *heap_alloc(size_t size)
{
	void		*block;
	os_error	*error;

	size += sizeof(int);

	error = xosheap_alloc(heap_anchor, size, &block);
	if (error != NULL) {
		if (flex_extend ((flex_ptr) &heap_anchor, heap_block_size + size + HEAP_BLOCK_OHEAD)) {
			osheap_resize(heap_anchor, size + HEAP_BLOCK_OHEAD);
			heap_block_size += (size + HEAP_BLOCK_OHEAD);

			error = xosheap_alloc(heap_anchor, size, &block);
			if (error != NULL)
				error_report_program (error);
		} else {
			block = NULL;
		}
	}

	if (block != NULL) {
		*(int *)block = size - sizeof (int);
		block = (int *) block + 1;
	}

	return block;
}


/* Free a block of memory previously claimed from the heap.
 *
 * This is an external interface, documented in heap.h
 */

void heap_free(void *ptr)
{
	int	shrink;

	ptr = (int *) ptr - 1;

	osheap_free(heap_anchor, ptr);

	shrink = osheap_resize_no_fail(heap_anchor, 0x80000000);

	if (shrink < 0) {
		heap_block_size += shrink;
		flex_extend((flex_ptr) &heap_anchor, heap_block_size);
	}
}


/* Change the size of a block of memory previously claimed from the heap.
 * This may result in the block moving.
 *
 * This is an external interface, documented in heap.h
 */

void *heap_extend(void *ptr, size_t new_size)
{
	int		change;
	void		*block;
	os_error	*error;

	new_size += sizeof(int);
	ptr = (int *) ptr - 1;
	change = new_size - *(int *)ptr;

	error = xosheap_realloc(heap_anchor, ptr, change, &block);
	if (error != NULL) {
		if (flex_extend((flex_ptr) &heap_anchor, heap_block_size + new_size + HEAP_BLOCK_OHEAD)) {
			osheap_resize(heap_anchor, new_size + HEAP_BLOCK_OHEAD);
			heap_block_size += (new_size + HEAP_BLOCK_OHEAD);

			error = xosheap_realloc(heap_anchor, ptr, change, &block);
			if (error != NULL)
				error_report_program(error);
		} else {
			block = NULL;
		}
	}

	if (block != NULL) {
		*(int *) block = new_size - sizeof(int);
		block = (int *) block + 1;
	}

	return block;
}


/* Find the size of a block of memory previously claimed from the heap.
 *
 * This is an external interface, documented in heap.h
 */

size_t heap_size(void *ptr)
{
	return *((size_t *) ptr - 1);
}


/* Perform a strdup() on a string, using memory cliamed by heap_alloc().
 *
 * This is an external interface, documented in heap.h
 */

char *heap_strdup(char *string)
{
	size_t		size = strlen(string) + 1;
	char		*new = heap_alloc(size);

	if (new != NULL)
		strncpy(new, string, size);

	return new;
}


/* Return the pointer to the heap base.
 *
 * This is an external interface, documented in heap.h
 */

byte *heap_base(void)
{
	return heap_anchor;
}

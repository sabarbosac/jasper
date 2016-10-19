/*
 * Copyright (c) 1999-2000 Image Power, Inc. and the University of
 *   British Columbia.
 * Copyright (c) 2001-2002 Michael David Adams.
 * All rights reserved.
 */

/* __START_OF_JASPER_LICENSE__
 * 
 * JasPer License Version 2.0
 * 
 * Copyright (c) 2001-2006 Michael David Adams
 * Copyright (c) 1999-2000 Image Power, Inc.
 * Copyright (c) 1999-2000 The University of British Columbia
 * 
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person (the
 * "User") obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 * 
 * 1.  The above copyright notices and this permission notice (which
 * includes the disclaimer below) shall be included in all copies or
 * substantial portions of the Software.
 * 
 * 2.  The name of a copyright holder shall not be used to endorse or
 * promote products derived from the Software without specific prior
 * written permission.
 * 
 * THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL PART OF THIS
 * LICENSE.  NO USE OF THE SOFTWARE IS AUTHORIZED HEREUNDER EXCEPT UNDER
 * THIS DISCLAIMER.  THE SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS
 * "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS.  IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
 * INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
 * FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.  NO ASSURANCES ARE
 * PROVIDED BY THE COPYRIGHT HOLDERS THAT THE SOFTWARE DOES NOT INFRINGE
 * THE PATENT OR OTHER INTELLECTUAL PROPERTY RIGHTS OF ANY OTHER ENTITY.
 * EACH COPYRIGHT HOLDER DISCLAIMS ANY LIABILITY TO THE USER FOR CLAIMS
 * BROUGHT BY ANY OTHER ENTITY BASED ON INFRINGEMENT OF INTELLECTUAL
 * PROPERTY RIGHTS OR OTHERWISE.  AS A CONDITION TO EXERCISING THE RIGHTS
 * GRANTED HEREUNDER, EACH USER HEREBY ASSUMES SOLE RESPONSIBILITY TO SECURE
 * ANY OTHER INTELLECTUAL PROPERTY RIGHTS NEEDED, IF ANY.  THE SOFTWARE
 * IS NOT FAULT-TOLERANT AND IS NOT INTENDED FOR USE IN MISSION-CRITICAL
 * SYSTEMS, SUCH AS THOSE USED IN THE OPERATION OF NUCLEAR FACILITIES,
 * AIRCRAFT NAVIGATION OR COMMUNICATION SYSTEMS, AIR TRAFFIC CONTROL
 * SYSTEMS, DIRECT LIFE SUPPORT MACHINES, OR WEAPONS SYSTEMS, IN WHICH
 * THE FAILURE OF THE SOFTWARE OR SYSTEM COULD LEAD DIRECTLY TO DEATH,
 * PERSONAL INJURY, OR SEVERE PHYSICAL OR ENVIRONMENTAL DAMAGE ("HIGH
 * RISK ACTIVITIES").  THE COPYRIGHT HOLDERS SPECIFICALLY DISCLAIM ANY
 * EXPRESS OR IMPLIED WARRANTY OF FITNESS FOR HIGH RISK ACTIVITIES.
 * 
 * __END_OF_JASPER_LICENSE__
 */

/*
 * Memory Allocator
 *
 * $Id$
 */

/******************************************************************************\
* Includes.
\******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

/* We need the prototype for memset. */
#include <string.h>

#include "jasper/jas_malloc.h"
#include "jasper/jas_debug.h"
#include "jasper/jas_math.h"

/******************************************************************************\
* Code.
\******************************************************************************/

#if defined(DEBUG_MEMALLOC)
#include "../../../local/src/memalloc.c"
#endif

#if !defined(DEBUG_MEMALLOC)

/******************************************************************************\
* Basic memory allocation and deallocation primitives.
\******************************************************************************/

void *jas_malloc(size_t size)
{
	void *result;
	JAS_DBGLOG(101, ("jas_malloc called with %zu\n", size));
	result = malloc(size);
	JAS_DBGLOG(100, ("jas_malloc(%zu) -> %p\n", size, result));
	return result;
}

void *jas_realloc(void *ptr, size_t size)
{
	void *result;
	JAS_DBGLOG(101, ("jas_realloc called with %x,%zu\n", ptr, size));
	result = realloc(ptr, size);
	JAS_DBGLOG(100, ("jas_realloc(%p, %zu) -> %p\n", ptr, size, result));
	return result;
}

void jas_free(void *ptr)
{
	JAS_DBGLOG(100, ("jas_free(%p)\n", ptr));
	free(ptr);
}

/******************************************************************************\
* Additional memory allocation and deallocation primitives
* (mainly for overflow checking).
\******************************************************************************/

void *jas_alloc2(size_t num_elements, size_t element_size)
{
	size_t size;
	if (!jas_safe_size_mul(num_elements, element_size, &size)) {
		return 0;
	}
	return jas_malloc(size);
}

void *jas_alloc3(size_t num_arrays, size_t array_size, size_t element_size)
{
	size_t size;
	if (!jas_safe_size_mul(array_size, element_size, &size) ||
	  !jas_safe_size_mul(size, num_arrays, &size)) {
		return 0;
	}
	return jas_malloc(size);
}

void *jas_realloc2(void *ptr, size_t num_elements, size_t element_size)
{
	size_t size;
	if (!jas_safe_size_mul(num_elements, element_size, &size)) {
		return 0;
	}
	return jas_realloc(ptr, size);
}

void *jas_calloc(size_t num_elements, size_t element_size)
{
	void *ptr;
	size_t size;
	if (!jas_safe_size_mul(num_elements, element_size, &size)) {
		return 0;
	}
	if (!(ptr = jas_malloc(size))) {
		return 0;
	}
	memset(ptr, 0, size);
	return ptr;
}

#endif

/* Copyright 2016, Stephen Fryatt (info@stevefryatt.org.uk)
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
 * \file: strdup.c
 *
 * Implementation of strdup() for use when compiling with Norcroft.
 */

#ifdef __CC_NORCROFT

#include "strdup.h"

/* ANSII C header files. */

#include <string.h>
#include <stdlib.h>

char *strdup(const char *string)
{
	char	*copy;
	size_t	size;

	size = strlen(string) + 1;

	copy = malloc(size);
	if (copy == NULL)
		return NULL;

	strncpy(copy, string, size);
	copy[size - 1] = '\0';

	return copy;
}
#endif


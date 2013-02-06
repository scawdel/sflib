/* Copyright 2003-2013, Stephen Fryatt
 *
 * This file is part of SFLib:
 *
 *   http://www.stevefryatt.org.uk/software/
 *
 * Licensed under the EUPL, Version 1.1 only (the "Licence");
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
 * \file: debug.c
 *
 * Debug support for writing data to Reporter.
 */

/* Acorn C Header files. */

#include "kernel.h"

/* OS-Lib header files. */

#include "oslib/os.h"
#include "oslib/report.h"

/* ANSII C header files. */

#include <stdarg.h>
#include <stdio.h>

/* SF-Lib header files. */

#include "debug.h"


/* Print a string to Reporter, using the standard printf() syntax and
 * functionality.  Expanded text is limited to 256 characters including
 * a null terminator.  If Reporter is not loaded, no characters will be
 * written.
 *
 * This function is an external interface, documented in debug.c.
 */

int debug_printf(char *cntrl_string, ...)
{
	char		s[256];
	int		ret;
	va_list		ap;

	if (xos_swi_number_from_string("Report_Text0", NULL) != NULL)
		return 0;

	va_start(ap, cntrl_string);
	ret = vsnprintf(s, sizeof(s), cntrl_string, ap);
	report_text0(s);

	return ret;
}


/* Copyright 2003-2020, Stephen Fryatt (info@stevefryatt.org.uk)
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
 * \file: errors.c
 *
 * Wimp error handling and message box support.
 */

/* OS-Lib header files. */

#include "oslib/wimp.h"
#include "oslib/wimpspriteop.h"
#include "oslib/messagetrans.h"

/* SF-Lib header files. */

#include "errors.h"
#include "msgs.h"
#include "string.h"

#ifdef __CC_NORCROFT
#include "strdup.h"
#endif

/* ANSII C header files. */

#include <string.h>
#include <stdlib.h>

#define APP_NAME_LOOKUP_LENGTH 256						/**< The size of the buffer used to look up application name tokens.		*/
#define ERROR_BUTTON_LENGTH 256							/**< The size of the buffer for expanding custom button message tokens.		*/
#define ERROR_NUMBER 255							/**< The error number used for the report boxes.				*/


static char			*error_app_name = NULL;				/**< The application name, as used in error messages.				*/
static char			*error_app_sprite = NULL;			/**< The application sprite, as used in error messages.				*/
static void			(*error_close_down_function)(void);		/**< Unused.									*/

static wimp_error_box_selection		error_wimp_os_report(os_error *error,
		wimp_error_box_flags type, wimp_error_box_flags buttons, char *custom_buttons);
static void				error_complete_msgs_block(os_error *error, char *token, char *a, char *b, char *c, char *d);
static void				error_complete_block(os_error *error, char *message);



/* Initialise the error message module.
 *
 * This is an external interface, documented in errors.h
 */

void error_initialise(char *name, char *sprite, void (*closedown)(void))
{
	char	lookup_buffer[APP_NAME_LOOKUP_LENGTH];

	error_close_down_function = closedown;

	if (name != NULL) {
		if (error_app_name != NULL)
			free(error_app_name);

		msgs_lookup(name, lookup_buffer, APP_NAME_LOOKUP_LENGTH);
		error_app_name = strdup(lookup_buffer);
	}

	if (sprite != NULL) {
		if (error_app_sprite != NULL)
			free(error_app_sprite);

		msgs_lookup(sprite, lookup_buffer, APP_NAME_LOOKUP_LENGTH);
		error_app_sprite = strdup(lookup_buffer);
	}
}


/**
 * Display a Wimp error box on the screen, using the specified type.  Either use
 * the specified standard buttons, or a set of custom buttons.
 *
 * \param *error		An Error Block defining the error and the message
 *				to display.
 * \param type			The error box type, from wimp_ERROR_BOX_CATEGORY_INFO,
 *				wimp_ERROR_BOX_CATEGORY_ERROR, wimp_ERROR_BOX_CATEGORY_PROGRAM
 *				or wimp_ERROR_BOX_CATEGORY_QUESTION.
 * \param buttons		The buttons to use, if *custom_buttons is NULL, from
 *				wimp_ERROR_BOX_OK_ICON and wimp_ERROR_BOX_CANCEL_ICON .
 * \param *custom_buttons	A comma-separated list of custom button texts.
 * \return			The selected button, counting from 1.
 */

static wimp_error_box_selection error_wimp_os_report(os_error *error, wimp_error_box_flags type,
		wimp_error_box_flags buttons, char *custom_buttons)
{
	wimp_error_box_selection	click;
	wimp_error_box_flags		flags;
	char				*name, *sprite;

	name = (error_app_name != NULL) ? error_app_name : "Application";
	sprite = (error_app_sprite != NULL) ? error_app_sprite : "application";

	if (custom_buttons != NULL && *custom_buttons != '\0') {
		flags = wimp_ERROR_BOX_GIVEN_CATEGORY | (type << wimp_ERROR_BOX_CATEGORY_SHIFT);
		click = wimp_report_error_by_category(error, flags, name, sprite, wimpspriteop_AREA, custom_buttons);
	} else {
		flags = wimp_ERROR_BOX_GIVEN_CATEGORY | buttons | (type << wimp_ERROR_BOX_CATEGORY_SHIFT);
		click = wimp_report_error_by_category(error, flags, name, sprite, wimpspriteop_AREA, NULL);
	}

	return click;
}


/**
 * Complete an OS Error block using a MessageTrans lookup.
 *
 * \param *error		Pointer to the error block to be completed.
 * \param *token		Pointer to the MessageTrans token for the message.
 * \param *a			Pointer to the %0 substitution, or NULL.
 * \param *b			Pointer to the %1 substitution, or NULL.
 * \param *c			Pointer to the %2 substitution, or NULL.
 * \param *d			Pointer to the %3 substitution, or NULL.
 */

static void error_complete_msgs_block(os_error *error, char *token, char *a, char *b, char *c, char *d)
{
	if (error == NULL || token == NULL)
		return;

	error->errnum = ERROR_NUMBER;
	msgs_param_lookup(token, error->errmess, os_ERROR_LIMIT, a, b, c, d);
}


/**
 * Complete an OS Error block using a supplied text string.
 *
 * \param *error		Pointer to the error block to be completed.
 * \param *message		Pointer to the string to be used for the message.
 */

static void error_complete_block(os_error *error, char *message)
{
	if (error == NULL || message == NULL)
		return;

	error->errnum = ERROR_NUMBER;
	string_copy(error->errmess, message, os_ERROR_LIMIT);
}


/* Display a Wimp error box of type wimp_ERROR_BOX_CATEGORY_ERROR, containing
 * details of the error as contained in an Error Block.
 *
 * This is an external interface, documented in errors.h
 */

wimp_error_box_selection error_report_os_error(os_error *error, wimp_error_box_flags buttons)
{
	if (error == NULL)
		return wimp_ERROR_BOX_SELECTED_NOTHING;

	return error_wimp_os_report(error, wimp_ERROR_BOX_CATEGORY_ERROR, buttons, NULL);
}


/* Open a Wimp error box of type wimp_ERROR_BOX_CATEGORY_INFO, containg the
 * message looked up via the given MessageTrans token and an OK button.
 *
 * This is an external interface, documented in errors.h
 */

wimp_error_box_selection error_msgs_report_info(char *token)
{
	return error_msgs_param_report_info(token, NULL, NULL, NULL, NULL);
}


/* Open a Wimp error box of type wimp_ERROR_BOX_CATEGORY_INFO, containg the
 * message looked up via the given MessageTrans token and the supplied
 * parameters, and an OK button.
 *
 * This is an external interface, documented in errors.h
 */

wimp_error_box_selection error_msgs_param_report_info(char *token, char *a, char *b, char *c, char *d)
{
	os_error	error;

	error_complete_msgs_block(&error, token, a, b, c, d);
	return error_wimp_os_report(&error, wimp_ERROR_BOX_CATEGORY_INFO, wimp_ERROR_BOX_OK_ICON, NULL);
}


/* Open a Wimp error box of type wimp_ERROR_BOX_CATEGORY_INFO, containg the
 * given message and an OK button.
 *
 * This is an external interface, documented in errors.h
 */

wimp_error_box_selection error_report_info(char *message)
{
	os_error	error;

	error_complete_block(&error, message);
	return error_wimp_os_report(&error, wimp_ERROR_BOX_CATEGORY_INFO, wimp_ERROR_BOX_OK_ICON, NULL);
}


/* Open a Wimp error box of type wimp_ERROR_BOX_CATEGORY_ERROR, containg the
 * message looked up via the given MessageTrans token and an OK button.
 *
 * This is an external interface, documented in errors.h
 */

wimp_error_box_selection error_msgs_report_error(char *token)
{
	return error_msgs_param_report_error(token, NULL, NULL, NULL, NULL);
}


/* Open a Wimp error box of type wimp_ERROR_BOX_CATEGORY_ERROR, containg the
 * message looked up via the given MessageTrans token and the supplied
 * paramaters, and an OK button.
 *
 * This is an external interface, documented in errors.h
 */

wimp_error_box_selection error_msgs_param_report_error(char *token, char *a, char *b, char *c, char *d)
{
	os_error	error;

	error_complete_msgs_block(&error, token, a, b, c, d);
	return error_wimp_os_report(&error, wimp_ERROR_BOX_CATEGORY_ERROR, wimp_ERROR_BOX_OK_ICON, NULL);
}


/* Open a Wimp error box of type wimp_ERROR_BOX_CATEGORY_ERROR, containg the
 * given message and an OK button.
 *
 * This is an external interface, documented in errors.h
 */

wimp_error_box_selection error_report_error(char *message)
{
	os_error	error;

	error_complete_block(&error, message);
	return error_wimp_os_report(&error, wimp_ERROR_BOX_CATEGORY_ERROR, wimp_ERROR_BOX_OK_ICON, NULL);
}


/* Open a Wimp error box of type wimp_ERROR_BOX_CATEGORY_QUESTION, containg the
 * message looked up via the given MessageTrans token and either OK and
 * Cancel buttons or buttons as specified in the comma-separated list
 * contained in the buttons token.
 *
 * This is an external interface, documented in errors.h
 */

wimp_error_box_selection error_msgs_report_question(char *token, char *buttons)
{
	return error_msgs_param_report_question(token, buttons, NULL, NULL, NULL, NULL);
}


/* Open a Wimp error box of type wimp_ERROR_BOX_CATEGORY_QUESTION, containg the
 * message looked up via the given MessageTrans token and the supplied parameters,
 * and either OK and Cancel buttons or buttons as specified in the comma-separated
 * list contained in the buttons token.
 *
 * This is an external interface, documented in errors.h
 */

wimp_error_box_selection error_msgs_param_report_question(char *token, char *buttons, char *a, char *b, char *c, char *d)
{
	os_error	error;
	char		button_text[ERROR_BUTTON_LENGTH];

	error_complete_msgs_block(&error, token, a, b, c, d);

	if (buttons != NULL)
		msgs_lookup(buttons, button_text, ERROR_BUTTON_LENGTH);

	return error_wimp_os_report(&error, wimp_ERROR_BOX_CATEGORY_QUESTION,
			wimp_ERROR_BOX_OK_ICON | wimp_ERROR_BOX_CANCEL_ICON, (buttons == NULL) ? NULL : button_text);
}


/* Open a Wimp error box of type wimp_ERROR_BOX_CATEGORY_QUESTION, containg the
 * given message and either OK and Cancel buttons or buttons as specified in
 * the comma-separated list.
 *
 * This is an external interface, documented in errors.h
 */

wimp_error_box_selection error_report_question(char *message, char *buttons)
{
	os_error	error;

	error_complete_block(&error, message);

	return error_wimp_os_report(&error, wimp_ERROR_BOX_CATEGORY_QUESTION,
			wimp_ERROR_BOX_OK_ICON | wimp_ERROR_BOX_CANCEL_ICON, buttons);
}


/* Open a Wimp error box of type wimp_ERROR_BOX_CATEGORY_PROGRAM, containg the
 * message looked up via the given MessageTrans token and a Cancel button.
 *
 * This function never returns.
 *
 * This is an external interface, documented in errors.h
 */

void error_msgs_report_fatal(char *token)
{
	error_msgs_param_report_fatal(token, NULL, NULL, NULL, NULL);
}


/* Open a Wimp error box of type wimp_ERROR_BOX_CATEGORY_PROGRAM, containg the
 * message looked up via the given MessageTrans token and supplied parameters,
 * and a Cancel button.
 *
 * This function never returns.
 *
 * This is an external interface, documented in errors.h
 */

void error_msgs_param_report_fatal(char *token, char *a, char *b, char *c, char *d)
{
	os_error	error;

	error_complete_msgs_block(&error, token, a, b, c, d);
	error_wimp_os_report(&error, wimp_ERROR_BOX_CATEGORY_PROGRAM, wimp_ERROR_BOX_CANCEL_ICON, NULL);
	exit(1);
}


/* Open a Wimp error box of type wimp_ERROR_BOX_CATEGORY_PROGRAM, containg the
 * given message and a Cancel button.
 *
 * This function never returns.
 *
 * This is an external interface, documented in errors.h
 */

void error_report_fatal(char *message)
{
	os_error	error;

	error_complete_block(&error, message);
	error_wimp_os_report(&error, wimp_ERROR_BOX_CATEGORY_PROGRAM, wimp_ERROR_BOX_CANCEL_ICON, NULL);
	exit(1);
}


/* Open a Wimp error box of type wimp_ERROR_BOX_CATEGORY_PROGRAM, containg
 * details of the error in an Error Block and a Cancel button.
 *
 * This function never returns.
 */

void error_report_program(os_error *error)
{
	if (error == NULL)
		return;

	error_wimp_os_report(error, wimp_ERROR_BOX_CATEGORY_PROGRAM, wimp_ERROR_BOX_CANCEL_ICON, NULL);
	exit(1);
}

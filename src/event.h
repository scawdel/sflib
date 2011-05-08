/* SF-Lib - Event.h
 *
 * Version 0.10 (3 May 2010)
 */

#ifndef SFLIB_EVENT
#define SFLIB_EVENT

#include "oslib/types.h"

/**
 * Accept and process a wimp event.
 *
 * \param  event	The Wimp event code to be handled.
 * \param  block	The Wimp poll block.
 * \param  pollword	The Wimp pollword.
 * \return		TRUE if the event was handled; else FALSE.
 */

osbool event_process_event(wimp_event_no event, wimp_block *block, int pollword);


/**
 * Add a window redraw event handler for the specified window.
 *
 * \param  w		The window handle to attach the action to.
 * \param  *callback()	The callback function to use on the event.
 * \return		TRUE if the handler was registered; else FALSE.
 */

osbool event_add_window_redraw_event(wimp_w w, void (*callback)(wimp_draw *draw));


/**
 * Add a window open event handler for the specified window.
 *
 * \param  w		The window handle to attach the action to.
 * \param  *callback()	The callback function to use on the event.
 * \return		TRUE if the handler was registered; else FALSE.
 */

osbool event_add_window_open_event(wimp_w w, void (*callback)(wimp_open *open));


/**
 * Add a window close event handler for the specified window.
 *
 * \param  w		The window handle to attach the action to.
 * \param  *callback()	The callback function to use on the event.
 * \return		TRUE if the handler was registered; else FALSE.
 */

osbool event_add_window_close_event(wimp_w w, void (*callback)(wimp_close *close));


/**
 * Add a pointer leaving event handler for the specified window.
 *
 * \param  w		The window handle to attach the action to.
 * \param  *callback()	The callback to use on the event.
 * \return		TRUE if the handler was registered; else FALSE.
 */

osbool event_add_window_pointer_leaving_event(wimp_w w, void (*callback)(wimp_leaving *leaving));


/**
 * Add a pointer entering event handler for the specified window.
 *
 * \param  w		The window handle to attach the action to.
 * \param  *callback()	The callback to use on the event.
 * \return		TRUE if the handler was registered; else FALSE.
 */

osbool event_add_window_pointer_entering_event(wimp_w w, void (*callback)(wimp_entering *entering));


/**
 * Add a mouse click (pointer) event handler for the specified window.
 *
 * If the window has a window menu attached, this handler is not called for
 * Menu clicks over the work area.
 *
 * \param  w		The window handle to attach the action to.
 * \param  *callback()	The callback to use on the event.
 * \return		TRUE if the handler was registered; else FALSE.
 */

osbool event_add_window_mouse_event(wimp_w w, void (*callback)(wimp_pointer *pointer));


/**
 * Add a keypress event handler for the specified window.
 *
 * \param  w		The window handle to attach the action to.
 * \param  *callback()	The callback to use on the event.
 * \return		TRUE if the handler was registered; else FALSE.
 */

osbool event_add_window_key_event(wimp_w w, void (*callback)(wimp_key *key));


/**
 * Add a scroll event handler for the specified window.
 *
 * \param  w		The window handle to attach the action to.
 * \param  *callback()	The callback to use on the event.
 * \return		TRUE if the handler was registered; else FALSE.
 */

osbool event_add_window_scroll_event(wimp_w w, void (*callback)(wimp_scroll *scroll));


/**
 * Add a lose caret event handler for the specified window.
 *
 * \param  w		The window handle to attach the action to.
 * \param  *callback()	The callback to use on the event.
 * \return		TRUE if the handler was registered; else FALSE.
 */

osbool event_add_window_lose_caret_event(wimp_w w, void (*callback)(wimp_caret *caret));


/**
 * Add a gain caret event handler for the specified window.
 *
 * \param  w		The window handle to attach the action to.
 * \param  *callback()	The callback to use on the event.
 * \return		TRUE if the handler was registered; else FALSE.
 */

osbool event_add_window_gain_caret_event(wimp_w w, void (*callback)(wimp_caret *caret));


/**
 * Register a menu to the specified window: this will then be opened whenever
 * there is a menu click within the work area (even over icons).
 *
 * If a menu is registered, no events related to it will be passed back from
 * event_process_event() -- even if specific handlers are registed as NULL.
 *
 * \param  w		The window handle to attach the menu to.
 * \param  *menu	The menu handle.
 * \param  iconbar	TRUE if the menu is an iconbar menu; else FALSE.
 * \return		TRUE if the handler was registered; else FALSE.
 */

osbool event_add_window_menu(wimp_w w, wimp_menu *menu, osbool iconbar);


/**
 * Add a menu prepare event handler for the specified window.
 *
 * The callback function takes the associated window handle, the associated
 * menu handle and wimp pointer data (which is NULL on a reopen).
 *
 * \param  w		The window handle to attach the action to.
 * \param  *callback()	The callback to use on the event.
 * \return		TRUE if the handler was registered; else FALSE.
 */

osbool event_add_window_menu_prepare(wimp_w w, void (*callback)(wimp_w w, wimp_menu *m, wimp_pointer *pointer));


/**
 * Add a menu selection event handler for the specified window.
 *
 * \param  w		The window handle to attach the action to.
 * \param  *callback()	The callback to use on the event.
 * \return		TRUE if the handler was registered; else FALSE.
 */

osbool event_add_window_menu_selection(wimp_w w, void (*callback)(wimp_w w, wimp_menu *m, wimp_selection *selection));


/**
 * Add a menu close event handler for the specified window.
 *
 * \param  w		The window handle to attach the action to.
 * \param  *callback()	The callback to use on the event.
 * \return		TRUE if the handler was registered; else FALSE.
 */

osbool event_add_window_menu_close(wimp_w w, void (*callback)(wimp_w w, wimp_menu *m));


/**
 * Add a menu warning event handler for the specified window.
 *
 * \param  w		The window handle to attach the action to.
 * \param  *callback()	The callback to use on the event.
 * \return		TRUE if the handler was registered; else FALSE.
 */

osbool event_add_window_menu_warning(wimp_w w, void (*callback)(wimp_w w, wimp_menu *m, wimp_message_menu_warning *warning));


/**
 * Add a user data pointer for the specified window.
 *
 * \param  w		The window handle to attach the data to.
 * \param  *data	The data to attach.
 * \return		TRUE if the handler was registered; else FALSE.
 */

osbool event_add_window_user_data(wimp_w w, void *data);


/**
 * Return the user data block associated with the specified window.
 *
 * \param  w		The window to locate the data for.
 * \return		A pointer to the user data, or NULL.
 */

void *event_get_window_user_data(wimp_w w);


/**
 * Remove a window and its associated event details from the records.
 *
 * \param  w		The window to remove the data for.
 */

void event_delete_window(wimp_w w);


/**
 * Add a message handler for the given user message.
 *
 * \param message		The message number.
 * \param *message_action	The callback function to handle the message.
 * \return			TRUE if the hander was registered; else FALSE.
 */

osbool event_add_message_handler(int message, osbool (*message_action)(wimp_message *message, wimp_event_no reason));


/**
 * Set a handler for the next drag box event and any Null Polls in between.
 * If either handler is NULL it will not be called; both will be cancelled on
 * the next User_Drag_Box event to be received.
 *
 * Null Polls can be passed on to the application by returning 1 from
 * (drag_null_poll)(); returning 0 causes event_process_event() to also return 0.
 *
 * \param  *drag_end		A callback function for the drag end event.
 * \param  *drag_null_poll	A callback function for Null Polls during the drag.
 * \param  *data		Private data to be passed to the callback routines.
 * \return			TRUE if the handler was registerd; else FALSE.
 */

osbool event_set_drag_handler(void (*drag_end)(wimp_dragged *dragged, void *data), int (*drag_null_poll)(void *data), void *data);


/**
 * Set a variable to store a pointer to the currently open menu block.
 *
 * \param  **menu		Pointer to a wimp_menu * to take the pointers.
 * \return			TRUE if the variable was registerd; else FALSE.
 */

osbool event_set_menu_pointer(wimp_menu **menu_handle);

#endif


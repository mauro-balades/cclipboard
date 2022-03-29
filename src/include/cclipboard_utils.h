

#ifndef CCLIPBOARD_UTILS_H
#define CCLIPBOARD_UTILS_H

#include "cclipboard-x11.h"

namespace cclipboard_utils {
    void handle_x11_clipboard_events();
    void handle_x11_clipboard_events(x11_clipboard* self) {
        xcb_generic_event_t* event;

        while(true) {
            std::lock_guard<std::mutex> guard(self->m_lock);
            event = xcb_poll_for_event(self->_xcb_connection);

            if(event == 0) {
                usleep(100);
                continue;
            }

            switch(event->response_type & ~0x80) {
                case XCB_SELECTION_NOTIFY:
                    self->m_paste_reply = xcb_get_property_reply(
                        self->_xcb_connection,
                        xcb_get_property(
                            self->_xcb_connection, false, self->_xcb_window,
                            self->_xcb_atoms[selection::result], XCB_ATOM_ANY,
                            0, -1
                        ),
                        NULL
                    );
                    break;

                case XCB_SELECTION_CLEAR:
                    if(self->m_copy_data) {
                        delete self->m_copy_data;
                        self->m_copy_data = nullptr;
                    }
                    break;

                case XCB_SELECTION_REQUEST: {
                    if(self->m_copy_data == nullptr)
                        break;

                    auto sel_event = (xcb_selection_request_event_t*) event;
                    auto notify_event = new xcb_selection_notify_event_t {
                        .response_type = XCB_SELECTION_NOTIFY,
                        .time = XCB_CURRENT_TIME,
                        .requestor = sel_event->requestor,
                        .selection = sel_event->selection,
                        .target = sel_event->target,
                        .property = 0
                    };

                    if(sel_event->target == self->_xcb_atoms[selection::utf8]) {
                        xcb_change_property(
                            self->_xcb_connection, XCB_PROP_MODE_REPLACE,
                            sel_event->requestor, sel_event->property,
                            sel_event->target, 8,
                            self->m_copy_data->m_length, self->m_copy_data->m_data
                        );

                        notify_event->property = sel_event->property;
                    }

                    xcb_send_event(self->_xcb_connection, false, sel_event->requestor,
                        XCB_EVENT_MASK_PROPERTY_CHANGE, (char*) notify_event);
                    xcb_flush(self->_xcb_connection);
                    delete notify_event;
                }
            }

            free(event);
        }
    }
}

#endif /* CCLIPBOARD_UTILS_H */

#include <stdio.h> // printf
#include <xcb/xcb.h>
#include <string.h>

#include <thread>
#include <mutex>
#include <atomic>
#include <unistd.h>

#include "cclipboard_data.h"
#include "cclipboard_utils.h"
#include "cclipboard-config.h"

#ifdef CCLIPBOARD_BUILD_X11

#ifndef CCLIPBOARD_PASTE_H
#define CCLIPBOARD_PASTE_H

static const char* atom_names[] = {
    "CLIPBOARD",
    "UTF8_STRING",
    "RESULT"
};

enum selection {
    clipboard = 0,
    utf8,
    result
};

class x11_clipboard {

    public:
        x11_clipboard() {
            _xcb_connection = xcb_connect(NULL, NULL);

            atom_reply();
            create_screen();

            m_paste_reply = nullptr;
            m_copy_data = nullptr;

            m_event_thread = new std::thread(cclipboard_utils::handle_x11_clipboard_events, this);
            m_event_thread->detach();
        }

        virtual ~x11_clipboard() {
            delete m_event_thread;
            if(m_copy_data != nullptr)
                delete m_copy_data;

            xcb_destroy_window(_xcb_connection, _xcb_window);
            xcb_disconnect(_xcb_connection);
        }

        virtual void copy(const char* text, size_t length) {
            std::lock_guard<std::mutex> guard(m_lock);

            if(m_copy_data != nullptr)
                delete m_copy_data;

            m_copy_data = new copy_data(text, length);
            xcb_set_selection_owner(_xcb_connection, _xcb_window, _xcb_atoms[selection::clipboard], XCB_CURRENT_TIME);
            xcb_flush(_xcb_connection);
        }

        virtual void paste(std::string& dest) {
            {
                std::lock_guard<std::mutex> guard(m_lock);
                auto sel_owner = std::unique_ptr<xcb_get_selection_owner_reply_t>(
                    xcb_get_selection_owner_reply(
                        _xcb_connection,
                        xcb_get_selection_owner(_xcb_connection, _xcb_atoms[selection::clipboard]),
                        NULL
                    )
                );
                
                if(sel_owner->owner == _xcb_window) {
                    dest = m_copy_data->m_data;
                    return;
                }
                
                xcb_convert_selection(
                    _xcb_connection, _xcb_window,
                    _xcb_atoms[selection::clipboard], _xcb_atoms[selection::utf8],
                    _xcb_atoms[selection::result], XCB_CURRENT_TIME
                );
                xcb_flush(_xcb_connection);
            }

            while(true) {
                if(m_paste_reply != nullptr) {
                    char* value = (char*) xcb_get_property_value(m_paste_reply);
                    value[m_paste_reply.load()->value_len] = '\0';
                    
                    dest = value;
                    delete m_paste_reply.load();
                    m_paste_reply = nullptr;
                    break;
                }

                usleep(1e4);
            }
        }

        friend class clipboard;

    private:


        xcb_connection_t* _xcb_connection;
        xcb_window_t _xcb_window;

        xcb_atom_t _xcb_atoms[3];

        std::thread* m_event_thread;
        std::mutex m_lock;

        copy_data* m_copy_data;
        std::atomic<xcb_get_property_reply_t*> m_paste_reply;

        void atom_reply() {
            for(int i=0; i < 3; i++) {
                const char* name = atom_names[i];
                auto reply = xcb_intern_atom_reply(
                    _xcb_connection,
                    xcb_intern_atom(_xcb_connection, false, strlen(name), name),
                    NULL
                );

                _xcb_atoms[i] = reply->atom;
                free(reply);
            }
        }

        void create_screen() {
            xcb_screen_t* screen = xcb_setup_roots_iterator(xcb_get_setup(_xcb_connection)).data;

            _xcb_window = xcb_generate_id(_xcb_connection);
            uint32_t mask_values[] = { XCB_EVENT_MASK_PROPERTY_CHANGE };
            xcb_create_window(
                _xcb_connection, XCB_COPY_FROM_PARENT, _xcb_window, screen->root,
                0, 0, 1, 1, 0,
                XCB_WINDOW_CLASS_COPY_FROM_PARENT, screen->root_visual,
                XCB_CW_EVENT_MASK, mask_values
            );
        }
};

#endif /* CCLIPBOARD_PASTE_H */
#endif /* CCLIPBOARD_BUILD_X11 */

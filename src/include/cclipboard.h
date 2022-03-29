
#ifndef CCLIPBOARD_H
#define CCLIPBOARD_H

#include <stdio.h>
#include <string>

#include "cclipboard_options.h"
#include "cclipboard-x11.h"

namespace cclipboard {

    class CClipboard {

        public:
            CClipboard();
            CClipboard(CClipboard_options* p_options);
            // ~CClipboard();

            char* paste();
            void copy(std::string p_string);
            // TODO: void copy(const char* p_string);

        private:
            CClipboard_options _options;
            x11_clipboard* _clipboard = new x11_clipboard;

    };
}


#endif /* CCLIPBOARD_H */

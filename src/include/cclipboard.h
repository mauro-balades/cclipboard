
#include "cclipboard_options.h"
#include "cclipboard-x11.h"

namespace cclipboard {

    class CClipboard {

        public:
            CClipboard();
            CClipboard(CClipboard_options* p_options);
            // ~CClipboard();

            char* paste();

        private:
            CClipboard_options _options;
            x11_clipboard* _clipboard = new x11_clipboard;

    };
}

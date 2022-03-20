
#include "cclipboard_options.h"

namespace cclipboard {

    class CClipboard {

        public:
            CClipboard();
            CClipboard(CClipboard_options* p_options);
            // ~CClipboard();

            char* paste();

        private:
            CClipboard_options _options;

    };
}

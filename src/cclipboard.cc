
#include "cclipboard_options.h"
#include "cclipboard.h"
#include "cclipboard_paste-x11.h"

#ifndef CCLIPBOARD_H
#define CCLIPBOARD_H


namespace cclipboard {

  CClipboard::CClipboard() {

  }

  CClipboard::CClipboard(CClipboard_options* p_options) {

  }

  char* CClipboard::paste() {
    return _paste();
  }
}

#endif /* CCLIPBOARD_H */

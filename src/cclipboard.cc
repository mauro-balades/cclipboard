
#include "cclipboard_options.h"
#include "cclipboard.h"

#ifndef CCLIPBOARD_H
#define CCLIPBOARD_H


namespace cclipboard {

  CClipboard::CClipboard() {

  }

  CClipboard::CClipboard(CClipboard_options* p_options) {

  }

  char* CClipboard::paste() {
    std::string dest;
    this->_clipboard->paste(dest);

    return (char *)dest.c_str();
  }
}

#endif /* CCLIPBOARD_H */

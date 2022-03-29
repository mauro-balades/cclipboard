
#include "cclipboard_options.h"
#include "cclipboard.h"

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

  void CClipboard::copy(std::string p_string) {
    this->_clipboard->copy(p_string.c_str(), p_string.length());
  }
}


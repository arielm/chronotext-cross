#pragma once

#include "chr/cross/Context.h"
#include "chr/cross/Keyboard.h"

namespace chr
{
  static const std::map<int, int> KEYMAP
  {
    { 33, KEY_PAGE_UP },
    { 34, KEY_PAGE_DOWN },
    { 35, KEY_END },
    { 36, KEY_HOME },
    { 37, KEY_LEFT },
    { 38, KEY_UP },
    { 39, KEY_RIGHT },
    { 40, KEY_DOWN },
    { 46, KEY_DELETE },

    { 0x10, KEY_SHIFT },
    { 0x11, KEY_CTRL },
    { 0x12, KEY_ALT },
    { 0x5B, KEY_META },
    { 0x5D, KEY_META }
  };
}

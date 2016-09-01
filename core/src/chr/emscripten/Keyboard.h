#pragma once

#include "chr/cross/Context.h"
#include "chr/cross/Keyboard.h"

namespace chr
{
  static const std::map<int, int> KEYMAP
  {
    { 0x10, KEY_SHIFT },
    { 0x11, KEY_CTRL },
    { 0x12, KEY_ALT },
    { 0x5B, KEY_META },
    { 0x5D, KEY_META }
  };
}

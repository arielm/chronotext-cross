#pragma once

#include "cross/Context.h"
#include "cross/Keyboard.h"

namespace chr
{
  static const std::map<int, int> KEYMAP
  {
    { GLFW_KEY_ENTER, KEY_RETURN },
    { GLFW_KEY_TAB, KEY_TAB },
    { GLFW_KEY_BACKSPACE, KEY_BACKSPACE },
    { GLFW_KEY_DELETE, KEY_DELETE },
    { GLFW_KEY_RIGHT, KEY_RIGHT },
    { GLFW_KEY_LEFT, KEY_LEFT },
    { GLFW_KEY_DOWN, KEY_DOWN },
    { GLFW_KEY_UP, KEY_UP },
    { GLFW_KEY_PAGE_UP, KEY_PAGE_UP },
    { GLFW_KEY_PAGE_DOWN, KEY_PAGE_DOWN },
    { GLFW_KEY_HOME, KEY_HOME },
    { GLFW_KEY_END, KEY_END },

    { GLFW_KEY_LEFT_SHIFT, KEY_SHIFT },
    { GLFW_KEY_LEFT_CONTROL, KEY_CTRL },
    { GLFW_KEY_LEFT_ALT, KEY_ALT },
    { GLFW_KEY_LEFT_SUPER, KEY_META },
    { GLFW_KEY_RIGHT_SHIFT, KEY_SHIFT },
    { GLFW_KEY_RIGHT_CONTROL, KEY_CTRL },
    { GLFW_KEY_RIGHT_ALT, KEY_ALT },
    { GLFW_KEY_RIGHT_SUPER, KEY_META },

    { GLFW_KEY_KP_0 , KEY_0 },
    { GLFW_KEY_KP_1 , KEY_1 },
    { GLFW_KEY_KP_2 , KEY_2 },
    { GLFW_KEY_KP_3 , KEY_3 },
    { GLFW_KEY_KP_4 , KEY_4 },
    { GLFW_KEY_KP_5 , KEY_5 },
    { GLFW_KEY_KP_6 , KEY_6 },
    { GLFW_KEY_KP_7 , KEY_7 },
    { GLFW_KEY_KP_8 , KEY_8 },
    { GLFW_KEY_KP_9 , KEY_9 },
    { GLFW_KEY_KP_ENTER, KEY_RETURN }
  };
}

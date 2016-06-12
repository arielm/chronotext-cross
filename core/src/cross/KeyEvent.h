
#pragma once

namespace chr
{
  struct KeyEvent
  {
    enum Kind
    {
      KIND_UNDEFINED,
      KIND_PRESSED,
      KIND_RELEASED,
      KIND_REPEATED
    };

    enum
    {
      MODIFIER_NONE = 0,
      MODIFIER_SHIFT = 1,
      MODIFIER_CTRL = 2,
      MODIFIER_ALT = 4,
      MODIFIER_SUPER = 8,
    };

    Kind kind;
    int modifiers;
    int keyCode;
    uint32_t codepoint;

    KeyEvent(Kind kind, int modifiers, int keyCode, uint32_t codepoint = 0)
    :
    kind(kind),
    modifiers(modifiers),
    keyCode(keyCode),
    codepoint(codepoint)
    {}
  };
}

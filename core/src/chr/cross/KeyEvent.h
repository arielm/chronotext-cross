#pragma once

namespace chr
{
  struct KeyEvent
  {
    enum Kind
    {
      KIND_UNDEFINED,
      KIND_PRESSED,
      KIND_UP,
      KIND_DOWN
    };

    enum
    {
      MODIFIER_NONE = 0,
      MODIFIER_SHIFT = 1,
      MODIFIER_CTRL = 2,
      MODIFIER_ALT = 4,
      MODIFIER_META = 8,
    };

    Kind kind;
    int modifiers;
    int keyCode;
    uint32_t codePoint;

    KeyEvent(Kind kind, int modifiers, int keyCode, uint32_t codePoint = 0)
    :
    kind(kind),
    modifiers(modifiers),
    keyCode(keyCode),
    codePoint(codePoint)
    {}
  };
}


#pragma once

namespace chr
{
  struct MouseEvent
  {
    enum Kind
    {
      KIND_MOVED,
      KIND_DRAGGED,
      KIND_PRESSED,
      KIND_RELEASED
    };

    float x;
    float y;
    int button;
    Kind kind;

    MouseEvent(float x, float y, int button, Kind kind)
    :
    x(x),
    y(y),
    button(button),
    kind(kind)
    {}
  };
}

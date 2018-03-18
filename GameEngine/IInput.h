#pragma once
#include "MyVmath.h"

enum eMouseButton
{
   eLeftMouseButton,
   eMiddleMouseButton,
   eRightMouseButton
};

class IInput
{
public:
   virtual bool MouseButton( eMouseButton button ) = 0;
   virtual bool MouseButtonDown( eMouseButton button ) = 0;
   virtual bool MouseButtonUp( eMouseButton button ) = 0;
   virtual vmath::vec2 MousePosition() = 0;
   virtual vmath::vec2 MouseDelta() = 0;
};

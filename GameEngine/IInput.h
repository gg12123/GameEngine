#pragma once
#include "MyVmath.h"

enum eMouseButton
{
   eLeftMouseButton,
   eRightMouseButton,
   eMiddleMouseButton,

   NUMBER_OF_MOUSE_BUTTONS
};

enum eInputKey
{
   eSpaceBar,

   NUMBER_ON_INPUT_KEYS
};

class IInput
{
public:
   virtual bool MouseButton( eMouseButton button ) = 0;
   virtual bool MouseButtonDown( eMouseButton button ) = 0;
   virtual bool MouseButtonUp( eMouseButton button ) = 0;
   virtual vmath::vec2 MousePosition() = 0;
   virtual vmath::vec2 MouseDelta() = 0;
   virtual float MouseScroll() = 0;

   virtual bool Key( eInputKey key ) = 0;
   virtual bool KeyDown( eInputKey key ) = 0;
   virtual bool KeyUp( eInputKey key ) = 0;
};

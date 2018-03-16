#pragma once

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
};

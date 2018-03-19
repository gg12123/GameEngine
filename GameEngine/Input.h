#pragma once
#include <vector>
#include "InputTrigger.h"
#include "IInput.h"

class Input : public IInput
{
public:
   Input();

   bool MouseButton( eMouseButton button ) override;
   bool MouseButtonDown( eMouseButton button ) override;
   bool MouseButtonUp( eMouseButton button ) override;
   vmath::vec2 MousePosition() override;
   vmath::vec2 MouseDelta() override;
   float MouseScroll() override;

   bool Key( eInputKey key ) override;
   bool KeyDown( eInputKey key ) override;
   bool KeyUp( eInputKey key ) override;

   void OnMouseButtonUp( int button );
   void OnMouseButtonDown( int button );
   void OnScroll( float offset );

   void OnKeyUp( eInputKey key );
   void OnKeyDown( eInputKey key );

   void PreUpdate( vmath::vec2 mousePos );

   // Called after all components have updated.
   void FinalUpdate();

private:
   InputTrigger m_MouseInputs[ NUMBER_OF_MOUSE_BUTTONS ];
   InputTrigger m_KeyInputs[ NUMBER_ON_INPUT_KEYS ];

   vmath::vec2 m_CurrentMousePosition;
   vmath::vec2 m_CurrentMouseDelta;
   float m_CurrentScrollOffset;
   float m_CurrentScrollDelta;

   std::vector<InputTrigger*> m_InputsToClear;
};

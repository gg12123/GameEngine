#include "Input.h"

Input::Input()
{
   m_CurrentScrollOffset = 0.0f;
   m_CurrentScrollDelta = 0.0f;
}

bool Input::MouseButton( eMouseButton button )
{
   return m_MouseInputs[ button ].IsTriggerd();
}

bool Input::MouseButtonDown( eMouseButton button )
{
   return m_MouseInputs[ button ].IsTriggerdDown();
}

bool Input::MouseButtonUp( eMouseButton button )
{
   return m_MouseInputs[ button ].IsTriggeredUp();
}

bool Input::Key( eInputKey key )
{
   return m_KeyInputs[ key ].IsTriggerd();
}

bool Input::KeyDown( eInputKey key )
{
   return m_KeyInputs[ key ].IsTriggerdDown();
}

bool Input::KeyUp( eInputKey key )
{
   return m_KeyInputs[ key ].IsTriggeredUp();
}

vmath::vec2 Input::MousePosition()
{
   return m_CurrentMousePosition;
}

vmath::vec2 Input::MouseDelta()
{
   return m_CurrentMouseDelta;
}

float Input::MouseScroll()
{
   return m_CurrentScrollDelta;
}

void Input::OnMouseButtonUp( int button )
{
   InputTrigger* x = &(m_MouseInputs[ button ]);
   x->OnUp();
   m_InputsToClear.push_back( x );
}

void Input::OnMouseButtonDown( int button )
{
   InputTrigger* x = &(m_MouseInputs[ button ]);
   x->OnDown();
   m_InputsToClear.push_back( x );
}

void Input::OnKeyUp( eInputKey key )
{
   InputTrigger* x = &(m_KeyInputs[ key ]);
   x->OnUp();
   m_InputsToClear.push_back( x );
}

void Input::OnKeyDown( eInputKey key )
{
   InputTrigger* x = &(m_KeyInputs[ key ]);
   x->OnDown();
   m_InputsToClear.push_back( x );
}

void Input::OnScroll( float offset )
{
   m_CurrentScrollDelta = offset - m_CurrentScrollOffset;
}

void Input::PreUpdate( vmath::vec2 mousePos )
{
   m_CurrentMouseDelta = mousePos - m_CurrentMousePosition;
   m_CurrentMousePosition = mousePos;
}

// Called after all components have updated.
void Input::FinalUpdate()
{
   for (auto it = m_InputsToClear.begin(); it != m_InputsToClear.end(); it++)
   {
      (*it)->Clear();
   }

   m_InputsToClear.clear();
   m_CurrentScrollDelta = 0.0;
}
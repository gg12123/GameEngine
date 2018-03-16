#include "Input.h"

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

// Called after all components have updated.
void Input::FinalUpdate()
{
   for (auto it = m_InputsToClear.begin(); it != m_InputsToClear.end(); it++)
   {
      (*it)->Clear();
   }

   m_InputsToClear.clear();
}
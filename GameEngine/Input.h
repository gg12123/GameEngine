#pragma once
#include <vector>
#include "InputTrigger.h"
#include "IInput.h"

class Input : public IInput
{
public:
   bool MouseButton( eMouseButton button ) override;
   bool MouseButtonDown( eMouseButton button ) override;
   bool MouseButtonUp( eMouseButton button ) override;
   vmath::vec2 MousePosition() override;
   vmath::vec2 MouseDelta() override;

   void OnMouseButtonUp( int button );
   void OnMouseButtonDown( int button );

   void PreUpdate( vmath::vec2 mousePos );

   // Called after all components have updated.
   void FinalUpdate();

private:
   InputTrigger m_MouseInputs[ 3 ];

   vmath::vec2 m_CurrentMousePosition;
   vmath::vec2 m_CurrentMouseDelta;

   std::vector<InputTrigger*> m_InputsToClear;
};

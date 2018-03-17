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

   void OnMouseButtonUp( int button );
   void OnMouseButtonDown( int button );

   // Called after all components have updated.
   void FinalUpdate();

private:
   InputTrigger m_MouseInputs[ 3 ];

   std::vector<InputTrigger*> m_InputsToClear;
};

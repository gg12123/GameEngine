#pragma once
#include "Component.h"

class IPhysics;

class BehaviourComponent : public Component
{
protected:
   void SetWorld( World& world ) override final;
   IPhysics& GetPhysics();

private:
   World* m_World;
};

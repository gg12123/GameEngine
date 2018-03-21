#include "BehaviourComponent.h"
#include "World.h"

BehaviourComponent::BehaviourComponent()
{
   m_World = nullptr;
}

void BehaviourComponent::SetWorld( World& world )
{
   m_World = &world;
}

IPhysics& BehaviourComponent::GetPhysics()
{
   return m_World->GetPhysics();
}
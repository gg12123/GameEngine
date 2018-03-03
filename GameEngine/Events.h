#pragma once
#include <functional>

class Event
{
public:
   virtual void Invoke()
   {
      throw std::exception( "Event called with wrong parameters" );
   }
};

template<typename T>
class VoidEvent : public Event
{
public:
   VoidEvent( void(T::*func)(), T& owner ) : m_Owner( owner )
   {
      m_Function = func;
   }

   void Invoke()
   {
      (m_Owner.*(m_Function))();
   }

private:
   void(T::*m_Function)();
   T& m_Owner;
};

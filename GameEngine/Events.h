#pragma once
#include <functional>

class GameObject;

class EventHandler
{
public:
   virtual void Invoke()
   {
      throw std::exception( "Event called with wrong parameters" );
   }

   virtual void Invoke( GameObject& param )
   {
      throw std::exception( "Event called with wrong parameters" );
   }
};

template<typename T>
class VoidEventHandler : public EventHandler
{
public:
   VoidEventHandler<T>* Init( void(T::*func)(), T* owner )
   {
      m_Function = func;
      m_Owner = owner;
      return this;
   }

   void Invoke()
   {
      ((*m_Owner).*(m_Function))();
   }

private:
   void(T::*m_Function)();
   T* m_Owner;
};

template<typename T>
class GOEventHandler : public EventHandler
{
public:
   GOEventHandler<T>* Init( void(T::*func)(GameObject&), T* owner )
   {
      m_Function = func;
      m_Owner = owner;
      return this;
   }

   void Invoke( GameObject& param )
   {
      ((*m_Owner).*(m_Function))( param );
   }

private:
   void(T::*m_Function)(GameObject&);
   T* m_Owner;
};

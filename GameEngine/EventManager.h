#pragma once
#include<vector>
#include "Events.h"

template<typename eventType, int eventCount>
class EventManager
{
public:
   ~EventManager()
   {
      for (int i = 0; i < eventCount; i++)
      {
         if (m_Events[ i ])
         {
            delete m_Events[ i ];
         }
      }
   }

   EventManager()
   {
      for (int i = 0; i < eventCount; i++)
         m_Events[ i ] = nullptr;
   }

   void Register( const eventType eventID, EventHandler& handler )
   {
      if (!m_Events[ eventID ])
      {
         m_Events[ eventID ] = new std::vector<EventHandler*>();
      }

      m_Events[ eventID ]->push_back( &handler );
   }

   void Invoke( const eventType eventID ) const
   {
      const std::vector<EventHandler*>* v = m_Events[ eventID ];

      if (v)
      {
         for (auto it = v->begin(); it != v->end(); it++)
         {
            (*it)->Invoke();
         }
      }
   }

   template<typename T>
   void Invoke( const eventType eventID, T param ) const
   {
      const std::vector<EventHandler*>* v = m_Events[ eventID ];

      if (v)
      {
         for (auto it = v->begin(); it != v->end(); it++)
         {
            (*it)->Invoke( param );
         }
      }
   }

private:
   std::vector<EventHandler*>* m_Events[ eventCount ];
};

#include "Debug.h"
#include "Path.h"

Debug::~Debug()
{
   if (m_File.is_open())
   {
      m_File.close();
   }
}

Debug& Debug::Instance()
{
   static Debug instance;
   return instance;
}

void Debug::LogError( std::string msg )
{
   msg.append( " ERROR \n" );
   m_File << msg;
}

void Debug::Log( std::string msg )
{
   msg.append( "\n" );
   m_File << msg;
}

Debug::Debug()
{
   m_File.open( Path::Instance().GetDebugPath( "DebugLog.txt" ) );

   if (!m_File.is_open())
   {
      throw std::exception( "Unable to open debug log" );
   }
}
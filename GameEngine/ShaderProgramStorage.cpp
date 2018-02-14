#include "ShaderProgramStorage.h"
#include "Debug.h"

static GLuint CompileProgram( std::string name )
{

}

void ShaderProgramStorage::CompileIfNotAlready( std::string name )
{
   if (m_Programs.count( name ) == 0)
   {
      m_Programs[ name ] = CompileProgram( name );
   }
}

GLuint ShaderProgramStorage::GetProgram( std::string name )
{
   if (m_Programs.count( name ) == 0)
   {
      Debug::Instance().LogError( "Program not ready on request" );
      CompileIfNotAlready( name );
   }

   return m_Programs[ name ];
}
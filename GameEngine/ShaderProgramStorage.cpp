#include "ShaderProgramStorage.h"
#include "Debug.h"
#include "Path.h"

static GLuint CompileProgram( std::string path )
{

}

void ShaderProgramStorage::CompileIfNotAlready( std::string name )
{
   if (m_Programs.count( name ) == 0)
   {
      m_Programs[ name ] = CompileProgram( Path::Instance().GetShaderPath( name ) );
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
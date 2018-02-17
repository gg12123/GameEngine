#include "ShaderProgram.h"
#include "Path.h"

void ShaderProgram::Load( std::string name )
{
   std::string fullPath = Path::Instance().GetShaderPath( name );

   // load and compile it
}

Asset* ShaderProgram::CreateInstance()
{
   return new ShaderProgram();
}

GLuint ShaderProgram::ShaderValue()
{
   return m_Program;
}
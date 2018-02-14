#pragma once
#include <string>
#include <unordered_map>
#include "GL/gl3w.h"

class ShaderProgramStorage
{
public:

   void InitPathToShaders( std::string path );
   void CompileIfNotAlready( std::string name );
   GLuint GetProgram( std::string name );

private:

   std::unordered_map<std::string, GLuint> m_Programs;
   std::string m_PathToShaders;
};

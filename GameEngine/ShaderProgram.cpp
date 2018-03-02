#include <iostream>
#include <fstream>
#include <vector>
#include "ShaderProgram.h"
#include "Path.h"
#include "Debug.h"

#define END_OF_SHADER '~'

using namespace std;

static GLchar* LoadShaderSource(ifstream &file)
{
   std::string source;
   char nextChar = file.get();

   while ((nextChar != END_OF_SHADER) && !file.eof())
   {
      source.append( 1, nextChar );
      nextChar = file.get();
   }

   if (nextChar != END_OF_SHADER)
   {
      Debug::Instance().LogError( "Error loading shader source" );
   }

   char * copy = new char[ source.size() + 1 ];

   for (unsigned int i = 0; i < source.size(); i++)
   {
      copy[ i ] = source.at( i );
   }

   copy[ source.size() ] = '\0';

   return copy;
}

void ShaderProgram::Load( std::string name )
{
   std::string fullPath = Path::Instance().GetShaderPath( name );

   ifstream file;
   file.open( fullPath, ios::in );

   if (!file.is_open())
   {
      throw std::exception( "Unable to open shader file." );
   }

   GLchar * vertexSource = LoadShaderSource( file );
   GLchar * fragSource = LoadShaderSource( file );

   file.close();

   // Vertex
   GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
   glShaderSource( vertexShader, 1, &vertexSource, nullptr );
   glCompileShader( vertexShader );

   GLint isCompiled = 0;
   glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &isCompiled );
   if (isCompiled == GL_FALSE)
   {
      throw std::exception( "vert shader failed" );
   }

   // Fragment
   GLuint fragShader = glCreateShader( GL_FRAGMENT_SHADER );
   glShaderSource( fragShader, 1, &fragSource, nullptr );
   glCompileShader( fragShader );

   glGetShaderiv( fragShader, GL_COMPILE_STATUS, &isCompiled );
   if (isCompiled == GL_FALSE)
   {
      throw std::exception( "frag shader failed" );
   }

   // Link program
   m_Program = glCreateProgram();
   glAttachShader( m_Program, vertexShader );
   glAttachShader( m_Program, fragShader );
   glLinkProgram( m_Program );

   GLint isLinked = 0;
   glGetProgramiv( m_Program, GL_LINK_STATUS, (int *)&isLinked );
   if (isLinked == GL_FALSE)
   {
      throw std::exception( "shader linkage failed" );
   }

   // Clean up
   glDeleteShader( vertexShader );
   glDeleteShader( fragShader );

   delete[] vertexSource;
   delete[] fragSource;
}

Asset* ShaderProgram::CreateInstance()
{
   return new ShaderProgram();
}

GLuint ShaderProgram::ShaderValue()
{
   return m_Program;
}
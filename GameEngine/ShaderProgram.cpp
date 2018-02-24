#include <iostream>
#include <fstream>
#include "ShaderProgram.h"
#include "Path.h"
#include "Debug.h"

using namespace std;

static GLchar* LoadShaderSource(ifstream &file)
{
   std::string source;
   char nextChar = file.get();

   while ((nextChar != '#') && !file.eof());
   {
      source.append( 1, nextChar );
      nextChar = file.get();
   }

   if (nextChar != '#')
   {
      Debug::Instance().LogError( "Error loading shader source" );
   }

   char * copy = new char[ source.size() ];

   for (unsigned int i = 0; i < source.size(); i++)
   {
      copy[ i ] = source.at( i );
   }

   return copy;
}

void ShaderProgram::Load( std::string name )
{
   std::string fullPath = Path::Instance().GetShaderPath( name );

   ifstream file;
   file.open( fullPath, ios::in );

   GLchar * vertexSource = LoadShaderSource( file );
   GLchar * fragSource = LoadShaderSource( file );

   GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
   glShaderSource( vertexShader, 1, &vertexSource, nullptr );
   glCompileShader( vertexShader );

   GLuint fragShader = glCreateShader( GL_FRAGMENT_SHADER );
   glShaderSource( fragShader, 1, &fragSource, nullptr );
   glCompileShader( fragShader );

   m_Program = glCreateProgram();
   glAttachShader( m_Program, vertexShader );
   glAttachShader( m_Program, fragShader );
   glLinkProgram( m_Program );

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
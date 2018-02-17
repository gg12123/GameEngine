#pragma once
#include "Asset.h"

class ShaderProgram : public Asset
{
public:
   static Asset* CreateInstance();

   void Load( std::string name ) override;
   GLuint ShaderValue() override;

private:
   GLuint m_Program;
};

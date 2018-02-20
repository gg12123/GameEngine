#include "WindowConfiguration.h"

void WindowConfiguration::Set( float width, float height )
{
   m_Width = width;
   m_Height = height;
}

float WindowConfiguration::GetWidth()
{
   return m_Width;
}

float WindowConfiguration::GetHeight()
{
   return m_Height;
}
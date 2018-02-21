#pragma once

class IWindowConfiguration
{
public:
   virtual float GetWidth() = 0;
   virtual float GetHeight() = 0;
};

class WindowConfiguration : public IWindowConfiguration
{
public:
   void Set( float width, float height );
   float GetWidth() override;
   float GetHeight() override;

private:
   float m_Width;
   float m_Height;
};

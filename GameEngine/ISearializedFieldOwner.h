#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>

class SerializedField;

class ISerializedFieldOwner
{
public:
   virtual std::string GetName() = 0;
   virtual void OnNewSerializedFields() = 0;
   virtual void GetSerializedFields( std::unordered_map<std::string, SerializedField*>& fields ) = 0;
   virtual void DeSerialize( std::ifstream& stream ) = 0;
   virtual void Serialize( std::ofstream& stream ) = 0;
};

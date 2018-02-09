#pragma once

struct Quaternion
{
   float r;
   float x;
   float y;
   float z;
};

Quaternion QuaternionIdentity();

Quaternion QuaternionConj( Quaternion q );

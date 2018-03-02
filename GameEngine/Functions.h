#pragma once

typedef void( *UpdaterFunctionPtr )();

enum EUpdaterFunction
{
   eUpdateFunction,
   eFixedUpdateFunction,
   eUpdateInEditMode
};
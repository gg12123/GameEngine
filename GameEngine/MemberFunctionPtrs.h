#pragma once

#define DECLARE_VOID_MEMBER_FUNC_PTR( classType, funcName ) void(classType::*funcName)()

#define INVOKE_VOID_MEMBER_FUNC_PTR( owner, funcName ) ((*owner).*(funcName))()

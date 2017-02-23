#pragma once

#ifdef CORSAIRLAYERS_DLL
#define CORSAIRLAYERS_EXPORT __declspec(dllexport)
#else
#define CORSAIRLAYERS_EXPORT __declspec(dllimport)
#endif

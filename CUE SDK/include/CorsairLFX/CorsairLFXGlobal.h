#pragma once

#ifdef CORSAIRLFX_DLL
#define CORSAIRLFX_EXPORT __declspec(dllexport)
#else
#define CORSAIRLFX_EXPORT __declspec(dllimport)
#endif

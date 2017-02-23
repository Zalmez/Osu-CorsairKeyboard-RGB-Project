#pragma once

#ifdef CUELFX_DLL
#define CUELFX_EXPORT __declspec(dllexport)
#else
#define CUELFX_EXPORT __declspec(dllimport)
#endif

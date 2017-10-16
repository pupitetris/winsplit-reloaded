#ifndef __HOOK_HEADER_H__
#define __HOOK_HEADER_H__

#include <windows.h>

#ifdef BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif


extern "C" bool DLL_EXPORT InstallAllHook (HWND hwnd);
extern "C" bool DLL_EXPORT StopAllHook();
extern "C" void DLL_EXPORT GetTransparencyValues (HWND hWnd, bool& IsEnabled, int& Degree);

#endif // __HOOK_HEADER_H__

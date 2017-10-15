#ifdef BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

#include <windows.h>
#include <commctrl.h>

#include <stdio.h>

HWND hwndWinSplitFrame __attribute__((section("hwnd_winsplit"), shared)) = NULL;
HHOOK m_hookMouse __attribute__((section("hhookMouse"), shared)) = NULL;
HHOOK m_hookCBT __attribute__((section("hhookCBT"), shared)) = NULL;

UINT WSM_STARTMOVING __attribute__((section("start_moving_message"), shared)) = 0;
UINT WSM_STOPMOVING __attribute__((section ("stop_moving_message"), shared)) = 0;
UINT WSM_MOLETTE __attribute__((section("wheel_message"), shared)) = 0;

HINSTANCE m_hDllInstance = NULL;
#ifndef LWA_ALPHA
#define LWA_ALPHA		0x00000002
#endif
// Format de la fonction GetLayeredWindowAttributes
typedef BOOL (WINAPI *GLWA_FUNC)(HWND hwnd,COLORREF *crKey,BYTE *bAlpha,DWORD *dwFlags);
GLWA_FUNC MyGetLayeredWindowAttributes=NULL;

LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam )
{
    if (nCode < 0)
        return CallNextHookEx(m_hookMouse,nCode,wParam,lParam);

    if (nCode == HC_ACTION)
    {
        if (wParam == WM_MOUSEWHEEL)
        {
            MSLLHOOKSTRUCT* pmouse_ll = (MSLLHOOKSTRUCT*) lParam;

            PostMessage(hwndWinSplitFrame,WSM_MOLETTE,(WPARAM)(pmouse_ll->mouseData),(LPARAM)NULL);
        }
        else if (wParam == WM_LBUTTONUP)
        {
            PostMessage(hwndWinSplitFrame,WSM_STOPMOVING,wParam,(LPARAM)NULL);
        }
    }

    return CallNextHookEx(m_hookMouse,nCode,wParam,lParam);
}

LRESULT CALLBACK MovingCBTProc(int nCode,WPARAM wParam,LPARAM lParam)
{
    if (nCode == HCBT_SYSCOMMAND)
    {
        if (wParam == SC_MOVE)
        {
            PostMessage(hwndWinSplitFrame,WSM_STARTMOVING,wParam,(LPARAM)NULL);
        }
    }
    if (nCode == HCBT_MOVESIZE)
    {
        PostMessage(hwndWinSplitFrame,WSM_STOPMOVING,wParam,(LPARAM)NULL);
    }

    return CallNextHookEx(m_hookCBT,nCode,wParam,lParam);
}


bool UninstallMouseHook()
{
    if (!m_hookMouse)
        return true;

    if (m_hookMouse)
        if (UnhookWindowsHookEx(m_hookMouse))
            m_hookMouse = NULL;

    return m_hookMouse == NULL;
}

bool UninstallCBTHook()
{
    if (!m_hookCBT)
        return true;

    if (m_hookCBT)
        if (UnhookWindowsHookEx(m_hookCBT))
            m_hookCBT = NULL;

    return m_hookCBT == NULL;
}

bool InstallMouseHook()
{
    if (m_hookMouse)
        return true;

    m_hookMouse = SetWindowsHookEx(WH_MOUSE,MouseProc,m_hDllInstance,0);
    return m_hookMouse != NULL;
}

bool InstallCBTHook()
{
    if (m_hookCBT)
        return true;

    m_hookCBT = SetWindowsHookEx(WH_CBT, MovingCBTProc, m_hDllInstance, 0);
    return m_hookCBT != NULL;
}


extern "C" bool DLL_EXPORT InstallAllHook(HWND hwnd)
{
    hwndWinSplitFrame = hwnd;
    WCHAR buffer[100];

    if (!InstallMouseHook())
    {
        wsprintf(buffer,L"Can not install mouse hook, Error N:%d",int(GetLastError()));
        MessageBox(NULL,buffer,L"Error",MB_OK);

        return false;
    }

    if (!InstallCBTHook())
    {
        wsprintf(buffer,L"Can not install CBT hook, Error N:%d",int(GetLastError()));
        MessageBox(NULL,buffer,L"Error",MB_OK);

        if (!UninstallMouseHook())
        {
            wsprintf(buffer,L"Can not Uninstall mouse hook, Error N:%d",int(GetLastError()));
            MessageBox(NULL,buffer,L"Error",MB_OK);
        }

        return false;
    }

    WSM_STARTMOVING = RegisterWindowMessage(L"WinSplitMessage_StartMoving");
    WSM_STOPMOVING = RegisterWindowMessage(L"WinSplitMessage_StopMoving");
    WSM_MOLETTE = RegisterWindowMessage(L"WinSplitMessage_Wheel");

    return true;
}

extern "C" bool DLL_EXPORT StopAllHook()
{
    bool isOk = true;

    if (!UninstallMouseHook())
        isOk = false;

    if (!UninstallCBTHook())
        isOk = false;

    return isOk;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        m_hDllInstance = hInstance;
        HMODULE hUser32 = GetModuleHandle(L"USER32.DLL");
        if (hUser32!=NULL)
        {
            MyGetLayeredWindowAttributes = (GLWA_FUNC)GetProcAddress(hUser32,"GetLayeredWindowAttributes");
        }
    }

    return TRUE;
}

extern "C" void DLL_EXPORT GetTransparencyValues(HWND hWnd, bool& IsEnabled, int& Degree)
{
    long lExStyle=GetWindowLong(hWnd, GWL_EXSTYLE);
    BYTE BVal=255;
    DWORD dwStyle=0;
    IsEnabled=((lExStyle & WS_EX_LAYERED)==WS_EX_LAYERED);
    if (IsEnabled)
    {
        if (MyGetLayeredWindowAttributes!=NULL)
        {
            MyGetLayeredWindowAttributes(hWnd, NULL, &BVal, &dwStyle);
            Degree=(int)BVal;
            IsEnabled=((dwStyle&LWA_ALPHA)==LWA_ALPHA);
        }
        else
        {
            Degree=255;
        }
    }
}

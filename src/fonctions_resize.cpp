

#include <windows.h>
#include <psapi.h>

#include <Windows.h>

// For Vista and newer "extended" border compensation:
#include "Dwmapi-compat.h"

#include "auto_placement.h"
#include "dialog_fusion.h"
#include "liste_windows.h"
#include "multimonitor_move.h"
#include "layout_manager.h"
#include "fonctions_speciales.h"

//=============================
// Resize window
//=============================
bool ResizeWindow(const int hotkey, bool fromKbd)
{
    HWND hwnd = ListWindows::ListWindow();
    bool flag_resizable = true;
    //((GetWindowLong(hwnd,GWL_STYLE)&WS_SIZEBOX)!=0);

    wxRect res = LayoutManager::GetInstance()->GetNext(hwnd,hotkey-1);

    WINDOWPLACEMENT placement;
    GetWindowPlacement(hwnd,&placement);

    if(placement.showCmd == SW_SHOWMAXIMIZED)
    {
        placement.showCmd = SW_RESTORE;
        SetWindowPlacement(hwnd,&placement);
    }

    bool bMoveMouse= fromKbd & SettingsManager::Get().getMouseFollowWindow();
    if (bMoveMouse) StoreOrSetMousePosition(true, hwnd);

    SetWindowPos(hwnd,HWND_TOP,res.x,res.y,res.width,res.height,flag_resizable?SWP_SHOWWINDOW:SWP_NOSIZE);

    {
        int major;
        wxGetOsVersion (&major, NULL);
        if (major >= 6)
        {
            // Windows Vista or newer.

            RECT ext_frame;
            HRESULT hr = DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &ext_frame, sizeof(RECT));
            if (SUCCEEDED(hr) &&
                ext_frame.top != res.x ||
                ext_frame.left != res.y ||
                ext_frame.bottom != res.GetBottom () ||
                ext_frame.right != res.GetRight ())
            {
                res.SetX (res.x * 2 - ext_frame.left);
                res.SetY (res.y * 2 - ext_frame.top);
                res.SetWidth (res.width * 2 - ext_frame.right + ext_frame.left);
                res.SetHeight (res.height * 2 - ext_frame.bottom + ext_frame.top);

                SetWindowPos(hwnd,HWND_TOP,res.x,res.y,res.width,res.height,flag_resizable?SWP_SHOWWINDOW:SWP_NOSIZE);
            }
        }
    }

    if (bMoveMouse) StoreOrSetMousePosition(false, hwnd);

    return true;
}


void MoveToScreen(DIRECTION sens)
{
    HWND hwnd = GetForegroundWindow();
    if(!ListWindows::ValidateWindow(hwnd)) return;

    MoveWindowToDirection(hwnd,sens);
}

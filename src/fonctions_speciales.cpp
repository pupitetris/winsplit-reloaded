
#include <windows.h>
#include <psapi.h>

#include "auto_placement.h"
#include "dialog_fusion.h"
#include "liste_windows.h"
#include "minimize_restore.h"
#include "settingsmanager.h"

using namespace std;

void StoreOrSetMousePosition(bool storeOnly, HWND wnd)
{
    static int mouseX, mouseY, wndW, wndH;
    POINT pt;
    GetCursorPos(&pt);
    RECT rc;
    GetWindowRect(wnd, &rc);
    if (storeOnly)
    {
        mouseX=pt.x-rc.left;
        mouseY=pt.y-rc.top;
        wndW=rc.right-rc.left;
        wndH=rc.bottom-rc.top;
        return;
    }
    bool bOut=(mouseX<0) | (mouseY<0) | (mouseX>wndW) | (mouseY>wndH);
    if ((bOut) && (SettingsManager::Get().getMouseFollowOnlyWhenIn())) return;
    float rx=float(mouseX)/wndW;
    float ry=float(mouseY)/wndH;
    int newX=rc.left + (int)((rc.right-rc.left)*rx);
    int newY=rc.top + (int)((rc.bottom-rc.top)*ry);
    SetCursorPos(newX, newY);
}

//==================
// Mosaique
//==================
void Mosaique()
{
    PostMessage(FindWindow(_T("Shell_TrayWnd"),NULL), WM_COMMAND,405,0);
}

//==================
// Fermer toutes les fenetres
//==================
bool CALLBACK CloseEnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    wxChar text[256];
    GetWindowText(hwnd,text,255);
    wxString title(text);

    if ((title.Length()!=0)&&IsWindow(hwnd)&&IsWindowVisible(hwnd)&&IsWindowEnabled(hwnd)&&(title!=_T("Program Manager")))
        PostMessage(hwnd,WM_CLOSE,0,0);

    return true;
}

void CloseAllFrame()
{
    EnumWindows((WNDENUMPROC)CloseEnumWindowsProc,0);
}

//==================
// Placement automatique
//==================
void AutoPlace()
{
    HWND m_hwnd = ListWindows::ListWindow();
    DWORD process_id;
    HANDLE process_handle;

    AutoPlacementManager m_auto_placement;
    WindowInfos m_structinfo;

    wxChar name[50];

    GetWindowThreadProcessId(m_hwnd,&process_id);
    process_handle = OpenProcess(PROCESS_ALL_ACCESS,TRUE,process_id);
    GetModuleFileNameEx(process_handle,NULL,name,49);
    CloseHandle(process_handle);

    wxString str_tmp(name);
    wxString process_name(str_tmp.Mid(str_tmp.Find(_T('\\'),true)+1));

    GetClassName(m_hwnd,name,49);

    str_tmp = process_name + wxString(_T("::")) + name;

    m_auto_placement.LoadData();

    if(m_auto_placement.Exist(wxString(str_tmp.c_str())))
    {
        m_structinfo = m_auto_placement.GetWindowInfos(str_tmp);

        SetWindowLong(m_hwnd,GWL_STYLE,m_structinfo.m_wndStyle);
        bool bMoveMouse= SettingsManager::Get().getMouseFollowWindow();
        if (bMoveMouse) StoreOrSetMousePosition(true, m_hwnd);
        SetWindowPos(m_hwnd, HWND_TOP, m_structinfo.m_rectxy.x, m_structinfo.m_rectxy.y, m_structinfo.m_rectxy.width,
                     m_structinfo.m_rectxy.height, m_structinfo.m_flagResize? SWP_SHOWWINDOW : SWP_NOSIZE);
        if (bMoveMouse) StoreOrSetMousePosition(false, m_hwnd);
    }
    else
    {
        wxString message=_("This classname from process ") + process_name + _(" is not saved.") + _T("\n") + _("Save this process at this placement?");

        if (MessageBox(NULL,message,_("WinSplit message"),MB_SETFOREGROUND|MB_ICONEXCLAMATION|MB_YESNO)==IDYES)
        {
            m_auto_placement.AddWindow(m_hwnd,str_tmp);
            m_auto_placement.SaveData();
        }
    }
}

//==================
// Fusion des fenetres
//==================
bool CALLBACK EnumWindowsMerge(HWND hwnd, LPARAM lParam)
{
	HWND* p_app_fusion = (HWND*)lParam;

    if (ListWindows::ValidateWindow(hwnd,false))
    {
        if (p_app_fusion[0] == 0)
        {
            p_app_fusion[0] = hwnd;
            return true;
        }
        else
        {
            p_app_fusion[1] = hwnd;
            return false;
        }
    }

    return true;
}

void fusion_fenetres()
{
    HWND app_fusion[2];
    RECT rcWnd[2];

    app_fusion[0] = app_fusion[1] = 0;

    EnumWindows((WNDENUMPROC)EnumWindowsMerge,(LPARAM)app_fusion);

    if(!(ListWindows::ValidateWindow(app_fusion[0],false) && ListWindows::ValidateWindow(app_fusion[1],false)))
        return;
    for (int i=0;i<2;i++) GetWindowRect(app_fusion[i], &rcWnd[i]);

    FusionDialog m_fusion_frame;
    m_fusion_frame.SetTabHwnd(app_fusion);
    m_fusion_frame.Show();
    m_fusion_frame.Raise();
    if (m_fusion_frame.ShowModal()==wxID_CANCEL)
    {
        for (int i=0;i<2;i++)
            MoveWindow(app_fusion[i], rcWnd[i].left, rcWnd[i].top, rcWnd[i].right-rcWnd[i].left, rcWnd[i].bottom-rcWnd[i].top, true);
    }
}

//==================
// Minimise des fenetres
//==================
void MinimizeRestore::MiniMizeWindow()
{
    WindowMinimized window;

    window.hwnd = GetForegroundWindow();

    if(!ListWindows::ValidateWindow(window.hwnd))
        return;

    //window.placement.length = sizeof(WINDOWPLACEMENT);
    //GetWindowPlacement(window.hwnd,&(window.placement));
    int iShow=SW_MINIMIZE;

    if (SettingsManager::Get().getMinMaxCycle())
    {
    	iShow=SW_SHOWMINIMIZED;
    	if (IsIconic(window.hwnd)) iShow=SW_MAXIMIZE;
    	if (IsZoomed(window.hwnd)) iShow=SW_SHOWNORMAL;
    }
    ShowWindow(window.hwnd,iShow);

    //m_vecMinimized.push(window);
}


void MinimizeRestore::RestoreMiniMizedWindow()
{
    WindowMinimized window;

	/*while(m_vecMinimized.size()!=0)
	{
	    window = m_vecMinimized.top();
	    m_vecMinimized.pop();

		if(IsWindow(window.hwnd))
		{
		    SetWindowPlacement(window.hwnd,&(window.placement));
            SetForegroundWindow(window.hwnd);
            break;
		}
	}*/
	window.hwnd = GetForegroundWindow();

    if(!ListWindows::ValidateWindow(window.hwnd))
        return;

	int iShow=SW_MAXIMIZE;

    if (SettingsManager::Get().getMinMaxCycle())
    {
    	if (IsIconic(window.hwnd)) iShow=SW_SHOWNORMAL;
    	if (IsZoomed(window.hwnd)) iShow=SW_SHOWMINIMIZED;
    }
	ShowWindow(window.hwnd,iShow);
}

void MinimizeRestore::MaximizeHorizontally()
{
    HWND hWnd=GetForegroundWindow();
    HMONITOR hmonitor;
    MONITORINFO monitor_info;
    RECT rcMonitor;

    // On recupere les informations liees au monitor courant => Gestion du multi-moniteur
    hmonitor = MonitorFromWindow(hWnd,MONITOR_DEFAULTTONEAREST);
    monitor_info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(hmonitor,&monitor_info);

    rcMonitor = monitor_info.rcWork;
    RECT rcWnd;
    GetWindowRect(hWnd, &rcWnd);
    ShowWindow(hWnd, SW_RESTORE);
    SetWindowPos(hWnd, NULL, rcMonitor.left, rcWnd.top, rcMonitor.right-rcMonitor.left, rcWnd.bottom-rcWnd.top, SWP_NOZORDER);
}

void MinimizeRestore::MaximizeVertically()
{
    HWND hWnd=GetForegroundWindow();
    HMONITOR hmonitor;
    MONITORINFO monitor_info;
    RECT rcMonitor;

    // On recupere les informations liees au monitor courant => Gestion du multi-moniteur
    hmonitor = MonitorFromWindow(hWnd,MONITOR_DEFAULTTONEAREST);
    monitor_info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(hmonitor,&monitor_info);

    rcMonitor = monitor_info.rcWork;
    RECT rcWnd;
    GetWindowRect(hWnd, &rcWnd);
    ShowWindow(hWnd, SW_RESTORE);
    SetWindowPos(hWnd, NULL, rcWnd.left, rcMonitor.top, rcWnd.right-rcWnd.left, rcMonitor.bottom-rcMonitor.top, SWP_NOZORDER);
}

void ShowAnimation(RECT& rcWnd, bool growing)
{
    const int STEPS=10;
    const int DELAY=10;
    int x0,y0,x1,y1,x,y,w0,h0,w1,h1,w,h;
    if (growing)
    {
        x0=((rcWnd.left+rcWnd.right)/2)-1;
        y0=((rcWnd.top+rcWnd.bottom)/2)-1;
        w0=2;
        h0=2;
        x1=rcWnd.left;
        y1=rcWnd.top;
        w1=rcWnd.right-rcWnd.left;
        h1=rcWnd.bottom-rcWnd.top;
    } else {
        x0=rcWnd.left;
        y0=rcWnd.top;
        w0=rcWnd.right-rcWnd.left;
        h0=rcWnd.bottom-rcWnd.top;
        x1=((rcWnd.left+rcWnd.right)/2)-1;
        y1=((rcWnd.top+rcWnd.bottom)/2)-1;
        w1=2;
        h1=2;
    }
    wxFrame frm(NULL, -1, _T(""), wxPoint(x0, y0), wxSize(w0,h0), wxFRAME_NO_TASKBAR|wxSTAY_ON_TOP);
    frm.SetTransparent(50);
    frm.SetBackgroundColour(*wxCYAN);
    frm.Show();
    HWND hFrm=(HWND)frm.GetHandle();
    wxStopWatch sw;

	for (int i=0;i<STEPS;i++)
	{
		wxTheApp->Yield();
		sw.Start();
		while(sw.Time()<DELAY){}
		x=x0+i*(x1-x0)/STEPS;
		y=y0+i*(y1-y0)/STEPS;
		w=w0+i*(w1-w0)/STEPS;
		h=h0+i*(h1-h0)/STEPS;
		MoveWindow(hFrm, x, y, w, h,false);
	}
	sw.Start();
    while(sw.Time()<DELAY){}
    frm.Hide();
}

void ToggleAlwaysOnTop()
{
    // Recuperation de la fenetre active
    HWND hWnd=GetForegroundWindow();
    // On verifie qu'il s'agit d'une fenetre "correcte"
    if(!ListWindows::ValidateWindow(hWnd)) return;
    // On recupere l'etat actuel du stylke AlwaysOnTop
    long lStyle=::GetWindowLong(hWnd, GWL_EXSTYLE);
	bool bState=((lStyle & WS_EX_TOPMOST)==WS_EX_TOPMOST);
	// On applique le nouveau style
	SetWindowPos(hWnd, (bState?HWND_NOTOPMOST:HWND_TOPMOST), 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	// On lance l'effet de la mort...
	RECT rcWnd;
	if (!GetWindowRect(hWnd, &rcWnd)) return;
    ShowAnimation(rcWnd, !bState);
}

// main_application.h file
#ifndef __WINSPLITMAIN__
#define __WINSPLITMAIN__

#include <wx/snglinst.h>

#include "tray_icon.h"
#include "frame_hook.h"

class WinSplitApp: public wxApp
{
private:
	TrayIcon* p_tray;

	FrameHook* p_frameHook;

	wxSingleInstanceChecker *p_checker;

	void OnCloseSession (wxCloseEvent& event);
public:
	WinSplitApp();
	~WinSplitApp();
	void ShowTrayIcon();

	bool IsAlreadyRunning();
	virtual bool OnInit();
	int OnExit();

	wxString GetVersion(); // En attente de la vraie version

	TrayIcon* GetTrayInstance()
	{
		return p_tray;
	}
	FrameHook* GetFrameHook()
	{
		return p_frameHook;
	}

	DECLARE_EVENT_TABLE()
};

DECLARE_APP (WinSplitApp)

#endif //__WINSPLITMAIN__

#include "liste_windows.h"
#include "settingsmanager.h"

//====================
// Listage fenetres
//====================
bool ListWindows::ValidateWindow (HWND hwnd, bool accept_tmw)
{
	wxChar buffer[256];
	bool filtre_tmw;

	GetWindowText (hwnd, buffer, 255);
	wxString title (buffer);

	filtre_tmw = accept_tmw ? true : ( (GetWindowLong (hwnd, GWL_EXSTYLE) &WS_EX_TOPMOST) == 0);

	if ( (title.Length() != 0) && IsWindowVisible (hwnd) && filtre_tmw && (title != _T ("Program Manager") ) )
		return true;

	return false;
}

bool CALLBACK EnumWindowsProc (HWND hwnd, LPARAM lParam)
{
	if (ListWindows::ValidateWindow (hwnd, false) )
	{
		* ( (HWND*) lParam) = hwnd;
		return false;
	}

	return true;
}

//======================
HWND ListWindows::ListWindow()
{
	SettingsManager &options = SettingsManager::Get();
	HWND hwnd_window;

	hwnd_window = GetForegroundWindow();

	if (ValidateWindow (hwnd_window, options.AcceptTopMostWindows() ) )
		return hwnd_window;

	EnumWindows ( (WNDENUMPROC) EnumWindowsProc, (LPARAM) &hwnd_window);
	return hwnd_window;
}

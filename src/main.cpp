// Fichier main_application.cpp
#include <windows.h>
#include <tlhelp32.h>
#include <wx/stdpaths.h>
#include <wx/socket.h>

#include "main.h"
#include "settingsmanager.h"
#include "dialog_activewndtools.h"

using namespace std;

BEGIN_EVENT_TABLE (WinSplitApp, wxApp)
	EVT_END_SESSION (WinSplitApp::OnCloseSession)
END_EVENT_TABLE()

IMPLEMENT_APP (WinSplitApp)

WinSplitApp::WinSplitApp() {}

WinSplitApp::~WinSplitApp()
{
}

bool WinSplitApp::OnInit()
{
	setlocale (LC_NUMERIC, "C");
	// On spécifie "en dur" le nom de l'application
	SetAppName (_T ("Winsplit Revolution") );
	// On vérifie tout de suite si une autre instance ne tourne pas déjà
	if (IsAlreadyRunning() )
	{
		wxMessageBox (_ ("Program already running!"), _ ("WinSplit message"), wxOK | wxICON_INFORMATION);
		return false;
	}

	SettingsManager &options = SettingsManager::Get();
	if (!options.IsOk() )
	{
		wxMessageBox (_ ("There were errors during initialization !\nWinsplit Revolution can't start."), _ ("Errors"), wxICON_ERROR);
		return false;
	}

	// Si les options le spécifient, on supprime les éventuels fichiers temporaires
	// provenant de captures d'écran précédentes
	if ( (options.getAutoDeleteTempFiles() ) && (options.getAutoDeleteTime() == 0) )
		ActiveWndToolsDialog::DeleteTempFiles();

	wxInitAllImageHandlers();
	wxSocketBase::Initialize();

	// Création de TrayIcon qui va gérer le lancement/management du programme
	p_frameHook = new FrameHook();

	p_frameHook->Show();

	p_tray = new TrayIcon();

	return true;
}

void WinSplitApp::OnCloseSession (wxCloseEvent& event)
{
	p_tray->SaveOnExit();

	event.Skip();
}

int WinSplitApp::OnExit()
{
	delete p_frameHook;
	delete p_tray;

	// Si les options le spécifient, on supprime les éventuels fichiers temporaires
	// provenant de captures d'écran précédentes
	SettingsManager& options = SettingsManager::Get();
	if ( (options.getAutoDeleteTempFiles() ) && (options.getAutoDeleteTime() == 1) )
		ActiveWndToolsDialog::DeleteTempFiles();
	// On pense à détruire l'instance du SettingsManager
	SettingsManager::Kill();
	// On détruit également l'instance du wxSingleInstanceChecker
	delete p_checker;
}

bool WinSplitApp::IsAlreadyRunning()
{
	SettingsManager &options = SettingsManager::Get();
	wxString processus = _T ("");

	processus << _T ("WinsplitRevolution") << _T (" - ") << options.GetUserName();
	p_checker = new wxSingleInstanceChecker();
	if (!p_checker->Create (processus) )
	{
		wxMessageBox (_T ("Unable to create SingleInstanceChecker !"), _ ("Error"), wxICON_ERROR);
		return false;
	}
	else
		return p_checker->IsAnotherRunning();
}

wxString WinSplitApp::GetVersion()
{
	wxString sResult;
	wxString sfname = argv[0];
	DWORD dwH;
	DWORD dwSize = GetFileVersionInfoSize (sfname.c_str(), &dwH);
	char *szVerInf = new char[dwSize + 5];
	GetFileVersionInfo (sfname, 0, dwSize + 4, szVerInf);
	wxChar *szValue;
	UINT uiSize;
	VerQueryValue (szVerInf, L"\\StringFileInfo\\040c04b0\\FileVersion", (void**) &szValue, &uiSize);
	sResult = wxString (szValue);
	delete szVerInf;
	return sResult;
}

void WinSplitApp::ShowTrayIcon()
{
	p_tray->ShowIcon();
}

// Fichier tray_icon.h
#ifndef __TASKBARICON__
#define __TASKBARICON__

#include <wx/taskbar.h>

#include "frame_virtualnumpad.h"
#include "hotkeys_manager.h"
#include "settingsmanager.h"
#include "update_thread.h"

enum Identifiers_tray
{
    ID_MENU_ABOUT=1000,
    ID_MENU_HK,
    ID_MENU_RULE,
    ID_MENU_LAYOUT,
    ID_MENU_AUTOBOOT,
    ID_MENU_QUIT,
    ID_TIMER,
    ID_MENU_HELP
};

class TrayIcon: public wxTaskBarIcon
{
private:
    bool m_autoStart;
    VirtualNumpad* p_virtNumpad;
    HotkeysManager* p_hotkeys;
    SettingsManager &m_options;
    ReadVersionThread* p_updateThread;

    wxTimer m_timer;
    //int iconupdate;

    wxImage im_about;
    wxImage im_help;
    wxImage im_hotkeys;
    wxImage im_layout_settings;
    wxImage im_options;
    wxImage im_startwin_true;
    wxImage im_startwin_false;
    wxImage im_exit;

private:
    void OnTimer(wxTimerEvent& event);
    virtual wxMenu* CreatePopupMenu();
    void OnMenuClickAbout(wxCommandEvent&);
    void OnMenuClickHelp(wxCommandEvent&);
    void OnMenuClickRule(wxCommandEvent&);
    void OnMenuClickLayout(wxCommandEvent& event);
    void OnMenuClickAutoStart(wxCommandEvent&);
    void OnMenuClickConfigureHK(wxCommandEvent &event);
    void OnMenuClickQuit(wxCommandEvent&);

public:
    TrayIcon();
    ~TrayIcon();

    void ReadRegisterAutoStart();
    void WriteRegisterAutoStart(bool start_with_windows);
    void SetHotkeys();
    void ShowOrHideVirtualNumpad ();
    void OnLeftClick(wxTaskBarIconEvent &event);
    void SaveOnExit();
    void ShowIcon();

    void LoadImages();

    DECLARE_EVENT_TABLE()

};

#endif //__TASKBARICON__

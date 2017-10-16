
#include <windows.h>
#include <wx/msw/registry.h>

#include "frame_virtualnumpad.h"
#include "tray_icon.h"
#include "dialog_about.h"
#include "dialog_hotkeys.h"
#include "fonctions_resize.h"
#include "fonctions_speciales.h"
#include "dialog_warnhotkeys.h"
#include "dialog_options.h"
#include "dialog_layout.h"
#include "layout_manager.h"
#include "dialog_download.h"

#include "main.h"

//==============
// Image loading
//==============
#include "../images/icone.xpm"
#include "../images/tray2.xpm"
#include "../images/tray3.xpm"
#include "../images/tray4.xpm"
#include "../images/tray5.xpm"
#include "../images/tray6.xpm"
//======================


BEGIN_EVENT_TABLE(TrayIcon,wxTaskBarIcon)

    EVT_MENU(ID_MENU_ABOUT,TrayIcon::OnMenuClickAbout)
    EVT_MENU(ID_MENU_RULE,TrayIcon::OnMenuClickRule)
    EVT_MENU(ID_MENU_LAYOUT,TrayIcon::OnMenuClickLayout)
    EVT_MENU(ID_MENU_HK,TrayIcon::OnMenuClickConfigureHK)
    EVT_MENU(ID_MENU_HELP,TrayIcon::OnMenuClickHelp)
    EVT_MENU(ID_MENU_AUTOBOOT,TrayIcon::OnMenuClickAutoStart)
    EVT_MENU(ID_MENU_QUIT,TrayIcon::OnMenuClickQuit)
    EVT_TIMER(ID_TIMER,TrayIcon::OnTimer)

    EVT_TASKBAR_LEFT_DOWN(TrayIcon::OnLeftClick)

END_EVENT_TABLE()

TrayIcon::TrayIcon() : m_options(SettingsManager::Get())
{
    ReadRegisterAutoStart();
    SetIcon(wxIcon(icone_xpm),_T("WinSplit Revolution ")+wxGetApp().GetVersion());

    p_hotkeys = new HotkeysManager(this);
    p_virtNumpad = new VirtualNumpad();

    p_updateThread = NULL;

    SetHotkeys();

    if (m_options.getSaveNumpadPosOnExit())
    {
        p_virtNumpad->SetPosition(m_options.getNumpadPosition());
        p_virtNumpad->SetReduced(m_options.getNumpadStyle());
        p_virtNumpad->SetCheckValue(!m_options.getNumpadAutoHide());
        p_virtNumpad->SetTransparency(m_options.getNumpadTransparency());
    }

    if (m_options.getShowNumpadAtBoot())
        p_virtNumpad->Show();

    LoadImages();

    LayoutManager::GetInstance()->LoadData();

    m_timer.SetOwner(this,ID_TIMER);
    m_timer.Start(200);
}

TrayIcon::~TrayIcon()
{
}

void TrayIcon::LoadImages()
{
    im_about.LoadFile(_T("..\\images\\about.png"),wxBITMAP_TYPE_PNG);

    im_help.LoadFile(_T("..\\images\\help.png"),wxBITMAP_TYPE_PNG);

    im_options.LoadFile(_T("..\\images\\options.png"),wxBITMAP_TYPE_PNG);

    im_layout_settings.LoadFile(_T("..\\images\\layout.png"),wxBITMAP_TYPE_PNG);

    im_hotkeys.LoadFile(_T("..\\images\\hotkeys.png"),wxBITMAP_TYPE_PNG);

    im_startwin_false.LoadFile(_T("..\\images\\auto_start_false.png"),wxBITMAP_TYPE_PNG);

    im_startwin_true.LoadFile(_T("..\\images\\auto_start_true.png"),wxBITMAP_TYPE_PNG);

    im_exit.LoadFile(_T("..\\images\\exit.png"),wxBITMAP_TYPE_PNG);
}

wxMenu* TrayIcon::CreatePopupMenu()
{
    wxMenu* main_menu = new wxMenu;

    wxMenuItem* menu_about = new  wxMenuItem(main_menu, ID_MENU_ABOUT,_("About WinSplit Revolution"));
    menu_about->SetBitmap(wxBitmap(im_about));

    wxMenuItem* menu_help = new  wxMenuItem(main_menu, ID_MENU_HELP,_("Help"));
    menu_help->SetBitmap(wxBitmap(im_help));

    wxMenuItem* menu_layout_settings = new  wxMenuItem(main_menu, ID_MENU_LAYOUT,_("Layout settings"));
    menu_layout_settings->SetBitmap(wxBitmap(im_layout_settings));

    wxMenuItem* menu_option = new  wxMenuItem(main_menu, ID_MENU_RULE,_("Options"));
    menu_option->SetBitmap(wxBitmap(im_options));

    wxMenuItem* menu_hk_configure = new  wxMenuItem(main_menu,ID_MENU_HK,_("Hotkeys settings"));
    menu_hk_configure->SetBitmap(wxBitmap(im_hotkeys));

    wxMenuItem* menu_start = new  wxMenuItem(main_menu, ID_MENU_AUTOBOOT,_("Launch with Windows"));
    menu_start->SetBitmap(wxBitmap(m_autoStart ? im_startwin_true : im_startwin_false));

    wxMenuItem* menu_quit = new wxMenuItem(main_menu, ID_MENU_QUIT,_("Exit"));
    menu_quit->SetBitmap(wxBitmap(im_exit));

    main_menu->Append(menu_about);
    main_menu->Append(menu_help);
    main_menu->AppendSeparator();
    main_menu->Append(menu_hk_configure);
    main_menu->Append(menu_layout_settings);
    main_menu->Append(menu_option);
    main_menu->Append(menu_start);
    main_menu->AppendSeparator();
    main_menu->Append(menu_quit);

    return main_menu;
}

void TrayIcon::OnMenuClickAbout(wxCommandEvent &event)
{
    AboutDialog dlg_about;
    dlg_about.ShowModal();
}

void TrayIcon::OnMenuClickHelp(wxCommandEvent& event)
{
    wxLaunchDefaultBrowser(_T("https://pupitetris.github.io/winsplit-reloaded/Revolution/help.html"));
}

void TrayIcon::OnMenuClickRule(wxCommandEvent &event)
{
    OptionsDialog dialog_option;
    dialog_option.SetVirtualNumPad(p_virtNumpad);
    dialog_option.ShowModal();
    // Do we need to reactivate or deactivate the Drag'N'Go hook?
    if (wxGetApp().GetFrameHook()->IsHookInstalled() != m_options.IsDragNGoEnabled())
    {
        if (m_options.IsDragNGoEnabled())
        {
            wxGetApp().GetFrameHook()->SetHook();
        }
        else
        {
            wxGetApp().GetFrameHook()->UnSetHook();
        }
    }
}

void TrayIcon::OnMenuClickConfigureHK(wxCommandEvent &event)
{
    int i;
    HotkeyConfigureDialog dlgHKConf(p_hotkeys);

    p_hotkeys->Stop();
    if (dlgHKConf.ShowModal() == wxID_OK)
    {
        if (dlgHKConf.IsModified())
        {
            p_hotkeys->SetVecHotkey(dlgHKConf.GetVecTmpHK());
            p_hotkeys->SaveData();
        }
    }
    p_hotkeys->Start();
}

void TrayIcon::OnMenuClickLayout(wxCommandEvent& event)
{
    LayoutDialog dialog;

    if (dialog.ShowModal() == wxOK)
        LayoutManager::GetInstance()->LoadData();
}

void TrayIcon::OnMenuClickAutoStart(wxCommandEvent &event)
{
    m_autoStart =! m_autoStart;
    WriteRegisterAutoStart(m_autoStart);
}

void TrayIcon::SaveOnExit()
{
    if (m_options.getSaveNumpadPosOnExit())
    {
        if (p_virtNumpad)
        {
            m_options.setNumpadAutoHide(!p_virtNumpad->GetCheckValue());
            m_options.setNumpadStyle(p_virtNumpad->IsReduced());
            m_options.setNumpadPosition(p_virtNumpad->GetNotReducedPosition());
        }
    }
}

void TrayIcon::OnMenuClickQuit(wxCommandEvent &event)
{
    if (p_hotkeys->Stop())
        delete p_hotkeys;

    SaveOnExit();
    p_virtNumpad->Destroy();
    RemoveIcon();
    LayoutManager::DeleteInstance();

    wxExit();
}

void TrayIcon::ReadRegisterAutoStart()
{
    wxRegKey* pRegKey = new wxRegKey(_T("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"));

    m_autoStart = pRegKey->HasValue(_T("Winsplit"));

    pRegKey->Close();

    delete pRegKey;
}

void TrayIcon::WriteRegisterAutoStart(bool start_with_windows)
{
    wxChar chemin[256];
    wxRegKey* pRegKey = new wxRegKey(_T("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"));
    if (start_with_windows)
    {
        GetModuleFileName(NULL,chemin,256);
        pRegKey->SetValue(_T("Winsplit"),chemin);
    }
    else
    {
        pRegKey->DeleteValue(_T("Winsplit"));
    }

    pRegKey->Close();

    delete pRegKey;
}

void TrayIcon::SetHotkeys()
{
    wxString message;
    WarnHotkeyDialog* p_warnframe = NULL;

    wxString erreur = p_hotkeys->Start();
    if (!erreur.IsEmpty())
    {
        message << _("Several global hotkeys are already allocated by another program:") << _T("\n");
        message << erreur;
        message << _("WinSplit will not invoke them.");

        if (m_options.hasToShowHotkeysWarnings())
        {
            p_warnframe = new WarnHotkeyDialog();
            p_warnframe->AppendText(message);
            p_warnframe->WriteErrorLog(message);
            p_warnframe->SetLabel(_("Hotkeys loading error!"));
            p_warnframe->ShowModal();

            if (p_warnframe->IsCheck())
            {
                m_options.setShowHotkeysWarnings(false);
                m_options.SaveSettings();
            }
            p_warnframe->Destroy();
        }
    }
}

// This is a tri-state toggle.
void TrayIcon::OnLeftClick(wxTaskBarIconEvent &event)
{
    p_virtNumpad->CycleShownReduced();
}

void TrayIcon::ShowOrHideVirtualNumpad ()
{
    p_virtNumpad->Show (!p_virtNumpad->IsShown ());
}

void TrayIcon::OnTimer(wxTimerEvent& event)
{
    static int iconupdate = 0;
    static int sens = 1;


    if(!p_updateThread)
    {
        p_updateThread = new ReadVersionThread(60);
        p_updateThread->Create();
        p_updateThread->Run();
    }

    if (p_updateThread->IsRunning())
    {
        switch (iconupdate)
        {
        case 0:
            sens = 1;
            ++iconupdate;
            SetIcon(wxIcon(icone_xpm),_("Checking for update"));
            break;
        case 1:
            if(sens>0)
                ++iconupdate;
            else
                --iconupdate;
            SetIcon(wxIcon(tray2_xpm),_("Checking for update"));
            break;
        case 2:
            if(sens>0)
                ++iconupdate;
            else
                --iconupdate;
            SetIcon(wxIcon(tray3_xpm),_("Checking for update"));
            break;
        case 3:
            if(sens>0)
                ++iconupdate;
            else
                --iconupdate;
            SetIcon(wxIcon(tray4_xpm),_("Checking for update"));
            break;
        case 4:
            if(sens>0)
                ++iconupdate;
            else
                --iconupdate;
            SetIcon(wxIcon(tray5_xpm),_("Checking for update"));
            break;
        case 5:
            sens = -1;
            --iconupdate;
            SetIcon(wxIcon(tray6_xpm),_("Checking for update"));
            break;
        default:
            iconupdate = 0;
            SetIcon(wxIcon(icone_xpm),_("Checking for update"));
            break;
        }
    }
    else
    {
        m_timer.Stop();
        SetIcon(wxIcon(icone_xpm),_T("WinSplit Revolution ")+wxGetApp().GetVersion());

        wxString versionText = p_updateThread->GetFeatures();
        double host_version = p_updateThread->GetHostVersion();
        double local_version;

        wxGetApp().GetVersion().ToDouble(&local_version);

        delete p_updateThread;
        p_updateThread = NULL;

        if (host_version > local_version)
        {
            if(versionText != wxEmptyString)
            {
                DownloadDialog dialog(versionText);
                if(dialog.ShowModal() == wxOK)
                {
                    wxCommandEvent event;
                    event.SetId(ID_MENU_QUIT);
                    event.SetEventType(wxEVT_COMMAND_MENU_SELECTED);
                    event.SetEventObject(this);
                    ProcessEvent(event);
                }

            }
        }
    }
}

void TrayIcon::ShowIcon()
{
    SetIcon(wxIcon(icone_xpm),_T("WinSplit Revolution ")+wxGetApp().GetVersion());
}

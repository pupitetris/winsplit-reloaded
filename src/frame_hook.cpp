
#include <windows.h>
#include <winuser.h>

#include "frame_hook.h"
#include "hook.h"
#include "main.h"

using namespace std;

FrameHook::FrameHook(wxWindow* parent, wxWindowID id, const wxString& title,const wxPoint& pos,const wxSize& size,long style)
:wxFrame(parent,id,title,pos,size,style),
p_panel(NULL),
p_stcTxtInfo(NULL),
m_timer(),
m_vec_solution(),
WSM_STARTMOVING(0),
WSM_STOPMOVING(0),
WSM_MOLETTE(0),
m_options(SettingsManager::Get()),
m_iTransparency(45),
m_is_near(false),
m_isInstalled(false),
m_rectPrevious(-1,0,0,0),
m_wheelpos(0),
m_wheelposPrevious(-1)
{
    // Pour etre sur que rien n'apparaisse
    SetTransparent(0);

    CreateControls();
    CreateConnection();

    SetHook();
}

FrameHook::~FrameHook()
{
    StopAllHook();

    // On libere la DLL
    HMODULE module_dll=GetModuleHandle(L"winsplithook.dll");
    //GetModuleHandleEx(0,L"winsplithook.dll",&module_dll);
    FreeLibrary(module_dll);

    p_panel->Show(false);
    p_panel->Destroy();
}


void FrameHook::SetHook()
{
    if(!(m_isInstalled = InstallAllHook((HWND)GetHandle())))
        wxMessageBox(_("Impossible to install hooks"));
}


void FrameHook::UnSetHook()
{
    if(!StopAllHook())
    {
        wxMessageBox(_("Next WinSplit Revolution run in this loging session will probably cause Drag n'go crash\n"
                    "Restart your computer may solve this problem"),
                    _("Impossible to uninstall some hooks"),wxICON_ERROR);

    }
    else
    {
        m_isInstalled = false;
    }
}


void FrameHook::CreateControls()
{
    p_panel = new wxFrame(NULL,wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(0,0),wxSTAY_ON_TOP|wxFRAME_NO_TASKBAR);
    p_panel->SetBackgroundColour(m_options.getDnGZoneBgColor());
    m_iTransparency=m_options.getDngZoneTransparency();
    p_panel->SetTransparent((255*m_iTransparency)/100);

    wxBoxSizer* vsizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* hsizer = new wxBoxSizer(wxVERTICAL);

    p_stcTxtInfo = new wxStaticText(p_panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(250,200),wxALIGN_CENTRE);
    p_stcTxtInfo->SetForegroundColour(m_options.getDnGZoneFgColor());
    p_stcTxtInfo->SetFont(wxFont(24,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD));

    hsizer->AddStretchSpacer();
    hsizer->Add(p_stcTxtInfo,0,wxALIGN_CENTER|wxALL,0);
    hsizer->AddStretchSpacer();

    vsizer->AddStretchSpacer();
    vsizer->Add(hsizer,0,wxALIGN_CENTER|wxALL,0);
    vsizer->AddStretchSpacer();

    p_panel->SetSizer(vsizer);
    p_panel->Disable();
}

void FrameHook::CreateConnection()
{
    WSM_STARTMOVING = RegisterWindowMessage(_T("WinSplitMessage_StartMoving"));
    WSM_STOPMOVING = RegisterWindowMessage(_T("WinSplitMessage_StopMoving"));
    WSM_MOLETTE = RegisterWindowMessage(_T("WinSplitMessage_Wheel"));

    m_timer.SetOwner(this,wxID_ANY);
    Connect(wxEVT_TIMER,wxTimerEventHandler(FrameHook::OnTimer),NULL,this);

    WSM_TASKBAR_CREATED = RegisterWindowMessage(_T("TaskbarCreated"));
}



void FrameHook::OnTimer(wxTimerEvent& event)
{
    if(IsDown())
    {
        wxString message;

        m_is_near = LayoutManager::GetInstance()->GetNearestFromCursor(m_vec_solution);

        if(m_is_near)
        {
            if((m_vec_solution[0] != m_rectPrevious) || (m_wheelpos != m_wheelposPrevious))
            {
                p_panel->SetSize(m_vec_solution[m_wheelpos%m_vec_solution.size()]);

                p_panel->Show(true);
                m_rectPrevious = m_vec_solution[0];
                m_wheelposPrevious = m_wheelpos;

                if(m_vec_solution.size()>1)
                {
                    message.Clear();
                    message.Printf(_T("%d\n"),(int)(m_vec_solution.size()));
                    message << _("Possibilities scroll to switch");

                    p_stcTxtInfo->SetLabel(message);
                    p_panel->GetSizer()->Layout();
                } else {
					p_stcTxtInfo->SetLabel(wxEmptyString);
				}
            }
        }
        else
        {
            p_panel->Show(false);
            m_rectPrevious.x = -1;
            m_wheelpos = 0;
            m_wheelposPrevious = 0;
        }
       //}
    }
    else
    {
        p_panel->Show(false);
    }

    if(!(GetKeyState(VK_LBUTTON )&0x0100))
    {
        if(m_timer.IsRunning())
            m_timer.Stop();

        p_panel->Show(false);
        m_rectPrevious.x = -1;
    }

	event.Skip(false);
}


WXLRESULT FrameHook::MSWWindowProc(WXUINT nMsg, WXWPARAM wparam, WXLPARAM lparam)
{
	if(nMsg == WSM_STARTMOVING)
	{
	    if (m_options.getDnGZoneBgColor()!=p_panel->GetBackgroundColour())
            p_panel->SetBackgroundColour(m_options.getDnGZoneBgColor());
        if (m_options.getDnGZoneFgColor()!=p_panel->GetForegroundColour())
            p_stcTxtInfo->SetForegroundColour(m_options.getDnGZoneFgColor());
        if (m_iTransparency!=m_options.getDngZoneTransparency())
        {
            m_iTransparency=m_options.getDngZoneTransparency();
            p_panel->SetTransparent((255*m_iTransparency)/100);
        }

		if(!m_timer.IsRunning())
		{
		    m_wheelpos = 0;
		    m_wheelposPrevious = 0;
		    m_rectPrevious.x = -1;

			m_timer.Start(100);
		}

		return 0;
	}
	else if(nMsg == WSM_STOPMOVING)
	{
		if(m_timer.IsRunning())
			m_timer.Stop();

        p_panel->Show(false);

		MoveWindowToDestination();

		return 0;
	}
	else if(nMsg == WSM_MOLETTE)
	{
	    if(m_timer.IsRunning())
	    {
	        m_wheelposPrevious = m_wheelpos;
            if(int(wparam)>0)
                ++m_wheelpos;
            else
                --m_wheelpos;
	    }
        return 0;
	}
	else if(nMsg == WSM_TASKBAR_CREATED)
	{
	    wxGetApp().ShowTrayIcon();
	}

    return wxFrame::MSWWindowProc(nMsg, wparam, lparam);
}

void FrameHook::MoveWindowToDestination()
{
	if(IsDown())
	{
	    p_panel->Show(false);

	    if(m_is_near)
	    {
	        wxRect rect_dest = m_vec_solution[m_wheelpos%m_vec_solution.size()];
            SetWindowPos(GetForegroundWindow(),HWND_TOP,rect_dest.x,rect_dest.y,rect_dest.width,rect_dest.height,SWP_SHOWWINDOW);
            m_rectPrevious.x = -1;
	    }
	}
}

bool FrameHook::IsDown()
{
    if((GetKeyState(ModifierToVK(m_options.getDnGMod1()))&0x0100) || (m_options.getDnGMod1()==0))
    {
        if((GetKeyState(ModifierToVK(m_options.getDnGMod2()))&0x0100) || (m_options.getDnGMod2()==0))
        {
            return true;
        }
    }
    return false;
}


unsigned int FrameHook::ModifierToVK(const unsigned int& mod)
{
    switch(mod)
    {
        case MOD_CONTROL:
            return VK_CONTROL;
        case MOD_ALT:
            return VK_MENU;
        case MOD_SHIFT:
            return VK_SHIFT;
        case MOD_WIN:
            return VK_LWIN;
		case 10:
			return VK_MBUTTON;
		case 11:
			return VK_RBUTTON;
        default:
            return 0;
    }

    return 0;
}

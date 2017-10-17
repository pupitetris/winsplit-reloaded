#include <windows.h>

#include "frame_virtualnumpad.h"
#include "fonctions_resize.h"
#include "settingsmanager.h"

#include "../images/a1.xpm"
#include "../images/a2.xpm"
#include "../images/a3.xpm"
#include "../images/a4.xpm"
#include "../images/a5.xpm"
#include "../images/a6.xpm"
#include "../images/a7.xpm"
#include "../images/a8.xpm"
#include "../images/a9.xpm"

VirtualNumpad::VirtualNumpad (wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame (parent, id, title, pos, size, style),
	  p_checkStay (NULL),
	  p_sizer (NULL),
	  p_panel (NULL),
	  m_reducedStyle (false),
	  m_frame_pos()
{
	CreateControls();
	CreateConnections();

	Center();

	m_reducedStyle = false;

	SettingsManager &settings = SettingsManager::Get();
	m_frame_pos = settings.getNumpadPosition();

	p_checkStay->SetValue (!settings.getNumpadAutoHide() );

	HMENU hSysMenu = GetSystemMenu ( (HWND) GetHandle(), FALSE);
	DeleteMenu (hSysMenu, SC_MAXIMIZE, MF_BYCOMMAND);
	DeleteMenu (hSysMenu, SC_MINIMIZE, MF_BYCOMMAND);

}


void VirtualNumpad::CreateConnections()
{
	Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (VirtualNumpad::OnClicEvent), NULL, this);
	Connect (wxEVT_CLOSE_WINDOW, wxCloseEventHandler (VirtualNumpad::OnCLose), NULL, this);
}


void VirtualNumpad::CreateControls()
{
	wxGridSizer* gridsizer;

	p_panel = new wxPanel (this);

	wxBitmapButton* btn1 = new wxBitmapButton (p_panel, ID_1, wxBitmap (a1_xpm), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	wxBitmapButton* btn2 = new wxBitmapButton (p_panel, ID_2, wxBitmap (a2_xpm), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	wxBitmapButton* btn3 = new wxBitmapButton (p_panel, ID_3, wxBitmap (a3_xpm), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	wxBitmapButton* btn4 = new wxBitmapButton (p_panel, ID_4, wxBitmap (a4_xpm), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	wxBitmapButton* btn5 = new wxBitmapButton (p_panel, ID_5, wxBitmap (a5_xpm), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	wxBitmapButton* btn6 = new wxBitmapButton (p_panel, ID_6, wxBitmap (a6_xpm), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	wxBitmapButton* btn7 = new wxBitmapButton (p_panel, ID_7, wxBitmap (a7_xpm), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	wxBitmapButton* btn8 = new wxBitmapButton (p_panel, ID_8, wxBitmap (a8_xpm), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	wxBitmapButton* btn9 = new wxBitmapButton (p_panel, ID_9, wxBitmap (a9_xpm), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	p_checkStay = new wxCheckBox (p_panel, ID_CHECK, _ ("Keep") );
	p_sizer = new wxBoxSizer (wxVERTICAL);

	gridsizer = new wxGridSizer (3, 3, 0, 0);

	gridsizer->Add (btn7, 0, wxALIGN_CENTER | wxALL, 0);
	gridsizer->Add (btn8, 0, wxALIGN_CENTER | wxALL, 0);
	gridsizer->Add (btn9, 0, wxALIGN_CENTER | wxALL, 0);
	gridsizer->Add (btn4, 0, wxALIGN_CENTER | wxALL, 0);
	gridsizer->Add (btn5, 0, wxALIGN_CENTER | wxALL, 0);
	gridsizer->Add (btn6, 0, wxALIGN_CENTER | wxALL, 0);
	gridsizer->Add (btn1, 0, wxALIGN_CENTER | wxALL, 0);
	gridsizer->Add (btn2, 0, wxALIGN_CENTER | wxALL, 0);
	gridsizer->Add (btn3, 0, wxALIGN_CENTER | wxALL, 0);

	p_sizer->Add (gridsizer, 0, wxALIGN_CENTER | wxALL, 0);
	p_sizer->Add (p_checkStay, 0, wxALIGN_CENTER | wxALL, 5);

	p_panel->SetSizer (p_sizer);
	p_sizer->SetSizeHints (this);
}


VirtualNumpad::~VirtualNumpad()
{
}


void VirtualNumpad::SetReduced (bool reduced)
{
	m_reducedStyle = reduced;

	SetStyleReduced (reduced);
}


bool VirtualNumpad::IsReduced()
{
	return m_reducedStyle;
}


void VirtualNumpad::OnCLose (wxCloseEvent& event)
{
	Show (false);
}

void VirtualNumpad::OnClicEvent (wxCommandEvent& event)
{
	ResizeWindow (event.GetId(), false);

	if (!p_checkStay->GetValue() )
		Show (false);
}

void VirtualNumpad::SetTransparency (int value)
{
	SetTransparent (255 - (255 * value) / 100);
}

void VirtualNumpad::SetStyleReduced (bool reduce)
{
	p_checkStay->Show (!reduce);

	long style = wxSTAY_ON_TOP | wxFRAME_TOOL_WINDOW;
	if (reduce)
	{
		m_frame_pos = GetPosition();
		wxPoint tmp = p_sizer->GetContainingWindow()->GetPosition();

		// Correction for a shift in the result.
		--tmp.x;
		--tmp.y;

		Move (ClientToScreen (tmp) );
	}
	else
	{
		style |= wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX;
		Move (m_frame_pos);
	}

	SetWindowStyleFlag (style);

	p_sizer->SetSizeHints (this);
}

void VirtualNumpad::CycleShownReduced()
{
	if (!IsShown() ) // First state: not shown.
	{
		SetReduced (true);
		Show();
		return;
	}

	if (IsReduced() ) // Second state: reduced.
	{
		SetReduced (false);
		return;
	}

	// Remaining third state: reduced.
	Show (false);
}

void VirtualNumpad::CenterOnScreen ()
{
	bool is_reduced = IsReduced();
	SetReduced(false);

	HMONITOR hmonitor;
	MONITORINFO monitor_info;

	HWND hwnd = (HWND) GetHandle ();

	hmonitor = MonitorFromWindow (hwnd, MONITOR_DEFAULTTONEAREST);
	monitor_info.cbSize = sizeof (MONITORINFO);
	GetMonitorInfo (hmonitor, &monitor_info);

	RECT screen_rect = monitor_info.rcWork;
	LONG scr_width = screen_rect.right - screen_rect.left;
	LONG scr_height = screen_rect.bottom - screen_rect.top;

	RECT wnd;
	GetWindowRect (hwnd, &wnd);

	LONG width = wnd.right - wnd.left;
	LONG height = wnd.bottom - wnd.top;

	RECT new_geom;
	new_geom.left = screen_rect.left + (scr_width - width) / 2;
	new_geom.top = screen_rect.top + (scr_height - height) / 2;

	new_geom.right = new_geom.left + width / 2;
	new_geom.bottom = new_geom.top + height / 2;

	SetWindowPos (hwnd, HWND_TOP, new_geom.left, new_geom.top, width, height, SWP_NOSIZE);

	m_frame_pos = GetPosition();

	if (is_reduced)
		SetReduced(true);
}

void VirtualNumpad::SetCheckValue (bool checked)
{
	p_checkStay->SetValue (checked);
}

bool VirtualNumpad::GetCheckValue()
{
	return p_checkStay->GetValue();
}

wxPoint VirtualNumpad::GetNotReducedPosition()
{
	return m_frame_pos;
}

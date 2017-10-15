
#include <wx/wx.h>
#include <wx/statline.h>
#include <windows.h>

#include "dialog_hotkeys.h"
#include "../images/icone.xpm"

using namespace std;

ChooseHotkeyDialog::ChooseHotkeyDialog(wxWindow* parent,int id,const wxString& title)
:wxDialog(parent,id,title)
{
	SetIcon(wxIcon(icone_xpm));
	CreateControls();
	InitCombo();

	p_btnOk->Connect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(ChooseHotkeyDialog::OnButOk),NULL,this);

	Center();
}

ChooseHotkeyDialog::~ChooseHotkeyDialog(){}


void ChooseHotkeyDialog::CreateControls()
{
	wxBoxSizer* pmainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* p_sizer2 = new wxBoxSizer(wxHORIZONTAL);
	this->SetSizer(pmainSizer);

	wxStaticText* p_label1 = new wxStaticText(this,wxID_ANY,_T("+"));
	wxStaticText* p_label2 = new wxStaticText(this,wxID_ANY,_T("+"));

	p_btnOk = new wxButton(this,wxID_OK,_("&Apply"));
	p_btnCancel = new wxButton(this,wxID_CANCEL,_("&Cancel"));
	p_cmbMod1 = new wxComboBox(this,wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(55,20),0,NULL,wxCB_READONLY);
	p_cmbMod2 = new wxComboBox(this,wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(55,20),0,NULL,wxCB_READONLY);
	p_cmbVirtKey = new wxComboBox(this,wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(100,20),0,NULL,wxCB_READONLY);

	wxStdDialogButtonSizer* p_dlgBtn = new wxStdDialogButtonSizer();
	p_dlgBtn->AddButton(p_btnOk);
	p_dlgBtn->AddButton(p_btnCancel);
	p_dlgBtn->Realize();

	p_sizer2->Add(p_cmbMod1,0,wxALIGN_CENTER|wxALL,5);
	p_sizer2->Add(p_label1,0,wxALIGN_CENTER|wxALL,5);
	p_sizer2->Add(p_cmbMod2,0,wxALIGN_CENTER|wxALL,5);
	p_sizer2->Add(p_label2,0,wxALIGN_CENTER|wxALL,5);
	p_sizer2->Add(p_cmbVirtKey,0,wxALIGN_CENTER|wxALL,5);

	wxStaticLine* p_line=new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxSize(150,-1));

	pmainSizer->Add(p_sizer2,0,wxALIGN_CENTER|wxALL,5);
	pmainSizer->Add(p_line,0,wxALIGN_CENTER|wxALL|wxEXPAND,5);
	pmainSizer->Add(p_dlgBtn,0,wxALIGN_CENTER|wxALL,5);

	pmainSizer->SetSizeHints(this);
}

void ChooseHotkeyDialog::InitCombo()
{
	p_cmbMod1->Clear();
	p_cmbMod2->Clear();
	p_cmbVirtKey->Clear();

	p_cmbMod1->Append(m_mod_manager.GetStringFromIndex(1));
	p_cmbMod1->Append(m_mod_manager.GetStringFromIndex(2));
	p_cmbMod1->Append(m_mod_manager.GetStringFromIndex(3));
    p_cmbMod1->Append(m_mod_manager.GetStringFromIndex(4));
	p_cmbMod1->Select(1);

    p_cmbMod2->Append(m_mod_manager.GetStringFromIndex(0));
    p_cmbMod2->Append(m_mod_manager.GetStringFromIndex(1));
    p_cmbMod2->Append(m_mod_manager.GetStringFromIndex(2));
    p_cmbMod2->Append(m_mod_manager.GetStringFromIndex(3));
    p_cmbMod2->Append(m_mod_manager.GetStringFromIndex(4));
	p_cmbMod2->Select(1);

	for(unsigned int i=0;i<m_keys_manager.GetSize();++i)
		p_cmbVirtKey->Append(m_keys_manager.GetStringFromIndex(i));

	p_cmbVirtKey->Select(0);
}

HotkeyStruct ChooseHotkeyDialog::GetHotkeys()
{
	HotkeyStruct st_hk;

	st_hk.modifier1 = m_mod_manager.GetValueFromIndex(p_cmbMod1->GetSelection()+1);
	st_hk.modifier2 = m_mod_manager.GetValueFromIndex(p_cmbMod2->GetSelection());//tab_modifier[p_cmbMod2->GetSelection()];
	st_hk.virtualKey = m_keys_manager.GetValueFromIndex(p_cmbVirtKey->GetSelection());

	return st_hk;
}

void ChooseHotkeyDialog::SetHotkeys(HotkeyStruct hk,int num_item)
{
	p_cmbMod1->SetSelection(m_mod_manager.GetIndexFromValue(hk.modifier1)-1);//GetIndexVal(tab_modifier,5,hk.modifier1)-1);
	p_cmbMod2->SetSelection(m_mod_manager.GetIndexFromValue(hk.modifier2));
	p_cmbVirtKey->SetSelection(m_keys_manager.GetIndexFromValue(hk.virtualKey));

	n_item = num_item;
}

void ChooseHotkeyDialog::OnButOk(wxCommandEvent &event)
{
	int id = 0;

	for(unsigned int i=0; i<vec_hk.size(); ++i)
	{
		if(i != (unsigned int)n_item)
			if(vec_hk[i].modifier1 == m_mod_manager.GetValueFromIndex(p_cmbMod1->GetSelection()+1) &&
				vec_hk[i].modifier2 == m_mod_manager.GetValueFromIndex(p_cmbMod2->GetSelection()) &&
				vec_hk[i].virtualKey == m_keys_manager.GetValueFromIndex(p_cmbVirtKey->GetSelection()))
			{
				wxMessageBox(_("This hotkey is already used by WinSplit"),_("WinSplit message"),wxOK,this);
				return;
			}
	}

	if(!(::RegisterHotKey((HWND)GetHandle(),id,
			m_mod_manager.GetValueFromIndex(p_cmbMod1->GetSelection()+1)|m_mod_manager.GetValueFromIndex(p_cmbMod2->GetSelection()),
			m_keys_manager.GetValueFromIndex(p_cmbVirtKey->GetSelection()))))
	{
		wxMessageBox(_("This hotkey is already used by Windows"),_("WinSplit message"),wxOK,this);
		::UnregisterHotKey((HWND)GetHandle(),id);
		return;
	}
	::UnregisterHotKey((HWND)GetHandle(),id);

	event.Skip();
}

void ChooseHotkeyDialog::SetVecHK(const vector<HotkeyStruct> &vec)
{
	vec_hk = vec;
}

//===========================
//  HOTKEY CONFIGURE FRAME
//===========================
HotkeyConfigureDialog::HotkeyConfigureDialog(HotkeysManager* p)
:p_listbox(NULL),
p_btnEdit(NULL),
p_btnActiv(NULL),
dlgChooseHK(this),
p_hkManager(p),
m_item_selected(-1),
m_modified(false)
{
    wxDialog::Create(NULL,wxID_ANY,_("Hotkey settings"));
	m_modified = false;
	SetIcon(wxIcon(icone_xpm));
	CreateControls();
	CreateConnexions();

    vec_tab_tmp = p_hkManager->GetCpyVector();

	InitList();
	p_listbox->SetColumnWidth(0,wxLIST_AUTOSIZE);
	p_listbox->SetColumnWidth(1,wxLIST_AUTOSIZE);

	GetSizer()->SetSizeHints(this);

	Center();

	dlgChooseHK.SetVecHK(vec_tab_tmp);
}

HotkeyConfigureDialog::~HotkeyConfigureDialog()
{
}


void HotkeyConfigureDialog::CreateControls()
{
	wxBoxSizer* pmainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* psizer_2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* psizer_3 = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(pmainSizer);

	wxButton* pbtnOk = new wxButton(this,wxID_OK,_("&Ok"));
	wxButton* pbtnCancel = new wxButton(this,wxID_CANCEL,_("&Cancel"));

	p_btnEdit = new wxButton(this,wxID_ANY,_("&Edit"));
	p_btnActiv = new wxButton(this,wxID_ANY,_("&Disable"));

	p_listbox = new wxListCtrl(this,wxID_ANY,wxDefaultPosition,wxSize(274,270),wxLC_REPORT|wxSTATIC_BORDER|wxLC_SINGLE_SEL|wxLC_VRULES|wxLC_HRULES);


	wxStdDialogButtonSizer* pdialog_but = new wxStdDialogButtonSizer();
	pdialog_but->AddButton(pbtnOk);
	pdialog_but->AddButton(pbtnCancel);
	pdialog_but->Realize();

	psizer_3->Add(p_btnEdit,0,wxALIGN_CENTER|wxALL,5);
	psizer_3->Add(p_btnActiv,0,wxALIGN_CENTER|wxALL,5);

	psizer_2->Add(p_listbox,1,wxALIGN_CENTER|wxALL,5);
	psizer_2->Add(psizer_3,0,wxALIGN_CENTER|wxALL,5);

	pmainSizer->Add(psizer_2,0,wxALIGN_CENTER|wxALL,5);
	pmainSizer->Add(new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxSize(10,-1)),1,wxALIGN_CENTER|wxALL|wxEXPAND,5);
	pmainSizer->Add(pdialog_but,0,wxALIGN_CENTER|wxALL,5);
}


void HotkeyConfigureDialog::CreateConnexions()
{
    p_btnActiv->Connect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(HotkeyConfigureDialog::OnButActiv),NULL,this);
    p_btnEdit->Connect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(HotkeyConfigureDialog::OnButEdit),NULL,this);
    p_listbox->Connect(wxEVT_COMMAND_LIST_ITEM_SELECTED,wxListEventHandler(HotkeyConfigureDialog::OnTaskChangeSelection),NULL,this);
    p_listbox->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED,wxListEventHandler(HotkeyConfigureDialog::OnTaskDoubleClick),NULL,this);
}


int HotkeyConfigureDialog::GetIndexVal(int* tab,int size,int val)
{
	int i;

	for(i = 0; i<size; ++i)
		if(tab[i] == val)
			return i;

	return -1;
}


wxString HotkeyConfigureDialog::GetIndexStr(int val)
{
	wxString str_out;
	int id = m_mod_manager.GetIndexFromValue(vec_tab_tmp[val].modifier1);

	str_out = m_mod_manager.GetStringFromIndex(id);
	str_out += _T("+");

	if((id=m_mod_manager.GetIndexFromValue(vec_tab_tmp[val].modifier2))!=0)
	{
		str_out += m_mod_manager.GetStringFromIndex(id);
		str_out += _T("+");
	}

	str_out += m_keys_manager.GetStringFromValue(vec_tab_tmp[val].virtualKey);

	return str_out;
}


void HotkeyConfigureDialog::InitList()
{
	wxString str_in;

	p_listbox->InsertColumn(0,_("Action"),wxLIST_FORMAT_LEFT,124);
	p_listbox->InsertColumn(1,_("Hotkey"),wxLIST_FORMAT_LEFT,137);

	p_listbox->InsertItem(0,_("Automatic placement"));
	p_listbox->SetItem(0,1,GetIndexStr(0));

	str_in.Clear();
	str_in << _("Down") << _T(" ") << _("Left");
	p_listbox->InsertItem(1,str_in);
	p_listbox->SetItem(1,1,GetIndexStr(1));

	p_listbox->InsertItem(2,_("Down"));
	p_listbox->SetItem(2,1,GetIndexStr(2));

	str_in.Clear();
	str_in << _("Down") << _T(" ") << _("Right");
	p_listbox->InsertItem(3,str_in);
	p_listbox->SetItem(3,1,GetIndexStr(3));

	p_listbox->InsertItem(4,_("Left"));
	p_listbox->SetItem(4,1,GetIndexStr(4));

	p_listbox->InsertItem(5,_("Center"));
	p_listbox->SetItem(5,1,GetIndexStr(5));

	p_listbox->InsertItem(6,_("Right"));
	p_listbox->SetItem(6,1,GetIndexStr(6));

	str_in.Clear();
	str_in << _("Up") << _T(" ") << _("Left");
	p_listbox->InsertItem(7,str_in);
	p_listbox->SetItem(7,1,GetIndexStr(7));

	p_listbox->InsertItem(8,_("Up"));
	p_listbox->SetItem(8,1,GetIndexStr(8));

	str_in.Clear();
	str_in << _("Up") << _T(" ") << _("Right");
	p_listbox->InsertItem(9,str_in);
	p_listbox->SetItem(9,1,GetIndexStr(9));

	p_listbox->InsertItem(10,_("Mosaic"));
	p_listbox->SetItem(10,1,GetIndexStr(10));

	p_listbox->InsertItem(11,_("Windows fusion"));
	p_listbox->SetItem(11,1,GetIndexStr(11));

	p_listbox->InsertItem(12,_("Close all window"));
	p_listbox->SetItem(12,1,GetIndexStr(12));

	p_listbox->InsertItem(13,_("Move to Left screen"));
	p_listbox->SetItem(13,1,GetIndexStr(13));

	p_listbox->InsertItem(14,_("Move to Right screen"));
	p_listbox->SetItem(14,1,GetIndexStr(14));

	p_listbox->InsertItem(15,_("Minimize Window"));
	p_listbox->SetItem(15,1,GetIndexStr(15));

	p_listbox->InsertItem(16,_("Maximize window"));
	p_listbox->SetItem(16,1,GetIndexStr(16));

	p_listbox->InsertItem(17, _("Active window tools"));
	p_listbox->SetItem(17,1,GetIndexStr(17));

	p_listbox->InsertItem(18, _("Toggle Always on Top"));
	p_listbox->SetItem(18,1,GetIndexStr(18));

	p_listbox->InsertItem(19, _("Maximize horizontally"));
	p_listbox->SetItem(19,1,GetIndexStr(19));

	p_listbox->InsertItem(20, _("Maximize vertically"));
	p_listbox->SetItem(20,1,GetIndexStr(20));

	p_listbox->SetItemState(0,wxLIST_STATE_SELECTED,wxLIST_STATE_SELECTED);
	m_item_selected=0;

	for(unsigned int i=0; i<vec_tab_tmp.size(); ++i)
	{
		if(vec_tab_tmp[i].session==false)
		{
			p_listbox->SetItemTextColour(i,wxColour(255,0,0));
			p_listbox->SetItemFont(i,*wxITALIC_FONT);
		}
	}
}


void HotkeyConfigureDialog::OnButEdit(wxCommandEvent &event)
{
	ShowDialogHK();
}

void HotkeyConfigureDialog::OnTaskDoubleClick(wxListEvent& event)
{
    ShowDialogHK();
}

void HotkeyConfigureDialog::OnButActiv(wxCommandEvent &event)
{
    bool val;
	if(vec_tab_tmp[m_item_selected].session)
	{
		p_listbox->SetItemTextColour(m_item_selected,wxColour(255,0,0));
		p_listbox->SetItemFont(m_item_selected,*wxITALIC_FONT);
		vec_tab_tmp[m_item_selected].actif = false;
		vec_tab_tmp[m_item_selected].session = false;
		p_btnActiv->SetLabel(_("Enable"));

		m_modified = true;
	}
	else
	{
	    val = ::RegisterHotKey((HWND)GetHandle(),0,vec_tab_tmp[m_item_selected].modifier1|vec_tab_tmp[m_item_selected].modifier2,
                            vec_tab_tmp[m_item_selected].virtualKey);

		::UnregisterHotKey((HWND)GetHandle(),0);

	    if(!val)
        {
            wxMessageBox(_("This hotkey is already used by Windows"),_("WinSplit message"),wxOK,this);
            return;
        }

		p_listbox->SetItemTextColour(m_item_selected,wxColour(0,0,0));
		p_listbox->SetItemFont(m_item_selected,*wxNORMAL_FONT);
		vec_tab_tmp[m_item_selected].actif = true;
		vec_tab_tmp[m_item_selected].session = true;
		p_btnActiv->SetLabel(_("Disable"));

		m_modified = true;
	}
}

void HotkeyConfigureDialog::OnTaskChangeSelection(wxListEvent& event)
{
	wxListItem item = event.GetItem();
	m_item_selected = p_listbox->FindItem(-1,item.GetText());

	if(vec_tab_tmp[m_item_selected].session == false)
		p_btnActiv->SetLabel(_("Enable"));
	else
		p_btnActiv->SetLabel(_("Disable"));
	event.Skip();
}

void HotkeyConfigureDialog::ShowDialogHK()
{
	HotkeyStruct st_hk;
	if(vec_tab_tmp[m_item_selected].actif)
	{
	    dlgChooseHK.SetVecHK(vec_tab_tmp);
		dlgChooseHK.SetLabel(p_listbox->GetItemText(m_item_selected));
		HotkeyStruct hk = vec_tab_tmp[m_item_selected];
		dlgChooseHK.SetHotkeys(hk,m_item_selected);
		if(dlgChooseHK.ShowModal() == wxID_OK)
		{
			st_hk = dlgChooseHK.GetHotkeys();
			vec_tab_tmp[m_item_selected].modifier1 = st_hk.modifier1;
			vec_tab_tmp[m_item_selected].modifier2 = st_hk.modifier2;
			vec_tab_tmp[m_item_selected].virtualKey = st_hk.virtualKey;
			p_listbox->SetItem(m_item_selected,1,GetIndexStr(m_item_selected));
			p_listbox->SetItemTextColour(m_item_selected,wxColour(0,0,0));
            p_listbox->SetItemFont(m_item_selected,*wxNORMAL_FONT);
			m_modified = true;
		}
	}
}


vector<HotkeyStruct> HotkeyConfigureDialog::GetVecTmpHK()
{
	return vec_tab_tmp;
}


bool HotkeyConfigureDialog::IsModified()
{
	return m_modified;
}

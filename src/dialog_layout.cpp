
#include <wx/statline.h>
#include <algorithm>

#include "dialog_layout.h"
#include "../images/icone.xpm"

using namespace std;


ChooseRatioDialog::ChooseRatioDialog (wxWindow* parent, wxWindowID id, const wxString& title)
	: wxDialog (parent, id, title),
	  p_txtCtrlX (NULL),
	  p_txtCtrlY (NULL),
	  p_txtCtrlWidth (NULL),
	  p_txtCtrlHeight (NULL),
	  p_btnOk (NULL),
	  p_btnCancel (NULL)
{
	CreateControls();
	Centre();
	SetIcon (wxIcon (icone_xpm) );

	p_btnOk->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (ChooseRatioDialog::CheckValidity), NULL, this);
}


ChooseRatioDialog::~ChooseRatioDialog()
{
}


void ChooseRatioDialog::SetRatio (RatioRect ratio)
{
	p_txtCtrlX->Clear();
	*p_txtCtrlX << ratio.x;

	p_txtCtrlY->Clear();
	*p_txtCtrlY << ratio.y;

	p_txtCtrlWidth->Clear();
	*p_txtCtrlWidth << ratio.width;

	p_txtCtrlHeight->Clear();
	*p_txtCtrlHeight << ratio.height;
}

RatioRect ChooseRatioDialog::GetRatio()
{
	wxString value;
	RatioRect ratio;

	value = p_txtCtrlX->GetValue();
	value.ToDouble (&ratio.x);

	value = p_txtCtrlY->GetValue();
	value.ToDouble (&ratio.y);

	value = p_txtCtrlWidth->GetValue();
	value.ToDouble (&ratio.width);

	value = p_txtCtrlHeight->GetValue();
	value.ToDouble (&ratio.height);

	return ratio;
}


void ChooseRatioDialog::CreateControls()
{
	wxBoxSizer* main_sizer = new wxBoxSizer (wxVERTICAL);
	SetSizer (main_sizer);

	wxGridSizer* grid_sizer = new wxGridSizer (4, 4, 5, 5);

	wxStaticText* stc_x = new wxStaticText (this, wxID_ANY, _ ("x") );
	wxStaticText* stc_y = new wxStaticText (this, wxID_ANY, _ ("y") );
	wxStaticText* stc_width = new wxStaticText (this, wxID_ANY, _ ("width") );
	wxStaticText* stc_height = new wxStaticText (this, wxID_ANY, _ ("height") );

	wxStaticText* percent_1 = new wxStaticText (this, wxID_ANY, _T ("%") );
	wxStaticText* percent_2 = new wxStaticText (this, wxID_ANY, _T ("%") );
	wxStaticText* percent_3 = new wxStaticText (this, wxID_ANY, _T ("%") );
	wxStaticText* percent_4 = new wxStaticText (this, wxID_ANY, _T ("%") );

	p_txtCtrlX = new wxTextCtrl (this, wxID_ANY, _T ("0.00"), wxDefaultPosition, wxSize (40, -1), 0, wxTextValidator (wxFILTER_NUMERIC) );
	p_txtCtrlY = new wxTextCtrl (this, wxID_ANY, _T ("0.00"), wxDefaultPosition, wxSize (40, -1), 0, wxTextValidator (wxFILTER_NUMERIC) );
	p_txtCtrlWidth = new wxTextCtrl (this, wxID_ANY, _T ("0.00"), wxDefaultPosition, wxSize (40, -1), 0, wxTextValidator (wxFILTER_NUMERIC) );
	p_txtCtrlHeight = new wxTextCtrl (this, wxID_ANY, _T ("0.00"), wxDefaultPosition, wxSize (40, -1), 0, wxTextValidator (wxFILTER_NUMERIC) );

	p_txtCtrlX->SetMaxLength (5);
	p_txtCtrlY->SetMaxLength (5);
	p_txtCtrlWidth->SetMaxLength (5);
	p_txtCtrlHeight->SetMaxLength (5);

	wxStdDialogButtonSizer* stdDlgBtn = new wxStdDialogButtonSizer();
	p_btnOk = new wxButton (this, wxID_OK, _ ("&Ok") );
	p_btnCancel = new wxButton (this, wxID_CANCEL, _ ("&Cancel") );

	stdDlgBtn->AddButton (p_btnOk);
	stdDlgBtn->AddButton (p_btnCancel);
	stdDlgBtn->Realize();

	grid_sizer->Add (stc_x, 0, wxALIGN_CENTER | wxALL, 5);
	grid_sizer->AddSpacer (10);
	grid_sizer->Add (p_txtCtrlX, 0, wxALIGN_CENTER | wxALL, 0);
	grid_sizer->Add (percent_1, 0, wxALIGN_CENTER | wxALL, 0);

	grid_sizer->Add (stc_y, 0, wxALIGN_CENTER | wxALL, 5);
	grid_sizer->AddSpacer (10);
	grid_sizer->Add (p_txtCtrlY, 0, wxALIGN_CENTER | wxALL, 0);
	grid_sizer->Add (percent_2, 0, wxALIGN_CENTER | wxALL, 0);

	grid_sizer->Add (stc_width, 0, wxALIGN_CENTER | wxALL, 5);
	grid_sizer->AddSpacer (10);
	grid_sizer->Add (p_txtCtrlWidth, 0, wxALIGN_CENTER | wxALL, 0);
	grid_sizer->Add (percent_3, 0, wxALIGN_CENTER | wxALL, 0);

	grid_sizer->Add (stc_height, 0, wxALIGN_CENTER | wxALL, 5);
	grid_sizer->AddSpacer (10);
	grid_sizer->Add (p_txtCtrlHeight, 0, wxALIGN_CENTER | wxALL, 0);
	grid_sizer->Add (percent_4, 0, wxALIGN_CENTER | wxALL, 0);

	main_sizer->Add (grid_sizer, 0, wxALIGN_CENTRE | wxALL, 5);
	main_sizer->Add (new wxStaticLine (this, wxID_ANY, wxDefaultPosition, wxSize (10, 1) ), 1, wxALIGN_CENTRE | wxALL | wxEXPAND, 5);
	main_sizer->Add (stdDlgBtn, 0, wxALIGN_CENTRE | wxALL, 5);

	main_sizer->Fit (this);

	p_txtCtrlX->SetFocus();
}


bool ChooseRatioDialog::IsValide()
{
	RatioRect ratio = GetRatio();

	if (ratio.x < 0. || ratio.x > 100.)
		return false;
	if (ratio.y < 0. || ratio.y > 100.)
		return false;
	if (ratio.width < 0. || ratio.width > 100.)
		return false;
	if (ratio.height < 0. || ratio.height > 100.)
		return false;

	return true;
}


bool ChooseRatioDialog::IsInside()
{
	RatioRect ratio = GetRatio();

	if ( (ratio.x + ratio.width) > 100.)
		return false;

	if ( (ratio.y + ratio.height) > 100.)
		return false;

	return true;
}


void ChooseRatioDialog::CheckValidity (wxCommandEvent& event)
{
	if (!IsValide() )
	{
		wxMessageBox (_ ("Invalide entry, value must be between 0. and 100."), _ ("Error"), wxICON_ERROR);
		return;
	}

	if (!IsInside() )
	{
		if (wxMessageBox (_ ("This settings define a window outside the screen!\n Continue anyway?"), _ ("Warning"), wxYES_NO | wxICON_EXCLAMATION) == wxYES)
			EndModal (wxOK);

		return;
	}

	RatioRect ratio = GetRatio();

	if ( (ratio.width <= 15.) || (ratio.height <= 15.) )
	{
		if (wxMessageBox (_ ("This settings define a very small window screen!\n Continue anyway?"), _ ("Warning"), wxYES_NO | wxICON_EXCLAMATION) == wxYES)
			EndModal (wxOK);

		return;
	}

	else
	{
		EndModal (wxOK);
	}
}

//===============
// LayoutDialog
//===============

LayoutDialog::LayoutDialog (wxWindow* parent, wxWindowID id, const wxString& title)
	: wxDialog (parent, id, title),
	  tab_seq_tmp(),
	  p_lstCtrlTask (NULL),
	  p_lstCtrlCombo (NULL),
	  p_btnAdd (NULL),
	  p_btnEdit (NULL),
	  p_btnDelete (NULL),
	  p_btnOk (NULL),
	  p_btnCancel (NULL),
	  p_stc_x (NULL),
	  p_stc_y (NULL),
	  p_stc_width (NULL),
	  p_stc_height (NULL),
	  p_spinBtnUpDown (NULL),
	  p_panel_screen (NULL),
	  p_panel_wnd (NULL),
	  p_stcBmpWnd (NULL),
	  p_stcBmpScreen (NULL),
	  p_preview (NULL),
	  m_item_task_selected (-1),
	  m_item_combo_selected (-1)
{
	tab_seq_tmp.resize (9);
	LayoutManager::GetInstance()->CopyTable (tab_seq_tmp);

	CreateControls();
	CreateConnections();

	InitListCtrl();

	GetSizer()->Fit (this);
	p_lstCtrlCombo->SetFocus();
	Center();
	SetIcon (wxIcon (icone_xpm) );
}


LayoutDialog::~LayoutDialog()
{
}


void LayoutDialog::CreateControls()
{
	wxBoxSizer* main_sizer = new wxBoxSizer (wxVERTICAL);

	wxBoxSizer* hsizer = new wxBoxSizer (wxHORIZONTAL);
	wxBoxSizer* hsizer_panel = new wxBoxSizer (wxHORIZONTAL);
	wxBoxSizer* vsizer = new wxBoxSizer (wxVERTICAL);
	wxBoxSizer* vsizer_panel = new wxBoxSizer (wxVERTICAL);

	p_lstCtrlTask = new wxListCtrl (this, wxID_ANY, wxDefaultPosition, wxSize (130, 150), wxLC_REPORT | wxSTATIC_BORDER | wxLC_SINGLE_SEL | wxLC_VRULES | wxLC_HRULES);
	p_lstCtrlCombo = new wxListCtrl (this, wxID_ANY, wxDefaultPosition, wxSize (286, 150), wxLC_REPORT | wxSTATIC_BORDER | wxLC_SINGLE_SEL | wxLC_VRULES | wxLC_HRULES);

	p_btnAdd = new  wxButton (this, wxID_ANY, _ ("&Add") );
	p_btnEdit = new  wxButton (this, wxID_ANY, _ ("&Edit") );
	p_btnDelete = new  wxButton (this, wxID_ANY, _ ("&Delete") );

	p_spinBtnUpDown = new wxSpinButton (this, wxID_ANY);

	vsizer->Add (p_btnAdd, 0, wxALIGN_CENTRE | wxALL, 5);
	vsizer->AddSpacer (10);
	vsizer->Add (p_btnEdit, 0, wxALIGN_CENTRE | wxALL, 5);
	vsizer->AddSpacer (10);
	vsizer->Add (p_btnDelete, 0, wxALIGN_CENTRE | wxALL, 5);

	hsizer->Add (p_lstCtrlTask, 0, wxALIGN_CENTRE | wxALL, 5);
	hsizer->Add (p_lstCtrlCombo, 0, wxALIGN_CENTRE | wxALL, 5);
	hsizer->Add (p_spinBtnUpDown, 0, wxALIGN_CENTRE | wxALL, 5);
	hsizer->Add (vsizer, 0, wxALIGN_CENTRE | wxALL, 5);

	p_preview = new LMPreview (this);

	p_stc_x = new wxStaticText (this, wxID_ANY, _ ("x:") );
	p_stc_y = new wxStaticText (this, wxID_ANY, _ ("y:") );
	p_stc_width = new wxStaticText (this, wxID_ANY, _ ("width:") );
	p_stc_height = new wxStaticText (this, wxID_ANY, _ ("height:") );

	vsizer_panel->Add (p_stc_x, 0, wxALIGN_LEFT | wxALL, 10);
	vsizer_panel->Add (p_stc_y, 0, wxALIGN_LEFT | wxALL, 10);
	vsizer_panel->Add (p_stc_width, 0, wxALIGN_LEFT | wxALL, 10);
	vsizer_panel->Add (p_stc_height, 0, wxALIGN_LEFT | wxALL, 10);

	wxImage im_schema;
	im_schema.LoadFile (_T ("..\\images\\ecran_winsplit.png"), wxBITMAP_TYPE_PNG);

	hsizer_panel->Add (new wxStaticBitmap (this, wxID_ANY, wxBitmap (im_schema) ), 0, wxALIGN_CENTRE | wxALL, 5);
	hsizer_panel->AddSpacer (10);
	hsizer_panel->Add (p_preview, 0, wxALIGN_CENTRE | wxALL, 5);
	hsizer_panel->Add (vsizer_panel, 0, wxALIGN_CENTRE | wxALL, 5);

	wxStdDialogButtonSizer* stdDlgBtn = new wxStdDialogButtonSizer();
	p_btnOk = new wxButton (this, wxID_OK, _ ("&Ok") );
	p_btnCancel = new wxButton (this, wxID_CANCEL, _ ("&Cancel") );

	stdDlgBtn->AddButton (p_btnOk);
	stdDlgBtn->AddButton (p_btnCancel);
	stdDlgBtn->Realize();

	main_sizer->Add (hsizer, 0, wxALIGN_CENTRE | wxALL, 5);
	main_sizer->Add (new wxStaticLine (this, wxID_ANY, wxDefaultPosition, wxSize (10, 1) ), 1, wxALIGN_CENTRE | wxALL | wxEXPAND, 5);
	main_sizer->Add (new wxStaticText (this, wxID_ANY, _ ("Preview") ), 0, wxALIGN_CENTRE | wxALL, 5);
	main_sizer->Add (hsizer_panel, 0, wxALIGN_CENTRE | wxALL, 5);
	main_sizer->Add (new wxStaticLine (this, wxID_ANY, wxDefaultPosition, wxSize (10, 1) ), 1, wxALIGN_CENTRE | wxALL | wxEXPAND, 5);
	main_sizer->Add (stdDlgBtn, 0, wxALIGN_CENTRE | wxALL, 5);

	SetSizer (main_sizer);
	main_sizer->SetSizeHints (this);
}


void LayoutDialog::CreateConnections()
{
	p_lstCtrlTask->Connect (wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler (LayoutDialog::OnTaskChangeSelection), NULL, this);
	p_lstCtrlCombo->Connect (wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler (LayoutDialog::OnComboChangeSelection), NULL, this);
	p_lstCtrlCombo->Connect (wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler (LayoutDialog::OnActiveCombo), NULL, this);
	p_btnAdd->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (LayoutDialog::OnAddCombo), NULL, this);
	p_btnEdit->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (LayoutDialog::OnEditCombo), NULL, this);
	p_btnDelete->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (LayoutDialog::OnDeleteCombo), NULL, this);
	p_spinBtnUpDown->Connect (wxEVT_SCROLL_LINEDOWN, wxSpinEventHandler (LayoutDialog::OnSpinDown), NULL, this);
	p_spinBtnUpDown->Connect (wxEVT_SCROLL_LINEUP, wxSpinEventHandler (LayoutDialog::OnSpinUp), NULL, this);
	p_btnOk->Connect (wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (LayoutDialog::OnOKBtn), NULL, this);
}

void LayoutDialog::InitListCtrl()
{
	wxString str_in;
	p_lstCtrlTask->InsertColumn (0, _ ("Action"), wxLIST_FORMAT_LEFT, 124);
	p_lstCtrlCombo->InsertColumn (0, _ ("Sequence in per cent"), wxLIST_FORMAT_LEFT, 280);

	str_in << _ ("Down") << _T (" ") << _ ("Left");
	p_lstCtrlTask->InsertItem (0, str_in);

	p_lstCtrlTask->InsertItem (1, _ ("Down") );

	str_in.Clear();
	str_in << _ ("Down") << _T (" ") << _ ("Right");
	p_lstCtrlTask->InsertItem (2, str_in);

	p_lstCtrlTask->InsertItem (3, _ ("Left") );

	p_lstCtrlTask->InsertItem (4, _ ("Middle") );

	p_lstCtrlTask->InsertItem (5, _ ("Right") );

	str_in.Clear();
	str_in << _ ("Up") << _T (" ") << _ ("Left");
	p_lstCtrlTask->InsertItem (6, str_in);

	p_lstCtrlTask->InsertItem (7, _ ("Up") );

	str_in.Clear();
	str_in << _ ("Up") << _T (" ") << _ ("Right");
	p_lstCtrlTask->InsertItem (8, str_in);

	m_item_task_selected = 0;
	p_lstCtrlTask->SetItemState (m_item_task_selected, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	RefreshLstCtrlCombo();
}


void LayoutDialog::OnTaskChangeSelection (wxListEvent& event)
{
	wxListItem item = event.GetItem();
	m_item_task_selected = p_lstCtrlTask->FindItem (-1, item.GetText() );

	RefreshLstCtrlCombo();
}


void LayoutDialog::OnComboChangeSelection (wxListEvent& event)
{
	wxListItem item = event.GetItem();
	m_item_combo_selected = p_lstCtrlCombo->FindItem (-1, item.GetText() );

	wxString value;

	value << _T ("x: ") << wxString::Format (_T ("%.2f"), tab_seq_tmp[m_item_task_selected][m_item_combo_selected].x) << _T ("%");
	p_stc_x->SetLabel (value);

	value.Clear();
	value << _T ("y: ") << wxString::Format (_T ("%.2f"), tab_seq_tmp[m_item_task_selected][m_item_combo_selected].y) << _T ("%");
	p_stc_y->SetLabel (value);

	value.Clear();
	value << _ ("width") << _T (": ") << wxString::Format (_T ("%.2f"), tab_seq_tmp[m_item_task_selected][m_item_combo_selected].width) << _T ("%");
	p_stc_width->SetLabel (value);

	value.Clear();
	value << _ ("height") << _T (": ") << wxString::Format (_T ("%.2f"), tab_seq_tmp[m_item_task_selected][m_item_combo_selected].height) << _T ("%");
	p_stc_height->SetLabel (value);

	RatioRect ratio = tab_seq_tmp[m_item_task_selected][m_item_combo_selected];

	p_preview->SetWndPos (ratio.x, ratio.y, ratio.width, ratio.height);
}


void LayoutDialog::RefreshLstCtrlCombo (bool select_first)
{
	vector<RatioRect>::iterator it;

	it = tab_seq_tmp[m_item_task_selected].begin();

	p_lstCtrlCombo->DeleteAllItems();

	wxString str_in;
	int count = 0;

	while (it != tab_seq_tmp[m_item_task_selected].end() )
	{
		str_in.Clear();

		str_in << wxString::Format (_T ("%d"), count + 1) << _T ("|  ");
		str_in << _T ("x:") << wxString::Format (_T ("%6.2f"), it->x) << _T ("   ");
		str_in << _T ("y:") << wxString::Format (_T ("%6.2f"), it->y) << _T ("   ");
		str_in << _ ("width") << _T (": ") << wxString::Format (_T ("%6.2f"), it->width) << _T ("   ");
		str_in << _ ("height") << _T (": ") << wxString::Format (_T ("%6.2f"), it->height);

		p_lstCtrlCombo->InsertItem (count, str_in);

		++it;
		++count;
	}

	if (tab_seq_tmp[m_item_task_selected].size() == 0)
	{
		m_item_combo_selected = -1;
		return;
	}

	if (select_first)
	{
		m_item_combo_selected = 0;
		p_lstCtrlCombo->SetItemState (m_item_combo_selected, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	}
}


void LayoutDialog::OnActiveCombo (wxListEvent& event)
{
	if (m_item_combo_selected == -1)
		return;

	ChooseRatioDialog* choose_ratio = new ChooseRatioDialog();

	choose_ratio->SetRatio (tab_seq_tmp[m_item_task_selected][m_item_combo_selected]);
	if (choose_ratio->ShowModal() == wxOK)
	{
		tab_seq_tmp[m_item_task_selected][m_item_combo_selected] = choose_ratio->GetRatio();;
		RefreshLstCtrlCombo();
	}

	choose_ratio->Destroy();

	event.Skip();
}

void LayoutDialog::OnAddCombo (wxCommandEvent& event)
{
	ChooseRatioDialog* choose_ratio = new ChooseRatioDialog();

	RatioRect ratio;

	ratio.x = ratio.y = ratio.width = ratio.height = 0.;

	choose_ratio->SetRatio (ratio);
	if (choose_ratio->ShowModal() == wxOK)
	{
		ratio = choose_ratio->GetRatio();
		tab_seq_tmp[m_item_task_selected].push_back (ratio);
		RefreshLstCtrlCombo();
	}

	choose_ratio->Destroy();

	event.Skip();
}


void LayoutDialog::OnEditCombo (wxCommandEvent& event)
{
	if (m_item_combo_selected == -1)
		return;

	ChooseRatioDialog* choose_ratio = new ChooseRatioDialog();

	choose_ratio->SetRatio (tab_seq_tmp[m_item_task_selected][m_item_combo_selected]);
	if (choose_ratio->ShowModal() == wxOK)
	{
		tab_seq_tmp[m_item_task_selected][m_item_combo_selected] = choose_ratio->GetRatio();;
		RefreshLstCtrlCombo();
	}

	choose_ratio->Destroy();

	event.Skip();
}


void LayoutDialog::OnDeleteCombo (wxCommandEvent& event)
{
	if (m_item_combo_selected == -1)
		return;

	if (int (tab_seq_tmp[m_item_task_selected].size() ) == 1)
	{
		wxMessageBox (_ ("One task must have at least one possibility") );
		return;
	}

	if (wxMessageBox (_ ("Are you sure you want delete this entry?"), _ ("Warning"), wxYES | wxNO) == wxYES)
	{
		tab_seq_tmp[m_item_task_selected].erase (tab_seq_tmp[m_item_task_selected].begin() + m_item_combo_selected);
		RefreshLstCtrlCombo();
	}

	event.Skip();
}


void LayoutDialog::OnSpinUp (wxSpinEvent& event)
{
	if (m_item_combo_selected == -1)
		return;

	if (m_item_combo_selected == 0)
		return;

	if (tab_seq_tmp[m_item_task_selected].size() > 1)
	{
		swap (tab_seq_tmp[m_item_task_selected][m_item_combo_selected], tab_seq_tmp[m_item_task_selected][m_item_combo_selected - 1]);
		RefreshLstCtrlCombo (false);

		--m_item_combo_selected;
		p_lstCtrlCombo->SetItemState (m_item_combo_selected, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	}
	event.Skip();
}

void LayoutDialog::OnSpinDown (wxSpinEvent& event)
{
	if (m_item_combo_selected == -1)
		return;

	if (m_item_combo_selected == (int (tab_seq_tmp[m_item_task_selected].size() ) - 1) )
		return;

	if (tab_seq_tmp[m_item_task_selected].size() > 1)
	{
		swap (tab_seq_tmp[m_item_task_selected][m_item_combo_selected], tab_seq_tmp[m_item_task_selected][m_item_combo_selected + 1]);
		RefreshLstCtrlCombo (false);

		++m_item_combo_selected;
		p_lstCtrlCombo->SetItemState (m_item_combo_selected, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	}
	event.Skip();
}

void LayoutDialog::OnOKBtn (wxCommandEvent& event)
{
	LayoutManager::GetInstance()->SetTable (tab_seq_tmp);
	LayoutManager::GetInstance()->SaveData();
	EndModal (wxOK);
}


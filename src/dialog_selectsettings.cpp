#include "dialog_selectsettings.h"
#include <wx/statline.h>

SelectSettingsDialog::SelectSettingsDialog(wxWindow* parent, int type) : wxDialog(parent,-1,_("Options"),wxDefaultPosition),
    m_options(SettingsManager::Get())
{
	m_iDialogType=type;
    CreateControls();
    ConnectControls();
    CenterOnParent();
}

SelectSettingsDialog::~SelectSettingsDialog()
{
    //dtor
}

void SelectSettingsDialog::CreateControls()
{
	wxString sMsg;
	switch(m_iDialogType)
	{
		case SSDT_EXPORT:
			sMsg=_("Select items to export :");
			break;
		case SSDT_IMPORT:
			sMsg=_("Select items to import :");
			break;
		case SSDT_DELETE:
			sMsg=_("Select items to delete :");
			break;
	}
    wxBoxSizer *mainsizer=new wxBoxSizer(wxVERTICAL);
        wxStaticBoxSizer *box=new wxStaticBoxSizer(wxHORIZONTAL, this, sMsg);
            p_lstChoices=new wxCheckListBox(this, -1 );
            box->Add(p_lstChoices, 1, wxALL|wxEXPAND, 5);
            wxBoxSizer *btnCol=new wxBoxSizer(wxVERTICAL);
                p_btnSelectAll=new wxButton(this, -1, _("Select all"));
                btnCol->Add(p_btnSelectAll, 0, wxALL|wxEXPAND, 0);
                p_btnUnselectAll=new wxButton(this, -1, _("Unselect all"));
                btnCol->Add(p_btnUnselectAll, 0, wxTOP|wxBOTTOM|wxEXPAND, 5);
                p_btnInvert=new wxButton(this, -1, _("Invert"));
                btnCol->Add(p_btnInvert, 0, wxALL|wxEXPAND, 0);
            box->Add(btnCol, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5);
        mainsizer->Add(box, 1, wxALL|wxEXPAND, 5);

        mainsizer->Add(new wxStaticLine(this, -1), 0, wxALL|wxEXPAND, 5);

        wxBoxSizer *btnLine2=new wxBoxSizer(wxHORIZONTAL);
            p_btnOk=new wxButton(this, wxID_OK, _("Ok"));
            btnLine2->Add(p_btnOk, 0,wxRIGHT, 5);
            p_btnCancel=new wxButton(this, wxID_CANCEL, _("Cancel"));
            btnLine2->Add(p_btnCancel, 0, wxALL, 0);
        mainsizer->Add(btnLine2, 0, wxALL|wxALIGN_RIGHT, 5);
    SetSizer(mainsizer);

    p_lstChoices->Append(_("General settings"));
    p_lstChoices->Append(_("Hotkeys settings"));
    p_lstChoices->Append(_("Layout settings"));
    p_lstChoices->Append(_("Registered placements"));
    // Par defaut, on selectionne tout
    SetSelectionFlags(SST_ALL);

    mainsizer->SetSizeHints(this);
}

void SelectSettingsDialog::ConnectControls()
{
    p_btnSelectAll->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SelectSettingsDialog::OnChangeSelection), NULL, this);
    p_btnUnselectAll->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SelectSettingsDialog::OnChangeSelection), NULL, this);
    p_btnInvert->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SelectSettingsDialog::OnChangeSelection), NULL, this);
    p_lstChoices->Connect(wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler(SelectSettingsDialog::OnChangeSelection), NULL, this);
}

void SelectSettingsDialog::OnChangeSelection(wxCommandEvent &event)
{
    size_t i;
    if (event.GetId()==p_btnSelectAll->GetId())
    {
        for (i=0;i<p_lstChoices->GetCount(); i++)
            p_lstChoices->Check(i, true);
    }
    if (event.GetId()==p_btnUnselectAll->GetId())
    {
        for (i=0;i<p_lstChoices->GetCount(); i++)
            p_lstChoices->Check(i, false);
    }
    if (event.GetId()==p_btnInvert->GetId())
    {
        for (i=0;i<p_lstChoices->GetCount(); i++)
            p_lstChoices->Check(i, !p_lstChoices->IsChecked(i));
    }
    bool bEnable=false;
    for (size_t i=0;i<p_lstChoices->GetCount(); i++)
        bEnable|=p_lstChoices->IsChecked(i);
    p_btnOk->Enable(bEnable);
}

void SelectSettingsDialog::SetSelectionFlags(int flags)
{
	if (p_lstChoices->GetCount()!=4) return;

	p_lstChoices->Check(0,((flags & SST_GENERAL)==SST_GENERAL));
	p_lstChoices->Check(1,((flags & SST_HOTKEYS)==SST_HOTKEYS));
	p_lstChoices->Check(2,((flags & SST_LAYOUT)==SST_LAYOUT));
	p_lstChoices->Check(3,((flags & SST_PLACES)==SST_PLACES));
	wxCommandEvent evt(wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, p_lstChoices->GetId());
	OnChangeSelection(evt);
}

int SelectSettingsDialog::GetSelectionFlags() const
{
    int iRes=0;
    iRes |= (p_lstChoices->IsChecked(0)? SST_GENERAL:0);
    iRes |= (p_lstChoices->IsChecked(1)? SST_HOTKEYS:0);
    iRes |= (p_lstChoices->IsChecked(2)? SST_LAYOUT:0);
    iRes |= (p_lstChoices->IsChecked(3)? SST_PLACES:0);
    return iRes;
}

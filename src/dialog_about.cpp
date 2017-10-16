
#include <wx/settings.h>

#include "dialog_about.h"
#include "main.h"

#include "../images/builtwithwx.xpm"
#include "../images/icone.xpm"
//#include "../images/creative_commons.xpm"
#include "../images/gplv3.xpm"

BEGIN_EVENT_TABLE(AboutDialog, wxDialog)
    EVT_BUTTON(ID_BITMAPWX,AboutDialog::OnImagewxClick)
    EVT_BUTTON(ID_BITMAPCC,AboutDialog::OnImageccClick)
    EVT_TEXT_URL(ID_TEXTABOUT, AboutDialog::OnURLClick)
END_EVENT_TABLE()


AboutDialog::AboutDialog(wxWindow* parent,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style)
:wxDialog(parent,id,title,pos,size,style)
{
    wxString dlg_title(_("About WinSplit Revolution"));

    dlg_title += _T(" v") + wxGetApp().GetVersion();

    SetTitle(dlg_title);

    SetIcon(wxIcon(icone_xpm));
    CreateControls();

    Centre();
}


AboutDialog::~AboutDialog()
{
}


void AboutDialog::CreateControls()
{
    wxBoxSizer* pmainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* psizer_h = new wxBoxSizer(wxHORIZONTAL);

    SetSizer(pmainSizer);

    wxTextCtrl* ptxtAbout = new wxTextCtrl(this,ID_TEXTABOUT,wxEmptyString,wxDefaultPosition,wxSize(370,185),
                            wxTE_READONLY|wxTE_AUTO_URL|wxTE_MULTILINE|wxTE_CENTRE|
                            wxSTATIC_BORDER|wxTE_RICH2);

    wxTextAttr attr_tmp=ptxtAbout->GetDefaultStyle();
    ptxtAbout->SetDefaultStyle(wxTextAttr(*wxBLACK,wxNullColour,wxFont(10,wxDEFAULT,wxFONTSTYLE_NORMAL,wxBOLD)));
    *ptxtAbout << _T("WINSPLIT REVOLUTION")<< _T("\n");;
    ptxtAbout->SetDefaultStyle(attr_tmp);
	*ptxtAbout << _T("Copyright (C) 2005-2009") << _T("\n");
    *ptxtAbout << _("Created by Raphael Lencrerot") << _T("\n\n");

    *ptxtAbout << _T("Reloaded fork website https://github.com/pupitetris/winsplit-reloaded") << _T("\n\n");
	*ptxtAbout << _("C++ based software developed thanks to the following:") << _T("\n");
	*ptxtAbout << _("wxWidgets GUI Library and Code::Blocks (C++ IDE).") << _T("\n\n");

	*ptxtAbout << _("This program comes with ABSOLUTELY NO WARRANTY. "
                    "This is free software, and you are welcome to redistribute it under certain conditions. "
	                "See GNU GPL v3 license for more details") << _T("\n\n");

	*ptxtAbout << _("WinSplit Revolution Team:") << _T("\n");
	*ptxtAbout << _T("Raphael Lencrerot (developper)") << _T("\n");
	*ptxtAbout << _T("Xavier Perrissoud (developper)") << _T("\n");

	*ptxtAbout << _T("Special thanks to NX (icons design)");
	ptxtAbout->SetInsertionPoint(0);
    /*ptxtAbout->SetDefaultStyle(wxTextAttr(*wxRED));
    *ptxtAbout << _("Some Rights Reserved");
    ptxtAbout->SetInsertionPoint(0);*/

    wxBitmap bitmapwx(builtwithwx_xpm);
    wxBitmap bitmapcc(gplv3_xpm);

    wxBitmapButton* pbmpBtnWX = new wxBitmapButton(this,ID_BITMAPWX,bitmapwx,wxDefaultPosition,wxDefaultSize);
    wxBitmapButton* pbmpBtnCC = new wxBitmapButton(this,ID_BITMAPCC,bitmapcc,wxDefaultPosition,wxDefaultSize);

    wxStdDialogButtonSizer* pdlgBtnSizer = new wxStdDialogButtonSizer;
    wxButton* pbtnOk = new wxButton(this,wxID_OK,_("&Ok"));

    pdlgBtnSizer->AddButton(pbtnOk);
    pdlgBtnSizer->Realize();

    wxStaticLine* stcLine = new wxStaticLine(this,ID_STATIC,wxDefaultPosition,wxSize(150,-1),wxLI_HORIZONTAL);

    pmainSizer->Add(ptxtAbout,0,wxALL|wxALIGN_CENTER,10);

    psizer_h->Add(pbmpBtnWX,0,wxALL|wxALIGN_CENTER,5);
    psizer_h->Add(pbmpBtnCC,0,wxALL|wxALIGN_CENTER,5);

    pmainSizer->Add(psizer_h,0,wxALL|wxALIGN_CENTER,5);

    pmainSizer->Add(stcLine ,0,wxALL|wxALIGN_CENTER|wxGROW,5);
    pmainSizer->Add(pdlgBtnSizer ,0,wxALL|wxALIGN_CENTER,5);

    pbtnOk->SetFocus();

    pmainSizer->Fit(this);
    pmainSizer->SetSizeHints(this);
}


void AboutDialog::OnImagewxClick(wxCommandEvent& event)
{
   wxLaunchDefaultBrowser(_T("http://www.wxwidgets.org"));
}


void AboutDialog::OnImageccClick(wxCommandEvent& event)
{
   wxLaunchDefaultBrowser(_T("http://www.gnu.org/licenses/gpl.html"));
}


void AboutDialog::OnURLClick(wxTextUrlEvent& event)
{
    wxMouseEvent mouse_event = event.GetMouseEvent();

    if (mouse_event.Button(wxMOUSE_BTN_LEFT))
        wxLaunchDefaultBrowser(_T("https://github.com/pupitetris/winsplit-reloaded"));

}

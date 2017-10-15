
#include <time.h>
#include <wx/textfile.h>

#include "dialog_warnhotkeys.h"
#include "settingsmanager.h"

#include "../images/alert.xpm"


WarnHotkeyDialog::WarnHotkeyDialog(wxWindow* parent,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size)
:wxDialog(parent,id,title,pos,size),
p_btnOk(NULL),
p_checkDoNotShowAgain(NULL),
p_dlgBtn(NULL)
{
    SetIcon(wxIcon(alert_xpm));

    wxBoxSizer* p_mainSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(p_mainSizer);

    wxBoxSizer* p_sizer2 = new wxBoxSizer(wxHORIZONTAL);

    p_label = new wxStaticText(this,0,wxEmptyString);
    p_dlgBtn = new wxStdDialogButtonSizer();
    p_btnOk = new wxButton(this,wxID_OK,_("Ok"));
    p_checkDoNotShowAgain = new wxCheckBox(this,ID_CHECKBOX,_("Do not display this message anymore"));

    p_dlgBtn->AddButton(p_btnOk);
    p_dlgBtn->Realize();

    p_sizer2->Add(p_checkDoNotShowAgain,0,wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL,5);
    p_sizer2->Add(p_dlgBtn,0,wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL,5);

    p_mainSizer->Add(p_label,0,wxALIGN_CENTER|wxALL,5);
    p_mainSizer->Add(p_sizer2,0,wxALIGN_LEFT|wxALL,5);

    GetSizer()->SetSizeHints(this);
    Centre();
}

void WarnHotkeyDialog::AppendText(const wxString& text)
{
    p_label->SetLabel(text);
    GetSizer()->SetSizeHints(this);
    Centre();
}

bool WarnHotkeyDialog::IsCheck()
{
    return p_checkDoNotShowAgain->GetValue();
}

void WarnHotkeyDialog::WriteErrorLog(const wxString& text)
{
    struct tm* p_time;
    time_t deftime;
    deftime = time(NULL);
    p_time = localtime(&deftime);

    wxString message;

    wxString sPath = SettingsManager::Get().GetDataDirectory() + _T("Error_Hotkeys.txt");
    wxTextFile f_out(sPath);

    message<<_("Hotkeys has been desactivated, date: ")<<wxString::Format(_T("%d"),p_time->tm_mday);
    message<<_T("/")<<wxString::Format(_T("%d"),p_time->tm_mon+1)<<_T("/")<<wxString::Format(_T("%d"),p_time->tm_year+1900);
    message<<_T("  ")<<wxString::Format(_T("%dH%dM%dS"),p_time->tm_hour,p_time->tm_min,p_time->tm_sec);

    f_out.AddLine(message);
    f_out.Close();
}

// dialog_warnhotkeys.h
#ifndef __WARNHOTKEYFRAME_H__
#define __WARNHOTKEYFRAME_H__

enum IdentifiersWarn
{
	ID_CHECKBOX = 2200,
};

class WarnHotkeyDialog : public wxDialog
{
private:
	wxStaticText* p_label;
	wxButton* p_btnOk;
	wxCheckBox* p_checkDoNotShowAgain;
	wxStdDialogButtonSizer* p_dlgBtn;

public:
	WarnHotkeyDialog (wxWindow* parent = NULL,
	                  wxWindowID id = wxID_ANY,
	                  const wxString& title = wxEmptyString,
	                  const wxPoint& pos = wxDefaultPosition,
	                  const wxSize& size = wxDefaultSize);

	~WarnHotkeyDialog() {};

	void AppendText (const wxString& text);
	bool IsCheck();
	void WriteErrorLog (const wxString& text);
};

#endif //__WARNHOTKEYFRAME_H__

// Fichier wxabout.h
#ifndef __WXABOUT__
#define __WXABOUT__

#include <wx/wxprec.h>
#include <wx/statline.h>
#include <wx/hyperlink.h>

enum IdentAbout
{
	ID_BITMAPCC = 20000,
	ID_BITMAPWX,
	ID_BITMAPCB,
	ID_TEXTABOUT,
	ID_STATIC,
	ID_STATICURL
};


class AboutDialog : public wxDialog
{
private:

	void CreateControls();
	void OnImagewxClick (wxCommandEvent& event);
	void OnImageccClick (wxCommandEvent& event);
	void OnURLClick (wxTextUrlEvent& event);

public:
	AboutDialog (wxWindow* parent = NULL,
	             wxWindowID id = wxID_ANY,
	             const wxString& title = wxEmptyString,
	             const wxPoint& pos = wxDefaultPosition,
	             const wxSize& size = wxDefaultSize,
	             long style = wxDEFAULT_DIALOG_STYLE);

	~AboutDialog();

	DECLARE_EVENT_TABLE()
};

#endif //__WXABOUT__

#ifndef SELECTSETTINGSDIALOG_H_INCLUDED
#define SELECTSETTINGSDIALOG_H_INCLUDED

#include "settingsmanager.h"

// Flags pour les valeurs selectionnees
#define SST_NONE	0
#define SST_GENERAL	1
#define SST_HOTKEYS	2
#define SST_LAYOUT	4
#define SST_PLACES	8
#define SST_ALL 	15
// Differents types pour la boite de dialogue
#define SSDT_EXPORT	1
#define SSDT_IMPORT	2
#define SSDT_DELETE	3

class SelectSettingsDialog : public wxDialog
{
public:
	SelectSettingsDialog (wxWindow* parent, int type);
	virtual ~SelectSettingsDialog();
	void SetSelectionFlags (int flags = SST_ALL);
	int GetSelectionFlags() const;
protected:
private:
	int m_iDialogType;
	void CreateControls();
	void ConnectControls();
	SettingsManager &m_options;
	wxCheckListBox *p_lstChoices;
	wxButton *p_btnSelectAll, *p_btnUnselectAll, *p_btnInvert;
	wxButton *p_btnOk, *p_btnCancel;
	void OnChangeSelection (wxCommandEvent &event);
};

#endif // SELECTSETTINGSDIALOG_H_INCLUDED

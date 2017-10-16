#ifndef OPTIONSDIALOG_H_INCLUDED
#define OPTIONSDIALOG_H_INCLUDED

#include <wx/dialog.h>
#include <wx/notebook.h>

#include "settingsmanager.h"
#include "auto_placement.h"
#include "update_thread.h"

class VirtualNumpad;

class OptionsDialog : public wxDialog
{
public:
	OptionsDialog (wxWindow *parent = NULL);
	virtual ~OptionsDialog();
	void SetVirtualNumPad (VirtualNumpad *numpad);
protected:
private:
	unsigned int m_timerCount;
	SettingsManager &m_options;
	VirtualNumpad *p_virtNumpad;
	AutoPlacementManager m_autoPlacement;
	wxNotebook *p_nBook;
	// Contrôles de l'onglet "Général"
	wxComboBox *p_cmbLanguage, *p_cmbAutoPlacement;
	wxCheckBox *p_checkTmw;
	wxButton *p_btnDeleteAuto;
	wxCheckBox *p_checkDelTmpFiles;
	wxComboBox *p_cmbDeleteTime;
	wxButton *p_btnOpenFolder;
	wxButton *p_btnExportOpt, *p_btnImportOpt;
	// Contrôles de l'onglet "Virtual numpad"
	wxCheckBox *p_checkSaveVirtNumpad, *p_checkShowVirtStart, *p_checkAutoHide;
	wxSlider *p_sliderNumpadTransparency;
	// Contrôles de l'onglet "Web update"
	wxCheckBox *p_checkUpdate;
	wxStaticText *p_sttFrequency;
	wxComboBox *p_cmbUpdate;
	wxButton *p_btnCheckNow;
	wxStaticText* p_stcTxtStatus;
	// Contrôles de l'onglet "Drag'N'Go
	wxCheckBox *p_checkEnableDnG;
	wxStaticText *p_sttRadius;
	wxTextCtrl *p_txtDnGRadius;
	wxPanel *p_pnlZoneBgColor;
	wxPanel *p_pnlZoneFgColor;
	wxSlider *p_sliderZoneTransparency;
	wxComboBox *p_cmbModifier1;
	wxComboBox *p_cmbModifier2;
	// Contrôles de l'onglet "Divers"
	wxCheckBox *p_checkEnableXMouse;
	wxCheckBox *p_checkEnableZOrder;
	wxStaticText *p_sttZOrderDelay;
	wxTextCtrl *p_txtZOrderDelay;
	wxCheckBox *p_checkFollowWnd;
	wxCheckBox *p_checkFollowOnlyIn;
	wxCheckBox *p_checkMinMaxCycle;
	// Boutons de commandes du dialogue
	wxButton *p_btnOk, *p_btnCancel, *p_btnApply;

	wxTimer m_timer;
	ReadVersionThread* p_updateThread;
	// Méthodes pour la création des différentes pages du wxNotebook
	void CreateAndFillPage_General (wxNotebook *book);
	void CreateAndFillPage_Numpad (wxNotebook *book);
	void CreateAndFillPage_WebUpdate (wxNotebook *book);
	void CreateAndFillPage_DragNGo (wxNotebook *book);
	void CreateAndFillPage_Misc (wxNotebook *book);
	// Méthode pour la connexion des contrôles aux méthodes événementielles
	void CreateEventsConnexions();
	bool ApplyChanges();
	// Méthodes événementielles
	void OnSomethingHasChanged (wxCommandEvent &event);
	void OnDeleteAutoPlacementClicked (wxCommandEvent &event);
	void OnManualUpdatesCheck (wxCommandEvent &event);
	void OnSelectPanelColor (wxMouseEvent &event);
	void OnApplyOrOkClicked (wxCommandEvent &event);
	void OnTimer (wxTimerEvent& event);
	void OnOpenScreenshotsFolder (wxCommandEvent &event);
	void OnExportSettings (wxCommandEvent &event);
	void OnImportSettings (wxCommandEvent &event);
	void OnResetSettings (wxCommandEvent &event);
};

#endif // OPTIONSDIALOG_H_INCLUDED

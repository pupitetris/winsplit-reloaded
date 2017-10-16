#include <memory>

#include <wx/colordlg.h>
#include <wx/zipstrm.h>
#include <wx/wfstream.h>

#include "dialog_options.h"
#include "frame_virtualnumpad.h"
#include "dialog_download.h"
#include "main.h"
#include "virtual_key_manager.h"
#include "dialog_selectsettings.h"


OptionsDialog::OptionsDialog(wxWindow *parent) : wxDialog(parent,-1,_("Options"),wxDefaultPosition),
        m_options(SettingsManager::Get())
{
    m_autoPlacement.LoadData();
    p_virtNumpad = NULL;
    p_updateThread = NULL;
    m_timerCount = 0;
    wxBoxSizer *mainsizer = new wxBoxSizer(wxVERTICAL);
    p_nBook = new wxNotebook(this,-1);

    // Page "Général"
    CreateAndFillPage_General(p_nBook);
    // Page "Virtual Numpad"
    CreateAndFillPage_Numpad(p_nBook);
    // Page "Web updates"
    CreateAndFillPage_WebUpdate(p_nBook);
    // Page "Drag'N'Go"
    CreateAndFillPage_DragNGo(p_nBook);
    // Page "Divers"
    CreateAndFillPage_Misc(p_nBook);

    mainsizer->Add(p_nBook,0,wxALL|wxEXPAND,0);

    // Panel contenant les boutons
    wxPanel *cmdPanel=new wxPanel(this,-1);
    wxBoxSizer *btnSizer=new wxBoxSizer(wxHORIZONTAL);
    btnSizer->AddStretchSpacer(1);
    p_btnOk=new wxButton(cmdPanel,wxID_OK,_("&Ok"));
    btnSizer->Add(p_btnOk,0,wxALL|wxEXPAND,5);
    p_btnCancel=new wxButton(cmdPanel,wxID_CANCEL, _("Cancel"));
    btnSizer->Add(p_btnCancel,0,wxALL|wxEXPAND,5);
    p_btnApply=new wxButton(cmdPanel,wxID_APPLY,_("Apply"));
    p_btnApply->Disable();
    btnSizer->Add(p_btnApply,0,wxALL|wxEXPAND,5);
    cmdPanel->SetSizer(btnSizer);
    mainsizer->Add(cmdPanel,0,wxALL|wxEXPAND,0);

    SetSizer(mainsizer);
    mainsizer->SetSizeHints(this);

    CreateEventsConnexions();
}

OptionsDialog::~OptionsDialog()
{
    // Si  l'utilisateur ferme la fenetre pendant la recherche de mise à jour
    if(m_timer.IsRunning())
        m_timer.Stop();

    if(p_updateThread)
    {
        if (p_updateThread->IsRunning())
            p_updateThread->Delete();

        delete p_updateThread;
        p_updateThread = NULL;
    }
}

void OptionsDialog::CreateAndFillPage_General(wxNotebook *book)
{
	// Sizer général de la page
    wxBoxSizer *pageszr;
    // Variable pour les différents wxStaticBoxSizer
    wxStaticBoxSizer *stbszr;

    wxPanel* pnlGeneral = new wxPanel(book,-1);
    pageszr = new wxBoxSizer(wxVERTICAL);
		// Première zone : le choix de la langue
		stbszr = new wxStaticBoxSizer(wxHORIZONTAL,pnlGeneral,_("Language"));
			stbszr->Add(new wxStaticText(pnlGeneral,-1,_("Choose language")),0,wxALL|wxALIGN_CENTER_VERTICAL,5);
			p_cmbLanguage = new wxComboBox(pnlGeneral,-1,wxEmptyString,wxDefaultPosition,wxSize(150,-1),0,NULL,wxCB_READONLY);
			for (int i=0;i<m_options.GetAvailableLanguagesCount();i++)
				p_cmbLanguage->Append(SettingsManager::AvailableLangNames[i]);
			p_cmbLanguage->Select(m_options.getLanguageIndex());
			stbszr->AddStretchSpacer(1);
			stbszr->Add(p_cmbLanguage,0,wxALL|wxEXPAND,5);
		pageszr->Add(stbszr,0,wxALL|wxEXPAND,5);
		// Deuxième zone : "Divers"
		stbszr = new wxStaticBoxSizer(wxHORIZONTAL,pnlGeneral,_("Miscellaneous"));
			p_checkTmw = new wxCheckBox(pnlGeneral,-1,_("Take into account topmost windows"));
			p_checkTmw->SetValue(m_options.AcceptTopMostWindows());
			stbszr->Add(p_checkTmw,0,wxALL|wxEXPAND,5);
		pageszr->Add(stbszr,0,wxLEFT|wxRIGHT|wxEXPAND,5);
		// Troisième zone : Supprimer des valeurs de l'auto-placement
		stbszr = new wxStaticBoxSizer(wxHORIZONTAL,pnlGeneral,_("Delete saved Auto-placements"));
			p_btnDeleteAuto = new wxButton(pnlGeneral,-1,_("&Delete"));
			stbszr->Add(p_btnDeleteAuto,0,wxALL|wxEXPAND,5);
			stbszr->AddStretchSpacer(1);
			p_cmbAutoPlacement = new wxComboBox(pnlGeneral,-1,wxEmptyString,wxDefaultPosition,wxSize(180,-1),0,NULL,wxCB_READONLY);
			stbszr->Add(p_cmbAutoPlacement,0,wxALL|wxEXPAND,5);
		pageszr->Add(stbszr,0,wxALL|wxEXPAND,5);
		// Quatrième zone : les fichiers temporaires
		stbszr = new wxStaticBoxSizer(wxVERTICAL, pnlGeneral, _("Screenshots temporary files"));
			wxBoxSizer* line1=new wxBoxSizer(wxHORIZONTAL);
				p_checkDelTmpFiles=new wxCheckBox(pnlGeneral,-1,_("Delete automatically"));
				p_checkDelTmpFiles->SetValue(m_options.getAutoDeleteTempFiles());
				line1->Add(p_checkDelTmpFiles, 0, wxALL|wxALIGN_CENTER_VERTICAL,0);
				line1->AddSpacer(5);
				p_cmbDeleteTime=new wxComboBox(pnlGeneral,-1,wxEmptyString,wxDefaultPosition,wxDefaultSize,0,NULL,wxCB_READONLY);
				p_cmbDeleteTime->Append(_("When WinSplit starts"));
				p_cmbDeleteTime->Append(_("When WinSplit ends"));
				p_cmbDeleteTime->Select(m_options.getAutoDeleteTime());
				p_cmbDeleteTime->Enable(m_options.getAutoDeleteTempFiles());
				line1->Add(p_cmbDeleteTime,0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
				p_btnOpenFolder=new wxButton(pnlGeneral, -1, _("Open folder"));
				line1->AddSpacer(5);
				line1->Add(p_btnOpenFolder,0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
			stbszr->Add(line1, 0, wxALL|wxEXPAND, 5);
		pageszr->Add(stbszr,0,wxLEFT|wxRIGHT|wxEXPAND,5);
		// Cinquième zone : import / export des settings
		stbszr = new wxStaticBoxSizer(wxHORIZONTAL, pnlGeneral, _("Settings managements"));
            p_btnExportOpt=new wxButton(pnlGeneral, -1, _("Export settings"));
            stbszr->Add(p_btnExportOpt, 0, wxALL, 5);
            p_btnImportOpt=new wxButton(pnlGeneral, -1, _("Import settings"));
            stbszr->Add(p_btnImportOpt, 0, wxALL, 5);
        pageszr->Add(stbszr, 0, wxALL|wxEXPAND, 5);

    pnlGeneral->SetSizer(pageszr);
    book->AddPage(pnlGeneral,_("General"));

    // Remplissage de la combobox pour l'auto-placement
    wxArrayString array_name(m_autoPlacement.GetArrayName());
    p_cmbAutoPlacement->Clear();

    if (!m_autoPlacement.IsEmpty())
    {
        for (int i=0; i<int(array_name.Count()); ++i)
            p_cmbAutoPlacement->Append(array_name[i]);
    }
    else
    {
        p_cmbAutoPlacement->Append(_("No registered process"));
        p_cmbAutoPlacement->Disable();
        p_btnDeleteAuto->Disable();
    }

    p_cmbAutoPlacement->Select(0);
}

void OptionsDialog::CreateAndFillPage_Numpad(wxNotebook *book)
{
    // Sizer général de la page
    wxBoxSizer *pageszr;
    // Variable pour les différents wxStaticBoxSizer
    wxStaticBoxSizer *stbszr;

    wxPanel *pnlNumpad=new wxPanel(p_nBook,-1);
    pageszr=new wxBoxSizer(wxVERTICAL);
		// First zone: options
		stbszr=new wxStaticBoxSizer(wxVERTICAL,pnlNumpad,_("Virtual numpad options"));
			p_checkSaveVirtNumpad = new wxCheckBox(pnlNumpad,-1,_("Save Virtual numpad state when exit"));
			p_checkSaveVirtNumpad->SetValue(m_options.getSaveNumpadPosOnExit());
			stbszr->Add(p_checkSaveVirtNumpad,0,wxALL|wxEXPAND,5);
			p_checkShowVirtStart = new wxCheckBox(pnlNumpad,-1,_("Show Virtual numpad at starting"));
			p_checkShowVirtStart->SetValue(m_options.getShowNumpadAtBoot());
			stbszr->Add(p_checkShowVirtStart,0,wxALL|wxEXPAND,5);
			p_checkAutoHide = new wxCheckBox(pnlNumpad,-1,_("Hide Virtual numpad after a virtual key selection"));
			p_checkAutoHide->SetValue(m_options.getNumpadAutoHide());
			stbszr->Add(p_checkAutoHide,0,wxALL|wxEXPAND,5);
		pageszr->Add(stbszr,0,wxALL|wxEXPAND,5);
		// Second zone: styling (transparency)
		stbszr=new wxStaticBoxSizer(wxVERTICAL,pnlNumpad,_("Virtual numpad style"));
			stbszr->Add(new wxStaticText(pnlNumpad,-1,_("Adjust Virtual numpad transparency")),0,wxALL|wxALIGN_CENTER_HORIZONTAL,5);
			p_sliderNumpadTransparency = new wxSlider(pnlNumpad,-1,50,0,100,wxDefaultPosition,wxSize(250,-1),wxSL_HORIZONTAL|wxSL_LABELS|wxSL_AUTOTICKS);
			p_sliderNumpadTransparency->SetTickFreq(10, 20);
			p_sliderNumpadTransparency->SetValue(m_options.getNumpadTransparency());
			stbszr->Add(p_sliderNumpadTransparency,0,wxALL|wxEXPAND,5);
		pageszr->Add(stbszr,0,wxALL|wxEXPAND,5);
	pnlNumpad->SetSizer(pageszr);
	p_nBook->AddPage(pnlNumpad,_("Virtual numpad"));
}

void OptionsDialog::CreateAndFillPage_WebUpdate(wxNotebook *book)
{
    // Sizer général de la page
    wxBoxSizer *pageszr;
    // Variable pour les différents wxStaticBoxSizer
    wxStaticBoxSizer *stbszr;
    wxBoxSizer *hszr;

    wxPanel *pnlUpdates = new wxPanel(p_nBook,-1);
    pageszr = new wxBoxSizer(wxVERTICAL);
		// Première zone : les mises à jour automatiques
		stbszr = new wxStaticBoxSizer(wxVERTICAL,pnlUpdates,_("Checking for update"));
			p_checkUpdate = new wxCheckBox(pnlUpdates,-1,_("Check website for Winsplit updates"));
			p_checkUpdate->SetValue(m_options.hasToCheckForUpdates());
			stbszr->Add(p_checkUpdate,0,wxALL|wxEXPAND,5);
			hszr = new wxBoxSizer(wxHORIZONTAL);
				p_sttFrequency=new wxStaticText(pnlUpdates,-1,_("Web updates check frequency :"));
				p_sttFrequency->Enable(m_options.hasToCheckForUpdates());
				hszr->Add(p_sttFrequency,0,wxALL|wxALIGN_CENTER_VERTICAL);
				p_cmbUpdate = new wxComboBox(pnlUpdates,-1,wxEmptyString,wxDefaultPosition,wxDefaultSize,0,NULL,wxCB_READONLY);
				p_cmbUpdate->Append(_("At start"));
				p_cmbUpdate->Append(_("Weekly"));
				p_cmbUpdate->Append(_("Monthly"));
				p_cmbUpdate->SetSelection(m_options.getUpdateCheckFrequency()-1);
				p_cmbUpdate->Enable(m_options.hasToCheckForUpdates());
				hszr->Add(p_cmbUpdate,0,wxALL|wxEXPAND,5);
			stbszr->Add(hszr,0,wxALL|wxEXPAND,5);
		pageszr->Add(stbszr,0,wxALL|wxEXPAND,5);
		// Deuxième zone : vérification manuelle
		stbszr = new wxStaticBoxSizer(wxHORIZONTAL,pnlUpdates,_("Manual check :"));
			stbszr->Add(new wxStaticText(pnlUpdates,-1,_("Perform a manual web update check now")),0,wxALL|wxALIGN_CENTER_VERTICAL,5);
			p_btnCheckNow = new wxButton(pnlUpdates,-1, _("Check now"));
			p_stcTxtStatus = new wxStaticText(pnlUpdates,wxID_ANY,wxEmptyString);
			stbszr->Add(p_btnCheckNow, wxALL|wxEXPAND,5);
			pageszr->Add(stbszr,0,wxALL|wxEXPAND,5);
		pageszr->Add(p_stcTxtStatus,0,wxALL|wxEXPAND|wxALIGN_LEFT,5);
	pnlUpdates->SetSizer(pageszr);
    p_nBook->AddPage(pnlUpdates,_("Web update"));
}

void OptionsDialog::CreateAndFillPage_DragNGo(wxNotebook *book)
{
    // Sizer général de la page
    wxBoxSizer *pageszr;
    // Variable pour les différents wxStaticBoxSizer
    wxStaticBoxSizer *stbszr;
    wxBoxSizer *hszr;
    wxFlexGridSizer *grdszr;
    VirtualModifierManager mod_manager;

    wxPanel *pnlDragNGo = new wxPanel(p_nBook);
    pageszr = new wxBoxSizer(wxVERTICAL);
		// Première zone : les options
		stbszr = new wxStaticBoxSizer(wxVERTICAL,pnlDragNGo,_("Drag'N'Go settings :"));
			hszr = new wxBoxSizer(wxHORIZONTAL);
				p_checkEnableDnG = new wxCheckBox(pnlDragNGo,wxID_ANY,_("Enable Drag'N'Go"));
				p_checkEnableDnG->SetValue(m_options.IsDragNGoEnabled());
				hszr->Add(p_checkEnableDnG,0,wxALL|wxALIGN_CENTER_VERTICAL,5);
				hszr->AddStretchSpacer(1);
				p_sttRadius = new wxStaticText(pnlDragNGo,wxID_ANY,_("Detection radius :"));
				p_sttRadius->Enable(m_options.IsDragNGoEnabled());
				hszr->Add(p_sttRadius,0,wxALL|wxALIGN_CENTER_VERTICAL,0);
				p_txtDnGRadius = new wxTextCtrl(pnlDragNGo,-1,wxEmptyString,wxDefaultPosition, wxSize(40,-1));
				wxTextValidator validator(wxFILTER_INCLUDE_CHAR_LIST, NULL);
				wxArrayString allowedChars;
				for(int i=0;i<10;++i)
					allowedChars.Add(wxString::Format(_T("%0d"),i));
				validator.SetIncludes(allowedChars);
				p_txtDnGRadius->SetValidator(validator);
				*p_txtDnGRadius << m_options.getDnGDetectionRadius();
				p_txtDnGRadius->Enable(m_options.IsDragNGoEnabled());
				hszr->Add(p_txtDnGRadius,0,wxALL|wxEXPAND,5);
			stbszr->Add(hszr,0,wxALL|wxEXPAND,0);
		pageszr->Add(stbszr,0,wxALL|wxEXPAND,5);
		// Deuxième zone : le style
		stbszr=new wxStaticBoxSizer(wxVERTICAL,pnlDragNGo,_("Destination zone style :"));
			grdszr=new wxFlexGridSizer(2,2,2,2);
				grdszr->Add(new wxStaticText(pnlDragNGo,wxID_ANY,_("Click here to select preview zone background color :")),0,wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT);
				p_pnlZoneBgColor=new wxPanel(pnlDragNGo,wxID_ANY, wxDefaultPosition, wxSize(20,20),wxBORDER_STATIC);
				p_pnlZoneBgColor->SetBackgroundColour(m_options.getDnGZoneBgColor());
				grdszr->Add(p_pnlZoneBgColor,0,wxALL|wxEXPAND,0);
				grdszr->Add(new wxStaticText(pnlDragNGo,wxID_ANY,_("Click here to select preview zone text color :")),0,wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT);
				p_pnlZoneFgColor=new wxPanel(pnlDragNGo,wxID_ANY, wxDefaultPosition, wxSize(20,20),wxBORDER_STATIC);
				p_pnlZoneFgColor->SetBackgroundColour(m_options.getDnGZoneFgColor());
				grdszr->Add(p_pnlZoneFgColor,0,wxALL|wxEXPAND,0);
				grdszr->SetFlexibleDirection(wxHORIZONTAL);
			stbszr->Add(grdszr,0,wxALL|wxEXPAND,5);
			stbszr->Add(new wxStaticText(pnlDragNGo,wxID_ANY,_("Adjust destination zone transparency")),0,wxTOP|wxALIGN_CENTER_HORIZONTAL,5);
			p_sliderZoneTransparency = new wxSlider(pnlDragNGo,-1,50,0,100,wxDefaultPosition,wxSize(250,-1),wxSL_HORIZONTAL|wxSL_LABELS);
			p_sliderZoneTransparency->SetValue(m_options.getDngZoneTransparency());
			stbszr->Add(p_sliderZoneTransparency,0,wxALL|wxEXPAND,5);
		pageszr->Add(stbszr,0,wxALL|wxEXPAND,5);
		// Troisième zone : les touches d'activation
		stbszr = new wxStaticBoxSizer(wxHORIZONTAL,pnlDragNGo,_("Modifier Activation :"));
			hszr = new wxBoxSizer(wxHORIZONTAL);

				p_cmbModifier1 = new wxComboBox(pnlDragNGo,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,0,NULL,wxCB_READONLY);
				p_cmbModifier2 = new wxComboBox(pnlDragNGo,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,0,NULL,wxCB_READONLY);

				for(unsigned int i=0;i<mod_manager.GetSize();++i)
				{
					p_cmbModifier1->Append(mod_manager.GetStringFromIndex(i));
					p_cmbModifier2->Append(mod_manager.GetStringFromIndex(i));
				}
				p_cmbModifier1->Select(mod_manager.GetIndexFromValue(m_options.getDnGMod1()));
				p_cmbModifier2->Select(mod_manager.GetIndexFromValue(m_options.getDnGMod2()));

				hszr->Add(new wxStaticText(pnlDragNGo,wxID_ANY,_("Modifier to active Drag'N'Go :")),0,wxALL|wxALIGN_CENTER_VERTICAL,5);
				hszr->Add(p_cmbModifier1,0,wxALL|wxALIGN_CENTER_VERTICAL,5);
				hszr->Add(new wxStaticText(pnlDragNGo,wxID_ANY,_T("+")),0,wxALL|wxALIGN_CENTER_VERTICAL,5);
				hszr->Add(p_cmbModifier2,0,wxALL|wxALIGN_CENTER_VERTICAL,5);
			stbszr->Add(hszr,0,wxALL|wxEXPAND,0);
		pageszr->Add(stbszr,0,wxALL|wxEXPAND,5);
    pnlDragNGo->SetSizer(pageszr);
    p_nBook->AddPage(pnlDragNGo,_("Drag'N'Go"));
}

void OptionsDialog::CreateAndFillPage_Misc(wxNotebook *book)
{
	// Sizer général de la page
    wxBoxSizer *pageszr;
    // Variable pour les différents wxStaticBoxSizer
    wxStaticBoxSizer *stbszr;
    wxStaticText *label;
    wxBoxSizer *lnszr;

    wxPanel *pnlMisc = new wxPanel(p_nBook);
    pageszr = new wxBoxSizer(wxVERTICAL);
		// Première zone : la fonction "auto-focus" de Windows
		stbszr = new wxStaticBoxSizer(wxVERTICAL,pnlMisc,_("Windows 'Active Window Tracking' built-in functionnality :"));
			label=new wxStaticText(pnlMisc, -1, _("Any modification here needs at least a session restart to take effect."));
			stbszr->Add(label,0,wxALL|wxEXPAND,5);
			p_checkEnableXMouse=new wxCheckBox(pnlMisc, wxID_ANY, _("Enable the Windows 'Active Window Tracking' built-in functionnality."));
			p_checkEnableXMouse->SetValue(m_options.IsXMouseActivated());
			stbszr->Add(p_checkEnableXMouse, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, 5);
			lnszr=new wxBoxSizer(wxHORIZONTAL);
                lnszr->AddSpacer(20);
                p_checkEnableZOrder=new wxCheckBox(pnlMisc, wxID_ANY, _("Active Window Tracking will bring the window to the top."));
                p_checkEnableZOrder->SetValue(m_options.IsAutoZOrderActivated());
                lnszr->Add(p_checkEnableZOrder, 0, wxALL, 0);
            stbszr->Add(lnszr, 0, wxALL|wxEXPAND, 5);
			lnszr=new wxBoxSizer(wxHORIZONTAL);
				//lnszr->AddSpacer(20);
				p_sttZOrderDelay=new wxStaticText(pnlMisc, -1, _("Delay before the action of the 'Active Window Tracking' (milliseconds):"));
				lnszr->Add(p_sttZOrderDelay, 0, wxRIGHT|wxALIGN_CENTER_VERTICAL, 5);
				p_txtZOrderDelay=new wxTextCtrl(pnlMisc, wxID_ANY, _T(""), wxDefaultPosition, wxSize(50,-1));
				p_txtZOrderDelay->SetValue(wxString::Format(_T("%0d"), m_options.getAutoZOrderDelay()));
				lnszr->Add(p_txtZOrderDelay, 0, wxALL, 0);
			stbszr->Add(lnszr, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, 5);
		pageszr->Add(stbszr,0,wxALL|wxEXPAND,5);
		// Zone "Options diverses"
		stbszr = new wxStaticBoxSizer(wxVERTICAL, pnlMisc, _("Miscellaneous options :"));
			p_checkFollowWnd=new wxCheckBox(pnlMisc, wxID_ANY, _("Move the mouse pointer to follow the active window."));
			p_checkFollowWnd->SetValue(m_options.getMouseFollowWindow());
			stbszr->Add(p_checkFollowWnd, 0, wxALL|wxEXPAND, 5);
			lnszr=new wxBoxSizer(wxHORIZONTAL);
				lnszr->AddSpacer(20);
				p_checkFollowOnlyIn=new wxCheckBox(pnlMisc, wxID_ANY, _("Move the mouse pointer only if it is over the active window's area"));
				p_checkFollowOnlyIn->SetValue(m_options.getMouseFollowOnlyWhenIn());
				lnszr->Add(p_checkFollowOnlyIn, 0, wxALL, 0);
			stbszr->Add(lnszr, 0, wxLEFT|wxRIGHT, 5);
			p_checkMinMaxCycle=new wxCheckBox(pnlMisc, wxID_ANY, _("Minimize and Maximize hotkeys gives a 'Minimize / Maximize / Restored' state cycle."));
			p_checkMinMaxCycle->SetValue(m_options.getMinMaxCycle());
			stbszr->Add(p_checkMinMaxCycle, 0, wxALL, 5);
		pageszr->Add(stbszr,0,wxALL|wxEXPAND,5);
	pnlMisc->SetSizer(pageszr);
	p_nBook->AddPage(pnlMisc,_("Misc"));
}
void OptionsDialog::CreateEventsConnexions()
{
    // Page "Général"
    p_cmbLanguage->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED,wxCommandEventHandler(OptionsDialog::OnSomethingHasChanged), NULL, this);
    p_checkTmw->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED ,wxCommandEventHandler(OptionsDialog::OnSomethingHasChanged), NULL, this);
    p_btnDeleteAuto->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(OptionsDialog::OnDeleteAutoPlacementClicked), NULL, this);
    p_checkDelTmpFiles->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(OptionsDialog::OnSomethingHasChanged), NULL, this);
    p_cmbDeleteTime->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED,wxCommandEventHandler(OptionsDialog::OnSomethingHasChanged), NULL, this);
    p_btnOpenFolder->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(OptionsDialog::OnOpenScreenshotsFolder), NULL, this);
    p_btnExportOpt->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(OptionsDialog::OnExportSettings), NULL, this);
    p_btnImportOpt->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(OptionsDialog::OnImportSettings), NULL, this);
    // Page "Virtual Numpad"
    p_checkSaveVirtNumpad->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED ,wxCommandEventHandler(OptionsDialog::OnSomethingHasChanged),NULL,this);
    p_checkShowVirtStart->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED ,wxCommandEventHandler(OptionsDialog::OnSomethingHasChanged),NULL,this);
    p_checkAutoHide->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED ,wxCommandEventHandler(OptionsDialog::OnSomethingHasChanged),NULL,this);
    p_sliderNumpadTransparency->Connect(wxEVT_SCROLL_CHANGED ,wxCommandEventHandler(OptionsDialog::OnSomethingHasChanged),NULL,this);
    // Page "Web updates"
    p_checkUpdate->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED ,wxCommandEventHandler(OptionsDialog::OnSomethingHasChanged),NULL,this);
    p_cmbUpdate->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED,wxCommandEventHandler(OptionsDialog::OnSomethingHasChanged),NULL,this);
    p_btnCheckNow->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(OptionsDialog::OnManualUpdatesCheck),NULL,this);
    // Page "Drag'N'Go
    p_checkEnableDnG->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED ,wxCommandEventHandler(OptionsDialog::OnSomethingHasChanged),NULL,this);
    p_txtDnGRadius->Connect(wxEVT_COMMAND_TEXT_UPDATED ,wxCommandEventHandler(OptionsDialog::OnSomethingHasChanged),NULL,this);
    p_pnlZoneBgColor->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(OptionsDialog::OnSelectPanelColor),NULL,this);
    p_pnlZoneFgColor->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(OptionsDialog::OnSelectPanelColor),NULL,this);
    p_sliderZoneTransparency->Connect(wxEVT_SCROLL_CHANGED ,wxCommandEventHandler(OptionsDialog::OnSomethingHasChanged),NULL,this);
    p_cmbModifier1->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED,wxCommandEventHandler(OptionsDialog::OnSomethingHasChanged),NULL,this);
    p_cmbModifier2->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED,wxCommandEventHandler(OptionsDialog::OnSomethingHasChanged),NULL,this);
    // Page "Divers"
    p_checkEnableXMouse->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED ,wxCommandEventHandler(OptionsDialog::OnSomethingHasChanged),NULL,this);
    p_checkEnableZOrder->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED ,wxCommandEventHandler(OptionsDialog::OnSomethingHasChanged),NULL,this);
    p_txtZOrderDelay->Connect(wxEVT_COMMAND_TEXT_UPDATED ,wxCommandEventHandler(OptionsDialog::OnSomethingHasChanged),NULL,this);
    p_checkFollowWnd->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED ,wxCommandEventHandler(OptionsDialog::OnSomethingHasChanged),NULL,this);
    p_checkFollowOnlyIn->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED ,wxCommandEventHandler(OptionsDialog::OnSomethingHasChanged),NULL,this);
    p_checkMinMaxCycle->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED ,wxCommandEventHandler(OptionsDialog::OnSomethingHasChanged),NULL,this);
    // Boutons "Ok" et "Appliquer"
    p_btnApply->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(OptionsDialog::OnApplyOrOkClicked),NULL,this);
    p_btnOk->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(OptionsDialog::OnApplyOrOkClicked),NULL,this);

    Connect(wxEVT_TIMER,wxTimerEventHandler(OptionsDialog::OnTimer),NULL,this);
}

void OptionsDialog::SetVirtualNumPad(VirtualNumpad * numpad)
{
    p_virtNumpad = numpad;
}

void OptionsDialog::OnDeleteAutoPlacementClicked(wxCommandEvent &event)
{
    if (!m_autoPlacement.IsEmpty())
    {
        m_autoPlacement.DeleteApplication(p_cmbAutoPlacement->GetSelection());
        m_autoPlacement.SaveData();
        m_autoPlacement.LoadData();
        p_cmbAutoPlacement->Delete(p_cmbAutoPlacement->GetCurrentSelection());
        if (p_cmbAutoPlacement->GetCount() == 0)
        {
            p_cmbAutoPlacement->Append(_("No registered process"));
            p_cmbAutoPlacement->Disable();
            p_btnDeleteAuto->Disable();
        }
        p_cmbAutoPlacement->Select(0);
    }
}

void OptionsDialog::OnManualUpdatesCheck(wxCommandEvent &event)
{
    p_btnCheckNow->Enable(false);
    p_stcTxtStatus->SetLabel(_("Processing"));
    m_timer.SetOwner(this,wxID_ANY);
    m_timer.Start(200);
}

void OptionsDialog::OnSelectPanelColor(wxMouseEvent &event)
{
    wxPanel *pnl=(wxPanel*)event.GetEventObject();
    wxColourData cdata;
    cdata.SetColour(pnl->GetBackgroundColour());
    cdata.SetChooseFull(true);
    wxColourDialog cdlg(this,&cdata);
    if (cdlg.ShowModal()==wxID_CANCEL) return;
    cdata=cdlg.GetColourData();
    pnl->SetBackgroundColour(cdata.GetColour());
    pnl->Refresh();
    p_btnApply->Enable();
}

void OptionsDialog::OnSomethingHasChanged(wxCommandEvent &event)
{
    bool bEnable=true;
    int id=event.GetId();
    if (id==p_cmbLanguage->GetId())
    {
        // On n'active le bouton "Appliquer" que si a sélection a changé
        if (p_cmbLanguage->GetSelection()==m_options.getLanguageIndex())
            bEnable=false;
    }
    else if (id==p_sliderNumpadTransparency->GetId())
    {
        if (p_virtNumpad!=NULL)
            p_virtNumpad->SetTransparency(p_sliderNumpadTransparency->GetValue());
    }
    else if (id==p_checkUpdate->GetId())
    {
        p_sttFrequency->Enable(p_checkUpdate->GetValue());
        p_cmbUpdate->Enable(p_checkUpdate->GetValue());
    }
    else if (id==p_cmbUpdate->GetId())
    {
        // On n'active le bouton "Appliquer" que si la sélection a changé
        if (p_cmbUpdate->GetSelection()==(m_options.getUpdateCheckFrequency()-1))
            bEnable=false;
    }
    else if (id==p_checkEnableDnG->GetId())
    {
        p_sttRadius->Enable(p_checkEnableDnG->GetValue());
        p_txtDnGRadius->Enable(p_checkEnableDnG->GetValue());
    }
    else if (id==p_checkDelTmpFiles->GetId())
    {
    	p_cmbDeleteTime->Enable(p_checkDelTmpFiles->GetValue());
    }
    else if (id==p_cmbDeleteTime->GetId())
    {
    	// On n'active le bouton "Appliquer" que si la sélection a changé
        if (p_cmbUpdate->GetSelection()==(m_options.getAutoDeleteTime()))
            bEnable=false;
    }
    bEnable |= p_btnApply->IsEnabled();
    p_btnApply->Enable(bEnable);
}

void OptionsDialog::OnApplyOrOkClicked(wxCommandEvent &event)
{
    if (ApplyChanges())
    {
        if (event.GetId()==wxID_OK)
        {
            m_options.SaveSettings();
            EndModal(wxID_OK);
        }
        else
        {
            p_btnApply->Disable();
        }
    }
}

bool OptionsDialog::ApplyChanges()
{
    long l;
    VirtualModifierManager mod_manager;

    if(p_cmbModifier1->GetCurrentSelection() == p_cmbModifier2->GetCurrentSelection())
    {
        if((p_cmbModifier1->GetCurrentSelection()!= 0))
        {
            wxMessageBox(_("The two modifiers must be different"), _("WinSplit message"), wxICON_EXCLAMATION);
            return false;
        }
    }
    // On vérifie la valeur du rayon de détection
    // Pour le Drag'n'Go est Ok, sinon, on ne sauve aucune options
    wxString sValue=p_txtDnGRadius->GetValue();
    sValue.ToLong(&l);
    if (l<=0) l=0;
    wxString sMsg=_("The value you entered as 'Detection radius' is ");
    if (l<20)
    {
        sMsg << _("too small") << _T(".\n");
        sMsg << _("The minimum value you can enter is '20'.");
        wxMessageBox(sMsg, _("WinSplit message"), wxICON_EXCLAMATION);
        p_txtDnGRadius->SetValue(_T("20"));
        return false;
    }
    else if (l>500)
    {
        sMsg << _("too big") << _T(".\n");
        sMsg << _("The maximum value you can enter is '500'.");
        wxMessageBox(sMsg, _("WinSplit message"), wxICON_EXCLAMATION);
        p_txtDnGRadius->SetValue(_T("500"));
        return false;
    }
    else m_options.setDnGDetectionRadius((int)l);

    // Onglet "Général"
    m_options.setLanguageIndex(p_cmbLanguage->GetSelection());
    m_options.setAcceptTopMostWindows(p_checkTmw->GetValue());
    m_options.setAutoDeleteTempFiles(p_checkDelTmpFiles->GetValue());
    m_options.setAutoDeleteTime(p_cmbDeleteTime->GetSelection());
    // Onglet "Virtual Numpad"
    m_options.setSaveNumpadPosOnExit(p_checkSaveVirtNumpad->GetValue());
    m_options.setShowNumpadAtBoot(p_checkShowVirtStart->GetValue());
    m_options.setNumpadAutoHide(p_checkAutoHide->GetValue());
    p_virtNumpad->SetCheckValue(!p_checkAutoHide->GetValue());
    m_options.setNumpadTransparency(p_sliderNumpadTransparency->GetValue());
    p_virtNumpad->SetTransparency(p_sliderNumpadTransparency->GetValue());
    // Onglet "Web update"
    m_options.setCheckForUpdates(p_checkUpdate->GetValue());
    m_options.setUpdateCheckFrequency(p_cmbUpdate->GetSelection()+1);
    // Onglet "Drag'N'Go
    m_options.EnableDragNGo(p_checkEnableDnG->GetValue());
    m_options.setDnGZoneBgColor(p_pnlZoneBgColor->GetBackgroundColour());
    m_options.setDnGZoneFgColor(p_pnlZoneFgColor->GetBackgroundColour());
    m_options.setDnGZoneTransparency(p_sliderZoneTransparency->GetValue());
    m_options.setDnGMod1(mod_manager.GetValueFromIndex(p_cmbModifier1->GetCurrentSelection()));
    m_options.setDnGMod2(mod_manager.GetValueFromIndex(p_cmbModifier2->GetCurrentSelection()));
    // Onglet "Divers"
    m_options.setXMouseActivation(p_checkEnableXMouse->GetValue());
    m_options.setAutoZOrderActivation(p_checkEnableZOrder->GetValue());
    sValue=p_txtZOrderDelay->GetValue();
    sValue.ToLong(&l);
    m_options.setAutoZOrderDelay((int)l);
    m_options.setMouseFollowWindow(p_checkFollowWnd->GetValue());
    m_options.setMouseFollowOnlyWhenIn(p_checkFollowOnlyIn->GetValue());
    m_options.setMinMaxCycle(p_checkMinMaxCycle->GetValue());

    return true;
}

void OptionsDialog::OnTimer(wxTimerEvent& event)
{
    if(!p_updateThread)
    {
        // timeout = 5 car devrait rapidement récupérer le fichier version.txt
        p_updateThread = new ReadVersionThread(5);
        p_updateThread->ForceChecking();
        p_updateThread->Create();
        p_updateThread->Run();
    }

    if (p_updateThread->IsRunning())
    {
        switch (m_timerCount)
        {
        case 0:
            ++m_timerCount;
            p_stcTxtStatus->SetLabel(p_stcTxtStatus->GetLabel() + _T("."));
            break;
        case 1:
            ++m_timerCount;
            p_stcTxtStatus->SetLabel(p_stcTxtStatus->GetLabel() + _T("."));
            break;
        case 2:
            ++m_timerCount;
            p_stcTxtStatus->SetLabel(p_stcTxtStatus->GetLabel() + _T("."));
            break;
        case 3:
            ++m_timerCount;
            p_stcTxtStatus->SetLabel(p_stcTxtStatus->GetLabel() + _T("."));
            break;
        case 4:
            ++m_timerCount;
            p_stcTxtStatus->SetLabel(p_stcTxtStatus->GetLabel() + _T("."));
            break;
        default:
            m_timerCount = 0;
            p_stcTxtStatus->SetLabel(_("Processing"));
            break;
        }
    }
    else
    {
        m_timer.Stop();

        wxString versionText = p_updateThread->GetFeatures();
        double host_version = p_updateThread->GetHostVersion();
        double local_version;

        wxGetApp().GetVersion().ToDouble(&local_version);

        delete p_updateThread;
        p_updateThread = NULL;

        if(versionText != wxEmptyString)
        {
            if (host_version > local_version)
            {
                DownloadDialog dialog(versionText);
                if(dialog.ShowModal() == wxOK)
                {
                    wxGetApp().GetTrayInstance()->RemoveIcon();
                    /// En attente de mieux
                    exit(0);
                }
            }
            else
            {
                wxString message;
                message << _("You have the latest version of WinSplit Revoluton. "
                            "Always feel free to check out the Reloaded fork website fore more information");
                message << _T("\nhttps://github.com/pupitetris/winsplit-reloaded");
                wxMessageBox(message,_T("WinSplit Revolution"),wxOK,this);

            }
        }
        else
        {
            wxMessageBox(_("Impossible to retrieve host informations"),_T("WinSplit Revolution"),wxICON_EXCLAMATION);
        }

        p_btnCheckNow->Enable(true);
        p_stcTxtStatus->SetLabel(wxEmptyString);
    }

}

void OptionsDialog::OnOpenScreenshotsFolder(wxCommandEvent &event)
{
	wxString sCmd=wxGetOSDirectory();
	if (sCmd.Right(1)!=_T("\\")) sCmd.Append(_T('\\'));
	sCmd.Append(_T("explorer.exe "));
	sCmd.Append(m_options.getAppPath());
	sCmd.append(_T("screenshots"));
	wxExecute(sCmd);
}

void OptionsDialog::OnExportSettings(wxCommandEvent &event)
{
    wxFileDialog fdlg(this, _("Export settings"), m_options.GetDataDirectory(),
        _T("WinSplitSettings.export"),
        _("Export files|*.export|All files|*.*"),
        wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (fdlg.ShowModal()==wxID_CANCEL) return;

    SelectSettingsDialog dlg(this, SSDT_EXPORT);
    if (dlg.ShowModal()==wxID_CANCEL) return;

    wxString sMsg=_T("Selected options :");
    int iSel=dlg.GetSelectionFlags();

    wxFileOutputStream f_out(fdlg.GetPath());
    if (!f_out.IsOk())
    {
        wxMessageBox(_("There was an error while creating the export file !"), _("Error"), wxICON_EXCLAMATION);
        return;
    }
    wxZipOutputStream z_out(f_out, 9);
    if (iSel && SST_GENERAL)
    {
        wxFileInputStream f_in(m_options.GetDataDirectory() + _T("Settings.xml"));
        z_out.PutNextEntry(_T("Settings.xml"));
        z_out << f_in;
    }

    if (iSel && SST_HOTKEYS)
    {
        wxFileInputStream f_in(m_options.GetDataDirectory() + _T("hotkeys.xml"));
        z_out.PutNextEntry(_T("hotkeys.xml"));
        z_out << f_in;
    }
    if (iSel && SST_LAYOUT)
    {
        wxFileInputStream f_in(m_options.GetDataDirectory() + _T("layout.xml"));
        z_out.PutNextEntry(_T("layout.xml"));
        z_out << f_in;
    }
    if (iSel && SST_PLACES)
    {
        wxFileInputStream f_in(m_options.GetDataDirectory() + _T("auto_placement.xml"));
        if (f_in.IsOk())
        {
            z_out.PutNextEntry(_T("auto_placement.xml"));
            z_out << f_in;
        }
    }
}

void OptionsDialog::OnImportSettings(wxCommandEvent &event)
{
    wxFileDialog fdlg(this, _("Import settings"), _T(""),
        _T(""),
        _("Export files|*.export|All files|*.*"),
        wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (fdlg.ShowModal()==wxID_CANCEL) return;

    wxFileInputStream f_in(fdlg.GetPath());
    if (!f_in.IsOk())
    {
        wxMessageBox(_("There was an error while opening the export file !"), _("Error"), wxICON_EXCLAMATION);
        return;
    }
    wxZipInputStream z_in(f_in);
    std::auto_ptr<wxZipEntry> entry;
    wxString sDir=m_options.GetDataDirectory();
    while (entry.reset(z_in.GetNextEntry()), entry.get() != NULL)
    {
    	wxString name=sDir + entry->GetName();
		wxFileOutputStream f_out(name);
		f_out << z_in;
    }
    m_options.LoadSettings();
    EndModal(wxID_CANCEL);
}

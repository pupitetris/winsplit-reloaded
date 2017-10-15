#ifndef __MERGEWINDOWS_h__
#define __MERGEWINDOWS_h__

class FusionDialog:public wxDialog
{
private:
    HWND AppToMerge[2];
    RECT cadre;

    wxCheckBox* p_checkSwitch;
    wxCheckBox* p_checkHV;
    wxSlider* p_sliderRatio;
    wxButton* p_btnCenter, *p_btnClose, *p_btnCancel;

    void CreateControls();
    void CreateConnexions();
    void InitialisationFrame();
    void OnClose(wxCloseEvent& event);
    void OnButtonClick(wxCommandEvent& WXUNUSED(event));
    void OnScrollSlide(wxScrollEvent &event);
    void SwitchWindows(wxCommandEvent &event);
    void OnButtonCentre(wxCommandEvent &ev);
    void HorizontalVertical(wxCommandEvent &event);

public:
    FusionDialog(wxWindow* parent = NULL,
                wxWindowID id = wxID_ANY,
                const wxString& title = wxEmptyString,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxCAPTION|wxSYSTEM_MENU|wxDIALOG_NO_PARENT|wxCLOSE_BOX|wxSTAY_ON_TOP);

    ~FusionDialog();

    void SetTabHwnd(HWND tab[2]);
    void ResizeAt(const int& pos);
};

#endif //__MERGEWINDOWS_h__

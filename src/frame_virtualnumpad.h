// Fichier frame_popup.h
#ifndef __POPUPTRAY_H__
#define __POPUPTRAY_H__

enum
{
    ID_1=1,
    ID_2,
    ID_3,
    ID_4,
    ID_5,
    ID_6,
    ID_7,
    ID_8,
    ID_9,
    ID_CHECK
};

class VirtualNumpad : public wxFrame
{
private:
    wxCheckBox* p_checkStay;
    wxBoxSizer* p_sizer;
    wxPanel* p_panel;

    bool m_reducedStyle;
    wxPoint m_frame_pos;

    void CreateControls();
    void CreateConnections();
    void OnClicEvent(wxCommandEvent& event);
    void OnCLose(wxCloseEvent& event);

public:
    VirtualNumpad(wxWindow* parent = NULL,
              wxWindowID id = wxID_ANY,
              const wxString& title = wxEmptyString,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = wxCAPTION|wxSTAY_ON_TOP|wxSYSTEM_MENU|wxCLOSE_BOX|wxFRAME_TOOL_WINDOW);

    ~VirtualNumpad();

    void SetFlag(bool b);
    bool GetFlag();
    void FitFrame();

    void SetTransparancy(int val);
    void SetStyleReduit(bool flag);
    void SetCheckValue(bool b);
    bool GetCheckValue();
    wxPoint GetNotReducedPosition();
};

#endif //__POPUPTRAY_H__

#ifndef __MAIN_FRAME_H__
#define __MAIN_FRAME_H__

#include <wx/wx.h>
#include <windows.h>
#include <vector>

#include "layout_manager.h"
#include "settingsmanager.h"


class FrameHook: public wxFrame
{
private:
    wxFrame* p_panel;
    wxStaticText* p_stcTxtInfo;

    wxTimer m_timer;
    std::vector<wxRect> m_vec_solution;

    unsigned int WSM_STARTMOVING;
    unsigned int WSM_STOPMOVING;
    unsigned int WSM_MOLETTE;

    unsigned int WSM_TASKBAR_CREATED;

    SettingsManager &m_options;
    int m_iTransparency;
    bool m_is_near;
    bool m_isInstalled;
    wxRect m_rectPrevious;
    int m_wheelpos;
    int m_wheelposPrevious;

    inline unsigned int ModifierToVK(const unsigned int& mod);
    bool IsDown();

private:
    void CreateConnection();
    void CreateControls();
    void UnSetHook(wxCommandEvent&);
    void SetHook(wxCommandEvent&);
    void OnTimer(wxTimerEvent& event);
    WXLRESULT MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);
    void MoveWindowToDestination();

public:
    FrameHook(wxWindow* parent = NULL,
          wxWindowID id = 0,
          const wxString& title = wxEmptyString, //_("Main_Frame"),
          const wxPoint& pos = wxDefaultPosition,
          const wxSize& size = wxSize(0,0),
          long style = wxFRAME_TOOL_WINDOW);
    ~FrameHook();

    bool IsHookInstalled(){return m_isInstalled;}
    void UnSetHook();
    void SetHook();
};

#endif // __MAIN_FRAME_H__


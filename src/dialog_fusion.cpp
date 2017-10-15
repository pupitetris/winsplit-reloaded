#include <wx/accel.h>

// Pour utiliser l'algo swap
#include <algorithm>

#include "dialog_fusion.h"

#include "../images/icone.xpm"


FusionDialog::FusionDialog(wxWindow *parent,wxWindowID id,const wxString &title,
                                       const wxPoint &position,const wxSize& size,long style)
:wxDialog(parent, id, _("Windows fusion"), position, size, style),
p_checkSwitch(NULL),
p_checkHV(NULL),
p_sliderRatio(NULL),
p_btnCenter(NULL)
{
    CreateControls();
    CreateConnexions();
    Center();
}


FusionDialog::~FusionDialog(){}


void FusionDialog::CreateControls()
{
    wxBoxSizer* pmainSizer;

    //Button special pour la touche ESC
    new wxButton(this,wxID_CANCEL,wxEmptyString,wxDefaultPosition,wxSize(0,0));

    pmainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* mainSizer_haut = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* mainSizer_check = new wxBoxSizer(wxVERTICAL);

    p_checkSwitch = new wxCheckBox(this,wxID_ANY,_("Switch windows"));
    p_checkHV = new wxCheckBox(this,wxID_ANY,_("Horizontal/Vertical"));
    p_btnCenter = new wxButton(this,wxID_ANY,_("Center"),wxDefaultPosition,wxDefaultSize);
    p_sliderRatio = new wxSlider(this,wxID_ANY,25,0,50,wxDefaultPosition,wxSize(330,21),
                          wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_SELRANGE);

    p_sliderRatio->SetFocus();
    mainSizer_check->Add(p_checkSwitch,0,wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL,5);
    mainSizer_check->Add(p_checkHV,0,wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL,5);

    mainSizer_haut->Add(mainSizer_check,0,wxALIGN_CENTER|wxALL,5);
    mainSizer_haut->Add(p_btnCenter,0,wxALIGN_CENTER|wxALL,5);

    pmainSizer->Add(mainSizer_haut,0,wxALIGN_CENTER|wxALL,5);
    pmainSizer->Add(p_sliderRatio,0,wxALIGN_CENTER|wxALL,10);

    wxBoxSizer* btnSizer=new wxBoxSizer(wxHORIZONTAL);
        btnSizer->AddStretchSpacer(1);
        p_btnClose=new wxButton(this, wxID_OK, _("Ok"));
        p_btnClose->SetDefault();
        btnSizer->Add(p_btnClose, 0, wxALL, 5);
        p_btnCancel=new wxButton(this, wxID_CANCEL, _T("Cancel"));
        btnSizer->Add(p_btnCancel, 0, wxALL, 5);
    pmainSizer->Add(btnSizer, 0, wxALL|wxEXPAND, 5);

    SetIcon(wxIcon(icone_xpm));
    SetSizer(pmainSizer);

    pmainSizer->Fit(this);
}

void FusionDialog::CreateConnexions()
{
    p_checkSwitch->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED,wxCommandEventHandler(FusionDialog::SwitchWindows),NULL,this);
    p_checkHV->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED,wxCommandEventHandler(FusionDialog::HorizontalVertical),NULL,this);
    p_btnCenter->Connect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(FusionDialog::OnButtonCentre),NULL,this);

    p_sliderRatio->Connect(wxEVT_SCROLL_CHANGED,wxScrollEventHandler(FusionDialog::OnScrollSlide),NULL,this);
    p_sliderRatio->Connect(wxEVT_SCROLL_THUMBRELEASE,wxScrollEventHandler(FusionDialog::OnScrollSlide),NULL,this);
    p_sliderRatio->Connect(wxEVT_SCROLL_THUMBTRACK,wxScrollEventHandler(FusionDialog::OnScrollSlide),NULL,this);
    p_sliderRatio->Connect(wxEVT_SCROLL_PAGEDOWN,wxScrollEventHandler(FusionDialog::OnScrollSlide),NULL,this);
    p_sliderRatio->Connect(wxEVT_SCROLL_PAGEUP,wxScrollEventHandler(FusionDialog::OnScrollSlide),NULL,this);
    p_sliderRatio->Connect(wxEVT_SCROLL_LINEDOWN,wxScrollEventHandler(FusionDialog::OnScrollSlide),NULL,this);
    p_sliderRatio->Connect(wxEVT_SCROLL_LINEUP,wxScrollEventHandler(FusionDialog::OnScrollSlide),NULL,this);
    p_sliderRatio->Connect(wxEVT_SCROLL_BOTTOM,wxScrollEventHandler(FusionDialog::OnScrollSlide),NULL,this);
    p_sliderRatio->Connect(wxEVT_SCROLL_TOP,wxScrollEventHandler(FusionDialog::OnScrollSlide),NULL,this);
}


void FusionDialog::InitialisationFrame()
{
    HMONITOR hmonitor;
    MONITORINFO monitor_info;

    // On recupere les informations liees au monitor courant => Gestion du multi-moniteur
    hmonitor = MonitorFromWindow(AppToMerge[0],MONITOR_DEFAULTTONEAREST);
    monitor_info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(hmonitor,&monitor_info);

    cadre = monitor_info.rcWork;

    ShowWindow(AppToMerge[0],SW_SHOWNORMAL);
    ShowWindow(AppToMerge[1],SW_SHOWNORMAL);

    ResizeAt((cadre.right-cadre.left)/2);
    p_sliderRatio->SetValue(25);

    p_sliderRatio->SetFocus();
}


void FusionDialog::SetTabHwnd(HWND tab[2])
{
    AppToMerge[0] = tab[0];
    AppToMerge[1] = tab[1];
    InitialisationFrame();
}


void FusionDialog::OnScrollSlide(wxScrollEvent &event)
{
    double size;

    if (!p_checkHV->IsChecked())
        size = (cadre.right-cadre.left)*(1.+(p_sliderRatio->GetValue()-25.)/25.)/2.;
    else
        size = (cadre.bottom-cadre.top)*(1.+(p_sliderRatio->GetValue()-25.)/25.)/2.;

    ResizeAt((int)size);
}


void FusionDialog::ResizeAt(const int& pos)
{
    if (!p_checkHV->IsChecked())
    {
        SetWindowPos(AppToMerge[0],HWND_TOP,cadre.left,cadre.top,pos,cadre.bottom-cadre.top,
                     SWP_NOACTIVATE|SWP_SHOWWINDOW|SWP_NOZORDER);
        SetWindowPos(AppToMerge[1],HWND_TOP,cadre.left+pos,cadre.top,cadre.right-cadre.left-pos,cadre.bottom-cadre.top,
                     SWP_NOACTIVATE|SWP_SHOWWINDOW|SWP_NOZORDER);
    }
    else
    {
        SetWindowPos(AppToMerge[0],HWND_TOP,cadre.left,cadre.top,cadre.right-cadre.left,pos,
                     SWP_NOACTIVATE|SWP_SHOWWINDOW|SWP_NOZORDER);
        SetWindowPos(AppToMerge[1],HWND_TOP,cadre.left,cadre.top+pos,cadre.right-cadre.left,cadre.bottom-cadre.top-pos,
                     SWP_NOACTIVATE|SWP_SHOWWINDOW|SWP_NOZORDER);
    }
}


void FusionDialog::SwitchWindows(wxCommandEvent &event)
{
    double size;

    std::swap(AppToMerge[0],AppToMerge[1]);

    if (!p_checkHV->IsChecked())
        size=(cadre.right-cadre.left)*(1.+(p_sliderRatio->GetValue()-25.)/25.)/2.;
    else
        size=(cadre.bottom-cadre.top)*(1.+(p_sliderRatio->GetValue()-25.)/25.)/2.;

    ResizeAt(int(size));
}


void FusionDialog::OnButtonCentre(wxCommandEvent &event)
{
    if (!p_checkHV->IsChecked())
        ResizeAt((cadre.right-cadre.left)/2);
    else
        ResizeAt((cadre.bottom-cadre.top)/2);

    p_sliderRatio->SetValue(25);
}


void FusionDialog::HorizontalVertical(wxCommandEvent &event)
{
    double size;

    if (!p_checkHV->IsChecked())
        size = (cadre.right-cadre.left)*(1.+(p_sliderRatio->GetValue()-25.)/25.)/2.;
    else
        size = (cadre.bottom-cadre.top)*(1.+(p_sliderRatio->GetValue()-25.)/25.)/2.;

    ResizeAt(int(size));
}

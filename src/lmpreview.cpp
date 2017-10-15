
#include <wx/msw/registry.h>
#include <wx/filename.h>

#include "lmpreview.h"
#include "layout_screens.h"


LMPreview::LMPreview(wxWindow* parent) : wxPanel(parent,-1)
{
    wxScreenDC scrDC;

    wxSize screen_size = scrDC.GetSize();

    wxImage img;

    if (double(screen_size.GetWidth())/screen_size.GetHeight() > 1.5)
    {
        img = wxGetImage_Ecran_16x9();
        m_vp = wxRect(13,10,226,126);
    }
    else
    {
        img = wxGetImage_Ecran_4x3();
        m_vp = wxRect(12,10,171,126);
    }

    wxBitmap bmp(img.GetWidth(),img.GetHeight());
    wxMemoryDC mdc(bmp);
    wxColour col = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
    mdc.SetPen(wxPen(col));
    mdc.SetBrush(wxBrush(col));
    mdc.DrawRectangle(mdc.GetSize());
    mdc.DrawBitmap(wxBitmap(img),0,0,true);

    mdc.SelectObject(wxNullBitmap);

    // Chargement de l'image correspondant au fond d'écran
    wxRegKey rKey(_T("HKEY_CURRENT_USER\\Software\\Microsoft\\Internet Explorer\\Desktop\\General"));
    wxString sScrBg;
    rKey.QueryValue(_T("Wallpaper"),sScrBg);
    wxBitmap scrBmp;

    wxImage bgImg;

    bool bFileExist = wxFileName::FileExists(sScrBg);

    if(bFileExist)
        bgImg = wxImage(sScrBg,wxBITMAP_TYPE_ANY);

    if (bgImg.IsOk() && bFileExist)
        scrBmp = wxBitmap(bgImg.Scale(m_vp.width,m_vp.height,wxIMAGE_QUALITY_HIGH));
    else
        scrBmp = wxNullBitmap;

    wxBoxSizer* szr = new wxBoxSizer(wxVERTICAL);
    p_stBmp = new wxStaticBitmap(this,-1,bmp);
    szr->Add(p_stBmp,0,wxALL|wxCENTER,5);
    SetSizer(szr);
    szr->SetSizeHints(this);

    wxStaticBitmap* prnt;
    if (!scrBmp.IsOk())
    {
        prnt = p_stBmp;
    }
    else
    {
        p_stScr = new wxStaticBitmap(p_stBmp,-1,scrBmp,wxPoint(m_vp.x,m_vp.y));
        m_vp.x = 0;
        m_vp.y = 0;
        prnt = p_stScr;
    }

    p_Wnd = new wxPanel(prnt,wxID_ANY,wxDefaultPosition, wxDefaultSize, wxBORDER_RAISED);
    wxBoxSizer* wndSzr = new wxBoxSizer(wxVERTICAL);
    wxPanel* wndTitle = new wxPanel(p_Wnd,wxID_ANY,wxDefaultPosition, wxSize(-1,5));
    wndSzr->Add(wndTitle,0,wxALL|wxEXPAND,0);
    wndTitle->SetBackgroundColour(*wxBLUE);
    p_Wnd->SetSizer(wndSzr);
}

LMPreview::~LMPreview()
{
    //dtor
}

void LMPreview::SetWndPos(double dX, double dY, double dW, double dH)
{
    int x,y,w,h;
    x = int(round(dX*m_vp.width/100.));
    y = int(round(dY*m_vp.height/100.));
    w = int(round(dW*m_vp.width/100.));
    h = int(round(dH*m_vp.height/100.));

    x += m_vp.x;
    y += m_vp.y;

    p_Wnd->SetSize(x,y,w,h);
}


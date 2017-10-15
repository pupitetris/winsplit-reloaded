#ifndef LMPREVIEW_H_INCLUDED
#define LMPREVIEW_H_INCLUDED

#include <wx/wx.h>
#include <wx/image.h>

class LMPreview : public wxPanel
{
public:
    LMPreview(wxWindow *parent);
    virtual ~LMPreview();
    void SetWndPos(double dX, double dY, double dW,double dH);
private:
    wxStaticBitmap *p_stBmp,*p_stScr;
    wxPanel* p_Wnd;
    wxRect m_vp;
};

#endif // LMPREVIEW_H_INCLUDED

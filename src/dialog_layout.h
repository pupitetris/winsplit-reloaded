#ifndef __DIALOG_LAYOUT_H__
#define __DIALOG_LAYOUT_H__

#include <vector>
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/spinbutt.h>

#include "lmpreview.h"
#include "layout_manager.h"

class ChooseRatioDialog: public wxDialog
{
private:
	wxTextCtrl* p_txtCtrlX;
	wxTextCtrl* p_txtCtrlY;
	wxTextCtrl* p_txtCtrlWidth;
	wxTextCtrl* p_txtCtrlHeight;

	wxButton* p_btnOk;
	wxButton* p_btnCancel;

	void CreateControls();
	void CheckValidity (wxCommandEvent& event);

public:

	ChooseRatioDialog (wxWindow* parent = NULL,
	                   wxWindowID id = wxID_ANY,
	                   const wxString& title = _ ("Ratio in per cent") );

	~ChooseRatioDialog();

	bool IsValide();
	bool IsInside();

	void SetRatio (RatioRect ratio);
	RatioRect GetRatio();
};


class LayoutDialog: public wxDialog
{
private:
	std::vector<std::vector<RatioRect> > tab_seq_tmp;
	wxListCtrl* p_lstCtrlTask;
	wxListCtrl* p_lstCtrlCombo;
	wxButton* p_btnAdd;
	wxButton* p_btnEdit;
	wxButton* p_btnDelete;
	wxButton* p_btnOk;
	wxButton* p_btnCancel;
	wxStaticText* p_stc_x;
	wxStaticText* p_stc_y;
	wxStaticText* p_stc_width;
	wxStaticText* p_stc_height;
	wxSpinButton* p_spinBtnUpDown;
	wxPanel* p_panel_screen;
	wxPanel* p_panel_wnd;
	wxStaticBitmap* p_stcBmpWnd;
	wxStaticBitmap* p_stcBmpScreen;
	LMPreview* p_preview;

	long m_item_task_selected;
	long m_item_combo_selected;


	void CreateControls();
	void CreateConnections();
	void InitListCtrl();

	void RefreshLstCtrlCombo (bool select_first = true);
	void OnTaskChangeSelection (wxListEvent& event);
	void OnComboChangeSelection (wxListEvent& event);
	void OnActiveCombo (wxListEvent& event);
	void OnAddCombo (wxCommandEvent& event);
	void OnEditCombo (wxCommandEvent& event);
	void OnDeleteCombo (wxCommandEvent& event);
	void OnSpinUp (wxSpinEvent& event);
	void OnSpinDown (wxSpinEvent& event);
	void OnOKBtn (wxCommandEvent& event);

	void OnPaint (wxPaintEvent &event);

public:
	LayoutDialog (wxWindow* parent = NULL,
	              wxWindowID id = wxID_ANY,
	              const wxString& title = _ ("Layout settings") );

	~LayoutDialog();
};



#endif // __DIALOG_LAYOUT_H__


#ifndef DIALOG_ACTIVEWNDTOOLS_H_INCLUDED
#define DIALOG_ACTIVEWNDTOOLS_H_INCLUDED

#include <wx/slider.h>
#include <wx/filename.h>

class ActiveWndToolsDialog : public wxDialog
{
	public:
		ActiveWndToolsDialog(wxWindow* parent , HWND activeWindow);
		virtual ~ActiveWndToolsDialog();
		static void DeleteTempFiles();
		static void ShowDialog();
	protected:
	private:
		wxFileName m_sTmpFName;
		bool m_bIsDragging;
		HWND m_hActiveWindow;
		wxBitmap m_bmpWindow;
		wxStaticBitmap *p_stbPreview;
		wxButton *p_btnSaveAs, *p_btnClipboard;
		wxButton *p_btnAlwaysOnTop;
		wxStaticText *p_lblEnableTransp;
		wxCheckBox *p_chkEnableTransp;
		wxSlider *p_sldTranparency;
		bool m_bIsAlwaysOnTop, m_bIsTransparent;
		int m_iTranparency;
		void GetScreenShots();
		void CreateControls();
		void CreateConnexions();
		void OnSaveAsImageFile(wxCommandEvent& event);
		void OnCopyToClipboard(wxCommandEvent& event);
		void OnChangeTopMostStyle(wxCommandEvent& event);
		void OnEnableDisableTransparency(wxCommandEvent& event);
		void OnTransparencyChanged(wxCommandEvent& event);
		void OnCancel(wxCommandEvent& event);
		void OnLeftButtonDown(wxMouseEvent& event);
		void OnMouseMove(wxMouseEvent& event);
};

#endif // DIALOG_ACTIVEWNDTOOLS_H_INCLUDED

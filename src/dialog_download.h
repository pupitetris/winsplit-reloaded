#ifndef __DIALOG_DOWNLOAD_H__
#define __DIALOG_DOWNLOAD_H__


#include <wx/wx.h>
#include <wx/protocol/http.h>


class DownloadDialog;

class DownloadThread: public wxThread
{
private:
	DownloadDialog* p_parent;
	wxString m_fileDestination;
	wxString m_hostName;
	wxString m_hostFile;

	bool isFinished;
	wxHTTP http;

	long m_fileSize;
	long m_sizeRead;

public:

	DownloadThread (DownloadDialog* parent, const wxString& urlHost, const wxString& fileLocal);
	~DownloadThread() {};

	bool IsDownloadFinished();

	void* Entry();
};


class DownloadDialog: public wxDialog
{
private:
	wxStaticText* p_stcTextInfo;
	wxTextCtrl* p_txtFeatures;
	wxGauge* p_gauge;
	wxButton* p_btnNext;
	wxButton* p_btnCancel;
	wxStaticText* p_sctTextDownload;

	unsigned int m_stage;
	wxString m_url;
	wxString m_localPath;

	DownloadThread* p_thread;

	void CreateControls();
	void CreateConnexions();
	void IniText (const wxString& versionFileInfo);
	void OnNext (wxCommandEvent& event);
	void OnCancel (wxCommandEvent& event);
	void OnDownloaded (wxCommandEvent& event);

public:
	DownloadDialog (const wxString& versionFileInfo);
	~DownloadDialog();

	void SetProgressValue (unsigned int val);
};

#endif // __DIALOG_DOWNLOAD_H__


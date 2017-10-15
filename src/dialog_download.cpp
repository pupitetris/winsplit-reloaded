
#include <wx/wfstream.h>
#include <wx/sstream.h>
#include <wx/txtstrm.h>
#include <string>
#include <wx/statline.h>
#include <wx/filename.h>

#include "dialog_download.h"
#include "main.h"
#include "../images/icone.xpm"

DECLARE_EVENT_TYPE(wxEVT_DOWNLOADED_EVENT, -1)
DEFINE_EVENT_TYPE(wxEVT_DOWNLOADED_EVENT)

DownloadThread::DownloadThread(DownloadDialog* parent,const wxString& urlHost,const wxString& fileLocal)
:wxThread(wxTHREAD_JOINABLE),
p_parent(parent),
m_fileDestination(fileLocal),
m_hostName(),
m_hostFile(),
isFinished(false),
http(),
m_fileSize(0),
m_sizeRead(0)

{
    #ifdef wxUSE_UNICODE
    std::wstring buffer(urlHost.c_str());
    #else
    std::string buffer(urlHost.c_str());
    #endif

    size_t slashPos = buffer.find('/',8);
    m_hostName = buffer.substr(7,slashPos-7);
    m_hostFile = buffer.substr(slashPos);

    http.SetHeader(_T("Accept"),_T("*/*"));
	http.SetHeader(_T("User-Agent"),wxGetApp().GetAppName());

	http.SetTimeout(120);
}


bool DownloadThread::IsDownloadFinished()
{
    return isFinished;
}


void* DownloadThread::Entry()
{
	wxFileOutputStream f_out(m_fileDestination);

	wxChar szBuff[1025];

	if (http.Connect(m_hostName,80))
	{
		if (wxInputStream *stream = http.GetInputStream(m_hostFile))
		{
		    m_fileSize = stream->GetSize();

			stream->Read(szBuff,1024);
			while (stream->LastRead())
			{
				szBuff[stream->LastRead()]='\0';

				m_sizeRead += stream->LastRead();

				f_out.Write(szBuff,stream->LastRead());

				p_parent->SetProgressValue(m_sizeRead*100/m_fileSize);

				stream->Read(szBuff,1024);

				TestDestroy();
			}

			if(stream->GetLastError() == wxSTREAM_EOF)
			{
			    isFinished = true;

			    wxCommandEvent event(wxEVT_DOWNLOADED_EVENT,GetId());
                event.SetEventObject(NULL);
                event.SetString(_T("END_DOWNLOAD"));

                wxPostEvent(p_parent,event);
			}

			delete stream;
		}
	}

    f_out.Close();
}


DownloadDialog::DownloadDialog(const wxString& versionFileInfo)
:p_stcTextInfo(NULL),
p_txtFeatures(NULL),
p_gauge(NULL),
p_btnNext(NULL),
p_btnCancel(NULL),
p_sctTextDownload(NULL),
m_stage(0),
m_url(),
m_localPath(),
p_thread(NULL)
{
    Create(NULL,wxID_ANY,wxEmptyString);
    CreateControls();
    IniText(versionFileInfo);
    CreateConnexions();

    GetSizer()->RecalcSizes();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    SetIcon(wxIcon(icone_xpm));
}

DownloadDialog::~DownloadDialog()
{
    if(p_thread)
    {
        delete p_thread;
        p_thread = NULL;
    }
}


void DownloadDialog::CreateControls()
{
    p_stcTextInfo = new wxStaticText(this,wxID_ANY,wxEmptyString);
    p_txtFeatures = new wxTextCtrl(this,wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(350,100),
                            wxTE_READONLY|wxTE_MULTILINE|wxTE_LEFT|wxSTATIC_BORDER|wxTE_RICH);

    p_btnCancel = new wxButton(this,wxID_CANCEL,_("&Cancel"));

    wxString message;
    message << _("&Next") << _T(" >");

    p_btnNext = new wxButton(this,wxID_ANY,message);
    p_gauge = new wxGauge(this,wxID_ANY,100,wxDefaultPosition,wxSize(250,20));
    p_sctTextDownload = new wxStaticText(this,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxALIGN_LEFT);

    wxBoxSizer* vsizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(vsizer);

    wxBoxSizer* hsizer = new wxBoxSizer(wxHORIZONTAL);

    hsizer->Add(p_btnNext,0,wxALL|wxALIGN_CENTER,0);
    hsizer->AddSpacer(10);
    hsizer->Add(p_btnCancel,0,wxALL|wxALIGN_CENTER,0);

    vsizer->Add(p_stcTextInfo,0,wxALL|wxALIGN_CENTER,10);
    vsizer->Add(p_txtFeatures,0,wxALL|wxALIGN_CENTER,10);
    vsizer->Add(p_sctTextDownload,0,wxLEFT|wxALIGN_LEFT,10);
    vsizer->Add(p_gauge,0,wxALL|wxALIGN_CENTER,10);
    vsizer->Add(new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxSize(150,-1)),0,wxEXPAND|wxALL|wxALIGN_CENTER,10);
    vsizer->Add(hsizer,0,wxALL|wxALIGN_RIGHT,10);

    p_btnNext->SetFocus();
}

void DownloadDialog::IniText(const wxString& versionFileInfo)
{
    wxStringInputStream wxiss(versionFileInfo);
    wxTextInputStream textInputStream(wxiss);

    wxString message;
    wxString version;

    version = textInputStream.ReadLine();

    message << _("New version") << _T(" ") << version;

    SetLabel(message);

    message.Clear();

    message << _("A new version of WinSplit Revolution is available:") << _T(" ") << version << _T("\n\n");
    message << _("Press next to download the last version.") << _T("\n");
    message << _("You can visite the official website http://winsplit-revolution.com to download it.");

    p_stcTextInfo->SetLabel(message);

    m_url = textInputStream.ReadLine();

    message.Clear();

    message = textInputStream.ReadLine();

    while(!message.IsEmpty())
    {
        *p_txtFeatures << message << _T("\n");
        message = textInputStream.ReadLine();
    }

    p_txtFeatures->ShowPosition(0);
}


void DownloadDialog::CreateConnexions()
{
    p_btnNext->Connect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(DownloadDialog::OnNext),NULL,this);
    p_btnCancel->Connect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(DownloadDialog::OnCancel),NULL,this);
    Connect(wxEVT_DOWNLOADED_EVENT,wxCommandEventHandler(DownloadDialog::OnDownloaded),NULL,this);
}

void DownloadDialog::OnDownloaded(wxCommandEvent& event)
{
    p_btnNext->Enable(true);

    wxString message;

    message << _("Press finish to process installation");
    p_sctTextDownload->SetLabel(message);

    message.Clear();
    message << _("&Finish") << _T(" >");

    p_btnNext->SetLabel(message);
    p_btnNext->SetFocus();
}


void DownloadDialog::OnNext(wxCommandEvent& event)
{
    if(m_stage == 0)
    {
        wxString tmpfile = wxFileName::GetTempDir();

        m_localPath = tmpfile + _T("\\WinSplitInstall.exe");

        p_thread = new DownloadThread(this,m_url,m_localPath);

        p_thread->Create();
        p_thread->Run();
        p_btnNext->Enable(false);

        ++m_stage;
        return;
    }
    else // m_stage = 1
    {
        if(p_thread->IsDownloadFinished())
        {
            delete p_thread;
            p_thread = NULL;

            wxExecute(m_localPath);
            EndModal(wxOK);
        }
        else
        {
            wxMessageBox(_("Problem encountered during download stage"),_("Error"),wxICON_ERROR,this);
            EndModal(wxCANCEL);
        }
    }
}


void DownloadDialog::OnCancel(wxCommandEvent& event)
{
    if(p_thread)
    {
        if(p_thread->IsRunning())
        {
            p_thread->Pause();
            if(wxMessageBox(_("Are you sure you want to stop downloading the new version?"),_("Warning"),wxYES_NO,this) == wxYES)
            {
                p_thread->Delete();

                delete p_thread;
                p_thread = NULL;
            }
            else
            {
                p_thread->Resume();
                return;
            }
        }
        else
        {
            if(p_thread->IsDownloadFinished())
            {
                wxString message;
                message << _("You should press finish to process installation") << _T("\n") << _("Cancel anyway?");
                if(wxMessageBox(message,_("Message"),wxYES_NO,this) != wxYES)
                    return;
            }

            delete p_thread;
            p_thread = NULL;
        }
    }

    EndModal(wxCANCEL);
    event.Skip();
}


void DownloadDialog::SetProgressValue(unsigned int val)
{
    // Pour eviter les problemes de scintillement
    unsigned int val_previous = p_gauge->GetValue();

    if(val_previous != val)
    {
        wxString message;
        message << _("Downloading file: ") << wxString::Format(_T("%d"),val) << _T("%");
        p_sctTextDownload->SetLabel(message);
        p_gauge->SetValue(val);
    }
}


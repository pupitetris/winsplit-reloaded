

#include <windows.h>
#include <wx/sstream.h>
#include <wx/txtstrm.h>
#include <time.h>
#include <wx/protocol/http.h>

#include "update_thread.h"
#include "main.h"



ReadVersionThread::ReadVersionThread(const unsigned int& timeout)
:wxThread(wxTHREAD_JOINABLE),
m_options(SettingsManager::Get()),
m_flagForceChecking(false),
m_host_version(0.),
m_strVersion(),
m_timeout(timeout)
{
}

ReadVersionThread::~ReadVersionThread(){}

void* ReadVersionThread::Entry()
{
    if(!HaveToCheck())
        return NULL;

    wxHTTP http;
    char szBuff[1025];

    http.SetHeader(_T("Accept"),_T("*/*"));
	http.SetHeader(_T("User-Agent"),wxGetApp().GetAppName());
	http.SetTimeout(m_timeout);

	if (http.Connect(_T("winsplit-revolution.com"),80))
	{
		if (wxInputStream *stream = http.GetInputStream(_T("/Soft/LastVersion.txt")))
		{
			stream->Read(szBuff,1024);

			while (stream->LastRead())
			{
				szBuff[stream->LastRead()]='\0';

				m_strVersion += wxString::FromAscii(szBuff);

				stream->Read(szBuff,1024);
			}

            // Probleme survenu lors de la lecture
			if(stream->GetLastError() != wxSTREAM_EOF)
			{
			    m_strVersion = wxEmptyString;
			}
			else
			{
			    wxStringInputStream wxiss(m_strVersion);
                wxTextInputStream textInputStream(wxiss);

                wxString strTmp = textInputStream.ReadLine();

                strTmp.ToDouble(&m_host_version);
			}

			delete stream;
		}
	}
}


void ReadVersionThread::ForceChecking()
{
    m_flagForceChecking = true;
}

bool ReadVersionThread::HaveToCheck()
{
    time_t now, last, tmp;
    struct tm timeinfo_last;
    struct tm timeinfo_now;

    bool retour = false;

    if(m_flagForceChecking) return true;

    if(!m_options.hasToCheckForUpdates()) return false;
    if(m_options.getUpdateCheckFrequency() == CHECK_UPDATES_ON_START) return true;

    time(&now);
    tmp = now;
    last = m_options.getLastCheckDate();

    timeinfo_now = *(localtime(&now));
    timeinfo_last = *(localtime(&last));

    if((timeinfo_now.tm_mon != timeinfo_last.tm_mon)&&(m_options.getUpdateCheckFrequency() == CHECK_UPDATES_MONTHLY))
    {
        m_options.setLastCheckDate(tmp);
        return true;
    }

    if(m_options.getUpdateCheckFrequency() == CHECK_UPDATES_WEEKLY)
    {
        if((timeinfo_now.tm_mday == timeinfo_last.tm_mday)&&((timeinfo_now.tm_mon != timeinfo_last.tm_mon)))
        {
			m_options.setLastCheckDate(tmp);
			return true;
        }

        if((timeinfo_now.tm_mday-timeinfo_last.tm_mday) >= 7)
        {
            m_options.setLastCheckDate(tmp);
            return true;
        }

        if(timeinfo_now.tm_mday < timeinfo_last.tm_mday)
        {
            if((30+timeinfo_now.tm_mday-timeinfo_last.tm_mday) >= 7)
            {
                m_options.setLastCheckDate(tmp);
                return true;
            }
        }
    }
    return false;
}

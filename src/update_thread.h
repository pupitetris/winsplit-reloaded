#ifndef __WINSPLIR_UPDATES_H__
#define __WINSPLIR_UPDATES_H__

#include "settingsmanager.h"

class ReadVersionThread: public wxThread
{
private:
    SettingsManager &m_options;
    bool m_flagForceChecking;
    double m_host_version;
    wxString m_strVersion;
    unsigned int m_timeout;

public:
    // Contructeur de la classe
    ReadVersionThread(const unsigned int& timeout);
    ~ReadVersionThread();

    void ForceChecking();
    bool HaveToCheck();

    wxString GetFeatures(){return m_strVersion;}
    double GetHostVersion(){return m_host_version;}

    void* Entry();
};

#endif // __WINSPLIR_UPDATES_H__

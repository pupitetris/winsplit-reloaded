#include "settingsmanager.h"
// Définition de l'objet "singleton"
SettingsManager* SettingsManager::m_instance=NULL;

#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/msw/registry.h>

#include "main.h"
#include "virtual_key_manager.h"


const wxLanguage SettingsManager::AvailableLangIds[] =
{
    wxLANGUAGE_ENGLISH,
    wxLANGUAGE_FRENCH,
    wxLANGUAGE_ITALIAN,
    wxLANGUAGE_SPANISH,
    wxLANGUAGE_GERMAN,
    wxLANGUAGE_CATALAN,
    wxLANGUAGE_DUTCH,
    wxLANGUAGE_PORTUGUESE,
    wxLANGUAGE_CZECH,
    wxLANGUAGE_POLISH,
    wxLANGUAGE_CHINESE_TRADITIONAL,
    wxLANGUAGE_CHINESE_SIMPLIFIED
};

const wxString SettingsManager::AvailableLangNames[] =
{
    _T("English"),
    _T("Français"),
    _T("Italiano"),
    _T("Español"),
    _T("Deutsch"),
    _T("Català"),
    _T("Dutch"),
    _T("Português"),
    _T("Czech"),
    _T("Polish"),
    _T("Chinese Traditional"),
    _T("Chinese Simplified")
};

SettingsManager::SettingsManager()
{
	setlocale(LC_NUMERIC,"C");
    m_bInitialized=false;
    m_bPortableMode=false;
    m_bIsModified=false;
}

SettingsManager::~SettingsManager()
{
    //
}

SettingsManager &SettingsManager::Get()
{
    if (m_instance==NULL)
    {
        m_instance=new SettingsManager();
        m_instance->Initialize();
    }
    return *m_instance;
}

bool SettingsManager::IsOk()
{
    if (!m_bInitialized) return false;

    return true;
}

wxString SettingsManager::GetDataDirectory()
{
    return m_sUserDataDir;
}

wxString SettingsManager::GetUserName()
{
    return m_sUserName;
}

void SettingsManager::Kill()
{
    if (m_instance!=NULL)
    {
        if (m_instance->m_bIsModified)
            m_instance->SaveSettings();
    }
    delete m_instance;
    m_instance=NULL;
}

int SettingsManager::GetAvailableLanguagesCount()
{
    return (sizeof(AvailableLangIds)/sizeof(wxLanguage));
}

wxString SettingsManager::getAppPath()
{
    return m_sAppPath;
}

bool SettingsManager::getSaveNumpadPosOnExit()
{
    return m_bVN_SavePosOnExit;
}

void SettingsManager::setSaveNumpadPosOnExit(bool save)
{
    if (save!=m_bVN_SavePosOnExit)
    {
        m_bVN_SavePosOnExit=save;
        m_bIsModified=true;
    }
}

bool SettingsManager::getShowNumpadAtBoot()
{
    return m_bVN_ShownAtBoot;
}

void SettingsManager::setShowNumpadAtBoot(bool show)
{
    if (show!=m_bVN_ShownAtBoot)
    {
        m_bVN_ShownAtBoot=show;
        m_bIsModified=true;
    }
}

bool SettingsManager::IsPortable()
{
    return m_bPortableMode;
}

bool SettingsManager::AcceptTopMostWindows()
{
    return m_bAcceptTopmostWindows;
}

void SettingsManager::setAcceptTopMostWindows(bool do_it)
{
    if (do_it!=m_bAcceptTopmostWindows)
    {
        m_bAcceptTopmostWindows=do_it;
        m_bIsModified=true;
    }
}

bool SettingsManager::hasToShowHotkeysWarnings()
{
    return m_bShowHKWarnings;
}

void SettingsManager::setShowHotkeysWarnings(bool show)
{
    if (show!=m_bShowHKWarnings)
    {
        m_bShowHKWarnings=show;
        m_bIsModified=true;
    }
}

bool SettingsManager::hasToCheckForUpdates()
{
    return m_bCheckForUpdates;
}

void SettingsManager::setCheckForUpdates(bool check)
{
    if (check!=m_bCheckForUpdates)
    {
        m_bCheckForUpdates=check;
        m_bIsModified=true;
    }
}

time_t SettingsManager::getLastCheckDate()
{
    return m_tLastUpdateCheck;
}

void SettingsManager::setLastCheckDate(time_t lastdate)
{
    if (lastdate!=m_tLastUpdateCheck)
    {
        m_tLastUpdateCheck=lastdate;
        m_bIsModified=true;
    }
}

int SettingsManager::getUpdateCheckFrequency()
{
    return m_iUpdateCheckFrequency;
}

void SettingsManager::setUpdateCheckFrequency(int freqIndex)
{
    if (freqIndex!=m_iUpdateCheckFrequency)
    {
        m_iUpdateCheckFrequency=freqIndex;
        m_bIsModified=true;
    }
}

int SettingsManager::getLanguageIndex()
{
    return m_iLanguage;
}

void SettingsManager::setLanguageIndex(int index)
{
    if (index>=GetAvailableLanguagesCount())
    {
        wxMessageBox(_("Language non supported in WinSplit !"),_("Error"), wxICON_ERROR);
        return;
    }
    if (index!=m_iLanguage)
    {
        if (!m_locale.Init(AvailableLangIds[index], wxLOCALE_CONV_ENCODING))
        {
            wxMessageBox(_T("Unable to initialize language !"),_("Error"), wxICON_ERROR);
        }
        else
        {
            m_locale.AddCatalog(_T("winsplit"));
            m_iLanguage=index;
            m_bIsModified=true;
        }
    }
    setlocale(LC_NUMERIC,"C");
}

void SettingsManager::setAutoDeleteTempFiles(bool del)
{
	m_bAutoDelTmpFiles=del;
}

bool SettingsManager::getAutoDeleteTempFiles()
{
	return m_bAutoDelTmpFiles;
}

void SettingsManager::setAutoDeleteTime(int value)
{
	// la variable peut avoir les valeurs suivantes :
	// 0 = suppression des fichiers au démarrage de WinSplit
	// 1 = suppression des fichiers lorsque WinSplit se termine
	m_iAutoDelTime=value;
}

int SettingsManager::getAutoDeleteTime()
{
	return m_iAutoDelTime;
}

int SettingsManager::getNumpadTransparency()
{
    return m_iVN_Transparency;
}

void SettingsManager::setNumpadTransparency(int value)
{
    if (value!=m_iVN_Transparency)
    {
        m_iVN_Transparency=value;
        m_bIsModified=true;
    }
}

bool SettingsManager::getNumpadStyle()
{
    return m_bVN_Reduced;
}

void SettingsManager::setNumpadStyle(bool reduced)
{
    if (reduced!=m_bVN_Reduced)
    {
        m_bVN_Reduced=reduced;
        m_bIsModified=true;
    }
}

bool SettingsManager::getNumpadAutoHide()
{
    return m_bVN_AutoHide;
}

void SettingsManager::setNumpadAutoHide(bool autoHide)
{
    if (autoHide!=m_bVN_AutoHide)
    {
        m_bVN_AutoHide=autoHide;
        m_bIsModified=true;
    }
}

wxPoint SettingsManager::getNumpadPosition()
{
    return wxPoint(m_iVN_PosX, m_iVN_PosY);
}

void SettingsManager::setNumpadPosition(wxPoint pos)
{
    if ((pos.x!=m_iVN_PosX)||(pos.y!=m_iVN_PosY))
    {
        m_iVN_PosX=pos.x;
        m_iVN_PosY=pos.y;
        m_bIsModified=true;
    }
}

bool SettingsManager::IsDragNGoEnabled()
{
    return m_bDNG_Enabled;
}

void SettingsManager::EnableDragNGo(bool enable)
{
    if (enable!=m_bDNG_Enabled)
    {
        m_bDNG_Enabled=enable;
        m_bIsModified=true;
    }
}

int SettingsManager::getDnGTimerFrequency()
{
    return m_iDNG_timer;
}

void SettingsManager::setDnGTimerFrequency(int freq)
{
    if (freq!=m_iDNG_timer)
    {
        m_iDNG_timer=freq;
        m_bIsModified=true;
    }
}

int SettingsManager::getDnGDetectionRadius()
{
    return m_iDNG_Radius;
}

void SettingsManager::setDnGDetectionRadius(int radius)
{
    if (radius!=m_iDNG_Radius)
    {
        m_iDNG_Radius=radius;
        m_bIsModified=true;
    }
}

wxColour SettingsManager::getDnGZoneBgColor()
{
    return m_cDNG_BgColor;
}

void SettingsManager::setDnGZoneBgColor(wxColour col)
{
    if (col!=m_cDNG_BgColor)
    {
        m_cDNG_BgColor=col;
        m_bIsModified=true;
    }
}

wxColour SettingsManager::getDnGZoneFgColor()
{
    return m_cDNG_FgColor;
}

void SettingsManager::setDnGZoneFgColor(wxColour col)
{
    if (col!=m_cDNG_FgColor)
    {
        m_cDNG_FgColor=col;
        m_bIsModified=true;
    }
}

int SettingsManager::getDngZoneTransparency()
{
    return m_iDNG_Transparency;
}

void SettingsManager::setDnGZoneTransparency(int value)
{
    if (value!=m_iDNG_Transparency)
    {
        m_iDNG_Transparency=value;
        m_bIsModified=true;
    }
}

void SettingsManager::setDnGMod1(int value)
{
    if (value!=(int)m_modDNG1)
    {
        m_modDNG1 = value;
        m_bIsModified=true;
    }
}

void SettingsManager::setDnGMod2(int value)
{
    if (value!=(int)m_modDNG2)
    {
        m_modDNG2 = value;
        m_bIsModified=true;
    }
}

unsigned int SettingsManager::getDnGMod1()
{
    return m_modDNG1;
}

unsigned int SettingsManager::getDnGMod2()
{
    return m_modDNG2;
}

void SettingsManager::setDnGMod1(const unsigned int& mod)
{
    if (mod!=m_modDNG1)
    {
        m_modDNG1 = mod;
        m_bIsModified=true;
    }
}

void SettingsManager::setDnGMod2(const unsigned int& mod)
{
    if (mod!=m_modDNG2)
    {
        m_modDNG2 = mod;
        m_bIsModified=true;
    }
}

void SettingsManager::setXMouseActivation(bool enable)
{
    wxRegKey rKey(_T("HKEY_CURRENT_USER\\Control Panel\\Desktop"));
    wxMemoryBuffer mBuff;
    rKey.QueryValue(_T("UserPreferencesMask"), mBuff);
    unsigned char c=*(unsigned char*)(mBuff.GetData());
    if (enable)
        c |= 1;
    else
        c &= 0xfe;
    *(unsigned char*)(mBuff.GetData())=c;
    rKey.SetValue(_T("UserPreferencesMask"), mBuff);
}

bool SettingsManager::IsXMouseActivated()
{
    wxRegKey rKey(_T("HKEY_CURRENT_USER\\Control Panel\\Desktop"));
    wxMemoryBuffer mBuff;
    rKey.QueryValue(_T("UserPreferencesMask"), mBuff);
    char c=*(char*)(mBuff.GetData());
    return ((c&1)==1);
}

void SettingsManager::setAutoZOrderActivation(bool enable)
{
    wxRegKey rKey(_T("HKEY_CURRENT_USER\\Control Panel\\Desktop"));
    wxMemoryBuffer mBuff;
    rKey.QueryValue(_T("UserPreferencesMask"), mBuff);
    unsigned char c=*(unsigned char*)(mBuff.GetData());
    if (enable)
        c |= 0x40;
    else
        c &= 0xbf;
    *(unsigned char*)(mBuff.GetData())=c;
    rKey.SetValue(_T("UserPreferencesMask"), mBuff);
}

bool SettingsManager::IsAutoZOrderActivated()
{
    wxRegKey rKey(_T("HKEY_CURRENT_USER\\Control Panel\\Desktop"));
    wxMemoryBuffer mBuff;
    rKey.QueryValue(_T("UserPreferencesMask"), mBuff);
    char c=*(char*)(mBuff.GetData());
    return ((c & 0x40)== 0x40);
}

void SettingsManager::setAutoZOrderDelay(int value)
{
    wxRegKey rKey(_T("HKEY_CURRENT_USER\\Control Panel\\Desktop"));
    if (rKey.HasValue(_T("ActiveWndTrackTimeout")))
		rKey.SetValue(_T("ActiveWndTrackTimeout"), (long)value);
	else
		rKey.SetValue(_T("ActiveWndTrkTimeout"), (long)value);
}

int SettingsManager::getAutoZOrderDelay()
{
    wxRegKey rKey(_T("HKEY_CURRENT_USER\\Control Panel\\Desktop"));
    long lVal=0;
    if (rKey.HasValue(_T("ActiveWndTrackTimeout")))
		rKey.QueryValue(_T("ActiveWndTrackTimeout"), &lVal);
	else
		rKey.QueryValue(_T("ActiveWndTrkTimeout"), &lVal);
    return (int)lVal;
}

void SettingsManager::setMouseFollowWindow(bool value)
{
    if (value!=m_bMouseFollowWnd)
    {
        m_bMouseFollowWnd=value;
        m_bIsModified=true;
    }
}

bool SettingsManager::getMouseFollowWindow()
{
    return m_bMouseFollowWnd;
}

void SettingsManager::setMouseFollowOnlyWhenIn(bool value)
{
	if (value!=m_bMouseFollowOnlyWhenIn)
	{
		m_bMouseFollowOnlyWhenIn=value;
		m_bIsModified=true;
	}
}

bool SettingsManager::getMouseFollowOnlyWhenIn()
{
	return m_bMouseFollowOnlyWhenIn;
}

void SettingsManager::setMinMaxCycle(bool value)
{
	if (value!=m_bMinMaxCycle)
	{
		m_bMinMaxCycle=value;
		m_bIsModified=true;
	}
}

bool SettingsManager::getMinMaxCycle()
{
	return m_bMinMaxCycle;
}

void SettingsManager::Initialize()
{
    // On n'appelle pas deux fois cette méthode
    if (m_bInitialized) return;

    // Récupération du nom d'utilisateur
    m_sUserName=wxGetUserId();

    m_sUserDataDir = wxEmptyString;
    wxFileName FName(wxGetApp().argv[0]);
    m_sAppPath=FName.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR);
    // On se place dans le répertoire de l'application
    wxSetWorkingDirectory(m_sAppPath);
    // On regarde d'abord si le fichier de données existe dans le répertoire de l'application.
    FName.SetFullName(_T("Settings.xml"));
    // Si c'est le cas, on est en mode "portable"
    m_bPortableMode=FName.FileExists();
    if (!m_bPortableMode)
    {
        // Sinon, on est en mode "classique"
        FName.SetPath(wxStandardPaths::Get().GetUserDataDir());
        // On s'assure que le répertoire existe, sinon, on le crée
        if (!wxDirExists(FName.GetPath(wxPATH_GET_VOLUME)))
            wxMkdir(FName.GetPath(wxPATH_GET_VOLUME));
    }
    m_sUserDataDir=FName.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR);
    // Valeurs par défaut des variables
    // Avertir des erreurs avec les Hotkeys
    m_bShowHKWarnings=true;
    // Recherche auto des mises à jour
    m_bCheckForUpdates=true;
    m_iUpdateCheckFrequency=CHECK_UPDATES_ON_START;
    // Langue par défaut = celle du système
    m_iLanguage=wxLANGUAGE_DEFAULT;
    // On indique au système ou se trouvent les fichiers de langue
    wxLocale::AddCatalogLookupPathPrefix(m_sAppPath + _T("languages"));
    // On initialise la variable wxLocale avec la langue par défaut du système
    if (!m_locale.Init(wxLANGUAGE_DEFAULT, wxLOCALE_CONV_ENCODING))
        wxMessageBox(_T("Unable to set default language !"), _("Error"), wxICON_ERROR);;
    m_locale.AddCatalog(_T("winsplit"));
    // On en profite pour vérifier si la langue système correspond à une langue supportée par Winsplit
    int iCount=GetAvailableLanguagesCount();
    for (int i=0;i<iCount;i++)
    {
        if (AvailableLangIds[i]==m_locale.GetLanguage())
        {
            m_iLanguage=i;
            break;
        }
    }
    // Pour être certains d'avoir le point dans les nombres à virgule
    setlocale(LC_NUMERIC,"C");
    // Comportement envers les fichiers temporaires des captures d'écran
    m_bAutoDelTmpFiles=true; // On supprime automatiquement
    m_iAutoDelTime=0; // Suppression au démarrage de WinSplit
    // Position, style, comportement et transparence du Virtual Numpad
    m_iVN_Transparency=65;
    m_iVN_PosX=640;
    m_iVN_PosY=480;
    m_bVN_Reduced=false;
    m_bVN_AutoHide=false;
    m_bVN_ShownAtBoot=false;
    m_bVN_SavePosOnExit=true;
    // Ne pas prendre en compte les fenêtre "TopMost"
    m_bAcceptTopmostWindows=false;
    m_tLastUpdateCheck=time(NULL);

    // Paramètres du Drag'N'Go
    m_bDNG_Enabled=true;
    m_iDNG_Radius=100;
    m_iDNG_timer=100;
    m_cDNG_BgColor=wxColour(65,105,225);
    m_cDNG_FgColor=*wxWHITE;
    m_iDNG_Transparency=45;
    m_modDNG1 = 0x02;  //MOD_CONTROL
    m_modDNG2 = 0x01;  //MOD_ALT

    // Par défaut, activer l'option "La souris suit la fenêtre"
    m_bMouseFollowWnd=false;
    // Par défaut la souris ne suit la fenêtre que si elle est dans sa zone client
    m_bMouseFollowOnlyWhenIn=true;
    // Par défaut, les Hotkeys "Minimize" et "Maximize" gardent leur fonctionnement classique
    m_bMinMaxCycle=false;

    // On essaye de lire les options depuis le fichier xml
    LoadSettings();

    m_bInitialized=true;
}

void SettingsManager::LoadSettings()
{
    // Le nom du fichier contenant les options
    wxFileName fname(m_sUserDataDir + _T("Settings.xml"));
    // Si le fichier d'options n'existe pas
    if (!fname.FileExists())
    {
        // On fait en sorte que Winsplit le crée
        // Pour cela, on déclare les réglages par défaut comme ayant été modifiés
        m_bIsModified=true;
        return;
    }
    wxXmlDocument doc;
    wxXmlNode *node;
    wxString sValue;

    // On essaye de charger le fichier contenant les options
    doc.Load(fname.GetFullPath());
    if (!doc.IsOk())
    {
        // S'il y a eut une erreur au chargement, on fait en sorte
        // que les options soient ré-écrites
        m_bIsModified=true;
        return;
    }
    node=doc.GetRoot()->GetChildren();
    while (node)
    {
        if (node->GetName() == _T("General"))
            ReadGeneralSettings(node);
        else if (node->GetName() == _T("VirtualNumpad"))
            ReadPopupSettings(node);
        else if (node->GetName() == _T("WebUpdate"))
            ReadWebUpdateSettings(node);
        else if (node->GetName() == _T("Drag_N_Go"))
            ReadDragNGoSettings(node);
        else if (node->GetName() == _T("Miscellaneous"))
            ReadMiscSettings(node);

        node = node->GetNext();
    }
    m_bIsModified=false;
}

void SettingsManager::SaveSettings()
{
    wxXmlDocument doc;
    wxXmlNode *node, *root;
    wxXmlProperty *prop;

    root = new wxXmlNode(NULL,wxXML_ELEMENT_NODE, _T("WinSplit_Settings"));
    prop = new wxXmlProperty(_T("Version"),_T("1.0"));
    root->SetProperties(prop);

    root->AddChild(node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("General")));
    SaveGeneralSettings(node);

    node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("VirtualNumpad")));
    node = node->GetNext();
    SavePopupSettings(node);

    node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("WebUpdate")));
    node = node->GetNext();
    SaveWebUpdateSettings(node);

    node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("Drag_N_Go")));
    node = node->GetNext();
    SaveDragNGoSettings(node);

    node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("Miscellaneous")));
    node = node->GetNext();
    SaveMiscSettings(node);

    wxFileName fname(m_sUserDataDir + _T("Settings.xml"));
    if (fname.FileExists()) wxRemoveFile(fname.GetFullPath());

    doc.SetRoot(root);
    doc.Save(fname.GetFullPath(),2);
}

void SettingsManager::ReadGeneralSettings(wxXmlNode *container)
{
    wxXmlNode *node=container->GetChildren();
    wxString nodName, sValue;
    long l;

    while (node)
    {
        nodName=node->GetName();
        long l;
        if (nodName==_T("AcceptTopMostWindow"))
        {
            m_bAcceptTopmostWindows=(node->GetProperties()->GetValue()==_T("True"));
        }
        else if (nodName==_T("ShowHotKeysWarnings"))
        {
            m_bShowHKWarnings=(node->GetProperties()->GetValue()==_T("True"));
        }
        else if (nodName==_T("Language"))
        {
            if (node->GetProperties()->GetValue().ToLong(&l))
            {
                setLanguageIndex((int)l);
            }
        }
        else if (nodName==_T("TempFiles"))
        {
        	if (node->GetPropVal(_T("AutoDelete"), &sValue))
            {
                m_bAutoDelTmpFiles=(sValue==_T("True"));
            }
            if (node->GetPropVal(_T("AutoDelTime"), &sValue))
            {
                sValue.ToLong(&l);
                m_iAutoDelTime = int(l);
            }
        }

        node=node->GetNext();
    }
}

void SettingsManager::SaveGeneralSettings(wxXmlNode *container)
{
    wxXmlNode *node;
    container->AddChild(node=new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                           _T("AcceptTopMostWindow"), wxEmptyString,
                                           new wxXmlProperty(_T("Value"), m_bAcceptTopmostWindows?_T("True"):_T("False"))));

    node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                _T("ShowHotKeysWarnings"), wxEmptyString,
                                new wxXmlProperty(_T("Value"), m_bShowHKWarnings?_T("True"):_T("False"))));
    node=node->GetNext();

    node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                _T("Language"), wxEmptyString,
                                new wxXmlProperty(_T("Value"), wxString::Format(_T("%0d"), m_iLanguage))));
    node=node->GetNext();

    node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("TempFiles")));
    node=node->GetNext();
    node->AddProperty(_T("AutoDelete"), m_bAutoDelTmpFiles?_T("True"):_T("False"));
    node->AddProperty(_T("AutoDelTime"), wxString::Format(_T("%0d"), m_iAutoDelTime));
}

void SettingsManager::ReadPopupSettings(wxXmlNode *container)
{
    wxXmlNode *node=container->GetChildren();
    wxString nodName, sValue;
    long l;

    while (node)
    {
        nodName=node->GetName();
        if (nodName==_T("Position"))
        {
            if (node->GetPropVal(_T("X"), &sValue))
            {
                sValue.ToLong(&l);
                m_iVN_PosX = int(l);
            }
            if (node->GetPropVal(_T("Y"), &sValue))
            {
                sValue.ToLong(&l);
                m_iVN_PosY = int(l);
            }
        }
        else if (nodName==_T("Style"))
        {
            if (node->GetPropVal(_T("Reduced"), &sValue))
            {
                m_bVN_Reduced=(sValue==_T("True"));
            }
            if (node->GetPropVal(_T("Transparency"), &sValue))
            {
                sValue.ToLong(&l);
                m_iVN_Transparency=(int)l;
            }
        }
        else if (nodName==_T("Comportment"))
        {
            if (node->GetPropVal(_T("ShowAtBoot"), &sValue))
            {
                m_bVN_ShownAtBoot=(sValue==_T("True"));
            }
            if (node->GetPropVal(_T("SaveAtExit"), &sValue))
            {
                m_bVN_SavePosOnExit=(sValue==_T("True"));
            }
            if (node->GetPropVal(_T("AutoHide"), &sValue))
            {
                m_bVN_AutoHide=(sValue==_T("True"));
            }
        }

        node=node->GetNext();
    }
}

void SettingsManager::SavePopupSettings(wxXmlNode *container)
{
    wxXmlNode *node;

    container->AddChild(node=new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("Position")));
    node->AddProperty(_T("X"), wxString::Format(_T("%0d"), m_iVN_PosX));
    node->AddProperty(_T("Y"), wxString::Format(_T("%0d"), m_iVN_PosY));

    node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("Style")));
    node=node->GetNext();
    node->AddProperty(_T("Reduced"), m_bVN_Reduced?_T("True"):_T("False"));
    node->AddProperty(_T("Transparency"), wxString::Format(_T("%0d"), m_iVN_Transparency));

    node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("Comportment")));
    node=node->GetNext();
    node->AddProperty(_T("ShowAtBoot"), (m_bVN_ShownAtBoot?_T("True"):_T("False")));
    node->AddProperty(_T("SaveAtExit"), (m_bVN_SavePosOnExit?_T("True"):_T("False")));
    wxString sTmp=(m_bVN_AutoHide?_T("True"):_T("False"));
    node->AddProperty(_T("AutoHide"), sTmp);
}

void SettingsManager::ReadWebUpdateSettings(wxXmlNode *container)
{
    wxXmlNode *node=container->GetChildren();
    wxString nodName, sValue;
    long l;

    while (node)
    {
        nodName=node->GetName();
        if (nodName==_T("AutoCheck"))
        {
            m_bCheckForUpdates=(node->GetProperties()->GetValue()==_T("True"));
        }
        else if (nodName==_T("Frequency"))
        {
            if (node->GetProperties()->GetValue().ToLong(&l))
                m_iUpdateCheckFrequency=l;
        }
        else if (nodName==_T("LastCheck"))
        {
            if (node->GetProperties()->GetValue().ToLong(&l))
                m_tLastUpdateCheck=l;
        }

        node=node->GetNext();
    }
}

void SettingsManager::SaveWebUpdateSettings(wxXmlNode *container)
{
    wxXmlNode *node;

    container->AddChild(node=new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("AutoCheck")));
    node->AddProperty(_T("Value"), m_bCheckForUpdates?_T("True"):_T("False"));

    node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("Frequency")));
    node=node->GetNext();
    node->AddProperty(_T("Value"), wxString::Format(_T("%0d"), m_iUpdateCheckFrequency));

    node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("LastCheck")));
    node=node->GetNext();
    node->AddProperty(_T("Value"), wxString::Format(_T("%0ld"), m_tLastUpdateCheck));
}

void SettingsManager::ReadDragNGoSettings(wxXmlNode *container)
{
    wxXmlNode *node=container->GetChildren();
    wxString nodName, sValue;
    long l;
    VirtualModifierManager modManager;

    while (node)
    {
        nodName=node->GetName();
        if (nodName==_T("Enabled"))
        {
            m_bDNG_Enabled = (node->GetProperties()->GetValue()==_T("True"));
        }
        else if (nodName==_T("TimerFrequency"))
        {
            if (node->GetProperties()->GetValue().ToLong(&l))
                m_iDNG_timer=l;
        }
        else if (nodName==_T("DetectionRadius"))
        {
            if (node->GetProperties()->GetValue().ToLong(&l))
                m_iDNG_Radius=l;
        }
        else if (nodName==_T("ZoneColors"))
        {
            if (node->GetPropVal(_T("Background"), &sValue))
                m_cDNG_BgColor.Set(sValue);

            if (node->GetPropVal(_T("Foreground"), &sValue))
                m_cDNG_FgColor.Set(sValue);
        }
        else if (nodName==_T("Transparency"))
        {
            if (node->GetProperties()->GetValue().ToLong(&l))
                m_iDNG_Transparency=l;
        }
        else if (nodName==_T("Modifiers"))
        {
            if (node->GetPropVal(_T("Modifier1"), &sValue))
                m_modDNG1 = modManager.GetValueFromString(sValue);

            if (node->GetPropVal(_T("Modifier2"), &sValue))
                m_modDNG2 = modManager.GetValueFromString(sValue);
        }

        node=node->GetNext();
    }
}

void SettingsManager::SaveDragNGoSettings(wxXmlNode *container)
{
    wxXmlNode *node;
    wxString strMod;
    VirtualModifierManager modManager;

    container->AddChild(node=new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("Enabled")));
    node->AddProperty(_T("Value"), m_bDNG_Enabled?_T("True"):_T("False"));

    node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("TimerFrequency")));
    node=node->GetNext();
    node->AddProperty(_T("Value"), wxString::Format(_T("%0d"), m_iDNG_timer));

    node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("DetectionRadius")));
    node=node->GetNext();
    node->AddProperty(_T("Value"), wxString::Format(_T("%0d"), m_iDNG_Radius));

    node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("ZoneColors")));
    node=node->GetNext();
    node->AddProperty(_T("Background"), m_cDNG_BgColor.GetAsString(wxC2S_HTML_SYNTAX));
    node->AddProperty(_T("Foreground"), m_cDNG_FgColor.GetAsString(wxC2S_HTML_SYNTAX));

    node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("Transparency")));
    node=node->GetNext();
    node->AddProperty(_T("Value"), wxString::Format(_T("%0d"), m_iDNG_Transparency));

    node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("Modifiers")));
    node=node->GetNext();

    strMod = modManager.GetStringFromValue(m_modDNG1);
    node->AddProperty(_T("Modifier1"), strMod);

    strMod = modManager.GetStringFromValue(m_modDNG2);
    node->AddProperty(_T("Modifier2"), strMod);
}

void SettingsManager::ReadMiscSettings(wxXmlNode *container)
{
    wxXmlNode *node=container->GetChildren();
    wxString nodName, sValue;
    long l;

    while (node)
    {
        nodName=node->GetName();
        if (nodName==_T("MouseFollowWindow"))
        {
            m_bMouseFollowWnd=(node->GetProperties()->GetValue()==_T("True"));
        }
        else if (nodName==_T("MouseFollowWindowOnlyWhenIn"))
        {
        	m_bMouseFollowOnlyWhenIn=(node->GetProperties()->GetValue()==_T("True"));
        }
        else if (nodName==_T("MinMaxCycle"))
        {
        	m_bMinMaxCycle=(node->GetProperties()->GetValue()==_T("True"));
        }

        node=node->GetNext();
    }
}

void SettingsManager::SaveMiscSettings(wxXmlNode *container)
{
    wxXmlNode *node;

    container->AddChild(node=new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _T("MouseFollowWindow")));
    node->AddProperty(_T("Value"), m_bMouseFollowWnd?_T("True"):_T("False"));

    node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                _T("MouseFollowWindowOnlyWhenIn"), wxEmptyString,
                                new wxXmlProperty(_T("Value"), m_bMouseFollowOnlyWhenIn?_T("True"):_T("False"))));
    node=node->GetNext();

    node->SetNext(new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                _T("MinMaxCycle"), wxEmptyString,
                                new wxXmlProperty(_T("Value"), m_bMinMaxCycle?_T("True"):_T("False"))));
}

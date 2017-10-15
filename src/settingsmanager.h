#ifndef SETTINGSMANAGER_H_INCLUDED
#define SETTINGSMANAGER_H_INCLUDED

#include <time.h>
#include <wx/xml/xml.h>
enum
{
	CHECK_UPDATES_ON_START=1,
	CHECK_UPDATES_WEEKLY,
	CHECK_UPDATES_MONTHLY
};

class SettingsManager
{
	public:
		SettingsManager();
		virtual ~SettingsManager();
		// Création / récupération de l'objet unique
		static SettingsManager& Get();
		// Destruction de l'objet unique
		static void Kill();
		// Vérification de l'intégrité de l'objet
		bool IsOk();
		// Obtention du répertoire dans lequel stocker les données
		wxString GetDataDirectory();
		// Obtention du nom d'utilisateur
		wxString GetUserName();
		// Est-on en mode "Portable" ou "Classique" ?
		bool IsPortable();

		// Accesseurs
		wxString getAppPath();
		// Options générales
		bool AcceptTopMostWindows();
		void setAcceptTopMostWindows(bool do_it);
		bool hasToShowHotkeysWarnings();
		void setShowHotkeysWarnings(bool show);
		int getLanguageIndex();
		void setLanguageIndex(int index);
		void setAutoDeleteTempFiles(bool del);
		bool getAutoDeleteTempFiles();
		void setAutoDeleteTime(int value);
		int getAutoDeleteTime();
		// Virtual Numpad
		int getNumpadTransparency();
		void setNumpadTransparency(int value);
		bool getNumpadStyle(); // Reduit ou non
		void setNumpadStyle(bool reduced);
		bool getNumpadAutoHide();
		void setNumpadAutoHide(bool autoHide);
		bool getSaveNumpadPosOnExit();
		void setSaveNumpadPosOnExit(bool save);
		bool getShowNumpadAtBoot();
		void setShowNumpadAtBoot(bool show);
		wxPoint getNumpadPosition();
		void setNumpadPosition(wxPoint pos);
		// Web Update
		bool hasToCheckForUpdates();
		void setCheckForUpdates(bool check);
		time_t getLastCheckDate();
		void setLastCheckDate(time_t lastdate);
		int getUpdateCheckFrequency();
		void setUpdateCheckFrequency(int freqIndex);
		// Drag'N'Go
		bool IsDragNGoEnabled();
		void EnableDragNGo(bool enable);
		int getDnGTimerFrequency();
		void setDnGTimerFrequency(int freq);
		int getDnGDetectionRadius();
		void setDnGDetectionRadius(int radius);
		wxColour getDnGZoneBgColor();
		void setDnGZoneBgColor(wxColour col);
		wxColour getDnGZoneFgColor();
		void setDnGZoneFgColor(wxColour col);
		int getDngZoneTransparency();
		void setDnGZoneTransparency(int value);
		void setDnGMod1(int value);
		void setDnGMod2(int value);
		unsigned int getDnGMod1();
		unsigned int getDnGMod2();
		void setDnGMod1(const unsigned int& mod);
		void setDnGMod2(const unsigned int& mod);
		// Divers
		void setXMouseActivation(bool enable);
		bool IsXMouseActivated();
		void setAutoZOrderActivation(bool enable);
		bool IsAutoZOrderActivated();
		void setAutoZOrderDelay(int value);
		int getAutoZOrderDelay();
		void setMouseFollowWindow(bool value);
		bool getMouseFollowWindow();
		void setMouseFollowOnlyWhenIn(bool value);
		bool getMouseFollowOnlyWhenIn();
		void setMinMaxCycle(bool value);
		bool getMinMaxCycle();

		void Initialize();
		void LoadSettings();
		void SaveSettings();
		static const wxLanguage AvailableLangIds[];
		static const wxString AvailableLangNames[];
		int GetAvailableLanguagesCount();
	protected:
	private:
        void ReadGeneralSettings(wxXmlNode *container);
        void SaveGeneralSettings(wxXmlNode *container);
        void ReadPopupSettings(wxXmlNode *container);
        void SavePopupSettings(wxXmlNode *container);
        void ReadWebUpdateSettings(wxXmlNode *container);
        void SaveWebUpdateSettings(wxXmlNode *container);
        void ReadDragNGoSettings(wxXmlNode *container);
        void SaveDragNGoSettings(wxXmlNode *container);
        void ReadMiscSettings(wxXmlNode *container);
        void SaveMiscSettings(wxXmlNode *container);
		wxLocale m_locale;
		wxString m_sAppPath, m_sUserDataDir, m_sUserName;
		bool m_bPortableMode, m_bInitialized, m_bIsModified;
		static SettingsManager *m_instance;

		// General settings
		bool m_bShowHKWarnings, m_bAcceptTopmostWindows;
		int m_iLanguage;
		bool m_bAutoDelTmpFiles;
		int m_iAutoDelTime;
		// VirtualNumpad Settings
		bool m_bVN_Reduced, m_bVN_AutoHide, m_bVN_ShownAtBoot, m_bVN_SavePosOnExit;
		int m_iVN_PosX, m_iVN_PosY, m_iVN_Transparency;
		// WebUpdate Settings
		bool m_bCheckForUpdates;
		time_t m_tLastUpdateCheck;
		int m_iUpdateCheckFrequency;
		// Drag'N'Go Settings
		bool m_bDNG_Enabled;
		int m_iDNG_timer, m_iDNG_Radius;
		wxColour m_cDNG_BgColor, m_cDNG_FgColor;
		int m_iDNG_Transparency;
		unsigned int m_modDNG1;
		unsigned int m_modDNG2;
		// Misc Settings
		bool m_bMouseFollowWnd;
		bool m_bMouseFollowOnlyWhenIn;
		bool m_bMinMaxCycle;
};

#endif // SETTINGSMANAGER_H_INCLUDED

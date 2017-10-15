# Décommenter la ligne suivante pour gagner du temps lors des différents tests
#SetCompress off
SetCompressor lzma

# Modifier la ligne ci-dessous pour qu'elle pointe vers le répertoire "bin" après compilation du soft et de sa dll
!define SRC_BIN_DIRECTORY "..\bin"

!include "InstallOptions.nsh"

# Variables diverses
Var PageInstallModeIniFile
Var InstallMode
Var OldVersionAutoRun
Var PortableModeForced
Var InstallInProcess
#Var DecimalBugIniFile

!include "MUI2.nsh"
!include "LogicLib.nsh"
!include "nsDialogs.nsh"
!include "WinMessages.nsh"

!define PRODUCT_VERSION "9.02"
!define PRODUCT_PUBLISHER "Raphael Lencrerot"
!define PRODUCT_WEB_SITE "http://winsplit-revolution.com/"
!define CHANGELOG_LINK "http://winsplit-revolution.com/changelog"
!define SOFTWARE_REGKEY "Software\WinSplit Revolution"
!define UNINSTALL_REGKEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\WinSplit Revolution"

!include "files\NSIS_Functions.nsh"

Name "WinSplit-Revolution"
OutFile "WinSplit-Revolution-v${PRODUCT_VERSION}.exe"
# Dossier d'installation par défaut
InstallDir "$PROGRAMFILES\WinSplit Revolution"
# Récupérer le dossier d'installation depuis la base de registre si disponible
InstallDirRegKey HKLM "${SOFTWARE_REGKEY}" "path"

# Clefs de registre pour le stockage de la langue utilisée lors de l'installation
!define MUI_LANGDLL_REGISTRY_ROOT HKLM
!define MUI_LANGDLL_REGISTRY_KEY "${SOFTWARE_REGKEY}" 
!define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

!define MUI_ABORTWARNING

# Icônes pour l'installateur et le désinstallateur
!define MUI_ICON "files\icone_install.ico"
!define MUI_UNICON "files\icone_desinstall.ico"

!insertmacro MUI_DEFAULT MUI_WELCOMEFINISHPAGE_BITMAP "files\image_install.bmp"
!insertmacro MUI_DEFAULT MUI_UNWELCOMEFINISHPAGE_BITMAP "files\image_install.bmp"

!define MUI_CUSTOMFUNCTION_GUIINIT MyOnGuiInit
# Différentes pages de l'installateur
!insertmacro MUI_PAGE_WELCOME
#Page custom DecimalBugPageEnter DecimalBugPageLeave
!insertmacro MUI_PAGE_LICENSE $(MyLicense)
Page custom InstallModePageEnter InstallModePageLeave
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
	#!define MUI_FINISHPAGE_NOAUTOCLOSE
	!define MUI_FINISHPAGE_RUN
	!define MUI_FINISHPAGE_RUN_TEXT $(AutoLaunch)
	!define MUI_FINISHPAGE_RUN_FUNCTION "FinishPageRunFunction"
	!define MUI_FINISHPAGE_SHOWREADME
	!define MUI_FINISHPAGE_SHOWREADME_TEXT $(StartWinSplitNow)
	!define MUI_FINISHPAGE_SHOWREADME_FUNCTION "FinishPageShowReadmeFunction"
	!define MUI_FINISHPAGE_LINK ${PRODUCT_WEB_SITE}
	!define MUI_FINISHPAGE_LINK_LOCATION ${PRODUCT_WEB_SITE}
	!define MUI_PAGE_CUSTOMFUNCTION_SHOW SetFinishPageOptions
!insertmacro MUI_PAGE_FINISH

Function SetFinishPageOptions
	StrCmp $InstallMode "1" LetItChecked
	SendMessage $mui.FinishPage.Run ${BM_SETCHECK} ${BST_UNCHECKED} 0
	LetItChecked:
FunctionEnd

!define MUI_CUSTOMFUNCTION_UNGUIINIT un.MyOnGuiInit
# Différentes pages du désinstallateur
	!define MUI_WELCOMEPAGE_TITLE_3LINES
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH
  
# Langues disponibles pour l'installation
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "English"
# Fichiers contenant la  licence en fonction de la langue choisie
LicenseLangString MyLicense ${LANG_ENGLISH} "files\lic_eng.rtf"
LicenseLangString MyLicense ${LANG_FRENCH} "files\lic_eng.rtf"

Section "Install"
	StrCpy $InstallInProcess "0"
	SetOutPath "$INSTDIR"
		File "${SRC_BIN_DIRECTORY}\WinSplit.exe"
		File "${SRC_BIN_DIRECTORY}\winsplithook.dll"
		!insertmacro CreateInternetShortcut "$INSTDIR\Changelog.url" ${CHANGELOG_LINK} "" ""
	SetOutPath "$INSTDIR\images"
		File "${SRC_BIN_DIRECTORY}\images\about.png"
		File "${SRC_BIN_DIRECTORY}\images\auto_start_false.png"
		File "${SRC_BIN_DIRECTORY}\images\auto_start_true.png"
		File "${SRC_BIN_DIRECTORY}\images\ecran_winsplit.png"
		File "${SRC_BIN_DIRECTORY}\images\exit.png"
		File "${SRC_BIN_DIRECTORY}\images\help.png"
		File "${SRC_BIN_DIRECTORY}\images\hotkeys.png"
		File "${SRC_BIN_DIRECTORY}\images\layout.png"
		File "${SRC_BIN_DIRECTORY}\images\options.png"
	SetOutPath "$INSTDIR\languages\ca"
		File "${SRC_BIN_DIRECTORY}\languages\ca\winsplit.mo"
	SetOutPath "$INSTDIR\languages\cs"
		File "${SRC_BIN_DIRECTORY}\languages\cs\winsplit.mo"
	SetOutPath "$INSTDIR\languages\de"
		File "${SRC_BIN_DIRECTORY}\languages\de\winsplit.mo"
	SetOutPath "$INSTDIR\languages\es"
		File "${SRC_BIN_DIRECTORY}\languages\es\winsplit.mo"
	SetOutPath "$INSTDIR\languages\fr"
		File "${SRC_BIN_DIRECTORY}\languages\fr\winsplit.mo"
	SetOutPath "$INSTDIR\languages\it"
		File "${SRC_BIN_DIRECTORY}\languages\it\winsplit.mo"
	SetOutPath "$INSTDIR\languages\nl"
		File "${SRC_BIN_DIRECTORY}\languages\nl\winsplit.mo"
	SetOutPath "$INSTDIR\languages\pl"
		File "${SRC_BIN_DIRECTORY}\languages\pl\winsplit.mo"
	SetOutPath "$INSTDIR\languages\pt"
		File "${SRC_BIN_DIRECTORY}\languages\pt\winsplit.mo"
	SetOutPath "$INSTDIR\languages\zh_TW"
		File "${SRC_BIN_DIRECTORY}\languages\zh_TW\winsplit.mo"
	SetOutPath "$INSTDIR\languages\zh_CN"
		File "${SRC_BIN_DIRECTORY}\languages\zh_TW\winsplit.mo"
	StrCmp $InstallMode "2" 0 no_setting_file
		SetOutPath "$INSTDIR"
		File "files\Settings.xml"
		Goto no_more_stuff
	no_setting_file:
		WriteUninstaller "$INSTDIR\Uninstall.exe"
		WriteRegStr HKLM "${UNINSTALL_REGKEY}" "DisplayName" "WinSplit Revolution (v${PRODUCT_VERSION})"
		WriteRegStr HKLM "${UNINSTALL_REGKEY}" "UninstallString" "$INSTDIR\Uninstall.exe"
		WriteRegStr HKLM "${UNINSTALL_REGKEY}" "DisplayIcon" "$INSTDIR\WinSplit.exe"
		WriteRegStr HKLM "${UNINSTALL_REGKEY}" "DisplayVersion" "${PRODUCT_VERSION}"
		WriteRegStr HKLM "${UNINSTALL_REGKEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
		WriteRegStr HKLM "${UNINSTALL_REGKEY}" "Publisher" "${PRODUCT_PUBLISHER}"
		WriteRegDWORD HKLM "${UNINSTALL_REGKEY}" "NoModify" 0x1
		WriteRegDWORD HKLM "${UNINSTALL_REGKEY}" "NoRepair" 0x1
		# Autres options : http://nsis.sourceforge.net/Add_uninstall_information_to_Add/Remove_Programs
		CreateDirectory "$SMPROGRAMS\WinSplit Revolution"
		CreateShortCut "$SMPROGRAMS\WinSplit Revolution\WinSplit Revolution.lnk" "$INSTDIR\WinSplit.exe"
		!insertmacro CreateInternetShortcut "$SMPROGRAMS\WinSplit Revolution\ChangeLog.url" ${CHANGELOG_LINK} "" ""
		CreateShortCut "$SMPROGRAMS\WinSplit Revolution\$(Uninstall_LinkName).lnk" "$INSTDIR\Uninstall.exe"
		WriteRegStr HKLM "${SOFTWARE_REGKEY}" "path" $INSTDIR
		WriteRegStr HKLM "${SOFTWARE_REGKEY}" "version" ${PRODUCT_VERSION}
	no_more_stuff:
SectionEnd

Section "Uninstall"
	RMDir /r "$INSTDIR\languages\ca"
	RMDir /r "$INSTDIR\languages\cs"
	RMDir /r "$INSTDIR\languages\de"
	RMDir /r "$INSTDIR\languages\es"
	RMDir /r "$INSTDIR\languages\fr"
	RMDir /r "$INSTDIR\languages\it"
	RMDir /r "$INSTDIR\languages\nl"
	RMDir /r "$INSTDIR\languages\pl"
	RMDir /r "$INSTDIR\languages\pt"
	RMDir /r "$INSTDIR\languages\zh_TW"
	RMDir /r "$INSTDIR\languages\zh_CN"
	RMDir /r "$INSTDIR\images"
	RMDir /r "$SMPROGRAMS\WinSplit Revolution"
	Delete "$INSTDIR\Uninstall.exe"
	RMDir /r "$INSTDIR"
	DeleteRegValue HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Run" "Winsplit"
	DeleteRegValue HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Run" "Winsplit"
	DeleteRegValue HKLM "${SOFTWARE_REGKEY}" "version"
	DeleteRegValue HKLM "${SOFTWARE_REGKEY}" "Installer Language"
	DeleteRegKey HKLM "${SOFTWARE_REGKEY}"
	DeleteRegKey HKLM "${UNINSTALL_REGKEY}"
SectionEnd

Function DetectInstalledVersion
	# Comme cette fonction est appelée depuis le "OnInit", la langue n'est
	# pas encore initialisée, il faut donc se palucher la traduction à la main.
	ReadRegStr $R0 HKLM "${UNINSTALL_REGKEY}" "DisplayVersion"
	StrCmp $R0 "" 0 version_detected
	Push 'ok'
	Goto main_detection_end
	version_detected:
		StrCmp $R0 "1.9" 0 not_version_1_9
		MessageBox MB_YESNOCANCEL $(OldVersionDetected) /SD IDYES IDNO force_portable_mode IDCANCEL abort_install
		Call RemoveVersion1_9
		Push 'ok'
		Goto main_detection_end
	not_version_1_9:
		StrCmp $R0 "8.06" new_old_version
		StrCmp $R0 "8.07" new_old_version
		Goto not_old_version
	new_old_version:
		StrCpy $R0 "$(OldVersionDetected)$\n$\n$(SettingsKeeped)"
		MessageBox MB_YESNOCANCEL $R0 /SD IDYES IDNO force_portable_mode IDCANCEL abort_install
		Call RemoveVersion8_xx
		Push 'ok'
		Goto main_detection_end
	not_old_version:
		StrCmp $R0 "${PRODUCT_VERSION}" 0 not_current_version
		MessageBox MB_OKCANCEL $(CurrentVersionDetected) IDOK force_portable_mode IDCANCEL abort_install
	not_current_version:
		MessageBox MB_OK $(UnknownVersionDetected)
		Goto abort_install
	force_portable_mode:
		Push 'portable'
		Goto main_detection_end
	abort_install:
		Abort
	main_detection_end:
FunctionEnd

Function .onInit
	StrCpy $InstallInProcess "0"
	InitPluginsDir
	GetTempFileName $PageInstallModeIniFile $PLUGINSDIR
	File /oname=$PageInstallModeIniFile "files\InstallMode.ini"
#	GetTempFileName $DecimalBugIniFile $PLUGINSDIR
#	File /oname=$DecimalBugIniFile "files\DecimalBug.ini"
	!insertmacro MUI_LANGDLL_DISPLAY	
	StrCpy $InstallMode "1"
	StrCpy $OldVersionAutoRun "0"
	StrCpy $PortableModeForced "0"
FunctionEnd

Function un.onInit
	!insertmacro MUI_UNGETLANGUAGE
FunctionEnd

!include "files\Language_Strings.nsh"

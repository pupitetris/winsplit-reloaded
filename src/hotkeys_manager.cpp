
#include <wx/xml/xml.h>

#include "hotkeys_manager.h"
#include "fonctions_resize.h"
#include "fonctions_speciales.h"
#include "virtual_key_manager.h"
#include "multimonitor_move.h"
#include "dialog_activewndtools.h"

const wxString xml_str[]=
{
    _T("AutoPlacement"),
    _T("LeftBottom"),
    _T("Bottom"),
    _T("RightBottom"),
    _T("Left"),
    _T("FullScreen"),
    _T("Right"),
    _T("LeftTop"),
    _T("Top"),
    _T("RightTop"),
    _T("CloseAllWindows"),
    _T("MosaicMode"),
    _T("FusionMode"),
    _T("WindowToLeftScreen"),
    _T("WindowToRigthScreen"),
    _T("MinimizeWindow"),
    _T("RestoreMinimizedWindow"),
    _T("ActiveWindowTools"),
    _T("AlwaysOnTop"),
    _T("MaximizeHorizontally"),
    _T("MaximizeVertically")
};

HotkeysManager::HotkeysManager()
:wxFrame(NULL,-1,wxEmptyString),
m_minimizeRestore(),
m_options(SettingsManager::Get())
{
    SetDefaultData();
}


wxString HotkeysManager::Start()
{
    wxString str_out;
    wxString str_in[vec_hotkey.size()];

    LoadData();

    str_in[0]=_("Automatic placement");

    str_in[1]=_("Down");
    str_in[1]+=_("Left");

    str_in[2]=_("Down");

    str_in[3]=_("Down");
    str_in[3]+=_("Right");

    str_in[4]=_("Left");

    str_in[5]=_("Full Screen");

    str_in[6]=_("Right");

    str_in[7]=_("Up");
    str_in[7]+=_("Left");

    str_in[8]=_("Up");

    str_in[9]=_("Up");
    str_in[9]+=_("Right");

    str_in[10]=_("Mosaic");

    str_in[11]=_("Windows fusion");

    str_in[12]=_("Close all window");

    str_in[13]=_("Move to Left screen");

    str_in[14]=_("Move to Right screen");

    str_in[15]=_("Minimize Window");

    str_in[16]=_("Maximize Window");

    str_in[17]=_("Active Window Tools");

    str_in[18]=_("Toggle Always on Top");

    str_in[19]=_("Maximize Horizontally");

    str_in[20]=_("Maximize Vertically");

    for (unsigned int i=0; i<vec_hotkey.size(); ++i)
    {
        if (vec_hotkey[i].session = vec_hotkey[i].actif)
        {
            vec_hotkey[i].session = ::RegisterHotKey((HWND)GetHandle(),HK_0+i,vec_hotkey[i].modifier1|vec_hotkey[i].modifier2,vec_hotkey[i].virtualKey);
            if (!vec_hotkey[i].session)
            {
                str_out+=str_in[i]+ _T("\n");
            }
        }
    }

    return str_out;
}


bool HotkeysManager::Stop()
{
    for (unsigned int i=0; i<vec_hotkey.size(); ++i)
        if (vec_hotkey[i].session) ::UnregisterHotKey((HWND)GetHandle(),HK_0+i);

    return true;
}


void HotkeysManager::SetDefaultData()
{
    // Resize au nombre de HK qu'utilise winsplit
    vec_hotkey.resize(21);

    for (unsigned int i = 0; i< vec_hotkey.size(); ++i)
    {
        vec_hotkey[i].modifier1 = MOD_CONTROL;
        vec_hotkey[i].modifier2 = MOD_ALT;
        vec_hotkey[i].actif = true;
    }
    // Valeurs des Hotkeys par défaut
    vec_hotkey[0].virtualKey = 0x60;    // Alt + Ctrl + NumPad 0 = Auto Placement
    vec_hotkey[1].virtualKey = 0x61;    // Alt + Ctrl + NumPad 1 = Bottom Left
    vec_hotkey[2].virtualKey = 0x62;    // Alt + Ctrl + NumPad 2 = Bottom
    vec_hotkey[3].virtualKey = 0x63;    // Alt + Ctrl + NumPad 3 = Bottom Right
    vec_hotkey[4].virtualKey = 0x64;    // Alt + Ctrl + NumPad 4 = Left
    vec_hotkey[5].virtualKey = 0x65;    // Alt + Ctrl + NumPad 5 = Center
    vec_hotkey[6].virtualKey = 0x66;    // Alt + Ctrl + NumPad 6 = Right
    vec_hotkey[7].virtualKey = 0x67;    // Alt + Ctrl + NumPad 7 = Up Left
    vec_hotkey[8].virtualKey = 0x68;    // Alt + Ctrl + NumPad 8 = Up
    vec_hotkey[9].virtualKey = 0x69;    // Alt + Ctrl + NumPad 9 = Up Right
    vec_hotkey[10].virtualKey = 0x4D;   // Alt + Ctrl + M = Mosaic
    vec_hotkey[11].virtualKey = 0x46;   // Alt + Ctrl + F = Windows Fusion
    vec_hotkey[12].virtualKey = 0x43;   // Alt + Ctrl + C = Close All Windows
    vec_hotkey[13].virtualKey = 0x25;   // Alt + Ctrl + Left = Window to Left Screen
    vec_hotkey[14].virtualKey = 0x27;   // Alt + Ctrl + Right = Window to Right Screen
    vec_hotkey[15].virtualKey = 0x22;   // Alt + Ctrl + Pg Dwn = Minimize Window
    vec_hotkey[16].virtualKey = 0x21;   // Alt + Ctrl + Pg Up = Restore Minimized Windows
    vec_hotkey[17].virtualKey = 0x54;   // Alt + Ctrl + T = Active Window Tools
    vec_hotkey[18].virtualKey = 0x4F;   // Alt + Ctrl + O = Toggle "Always on Top"
    vec_hotkey[19].virtualKey = 0x48;   // Alt + Ctrl + H = Maximize Horiz.
    vec_hotkey[20].virtualKey = 0x56;   // Alt + Ctrl + V = Maximize Vertic.
}

void HotkeysManager::SetVecHotkey(const std::vector<HotkeyStruct>& vec)
{
    vec_hotkey = vec;
}

std::vector<HotkeyStruct> HotkeysManager::GetCpyVector()
{
    return vec_hotkey;
}


int HotkeysManager::GetTaskIndex(wxString name)
{
    for(unsigned int i=0; i<vec_hotkey.size(); ++i)
    {
        if (name == xml_str[i])
            return i;
    }
}

bool HotkeysManager::LoadData()
{
    int i;
    long vk;
    wxString path;

    wxXmlDocument doc;
    wxXmlNode* child;
    wxXmlProperty* properties;
    wxString value;

    VirtualModifierManager mod_manager;

    path = m_options.GetDataDirectory() +_T("hotkeys.xml");

    if (!wxFileExists(path))
    {
        SaveData();
    }

    doc.Load(path);

    child = doc.GetRoot()->GetChildren();

    while (child)
    {
        i = GetTaskIndex(child->GetName());

        properties = child->GetProperties();
        value = properties->GetValue();
        vec_hotkey[i].modifier1 = mod_manager.GetValueFromString(value);

        properties = properties->GetNext();
        value = properties->GetValue();
        vec_hotkey[i].modifier2 = mod_manager.GetValueFromString(value);

        properties = properties->GetNext();
        value = properties->GetValue();
        value.ToLong(&vk);
        vec_hotkey[i].virtualKey = (unsigned int)vk;

        properties = properties->GetNext();
        value = properties->GetValue();
        vec_hotkey[i].actif = value.Cmp(_T("True"))==0;

        child = child->GetNext();
    }

    child = doc.DetachRoot();
    delete child;

    return true;
}


bool HotkeysManager::SaveData()
{
    wxString path;

    wxXmlDocument doc;
    wxXmlNode* node;
    wxXmlNode* root;
    wxXmlProperty* properties;
    wxString value;
    VirtualModifierManager mod_manager;

    path = m_options.GetDataDirectory() +_T("hotkeys.xml");

    root = new wxXmlNode(NULL,wxXML_ELEMENT_NODE,_T("WinSplit_HotKeys"));
    doc.SetRoot(root);

    for( unsigned int i = 0; i<vec_hotkey.size(); ++i)
    {
        node = new wxXmlNode(root,wxXML_ELEMENT_NODE,xml_str[i]);
        properties = new wxXmlProperty(_T("Modifier1"),mod_manager.GetStringFromValue(vec_hotkey[i].modifier1));
        node->SetProperties(properties);
        properties->SetNext(new wxXmlProperty(_T("Modifier2"),mod_manager.GetStringFromValue(vec_hotkey[i].modifier2)));
        properties = properties->GetNext();
        properties->SetNext(new wxXmlProperty(_T("VirtualHotkey"),wxString::Format(_T("%d"),vec_hotkey[i].virtualKey)));
        properties = properties->GetNext();
        properties->SetNext(new wxXmlProperty(_T("Activate"),vec_hotkey[i].actif?_T("True"):_T("False")));
    }

    doc.Save(path.c_str());

    root = doc.DetachRoot();
    delete root;

    return true;
}


WXLRESULT HotkeysManager::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
{
    if (nMsg == WM_HOTKEY)
    {
        switch ((int)wParam)
        {
        case HK_0:
            AutoPlace();
            break;
        case HK_1:
            ResizeWindow(1);
            break;
        case HK_2:
            ResizeWindow(2);
            break;
        case HK_3:
            ResizeWindow(3);
            break;
        case HK_4:
            ResizeWindow(4);
            break;
        case HK_5:
            ResizeWindow(5);
            break;
        case HK_6:
            ResizeWindow(6);
            break;
        case HK_7:
            ResizeWindow(7);
            break;
        case HK_8:
            ResizeWindow(8);
            break;
        case HK_9:
            ResizeWindow(9);
            break;
        case HK_M:
            Mosaique();
            break;
        case HK_C:
            CloseAllFrame();
            break;
        case HK_F:
            fusion_fenetres();
            break;
        case HK_LEFT:
            MoveToScreen(LEFT_SCREEN);
            break;
        case HK_RIGHT:
            MoveToScreen(RIGHT_SCREEN);
            break;
        case HK_MINIMIZE:
            m_minimizeRestore.MiniMizeWindow();
            break;
		case HK_MAXIMIZE:
            m_minimizeRestore.RestoreMiniMizedWindow();
            break;
		case HK_ACTWNDTOOLS:
			ActiveWndToolsDialog::ShowDialog();
			break;
        case HK_ALWAYSONTOP:
            ToggleAlwaysOnTop();
            break;
        case HK_HMAXIMIZE:
            m_minimizeRestore.MaximizeHorizontally();
            break;
        case HK_VMAXIMIZE:
            m_minimizeRestore.MaximizeVertically();
            break;
        default:
            break;
        }
    }

    return wxFrame::MSWWindowProc(nMsg, wParam, lParam);
}

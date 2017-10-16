// For Vista and newer "extended" frame compensation:
#include "Dwmapi-compat.h"

#include "layout_manager.h"

#include <wx/utils.h>

using namespace std;

LayoutManager* LayoutManager::p_instance = NULL;

LayoutManager::LayoutManager() : m_options(SettingsManager::Get())
{
    tab_seq.resize(9);
}


LayoutManager::~LayoutManager()
{
}


LayoutManager* LayoutManager::GetInstance()
{
    if (!p_instance)
        p_instance = new LayoutManager();

    return p_instance;
}


void LayoutManager::DeleteInstance()
{
    delete p_instance;
    p_instance = NULL;
}


void LayoutManager::SaveData()
{
    wxXmlDocument doc;
    wxXmlNode* elm = NULL;
    wxXmlNode* sub_elm = NULL;
    wxXmlNode* root = NULL;
    wxXmlProperty* property = NULL;

    vector<RatioRect>::iterator it;
    int count;

    root = new wxXmlNode(NULL,wxXML_ELEMENT_NODE,_T("LayoutManager"));

    doc.SetRoot(root);

    for (unsigned int i = 0; i<tab_seq.size(); ++i)
    {
        if (i == 0)
        {
            elm = new wxXmlNode(root,wxXML_ELEMENT_NODE,wxString::Format(_T("Sequence_%d"),i+1));
        }
        else
        {
            elm->SetNext(new wxXmlNode(NULL,wxXML_ELEMENT_NODE,wxString::Format(_T("Sequence_%d"),i+1)));
            elm = elm->GetNext();
        }

        property = new wxXmlProperty(_T("NbCombo"),wxString::Format(_T("%d"),tab_seq[i].size()));
        elm->SetProperties(property);

        it = tab_seq[i].begin();
        count = 0;

        while (it != tab_seq[i].end())
        {
            if (it == tab_seq[i].begin())
            {
                sub_elm = new wxXmlNode(elm,wxXML_ELEMENT_NODE,wxString::Format(_T("Combo_%d"),count));
            }
            else
            {
                sub_elm->SetNext(new wxXmlNode(NULL,wxXML_ELEMENT_NODE,wxString::Format(_T("Combo_%d"),count)));
                sub_elm = sub_elm->GetNext();
            }

            property = new wxXmlProperty(_T("x"),wxString::Format(_T("%.2f"),(*it).x));
            sub_elm->SetProperties(property);

            property->SetNext( new wxXmlProperty(_T("y"),wxString::Format(_T("%.2f"),(*it).y)));
            property = property->GetNext();

            property->SetNext( new wxXmlProperty(_T("width"),wxString::Format(_T("%.2f"),(*it).width)));
            property = property->GetNext();

            property->SetNext( new wxXmlProperty(_T("height"),wxString::Format(_T("%.2f"),(*it).height)));

            ++count;
            ++it;
        }
    }

    doc.Save(m_options.GetDataDirectory() + _T("layout.xml"));
    root = doc.DetachRoot();

    delete root;
}


void LayoutManager::LoadData()
{
    wxXmlDocument doc;
    wxXmlNode* elm;
    wxXmlNode* sub_elmt;
    wxXmlProperty* property;
    wxString value;
    double val_tmp;
    long nb_combo;

    if (!wxFileExists(m_options.GetDataDirectory() + _T("layout.xml")))
    {
        SetDefault();
        SaveData();

        return;
    }

    doc.Load(m_options.GetDataDirectory() + _T("layout.xml"));

    elm = doc.GetRoot()->GetChildren();

    for (unsigned int i = 0; i<tab_seq.size(); ++i)
    {
        tab_seq[i].clear();

        // Sequence
        property = elm->GetProperties();
        value = property->GetValue();
        value.ToLong(&nb_combo);
        tab_seq[i].resize(nb_combo);

        // combo
        sub_elmt = elm->GetChildren();

        for (int j=0; j<nb_combo; ++j)
        {
            property = sub_elmt->GetProperties();
            value = property->GetValue();
            value.ToDouble(&tab_seq[i][j].x);

            property = property->GetNext();
            value = property->GetValue();
            value.ToDouble(&tab_seq[i][j].y);

            property = property->GetNext();
            value = property->GetValue();
            value.ToDouble(&tab_seq[i][j].width);

            property = property->GetNext();
            value = property->GetValue();
            value.ToDouble(&tab_seq[i][j].height);

            sub_elmt = sub_elmt->GetNext();
        }
        elm = elm->GetNext();
    }

    elm = doc.DetachRoot();

    delete elm;
}


void LayoutManager::CopyTable(vector<vector<RatioRect> >& dest)
{
    dest = tab_seq;
}

void LayoutManager::SetTable(const vector<vector<RatioRect> >& source)
{
    tab_seq = source;
}


wxRect LayoutManager::GetNext(HWND hwnd,int sequence)
{
    wxRect result;
    RatioRect ratio_result;
    RECT screen_rect,wnd,wnd_tmp;
    HMONITOR hmonitor;
    MONITORINFO monitor_info;

    hmonitor = MonitorFromWindow(hwnd,MONITOR_DEFAULTTONEAREST);
    monitor_info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(hmonitor,&monitor_info);

    screen_rect = monitor_info.rcWork;

    {
        int major;
        wxGetOsVersion (&major, NULL);
        if (major < 6) // Older than Windows Vista.
            GetWindowRect(hwnd,&wnd);
        else
            DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &wnd, sizeof(RECT));
    }

    vector<RatioRect>::iterator it;

    it = tab_seq[sequence].begin();

    while (it != tab_seq[sequence].end())
    {
        wnd_tmp.left = screen_rect.left + int(round((*it).x*0.01*(screen_rect.right-screen_rect.left)));
        wnd_tmp.top = screen_rect.top + int(round((*it).y*0.01*(screen_rect.bottom-screen_rect.top)));

        wnd_tmp.right = wnd_tmp.left + int(round((*it).width*0.01*(screen_rect.right-screen_rect.left)));
        wnd_tmp.bottom = wnd_tmp.top + int(round((*it).height*0.01*(screen_rect.bottom-screen_rect.top)));

        ++it; // point to the next one
        if ( (wnd_tmp.left == wnd.left) && (wnd_tmp.right == wnd.right) && (wnd_tmp.bottom == wnd.bottom) && (wnd_tmp.top == wnd.top))
            break;
    }

    if (it == tab_seq[sequence].end())
        it = tab_seq[sequence].begin();

    result.x = screen_rect.left + int(round((*it).x*0.01*(screen_rect.right-screen_rect.left)));
    result.y = screen_rect.top + int(round((*it).y*0.01*(screen_rect.bottom-screen_rect.top)));

    result.width = int(round((*it).width*0.01*(screen_rect.right-screen_rect.left)));
    result.height = int(round((*it).height*0.01*(screen_rect.bottom-screen_rect.top)));

    return result;
}


bool LayoutManager::GetNearestFromCursor(vector<wxRect>& result)
{
    wxRect rect_tmp;
    wxPoint point_tmp;
    unsigned int distance_tmp;
    unsigned int distance_min = 99999999;
	unsigned int rayon = SettingsManager::Get().getDnGDetectionRadius();

	vector<RatioRect>::iterator it;

	RECT screen_rect;
    HMONITOR hmonitor;
    MONITORINFO monitor_info;

    POINT mouse_point;
    GetCursorPos(&mouse_point);

    wxPoint cursor_pos(mouse_point.x,mouse_point.y);

    hmonitor = MonitorFromWindow(GetForegroundWindow(),MONITOR_DEFAULTTONEAREST);
    monitor_info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(hmonitor,&monitor_info);

    screen_rect = monitor_info.rcWork;

    result.clear();

	for (int i=0; i< int(tab_seq.size()); ++i)
	{
		it= tab_seq[i].begin();

		while(it != tab_seq[i].end())
        {
            point_tmp.y = screen_rect.top + int(round((*it).y*0.01*(screen_rect.bottom-screen_rect.top))) + int(round((*it).height*0.01*(screen_rect.bottom-screen_rect.top)/2));
            point_tmp.x = screen_rect.left + int(round((*it).x*0.01*(screen_rect.right-screen_rect.left))) + int(round((*it).width*0.01*(screen_rect.right-screen_rect.left)/2));

            distance_tmp = (cursor_pos.x - point_tmp.x)*(cursor_pos.x - point_tmp.x) + (cursor_pos.y - point_tmp.y)*(cursor_pos.y - point_tmp.y);

            if(distance_tmp < distance_min)
            {
                distance_min = int(distance_tmp);
                result.clear();

                rect_tmp.x = screen_rect.left + int(round((*it).x*0.01*(screen_rect.right-screen_rect.left)));
                rect_tmp.y = screen_rect.top + int(round((*it).y*0.01*(screen_rect.bottom-screen_rect.top)));
                rect_tmp.width = int(round((*it).width*0.01*(screen_rect.right-screen_rect.left)));
                rect_tmp.height = int(round((*it).height*0.01*(screen_rect.bottom-screen_rect.top)));

                result.push_back(rect_tmp);
            }
            else if(distance_tmp == distance_min)
            {
                rect_tmp.x = screen_rect.left + int(round((*it).x*0.01*(screen_rect.right-screen_rect.left)));
                rect_tmp.y = screen_rect.top + int(round((*it).y*0.01*(screen_rect.bottom-screen_rect.top)));
                rect_tmp.width = int(round((*it).width*0.01*(screen_rect.right-screen_rect.left)));
                rect_tmp.height = int(round((*it).height*0.01*(screen_rect.bottom-screen_rect.top)));

                result.push_back(rect_tmp);
            }

            ++it;
        }
	}

	if(distance_min > 2*rayon*rayon)
        return false;

	return true;
}


void LayoutManager::SetDefault()
{
    vector<RatioRect>::iterator it;

    //==================
    // Ctrl+Alt+1
    //==================
    tab_seq[0].resize(3);
    // set 1
    it = tab_seq[0].begin();
    (*it).x = 0;
    (*it).y = 50;
    (*it).width = 50;
    (*it).height = 50;

    // set 2
    ++it;
    (*it).x = 0;
    (*it).y = 50;
    (*it).width = 100./3.;
    (*it).height = 50;

    // set 3
    ++it;
    (*it).x = 0;
    (*it).y = 50;
    (*it).width = 200./3.;
    (*it).height = 50;

    //==================
    // Ctrl+Alt+2
    //==================
    tab_seq[1].resize(2);
    // set 1
    it = tab_seq[1].begin();
    (*it).x = 0;
    (*it).y = 50;
    (*it).width = 100;
    (*it).height = 50;

    // set 2
    ++it;
    (*it).x = 100./3.;
    (*it).y = 50;
    (*it).width = 100./3.;
    (*it).height = 50;

    //==================
    // Ctrl+Alt+3
    //==================
    tab_seq[2].resize(3);
    // set 1
    it = tab_seq[2].begin();
    (*it).x = 50;
    (*it).y = 50;
    (*it).width = 50;
    (*it).height = 50;

    // set 2
    ++it;
    (*it).x = 200./3.;
    (*it).y = 50;
    (*it).width = 100./3.;
    (*it).height = 50;

    // set 3
    ++it;
    (*it).x = 100./3.;
    (*it).y = 50;
    (*it).width = 200./3.;
    (*it).height = 50;

    //==================
    // Ctrl+Alt+4
    //==================
    tab_seq[3].resize(3);
    // set 1
    it = tab_seq[3].begin();
    (*it).x = 0;
    (*it).y = 0;
    (*it).width = 50;
    (*it).height = 100;

    // set 2
    ++it;
    (*it).x = 0;
    (*it).y = 0;
    (*it).width = 100./3.;
    (*it).height = 100;

    // set 3
    ++it;
    (*it).x = 0;
    (*it).y = 0;
    (*it).width = 200./3.;
    (*it).height = 100;

    //==================
    // Ctrl+Alt+5
    //==================
    tab_seq[4].resize(3);
    // set 1
    it = tab_seq[4].begin();
    (*it).x = 0;
    (*it).y = 0;
    (*it).width = 100;
    (*it).height = 100;

    // set 2
    ++it;
    (*it).x = 100./3.;
    (*it).y = 0;
    (*it).width = 100./3.;
    (*it).height = 100;

    // set 3
    ++it;
    (*it).x = 100./6.;
    (*it).y = 0;
    (*it).width = 200./3.;
    (*it).height = 100;

    //==================
    // Ctrl+Alt+6
    //==================
    tab_seq[5].resize(3);
    // set 1
    it = tab_seq[5].begin();
    (*it).x = 50;
    (*it).y = 0;
    (*it).width = 50;
    (*it).height = 100;

    // set 2
    ++it;
    (*it).x = 200./3.;
    (*it).y = 0;
    (*it).width = 100./3.;
    (*it).height = 100;

    // set 3
    ++it;
    (*it).x = 100./3.;
    (*it).y = 0;
    (*it).width = 200./3.;
    (*it).height = 100;

    //==================
    // Ctrl+Alt+7
    //==================
    tab_seq[6].resize(3);
    // set 1
    it = tab_seq[6].begin();
    (*it).x = 0;
    (*it).y = 0;
    (*it).width = 50;
    (*it).height = 50;

    // set 2
    ++it;
    (*it).x = 0;
    (*it).y = 0;
    (*it).width = 100./3.;
    (*it).height = 50;

    // set 3
    ++it;
    (*it).x = 0;
    (*it).y = 0;
    (*it).width = 200./3.;
    (*it).height = 50;

    //==================
    // Ctrl+Alt+8
    //==================
    tab_seq[7].resize(2);
    // set 1
    it = tab_seq[7].begin();
    (*it).x = 0;
    (*it).y = 0;
    (*it).width = 100;
    (*it).height = 50;

    // set 2
    ++it;
    (*it).x = 100./3.;
    (*it).y = 0;
    (*it).width = 100./3.;
    (*it).height = 50;

    //==================
    // Ctrl+Alt+9
    //==================
    tab_seq[8].resize(3);
    // set 1
    it = tab_seq[8].begin();
    (*it).x = 50;
    (*it).y = 0;
    (*it).width = 50;
    (*it).height = 50;

    // set 2
    ++it;
    (*it).x = 200./3.;
    (*it).y = 0;
    (*it).width = 100./3.;
    (*it).height = 50;

    // set 3
    ++it;
    (*it).x = 100./3.;
    (*it).y = 0;
    (*it).width = 200./3.;
    (*it).height = 50;
}

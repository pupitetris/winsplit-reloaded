#include <algorithm>
#include <psapi.h>
#include <windows.h>

#include <wx/xml/xml.h>

#include "auto_placement.h"
#include "settingsmanager.h"

using namespace std;

bool WindowInfos::operator== (const WindowInfos& rhs)
{
	return m_strName.CmpNoCase (rhs.m_strName) == 0;
}


bool AutoPlacementManager::Exist (const wxString& name)
{
	WindowInfos tmp;
	tmp.m_strName = name;

	return find (m_vecWnd.begin(), m_vecWnd.end(), tmp) != m_vecWnd.end();
}


WindowInfos AutoPlacementManager::GetWindowInfos (const wxString& name)
{
	vector<WindowInfos>::iterator iter;
	WindowInfos tmp;

	tmp.m_strName = name;
	return * (find (m_vecWnd.begin(), m_vecWnd.end(), tmp) );
}


void AutoPlacementManager::AddWindow (const HWND& hwnd, const wxString& name)
{
	WindowInfos structinfo;
	RECT rect;

	structinfo.m_strName = name;

	structinfo.m_flagResize = (GetWindowLong (hwnd, GWL_STYLE) &WS_SIZEBOX) == 0 ? 0 : 1;
	structinfo.m_wndStyle = GetWindowLong (hwnd, GWL_STYLE);

	GetWindowRect (hwnd, &rect);
	structinfo.m_rectxy.x = rect.left;
	structinfo.m_rectxy.y = rect.top;
	structinfo.m_rectxy.width = rect.right - rect.left;
	structinfo.m_rectxy.height = rect.bottom - rect.top;

	m_vecWnd.push_back (structinfo);
}


bool AutoPlacementManager::LoadData()
{
	wxString path;
	wxXmlDocument doc;
	wxXmlNode* child;
	wxXmlProperty* properties;
	wxString value;
	WindowInfos window_info;

	path = SettingsManager::Get().GetDataDirectory() +  _T ("auto_placement.xml");

	if (!wxFileExists (path.c_str() ) )
	{
		SaveData();
	}

	doc.Load (path);
	child = doc.GetRoot()->GetChildren();

	m_vecWnd.clear();

	while (child)
	{
		properties = child->GetProperties();
		window_info.m_strName = properties->GetValue();
		properties = properties->GetNext();

		value = properties->GetValue();
		value.ToLong ( (long*) &window_info.m_wndStyle);
		properties = properties->GetNext();

		value = properties->GetValue();
		value.ToLong ( (long*) &window_info.m_rectxy.x);
		properties = properties->GetNext();

		value = properties->GetValue();
		value.ToLong ( (long*) &window_info.m_rectxy.y);
		properties = properties->GetNext();

		value = properties->GetValue();
		value.ToLong ( (long*) &window_info.m_rectxy.width);
		properties = properties->GetNext();

		value = properties->GetValue();
		value.ToLong ( (long*) &window_info.m_rectxy.height);
		properties = properties->GetNext();

		value = properties->GetValue();
		window_info.m_flagResize = value.Cmp (_T ("True") ) == 0;

		m_vecWnd.push_back (window_info);

		child = child->GetNext();
	}
	child = doc.DetachRoot();
	delete child;
	return true;
}


bool AutoPlacementManager::SaveData()
{
	wxString path;
	vector<WindowInfos>::iterator iter;

	wxXmlDocument doc;
	wxXmlNode* node;
	wxXmlNode* root;
	wxXmlProperty* properties;

	path = SettingsManager::Get().GetDataDirectory() +  _T ("auto_placement.xml");

	root = new wxXmlNode (NULL, wxXML_ELEMENT_NODE, _T ("WinSplit_AutoPlacement") );

	doc.SetRoot (root);

	for (iter = m_vecWnd.begin(); iter != m_vecWnd.end(); ++iter)
	{
		node = new wxXmlNode (root, wxXML_ELEMENT_NODE, _T ("Application") );
		properties = new wxXmlProperty (_T ("Name"), (*iter).m_strName);
		node->SetProperties (properties);
		properties->SetNext (new wxXmlProperty (_T ("WindowStyle"), wxString::Format (_T ("%ld"), (*iter).m_wndStyle) ) );
		properties = properties->GetNext();
		properties->SetNext (new wxXmlProperty (_T ("x"), wxString::Format (_T ("%d"), (*iter).m_rectxy.x) ) );
		properties = properties->GetNext();
		properties->SetNext (new wxXmlProperty (_T ("y"), wxString::Format (_T ("%d"), (*iter).m_rectxy.y) ) );
		properties = properties->GetNext();
		properties->SetNext (new wxXmlProperty (_T ("width"), wxString::Format (_T ("%d"), (*iter).m_rectxy.width) ) );
		properties = properties->GetNext();
		properties->SetNext (new wxXmlProperty (_T ("height"), wxString::Format (_T ("%d"), (*iter).m_rectxy.height) ) );
		properties = properties->GetNext();
		properties->SetNext (new wxXmlProperty (_T ("Resize"), (*iter).m_flagResize ? _T ("True") : _T ("False") ) );
	}

	doc.Save (path.c_str() );

	delete doc.DetachRoot();

	return true;
}


bool AutoPlacementManager::IsEmpty()
{
	return m_vecWnd.empty();
}


wxArrayString AutoPlacementManager::GetArrayName()
{
	wxArrayString array_name;
	vector<WindowInfos>::iterator iter = m_vecWnd.begin();

	while (iter != m_vecWnd.end() )
	{
		array_name.Add ( (*iter).m_strName);
		++iter;
	}

	return array_name;
}


void AutoPlacementManager::DeleteApplication (const int& position)
{
	m_vecWnd.erase (m_vecWnd.begin() + position);
}

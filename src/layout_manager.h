#ifndef __LAYOUT_MANAGER_H__
#define __LAYOUT_MANAGER_H__

#include <wx/wx.h>
#include <wx/xml/xml.h>
#include <wx/filefn.h>
#include <vector>
#include <windows.h>
#include "settingsmanager.h"

struct RatioRect
{
	double x;
	double y;
	double width;
	double height;
};


class LayoutManager // Classe singleton
{
private:
	static LayoutManager* p_instance;
	SettingsManager &m_options;

	std::vector<std::vector<RatioRect> > tab_seq;

	LayoutManager();
	~LayoutManager();

public:

	static LayoutManager* GetInstance();
	static void DeleteInstance();

	void LoadData();
	void SaveData();
	void SetDefault();

	void CopyTable (std::vector<std::vector<RatioRect> >& destination);
	void SetTable (const std::vector<std::vector<RatioRect> >& source);

	wxRect GetNext (HWND hwnd, int sequence);
	bool GetNearestFromCursor (std::vector<wxRect>& result);
};


#endif //#ifndef __LAYOUT_MANAGER_H__


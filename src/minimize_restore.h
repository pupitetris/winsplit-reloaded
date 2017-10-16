#ifndef __MINIMIZE_RESTORE_H__
#define __MINIMIZE_RESTORE_H__


#include <stack>
#include <windows.h>


struct WindowMinimized
{
	HWND hwnd;
	WINDOWPLACEMENT placement;
};

class MinimizeRestore
{
private:
	std::stack<WindowMinimized> m_vecMinimized;
public:
	MinimizeRestore() {}

	~MinimizeRestore() {}

	void MiniMizeWindow();
	void RestoreMiniMizedWindow();
	void MaximizeHorizontally();
	void MaximizeVertically();
};


#endif // __MINIMIZE_RESTORE_H__


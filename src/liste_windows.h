#ifndef __LISTER_FENETRE_H__
#define __LISTER_FENETRE_H__

#include <windows.h>


class ListWindows
{
private:

public:
    ListWindows(){}
    static bool ValidateWindow(HWND hwnd,bool accept_tmw=true);
    static HWND ListWindow();
};




#endif // __LISTER_FENETRE_H__

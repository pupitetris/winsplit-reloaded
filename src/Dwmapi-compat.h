#ifndef DWMAPI
#if !defined(_DWMAPI_)
#define DWMAPI          EXTERN_C DECLSPEC_IMPORT HRESULT STDAPICALLTYPE
#define DWMAPI_(type)   EXTERN_C DECLSPEC_IMPORT type STDAPICALLTYPE
#else
#define DWMAPI          STDAPI
#define DWMAPI_(type)   STDAPI_(type)
#endif /* _DWMAPI_ */
#endif /* DWMAPI */

#include <WinNT.h>

#ifdef __cplusplus
extern "C"
{
#endif

// Window attributes
enum DWMWINDOWATTRIBUTE
{
    DWMWA_NCRENDERING_ENABLED = 1,      // [get] Is non-client rendering enabled/disabled
    DWMWA_NCRENDERING_POLICY,           // [set] Non-client rendering policy
    DWMWA_TRANSITIONS_FORCEDISABLED,    // [set] Potentially enable/forcibly disable transitions
    DWMWA_ALLOW_NCPAINT,                // [set] Allow contents rendered in the non-client area to be visible on the DWM-drawn frame.
    DWMWA_CAPTION_BUTTON_BOUNDS,        // [get] Bounds of the caption button area in window-relative space.
    DWMWA_NONCLIENT_RTL_LAYOUT,         // [set] Is non-client content RTL mirrored
    DWMWA_FORCE_ICONIC_REPRESENTATION,  // [set] Force this window to display iconic thumbnails.
    DWMWA_FLIP3D_POLICY,                // [set] Designates how Flip3D will treat the window.
    DWMWA_EXTENDED_FRAME_BOUNDS,        // [get] Gets the extended frame bounds rectangle in screen space
    DWMWA_HAS_ICONIC_BITMAP,            // [set] Indicates an available bitmap when there is no better thumbnail representation.
    DWMWA_DISALLOW_PEEK,                // [set] Don't invoke Peek on the window.
    DWMWA_EXCLUDED_FROM_PEEK,           // [set] LivePreview exclusion information
    DWMWA_LAST
};

DWMAPI
DwmGetWindowAttribute(
    HWND hwnd,
    DWORD dwAttribute,
//    __out_bcount(cbAttribute)
    PVOID pvAttribute,
    DWORD cbAttribute
    );

DWMAPI
DwmSetWindowAttribute(
    HWND hwnd,
    DWORD dwAttribute,
    LPCVOID pvAttribute,
    DWORD cbAttribute
    );

#ifdef __cplusplus
}
#endif

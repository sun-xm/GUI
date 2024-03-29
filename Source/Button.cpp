#include "Button.h"
#include <CommCtrl.h>

using namespace std;

Button::Button(HWND hWnd) : DialogItem(hWnd)
{
}

Button Button::Create(HWND parent, UINT id, const wstring& text, HINSTANCE instance)
{
    DialogItem di;

    if (parent)
    {
        auto hwnd = CreateWindowExW(0, WC_BUTTONW, text.c_str(), WS_CHILD | WS_TABSTOP, 0, 0, 0, 0, parent, (HMENU)(UINT_PTR)id, instance, nullptr);
        if (hwnd)
        {
            di = DialogItem(hwnd);
        }
    }

    return (Button&)di;
}
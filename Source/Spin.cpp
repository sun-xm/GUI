#include "Spin.h"
#include <CommCtrl.h>

Spin::Spin(HWND hWnd) : DialogItem(hWnd)
{
}

HWND Spin::Buddy(HWND hwnd)
{
    return (HWND)this->Send(UDM_SETBUDDY, (WPARAM)hwnd);
}

void Spin::Range(int low, int high)
{
    this->Send(UDM_SETRANGE32, (WPARAM)low, (LPARAM)high);
}
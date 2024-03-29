#pragma once

#include "DialogItem.h"

class ProgressBar : public DialogItem
{
public:
    ProgressBar(HWND hWnd);

    bool SetRange(int min, int max);
    void GetRange(int& min, int& max);

    void Marquee(bool start, int updateInterval = 0);

    int  Position();
    void Position(int pos);

    static ProgressBar Create(HWND parent, UINT id, HINSTANCE instance = nullptr);
};
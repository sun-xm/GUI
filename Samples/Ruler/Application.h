#pragma once

#include "MainWindow.h"
#include "DDCRenderer.h"

class Application : public MainWindow
{
public:
    Application(HINSTANCE);

protected:
    bool OnCreated() override;
    void OnSize() override;

private:
    void OnKeyDown();
    void OnMouseMove();
    void Update();

private:
    POINT pos;

    int  color;
    int  length;
    bool sizing;
    bool vertical;

    DDCRenderer renderer;
};
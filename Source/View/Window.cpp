#include "Window.h"

#define WNDCLASSNAME L"{317B2C5D-2708-458B-874A-93B07E3723D1}"

bool Window::registered = false;

Window::Window(HINSTANCE instance)
  : View(instance), style(0), styleEx(0), bkgColor(GetSysColor(COLOR_WINDOW))
{
}

bool Window::Create(View* parent)
{
    if (!this->hwnd)
    {
        if (!Register())
        {
            return false;
        }

        this->owner  = parent;
        this->parent = parent;

        HWND hWndParent;

        if (parent)
        {
            hWndParent = parent->Handle();
            if (!this->Style())
            {
                this->Style(WS_CHILDWINDOW);
            }
        }
        else
        {
            hWndParent = nullptr;
            if (!this->Style())
            {
                this->Style(WS_OVERLAPPEDWINDOW);
            }
        }

        this->hwnd = CreateWindowExW(this->StyleEx(),
                                     WNDCLASSNAME,
                                     L"",
                                     this->Style(),
                                     CW_USEDEFAULT,
                                     CW_USEDEFAULT,
                                     CW_USEDEFAULT,
                                     CW_USEDEFAULT,
                                     hWndParent,
                                     nullptr,
                                     this->instance,
                                     this);

        if (this->hwnd)
        {
            if (!this->OnCreated())
            {
                this->Destroy();
            }
        }
        else
        {
            this->owner  = nullptr;
            this->parent = nullptr;
        }
    }

    return nullptr != this->hwnd;
}

void Window::Style(DWORD style)
{
    this->style = style;

    if (this->hwnd)
    {
        SetWindowLongPtrW(this->hwnd, GWL_STYLE, style);
        SetWindowPos(this->hwnd, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);
    }
}

DWORD Window::Style() const
{
    return this->style;
}

void Window::StyleEx(DWORD style)
{
    this->styleEx = style;

    if (this->hwnd)
    {
        SetWindowLongPtr(this->hwnd, GWL_EXSTYLE, style);
        SetWindowPos(this->hwnd, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);
    }
}

DWORD Window::StyleEx() const
{
    return this->styleEx;
}

COLORREF Window::Background() const
{
    return this->bkgColor;
}

void Window::Background(COLORREF color)
{
    this->bkgColor = color;
    this->Invalidate();
}

LRESULT Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_ERASEBKGND:
        HBRUSH brush;
        brush = CreateSolidBrush(this->bkgColor);
        FillRect((HDC)wParam, &this->ClientRect(), brush);
        DeleteObject(brush);
        return TRUE;

    case WM_NCCREATE:
        if (!this->OnNcCreate())
        {
            return FALSE;
        }
        break;

    case WM_CREATE:
        GetWindowRect(this->Handle(), &this->wrect);
        GetClientRect(this->Handle(), &this->crect);
        this->OnCreate();
        break;

    case WM_GETDLGCODE:
        return DLGC_WANTALLKEYS;

    default:
        break;
    }

    return View::WindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::DefaultProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

bool Window::OnNcCreate()
{
    return true;
}

void Window::OnCreate()
{
}

bool Window::Register()
{
    if (registered)
    {
        return true;
    }

    WNDCLASSEXW wcex   = {0};
    wcex.cbSize        = sizeof(wcex);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = (WNDPROC)View::MessageRouter;
    wcex.hInstance     = GetModuleHandleW(nullptr);
    wcex.hIcon         = nullptr;
    wcex.hIconSm       = nullptr;
    wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = nullptr;
    wcex.lpszMenuName  = nullptr;
    wcex.lpszClassName = WNDCLASSNAME;

    auto atom = RegisterClassExW(&wcex);

    registered = (0 != atom);

    return registered;
}
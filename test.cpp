// FrameworkUI.cpp : 定义应用程序的入口点。
//
#include "UIMain.h"

#include <bits/stdc++.h>
#include <windows.h>
#pragma comment(linker,"/manifestdependency:"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'"")
#define IDC_2 1002
#define IDC_3 2002

HINSTANCE g_hInstance;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // TODO: Place code here.
    g_hInstance = hInstance;
    TCHAR className[] = TEXT("my class");
    WNDCLASS wndclass = { 0 };
    wndclass.hInstance = hInstance;
    wndclass.hbrBackground = (CreateSolidBrush(RGB(30, 30, 30)));
    wndclass.lpszClassName = className;
    wndclass.lpfnWndProc = WindowProc;
    wndclass.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));

    RegisterClass(&wndclass);

    HWND hMain = CreateWindow(className, "FrameworkUI", WS_OVERLAPPEDWINDOW, 300, 300, 400, 300, NULL, NULL, hInstance, NULL);

    if (!hMain)
    {
        OutputDebugString("=),");
        return 0;
    }

    ShowWindow(hMain, SW_SHOW);

    MSG msg;
    BOOL ret;
    while (ret = GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

//                                                                                                                                                PROC
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HFONT hFont;//定义静态字体变量
    static HWND close;
    static HWND txt1;
    static HWND txt2;
    static HWND bt1;

    switch (uMsg)
    {
        case WM_CREATE:
        {
            hFont = CreateFont(32, 16, 0, 0, 0, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Minecraft");//创建字体
            close = CreateWindow("STATIC", "×", WS_CHILD | WS_VISIBLE | SS_NOTIFY, 360, -5, 130, 195, hwnd, (HMENU)IDC_CLOSE, g_hInstance, NULL);//创建静态文本
            SendMessage(close, WM_SETFONT, (WPARAM)hFont, TRUE);//发送设置字体消息
            hFont = CreateFont(14, 7, 0, 0, 0, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Minecraft");//创建字体
            txt1 = CreateWindow("STATIC", "FrameworkUI", WS_CHILD | WS_VISIBLE | SS_NOTIFY, 10, 10, 120, 20, hwnd, (HMENU)IDC_2, g_hInstance, NULL);//创建静态文本
            SendMessage(txt1, WM_SETFONT, (WPARAM)hFont, TRUE);//发送设置字体消息
            bt1 = CreateWindow("Button", "UI BUTTON", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,//默认按钮样式，就这里和下面想自定义
                115, 80, 110, 60, hwnd, (HMENU)IDC_3, g_hInstance, NULL);
            bt1 = CreateWindow("Button", "UI BUTTON", WS_VISIBLE | WS_CHILD | BS_3STATE,
                115, 140, 110, 60, hwnd, (HMENU)IDC_STATE, g_hInstance, NULL);
            break;
        }

        case WM_CTLCOLORSTATIC://拦截WM_CTLCOLORSTATIC消息
        {

            if ((HWND)lParam == GetDlgItem(hwnd, IDC_2))//2
            {
                SetTextColor((HDC)wParam, RGB(165, 120, 120));
                SetBkMode((HDC)wParam, TRANSPARENT);
            }
            if ((HWND)lParam == GetDlgItem(hwnd, IDC_CLOSE))//2
            {
                SetTextColor((HDC)wParam, RGB(199, 0, 0));
                SetBkMode((HDC)wParam, TRANSPARENT);
            }
            return (INT_PTR)GetStockObject((NULL_BRUSH));//返回一个空画刷(必须)
        }
        case WM_SIZE:
        {
            LONG_PTR Style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
            Style = Style & ~WS_CAPTION & ~WS_SYSMENU & ~WS_SIZEBOX;
            ::SetWindowLongPtr(hwnd, GWL_STYLE, Style);
            break;
        }
        case WM_LBUTTONDOWN:
        {
            SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);  //向窗口发送WM_NCLBUTTONDOWN消息，HTCAPTION 标题
            break;
        }
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDC_2:
                    MessageBox(hwnd, "标题栏的消息响应（这明明是静态文本）xD", "提示", MB_OK | MB_ICONINFORMATION);
                    break;
                case IDC_3:
                    MessageBox(hwnd, "您点击了按钮。(我需要告诉你你做了什么）", "提示", MB_OK | MB_ICONINFORMATION);
                    break;
                case IDC_CLOSE:
                    DeleteObject(hFont);//删除所创建字体对象
                    PostQuitMessage(0);
                    return 0;
            }
        case WM_DESTROY:
            DeleteObject(hFont);//删除所创建字体对象
            PostQuitMessage(0);
            break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


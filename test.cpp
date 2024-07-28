// FrameworkUI.cpp : ����Ӧ�ó������ڵ㡣
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
    static HFONT hFont;//���徲̬�������
    static HWND close;
    static HWND txt1;
    static HWND txt2;
    static HWND bt1;

    switch (uMsg)
    {
        case WM_CREATE:
        {
            hFont = CreateFont(32, 16, 0, 0, 0, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Minecraft");//��������
            close = CreateWindow("STATIC", "��", WS_CHILD | WS_VISIBLE | SS_NOTIFY, 360, -5, 130, 195, hwnd, (HMENU)IDC_CLOSE, g_hInstance, NULL);//������̬�ı�
            SendMessage(close, WM_SETFONT, (WPARAM)hFont, TRUE);//��������������Ϣ
            hFont = CreateFont(14, 7, 0, 0, 0, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Minecraft");//��������
            txt1 = CreateWindow("STATIC", "FrameworkUI", WS_CHILD | WS_VISIBLE | SS_NOTIFY, 10, 10, 120, 20, hwnd, (HMENU)IDC_2, g_hInstance, NULL);//������̬�ı�
            SendMessage(txt1, WM_SETFONT, (WPARAM)hFont, TRUE);//��������������Ϣ
            bt1 = CreateWindow("Button", "UI BUTTON", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,//Ĭ�ϰ�ť��ʽ����������������Զ���
                115, 80, 110, 60, hwnd, (HMENU)IDC_3, g_hInstance, NULL);
            bt1 = CreateWindow("Button", "UI BUTTON", WS_VISIBLE | WS_CHILD | BS_3STATE,
                115, 140, 110, 60, hwnd, (HMENU)IDC_STATE, g_hInstance, NULL);
            break;
        }

        case WM_CTLCOLORSTATIC://����WM_CTLCOLORSTATIC��Ϣ
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
            return (INT_PTR)GetStockObject((NULL_BRUSH));//����һ���ջ�ˢ(����)
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
            SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);  //�򴰿ڷ���WM_NCLBUTTONDOWN��Ϣ��HTCAPTION ����
            break;
        }
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDC_2:
                    MessageBox(hwnd, "����������Ϣ��Ӧ���������Ǿ�̬�ı���xD", "��ʾ", MB_OK | MB_ICONINFORMATION);
                    break;
                case IDC_3:
                    MessageBox(hwnd, "������˰�ť��(����Ҫ������������ʲô��", "��ʾ", MB_OK | MB_ICONINFORMATION);
                    break;
                case IDC_CLOSE:
                    DeleteObject(hFont);//ɾ���������������
                    PostQuitMessage(0);
                    return 0;
            }
        case WM_DESTROY:
            DeleteObject(hFont);//ɾ���������������
            PostQuitMessage(0);
            break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


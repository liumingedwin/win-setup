#include <windows.h>
#include <commctrl.h>
const TCHAR* winClassName = TEXT("WinExample");
HICON hIcon;
const wchar_t* note1 = L"I Love Windows";
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
/**
 * 104 Computer
 * 229 Upd
 * 152 software
*/
// 窗口函数声明
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
        {
            // 加载图标
            // hIcon = LoadIcon(NULL, IDI_APPLICATION);
            hIcon = ExtractIcon(GetModuleHandle(NULL), TEXT("C:\\Windows\\System32\\imageres.dll"), 229);
            if (hIcon)
            {
                // 创建带图标的按钮
                HWND btnID = CreateWindowA("BUTTON", "Click Me", WS_CHILD | WS_VISIBLE | BS_COMMANDLINK | BS_VCENTER | BS_CENTER,
                    10, 10, 100 * 4.618, 100, hwnd, (HMENU)hIcon, NULL, NULL);
                SendMessage(btnID, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
                SendMessage(btnID, BCM_SETNOTE, 0, (LPARAM)note1);
                SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            }
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 注册窗口类等代码省略...

    // 创建窗口等代码省略...

    // 消息循环等代码省略...
    WNDCLASSEX wc; /* A properties struct of our window */
    HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
    MSG msg; /* A temporary location for all messages */

    /* zero out the struct and set the stuff we want to modify */
    memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProcedure; /* This is where we will send messages to */
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    /* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = winClassName;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, TEXT("Class Registration Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, winClassName, TEXT("Example"), WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, /* x */
        CW_USEDEFAULT, /* y */
        640, /* width */
        480, /* height */
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, TEXT("Window Creation Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    /*
        This is the heart of our program where all input is processed and
        sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
        this loop will not produce unreasonably high CPU usage
    */
    while (GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
        TranslateMessage(&msg); /* Translate key codes to chars if present */
        DispatchMessage(&msg); /* Send it to WndProc */
    }
    return msg.wParam;
}
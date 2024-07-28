#include <bits/stdc++.h>
#include <windows.h>
#include <shellapi.h>
using namespace std;



int main(void) {
    while (1) {
        string path;
        UINT cnt;
        cin >> path >> cnt;
        HICON icon = ExtractIconA(GetModuleHandleA(NULL), path.c_str(), cnt);
        SendMessageA(GetConsoleWindow(), WM_SETICON, ICON_BIG, (LPARAM)icon);
        SendMessageA(GetConsoleWindow(), WM_SETICON, ICON_SMALL, (LPARAM)icon);
        SendMessageA(GetConsoleWindow(), WM_SETICON, ICON_SMALL2, (LPARAM)icon);
    }

    return 0;
}
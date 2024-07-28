#include <bits/stdc++.h>
#include <windows.h>
#include <shellapi.h>
using namespace std;



int main(void) {
    for (UINT i = 0; ; ++i) {
        HICON icon = ExtractIconA(GetModuleHandleA(NULL), "C:\\Windows\\System32\\shell32.dll", i);
        // if (GetLastError()) return 0;
        cout << GetLastError() << icon << " " << i / 10 * 100 << " " << i % 10 * 100 << endl;
        DrawIcon(GetDC(0), i / 10 * 100, i % 10 * 100, icon);
        Sleep(10);
    }
    return 0;
}
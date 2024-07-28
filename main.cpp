#include <windows.h>
#include <dismapi.h>
#include <commctrl.h>
#include <bits/stdc++.h>
#include <setupapi.h>
#include "wimlib.h"
#include "Resource.h"
#define print(x) MessageBoxA(0, to_string(x).c_str(), "tip", 0)
#define ext_imageres(x) ExtractIcon(GetModuleHandle(NULL), \
TEXT("C:\\Windows\\System32\\imageres.dll"), x)
#define ext_shell32(x) ExtractIcon(GetModuleHandle(NULL), \
TEXT("C:\\Windows\\System32\\shell32.dll"), x)
using namespace std;
// HFONT globalFont;
HICON globalIcon;
HINSTANCE g_Inst;
const TCHAR* winClassName = TEXT("WinSetupWindowClass");
class RootOption {
private:
    string btnCaption;
public:
    HWND btnID;
    HICON icon;
    wstring title;
    wstring description;
    HWND parent;
    int i;
    void init() {
        btnID = CreateWindowW(L"BUTTON", title.c_str(), WS_CHILD | WS_VISIBLE | BS_COMMANDLINK | BS_VCENTER | BS_RIGHT,
            0, i * 100, 100 * 5.618, 100, parent, (HMENU)(i + 1700), NULL, NULL);
        // globalFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
        //     ANSI_CHARSET, OUT_DEFAULT_PRECIS,
        //     CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        //     DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
        // MessageBoxA(0, to_string(GetLastError()).c_str(), "tip", 0);
        SendMessage(btnID, BM_SETIMAGE, IMAGE_ICON, (LPARAM)icon);
        SendMessage(btnID, BCM_SETNOTE, 0, (LPARAM)(description.c_str()));
    }
};
RootOption ro[8];
// CreateListView: Creates a list-view control in report view.
// Returns the handle to the new control
// TO DO:  The calling procedure should determine whether the handle is NULL, in case 
// of an error in creation.
//
// HINST hInst: The global handle to the application instance.
// HWND  hWndParent: The handle to the control's parent window. 
//
HWND lv_img, lv_dri;
// char mImgName[1024], mImgDesc[1024];
wchar_t mImgName[1024], mImgDesc[1024];
// wchar_t imgPath[MAX_PATH] = L"G:\\sources\\install.esd";
wchar_t imgPath[MAX_PATH] = L"boot.wim";
// wchar_t installPath[MAX_PATH] = L"S:\\";
wchar_t installPath[MAX_PATH] = L"C:\\Users\\Administrator\\Documents\\WinSetup\\mount";
UINT img_n;
int selectedIndex = 1;
#define ListView_InsertColumnA(hwnd,iCol,pcol) (int)SendMessageA((hwnd),LVM_INSERTCOLUMN,(WPARAM)(int)(iCol),(LPARAM)(const LV_COLUMNA *)(pcol))
#define ListView_InsertColumnW(hwnd,iCol,pcol) (int)SendMessageW((hwnd),LVM_INSERTCOLUMN,(WPARAM)(int)(iCol),(LPARAM)(const LV_COLUMNW *)(pcol))
#define ListView_SetItemW(hwnd,pitem) (WINBOOL)SendMessageW((hwnd),LVM_SETITEMW,0,(LPARAM)(const LV_ITEMW *)(pitem))
#define ListView_InsertItemW(hwnd,pitem) (int)SendMessageW((hwnd),LVM_INSERTITEMW,0,(LPARAM)(const LV_ITEMW *)(pitem))

WIMStruct* wim = NULL;
wimlib_wim_info info;
UINT img_cnt;
UINT dri_cnt;
void InitImageList(HWND dlg) {

    LVITEMW lvI = { 0 };
    // Initialize LVITEM members that are common to all items.
    // lvI.pszText = LPSTR_TEXTCALLBACK; // Sends an LVN_GETDISPINFO message.
    lvI.mask = LVIF_TEXT | LVIF_STATE;
    lvI.cchTextMax = 1024;
    wchar_t num_buf[16];
    for (UINT i = 1; i <= img_cnt; ++i) {
        lvI.iItem = i - 1;
        PCWSTR imgName = wimlib_get_image_name(wim, i);
        PCWSTR imgDesc = wimlib_get_image_description(wim, i);
        // MessageBoxW(0, imgDesc, imgName, 0);
        wcscpy_s<16>(num_buf, to_wstring(i).c_str());
        wcscpy_s<1024>(mImgName, imgName);
        wcscpy_s<1024>(mImgDesc, imgDesc);

        lvI.iSubItem = 0;
        lvI.pszText = num_buf;
        int ret = ListView_InsertItemW(lv_img, &lvI);
        lvI.iSubItem = 1;
        lvI.pszText = mImgName;
        ret = ListView_SetItemW(lv_img, &lvI);
        lvI.iSubItem = 2;
        lvI.pszText = mImgDesc;
        ret = ListView_SetItemW(lv_img, &lvI);
    }
}
void idl(HWND dlg) {

    LVITEMW lvI = { 0 };
    // Initialize LVITEM members that are common to all items.
    // lvI.pszText = LPSTR_TEXTCALLBACK; // Sends an LVN_GETDISPINFO message.
    // lvI.mask = LVIF_TEXT | LVIF_STATE;
    // lvI.cchTextMax = 1024;
    // wchar_t num_buf[16];
    // PCWSTR imgName = wimlib_get_image_name(wim, i);
    // PCWSTR imgDesc = wimlib_get_image_description(wim, i);
    // // MessageBoxW(0, imgDesc, imgName, 0);
    // wcscpy_s<16>(num_buf, to_wstring(i).c_str());
    // wcscpy_s<1024>(mImgName, imgName);
    // wcscpy_s<1024>(mImgDesc, imgDesc);

    // lvI.iSubItem = 0;
    // lvI.pszText = num_buf;
    // int ret = ListView_InsertItemW(lv_dri, &lvI);
    // lvI.iSubItem = 1;
    // lvI.pszText = mImgName;
    // ret = ListView_SetItemW(lv_dri, &lvI);
}

wchar_t szFile[MAX_PATH];       // buffer for file name
bool LoadDriverINF(const wchar_t* infPath) {
    // 获取系统的设备信息集
    // HDEVINFO deviceInfoSet = SetupOpenMasterInfCatalog(NULL, 0);
    // if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        // 错误处理
        // return false;
    // }

    // 添加指定的INF文件到系统中
    if (!SetupCopyOEMInf(infPath, NULL, COPY_INF_ALL_FILES, NULL, NULL)) {
        // 错误处理
        // SetupCloseMasterInfCatalog(deviceInfoSet);
        return false;
    }

    // 注册表中添加相关的INF文件信息
    if (!SetupInstallFilesFromInfSection(NULL, NULL, INF_STYLE_WIN4, infPath, NULL, 0, NULL, NULL)) {
        // 错误处理
        // SetupCloseMasterInfCatalog(deviceInfoSet);
        return false;
    }

    // // 关闭设备信息集
    // if (!SetupCloseMasterInfCatalog(deviceInfoSet)) {
    //     // 错误处理
    //     return false;
    // }

    return true;
}
//Powered By WXYY



void import_new(HWND dlg) {

    OPENFILENAMEW ofn;       // common dialog box structure
    HWND hwnd;              // owner window
    HANDLE hf;              // file handle

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = dlg;
    ofn.lpstrFile = szFile;
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"INF File\0*.inf\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the Open dialog box. 

    if (GetOpenFileNameW(&ofn) == FALSE) return;
    bool flag = (IDYES == MessageBoxW(dlg, L"是否需要在目标系统加载驱动?", L"Question", MB_YESNO));
    SHELLEXECUTEINFOW shiw = { 0 };
    shiw.cbSize = sizeof(shiw);
    shiw.hInstApp = g_Inst;
    shiw.hwnd = dlg;
    shiw.lpFile = szFile;
    shiw.lpVerb = L"INSTALL";
    shiw.nShow = SW_SHOW;
    BOOL hr = ShellExecuteExW(&shiw);
    if (!hr) {
        TaskDialog(dlg, g_Inst, L"错误出了点小问题", L"应用程序启动失败", L":(", NULL, NULL, NULL);
        return;
    }
    HANDLE hProcess = shiw.hProcess;
    //RUNDLL32.EXE SETUPAPI.DLL,InstallHinfSection DefaultInstall 132 path-to-inf\infname.inf

    DWORD ret = WaitForSingleObject(hProcess, 60u * 1000u);    //天荒地老，么？1min!
    if (!ret) {
        TaskDialog(dlg, g_Inst, L"错误出了点小问题 - QwQ", L"您是否提升了管理员?", to_wstring((unsigned)ret).c_str(), NULL, NULL, NULL);
        return;
    }
    if (!flag) {
        //ShellExecuteW(dlg, L"INSTALL", szFile, NULL, NULL, SW_SHOW);
        // DismSession sessionOnline = DISM_SESSION_DEFAULT;
        // HRESULT hr = DismInitialize(DismLogErrorsWarningsInfo, L"C:\\log_qwq.txt", NULL);
        // if (hr != S_OK) {
        //     TaskDialog(dlg, g_Inst, L"错误出了点小问题 - QwQ - Layer 1", L"您是否提升了管理员?", to_wstring((unsigned)hr).c_str(), NULL, NULL, NULL);
        //     return;
        // }

        // hr = DismOpenSession(DISM_ONLINE_IMAGE,
        //     NULL,
        //     NULL,
        //     &sessionOnline);
        // if (hr != S_OK) {
        //     TaskDialog(dlg, g_Inst, L"错误出了点小问题 - Layer 2", L"QwQ", to_wstring((unsigned)hr).c_str(), NULL, NULL, NULL);
        //     return;
        // }
        // hr = DismAddDriver(sessionOnline, szFile, true);
        // if (hr != S_OK) {
        //     TaskDialog(dlg, g_Inst, L"错误出了点小问题 - Layer 3", L"QwQ", to_wstring((unsigned)hr).c_str(), NULL, NULL, NULL);
        //     return;
        // }

        // TaskDialog(dlg, g_Inst, L"好辣", L"淡定", to_wstring((unsigned)hr).c_str(), NULL, NULL, NULL);
        return;
    }
    TaskDialog(dlg, g_Inst, L"QWQ", L"WTF", L"AWA", NULL, NULL, NULL);
    ++dri_cnt;
    LVITEMW lvI = { 0 };
    // Initialize LVITEM members that are common to all items.
    // lvI.pszText = LPSTR_TEXTCALLBACK; // Sends an LVN_GETDISPINFO message.
    lvI.mask = LVIF_TEXT | LVIF_STATE;
    lvI.cchTextMax = 1024;
    wchar_t num_buf[16];
    lvI.iItem = dri_cnt;
    wsprintfW(num_buf, L"%u", dri_cnt);
    lvI.iSubItem = 0;
    lvI.pszText = num_buf;
    int ret = ListView_InsertItemW(lv_dri, &lvI);
    lvI.iSubItem = 1;
    lvI.pszText = mImgName;
    ret = ListView_SetItemW(lv_dri, &lvI);
}
LRESULT CALLBACK DlgImageProc(HWND dlg, UINT Message, WPARAM wParam, LPARAM lParam) {
    // print(Message);
    switch (Message) {
        case WM_INITDIALOG: {
            lv_img = GetDlgItem(dlg, IDC_LSV_IMG);
            ListView_SetExtendedListViewStyle(lv_img, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
            do {
                char Keys[3][12] = { "Index", "Name", "Description" };
                LV_COLUMNA lvc = { 0 };
                lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
                lvc.iSubItem = 0;
                lvc.pszText = Keys[0];
                lvc.cx = 50;
                int x = ListView_InsertColumnA(lv_img, 0, &lvc);    //我不相信意外, 100%
                // print(x);
                lvc.iSubItem = 1;
                lvc.pszText = Keys[1];
                lvc.cx = 200;
                ListView_InsertColumnA(lv_img, 1, &lvc);    //我不相信意外, 100% * 2
                lvc.iSubItem = 2;
                lvc.pszText = Keys[2];
                lvc.cx = 250;
                ListView_InsertColumnA(lv_img, 2, &lvc);    //我不相信意外, 100% * 3
            } while (0);    //Column
            // DismImageInfo* img_inf = NULL;
            //1~n
            // HRESULT ret = DismGetImageInfo(imgPath, &img_inf, &img_n);
            // for (int j = 0; j < img_n; ++j, ++img_inf) {
            //     PCWSTR imgName = img_inf->ImageName;
            //     PCWSTR imgDesc = img_inf->ImageDescription;
            //     memset(mImgName, 0, sizeof(mImgName));
            //     memset(mImgDesc, 0, sizeof(mImgDesc));
            //     // sprintf(mImgName, "%S", imgName);
            //     // sprintf(mImgDesc, "%S", imgDesc);
            //     WideCharToMultiByte(936, 0, imgName, wcslen(imgName), mImgName, 1024, 0, 0);
            //     WideCharToMultiByte(936, 0, imgDesc, wcslen(imgDesc), mImgDesc, 1024, 0, 0);
            //     // wcstombs(mImgName, imgName, wcslen(imgName));
            //     // wcstombs(mImgDesc, imgDesc, wcslen(imgDesc));
            //     // MessageBoxW(0, imgDesc, imgName, 0);
            //     // MessageBoxA(0, mImgDesc, mImgName, 0);
            //     lvI.iItem = j;
            //     lvI.iSubItem = 0;
            //     strcpy(num_buf, to_string(j).c_str());
            //     lvI.pszText = num_buf;
            //     int r;
            //     r = ListView_InsertItem(lv_img, &lvI);
            //     lvI.iSubItem = 1;
            //     lvI.pszText = mImgName;
            //     r = ListView_SetItem(lv_img, &lvI);
            //     // print(r);
            //     lvI.iSubItem = 2;
            //     lvI.pszText = mImgDesc;
            //     r = ListView_SetItem(lv_img, &lvI);
            //     // print(r);

            // }
            InitImageList(dlg);
            ListView_SetItemState(lv_img, selectedIndex - 1, LVIS_SELECTED, LVIS_SELECTED);
            ListView_SetSelectionMark(lv_img, selectedIndex - 1);
            SetFocus(lv_img);
            break;
        }
        case WM_COMMAND: {
            // MessageBoxA(hwnd, to_string(LOWORD(wParam)).c_str(), "LN79", 0);
            switch (LOWORD(wParam)) {
                case IDC_BTN1: {
                    SendMessage(dlg, WM_CLOSE, 0, 0);
                    break;
                }
            }
            break;

        }
        case WM_CLOSE: {
            selectedIndex = ListView_GetSelectionMark(lv_img) + 1;
            print(selectedIndex);
            EndDialog(dlg, 0);
            break;
        }
    }
    return 0;
}
HWND ProgressBar, ProgressText;
// void dism_progress_callback(UINT Current, UINT Total, PVOID UserData) {
//     print(Current);
//     SendMessage(ProgressBar, PBM_SETRANGE32, 0, Total);
//     SendMessage(ProgressBar, PBM_SETSTEP, (WPARAM)1, 0);
//     SendMessage(ProgressBar, PBM_SETPOS, (WPARAM)Current, 0);
//     sprintf_s<16>(buf_percent, "%d/%d %.2f%%", Current, Total, Current * 100.0d / Total);
//     SetWindowTextA(ProgressText, buf_percent);
// }

#define TO_PERCENT(numerator, denominator) \
	((float)(((denominator) == 0) ? 0 : ((numerator) * 100 / (float)(denominator))))
char percent_buf[128];
static wimlib_progress_status
extract_progress(wimlib_progress_msg msg,
    wimlib_progress_info* info, void* progctx)
{
    wimlib_progress_info::wimlib_progress_info_extract X = info->extract;
    uint64_t Current = X.completed_bytes, Total = X.total_bytes;
    switch (msg) {
        case WIMLIB_PROGRESS_MSG_EXTRACT_STREAMS:
            // printf("Extracting files: %.2f%% complete\n",
            //     TO_PERCENT(info->extract.completed_bytes, info->extract.total_bytes));
            // printf("Extracting files: %.2f%% complete\n",
            //     TO_PERCENT(info->extract.completed_bytes, info->extract.total_bytes));
            SendMessage(ProgressBar, PBM_SETRANGE32, 0, Total);
            SendMessage(ProgressBar, PBM_SETSTEP, (WPARAM)1, 0);
            SendMessage(ProgressBar, PBM_SETPOS, (WPARAM)Current, 0);
            sprintf(percent_buf, "%.2f%%(%12llu/%12llu)", TO_PERCENT(Current, Total), Current, Total);
            SetWindowTextA(ProgressText, percent_buf);

            break;
        default:
            break;
    }
    return WIMLIB_PROGRESS_STATUS_CONTINUE;
}

DWORD install(LPVOID param) {
    // HRESULT hr = DismMountImage(imgPath,
    //     installPath,
    //     selectedIndex + 1,
    //     NULL,
    //     DismImageIndex,
    //     DISM_MOUNT_READONLY,
    //     NULL,
    //     NULL,
    //     NULL);
    // print(hr);
    // DismSession session = DISM_SESSION_DEFAULT;
    // // hr = DismOpenSession(installPath,
    // //     NULL,
    // //     NULL,
    // //     &session);

    // // print(hr);
    // hr = DismCommitImage(session, DISM_DISCARD_IMAGE, NULL, dism_progress_callback, NULL);
    // print(hr);
    // MessageBoxA(0, "successs", "succeed", 0);

    /* Register our progress function.  */
    wimlib_register_progress_function(wim, extract_progress, NULL);

    /* Extract the first image.  */
    int ret = wimlib_extract_image(wim,     /* WIMStruct from which to extract the image  */
        selectedIndex,       /* Image to extract  */
        installPath, /* Directory to extract the image to  */
        0);      /* WIMLIB_EXTRACT_FLAG_* flags (0 means all defaults)  */



    if (~ret) {
        HWND dlg = (HWND)param;
        MessageBoxW(dlg, L"安装成功", L"提示", MB_ICONINFORMATION);
    }
    return 0;
}

LRESULT CALLBACK DlgInstallProc(HWND dlg, UINT Message, WPARAM wParam, LPARAM lParam) {
    // print(Message);
    switch (Message) {
        case WM_INITDIALOG: {
            ProgressBar = GetDlgItem(dlg, IDC_PGB1);
            ProgressText = GetDlgItem(dlg, IDC_PROG_TITLE1);
            CreateThread(NULL, NULL, install, (LPVOID)dlg, NULL, NULL);
            return 0;
        }
        case WM_CLOSE: {
            EndDialog(dlg, 0);
            break;
        }
    }
    return 0;
}
LRESULT CALLBACK DlgDriverProc(HWND dlg, UINT Message, WPARAM wParam, LPARAM lParam) {
    // print(Message);
    switch (Message) {
        case WM_INITDIALOG: {

            lv_dri = GetDlgItem(dlg, IDC_LSV_DRI);
            ListView_SetExtendedListViewStyle(lv_dri, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
            do {
                char Keys[2][12] = { "Index", "Location" };
                LV_COLUMNA lvc = { 0 };
                lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
                lvc.iSubItem = 0;
                lvc.pszText = Keys[0];
                lvc.cx = 50;
                int qwq = ListView_InsertColumnA(lv_dri, 0, &lvc);    //我不相信意外, 100%
                print(qwq);
                lvc.iSubItem = 1;
                lvc.pszText = Keys[1];
                lvc.cx = 200;
                qwq = ListView_InsertColumnA(lv_dri, 1, &lvc);    //我不相信意外, 100% * 2
                //现在我相信了
                print(qwq);
                // import_new(dlg);
            } while (0);    //Column
            idl(dlg);
            return 0;
        }
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDC_BTN2: {
                    import_new(dlg);
                    break;
                }
            }
            break;
        }
        case WM_CLOSE: {
            EndDialog(dlg, 0);
            break;
        }
    }
    return 0;
}
DWORD DlgThreadLocate(LPVOID param) {
    DialogBoxW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDD_DLG1), (HWND)param, DlgImageProc);
    return 0;
}
DWORD DlgThreadDriver(LPVOID param) {
    DialogBoxW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDD_DLG2), (HWND)param, DlgDriverProc);
    return 0;

}
DWORD DlgThreadInstall(LPVOID param) {
    DialogBoxW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDD_DLG3), (HWND)param, DlgInstallProc);
    return 0;
}
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    switch (Message) {
        case WM_CREATE: {
            do {
                ro[0].icon = ext_imageres(56);
                ro[0].title = L"寻找镜像";
                ro[0].description = L"选定变体";
            } while (0);
            do {
                ro[1].icon = ext_imageres(185);
                ro[1].title = L"驱动安装";
                ro[1].description = L"为未预装驱动的系统安装驱动(通常是虚拟机)";
            } while (0);
            do {
                ro[2].icon = ext_shell32(8);
                ro[2].title = L"磁盘管理";
                ro[2].description = L"请注意: 请调整至系统根目录可访问";
            } while (0);
            do {
                ro[3].icon = ext_imageres(31);
                ro[3].title = L"选择目标分区";
                ro[3].description = L"请再次确认您以管理员身份运行";
            } while (0);
            do {
                ro[4].title = L"开始安装";
                ro[4].description = L"过程耗时较久，请耐心等待";
            } while (0);
            for (int i = 0; i < 5; ++i) ro[i].i = i, ro[i].parent = hwnd, ro[i].init();

            // SendMessage(hwnd, WM_SETFONT, (WPARAM)globalFont, true);
            break;
        }
                      // case WM_PAINT: {
                      //     PAINTSTRUCT ps;
                      //     HDC hdc = BeginPaint(hwnd, &ps);
                      //     HGDIOBJ oldFont = SelectObject(hdc, hFont);
                      //     TextOutW(hdc, 0, 0, L"我爱学习", 4);
                      //     SelectObject(hdc, oldFont);
                      //     EndPaint(hwnd, &ps);
                      //     break;
                      // }
        case WM_COMMAND: {
            // MessageBoxA(hwnd, to_string(LOWORD(wParam)).c_str(), "LN79", 0);
            switch (LOWORD(wParam)) {
                case 1700: {
                    // int ret;
                    // TCHAR* wimfile;
                    // TCHAR* wim_target_path;
                    // TCHAR* fs_source_path;
                    // WIMStruct* wim = NULL;
                    // // wimlib_update_command cmds[1];
                    // wimlib_wim_info info;
                    // ret = wimlib_open_wim(L"G:\\sources\\boot.wim", 0, &wim);
                    // if (ret) {
                    //     wimlib_get_wim_info(wim, &info);
                    // }
                    // MessageBoxA(0, to_string(info.image_count).c_str(), "tip", 0);
                    CreateThread(NULL, NULL, DlgThreadLocate, (LPVOID)hwnd, NULL, NULL);
                    break;
                }
                case 1701: {
                    CreateThread(NULL, NULL, DlgThreadDriver, (LPVOID)hwnd, NULL, NULL);

                    break;
                }
                case 1702: {
                    ShellExecuteW(hwnd, L"OPEN", L"C:\\Windows\\System32\\mmc.exe", L"diskmgmt.msc", NULL, 1);
                    break;
                }
                case 1704: {
                    CreateThread(NULL, NULL, DlgThreadInstall, (LPVOID)hwnd, NULL, NULL);
                    break;
                }
            }
            break;
        }
        case WM_DESTROY: {
            /* Upon destruction, tell the main thread to stop */
            PostQuitMessage(0);
            break;
        }
        default:
            /* All other messages (a lot of them) are processed using default procedures */
            return DefWindowProc(hwnd, Message, wParam, lParam);
    }
    return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc; /* A properties struct of our window */
    HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
    MSG msg; /* A temporary location for all messages */
    globalIcon = ext_imageres(56);
    g_Inst = hInstance;


    int ret = wimlib_open_wim(imgPath, 0, &wim);

    ret = wimlib_get_wim_info(wim, &info);
    img_cnt = info.image_count;
    // HRESULT hr = DismInitialize(DismLogErrorsWarningsInfo, L"setup-log.txt", NULL);
    // globalFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
    //     ANSI_CHARSET, OUT_DEFAULT_PRECIS,
    //     CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
    //     DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));




    SetProcessDPIAware();
    // MessageBoxA(0, to_string((UINT)globalFont).c_str(), "tip", 0);
    /* zero out the struct and set the stuff we want to modify */
    memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc; /* This is where we will send messages to */
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    /* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = winClassName;
    // wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
    // wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */
    wc.hIcon = globalIcon; /* Load a standard icon */
    wc.hIconSm = globalIcon; /* use the name "A" to use the project icon */

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, TEXT("Class Registration Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, winClassName, TEXT("Windows Setup"), WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, /* x */
        CW_USEDEFAULT, /* y */
        100 * 5.618, /* width */
        100 * 5 + 55, /* height */
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

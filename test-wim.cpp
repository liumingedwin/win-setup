#include <windows.h>
#include <dismapi.h>
#include <cstdio>
#include "wimlib.h"
int main(void) {

    HRESULT hr = S_OK;
    // hr = DismInitialize(DismLogErrorsWarningsInfo, L"MyLogFile.txt", NULL);
    printf("5");
    int ret;
    TCHAR* wimfile;
    printf("9");
    TCHAR* wim_target_path;
    TCHAR* fs_source_path;
    WIMStruct* wim = NULL;
    // wimlib_update_command cmds[1];
    wimlib_wim_info info;
    // HMODULE wimModule = LoadLibraryW(L"./libwim-15.dll");
    // WIMLIBAPI int
    // (*wimlib_open_wim)(const wimlib_tchar * wim_file,
    //     int open_flags,
    //     WIMStruct * *wim_ret) = GetProcAddress(wimModule, "wimlib_open_wim");

    // WIMLIBAPI int
    // (*wimlib_get_wim_info)(WIMStruct * wim, struct wimlib_wim_info* info) = GetProcAddress(wimModule, "wimlib_get_wim_info");

    // WIMLIBAPI const wimlib_tchar*
    //     (*wimlib_get_image_description)(const WIMStruct * wim, int image) = GetProcAddress(wimModule, "wimlib_get_image_description");
    // WIMLIBAPI void
    // (*wimlib_free)(WIMStruct * wim) = GetProcAddress(wimModule, "wimlib_free");
    // WIMLIBAPI const wimlib_tchar*
    //     (*wimlib_get_image_name)(const WIMStruct * wim, int image) = GetProcAddress(wimModule, "wimlib_get_image_name");
    // printf("%u\n", wimlib_open_wim);
    ret = wimlib_open_wim(L"G:\\sources\\install.esd", 0, &wim);
    printf("ret: %d\n", ret);
    if (!ret) {
        ret = wimlib_get_wim_info(wim, &info);
        if (!ret) {
            UINT n = info.image_count;

            for (int i = 1; i <= n; ++i) {
                MessageBoxW(0, wimlib_get_image_description(wim, i), wimlib_get_image_name(wim, i), 0);
                // wprintf(L"%s\n", wimlib_get_image_name(wim, i));
            }
            // wimlib_register_progress_function(wim, extract_progress, NULL);

            // /* Extract the first image.  */
            // ret = wimlib_extract_image(wim,     /* WIMStruct from which to extract the image  */
            //     1,       /* Image to extract  */
            //     "", /* Directory to extract the image to  */
            //     0);      /* WIMLIB_EXTRACT_FLAG_* flags (0 means all defaults)  */

            wimlib_free(wim);

            // DismImageInfo* ImgInf = NULL;
            // //1~n
            // UINT qwq = 3;
            // ret = DismGetImageInfo(L"G:\\sources\\install.esd", &ImgInf, &qwq);
            // for (int j = 0; j < n; ++j, ++ImgInf) {
            //     PCWSTR ps = ImgInf->ImageName;
            //     // for (int i = 0; ps[i] != '\0'; ++i) {
            //     //     wprintf(L"%c", ps[i]);
            //     // }
            //     MessageBoxW(0, ImgInf->ImageDescription, ImgInf->ImageName, 0);
            // }
        }
    }
    return 0;
}
#include "windows.h"
#include <stdio.h>
#include "DismApi.h"

int main()
{
    HRESULT hr = S_OK;
    HRESULT hrLocal = S_OK;
    DismSession session = DISM_SESSION_DEFAULT;
    DismSession sessionOnline = DISM_SESSION_DEFAULT;
    BOOL bMounted = FALSE;
    DismFeatureInfo* pFeatureInfo = NULL;
    DismString* pErrorString = NULL;

    // Initialize the API
    hr = DismInitialize(DismLogErrorsWarningsInfo, L"testMyLogFile.txt", NULL);
    if (FAILED(hr))
    {
        wprintf(L"DismInitialize Failed: %x\n", hr);
        goto Cleanup;
    }

    wprintf(L"Mounting: ");

    // Mount a VHD image 
    hr = DismMountImage(L"G:\\sources\\boot.wim",
        L"C:\\Users\\Administrator\\Documents\\WinSetup\\mount",
        1,
        NULL,
        DismImageIndex,
        DISM_MOUNT_READONLY,
        NULL,
        NULL,
        NULL);
    if (FAILED(hr))
    {
        wprintf(L"\nDismMountImage Failed: %x\n", hr);
        goto Cleanup;
    }

    bMounted = TRUE;
    wprintf(L"Complete.\n\n");

    // Open a session against the mounted image
    hr = DismOpenSession(L"C:\\Users\\Administrator\\Documents\\WinSetup\\mount",
        NULL,
        NULL,
        &session);
    if (FAILED(hr))
    {
        wprintf(L"DismOpenSession Failed: %x\n", hr);
        goto Cleanup;
    }

    // Apply an unattend file to the offline image
    hr = DismApplyUnattend(session,
        L"C:\\unattend.xml",
        FALSE);
    if (FAILED(hr))
    {
        wprintf(L"DismApplyUnattend Failed: %x\n", hr);
        goto Cleanup;
    }

    // Also open a session against the running operating system (online image) 
    hr = DismOpenSession(DISM_ONLINE_IMAGE,
        NULL,
        NULL,
        &sessionOnline);
    if (FAILED(hr))
    {
        wprintf(L"DismOpenSession Failed: %x\n", hr);
        goto Cleanup;
    }

    // Get the feature info for a non-existent feature to demonstrate error
    // functionality
    hr = DismGetFeatureInfo(sessionOnline,
        L"ThisIsAFakeFeatureName",
        NULL,
        DismPackageNone,
        &pFeatureInfo);
    if (FAILED(hr))
    {
        wprintf(L"DismGetFeatureInfo Failed: %x\n", hr);

        hrLocal = DismGetLastErrorMessage(&pErrorString);
        if (FAILED(hrLocal))
        {
            wprintf(L"DismGetLastErrorMessage Failed: %x\n", hr);
            goto Cleanup;
        }
        else
        {
            wprintf(L"The last error string was: %s\n", pErrorString->Value);
        }
    }


Cleanup:

    // Delete the memory associated with the objects that were returned. 
    // pFeatureInfo should be NULL due to the expected failure above, but the
    // DismDelete function will still return success in this case. 
    hrLocal = DismDelete(pFeatureInfo);
    if (FAILED(hrLocal))
    {
        wprintf(L"DismDelete Failed: %x\n", hrLocal);
    }

    hrLocal = DismDelete(pErrorString);
    if (FAILED(hrLocal))
    {
        wprintf(L"DismDelete Failed: %x\n", hrLocal);
    }

    // Close the DismSession to free up resources tied to the offline session
    hrLocal = DismCloseSession(session);
    if (FAILED(hrLocal))
    {
        wprintf(L"DismCloseSession Failed: %x\n", hrLocal);
    }

    // Close the DismSession to free up resources tied to the online session
    hrLocal = DismCloseSession(sessionOnline);
    if (FAILED(hrLocal))
    {
        wprintf(L"DismCloseOnlineSession Failed: %x\n", hrLocal);
    }

    // Unmount the image if is has been mounted.  If the package was successfully added,
    // then commit the changes.  Otherwise, discard the
    // changes
    if (bMounted)
    {
        hrLocal = DismUnmountImage(L"C:\\Users\\Administrator\\Documents\\WinSetup\\mount",
            DISM_DISCARD_IMAGE,
            NULL,
            NULL,
            NULL);
        if (FAILED(hrLocal))
        {
            wprintf(L"DismUnmountImage Failed: %x\n", hrLocal);
        }
    }

    // Shutdown the DISM API to free up remaining resources
    hrLocal = DismShutdown();
    if (FAILED(hrLocal))
    {
        wprintf(L"DismShutdown Failed: %x\n", hr);
    }

    wprintf(L"Return code is: %x\n", hr);
    return hr;
}
/* 2008-2009 (C) Upek, inc.
 * BSAPI sample Grab
 * 
 * This sample presents usage of BSAPI grabbing functions.
 */

#include <windows.h>
#include <tchar.h>
#include <stdio.h>  /* _snprintf() */

#include <bsapi.h>
#include <bsgui.h>


/* Used to supress compiler warnings */
#ifndef UNREFERENCED_PARAMETER
    #define UNREFERENCED_PARAMETER(param)   (void)param
#endif


static HINSTANCE hInstance;

/* Main window handle */
static HWND hMainWnd;

/* Control IDs of buttons on the main window */
#define IDC_START            500
#define IDC_STOP             501


/* BSAPI connection handle */
static ABS_CONNECTION conn;

/* An image we work with. In this example it is output of ABSGrab(), 
 * ABSRawGrab() and it can be converted to black and white by 
 * ABSBinarizeSampleImage(). */
static ABS_SAMPLE_IMAGE* pImage = NULL;

/* ABS_OPERATION structure for all interactive operations - ABSRawGrab() in this sample. 
 * Callbacks are ignored in this sample, ABS_OPERATION_FLAG_LL_CALLBACK removes 1.2s delay making callbacks readable.
 */
static ABS_OPERATION op = { 1, NULL, NULL, 0, ABS_OPERATION_FLAG_LL_CALLBACK };

/* Handle for a thread executing rawGrab.
*/
static HANDLE hGrabThread = 0;
static DWORD grabThreadId = 0;


/* Helper function displaying a dialog with error message if some ABSAPI
 * function fails. */
static void Error(const char* str, ABS_STATUS status)
{
    ABS_DWORD code;
    const char* msg;
    char buffer[1024];

    ABSGetLastErrorInfo(&code, &msg);    
    _snprintf(buffer, 1024, "%s\n\nStatus: %ld\nCode: %ld\nMessage: %s",
               str, (long)status, (long)code, msg);               
    MessageBoxA(hMainWnd, buffer, "BSAPI Error", MB_OK | MB_ICONERROR);
}

/** Dummy implementation of callbacks so no notification comes. 
 */
void __stdcall guiCallback(const ABS_OPERATION *pOperation,IN ABS_DWORD dwMsgID,IN void *pMsgData)
{
}

/* Thread execution function calling repeatedly ABSRawGrab().
 */
DWORD WINAPI RawGrabExec( LPVOID lpParam )
{
    ABS_STATUS status = ABS_STATUS_OK;
    ABS_PROFILE_DATA profile[] = {
        {ABS_PKEY_IMAGE_FORMAT, ABS_PVAL_IFMT_508_508_8_SCAN508_508_8},     // 508 dpi image
        {ABS_PKEY_WAIT_FOR_ACCEPTABLE, 0}   // Skip image quality check and image post-processing
    };

    if(pImage != NULL) {
        ABSFree(pImage);
        pImage = NULL;
    }
    
    op.Callback = guiCallback;
    
    while(status == ABS_STATUS_OK)
    {
        status = ABSRawGrab(conn, &op, sizeof(profile) / sizeof(profile[0]), 
            profile, &pImage, NULL, 0);
            
        if((status != ABS_STATUS_OK) && (status != ABS_STATUS_CANCELED))
        {
            Error("ABSRawGrab() failed.", status);
        }
        
        // Redraw image   
        InvalidateRect(hMainWnd, NULL, TRUE);
    }   

    hGrabThread = 0;
    
    return 0;
}

/* This function starts thread performing ABSRawGrab() when user clicks on the START button */
static void Start(void)
{   
    if(IsWindowEnabled(GetDlgItem(hMainWnd, IDC_START)))
    {
        // Enable/disable control buttons
        EnableWindow(GetDlgItem(hMainWnd, IDC_START), 0);
        EnableWindow(GetDlgItem(hMainWnd, IDC_STOP), 1);

        // Create new execution thread
        hGrabThread = CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
            RawGrabExec,            // thread function name
            0,                      // argument to thread function 
            0,                      // use default creation flags 
            &grabThreadId);         // returns the thread identifier
    }
       
}

/* This function stop thread performing ABSRawGrab() when user clicks on the STOP or CLOSE button */
static void Stop(void)
{
    ABS_STATUS status;
    
    if(hGrabThread != 0)
    {
        status = ABSCancelOperation(conn, op.OperationID);
        if(status != ABS_STATUS_OK)
            Error("ABSCancelOperation() failed.", status);
            
        WaitForSingleObject(hGrabThread, INFINITE);
        
        EnableWindow(GetDlgItem(hMainWnd, IDC_START), 1);
        EnableWindow(GetDlgItem(hMainWnd, IDC_STOP), 0);
    }    
}


/* Paints the fingerprint sample image. */
static void Paint(void)
{
    HDC hDC;
    PAINTSTRUCT ps;
    char bmpBuffer[sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD)] = { 0 };
    BITMAPINFO* pBmpInfo;
    BYTE* pBmpData;
    ABS_DWORD i;
    
    if(pImage == NULL)
        return;
    
    /* Create windows bitmap header in memory for our sample image. */
    pBmpInfo = (BITMAPINFO*) bmpBuffer;
    pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pBmpInfo->bmiHeader.biWidth = pImage->Width;
    /* The following minus sign tells GDI to draw it in the right vertical 
     * direction: */
    pBmpInfo->bmiHeader.biHeight = -((LONG)pImage->Height);
    pBmpInfo->bmiHeader.biPlanes = 1;
    pBmpInfo->bmiHeader.biBitCount = 8;
    pBmpInfo->bmiHeader.biCompression = BI_RGB;
    /* The colormap of the bitmap has to contain grayscale colors,
     * spread lineary from black (0) to white (pImage->ColorCount - 1) */
    for(i = 0; i < pImage->ColorCount; i++) {
        BYTE byGray = (BYTE) ((255 * i) / (pImage->ColorCount - 1));
        pBmpInfo->bmiColors[i].rgbRed = byGray;
        pBmpInfo->bmiColors[i].rgbGreen = byGray;
        pBmpInfo->bmiColors[i].rgbBlue = byGray;
    }
    
    /* Each scan line of the image must end on 4-byte boundary, so if
     * width of the image is not divisible by 4, we have to rearrange 
     * the bytes in a temporary buffer. */
    if(pImage->Width % 4 == 0) {
        pBmpData = NULL;
    } else {
        ABS_DWORD scanLineLen;
        ABS_DWORD scanLineNo;
        
        scanLineLen = pImage->Width + (4 - pImage->Width % 4);        
        pBmpData = (BYTE*) malloc(scanLineLen * pImage->Height);
        for(scanLineNo = 0; scanLineNo < pImage->Height; scanLineNo++) {
            memcpy(
                pBmpData + scanLineLen * scanLineNo, 
                pImage->ImageData + pImage->Width * scanLineNo,
                pImage->Width
            );
        }
    }
    
    /* Draw the bitmap to the window */
    hDC = BeginPaint(hMainWnd, &ps);
    StretchDIBits(hDC, 
        175, 10, pImage->Width, pImage->Height, 
        0, 0, pImage->Width, pImage->Height, 
        (void*) (pBmpData != NULL ? pBmpData : pImage->ImageData),
        pBmpInfo,
        DIB_RGB_COLORS,
        SRCCOPY
    );
    EndPaint(hMainWnd, &ps);
    
    /* If we used the temporary buffer for image data, we have to release it */
    if(pBmpData != NULL)
        free(pBmpData);
}

static LRESULT CALLBACK MainWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg) {
        case WM_PAINT:
            Paint();
            break;
        
        case WM_COMMAND:
            /* Do the right thing when user clicks any button */
            switch(LOWORD(wParam)) {
                case IDC_START:
                    Start();
                    break;
                case IDC_STOP:
                    Stop();
            }
            break;
            
        case WM_CREATE:
            /* Create some buttons, so user can do something useful. */
            CreateWindow(_T("BUTTON"), _T("Start"), 
                WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON,
                10, 10, 140, 30,
                hWnd, (HMENU)(UINT_PTR)IDC_START, hInstance, NULL);
            CreateWindow(_T("BUTTON"), _T("Stop"), 
                WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON,
                10, 50, 140, 30,
                hWnd, (HMENU)(UINT_PTR)IDC_STOP, hInstance, NULL);
            hMainWnd = hWnd;
            EnableWindow(GetDlgItem(hMainWnd, IDC_START), 1);
            EnableWindow(GetDlgItem(hMainWnd, IDC_STOP), 0);
            return 0;
            
        case WM_CLOSE:
            Stop();           
            PostQuitMessage(0);
            break;
    }
    
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int iCmdShow)
{
    ABS_STATUS status;
    WNDCLASS wc = { 0 };
    MSG msg;
    ABS_DATA securityData;
    
    UNREFERENCED_PARAMETER(hPrevInst);
    UNREFERENCED_PARAMETER(lpCmdLine);
    
    hInstance = hInst;
    
    /* Initialize BSAPI */
    status = ABSInitialize();
    if(status != ABS_STATUS_OK) {
        Error("ABSInitialize() failed", status);
        return 1;
    }
    
    /* Disable communication enryption. Ifd evice supports authentification without securechannel in its firmware configuration,
     * this roughly doubles image frame rate.
     */
    securityData.Length = 1;
    securityData.Data[0] = ABS_SENSOR_SECURITY_LOW;
    status = ABSSetGlobalParameter(ABS_PARAM_SENSOR_SECURITY, &securityData);
    if(status != ABS_STATUS_OK) {
        Error("ABSSetGlobalParameter() failed", status);
        return 1;
    }
    
    /* Open BSAPI session */
    status = ABSOpen("usb", &conn);
    if(status != ABS_STATUS_OK) {
        Error("ABSOpen() failed", status);
        return 1;
    }
    
    
    /* Create main window */
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hbrBackground = (HBRUSH)(UINT_PTR)(COLOR_WINDOW+1);
    wc.lpfnWndProc = MainWinProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = _T("MyWin");
    RegisterClass(&wc);
    hMainWnd = CreateWindow(
        _T("MyWin"), 
        _T("BSAPI Live Imagery Sample"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, 
        NULL, 
        NULL,
        hInstance, 
        NULL
    );
    ShowWindow(hMainWnd, iCmdShow);
    
    /* Main message loop */
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }    
    
    /* Free any memory we still may have acquired */
    if(pImage != NULL)
        ABSFree(pImage);
    
    /* Close BSAPI session */
    ABSClose(conn);
    
    /* Free resources acquired in ABSInitialize() */
    ABSTerminate();
    return 0;
}

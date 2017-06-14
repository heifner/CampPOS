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
#define IDC_GRAB             500
#define IDC_RAW_GRAB         501
#define IDC_GRAB_IMAGE       502
#define IDC_RAW_GRAB_IMAGE   503
#define IDC_BINARIZE         504
#define IDC_RESET            505


/* BSAPI connection handle */
static ABS_CONNECTION conn;

/* An image we work with. In this example it is output of ABSGrab(), 
 * ABSRawGrab() and it can be converted to black and white by 
 * ABSBinarizeSampleImage(). */
static ABS_SAMPLE_IMAGE* pImage = NULL;

/* ABS_OPERATION structure for all interactive operations
 * i.e. for ABSGrab() and ABSRawGrab() in this sample. */
static ABS_OPERATION op = { 0, NULL, ABSDefaultCallback, 0, 0 };



/* During initialization, we choose a format we will then use for ABSGrab()
 * and ABSRawGrab() functions. */
static ABS_IMAGE_FORMAT imageFormat;


/* Helper function displaying a dialog with error message if some ABSAPI
 * function fails. */
static void
Error(const char* str, ABS_STATUS status)
{
    ABS_DWORD code;
    const char* msg;
    char buffer[1024];

    ABSGetLastErrorInfo(&code, &msg);    
    _snprintf(buffer, 1024, "%s\n\nStatus: %ld\nCode: %ld\nMessage: %s",
               str, (long)status, (long)code, msg);               
    MessageBoxA(hMainWnd, buffer, "BSAPI Error", MB_OK | MB_ICONERROR);
}

/* Helper function enabling/disabling button controls. */
static void
SetupButtons(void)
{
    /* The following buttons are enabled only when we already have 
     * some image */
    EnableWindow(GetDlgItem(hMainWnd, IDC_BINARIZE), (pImage != NULL));
    EnableWindow(GetDlgItem(hMainWnd, IDC_RESET), (pImage != NULL));
}

/* This function performs ABSGrab() when user clicks on the corresponding 
 * button */
static void
Grab(void)
{
    ABS_STATUS status;
    
    if(pImage != NULL) {
        ABSFree(pImage);
        pImage = NULL;
    }
    
    status = ABSGrab(conn, &op, ABS_PURPOSE_UNDEFINED, &pImage, 0);    
    if(status != ABS_STATUS_OK)
        Error("ABSGrab() failed.", status);
    InvalidateRect(hMainWnd, NULL, TRUE);
    SetupButtons();
}

/* This function performs ABSRawGrab() when user clicks on the corresponding 
 * button */
static void
RawGrab(void)
{
    ABS_STATUS status;
    ABS_PROFILE_DATA profile[] = {
        {ABS_PKEY_IMAGE_FORMAT, ABS_PVAL_IFMT_381_381_8}
    };

    if(pImage != NULL) {
        ABSFree(pImage);
        pImage = NULL;
    }

    status = ABSRawGrab(conn, &op, sizeof(profile) / sizeof(profile[0]), 
                        profile, &pImage, NULL, 0);
    if(status != ABS_STATUS_OK)
        Error("ABSRawGrab() failed.", status);

    InvalidateRect(hMainWnd, NULL, TRUE);
    SetupButtons();
}

/* This function performs ABSGrabImage() when user clicks on the corresponding 
 * button */
static void
GrabImage(void)
{
    ABS_STATUS status;
    
    if(pImage != NULL) {
        ABSFree(pImage);
        pImage = NULL;
    }
    
    status = ABSGrabImage(conn, &op, ABS_PURPOSE_UNDEFINED, &imageFormat,
                          &pImage, NULL, NULL, 0);
    if(status != ABS_STATUS_OK)
        Error("ABSGrabImag() failed.", status);
    InvalidateRect(hMainWnd, NULL, TRUE);
    SetupButtons();
}

/* This function performs ABSRawGrabImage() when user clicks on the 
 * corresponding button */
static void
RawGrabImage(void)
{
    ABS_STATUS status;
    
    /* To show how to use the profile, we will set swipe and no-movement 
     * timeouts to some values. */
    ABS_PROFILE_DATA profile[] = {
        {ABS_PKEY_REC_SWIPE_TIMEOUT, 2000 },
        {ABS_PKEY_REC_NO_MOVEMENT_TIMEOUT, 150 }
    };

    if(pImage != NULL) {
        ABSFree(pImage);
        pImage = NULL;
    }

    status = ABSRawGrabImage(conn, &op, sizeof(profile) / sizeof(profile[0]), 
                        profile, &imageFormat, &pImage, NULL, NULL, 0);
    if(status != ABS_STATUS_OK)
        Error("ABSRawGrab() failed.", status);

    InvalidateRect(hMainWnd, NULL, TRUE);
    SetupButtons();
}

/* This function detects whether the device supports ABSGrabImage() and 
 * ABSRawGrabImage() functions. If yes, it also setups the global variable
 * imageFormat, used as parameter for those two functions. */
static void
SetupImageFormat(void)
{
    ABS_STATUS status;
    ABS_DWORD dwCount;
    ABS_IMAGE_FORMAT* pImageFormatList;
    
    status = ABSListImageFormats(conn, &dwCount, &pImageFormatList, 0);
    if(status != ABS_STATUS_OK)
        Error("ABSListImageFormats() failed.", status);    
    
    /* We will simply use the first image format in the list. Real 
     * application could allow user to choose which one to use. */
    memcpy(&imageFormat, pImageFormatList, sizeof(ABS_IMAGE_FORMAT));
    
    ABSFree(pImageFormatList);
}

/* This function performs ABSBinarizeSampleImage() when user clicks on 
 * the corresponding button */
static void
Binarize(void)
{
    ABS_STATUS status;
    
    status = ABSBinarizeSampleImage(pImage, NULL);
    if(status != ABS_STATUS_OK)
        Error("ABSBinarizeSampleImage() failed.", status);

    InvalidateRect(hMainWnd, NULL, TRUE);
    SetupButtons();
}

/**
 * Resets the remembered image.
 */
static void
Reset(void)
{
    if(pImage == NULL)
        return;
    
    ABSFree(pImage);
    pImage = NULL;
    InvalidateRect(hMainWnd, NULL, TRUE);
    SetupButtons();
}

/* Paints the fingerprint sample image. */
static void
Paint(void)
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

static LRESULT CALLBACK
MainWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg) {
        case WM_PAINT:
            Paint();
            break;
        
        case WM_COMMAND:
            /* Do the right thing when user clicks any button */
            switch(LOWORD(wParam)) {
                case IDC_GRAB:
                    Grab();
                    break;
                case IDC_RAW_GRAB:
                    RawGrab();
                    break;
                case IDC_GRAB_IMAGE:
                    GrabImage();
                    break;
                case IDC_RAW_GRAB_IMAGE:
                    RawGrabImage();
                    break;
                case IDC_BINARIZE:
                    Binarize();
                    break;
                case IDC_RESET:
                    Reset();
                    break;
            }
            break;
            
        case WM_CREATE:
            /* Create some buttons, so user can do something useful. */
            CreateWindow(_T("BUTTON"), _T("ABSGrab"), 
                WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON,
                10, 10, 140, 30,
                hWnd, (HMENU)(UINT_PTR)IDC_GRAB, hInstance, NULL);
            CreateWindow(_T("BUTTON"), _T("ABSRawGrab"), 
                WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON,
                10, 50, 140, 30,
                hWnd, (HMENU)(UINT_PTR)IDC_RAW_GRAB, hInstance, NULL);
            CreateWindow(_T("BUTTON"), _T("ABSGrabImage"), 
                WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON,
                10, 90, 140, 30,
                hWnd, (HMENU)(UINT_PTR)IDC_GRAB_IMAGE, hInstance, NULL);
            CreateWindow(_T("BUTTON"), _T("ABSRawGrabImage"), 
                WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON,
                10, 130, 140, 30,
                hWnd, (HMENU)(UINT_PTR)IDC_RAW_GRAB_IMAGE, hInstance, NULL);
            CreateWindow(_T("BUTTON"), _T("ABSBinarize"), 
                WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON,
                10, 170, 140, 30,
                hWnd, (HMENU)(UINT_PTR)IDC_BINARIZE, hInstance, NULL);
            CreateWindow(_T("BUTTON"), _T("Reset image"), 
                WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON,
                10, 210, 140, 30,
                hWnd, (HMENU)(UINT_PTR)IDC_RESET, hInstance, NULL);
            hMainWnd = hWnd;
            SetupButtons();
            return 0;
            
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
    }
    
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


int APIENTRY
WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int iCmdShow)
{
    ABS_STATUS status;
    WNDCLASS wc = { 0 };
    MSG msg;
    
    UNREFERENCED_PARAMETER(hPrevInst);
    UNREFERENCED_PARAMETER(lpCmdLine);
    
    hInstance = hInst;
    
    /* Initialize BSAPI */
    status = ABSInitialize();
    if(status != ABS_STATUS_OK) {
        Error("ABSInitialize() failed", status);
        return 1;
    }

    /* Open BSAPI session */
    status = ABSOpen("usb", &conn);
    if(status != ABS_STATUS_OK) {
        Error("ABSOpen() failed", status);
        return 1;
    }
    
    /* Detect if ABSGrabImage() and ABSRawGrabImage() are supported by the 
     * device, and (if yes) select what image format to use. */
    SetupImageFormat();
    
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
        _T("BSAPI Grab Sample"),
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

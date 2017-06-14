/* 2008-2009 (C) Upek, inc.
 * BSAPI sample Navigation
 * 
 * This sample presents usage of ABSNavigate function.
 */


#include <windows.h>
#include <winnt.h>
#include <tchar.h>
#include <process.h>  /* _beginthreadex() */
#include <stdarg.h>

#include <bsapi.h>

#include "resource.h"


/* Used to supress compiler warnings */
#ifndef UNREFERENCED_PARAMETER
    #define UNREFERENCED_PARAMETER(param)   (void)param
#endif


static HINSTANCE hInst;    /* app instance handle */
static HWND hMainWin;      /* main window handle */
static HWND hMapWin;       /* child window managing the bitmap (navigation map) */
static HMENU hMenu;        /* main window menu */
static HBITMAP hBitmap;    /* bitmap handle */

static DWORD dwMainThread;            /* main thread ID */
static DWORD dwNavigationThread;      /* navigation thread ID */
static HANDLE hNavigationThread;      /* navigation thread handle */

/* BSAPI connection handle */
static ABS_CONNECTION conn;    

/* Magic number we use to identify the navigation operation.
 * Any non-zero constant can be used, because in this sample only
 * one operation can be run at any time -- the ABSNavigate(). 
 * I.e. the constant cannot collide with any concurrently running operation.
 */
#define OPERATION_ID          (0xabcd)  

/* Message ID to be sent to navigation thread, to start the navigation */
#define MSG_START_NAVIGATION  (WM_USER + 100)

/* Message ID to be sent from navigation thread to main thread, with an
 * information about a navigation change */ 
#define MSG_NAVIGATION_CHANGE (WM_USER + 101)

/* Message ID to be sent from navigation thread to main thread, with an
 * information about a "click" event */
#define MSG_NAVIGATION_CLICK  (WM_USER + 102)


/* Trivial error handling */
static void
Error(const TCHAR* fmt, ...)
{
    TCHAR buffer[1024];
    va_list args;

    va_start(args, fmt);
    _vsntprintf(buffer, 1024, fmt, args);
    va_end(args);

    MessageBox(hMainWin, buffer, _T("Error"), MB_OK | MB_ICONERROR);
}


/* Callback, called from ABSNavigate(). */
static void __stdcall
NavigationCallback(const ABS_OPERATION* pOperation, 
                   ABS_DWORD dwMsg, void* pMsgData)
{
    UNREFERENCED_PARAMETER(pOperation);
    
    /* We are interested only in MSG_NAVIGATION_CHANGE and MSG_NAVIGATION_CLICK
     * messages. However more clever applications could track more messages and
     * react on them accordingly. 
     *
     * For both events, we set appropriate data to the main thread which
     * updates GUI accordingly.
     */ 
    switch(dwMsg) {
        case ABS_MSG_NAVIGATE_CHANGE:
        {
            ABS_NAVIGATION_DATA* data = (ABS_NAVIGATION_DATA*) pMsgData;
            PostMessage(hMainWin, MSG_NAVIGATION_CHANGE, 
                        (WPARAM) data->DeltaX, (LPARAM) data->DeltaY);
            break;
        }
            
        case ABS_MSG_NAVIGATE_CLICK:
        {
            PostMessage(hMainWin, MSG_NAVIGATION_CLICK, 0, 0);
            break;
        }
    }
}

/* Main function of navigation thread.
 * It just waits for message MSG_START_NAVIGATION to call ABSNavigate().
 * ABSNavigate() then blocks until it's canceled with ABSCancelOperation() 
 * called from the main thread, or until an error occures.
 */
static unsigned __stdcall
NavigationThreadProc(void* param)
{
    ABS_STATUS status;
    ABS_OPERATION operation;
    MSG msg;
    
    operation.OperationID = OPERATION_ID;
    operation.Context = param;
    operation.Callback = NavigationCallback;  /* Pointer to callback function */
    operation.Timeout = 0;             /* Timeout is ignored by ABSNavigate() */
    operation.Flags = 0;
    
    while(GetMessage(&msg, NULL, 0, 0)) {
        if(msg.message == MSG_START_NAVIGATION) {
            /* Main thread is asking us to start navigation. So we actualize
             * state of menu items (disable/enable start/stop menu items. */
            EnableMenuItem(hMenu, IDM_START, MF_BYCOMMAND | MF_GRAYED);
            EnableMenuItem(hMenu, IDM_STOP, MF_BYCOMMAND | MF_ENABLED);
            status = ABSNavigate(conn, &operation, 0);
            
            /* After the navigation is over, it is either an error; or it has 
             * been canceled from the main thread. ABSNavigate never returns
             * ABS_STATUS_OK. */
            if(status != ABS_STATUS_CANCELED) {
                /* Some devices do not support navigation so be more user 
                 * friendly and check this error specially */
                if(status == ABS_STATUS_NOT_SUPPORTED) {
                    Error(_T("[%ld] ABSNavigate() failed.\n\n")
                          _T("Your fingerpruint device does not support ")
                          _T("navigation mode."), (long)status);
                } else {
                    Error(_T("[%ld] ABSNavigate() failed."), (long)status);
                }
            }
            
            /* The navigation has finished: reset the menu items states, 
             * so the user can restart the navigation again. */
            EnableMenuItem(hMenu, IDM_START, MF_BYCOMMAND | MF_ENABLED);
            EnableMenuItem(hMenu, IDM_STOP, MF_BYCOMMAND | MF_GRAYED);
        }
    }
    
    return 0;
}


/* Main window procedure. It moves a bitmap as user moves his finger on the
 * sensor of fingerprint device. When "click" is detected, we simply popup
 * a message box. */
static LRESULT CALLBACK
MyWinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg) {
        case MSG_NAVIGATION_CHANGE:  /* sent from the callback in the navigation thread */
        {
            /* Move the static control with the bitmap accordingly to detected
             * move on the sensor (but not outside of the main window) */
            RECT rectMain;
            RECT rectMap;
            GetClientRect(hMainWin, &rectMain);
            GetWindowRect(hMapWin, &rectMap);
            MapWindowPoints(GetDesktopWindow(), hMainWin, (LPPOINT) &rectMap, 2);
            /* Note that we subract in horizontal direction and add in 
             * vertical direction. This is due historical reasons, to be 
             * consistent with older Upek SDKs.
             */
            rectMap.left -= (ABS_LONG) wParam;
            rectMap.top += (ABS_LONG) lParam;
            if(rectMap.left > 0)
                rectMap.left = 0;
            if(rectMap.top > 0)
                rectMap.top = 0;
            if(rectMap.left < rectMain.right - NAVIGATION_W)
                rectMap.left = rectMain.right - NAVIGATION_W;
            if(rectMap.top < rectMain.bottom - NAVIGATION_H)
                rectMap.top = rectMain.bottom - NAVIGATION_H;
            SetWindowPos(hMapWin, NULL, rectMap.left, rectMap.top, 0, 0, SWP_NOSIZE);
            break;
        }
    
        case MSG_NAVIGATION_CLICK:  /* sent from the callback in the navigation thread */
            /* Just show a message box */
            MessageBox(hwnd, _T("Click on the fingerprint sensor has been detected."),
                       _T("Click!"), MB_OK);
            break;
        
        case WM_COMMAND:
            /* Handle menu items */
            switch(LOWORD(wParam)) {
                case IDM_START:  
                    /* Tell the navigation thread to start navigation */
                    PostThreadMessage(dwNavigationThread, MSG_START_NAVIGATION, 0, 0); 
                    break;
                    
                case IDM_STOP:       
                {
                    /* Cancel the navigation operation. We might cancel any 
                     * interactive biometric operation in the same way, as long
                     * as that operation has (unique) non-zero value in its 
                     * ABS_OPERATION structure. 
                     *
                     * Alternativelly, the operation might be canceled from
                     * its own callback. In that case the zero could be passed
                     * as teh second parameter of ABSCancelOperation.
                     */
                    ABS_STATUS status;
                    status = ABSCancelOperation(conn, OPERATION_ID);
                    if(status != ABS_STATUS_OK)
                        Error(_T("[%ld] ABSCancelOperation() failed."), (long)status);
                    break;
                }
                    
                case IDM_QUIT:   
                    /* Quit the application */
                    PostQuitMessage(0); 
                    break;
            }
            break;
            
        case WM_CREATE:
        {
            RECT rect;
            UINT w, h;
            
            /* Get window dimenstions, so we can place the bitmap centered
             * in the window. */
            GetClientRect(hwnd, &rect);
            w = rect.right - rect.left;
            h = rect.bottom - rect.top;
            
            /* Create child static window holding the bitmap. */
            hMapWin = CreateWindow(
                    _T("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, 
                    w / 2 - NAVIGATION_W / 2, h / 2 - NAVIGATION_H / 2, 
                    NAVIGATION_W, NAVIGATION_H, hwnd, (HMENU)(UINT_PTR)500, 
                    hInst, 0);
            SendMessage(hMapWin, STM_SETIMAGE, 
                    (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);
            break;
        }
        
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}


/* Program entry point */
int APIENTRY
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
        LPSTR lpCmdLine, int iCmdShow)
{
    ABS_STATUS status;
    WNDCLASS wc = { 0 };
    MSG msg;
    
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    
    hInst = hInstance;
    dwMainThread = GetCurrentThreadId();
    
    /* Create main window */
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MyWinProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = _T("MyWin");
    RegisterClass(&wc);    
    hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(ID_MENU));
    hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(ID_NAVIGATION_MAP));
    hMainWin = CreateWindow(_T("MyWin"), _T("BSAPI Navigation Sample"), 
                    WS_OVERLAPPED | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 
                    500, 300, NULL, hMenu, hInstance, 0);
    ShowWindow(hMainWin, iCmdShow);

    /* Initialize BSAPI and open session with fingerprint reader device. */
    ABSInitialize();
    status = ABSOpen("usb", &conn);
    if(status != ABS_STATUS_OK) {
        Error(_T("[%ld] ABSOpen() failed. The app will be terminated."), (long)status);
        exit(-1);
    }
    
    /* Start navigation thread. */
    hNavigationThread = (HANDLE) _beginthreadex(NULL, 0, NavigationThreadProc,
             NULL, 0, (unsigned*) &dwNavigationThread);
    
    /* Give the new thread some time to create its message loop.
     * (to be 100% correct, we should instead wait for some event signaled
     * by the new thread, but this sample is to demonstrate BSAPI, not to 
     * learn Win32API, so lets go this way for sake of simplicity. If this 
     * hack fails, no catastroph happens. Only the following message drops in
     * a black hole so the navigation will not start automatically after the
     * application starts, but user will have to start it manually in 
     * application's menu.) */
    Sleep(100);
    
    /* Start the navigation operation in the new helper thread. */
    PostThreadMessage(dwNavigationThread, MSG_START_NAVIGATION, 0, 0);
    
    /* Main event loop, responsible to manage GUI. */
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    /* Ensure that the navigation thread is not blocked inside the function
     * ABSNavigate() (if it is not, this calling just fails with error
     * ABS_STATUS_NO_SUCH_OPERATION, which does not harm anything here). */
    ABSCancelOperation(conn, OPERATION_ID);
    
    /* Terminate the navigation thread. */
    PostThreadMessage(dwNavigationThread, WM_QUIT, 0, 0);
    WaitForSingleObject(hNavigationThread, INFINITE);
    
    /* Cleanup. */
    ABSClose(conn);
    ABSTerminate();
    return 0;
}


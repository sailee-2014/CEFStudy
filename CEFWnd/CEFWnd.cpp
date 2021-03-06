// CEFWnd.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CEFWnd.h"
#include "ClientHandler.h"
#include "internal/cef_ptr.h"
#include "cef_client.h"
#include "cef_app.h"


#define MAX_LOADSTRING 100
#define MAX_URL_LENGTH  255
#define BUTTON_WIDTH 72
#define URLBAR_HEIGHT  24


// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

//ADD TODO:
CefRefPtr<ClientHandler> g_handler;

char szWorkingDir[MAX_PATH];  // The current working directory


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	CefMainArgs main_args(hInstance);
	//CefRefPtr<ClientApp> app(new ClientApp);

	// Execute the secondary process, if any.
	//int exit_code = CefExecuteProcess(main_args, app.get());
	//if (exit_code >= 0)
	//	return exit_code;

	// Retrieve the current working directory.
	//if (_getcwd(szWorkingDir, MAX_PATH) == NULL)
	//	szWorkingDir[0] = 0;

	//// Parse command line arguments. The passed in values are ignored on Windows.
	//AppInitCommandLine(0, NULL);

	CefSettings settings;

	//// Populate the settings based on command line arguments.
	//AppGetSettings(settings);

	// Initialize CEF.
	CefInitialize(main_args, settings, app.get());

	// Register the scheme handler.
	//scheme_test::InitTest();


	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CEFWND, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CEFWND));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	// Shut down CEF.
	CefShutdown();


	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CEFWND));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_CEFWND);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
// 
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND backWnd = NULL, forwardWnd = NULL, reloadWnd = NULL,
		stopWnd = NULL, editWnd = NULL;
	static WNDPROC editWndOldProc = NULL;

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		{
			// Create the child windows used for navigation
			RECT rect;
			int x = 0;

			GetClientRect(hWnd, &rect);

			backWnd = CreateWindow(_T("Button"), _T("Back"), 
									WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
									| WS_DISABLED, x, 2, BUTTON_WIDTH, URLBAR_HEIGHT,
									hWnd, (HMENU) IDC_NAV_BACK, hInst, NULL);
			x += BUTTON_WIDTH;

			forwardWnd = CreateWindow(_T("Button"), _T("Forward"),
										WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
										|WS_DISABLED, x, 2, BUTTON_WIDTH, URLBAR_HEIGHT,
										hWnd, (HMENU) IDC_NAV_FORWARD, hInst, NULL); 
			x += BUTTON_WIDTH;
			reloadWnd = CreateWindow(_T("Button"), _T("Reload"), 
										WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
										|WS_DISABLED, x, 2, BUTTON_WIDTH, URLBAR_HEIGHT,
										hWnd, (HMENU) IDC_NAV_RELOAD, hInst, NULL);

			x += BUTTON_WIDTH;
			stopWnd = CreateWindow(_T("Button"), _T("Stop"), 
									WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
									x, 2, BUTTON_WIDTH, URLBAR_HEIGHT,
									hWnd, (HMENU)IDC_NAV_STOP, hInst, NULL);

			x += BUTTON_WIDTH;

			editWnd = CreateWindow(_T("EDIT"), _T("URL"),
									WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT
									| ES_AUTOVSCROLL | ES_AUTOHSCROLL ,
									x, 2, rect.right - BUTTON_WIDTH * 4,
									URLBAR_HEIGHT, hWnd, 0, hInst, NULL);
			
			editWndOldProc = 
				reinterpret_cast<WNDPROC> (GetWindowLongPtr(editWnd, GWLP_WNDPROC));
			SetWindowLongPtr(editWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc));
			
			rect.top += URLBAR_HEIGHT;
			CefBrowserSettings setting;
			CefWindowInfo info;

			g_handler = new ClientHandler();

			CefBrowserHost::CreateBrowser(info, g_handler.get(), g_handler->GetStartupURL(), setting, NULL);




			
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

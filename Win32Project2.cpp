// Win32Project2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Win32Project2.h"
#include "MonitorGraphUnit.h"
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECT2, szWindowClass, MAX_LOADSTRING);
    
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(10, 10, 10));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassExW(&wcex);

	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, L"그래프 테스트", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) return 0;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT2));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

CMonitorGraphUnit *p1;
CMonitorGraphUnit *p2;
CMonitorGraphUnit *p3;
CMonitorGraphUnit *p4;
CMonitorGraphUnit *p5;
CMonitorGraphUnit *p6;
CMonitorGraphUnit *p7;
HINSTANCE g_hInst;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		p1 = new CMonitorGraphUnit(g_hInst, hWnd, L"단일그래프", RGB(100, 50, 100), CMonitorGraphUnit::LINE_SINGLE, 10, 10, 200, 200);
		p2 = new CMonitorGraphUnit(g_hInst, hWnd, L"막대그래프", RGB(100, 50, 50), CMonitorGraphUnit::BAR_SINGLE_HORZ, 220, 10, 200, 200);
		p3 = new CMonitorGraphUnit(g_hInst, hWnd, L"파랑그래프", RGB(50, 50, 100), CMonitorGraphUnit::LINE_SINGLE, 430, 10, 400, 200);
		p4 = new CMonitorGraphUnit(g_hInst, hWnd, L"큰  그래프", RGB(50, 50, 100), CMonitorGraphUnit::LINE_SINGLE, 10, 220, 300, 250);
		p5 = new CMonitorGraphUnit(g_hInst, hWnd, L"다중그래프", RGB(25, 25, 25), CMonitorGraphUnit::LINE_MULTI, 320, 220, 510, 250, 4);
		p6 = new CMonitorGraphUnit(g_hInst, hWnd, L"다중막대그래프", RGB(25, 25, 25), CMonitorGraphUnit::BAR_COLUMN_HORZ, 10, 480, 820, 250, 10);
		p7 = new CMonitorGraphUnit(g_hInst, hWnd, L"원  그래프", RGB(25, 25, 25), CMonitorGraphUnit::PIE, 840, 10, 500, 430, 5);

		p5->SetDataColumnInfo(1, 1, L"테스트1");
		p5->SetDataColumnInfo(1, 2, L"테스트2");
		p5->SetDataColumnInfo(1, 3, L"테스트3");
		p5->SetDataColumnInfo(1, 4, L"테스트4");
	
		p6->SetDataColumnInfo(1, 1, L"테스트1");
		p6->SetDataColumnInfo(1, 2, L"테스트2");
		p6->SetDataColumnInfo(1, 3, L"테스트3");
		p6->SetDataColumnInfo(1, 4, L"테스트4");
		p6->SetDataColumnInfo(1, 5, L"테스트5");
		p6->SetDataColumnInfo(1, 6, L"테스트6");
		p6->SetDataColumnInfo(1, 7, L"테스트7");
		p6->SetDataColumnInfo(1, 8, L"테스트8");
		p6->SetDataColumnInfo(1, 9, L"테스트9");
		p6->SetDataColumnInfo(1, 10, L"테스트10");

		p7->SetDataColumnInfo(1, 1, L"테스트1");
		p7->SetDataColumnInfo(1, 2, L"테스트2");
		p7->SetDataColumnInfo(1, 3, L"테스트3");
		p7->SetDataColumnInfo(1, 4, L"테스트4");
		p7->SetDataColumnInfo(1, 5, L"테스트5");
		
		SetTimer(hWnd, 1, 500, NULL);
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
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
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;

	case WM_TIMER:
		p1->InsertData(rand() % 100);
		p2->InsertData(rand() % 100);
		p3->InsertData(rand() % 100);
		p4->InsertData(rand() % 100);

		p5->InsertData(rand() % 100, 1, 1);
		p5->InsertData(rand() % 100, 1, 2);
		p5->InsertData(rand() % 100, 1, 3);
		p5->InsertData(rand() % 100, 1, 4);

		p6->InsertData(rand() % 100, 1, 1);
		p6->InsertData(rand() % 100, 1, 2);
		p6->InsertData(rand() % 100, 1, 3);
		p6->InsertData(rand() % 100, 1, 4);
		p6->InsertData(rand() % 100, 1, 5);
		p6->InsertData(rand() % 100, 1, 6);
		p6->InsertData(rand() % 100, 1, 7);
		p6->InsertData(rand() % 100, 1, 8);
		p6->InsertData(rand() % 100, 1, 9);
		p6->InsertData(rand() % 100, 1, 10);

		p7->InsertData(rand() % 100, 1, 1);
		p7->InsertData(rand() % 100, 1, 2);
		p7->InsertData(rand() % 100, 1, 3);
		p7->InsertData(rand() % 100, 1, 4);
		p7->InsertData(rand() % 100, 1, 5);

		InvalidateRect(p1->GetHWND(), NULL, true);
		InvalidateRect(p2->GetHWND(), NULL, true);
		InvalidateRect(p3->GetHWND(), NULL, true);
		InvalidateRect(p4->GetHWND(), NULL, true);
		InvalidateRect(p5->GetHWND(), NULL, true);
		InvalidateRect(p6->GetHWND(), NULL, true);
		InvalidateRect(p7->GetHWND(), NULL, true);
		break;

    case WM_DESTROY:
		delete p1;
		delete p2;
		delete p3;
		delete p4;
		delete p5;
		KillTimer(hWnd,1);
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

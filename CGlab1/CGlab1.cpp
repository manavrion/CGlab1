#include "stdafx.h"
#include <windows.h>
#include <CommCtrl.h>
#include <string>

#include "resource.h"

#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

using namespace Gdiplus;
//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------

HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;

HWND                    g_hScrollX = NULL;
HWND                    g_hScrollY = NULL;
HWND                    g_hScrollZ = NULL;

const int g_sqsize = 350;
const int g_border = 15;
const int g_scrollheight = 30;

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Init GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    if (FAILED(InitWindow(hInstance, nCmdShow)))
        return 0;

    // Main message loop
    MSG msg = { 0 };
    while (WM_QUIT != msg.message) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int)msg.wParam;
}



//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow) {
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CGLAB1));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_CGLAB1);
    if (!RegisterClassEx(&wcex))
        return E_FAIL;
    
    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, g_sqsize * 2 + g_border * 3, 580 };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    g_hWnd = CreateWindow(L"TutorialWindowClass", L"GDI+ PRO-210 Lab 1 (III octant)", (WS_OVERLAPPED |
        WS_CAPTION |
        WS_SYSMENU),
        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
        NULL);

    // Create window elements
    g_hScrollX = CreateWindowExW(0L, TRACKBAR_CLASS, L"",
        WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE,
        g_border,
        g_sqsize + g_border * 2,
        g_sqsize,
        g_scrollheight,
        g_hWnd, 0, hInstance, 0);

    g_hScrollY = CreateWindowExW(0L, TRACKBAR_CLASS, L"",
        WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE,
        g_border,
        g_sqsize + g_border * 2 + g_border + g_scrollheight,
        g_sqsize,
        g_scrollheight,
        g_hWnd, 0, hInstance, 0);

    g_hScrollZ = CreateWindowExW(0L, TRACKBAR_CLASS, L"",
        WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE,
        g_border,
        g_sqsize + g_border * 2 + g_border * 2 + g_scrollheight * 2,
        g_sqsize,
        g_scrollheight,
        g_hWnd, 0, hInstance, 0);

    int minPos = -100;
    int maxPos = 100;
    SendMessage(g_hScrollX, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, maxPos));  // min. & max. positions
    SendMessage(g_hScrollY, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, maxPos));  // min. & max. positions
    SendMessage(g_hScrollZ, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, maxPos));  // min. & max. positions
        
    SendMessage(g_hScrollX, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);
    SendMessage(g_hScrollY, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);
    SendMessage(g_hScrollZ, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);

    if (!g_hWnd)
        return E_FAIL;

    ShowWindow(g_hWnd, nCmdShow);

    return S_OK;
}


//--------------------------------------------------------------------------------------
// GDI+ provider
//--------------------------------------------------------------------------------------

Size g_size(g_sqsize, g_sqsize); //backBuffer sizes

// -- First screen plate --

byte *g_backBuffer_one; //backBuffer bits
HBITMAP g_backBufferHBitmap_one;//backBuffer handle
HDC g_backBufferHDC_one;//backBuffer hdc
Gdiplus::Graphics *g_graphics_one;//graphics

// -- Second screen plate --

byte *g_backBuffer_two;
HBITMAP g_backBufferHBitmap_two; 
HDC g_backBufferHDC_two;         
Gdiplus::Graphics *g_graphics_two; 


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam) {

    switch (message) {        
        case WM_CREATE:
        {
            // -- First screen plate --

            //init backBuffer
            g_backBuffer_one = new byte[g_size.Width * g_size.Height * 4];
            g_backBufferHBitmap_one = CreateBitmap(g_size.Width, g_size.Height, 1, 32, g_backBuffer_one);

            //init HDC
            g_backBufferHDC_one = CreateCompatibleDC(nullptr);
            SelectObject(g_backBufferHDC_one, g_backBufferHBitmap_one);

            //init graphics
            g_graphics_one = new Gdiplus::Graphics(g_backBufferHDC_one);                        
            g_graphics_one->SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);


            // -- Second screen plate --

            //init backBuffer
            g_backBuffer_two = new byte[g_size.Width * g_size.Height * 4];
            g_backBufferHBitmap_two = CreateBitmap(g_size.Width, g_size.Height, 1, 32, g_backBuffer_two);

            //init HDC
            g_backBufferHDC_two = CreateCompatibleDC(nullptr);
            SelectObject(g_backBufferHDC_two, g_backBufferHBitmap_two);

            //init graphics
            g_graphics_two = new Gdiplus::Graphics(g_backBufferHDC_two);
            g_graphics_two->SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);

            return DefWindowProc(hWindow, message, wParam, lParam);
        }
        case WM_HSCROLL:
        {
            //processing scroll events
            HWND hWindowTarget = (HWND)lParam;
            if (hWindowTarget == g_hScrollX || 
                hWindowTarget == g_hScrollY || 
                hWindowTarget == g_hScrollZ) 
            {
                //call repaint event
                InvalidateRect(hWindow, NULL, false);
            }
            return DefWindowProc(hWindow, message, wParam, lParam);
        }
        case WM_PAINT:
        {
            // init paint event
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWindow, &ps);

            // setting standart font
            NONCLIENTMETRICS ncm;
            ncm.cbSize = sizeof(NONCLIENTMETRICS);
            UINT uiParam = sizeof(NONCLIENTMETRICS);
            SystemParametersInfo(SPI_GETNONCLIENTMETRICS, uiParam, &ncm, 0);
            LOGFONT _lf;
            HFONT _old, _new;
            _lf = ncm.lfMenuFont;
            _new = CreateFontIndirect(&_lf);
            _old = (HFONT)SelectObject(hdc, _new);


            // getting pos from scrollers
            float dx = SendMessage(g_hScrollX, TBM_GETPOS, 0, 0) / float(100);
            float dy = -SendMessage(g_hScrollY, TBM_GETPOS, 0, 0) / float(100);
            float dz = -SendMessage(g_hScrollZ, TBM_GETPOS, 0, 0) / float(100);

            // presets
            Color xyzColor   (140, 140, 140);
            Color pointsColor(255, 255, 255);
            Color linesColor (0, 255 / 4 * 3, 255);
            int center = g_sqsize / 2;

            //--------------------------------------------------------------------------------------
            // Rendering first screen
            //--------------------------------------------------------------------------------------

            // background
            g_graphics_one->FillRectangle(&SolidBrush(Color(int(0.0f), int(255 * 0.125f), int(255 * 0.3f))), Rect(0, 0, g_size.Width, g_size.Height));

            // xyz
            g_graphics_one->DrawLine(&Pen(xyzColor, 2), Point(center/4, center), Point(g_sqsize - center/4, center)); //y
            g_graphics_one->DrawLine(&Pen(xyzColor, 2), Point(center, center/4), Point(center, g_sqsize - center/4)); //z
            g_graphics_one->DrawLine(&Pen(xyzColor, 2), Point(center/2, center/2), Point(g_sqsize - center/2, g_sqsize - center/2)); //x

            //arrows
            g_graphics_one->DrawLine(&Pen(xyzColor, 2), Point(center / 4, center), Point(center / 4 + 16, center + 8)); //y
            g_graphics_one->DrawLine(&Pen(xyzColor, 2), Point(center / 4, center), Point(center / 4 + 16, center - 8));

            g_graphics_one->DrawLine(&Pen(xyzColor, 2), Point(center, center / 4), Point(center + 8, center / 4 + 16)); //z
            g_graphics_one->DrawLine(&Pen(xyzColor, 2), Point(center, center / 4), Point(center - 8, center / 4 + 16));

            g_graphics_one->DrawLine(&Pen(xyzColor, 2), Point(g_sqsize - center / 2, g_sqsize - center / 2), Point(g_sqsize - center / 2 - 8, g_sqsize - center / 2 - 16)); //x
            g_graphics_one->DrawLine(&Pen(xyzColor, 2), Point(g_sqsize - center / 2, g_sqsize - center / 2), Point(g_sqsize - center / 2 - 16, g_sqsize - center / 2 - 8));

            // painting labels
            g_graphics_one->DrawString(L"Z", -1, &Font(hdc), PointF(center + 10, center / 4), &SolidBrush(Color(255, 255, 255)));
            g_graphics_one->DrawString(L"Y", -1, &Font(hdc), PointF(center / 4, center + 3), &SolidBrush(Color(255, 255, 255)));
            g_graphics_one->DrawString(L"X", -1, &Font(hdc), PointF(g_sqsize - center / 2, g_sqsize - center / 2), &SolidBrush(Color(255, 255, 255)));

            // scalling on xyz
            int dx_one = int(dx*(center * 0.25f));
            int dy_one = int(dy*(center * 0.75f));
            int dz_one = int(dz*(center * 0.75f));

            Point point(center, center);

            // placing point
            point.X += -dy_one;
            point.Y += -dz_one;

            point.X += dx_one;
            point.Y += dx_one;                       

            // painting cube
            g_graphics_one->DrawLine(&Pen(linesColor, 1), Point(center, point.Y - dx_one), Point(point.X - dx_one, point.Y - dx_one));
            g_graphics_one->DrawLine(&Pen(linesColor, 1), Point(point.X - dx_one, center), Point(point.X - dx_one, point.Y - dx_one));

            g_graphics_one->DrawLine(&Pen(linesColor, 1), Point(point.X - dx_one, point.Y - dx_one), point);
            g_graphics_one->DrawLine(&Pen(linesColor, 1), Point(center + dx_one, center + dx_one), Point(center, center));
            
            g_graphics_one->DrawLine(&Pen(linesColor, 1), Point(center + dx_one, point.Y), point);
            g_graphics_one->DrawLine(&Pen(linesColor, 1), Point(point.X, center + dx_one), point);
            
            g_graphics_one->DrawLine(&Pen(linesColor, 1), Point(center, point.Y - dx_one), Point(center + dx_one, point.Y));
            g_graphics_one->DrawLine(&Pen(linesColor, 1), Point(point.X - dx_one, center), Point(point.X, center + dx_one));
            
            g_graphics_one->DrawLine(&Pen(linesColor, 1), Point(center, center), Point(center, point.Y - dx_one));
            g_graphics_one->DrawLine(&Pen(linesColor, 1), Point(center + dx_one, center + dx_one), Point(center + dx_one, point.Y));
            
            g_graphics_one->DrawLine(&Pen(linesColor, 1), Point(center, center), Point(point.X - dx_one, center));
            g_graphics_one->DrawLine(&Pen(linesColor, 1), Point(center + dx_one, center + dx_one), Point(point.X, center + dx_one));

            // painting point
            g_graphics_one->FillEllipse(&SolidBrush(pointsColor), Rect(Point(point.X - 3, point.Y - 3), Size(6, 6)));

            //painting backbuffer to screen
            BitBlt(hdc, 
                g_border, 
                g_border, 
                g_size.Width, 
                g_size.Height, 
                g_backBufferHDC_one, 
                0, 0, SRCCOPY);

            //--------------------------------------------------------------------------------------
            // Rendering second screen
            //--------------------------------------------------------------------------------------

            // background
            g_graphics_two->FillRectangle(&SolidBrush(Color(int(0.0f), int(255 * 0.125f), int(255 * 0.3f))), Rect(0, 0, g_size.Width, g_size.Height));

            // vert. and hor. lines
            g_graphics_two->DrawLine(&Pen(xyzColor, 2), Point(0, center), Point(g_sqsize, center));
            g_graphics_two->DrawLine(&Pen(xyzColor, 2), Point(center, 0), Point(center, g_sqsize));

            // painting labels
            g_graphics_two->DrawString(L"X", -1, &Font(hdc), PointF(0, center), &SolidBrush(Color(255, 255, 255)));
            g_graphics_two->DrawString(L"-Y", -1, &Font(hdc), PointF(center - 15, 0), &SolidBrush(Color(255, 255, 255)));
            g_graphics_two->DrawString(L"-Z", -1, &Font(hdc), PointF(center - 15, center * 2 - 15), &SolidBrush(Color(255, 255, 255)));
            g_graphics_two->DrawString(L"-Z", -1, &Font(hdc), PointF(center * 2 - 15, center - 15), &SolidBrush(Color(255, 255, 255)));

            // scalling
            int dx_two = int(dx*center);
            int dy_two = int(dy*center);
            int dz_two = int(dz*center);


            Point p1(center - dx_two, center + dy_two);
            Point p2(center - dx_two, center - dz_two);
            Point p3(center - dz_two, center + dy_two);

            Point k1(center, p2.Y);
            Point k2(p3.X, center);
            
            g_graphics_two->DrawLine(&Pen(linesColor, 1), p1, p2);
            g_graphics_two->DrawLine(&Pen(linesColor, 1), p1, p3);
            g_graphics_two->DrawLine(&Pen(linesColor, 1), p2, k1);
            g_graphics_two->DrawLine(&Pen(linesColor, 1), p3, k2);

            g_graphics_two->DrawBezier(&Pen(linesColor, 1), k2, Point(k2.X, k1.Y), Point(k2.X, k1.Y), k1);

            // painting points
            p1.X -= 3; p1.Y -= 3;
            p2.X -= 3; p2.Y -= 3;
            p3.X -= 3; p3.Y -= 3;
            g_graphics_two->FillEllipse(&SolidBrush(pointsColor), Rect(p1, Size(6, 6)));
            g_graphics_two->FillEllipse(&SolidBrush(pointsColor), Rect(p2, Size(6, 6)));
            g_graphics_two->FillEllipse(&SolidBrush(pointsColor), Rect(p3, Size(6, 6)));

            //painting backbuffer to screen
            BitBlt(hdc, 
                1 * g_sqsize + g_border * 2,
                g_border, 
                g_size.Width, 
                g_size.Height, 
                g_backBufferHDC_two, 
                0, 0, SRCCOPY);



            //--------------------------------------------------------------------------------------
            // Printing texts
            //--------------------------------------------------------------------------------------

            // scalling
            int sdx = int(dx*100);
            int sdy = int(dy*100);
            int sdz = int(dz*100);

            //clearing text place
            Rectangle(hdc, g_border - 5, g_sqsize + g_border * 2 + g_border * 3 + g_scrollheight * 3 - 5, g_border + 100, g_sqsize + g_border * 2 + g_border * 3 + g_scrollheight * 3 + 60);

            std::wstring sx = L"x = " + std::to_wstring(sdx);
            TextOut(hdc, g_border, g_sqsize + g_border * 2 + g_border * 3 + g_scrollheight * 3, sx.c_str(), sx.size());

            std::wstring sy = L"y = " + std::to_wstring(sdy);
            TextOut(hdc, g_border, g_sqsize + g_border * 2 + g_border * 4 + g_scrollheight * 3, sy.c_str(), sy.size());

            std::wstring sz = L"z = " + std::to_wstring(sdz);
            TextOut(hdc, g_border, g_sqsize + g_border * 2 + g_border * 5 + g_scrollheight * 3, sz.c_str(), sz.size());
            


            //return old font to system
            SelectObject(hdc, _old);
            DeleteObject(_new);

            //end paint event
            EndPaint(hWindow, &ps);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWindow, message, wParam, lParam);
    }

    return 0;
}

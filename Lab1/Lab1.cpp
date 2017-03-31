// Lab1.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Lab1.h"
#include "GraphElements.h"

INT_PTR CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // init GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    
    // run window
    DialogBox(NULL, MAKEINTRESOURCE(IDD_ABOUTBOX), NULL, wndProc);

    return 0;
}

//--------------------------------------------------------------------------------------
// GDI+
//--------------------------------------------------------------------------------------

class WindowPlate {
public:
    WindowPlate(function<void(Graphics&, PointF)> paint) : paint(paint) {
        bufferSize = Size(350, 350);

        //init backBuffer
        backBuffer = new byte[bufferSize.Width * bufferSize.Height * 4];
        backBufferHBitmap = CreateBitmap(bufferSize.Width, bufferSize.Height, 1, 32, backBuffer);

        //init HDC
        backBufferHDC = CreateCompatibleDC(nullptr);
        SelectObject(backBufferHDC, backBufferHBitmap);

        //init graphics
        backBufferGraphics = new Graphics(backBufferHDC);
        backBufferGraphics->SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);

    }

    void render(HDC hdc, int x, int y) {
        backBufferGraphics->Clear(Color(int(0.0f), int(255 * 0.125f), int(255 * 0.3f)));

        paint(*backBufferGraphics, PointF(bufferSize.Width/2, bufferSize.Height/2));

        BitBlt(hdc, x, y, bufferSize.Width, bufferSize.Height, backBufferHDC, 0, 0, SRCCOPY);
    }

    Size getSize() {
        return bufferSize;
    }
private:    
    Size bufferSize;

    byte *backBuffer; //backBuffer bits
    HBITMAP backBufferHBitmap;//backBuffer handle
    HDC backBufferHDC;//backBuffer hdc
    Graphics *backBufferGraphics;//graphics

    function<void(Graphics&, PointF)> paint;

public:
    ~WindowPlate() {
        //delete backBuffer
        delete[] backBuffer;
        DeleteObject(backBufferHBitmap);

        //delete HDC and graphics
        DeleteDC(backBufferHDC);
        delete backBufferGraphics;
    }
};

void paintFirstPlate(Graphics &graphics, PointF center);
void paintSecondPlate(Graphics &graphics, PointF center);

//--------------------------------------------------------------------------------------
// Common elements
//--------------------------------------------------------------------------------------

GraphTargetPoint *targetPoint;

vector<GraphElement*> g_world;

vector<GraphElement*> g_worldFirst;

void initCommonElements() {

    Color xyzColor(140, 140, 140);
    int xyzHeight = 130;

    Color pointsColor(255, 255, 255);
    Color linesColor(120, 0, 255 / 4 * 3, 255);

    //xyz
    g_worldFirst.push_back(new GraphLine(GraphTargetPoint(xyzHeight, 0, 0), GraphTargetPoint(-xyzHeight, 0, 0), xyzColor));
    g_worldFirst.push_back(new GraphLine(GraphTargetPoint(0, xyzHeight, 0), GraphTargetPoint(0, -xyzHeight, 0), xyzColor));
    g_worldFirst.push_back(new GraphLine(GraphTargetPoint(0, 0, xyzHeight), GraphTargetPoint(0, 0, -xyzHeight), xyzColor));

    //xyz arrows
    g_worldFirst.push_back(new GraphLine(GraphTargetPoint(xyzHeight, 0, 0), GraphTargetPoint(xyzHeight - 20, 0, 10), xyzColor));
    g_worldFirst.push_back(new GraphLine(GraphTargetPoint(xyzHeight, 0, 0), GraphTargetPoint(xyzHeight - 20, 0, -10), xyzColor));

    g_worldFirst.push_back(new GraphLine(GraphTargetPoint(0, xyzHeight, 0), GraphTargetPoint(0, xyzHeight - 20, 10), xyzColor));
    g_worldFirst.push_back(new GraphLine(GraphTargetPoint(0, xyzHeight, 0), GraphTargetPoint(0, xyzHeight - 20, -10), xyzColor));

    g_worldFirst.push_back(new GraphLine(GraphTargetPoint(0, 0, xyzHeight), GraphTargetPoint(10, 0, xyzHeight - 20), xyzColor));
    g_worldFirst.push_back(new GraphLine(GraphTargetPoint(0, 0, xyzHeight), GraphTargetPoint(-10, 0, xyzHeight - 20), xyzColor));

    //point
    g_world.push_back(targetPoint = new GraphTargetPoint(0, 0, 0, Color(255, 255, 255)));
    g_world.push_back(new GraphWrapCube(*targetPoint, linesColor));

}


//slider helper
int sliderDx;
int sliderDy;
int sliderDz;

//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
INT_PTR CALLBACK wndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);

    static WindowPlate *firstPlate;
    static WindowPlate *secondPlate;

    switch (message) {
        case WM_INITDIALOG:
        {
            initCommonElements();
            firstPlate = new WindowPlate(paintFirstPlate);
            secondPlate = new WindowPlate(paintSecondPlate);
            
            //III octant
            sliderDx = 1;
            sliderDy = -1;
            sliderDz = -1;

            SendMessage(GetDlgItem(hDlg, IDC_SLIDER_X), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 100));  // min. & max. positions
            SendMessage(GetDlgItem(hDlg, IDC_SLIDER_X), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);

            SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Y), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 100));  // min. & max. positions
            SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Y), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);

            SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Z), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 100));
            SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Z), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);
            
            return (INT_PTR)TRUE;
        }
        case WM_PAINT:
        {
            // init paint event
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hDlg, &ps);

            firstPlate->render(hdc, 10, 10);
            secondPlate->render(hdc, 10 + firstPlate->getSize().Width + 10, 10);

            wstring s = L"Position: (";
            s += to_wstring((int)targetPoint->x);
            s += L", ";
            s += to_wstring((int)targetPoint->y);
            s += L", ";
            s += to_wstring((int)targetPoint->z);
            s += L")";

            SendMessage(GetDlgItem(hDlg, IDC_STATIC_POSITION), WM_SETTEXT, 0, (LPARAM)(s.c_str()));

            EndPaint(hDlg, &ps);
            break;
        }
        case WM_HSCROLL:
        {
            if ((HWND)lParam == GetDlgItem(hDlg, IDC_SLIDER_X)) {
                targetPoint->x = sliderDx*SendMessage(GetDlgItem(hDlg, IDC_SLIDER_X), TBM_GETPOS, 0, 0);
                InvalidateRect(hDlg, NULL, false);
            }
            if ((HWND)lParam == GetDlgItem(hDlg, IDC_SLIDER_Y)) {
                targetPoint->y = sliderDy*SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Y), TBM_GETPOS, 0, 0);
                InvalidateRect(hDlg, NULL, false);
            }
            if ((HWND)lParam == GetDlgItem(hDlg, IDC_SLIDER_Z)) {
                targetPoint->z = sliderDz*SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Z), TBM_GETPOS, 0, 0);
                InvalidateRect(hDlg, NULL, false);
            }
            break;
        }
        case WM_COMMAND:
        {
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, LOWORD(wParam));
                delete firstPlate;
                delete secondPlate;
                return (INT_PTR)TRUE;
            }

            if (LOWORD(wParam) == IDC_BUTTON_RESETX) {
                SendMessage(GetDlgItem(hDlg, IDC_SLIDER_X), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);
                targetPoint->x = 0;
                InvalidateRect(hDlg, NULL, false);
            }
            if (LOWORD(wParam) == IDC_BUTTON_RESETY) {
                SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Y), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);
                targetPoint->y = 0;
                InvalidateRect(hDlg, NULL, false);
            }
            if (LOWORD(wParam) == IDC_BUTTON_RESETZ) {
                SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Z), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);
                targetPoint->z = 0;
                InvalidateRect(hDlg, NULL, false);
            }
            break;
        }
    }
    return (INT_PTR)FALSE;
}



//--------------------------------------------------------------------------------------
// GDI+ paint provider
//--------------------------------------------------------------------------------------

void paintFirstPlate(Graphics &graphics, PointF center_p) {

    int center = center_p.X;
    Font font(L"Arial", 12, FontStyleBold);
    SolidBrush textBrush(Color(255, 255, 255));

    // painting labels
    graphics.DrawString(L"Z", -1, &font, PointF(center + 10, center / 4), &textBrush);
    graphics.DrawString(L"X", -1, &font, PointF(center / 4, center + 9), &textBrush);
    graphics.DrawString(L"Y", -1, &font, PointF(center*2 - center / 2 + 9, center*2 - center / 2 + 9), &textBrush);

    
    for (auto ob : g_worldFirst) {
        ob->paint(graphics, center_p);
    }
    for (auto ob : g_world) {
        ob->paint(graphics, center_p);
    }
};


void paintSecondPlate(Graphics &graphics, PointF center_p) {

    int center = center_p.X;
    Font font(L"Arial", 12, FontStyleBold);
    SolidBrush textBrush(Color(255, 255, 255));
    // painting labels
    graphics.DrawString(L"-Y", -1, &font, PointF(0, center + 3), &textBrush);
    graphics.DrawString(L"Y", -1, &font, PointF(center*2 - 18, center + 3), &textBrush);

    graphics.DrawString(L"X", -1, &font, PointF(3, center - 23), &textBrush);
    graphics.DrawString(L"-X", -1, &font, PointF(center * 2 - 23, center - 23), &textBrush);


    graphics.DrawString(L"-Y", -1, &font, PointF(center - 23, 5), &textBrush);
    graphics.DrawString(L"Z", -1, &font, PointF(center + 5, 5), &textBrush);

    graphics.DrawString(L"-Z", -1, &font, PointF(center - 23, center*2 - 23), &textBrush);
    graphics.DrawString(L"Y", -1, &font, PointF(center + 5,   center*2 - 23), &textBrush);

    Color xyzColor(140, 140, 140);
    // vert. and hor. lines
    graphics.DrawLine(&Pen(xyzColor, 2), Point(0, center), Point(center*2, center));
    graphics.DrawLine(&Pen(xyzColor, 2), Point(center, 0), Point(center, center*2));
    
    for (auto ob : g_world) {
        ob->paintComplex(graphics, center_p);
    }
};
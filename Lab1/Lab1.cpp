// Lab1.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Lab1.h"

#include "WindowPlate.h"
#include "GraphElement.h"
#include "GraphLine.h"
#include "GraphPoint.h"
#include "GraphWrapPoint.h"
#include "GraphXYZ.h"

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
// Common elements
//--------------------------------------------------------------------------------------

GraphPoint *targetPoint;

vector<GraphElement*> g_world;

void initCommonElements() {

	g_world.push_back(new GraphXYZ(Color(140, 140, 140), Color(255, 255, 255)));
    
    g_world.push_back(targetPoint = new GraphPoint(0, 0, 0, Color(255, 255, 255)));
    g_world.push_back(new GraphWrapCube(*targetPoint, Color(120, 0, 255 / 4 * 3, 255)));

}


//slider helper
int sliderDx;
int sliderDy;
int sliderDz;

void paintFirstPlate(Graphics &graphics, PointF center);
void paintSecondPlate(Graphics &graphics, PointF center);

//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
INT_PTR CALLBACK wndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);

    static WindowPlate *firstPlate;
    static WindowPlate *secondPlate;

    switch (message) {
		case WM_DESTROY: {
			for (auto &ob : g_world) {
				delete ob;
			}
			g_world.clear();
			delete firstPlate;
			delete secondPlate;
			break;
		}
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
    for (auto ob : g_world) {
        ob->paint(graphics, center_p);
    }
};


void paintSecondPlate(Graphics &graphics, PointF center_p) {    
    for (auto ob : g_world) {
        ob->paintComplex(graphics, center_p);
    }
};
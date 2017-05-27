// Lab2.cpp: ���������� ����� ����� ��� ����������.
//

#include "stdafx.h"
#include "Lab2.h"
#include <map>
#include <set>

#include "../CommonFiles/CGCommon.h"

#include "GraphWrapLine.h"

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

GraphLine *targetLine;
vector<GraphElement*> g_world;

set<GraphElement*> wrapCubes;
set<GraphLabel*> wrapLabels;

map<wchar_t, GraphPoint*> targetPoints;

void initCommonElements() {

	targetPoints[L'A'] = new GraphPoint(0, 0, 0, L"A");
	targetPoints[L'B'] = new GraphPoint(0, 0, 0, L"B");

	g_world.push_back(new GraphXYZ(Color(140, 140, 140), Color(255, 255, 255)));

	targetLine = new GraphLine(*targetPoints[L'A'], *targetPoints[L'B'], Color(255, 255, 255));

	GraphWrapLine *wrapLine = new GraphWrapLine(*targetLine, Color(120, 0, 255 / 4 * 3, 255), 1);
	g_world.push_back(wrapLine);
	wrapLine->setVisible(false);
	wrapCubes.insert(wrapLine);

	g_world.push_back(targetLine);

	for (auto &ob : targetPoints) {
		GraphLabel *wrapLabel = new GraphLabel(*ob.second, Color::Yellow);
		wrapLabel->setVisible(false);
		wrapLabels.insert(wrapLabel);
		g_world.push_back(wrapLabel);
	}
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
		case WM_DESTROY:
		{
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
			sliderDy = 1;
			sliderDz = 1;

			SendMessage(GetDlgItem(hDlg, IDC_SLIDER_X1), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(-100, 100));  // min. & max. positions
			SendMessage(GetDlgItem(hDlg, IDC_SLIDER_X1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);

			SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Y1), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(-100, 100));  // min. & max. positions
			SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Y1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);

			SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Z1), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(-100, 100));
			SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Z1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);

			//init list

			SendMessage(GetDlgItem(hDlg, IDC_LISTOFPOINTS), LB_ADDSTRING, 0, (LPARAM)L"A (line)");
			SendMessage(GetDlgItem(hDlg, IDC_LISTOFPOINTS), LB_ADDSTRING, 0, (LPARAM)L"B (line)");

			SendMessage(GetDlgItem(hDlg, IDC_LISTOFPOINTS), LB_SETCURSEL, 0, 0);

			return (INT_PTR)TRUE;
		}
		case WM_PAINT:
		{
			// init paint event
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hDlg, &ps);


			firstPlate->render();
			secondPlate->render();

			firstPlate->blt(hdc, 10, 10);
			secondPlate->blt(hdc, 10 + firstPlate->getSize().Width + 10, 10);


			wchar_t buf[255];
			int it = SendMessage(GetDlgItem(hDlg, IDC_LISTOFPOINTS), LB_GETCURSEL, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_LISTOFPOINTS), LB_GETTEXT, it, (LPARAM)buf);

			if (targetPoints[buf[0]] != nullptr) {
				wstring s;
				s.push_back(buf[0]);
				s += L" position: (";
				s += to_wstring((int)targetPoints[buf[0]]->x);
				s += L", ";
				s += to_wstring((int)targetPoints[buf[0]]->y);
				s += L", ";
				s += to_wstring((int)targetPoints[buf[0]]->z);
				s += L")";
				SendMessage(GetDlgItem(hDlg, IDC_STATIC_POSITION1), WM_SETTEXT, 0, (LPARAM)(s.c_str()));
			}
			

			EndPaint(hDlg, &ps);
			break;
		}
		case WM_HSCROLL:
		{
			wchar_t buf[255];
			int it = SendMessage(GetDlgItem(hDlg, IDC_LISTOFPOINTS), LB_GETCURSEL, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_LISTOFPOINTS), LB_GETTEXT, it, (LPARAM)buf);

			if (targetPoints[buf[0]] == nullptr) {
				break;
			}

			if ((HWND)lParam == GetDlgItem(hDlg, IDC_SLIDER_X1)) {
				targetPoints[buf[0]]->x = sliderDx*SendMessage(GetDlgItem(hDlg, IDC_SLIDER_X1), TBM_GETPOS, 0, 0);
				InvalidateRect(hDlg, NULL, false);
			}
			if ((HWND)lParam == GetDlgItem(hDlg, IDC_SLIDER_Y1)) {
				targetPoints[buf[0]]->y = sliderDy*SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Y1), TBM_GETPOS, 0, 0);
				InvalidateRect(hDlg, NULL, false);
			}
			if ((HWND)lParam == GetDlgItem(hDlg, IDC_SLIDER_Z1)) {
				targetPoints[buf[0]]->z = sliderDz*SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Z1), TBM_GETPOS, 0, 0);
				InvalidateRect(hDlg, NULL, false);
			}

			if ((HWND)lParam == GetDlgItem(hDlg, IDC_SLIDER_X2)) {
				targetPoints[buf[0]]->x = sliderDx*SendMessage(GetDlgItem(hDlg, IDC_SLIDER_X2), TBM_GETPOS, 0, 0);
				InvalidateRect(hDlg, NULL, false);
			}
			if ((HWND)lParam == GetDlgItem(hDlg, IDC_SLIDER_Y2)) {
				targetPoints[buf[0]]->y = sliderDy*SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Y2), TBM_GETPOS, 0, 0);
				InvalidateRect(hDlg, NULL, false);
			}
			if ((HWND)lParam == GetDlgItem(hDlg, IDC_SLIDER_Z2)) {
				targetPoints[buf[0]]->z = sliderDz*SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Z2), TBM_GETPOS, 0, 0);
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

			wchar_t buf[255];
			int it = SendMessage(GetDlgItem(hDlg, IDC_LISTOFPOINTS), LB_GETCURSEL, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_LISTOFPOINTS), LB_GETTEXT, it, (LPARAM)buf);

			if (targetPoints[buf[0]] == nullptr) {
				break;
			}

			if (LOWORD(wParam) == IDC_LISTOFPOINTS) {
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER_X1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)int(targetPoints[buf[0]]->x)*sliderDx);
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Y1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)int(targetPoints[buf[0]]->y)*sliderDx);
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Z1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)int(targetPoints[buf[0]]->z)*sliderDx);
			}


			if (LOWORD(wParam) == IDC_BUTTON_RESETX1) {
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER_X1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);
				targetPoints[buf[0]]->x = 0;
				InvalidateRect(hDlg, NULL, false);
			}
			if (LOWORD(wParam) == IDC_BUTTON_RESETY1) {
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Y1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);
				targetPoints[buf[0]]->y = 0;
				InvalidateRect(hDlg, NULL, false);
			}
			if (LOWORD(wParam) == IDC_BUTTON_RESETZ1) {
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Z1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);
				targetPoints[buf[0]]->z = 0;
				InvalidateRect(hDlg, NULL, false);
			}


			if (LOWORD(wParam) == IDC_BUTTON_RESETALL) {
				for (auto &ob : targetPoints) {
					ob.second->x = 0;
					ob.second->y = 0;
					ob.second->z = 0;
				}

				SendMessage(GetDlgItem(hDlg, IDC_SLIDER_X1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)int(targetPoints[buf[0]]->x)*sliderDx);
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Y1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)int(targetPoints[buf[0]]->y)*sliderDx);
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Z1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)int(targetPoints[buf[0]]->z)*sliderDx);
				InvalidateRect(hDlg, NULL, false);
			}



			if (LOWORD(wParam) == IDC_CHECKBOX_LABELS) {
				bool flag = SendMessage(GetDlgItem(hDlg, IDC_CHECKBOX_LABELS), BM_GETCHECK, 0, 0);
				for (auto &ob : wrapLabels) {
					ob->setVisible(flag);
				}
				InvalidateRect(hDlg, NULL, false);
			}

			if (LOWORD(wParam) == IDC_CHECKBOX_WRAPS) {
				bool flag = SendMessage(GetDlgItem(hDlg, IDC_CHECKBOX_WRAPS), BM_GETCHECK, 0, 0);
				for (auto &ob : wrapCubes) {
					ob->setVisible(flag);
				}
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
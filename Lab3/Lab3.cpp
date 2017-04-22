// Lab3.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Lab3.h"

#include "WindowPlate.h"
#include "GraphElement.h"
#include "GraphLine.h"
#include "GraphPoint.h"
#include "GraphWrapCube.h"
#include "GraphXYZ.h"
#include "GraphTriangle.h"
#include "GraphLabel.h"
#include "GraphSpecialLine.h"

#include <map>
#include <set>

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

vector<GraphElement*> g_world;

map<wchar_t, GraphPoint*> targetPoints;

set<GraphWrapCube*> wrapCubes;
set<GraphLabel*> wrapLabels;


vector<GraphElement*> intersectItems;
GraphTriangle *graphTriangle;
GraphSpecialLine *graphLine;

HWND g_hDlg;

void intersectLineWithTriangle() {

	for (auto ob : intersectItems) {
		wrapCubes.erase((GraphWrapCube*)ob);
		wrapLabels.erase((GraphLabel*)ob);
		delete ob;
	}
	intersectItems.clear();

	GraphPoint &a = graphTriangle->a;
	GraphPoint &b = graphTriangle->b;
	GraphPoint &c = graphTriangle->c;

	GraphPoint &m = graphLine->a;
	GraphPoint &n = graphLine->b;

	float A = (a.y - b.y)*(a.z + b.z) + (b.y - c.y)*(b.z + c.z) + (c.y - a.y)*(c.z + a.z);
	float B = (a.z - b.z)*(a.x + b.x) + (b.z - c.z)*(b.x + c.x) + (c.z - a.z)*(c.x + a.x);
	float C = (a.x - b.x)*(a.y + b.y) + (b.x - c.x)*(b.y + c.y) + (c.x - a.x)*(c.y + a.y);

	float D = -(A*a.x + B*a.y + C*a.z);

	float t = -1;
	float zn = (A*(m.x - n.x) + B*(m.y - n.y) + C*(m.z - n.z));
	if (zn != 0) {
		t = (A*m.x + B*m.y + C*m.z + D) / zn;
	};

	if (t >= 0 && t <= 1) {
		float tx = m.x + (float)(n.x - m.x)*t;
		float ty = m.y + (float)(n.y - m.y)*t;
		float tz = m.z + (float)(n.z - m.z)*t;
		GraphPoint *intersectPoint = new GraphPoint(tx, ty, tz, Color::Red);
		intersectPoint->name = L"T";
		graphLine->intersectPoint = intersectPoint;

		GraphWrapCube *cube = new GraphWrapCube(*intersectPoint, Color(160, 160, 255), 1);
		GraphLabel *label = new GraphLabel(*intersectPoint, Color::Yellow);

		cube->visible = SendMessage(GetDlgItem(g_hDlg, IDC_CHECKBOX_WRAPPERS), BM_GETCHECK, 0, 0);
		label->visible = SendMessage(GetDlgItem(g_hDlg, IDC_CHECKBOX_LABELS), BM_GETCHECK, 0, 0);

		wrapCubes.insert(cube);
		wrapLabels.insert(label);

		intersectItems.push_back(cube);
		intersectItems.push_back(label);

		intersectItems.push_back(intersectPoint);
	} else {
		graphLine->intersectPoint = nullptr;
	}

	
}

void initCommonElements() {

	targetPoints[L'A'] = new GraphPoint(0, 0, 0, L"A");
	targetPoints[L'B'] = new GraphPoint(0, 0, 0, L"B");
	targetPoints[L'C'] = new GraphPoint(0, 0, 0, L"C");
	targetPoints[L'M'] = new GraphPoint(0, 0, 0, L"M");
	targetPoints[L'N'] = new GraphPoint(0, 0, 0, L"N");

	g_world.push_back(new GraphXYZ(Color(140, 140, 140), Color(255, 255, 255)));
	
	for (auto &ob : targetPoints) {
		GraphWrapCube *wrapCube = new GraphWrapCube(*ob.second, Color(160, 160, 255), 1);
		wrapCube->visible = false;
		wrapCubes.insert(wrapCube);
		g_world.push_back(wrapCube);
	}

	g_world.push_back(graphTriangle = new GraphTriangle(*targetPoints[L'A'], *targetPoints[L'B'], *targetPoints[L'C'], Color(255, 170, 170)));

	g_world.push_back(graphLine = new GraphSpecialLine(*targetPoints[L'M'], *targetPoints[L'N'], *graphTriangle, Color(170, 255, 170)));


	for (auto &ob : targetPoints) {
		GraphLabel *wrapLabel = new GraphLabel(*ob.second, Color::Yellow);
		wrapLabel->visible = false;
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
			for (auto &ob : intersectItems) {
				delete ob;
			}
			intersectItems.clear();
			delete firstPlate;
			delete secondPlate;
			break;
		}
		case WM_INITDIALOG:
		{
			g_hDlg = hDlg;
			initCommonElements();
			firstPlate = new WindowPlate(paintFirstPlate);
			secondPlate = new WindowPlate(paintSecondPlate);

			//I octant
			sliderDx = 1;
			sliderDy = 1;
			sliderDz = 1;

			SendMessage(GetDlgItem(hDlg, IDC_SLIDER_X1), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 100));  // min. & max. positions
			SendMessage(GetDlgItem(hDlg, IDC_SLIDER_X1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);

			SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Y1), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 100));  // min. & max. positions
			SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Y1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);

			SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Z1), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 100));
			SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Z1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);


			//init list

			SendMessage(GetDlgItem(hDlg, IDC_LISTOFPOINTS), LB_ADDSTRING, 0, (LPARAM)L"A (triangle)");
			SendMessage(GetDlgItem(hDlg, IDC_LISTOFPOINTS), LB_ADDSTRING, 0, (LPARAM)L"B (triangle)");
			SendMessage(GetDlgItem(hDlg, IDC_LISTOFPOINTS), LB_ADDSTRING, 0, (LPARAM)L"C (triangle)");
			SendMessage(GetDlgItem(hDlg, IDC_LISTOFPOINTS), LB_ADDSTRING, 0, (LPARAM)L"M (line)");
			SendMessage(GetDlgItem(hDlg, IDC_LISTOFPOINTS), LB_ADDSTRING, 0, (LPARAM)L"N (line)");

			SendMessage(GetDlgItem(hDlg, IDC_LISTOFPOINTS), LB_SETCURSEL, 0, 0);

			return (INT_PTR)TRUE;
		}
		case WM_PAINT:
		{
			// init paint event
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hDlg, &ps);

			firstPlate->render(hdc, 10, 10);
			secondPlate->render(hdc, 10 + firstPlate->getSize().Width + 10, 10);



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
			intersectLineWithTriangle();
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
					ob->visible = flag;
				}

				InvalidateRect(hDlg, NULL, false);
			}

			if (LOWORD(wParam) == IDC_CHECKBOX_WRAPPERS) {
				bool flag = SendMessage(GetDlgItem(hDlg, IDC_CHECKBOX_WRAPPERS), BM_GETCHECK, 0, 0);

				for (auto &ob : wrapCubes) {
					ob->visible = flag;
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
	for (auto ob : intersectItems) {
		ob->paint(graphics, center_p);
	}
};


void paintSecondPlate(Graphics &graphics, PointF center_p) {
	for (auto ob : g_world) {
		ob->paintComplex(graphics, center_p);
	}
	for (auto ob : intersectItems) {
		ob->paintComplex(graphics, center_p);
	}
};
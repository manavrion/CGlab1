// Lab4.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Lab4.h"

#include "GraphElement.h"
#include "GraphPoint.h"
#include "GraphLine.h"
#include "GraphWrapCube.h"
#include "GraphXYZ.h"
#include "GraphLabel.h"

#include "GraphCube.h"

#include "../CommonFiles/WindowPlate.h"
#include <map>
#include <set>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

bool g_debug = false;
bool g_fps = false;
bool g_proj = false;

INT_PTR CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

#undef max
#undef min

struct Mxn {
	Mxn() : min(), max(), def(), dx(1.0) {};
	Mxn(int min, int max, int def = 0, float dx = 1) : min(min), max(max), def(def), dx(dx) {};
	int min;
	int max;
	int def;
	float dx;
};

struct SliderPermition {
	SliderPermition()
		: x(), y(), z(), lockX(), lockY(), lockZ() {};
	SliderPermition(Mxn x, Mxn y, Mxn z, bool lockX = false, bool lockY = false, bool lockZ = false) 
		: x(x), y(y), z(z), lockX(lockX), lockY(lockY), lockZ(lockZ) {};
	Mxn x;
	Mxn y;
	Mxn z;
	bool lockX, lockY, lockZ;
};


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
map<wchar_t, SliderPermition> sliderPermitions;

set<GraphWrapCube*> wrapCubes;
set<GraphLabel*> wrapLabels;


GraphLine *graphLine;

GraphCube *graphCube;

void initCommonElements() {

	targetPoints[L'M'] = new GraphPoint(0, 0, 0, L"M");
	targetPoints[L'N'] = new GraphPoint(0, 0, 0, L"N");

	targetPoints[L'C'] = new GraphPoint(0, 0, 50, L"C");


	targetPoints[L'F'] = new GraphPoint(0, 0, 20, L"F");
	targetPoints[L'S'] = new GraphPoint(0, 0, 8, L"S");

	sliderPermitions[L'M'] = SliderPermition(Mxn(0, 100, 0, 1), Mxn(0, 100, 0, 1), Mxn(0, 100, 0, 1));
	sliderPermitions[L'N'] = SliderPermition(Mxn(0, 100, 0, 1), Mxn(0, 100, 0, 1), Mxn(0, 100, 0, 1));
	sliderPermitions[L'C'] = SliderPermition(Mxn(0, 100, 0, 1), Mxn(0, 100, 0, 1), Mxn(0, 1000, 50*10, 0.1), true, true, false);


	g_world.push_back(new GraphXYZ(Color(140, 140, 140), Color(140, 255, 255, 255)));
	
	for (auto &ob : targetPoints) {
		GraphWrapCube *wrapCube = new GraphWrapCube(*ob.second, Color(160, 160, 255), 1);
		wrapCube->setVisible(false);
		wrapCubes.insert(wrapCube);
		g_world.push_back(wrapCube);
	}
	
	g_world.push_back(graphCube = new GraphCube(0, 50));

	g_world.push_back(graphLine = new GraphLine(*targetPoints[L'M'], *targetPoints[L'N'], Color(170, 255, 170)));
	g_world.push_back(targetPoints[L'C']);

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


void slidersReset() {

}

//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
INT_PTR CALLBACK wndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);

	static WindowPlate *firstPlate;
	static WindowPlate *secondPlate;

	static function<void(wchar_t)> initSliders;

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

			//init list
			SendMessage(GetDlgItem(hDlg, IDC_LISTOFPOINTS), LB_ADDSTRING, 0, (LPARAM)L"C (view point)");
			SendMessage(GetDlgItem(hDlg, IDC_LISTOFPOINTS), LB_ADDSTRING, 0, (LPARAM)L"M (line)");
			SendMessage(GetDlgItem(hDlg, IDC_LISTOFPOINTS), LB_ADDSTRING, 0, (LPARAM)L"N (line)");
			SendMessage(GetDlgItem(hDlg, IDC_LISTOFPOINTS), LB_SETCURSEL, 0, 0);

			initSliders = [=](wchar_t obj) {
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER_X1), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(sliderPermitions[obj].x.min, sliderPermitions[obj].x.max));
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Y1), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(sliderPermitions[obj].y.min, sliderPermitions[obj].y.max));
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Z1), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(sliderPermitions[obj].z.min, sliderPermitions[obj].z.max));

				SendMessage(GetDlgItem(hDlg, IDC_SLIDER_X1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)int(targetPoints[obj]->x / sliderPermitions[obj].x.dx));
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Y1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)int(targetPoints[obj]->y / sliderPermitions[obj].y.dx));
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Z1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)int(targetPoints[obj]->z / sliderPermitions[obj].z.dx));
			};

			initSliders(L'C');

			return (INT_PTR)TRUE;
		}
		case WM_PAINT:
		{
			auto start = Clock::now();
			// init paint event
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hDlg, &ps);

			secondPlate->render();
			firstPlate->render();
						
			secondPlate->render();		
			
			firstPlate->blt(hdc, 10, 10);

			auto end = Clock::now();
			if (g_fps) {
				secondPlate->paintTimeOfFrame(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
			}
			
			secondPlate->blt(hdc, 10 + firstPlate->getSize().Width + 10, 10);

			wchar_t buf[255];
			int it = SendMessage(GetDlgItem(hDlg, IDC_LISTOFPOINTS), LB_GETCURSEL, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_LISTOFPOINTS), LB_GETTEXT, it, (LPARAM)buf);

			if (targetPoints[buf[0]] != nullptr) {
				wstring s;
				s.push_back(buf[0]);
				s += L" position: (";
				s += to_wstring(targetPoints[buf[0]]->x);
				s += L", ";
				s += to_wstring(targetPoints[buf[0]]->y);
				s += L", ";
				s += to_wstring(targetPoints[buf[0]]->z);
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
				targetPoints[buf[0]]->x = SendMessage(GetDlgItem(hDlg, IDC_SLIDER_X1), TBM_GETPOS, 0, 0) * sliderPermitions[buf[0]].x.dx;
				InvalidateRect(hDlg, NULL, false);
			}
			if ((HWND)lParam == GetDlgItem(hDlg, IDC_SLIDER_Y1)) {
				targetPoints[buf[0]]->y = SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Y1), TBM_GETPOS, 0, 0) * sliderPermitions[buf[0]].y.dx;
				InvalidateRect(hDlg, NULL, false);
			}
			if ((HWND)lParam == GetDlgItem(hDlg, IDC_SLIDER_Z1)) {
				targetPoints[buf[0]]->z = SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Z1), TBM_GETPOS, 0, 0) * sliderPermitions[buf[0]].z.dx;
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
				initSliders(buf[0]);
			}


			if (LOWORD(wParam) == IDC_BUTTON_RESETX1) {
				if (!sliderPermitions[buf[0]].lockX) {
					SendMessage(GetDlgItem(hDlg, IDC_SLIDER_X1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)sliderPermitions[buf[0]].x.def);
					targetPoints[buf[0]]->x = sliderPermitions[buf[0]].x.def * sliderPermitions[buf[0]].x.dx;
					InvalidateRect(hDlg, NULL, false);
				}
			}
			if (LOWORD(wParam) == IDC_BUTTON_RESETY1) {
				if (!sliderPermitions[buf[0]].lockY) {
					SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Y1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)sliderPermitions[buf[0]].y.def);
					targetPoints[buf[0]]->y = sliderPermitions[buf[0]].y.def * sliderPermitions[buf[0]].y.dx;
					InvalidateRect(hDlg, NULL, false);
				}
			}
			if (LOWORD(wParam) == IDC_BUTTON_RESETZ1) {
				if (!sliderPermitions[buf[0]].lockZ) {
					SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Z1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)sliderPermitions[buf[0]].z.def);
					targetPoints[buf[0]]->z = sliderPermitions[buf[0]].z.def * sliderPermitions[buf[0]].z.dx;
					InvalidateRect(hDlg, NULL, false);
				}
			}


			if (LOWORD(wParam) == IDC_BUTTON_RESETALL) {
				for (auto &ob : targetPoints) {
					if (!sliderPermitions[ob.first].lockX) {
						targetPoints[ob.first]->x = sliderPermitions[ob.first].x.def * sliderPermitions[buf[0]].x.dx;
					}
					if (!sliderPermitions[ob.first].lockY) {
						targetPoints[ob.first]->y = sliderPermitions[ob.first].y.def * sliderPermitions[buf[0]].y.dx;
					}
					if (!sliderPermitions[ob.first].lockZ) {
						targetPoints[ob.first]->z = sliderPermitions[ob.first].z.def * sliderPermitions[buf[0]].z.dx;
					}
				}

				graphCube->resetCube();

				if (!sliderPermitions[buf[0]].lockX) {
					SendMessage(GetDlgItem(hDlg, IDC_SLIDER_X1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)sliderPermitions[buf[0]].x.def);
					targetPoints[buf[0]]->x = sliderPermitions[buf[0]].x.def * sliderPermitions[buf[0]].x.dx;
					InvalidateRect(hDlg, NULL, false);
				}
				if (!sliderPermitions[buf[0]].lockY) {
					SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Y1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)sliderPermitions[buf[0]].y.def);
					targetPoints[buf[0]]->y = sliderPermitions[buf[0]].y.def * sliderPermitions[buf[0]].y.dx;
					InvalidateRect(hDlg, NULL, false);
				}
				if (!sliderPermitions[buf[0]].lockZ) {
					SendMessage(GetDlgItem(hDlg, IDC_SLIDER_Z1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)sliderPermitions[buf[0]].z.def);
					targetPoints[buf[0]]->z = sliderPermitions[buf[0]].z.def * sliderPermitions[buf[0]].z.dx;
					InvalidateRect(hDlg, NULL, false);
				}
				InvalidateRect(hDlg, NULL, false);
			}

			if (LOWORD(wParam) == IDC_BUTTONSHIFT) {
				graphCube->shiftTo(*targetPoints[L'M'], *targetPoints[L'N']);
				InvalidateRect(hDlg, NULL, false);
			}

			if (LOWORD(wParam) == IDC_BUTTONROTATE) {
				graphCube->rotateTo(*targetPoints[L'M'], *targetPoints[L'N']);
				InvalidateRect(hDlg, NULL, false);
			}


			if (LOWORD(wParam) == IDC_CHECKBOX_LABELS) {
				bool flag = SendMessage(GetDlgItem(hDlg, IDC_CHECKBOX_LABELS), BM_GETCHECK, 0, 0);
				g_debug = flag;
				graphCube->debug = flag;

				InvalidateRect(hDlg, NULL, false);
			}

			if (LOWORD(wParam) == IDC_CHECKBOX_FPS) {
				bool flag = SendMessage(GetDlgItem(hDlg, IDC_CHECKBOX_FPS), BM_GETCHECK, 0, 0);
				g_fps = flag;
				InvalidateRect(hDlg, NULL, false);
			}
			if (LOWORD(wParam) == IDC_CHECKBOX_PROJ) {
				bool flag = SendMessage(GetDlgItem(hDlg, IDC_CHECKBOX_FPS), BM_GETCHECK, 0, 0);
				g_proj = flag;
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

void paintFirstPlate(Graphics &graphics, PointF center) {
	for (auto ob : g_world) {
		ob->paint(graphics, center);
	}
};


void paintSecondPlate(Graphics &graphics, PointF center) {
	for (auto ob : g_world) {
		ob->paintPerspective(graphics, center, *targetPoints[L'C']);
	}
};
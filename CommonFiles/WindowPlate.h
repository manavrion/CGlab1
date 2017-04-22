#pragma once
#include "afx.h"
//--------------------------------------------------------------------------------------
// GDI+ plate
//--------------------------------------------------------------------------------------


class WindowPlate {
public:
	WindowPlate(std::function<void(Graphics&, PointF)> paint, Size bufferSize = Size(350, 350)) : paint(paint), bufferSize(bufferSize) {

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

		paint(*backBufferGraphics, PointF(bufferSize.Width / 2, bufferSize.Height / 2));

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

	std::function<void(Graphics&, PointF)> paint;

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
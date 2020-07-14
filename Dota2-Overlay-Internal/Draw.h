#pragma once
#include "pch.h"
#include "d3d9.h"


class Draw
{
public:
	void DrawInit(IDirect3DDevice9* pDevice);
	void DrawTest(LPCSTR text);

private:
	unsigned int screenWidth;
	unsigned int screenHeight;

	LPD3DXFONT					m_font = NULL;
	D3DVIEWPORT9 Viewport;
	IDirect3DDevice9* pDevice;


	unsigned int getPercent(unsigned int perc, unsigned int value);
	bool DrawMessage(LPD3DXFONT font, unsigned int x, unsigned int y, int alpha, unsigned char r, unsigned char g, unsigned char b, LPCSTR Message); //Draw message
};


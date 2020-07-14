#pragma once
#include "pch.h"
#include "Draw.h"



void Draw::DrawInit(IDirect3DDevice9* pDevice)
{

	this->pDevice = pDevice;
	D3DXCreateFont(this->pDevice, 55, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_font);
	pDevice->GetViewport(&Viewport);
	screenWidth = Viewport.Width;
	screenHeight = Viewport.Height;
}


void Draw::DrawTest(LPCSTR text)
{
	if (text == "Visible")
	{
		DrawMessage(m_font, getPercent(45, screenWidth), 30, 255, 255, 0, 0, text);
	}
	else if (text == "Not Visible")
	{
		DrawMessage(m_font, getPercent(43, screenWidth), 30, 255, 52, 241, 23, text);
	}
	else
	{
		DrawMessage(m_font, getPercent(43, screenWidth), 30, 255, 170, 235, 248, (LPCSTR)"Waiting..");
	}

}

bool Draw::DrawMessage(LPD3DXFONT font, unsigned int x, unsigned int y, int alpha, unsigned char r, unsigned char g, unsigned char b, LPCSTR Message)
{	// Create a colour for the text
	D3DCOLOR fontColor = D3DCOLOR_ARGB(alpha, r, g, b);
	RECT rct; //Font
	rct.left = x;
	rct.right = 1680;
	rct.top = y;
	rct.bottom = rct.top + 200;
	font->DrawTextA(NULL, Message, -1, &rct, 0, fontColor);
	return true;
}

unsigned int Draw::getPercent(unsigned int perc , unsigned int value) {

	return  (value * perc) / 100;
}
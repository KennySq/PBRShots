#pragma once
#include"stdafx.h"


class DXSample;
class Window
{
public:

	static int Run(DXSample* sample, HINSTANCE handleInst, int nCmdShow);
		
	static HWND GetHwnd() { return mHandle; }
private:

	static LRESULT __stdcall WndProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
	static void registerClass(DXSample* sample, HINSTANCE handleInst);


	static HWND mHandle;
	

};
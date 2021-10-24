#include"PBRSample_Metal.h"

int WinMain(HINSTANCE hInst, HINSTANCE prevInst, LPSTR message, int nCmdShow)
{
	PBRSample_Metal sample;

	return Window::Run(&sample, hInst, nCmdShow);
}
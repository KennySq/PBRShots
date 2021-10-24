#include"Window.h"
#include"DXSample.h"

HWND Window::mHandle = nullptr;
void Window::registerClass(DXSample* sample, HINSTANCE handleInst)
{
	WNDCLASS winClass{};


	winClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	winClass.hInstance = handleInst;
	winClass.lpfnWndProc = WndProc;
	winClass.style = CS_HREDRAW | CS_VREDRAW;
	winClass.lpszClassName = "PBR Samples";

	RegisterClass(&winClass);

	RECT winRect{};

	winRect.right = sample->GetWidth();
	winRect.bottom = sample->GetHeight();

	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, false);

	mHandle = CreateWindow(winClass.lpszClassName, "DXSample", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, winRect.right - winRect.left, winRect.bottom - winRect.top, nullptr, nullptr, handleInst, sample);

	if (mHandle == nullptr)
	{
		throw std::runtime_error("");
	}


}

int Window::Run(DXSample* sample, HINSTANCE handleInst, int nCmdShow)
{
	registerClass(sample, handleInst);

	sample->Awake();

	ShowWindow(mHandle, nCmdShow);


	MSG msg{};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	sample->Release();

	return static_cast<char>(msg.wParam);
}

LRESULT __stdcall Window::WndProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	DXSample* sample = reinterpret_cast<DXSample*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	
	switch (message)
	{
	case WM_CREATE:
	{
		LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		SetWindowLongPtrA(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));
	}
	return 0;

	case WM_PAINT:
	{
		if (sample != nullptr)
		{
			sample->Update(0.0f);
			sample->Render(0.0f);
		}
	}
	return 0;
	
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	return 0;
	
	}


	return DefWindowProc(hWnd, message, wParam, lParam);
}

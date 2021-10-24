#pragma once
#include"stdafx.h"
#include"Util.h"

#include<d3d11.h>
#include<dxgi.h>

#include<wrl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;

class DXSample
{
public:
	DXSample(uint width, uint height, const char* appName);

	uint GetWidth() const { return mWidth; }
	uint GetHeight() const { return mHeight; }

	virtual void Awake() = 0;
	virtual void Update(float delta) = 0;
	virtual void Render(float delta) = 0;
	virtual void Release() = 0;

protected:



	void AcquireHardware();

private:

	ComPtr<ID3D11Device> mDevice;
	ComPtr<IDXGISwapChain> mSwapchain;
	ComPtr<ID3D11DeviceContext> mContext;

	ComPtr<IDXGIFactory> mFactory;

	uint mWidth;
	uint mHeight;

	const char* mAppName;



};
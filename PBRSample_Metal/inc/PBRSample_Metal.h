#pragma once
#include"DXSample.h"

#pragma comment(lib, "DXSample.lib")

using namespace DirectX;

class PBRSample_Metal : public DXSample
{
	struct Constants
	{
		XMFLOAT4X4 World;
		XMFLOAT4X4 View;
		XMFLOAT4X4 Projection;

		XMFLOAT4 LightPosition;
		XMFLOAT4 LightColor;
	};

public:
	PBRSample_Metal();
	


	virtual void Awake() override;
	virtual void Update(float delta) override;
	virtual void Render(float delta) override;
	virtual void Release() override;

private:

	void loadAssets();

	ComPtr<ID3D11Buffer> mVertexBuffer;
	ComPtr<ID3D11Buffer> mIndexBuffer;
	uint mIndexCount;

	ComPtr<ID3D11InputLayout> mInputLayout;

	D3D11_VIEWPORT mViewport{};

	ComPtr<ID3D11RenderTargetView> mBackBufferRTV;
	ComPtr<ID3D11Texture2D> mBackBufferTexture;

	ComPtr<ID3D11DepthStencilView> mDepthStencilView;
	ComPtr<ID3D11Texture2D> mDepthTexture;

	ComPtr<ID3D11VertexShader> mVertexShader;
	ComPtr<ID3D11PixelShader> mPixelShader;

	ComPtr<ID3D11Buffer> mConstantBuffer;



	


};
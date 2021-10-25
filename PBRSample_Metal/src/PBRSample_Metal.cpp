#include"PBRSample_Metal.h"

#include<FbxLoader.h>

#pragma comment(lib, "FbxLoader.lib")

PBRSample_Metal::PBRSample_Metal()
	: DXSample(1280, 720, "PBR Metal (DX11)")
{
}

void PBRSample_Metal::Awake()
{
	AcquireHardware();
	D3D11_TEXTURE2D_DESC backBufferDesc{};
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};

	Throw(mSwapchain->GetBuffer(0, IID_PPV_ARGS(&mBackBufferTexture)));

	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	Throw(mDevice->CreateRenderTargetView(mBackBufferTexture.Get(), &rtvDesc, mBackBufferRTV.GetAddressOf()));

	D3D11_TEXTURE2D_DESC depthTexDesc{};
	depthTexDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthTexDesc.Width = mWidth;
	depthTexDesc.Height = mHeight;
	depthTexDesc.ArraySize = 1;
	depthTexDesc.MipLevels = 1;
	depthTexDesc.SampleDesc.Count = 1;
	depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};

	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	
	Throw(mDevice->CreateTexture2D(&depthTexDesc, nullptr, mDepthTexture.GetAddressOf()));
	Throw(mDevice->CreateDepthStencilView(mDepthTexture.Get(), &dsvDesc, mDepthStencilView.GetAddressOf()));


	loadAssets();
	mConstantData = reinterpret_cast<Constants*>(mCbufferMap.pData);


}

void PBRSample_Metal::Update(float delta)
{


	//XMVECTOR eye, at, up;

	//eye = XMVectorSet(0.5f, 0.0f, 1.5f, 1.0f);
	//at = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	//up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	////XMVECTOR quat = XMQuaternionRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), 1.0f);
	//XMVECTOR origin = XMLoadFloat4(&mConstantData->LightPosition);

	//XMStoreFloat4x4(&mConstantData->World, XMMatrixIdentity());
	//XMStoreFloat4x4(&mConstantData->View, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));
	//XMStoreFloat4x4(&mConstantData->Projection, XMMatrixTranspose(XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), 1.777f, 0.001f, 1000.0f)));
	//
	//quat = XMVectorSet(XMConvertToDegrees(quat.m128_f32[0]), XMConvertToDegrees(quat.m128_f32[1]), XMConvertToDegrees(quat.m128_f32[2]), XMConvertToDegrees(quat.m128_f32[3]));

	//origin *= quat;

	//XMStoreFloat4(&mConstantData->LightPosition, origin);

	//XMStoreFloat4(&mConstantData->LightColor, Colors::White);


}

void PBRSample_Metal::Render(float delta)
{
	static uint strides[] = { sizeof(Vertex) };
	static uint offsets[] = { 0 };

	mContext->ClearRenderTargetView(mBackBufferRTV.Get(), DirectX::Colors::Green);
	mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);


	mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), strides, offsets);
	mContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	mContext->IASetInputLayout(mInputLayout.Get());

	mContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());
	mContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	mContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
	mContext->PSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());

	mContext->OMSetRenderTargets(1, mBackBufferRTV.GetAddressOf(), mDepthStencilView.Get());


	mContext->RSSetViewports(1, &mViewport);

	mContext->DrawIndexed(mIndexCount, 0, 0);

	mSwapchain->Present(0, 0);
}

void PBRSample_Metal::Release()
{
	mContext->Unmap(mConstantBuffer.Get(), 0);

}

void PBRSample_Metal::loadAssets()
{
	mViewport.Width = mWidth;
	mViewport.Height = mHeight;
	mViewport.MaxDepth = 1.0f;
	std::string workPath = GetWorkingDirectoryA();

	std::string meshPath = workPath;
	meshPath += "..\\..\\PBRSample_Metal\\resources\\knight-final\\knight.fbx";

	FbxLoader loader = FbxLoader(meshPath.c_str());

	D3D11_BUFFER_DESC vbDesc{};

	vbDesc.ByteWidth = sizeof(Vertex) * loader.Vertices.size();
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subData{};

	subData.pSysMem = loader.Vertices.data();

	Throw(mDevice->CreateBuffer(&vbDesc, &subData, mVertexBuffer.GetAddressOf()));

	D3D11_BUFFER_DESC ibDesc{};

	ibDesc.ByteWidth = sizeof(unsigned int) * loader.Indices.size();
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;

	subData.pSysMem = loader.Indices.data();
	mIndexCount = loader.Indices.size();

	Throw(mDevice->CreateBuffer(&ibDesc, &subData, mIndexBuffer.GetAddressOf()));

	D3D11_INPUT_ELEMENT_DESC inputElements[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0xFFFFFFFF, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0xFFFFFFFF, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	ID3DBlob* vertexBlob, *pixelBlob, *errorBlob;

#ifdef _DEBUG
	DWORD compileFlag = D3DCOMPILE_DEBUG;
#else
	DWORD compileFlag = 0;

#endif


	std::wstring shaderPath = GetWorkingDirectoryW();
	shaderPath += L"..\\..\\PBRSample_Metal\\resources\\Metal.hlsl";

	Throw(D3DCompileFromFile(shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "vert", "vs_5_0", compileFlag, 0, &vertexBlob, &errorBlob));
	Throw(D3DCompileFromFile(shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "frag", "ps_5_0", compileFlag, 0, &pixelBlob, &errorBlob));
	
	Throw(mDevice->CreateInputLayout(inputElements, ARRAYSIZE(inputElements), vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), mInputLayout.GetAddressOf()));

	Throw(mDevice->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), nullptr, mVertexShader.GetAddressOf()));
	Throw(mDevice->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), nullptr, mPixelShader.GetAddressOf()));

	D3D11_BUFFER_DESC cbufferDesc{};

	cbufferDesc.ByteWidth = sizeof(Constants);
	cbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	Throw(mDevice->CreateBuffer(&cbufferDesc, nullptr, mConstantBuffer.GetAddressOf()));
	mContext->Map(mConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mCbufferMap);

	mConstantData = reinterpret_cast<Constants*>(mCbufferMap.pData);

	XMStoreFloat4x4(&mConstantData->World, XMMatrixIdentity());

	XMVECTOR eye, at, up;

	eye = XMVectorSet(0.5f, 0.0f, 1.5f, 1.0f);
	at = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);


	XMStoreFloat4(&mConstantData->LightPosition, XMVectorSet(250.0f, -500.0f, -500.0f, 1.0f));
	XMStoreFloat4(&mConstantData->LightColor, Colors::White);

	XMStoreFloat4x4(&mConstantData->View, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));
	XMStoreFloat4x4(&mConstantData->Projection, XMMatrixTranspose(XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), 1.777f, 0.001f, 1000.0f)));

}

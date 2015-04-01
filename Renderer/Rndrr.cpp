#include "Rndrr.hpp"
#include <d3dcompiler.h>
#include "DDSTextureLoader.h"
//#include <d3d11_1.h>

Rndrr::Rndrr()
{
}


Rndrr::~Rndrr()
{
}

auto Rndrr::setupViewport(long width, long height, ID3D11DeviceContext*& immediateContext) -> void
{
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(width);
	vp.Height = static_cast<FLOAT>(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	immediateContext->RSSetViewports(1, &vp);
}

auto Rndrr::createInputLayout(ID3DBlob*& pVSBlob, ID3D11Device*& pd3dDevice, ID3D11InputLayout*& pVertexLayout)->HRESULT
{
	auto hr = S_OK;
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	auto numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &pVertexLayout);
	return hr;
}

auto Rndrr::compileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)->HRESULT
{
	auto hr = S_OK;

	auto dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}

	if (pErrorBlob)
	{
		pErrorBlob->Release();
	}

	return S_OK;
}

auto Rndrr::createDepthStencilTextureAndView(long width, long height, ID3D11Device*& pd3dDevice, ID3D11Texture2D*& pDepthStencil, ID3D11DepthStencilView*& pDepthStencilView, ID3D11DeviceContext*& immediateContext, ID3D11RenderTargetView*& pRenderTargetView)->HRESULT
{
	auto hr = S_OK;
	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = pd3dDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);
	if (FAILED(hr))
	{
		return hr;
	}

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = pd3dDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &pDepthStencilView);
	if (FAILED(hr))
	{
		return hr;
	}

	immediateContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
	return hr;
}

auto Rndrr::initTexture(ID3D11Device*& pd3dDevice, ID3D11ShaderResourceView*& pTextureRV, ID3D11SamplerState*& pSamplerLinear)->HRESULT
{
	auto hr = S_OK;
	// Load the Texture
	hr = DirectX::CreateDDSTextureFromFile(pd3dDevice, L"seafloor.dds", nullptr, &pTextureRV);
	if (FAILED(hr))
	{
		return hr;
	}

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = pd3dDevice->CreateSamplerState(&sampDesc, &pSamplerLinear);

	return hr;
}

auto Rndrr::initShaders(ID3DBlob*& pVSBlob, ID3DBlob*& pPSBlob, ID3D11Device*& pd3dDevice, ID3D11VertexShader*& pVertexShader, ID3D11PixelShader*& pPixelShader)->HRESULT
{
	auto hr = S_OK;
	// Compile the vertex shader
	hr = compileShaderFromFile(L"Renderer.fx", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	// Compile the pixel shader
	hr = compileShaderFromFile(L"Renderer.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &pPixelShader);
	pPSBlob->Release();
	return hr;
}

auto Rndrr::initMatrices(long width, long height, DirectX::XMMATRIX& world, DirectX::XMMATRIX& view, DirectX::XMMATRIX& projection, ID3D11DeviceContext*& immediateContext, ID3D11Buffer*& pcbNeverChanges, ID3D11Buffer*& pcbChangeOnResize) -> void
{
	using namespace DirectX; // for matrix math

	// Initialize the world matrices
	world = XMMatrixIdentity();

	// Initialize the view matrix
	auto Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
	auto At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	auto Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	view = XMMatrixLookAtLH(Eye, At, Up);

	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = XMMatrixTranspose(view);
	immediateContext->UpdateSubresource(pcbNeverChanges, 0, nullptr, &cbNeverChanges, 0, 0);

	// Initialize the projection matrix
	projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / static_cast<FLOAT>(height), 0.01f, 100.0f);

	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = XMMatrixTranspose(projection);
	immediateContext->UpdateSubresource(pcbChangeOnResize, 0, nullptr, &cbChangesOnResize, 0, 0);
}
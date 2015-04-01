#pragma once
//#include <windows.h>
#include <d3d11_1.h>
#include <directxmath.h>
//#include <d3dcompiler.h>
//#include <directxmath.h>
//#include <directxcolors.h>
//#include "DDSTextureLoader.h"
//#include "resource.h"
class Rndrr
{
private:
	struct SimpleVertex
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT2 Tex;
	};

	struct CBNeverChanges
	{
		DirectX::XMMATRIX mView;
	};

	struct CBChangeOnResize
	{
		DirectX::XMMATRIX mProjection;
	};

	struct CBChangesEveryFrame
	{
		DirectX::XMMATRIX mWorld;
		DirectX::XMFLOAT4 vMeshColor;
	};
public:
	Rndrr();
	~Rndrr();
	auto setupViewport(long width, long height, ID3D11DeviceContext*& immediateContext) -> void;
	auto createInputLayout(ID3DBlob*& pVSBlob, ID3D11Device*& pd3dDevice, ID3D11InputLayout*& pVertexLayout)->HRESULT;
	auto compileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)->HRESULT;
	auto createDepthStencilTextureAndView(long width, long height, ID3D11Device*& pd3dDevice, ID3D11Texture2D*& pDepthStencil, ID3D11DepthStencilView*& pDepthStencilView, ID3D11DeviceContext*& immediateContext, ID3D11RenderTargetView*& pRenderTargetView)->HRESULT;
	auto initTexture(ID3D11Device*& pd3dDevice, ID3D11ShaderResourceView*& pTextureRV, ID3D11SamplerState*& pSamplerLinear)->HRESULT;
	auto initShaders(ID3DBlob*& pVSBlob, ID3DBlob*& pPSBlob, ID3D11Device*& pd3dDevice, ID3D11VertexShader*& pVertexShader, ID3D11PixelShader*& pPixelShader)->HRESULT;
	auto initMatrices(long width, long height, DirectX::XMMATRIX& world, DirectX::XMMATRIX& view, DirectX::XMMATRIX& projection, ID3D11DeviceContext*& immediateContext, ID3D11Buffer*& pcbNeverChanges, ID3D11Buffer*& pcbChangeOnResize) -> void;
};


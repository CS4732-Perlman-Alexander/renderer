#pragma once
#include <d3d11_1.h>
#include "RndrrStructures.hpp"
class Rndrr
{
private:
	
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
	auto initBuffers(SimpleVertex vertices[], unsigned int numVertices, WORD indices[], unsigned int numIndices, ID3D11Device*& pd3dDevice, ID3D11Buffer*& pVertexBuffer, ID3D11Buffer*& pIndexBuffer, ID3D11DeviceContext*& immediateContext, ID3D11Buffer*& pcbNeverChanges, ID3D11Buffer*& pcbChangeOnResize, ID3D11Buffer*& pcbChangesEveryFrame)->HRESULT;
	auto Rndrr::initDevice(long width, long height, ID3D11Device*& pd3dDevice, ID3D11Device1*& pd3dDevice1, D3D_FEATURE_LEVEL& featurelevel, ID3D11DeviceContext*& immediateContext, HWND& hWnd, IDXGISwapChain*& pSwapchain, IDXGISwapChain1*& pSwapchain1, ID3D11RenderTargetView*& pRenderTargetView)->HRESULT;
	template < typename Func >
	auto render(Func func) -> void
	{
		func();

		//
		// Present our back buffer to our front buffer
		//
		g_pSwapChain->Present(0, 0);
	}
};


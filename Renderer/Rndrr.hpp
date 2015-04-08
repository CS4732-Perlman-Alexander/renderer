#pragma once
#include <d3d11_1.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "RndrrStructures.hpp"
class Rndrr
{
private:
	HINSTANCE					g_hInst;
	HWND						g_hWnd;
	D3D_DRIVER_TYPE				g_driverType;
	ID3D11Device*				g_pd3dDevice;
	ID3D11Device1*				g_pd3dDevice1;
	ID3D11DeviceContext*		g_pImmediateContext;
	ID3D11DeviceContext1*		g_pImmediateContext1;
	IDXGISwapChain*				g_pSwapChain;
	IDXGISwapChain1*			g_pSwapChain1;
	ID3D11RenderTargetView*		g_pRenderTargetView;
	ID3D11Texture2D*			g_pDepthStencil;
	ID3D11DepthStencilView*		g_pDepthStencilView;
	ID3D11VertexShader*			g_pVertexShader;
	ID3D11PixelShader*			g_pPixelShader;
	ID3D11Buffer*				g_pVertexBuffer;
	ID3D11Buffer*				g_pIndexBuffer;
	ID3D11InputLayout*			g_pVertexLayout;
	ID3D11Buffer*				g_pCBNeverChanges;
	ID3D11Buffer*				g_pCBChangeOnResize;
	ID3D11Buffer*				g_pCBChangesEveryFrame;
	ID3D11ShaderResourceView*	g_pTextureRV;
	ID3D11SamplerState*			g_pSamplerLinear;
	DirectX::XMMATRIX			g_World;
	DirectX::XMMATRIX			g_View;
	DirectX::XMMATRIX			g_Projection;
	DirectX::XMFLOAT4			g_vMeshColor;
	D3D_FEATURE_LEVEL			g_featureLevel;
public:
	Rndrr();
	~Rndrr();

	auto setupViewport(long width, long height) -> void;

	auto createInputLayout(ID3DBlob*& pVSBlob)->HRESULT;

	auto InitWindow(WNDPROC WndProc, int nCmdShow)->HRESULT;

	auto compileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)->HRESULT;

	auto createDepthStencilTextureAndView(long width, long height)->HRESULT;

	auto initTexture()->HRESULT;

	auto initShaders(ID3DBlob*& pVSBlob, ID3DBlob*& pPSBlob)->HRESULT;

	auto initMatrices(long width, long height) -> void;

	auto initBuffers(SimpleVertex vertices[], unsigned int numVertices, WORD indices[], unsigned int numIndices)->HRESULT;

	auto Rndrr::CleanupDevice() -> void;

	auto Rndrr::initDevice(long width, long height)->HRESULT;

	auto initialize()->HRESULT;

	template < typename Func >
	auto render(Func func) -> void
	{
		func();

		// Clear the back buffer
		g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, DirectX::Colors::Black);

		// Clear the depth buffer to 1.0 (max depth)
		g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		// Update variables that change once per frame
		CBChangesEveryFrame cb;
		cb.mWorld = XMMatrixTranspose(g_World);
		cb.vMeshColor = g_vMeshColor;
		g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, nullptr, &cb, 0, 0);

		// Render the cube
		g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
		g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pCBNeverChanges);
		g_pImmediateContext->VSSetConstantBuffers(1, 1, &g_pCBChangeOnResize);
		g_pImmediateContext->VSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
		g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
		g_pImmediateContext->PSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
		g_pImmediateContext->PSSetShaderResources(0, 1, &g_pTextureRV);
		g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);
		g_pImmediateContext->DrawIndexed(36, 0, 0);

		// Present our back buffer to our front buffer
		g_pSwapChain->Present(0, 0);
	}
};


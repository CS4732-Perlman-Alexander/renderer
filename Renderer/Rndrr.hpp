#pragma once
#include <memory>
#include "resource.h"
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "DDSTextureLoader.h"
#include "RndrrStructures.hpp"
#include "Node.hpp"
#include "nodeMesh.hpp"

class Rndrr
{
private:
	std::unique_ptr<HINSTANCE>	g_hInst;
	std::unique_ptr<HWND>		g_hWnd;
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

	std::shared_ptr<Node> 		scenegraph;

	unsigned int				width;
	unsigned int				height;

public:
	// and a traverse/bufferstuff/render function(s) here
	Rndrr();
	~Rndrr() = default;

	//setscenepgraph
	auto setGraphRoot(std::shared_ptr<Node> n) -> void;

	auto setupViewport() -> void;
	auto createInputLayout(ID3DBlob*& pVSBlob)->HRESULT;
	auto InitWindow(WNDPROC WndProc, int nCmdShow)->HRESULT;
	auto compileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)->HRESULT;
	auto createDepthStencilTextureAndView()->HRESULT;

	auto initTexture()->HRESULT;

	auto initShaders(ID3DBlob*& pVSBlob, ID3DBlob*& pPSBlob)->HRESULT;

	auto initMatrices() -> void;
	auto initBuffers()->HRESULT;
	auto CleanupDevice() -> void;
	auto initDevice()->HRESULT;
	auto initialize()->HRESULT;
	auto prerenderSetup()->HRESULT;

	//Immediate Context: Getters and Setters
	auto getImmediateContext()->ID3D11DeviceContext*;
	auto setImmediateContext(ID3D11DeviceContext* iContext)->void;
	//World: Getters and Setters
	auto getWorld()->DirectX::XMMATRIX;
	auto setWorld(const DirectX::XMMATRIX& wMatrix)->void;
	//View: Getters and Setters
	auto getView()->DirectX::XMMATRIX;
	auto setView(const DirectX::XMMATRIX& vMatrix)->void;
	//Projection: Getters and Setters
	auto getProjection()->DirectX::XMMATRIX;
	auto setProjection(const DirectX::XMMATRIX& pMatrix)->void;
	//Driver Type: Getters and Setters
	auto getDriverType()->D3D_DRIVER_TYPE;
	auto setDriverType(D3D_DRIVER_TYPE dType)->void;
	//Mesh Color: Getters and Setters
	auto getMeshColor()->DirectX::XMFLOAT4;
	auto setMeshColor(DirectX::XMFLOAT4 meshColor)->void;

	auto drawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation) -> void 
	{ 
		g_pImmediateContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation); 
	};

	auto updateShaders() -> void;
	auto updateConstantBuffers() -> void;

	template < typename Func >
	auto render(Func func) -> void
	{
		// Clear the back buffer
		g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, DirectX::Colors::Black);

		// Clear the depth buffer to 1.0 (max depth)
		g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		func();

		// Update variables that change once per frame
		//updateConstantBuffers();
		//updateShaders();

		// First 36 vertices
		//drawIndexed(36, 0, 0);
		// Next 36 vertices
		//drawIndexed(36, 36, 0);

		// Present our back buffer to our front buffer
		g_pSwapChain->Present(0, 0);
	}
};


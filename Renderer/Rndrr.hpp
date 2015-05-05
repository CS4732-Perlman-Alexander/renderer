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
#include "nodeTransform.hpp"
#include "nodeLight.hpp"

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

	SimpleVertex*				mainVerticesArray;
	unsigned int				mainNumVertices;
	WORD*						mainIndicesArray;
	unsigned int				mainNumIndices;
	const wchar_t*				textureFileName;

	unsigned int				width;
	unsigned int				height;

public:
	Rndrr();
	~Rndrr() = default;

	auto setGraphRoot(std::shared_ptr<Node> n) -> void;
	auto setMainArrays(SimpleVertex* vertices, unsigned int numVertices, WORD* indices, unsigned int numIndices, const wchar_t* texture) -> void;

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

	auto setTransformation(const DirectX::XMMATRIX& wMatrix)->void;

	//Mesh Color: Getters and Setters
	auto getMeshColor()->DirectX::XMFLOAT4;
	auto setMeshColor(DirectX::XMFLOAT4 meshColor)->void;

	auto visitTree(float timeTick)->void;

	auto updateShaders() -> void;
	auto updateConstantBuffers() -> void;

	auto render(float timeTick) -> void;
};

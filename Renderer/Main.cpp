//-----------------------------------------------------------------------------------
// File: Renderer.cpp
//-----------------------------------------------------------------------------------
#include "Rndrr.hpp"
#include <windows.h>
#include <vector>
#include <string>
//-----------------------------------------------------------------------------------
// Structures
//-----------------------------------------------------------------------------------
enum struct lightSourceType	{UNASSIGNED = 0, POINT = 1, DIRECTIONAL = 2, SPOTLIGHT = 3, AREA = 4};
enum struct materialType	{UNASSIGNED = 0, OBJ = 1};
struct meshMaterial
{
	materialType			type			= materialType::UNASSIGNED;
	std::string*			objMaterialName	= nullptr;
	std::string*			objTextureName	= nullptr;
	DirectX::XMVECTOR*		objAmbient		= nullptr;
	DirectX::XMVECTOR*		objDiffuse		= nullptr;
	DirectX::XMVECTOR*		objSpecular		= nullptr;
	int						objIllumination;
	float					objShininess;
	float					objTransparency;
};
enum struct nodeType		{UNASSIGNED = 0, TRANSFORMATION = 1, MESH = 2, LIGHT = 3};
struct nodeMeshData
{
	SimpleVertex*			vertices	= nullptr;			//array
	WORD*					indices		= nullptr;			//array
};
struct node
{
	nodeType				type = nodeType::UNASSIGNED;
	std::vector<node*>		parents;
	std::vector<node*>		children;
	DirectX::XMMATRIX*		transform	= nullptr;
	nodeMeshData*			meshData	= nullptr;
};
//-----------------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------------
std::unique_ptr<Rndrr> renderer;
std::unique_ptr<node> scenegraph;
using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;
SimpleVertex vertices[] =
{
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },

	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },

	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },

	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
};
WORD indices[] =
{
	3, 1, 0,
	2, 1, 3,

	6, 4, 5,
	7, 4, 6,

	11, 9, 8,
	10, 9, 11,

	14, 12, 13,
	15, 12, 14,

	19, 17, 16,
	18, 17, 19,

	22, 20, 21,
	23, 20, 22
};
//-----------------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------------
auto CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM) -> LRESULT;
auto renderFunction() -> void;
//-----------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//-----------------------------------------------------------------------------------
auto WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) -> int
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	renderer = std::make_unique<Rndrr>(vertices, ARRAYSIZE(vertices), indices, ARRAYSIZE(indices));

	if(FAILED(renderer->InitWindow(WndProc, nCmdShow)))
	{
		return 0;
	}

	//renderer->setGeometry(vertices, ARRAYSIZE(vertices), indices, ARRAYSIZE(indices));

	if(FAILED(renderer->initialize()))
	{
		renderer->CleanupDevice();
		return 0;
	}
	// Main message loop
	MSG msg = {0};
	while(WM_QUIT != msg.message)
	{
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			renderer->render(renderFunction);
		}
	}
	renderer->CleanupDevice();
	return static_cast<int>(msg.wParam);
}

//-----------------------------------------------------------------------------------
// Called every time the application receives a message
//-----------------------------------------------------------------------------------
auto CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch( message )
	{
		case WM_PAINT:
			hdc = BeginPaint( hWnd, &ps );
			EndPaint( hWnd, &ps );
		break;
		case WM_DESTROY:
			PostQuitMessage( 0 );
		break;
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}

//-----------------------------------------------------------------------------------
// Render a frame
//-----------------------------------------------------------------------------------
auto renderFunction() -> void
{
	// Update our time
	static auto t = 0.0f;
	if (renderer->getDriverType() == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += static_cast<float>(DirectX::XM_PI * 0.0125f);
	}
	else
	{
		static ULONGLONG timeStart = 0;
		auto timeCur = GetTickCount64();
		if( timeStart == 0 )
			timeStart = timeCur;
		t = ( timeCur - timeStart ) / 1000.0f;
	}

	// Rotate cube around the origin
	renderer->setWorld(
		DirectX::XMMatrixRotationX(t) + 
		DirectX::XMMatrixRotationY(t) + 
		DirectX::XMMatrixRotationZ(t)
		);

	// Modify the color
	renderer->setMeshColor(DirectX::XMFLOAT4(
		((sinf(t * 1.0f) + 1.0f) * 0.5f),
		((cosf(t * 3.0f) + 1.0f) * 0.5f),
		((sinf(t * 5.0f) + 1.0f) * 0.5f),
		1.0f));
}
//-----------------------------------------------------------------------------------
// File: Renderer.cpp
//-----------------------------------------------------------------------------------
#include "Rndrr.hpp"
#include "Node.hpp"
#include "nodeLight.hpp"
#include "nodeMesh.hpp"
#include "nodeTransform.hpp"
#include <windows.h>
#include <vector>
#include <string>
//-----------------------------------------------------------------------------------
// Structures
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------------
std::unique_ptr<Rndrr> renderer;
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

SimpleVertex vertices2[] =
{
	{ XMFLOAT3(-1.0f, 2.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 2.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 2.0f, 3.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, 2.0f, 3.0f), XMFLOAT2(1.0f, 1.0f) },

	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, 3.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, 3.0f), XMFLOAT2(0.0f, 1.0f) },

	{ XMFLOAT3(-1.0f, -1.0f, 3.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, 2.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, 2.0f, 3.0f), XMFLOAT2(0.0f, 0.0f) },

	{ XMFLOAT3(1.0f, -1.0f, 3.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(1.0f, 2.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 2.0f, 3.0f), XMFLOAT2(1.0f, 0.0f) },

	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(1.0f, 2.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, 2.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

	{ XMFLOAT3(-1.0f, -1.0f, 3.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(1.0f, -1.0f, 3.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(1.0f, 2.0f, 3.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, 2.0f, 3.0f), XMFLOAT2(1.0f, 0.0f) },
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
	23, 20, 22,

	27, 25, 24,
	26, 25, 27,

	30, 28, 29,
	31, 28, 30,

	35, 33, 32,
	34, 33, 35,

	38, 36, 37,
	39, 36, 38,

	43, 41, 40,
	42, 41, 43,

	46, 44, 45,
	47, 44, 46
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
	
	auto hr = S_OK;

	// Initialize the unique pointer for the renderer.
	renderer = std::make_unique<Rndrr>();
	
	// Initialize window.
	hr = renderer->InitWindow(WndProc, nCmdShow);
	if(FAILED(hr))
	{
		return hr;
	}

	// Other DirectX initialization.
	hr = renderer->initialize();
	if(FAILED(hr))
	{
		renderer->CleanupDevice();
		return hr;
	}

	// Initialize a unique pointer for a node mesh.
	auto mNode0 = std::make_shared<nodeMesh>(vertices, ARRAYSIZE(vertices), indices, ARRAYSIZE(indices), L"seafloor.dds");
	auto mNode1 = std::make_shared<nodeMesh>(vertices2, ARRAYSIZE(vertices2), indices, ARRAYSIZE(indices), L"seafloor.dds");

	mNode0.get()->addChild(mNode1, mNode0);
	
	// Set the scenegraph and corresponding buffers.
	renderer->setGraphRoot(std::move(mNode0));

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
	
	// Clean up renderer device.
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
	/*
	renderer->setWorld(
		DirectX::XMMatrixRotationX(t) + 
		DirectX::XMMatrixRotationY(t) + 
		DirectX::XMMatrixRotationZ(t)
		);
	*/
	// Modify the color
	renderer->setMeshColor(DirectX::XMFLOAT4(
		((sinf(t * 1.0f) + 1.0f) * 0.5f),
		((cosf(t * 3.0f) + 1.0f) * 0.5f),
		((sinf(t * 5.0f) + 1.0f) * 0.5f),
		1.0f));
}
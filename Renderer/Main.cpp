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

const auto numCubes = 3;
const auto numVertices = numCubes * 24;
const auto numIndices = numCubes * 36;
SimpleVertex vertices[numVertices];
WORD indices[numIndices];

//-----------------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------------
auto CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM) -> LRESULT;
auto renderFunction() -> void;
auto generateCubeGeometry() -> void;
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

	//time tick
	static auto t = 0.0f;

	generateCubeGeometry();
	//prepare tree elements
	renderer->setMainArrays(vertices, numVertices, indices, numIndices, L"seafloor.dds");
	auto mNode1 = std::make_shared<nodeMesh>(0, 36, L"seafloor.dds");

	auto piyo = [](float t) -> DirectX::XMMATRIX
	{
		return DirectX::XMMatrixRotationZ(t) * DirectX::XMMatrixTranslation(3.0f, 0.f, 0.f);
	};
	auto tNode1 = std::make_shared<nodeTransform>(piyo);

	auto mNode2 = std::make_shared<nodeMesh>(36, 36, L"seafloor.dds");

	auto hoge = [](float t) -> DirectX::XMMATRIX
	{
		return DirectX::XMMatrixTranslation(-3.0f, 0.f, 0.f) * DirectX::XMMatrixRotationY(t);
	};
	auto tNode2 = std::make_shared<nodeTransform>(hoge);

	auto mNode3 = std::make_shared<nodeMesh>(72, 36, L"seafloor.dds");

	//constuct tree backwards (just to be safe)
	tNode2.get()->addChild(mNode3, tNode2);
	mNode2.get()->addChild(tNode2, mNode2);
	tNode1.get()->addChild(mNode2, tNode1);
	mNode1.get()->addChild(tNode1, mNode1);
	renderer->setGraphRoot(mNode1);

	// Main message loop
	MSG msg = {0};
	while(WM_QUIT != msg.message)
	{
		if (renderer->getDriverType() == D3D_DRIVER_TYPE_REFERENCE)
		{
			t += static_cast<float>(DirectX::XM_PI * 0.0125f);
		}
		else
		{
			static ULONGLONG timeStart = 0;
			auto timeCur = GetTickCount64();
			if (timeStart == 0)
				timeStart = timeCur;
			t = (timeCur - timeStart) / 1000.0f;
		}

		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			renderer->render(t);
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
// Generate the cube vertices/indices for n cubes.
//-----------------------------------------------------------------------------------
auto generateCubeGeometry() -> void
{
	// Generate the vertices.
	std::vector<SimpleVertex> tempVertexVector;
	for (auto i = 0; i < numCubes; ++i)
	{
		tempVertexVector.push_back({XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f)});
		tempVertexVector.push_back({XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)});
		tempVertexVector.push_back({XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)});
		tempVertexVector.push_back({XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)});

		tempVertexVector.push_back({XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)});
		tempVertexVector.push_back({XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f)});
		tempVertexVector.push_back({XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)});
		tempVertexVector.push_back({XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)});

		tempVertexVector.push_back({XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)});
		tempVertexVector.push_back({XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f)});
		tempVertexVector.push_back({XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f)});
		tempVertexVector.push_back({XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)});

		tempVertexVector.push_back({XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)});
		tempVertexVector.push_back({XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f)});
		tempVertexVector.push_back({XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)});
		tempVertexVector.push_back({XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)});

		tempVertexVector.push_back({XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f)});
		tempVertexVector.push_back({XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f)});
		tempVertexVector.push_back({XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f)});
		tempVertexVector.push_back({XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)});

		tempVertexVector.push_back({XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)});
		tempVertexVector.push_back({XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)});
		tempVertexVector.push_back({XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)});
		tempVertexVector.push_back({XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)});
	}
	// Copy the vertices.
	for (size_t i = 0; i < tempVertexVector.size(); ++i)
	{
		vertices[i] = tempVertexVector.at(i);
	}

	std::vector<WORD> tempIndexVector;
	auto currentIndex = 0;
	// For each cube
	for (auto i = 0; i < numCubes; ++i)
	{
		// For every 2 faces.
		for (auto j = 0; j < 3; ++j)
		{
			tempIndexVector.emplace_back(currentIndex + 3);
			tempIndexVector.emplace_back(currentIndex + 1);
			tempIndexVector.emplace_back(currentIndex);
			tempIndexVector.emplace_back(currentIndex + 2);
			tempIndexVector.emplace_back(currentIndex + 1);
			tempIndexVector.emplace_back(currentIndex + 3);

			currentIndex += 3;

			tempIndexVector.emplace_back(currentIndex + 3);
			tempIndexVector.emplace_back(currentIndex + 1);
			tempIndexVector.emplace_back(currentIndex + 2);
			tempIndexVector.emplace_back(currentIndex + 4);
			tempIndexVector.emplace_back(currentIndex + 1);
			tempIndexVector.emplace_back(currentIndex + 3);

			currentIndex += 5;
		}
	}

	// Copy the indices.
	for (size_t i = 0; i < tempIndexVector.size(); ++i)
	{
		indices[i] = tempIndexVector.at(i);
	}
};
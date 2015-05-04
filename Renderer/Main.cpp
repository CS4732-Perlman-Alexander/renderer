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

	generateCubeGeometry();
	renderer->setMainArrays(vertices, numVertices, indices, numIndices, L"seafloor.dds");

	{
		// The colors.
		auto red = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		auto green = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		auto blue = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		auto purple = DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);

		// First transform and mesh.
		auto tFunc1 = [](float t) -> DirectX::XMMATRIX
		{
			return DirectX::XMMatrixRotationY(t);
		};
		auto tNode1 = std::make_shared<nodeTransform>(tFunc1, "tNode1");
		auto mNode1 = std::make_shared<nodeMesh>(0, 36, red, "mNode1");

		// Second transform and mesh.
		auto tFunc2 = [](float t) -> DirectX::XMMATRIX
		{
			return DirectX::XMMatrixRotationY(t) * DirectX::XMMatrixTranslation(3.0f, 0.f, 0.f);
		};
		auto tNode2 = std::make_shared<nodeTransform>(tFunc2, "tNode2");
		auto mNode2 = std::make_shared<nodeMesh>(36, 36, green, "mNode2");

		
		auto tFunc4 = [](float t) -> DirectX::XMMATRIX
		{
			return DirectX::XMMatrixRotationY(t) * DirectX::XMMatrixTranslation(-3.0f, 0.f, 0.f);
		};
		auto tNode4 = std::make_shared<nodeTransform>(tFunc4, "tNode4");
		auto mNode4 = std::make_shared<nodeMesh>(36, 36, purple, "mNode4");
		

		// Third transform and mesh.
		auto tFunc3 = [](float t) -> DirectX::XMMATRIX
		{
			return DirectX::XMMatrixTranslation(0.0f, 3.f, 0.f) * DirectX::XMMatrixRotationZ(t);
		};
		auto tNode3 = std::make_shared<nodeTransform>(tFunc3, "tNode3");
		auto mNode3 = std::make_shared<nodeMesh>(72, 36, blue, "mNode3");

		// Construct the tree.
		renderer->setGraphRoot(tNode1);

		tNode1.get()->addChild(mNode1, tNode1);

		mNode1.get()->addChild(tNode2, mNode1);
		mNode1.get()->addChild(tNode4, mNode1);

		tNode2.get()->addChild(mNode2, tNode2);
		mNode2.get()->addChild(tNode3, mNode2);

		tNode4.get()->addChild(mNode4, tNode4);

		tNode3.get()->addChild(mNode3, tNode3);
		
	}

	//time tick
	static auto t = 0.0f;

	// Main message loop
	MSG msg = {0};
	while(WM_QUIT != msg.message)
	{
		static ULONGLONG timeStart = 0;
		auto timeCur = GetTickCount64();
		if (timeStart == 0)
		{
			timeStart = timeCur;
		}
		t = (timeCur - timeStart) / 1000.0f;

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
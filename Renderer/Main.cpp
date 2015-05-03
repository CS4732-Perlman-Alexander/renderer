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

	// Cube 1 rotates, color is green.
	renderer->setWorld(DirectX::XMMatrixRotationZ(2*t));
	renderer->setMeshColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	// Update cube 1 stuff.
	renderer->updateConstantBuffers();
	renderer->updateShaders();
	// Draw cube 1.
	renderer->drawIndexed(numIndices/numCubes, 0, 0);

	// Cube 2 rotates around cube 1, color is red.
	renderer->setWorld(DirectX::XMMatrixTranslation(5.0f, 0.0f, 0.0f)*DirectX::XMMatrixRotationY(4 * t));
	renderer->setMeshColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	// Update cube 2 stuff.
	renderer->updateConstantBuffers();
	// Draw cube 2.
	renderer->drawIndexed(numIndices / numCubes, numIndices / numCubes, 0);

	// Cube 3 scales, color is blue.
	renderer->setWorld(DirectX::XMMatrixScaling(2 * abs(sin(2 * t)), 2 * abs(sin(2 * t)), 2 * abs(sin(2 * t)))*DirectX::XMMatrixTranslation(5.0f, 0.0f, 5.0f));
	renderer->setMeshColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
	// Update cube 3 stuff.
	renderer->updateConstantBuffers();
	// Draw cube 3.
	renderer->drawIndexed(numIndices / numCubes, 2*numIndices / numCubes, 0);
}

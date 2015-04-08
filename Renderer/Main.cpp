//--------------------------------------------------------------------------------------
// File: Renderer.cpp
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//--------------------------------------------------------------------------------------
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <memory>
#include "DDSTextureLoader.h"
#include "resource.h"
#include "Rndrr.hpp"
#include "RndrrStructures.hpp"
//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
std::unique_ptr<Rndrr> renderer;

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
auto CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM ) -> LRESULT;
auto renderFunction() -> void;

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
auto WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow ) -> int
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	renderer = std::make_unique<Rndrr>();

	if (FAILED(renderer->InitWindow(WndProc, nCmdShow)))
	{
		return 0;
	}

	if( FAILED( renderer->initialize() ) )
	{
		renderer->CleanupDevice();
		return 0;
	}

	// Main message loop
	MSG msg = {0};
	while( WM_QUIT != msg.message )
	{
		if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			renderer->render(renderFunction);
		}
	}

	renderer->CleanupDevice();

	return static_cast<int>(msg.wParam);
}

//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
auto CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) -> LRESULT
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

		// Note that this tutorial does not handle resizing (WM_SIZE) requests,
		// so we created the window without the resize border.

		default:
		return DefWindowProc( hWnd, message, wParam, lParam );
	}

	return 0;
}

//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
auto renderFunction() -> void
{
	// Update our time
	static auto t = 0.0f;
	/*if( g_driverType == D3D_DRIVER_TYPE_REFERENCE )
	{
		t += static_cast<float>(XM_PI * 0.0125f);
	}
	else
	{
		static ULONGLONG timeStart = 0;
		auto timeCur = GetTickCount64();
		if( timeStart == 0 )
			timeStart = timeCur;
		t = ( timeCur - timeStart ) / 1000.0f;
	}

	*/
	// Rotate cube around the origin
	//g_World = XMMatrixRotationY( t );

	// Modify the color
	/*
	g_vMeshColor.x = ( sinf( t * 1.0f ) + 1.0f ) * 0.5f;
	g_vMeshColor.y = ( cosf( t * 3.0f ) + 1.0f ) * 0.5f;
	g_vMeshColor.z = ( sinf( t * 5.0f ) + 1.0f ) * 0.5f;
	*/
	
}

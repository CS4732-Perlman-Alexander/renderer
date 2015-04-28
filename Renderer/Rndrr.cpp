#include "Rndrr.hpp"

Rndrr::Rndrr()
{
	g_hInst = std::make_unique<HINSTANCE>();
	g_hWnd = std::make_unique<HWND>();
	g_driverType = D3D_DRIVER_TYPE_NULL;
	g_pd3dDevice = nullptr;
	g_pd3dDevice1 = nullptr;
	g_pImmediateContext = nullptr;
	g_pImmediateContext1 = nullptr;
	g_pSwapChain = nullptr;
	g_pSwapChain1 = nullptr;
	g_pRenderTargetView = nullptr;
	g_pDepthStencil = nullptr;
	g_pDepthStencilView = nullptr;
	g_pVertexShader = nullptr;
	g_pPixelShader = nullptr;
	g_pVertexBuffer = nullptr;
	g_pIndexBuffer = nullptr;
	g_pVertexLayout = nullptr;
	g_pCBNeverChanges = nullptr;
	g_pCBChangeOnResize = nullptr;
	g_pCBChangesEveryFrame = nullptr;
	g_pTextureRV = nullptr;
	g_vMeshColor = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	g_pSamplerLinear = nullptr;
	g_featureLevel = D3D_FEATURE_LEVEL_11_0;
}

auto Rndrr::setGraphRoot(std::shared_ptr<Node> n) -> void
{
	scenegraph = std::move(n);
	prerenderSetup();
};

auto Rndrr::setupViewport() -> void
{
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(width);
	vp.Height = static_cast<FLOAT>(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);
}

auto Rndrr::InitWindow(WNDPROC WndProc, int nCmdShow) -> HRESULT
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = *g_hInst.get();
	wcex.hIcon = LoadIcon(*g_hInst.get(), reinterpret_cast<LPCTSTR>(IDI_TUTORIAL1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"RenderingWindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, reinterpret_cast<LPCTSTR>(IDI_TUTORIAL1));
	if (!RegisterClassEx(&wcex))
	{
		return E_FAIL;
	}

	// Create window
	RECT rc = {0, 0, 800, 450};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	*g_hWnd = CreateWindow(L"RenderingWindowClass",
		L"rndrr",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left, rc.bottom - rc.top,
		nullptr,
		nullptr,
		*g_hInst.get(),
		nullptr);
	if (!g_hWnd)
	{
		return E_FAIL;
	}

	ShowWindow(*g_hWnd.get(), nCmdShow);

	return S_OK;
}

auto Rndrr::createInputLayout(ID3DBlob*& pVSBlob)->HRESULT
{
	auto hr = S_OK;
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	auto numElements = ARRAYSIZE(layout);
	// Create the input layout
	hr = g_pd3dDevice->CreateInputLayout( layout,numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pVertexLayout);
	return hr;
}

auto Rndrr::compileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)->HRESULT
{
	auto hr = S_OK;

	auto dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}

	if (pErrorBlob)
	{
		pErrorBlob->Release();
	}

	return S_OK;
}

auto Rndrr::createDepthStencilTextureAndView()->HRESULT
{
	auto hr = S_OK;

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	hr = g_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &g_pDepthStencil);
	if (FAILED(hr))
	{
		return hr;
	}

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
	if (FAILED(hr))
	{
		return hr;
	}

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
	return hr;
}

auto Rndrr::initTexture()->HRESULT
{
	auto hr = S_OK;
	// Load the Texture
	hr = DirectX::CreateDDSTextureFromFile(g_pd3dDevice, L"seafloor.dds", nullptr, &g_pTextureRV);
	if (FAILED(hr))
	{
		return hr;
	}

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);

	return hr;
}

auto Rndrr::initShaders(ID3DBlob*& pVSBlob, ID3DBlob*& pPSBlob)->HRESULT
{
	auto hr = S_OK;

	// Compile the vertex shader
	hr = compileShaderFromFile(L"Renderer.fx", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	// Compile the pixel shader
	hr = compileShaderFromFile(L"Renderer.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);
	pPSBlob->Release();
	return hr;
}

auto Rndrr::initMatrices() -> void
{
	using namespace DirectX;

	// Initialize the world matrices
	g_World = XMMatrixIdentity();

	// Initialize the view matrix
	auto Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
	auto At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	auto Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);

	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = XMMatrixTranspose(g_View);
	g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges, 0, nullptr, &cbNeverChanges, 0, 0);

	// Initialize the projection matrix
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / static_cast<FLOAT>(height), 0.01f, 100.0f);

	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = XMMatrixTranspose(g_Projection);
	g_pImmediateContext->UpdateSubresource(g_pCBChangeOnResize, 0, nullptr, &cbChangesOnResize, 0, 0);
}

auto Rndrr::initBuffers()->HRESULT
{
	auto hr = S_OK;
	auto nodemesh0 = static_cast<nodeMesh*>(scenegraph.get());
	//auto nodemesh1 = static_cast<nodeMesh*>(nodemesh0->getChildren().at(0).get());

	//std::vector<nodeMesh*> nodemeshes{ nodemesh0, nodemesh1 };
	
	// Create vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * nodemesh0->getNumVertices();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	//InitData.pSysMem = vertices;
	InitData.pSysMem = nodemesh0->getVertices();
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	/*
	D3D11_BUFFER_DESC bd2;
	ZeroMemory(&bd2, sizeof(bd2));
	bd2.Usage = D3D11_USAGE_DEFAULT;
	bd2.ByteWidth = sizeof(SimpleVertex) * nodemesh1->getNumVertices();
	bd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd2.CPUAccessFlags = 0;
	ID3D11Buffer* g_pVertexBuffer2 = nullptr;
	D3D11_SUBRESOURCE_DATA InitData2;
	ZeroMemory(&InitData2, sizeof(InitData2));
	InitData2.pSysMem = nodemesh1->getVertices();
	hr = g_pd3dDevice->CreateBuffer(&bd2, &InitData2, &g_pVertexBuffer2);
	if (FAILED(hr))
	{
		return hr;
	}
	*/
	//ID3D11Buffer* vertexBuffers[2];
	//vertexBuffers[0] = g_pVertexBuffer;
	//vertexBuffers[1] = g_pVertexBuffer2;
//	{
//		g_pVertexBuffer, g_pVertexBuffer2
//	};

	unsigned int strides[] =
	{
		sizeof(SimpleVertex), sizeof(SimpleVertex)
	};

	UINT offsets[] =
	{
		0, 0
	};

	// Set vertex buffer
	auto stride = sizeof(SimpleVertex);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * nodemesh0->getNumIndices();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	//InitData.pSysMem = indices;
	InitData.pSysMem = nodemesh0->getIndices();
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	// Set index buffer
	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// Create the constant buffers
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBNeverChanges);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pCBNeverChanges);
	if (FAILED(hr))
	{
		return hr;
	}

	bd.ByteWidth = sizeof(CBChangeOnResize);
	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pCBChangeOnResize);
	if (FAILED(hr))
	{
		return hr;
	}

	bd.ByteWidth = sizeof(CBChangesEveryFrame);
	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pCBChangesEveryFrame);

	return hr;
}

auto Rndrr::initDevice()->HRESULT
{
	auto hr = S_OK;
	auto createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	//auto numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	auto numFeatureLevels = ARRAYSIZE(featureLevels);

	for (auto g_driverType : driverTypes)
	{
		hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);

		if (hr == E_INVALIDARG)
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
		}

		if (SUCCEEDED(hr))
		{
			break;
		}
	}

	if (FAILED(hr))
	{
		return hr;
	}

	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
	IDXGIFactory1* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}
	if (FAILED(hr))
	{
		return hr;
	}

	// Create swap chain
	IDXGIFactory2* dxgiFactory2 = nullptr;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
	if (dxgiFactory2)
	{
		// DirectX 11.1 or later
		hr = g_pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(g_pd3dDevice1));
		if (SUCCEEDED(hr))
		{
			g_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&g_pImmediateContext1));
			static_cast<void>(g_pImmediateContext);
		}

		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Width = width;
		sd.Height = height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		hr = dxgiFactory2->CreateSwapChainForHwnd(g_pd3dDevice, *g_hWnd.get(), &sd, nullptr, nullptr, &g_pSwapChain1);
		if (SUCCEEDED(hr))
		{
			hr = g_pSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&g_pSwapChain));
		}

		dxgiFactory2->Release();
	}
	else
	{
		// DirectX 11.0 systems
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = *g_hWnd.get();
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(g_pd3dDevice, &sd, &g_pSwapChain);
	}

	// Doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
	dxgiFactory->MakeWindowAssociation(*g_hWnd.get(), DXGI_MWA_NO_ALT_ENTER);

	dxgiFactory->Release();

	if (FAILED(hr))
	{
		return hr;
	}

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
	{
		return hr;
	}

	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
	pBackBuffer->Release();
	return hr;
}

auto Rndrr::CleanupDevice() -> void
{
	if(g_pImmediateContext)			g_pImmediateContext->ClearState();
	if(g_pSamplerLinear)			g_pSamplerLinear->Release();
	if(g_pTextureRV)				g_pTextureRV->Release();
	if(g_pCBNeverChanges)			g_pCBNeverChanges->Release();
	if(g_pCBChangeOnResize)			g_pCBChangeOnResize->Release();
	if(g_pCBChangesEveryFrame)		g_pCBChangesEveryFrame->Release();
	if(g_pVertexBuffer)				g_pVertexBuffer->Release();
	if(g_pIndexBuffer)				g_pIndexBuffer->Release();
	if(g_pVertexLayout)				g_pVertexLayout->Release();
	if(g_pVertexShader)				g_pVertexShader->Release();
	if(g_pPixelShader)				g_pPixelShader->Release();
	if(g_pDepthStencil)				g_pDepthStencil->Release();
	if(g_pDepthStencilView)			g_pDepthStencilView->Release();
	if(g_pRenderTargetView)			g_pRenderTargetView->Release();
	if(g_pSwapChain1)				g_pSwapChain1->Release();
	if(g_pSwapChain)				g_pSwapChain->Release();
	if(g_pImmediateContext1)		g_pImmediateContext1->Release();
	if(g_pImmediateContext)			g_pImmediateContext->Release();
	if(g_pd3dDevice1)				g_pd3dDevice1->Release();
	if(g_pd3dDevice)				g_pd3dDevice->Release();
}

auto Rndrr::initialize() ->HRESULT
{
	RECT rc;
	GetClientRect(*g_hWnd.get(), &rc);
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;

	// Initialize devices.
	auto hr = Rndrr::initDevice();
	if (FAILED(hr))
	{
		return hr;
	}

	// Depth stencil texture and view.
	hr = Rndrr::createDepthStencilTextureAndView();
	if (FAILED(hr))
	{
		return hr;
	}

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

	// Setup the viewport
	Rndrr::setupViewport();

	// Initialize the shaders.
	ID3DBlob* pVSBlob = nullptr;
	ID3DBlob* pPSBlob = nullptr;
	hr = Rndrr::initShaders(pVSBlob, pPSBlob);
	if (FAILED(hr))
	{
		return hr;
	}

	// Define and create the input layout.
	hr = Rndrr::createInputLayout(pVSBlob);

	pVSBlob->Release();
	if (FAILED(hr))
	{
		return hr;
	}

	// Set the input layout
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

	return hr;
}

auto Rndrr::prerenderSetup() -> HRESULT
{
	auto hr = S_OK;

	// get scenegraph vertices, indices, textures

	hr = Rndrr::initBuffers();

	if (FAILED(hr))
	{
		return hr;
	}

	// Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	hr = Rndrr::initTexture();
	if (FAILED(hr))
	{
		return hr;
	}

	Rndrr::initMatrices();

	return S_OK;
}

//Immediate Context: Getters and Setters
auto Rndrr::getImmediateContext()->ID3D11DeviceContext*
{
	return this->g_pImmediateContext;
};
auto Rndrr::setImmediateContext(ID3D11DeviceContext* iContext)->void
{
	this->g_pImmediateContext = iContext;
};

//World: Getters and Setters
auto Rndrr::getWorld()->DirectX::XMMATRIX
{
	return this->g_World;
};
auto Rndrr::setWorld(const DirectX::XMMATRIX& wMatrix)->void
{
	this->g_World = wMatrix;
};

//View: Getters and Setters
auto Rndrr::getView()->DirectX::XMMATRIX
{
	return this->g_View;
};
auto Rndrr::setView(const DirectX::XMMATRIX& vMatrix)->void
{
	this->g_View = vMatrix;
};

//Projection: Getters and Setters
auto Rndrr::getProjection()->DirectX::XMMATRIX
{
	return this->g_Projection;
};
auto Rndrr::setProjection(const DirectX::XMMATRIX& pMatrix)->void
{
	this->g_Projection = pMatrix;
};

//Driver Type: Getters and Setters
auto Rndrr::getDriverType()->D3D_DRIVER_TYPE
{
	return this->g_driverType;
};
auto Rndrr::setDriverType(D3D_DRIVER_TYPE dType)->void
{
	this->g_driverType = dType;
};

//Mesh Color: Getters and Setters
auto Rndrr::getMeshColor()->DirectX::XMFLOAT4
{
	return this->g_vMeshColor;
};
auto Rndrr::setMeshColor(DirectX::XMFLOAT4 meshColor)->void
{
	this->g_vMeshColor = meshColor;
};

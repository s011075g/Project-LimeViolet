#include "DX11Render.h"
#include "../../Common/resource.h"
#include "DX11VBOManager.h"
#include "DX11TextureManager.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_DESTROY)
		PostQuitMessage(0);
	else
		return DefWindowProc(hWnd, message, wParam, lParam);
	return 0;
}

DX11Render::DX11Render()
	:_msg({nullptr}) ,_driverType(), _featureLevel(), _device(nullptr), _context(nullptr), _swapChain(nullptr), 
	_renderTargetView(nullptr), _depthStencilView(nullptr), _offScreenView(nullptr), _offScreen(nullptr), _hWnd(nullptr)
{ }

DX11Render::~DX11Render()
{
	DX11Render::CleanUp();
}

#define IDI_TUTORIAL1 109

HRESULT DX11Render::InitWindow(RECT& rc, const char*& windowTitle)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(nullptr);;
	wcex.hIcon = LoadIcon(GetModuleHandle(nullptr), reinterpret_cast<LPCTSTR>(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"WindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, reinterpret_cast<LPCTSTR>(IDI_TUTORIAL1));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(nullptr, L"Failed to register window class", L"", 0);
		return E_FAIL;
	}

	//_hInst = GetModuleHandle(NULL);
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	std::string title_(windowTitle); //todo find a better soultion 
	std::wstring title(title_.begin(), title_.end());
	_hWnd = CreateWindow(L"WindowClass", title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
							rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
	if (!_hWnd)
	{
		MessageBox(nullptr, L"Failed to create window: " + GetLastError(), L"", 0);
		return E_FAIL;
	}

	ShowWindow(_hWnd, 10);

	_windowWidth = rc.right - rc.left;
	_windowHeight = rc.bottom - rc.top;

	return S_OK;
}

HRESULT DX11Render::InitRenderer()
{
	UINT deviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = _windowWidth;
	sd.BufferDesc.Height = _windowHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = _hWnd;
	sd.SampleDesc.Count = 4;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	HRESULT hr;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, deviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &_swapChain, &_device, &_featureLevel, &_context);
		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
		return hr;
	
	//Back buffer
	D3D11_TEXTURE2D_DESC depthDesc;
	depthDesc.Width = _windowWidth;
	depthDesc.Height = _windowHeight;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 4;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;

	ID3D11Texture2D* pDepthStencilBuffer(nullptr);
	hr = _device->CreateTexture2D(&depthDesc, nullptr, &pDepthStencilBuffer);
	if (FAILED(hr))
		return hr;
	hr = _device->CreateDepthStencilView(pDepthStencilBuffer, nullptr, &_depthStencilView);
	pDepthStencilBuffer->Release();
	if (FAILED(hr))
		return hr;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return hr;
	hr = _device->CreateRenderTargetView(pBackBuffer, nullptr, &_renderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	//Create a offscreen render target view
	D3D11_TEXTURE2D_DESC offScreenDesc;
	offScreenDesc.Width = _windowWidth;
	offScreenDesc.Height = _windowHeight;
	offScreenDesc.MipLevels = 1;
	offScreenDesc.ArraySize = 1;
	offScreenDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	offScreenDesc.SampleDesc.Count = 1;
	offScreenDesc.SampleDesc.Quality = 0;
	offScreenDesc.Usage = D3D11_USAGE_DEFAULT;
	offScreenDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	offScreenDesc.CPUAccessFlags = 0;
	offScreenDesc.MiscFlags = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = offScreenDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = offScreenDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = offScreenDesc.MipLevels - 1;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = srvDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	//OffScreen
	ID3D11Texture2D* pOffScreen(nullptr);
	hr = _device->CreateTexture2D(&offScreenDesc, nullptr, &pOffScreen);
	if (FAILED(hr))
		return hr;
	hr = _device->CreateRenderTargetView(pOffScreen, &rtvDesc, &_offScreenView);
	if (FAILED(hr))
		return hr;
	hr = _device->CreateShaderResourceView(pOffScreen, &srvDesc, &_offScreen);
	if (FAILED(hr))
		return hr;
	pOffScreen->Release();

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(_windowWidth);
	vp.Height = static_cast<FLOAT>(_windowHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_context->RSSetViewports(1, &vp);

	//InitShaderManager();

	if (FAILED(hr))
		return hr;

	_vboManager = new DX11VBOManager(_device);
	_textureManager = new DX11TextureManager(_device);

	return hr;
}

void DX11Render::Update()
{
	IRender::Update();
	if (PeekMessage(&_msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
	}
}

void DX11Render::DrawStart()
{
	ID3D11ShaderResourceView* null = nullptr;
	_context->PSSetShaderResources(0, 1, &null);
	_context->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
	_context->ClearRenderTargetView(_renderTargetView, _activeCamera->clearColor.rgba);
	_context->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DX11Render::DrawObject(TransformComponent* transform, RenderableMeshComponent* mesh, MeshRenderComponent* materials)
{
	//Sets shaders and resources
	//Draw object
}

void DX11Render::DrawEnd()
{
	_swapChain->Present(1, 0); //(1,0) = vsync
}

bool DX11Render::ShouldExit()
{
	return _msg.message != WM_QUIT;
}

void DX11Render::CleanUp()
{
	if (_context)
	{
		_context->ClearState();
		_context->Release();
	}
	if (_device) 
		_device->Release();

	if (_swapChain)
		_swapChain->Release();
	if (_renderTargetView)
		_renderTargetView->Release();
	if (_depthStencilView)
		_depthStencilView->Release();
	if (_offScreenView)
		_offScreenView->Release();
	if (_offScreen)
		_offScreen->Release();
}

#include <directxmath.h>

void DX11Render::UpdateViewMatrix()
{
	DirectX::XMFLOAT3 eye(_activeCamera->eye.x, _activeCamera->eye.y, _activeCamera->eye.z);
	DirectX::XMFLOAT3 at(_activeCamera->at.x, _activeCamera->at.y, _activeCamera->at.z);
	DirectX::XMFLOAT3 up(_activeCamera->up.x, _activeCamera->up.y, _activeCamera->up.z);
	DirectX::XMFLOAT4X4 m;
	DirectX::XMStoreFloat4x4(&m, DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eye), DirectX::XMLoadFloat3(&at), DirectX::XMLoadFloat3(&up)));
	for(int x = 0; x != 4; x++)
		for(int y = 0; y != 4; y++)
			_view.m[x][y] = m.m[x][y];
}

void DX11Render::UpdateProjectionMatrix()
{
#undef far //undefined from minwindef as they are macros
#undef near
	DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&_projection.m), DirectX::XMMatrixPerspectiveFovLH(_activeCamera->fieldOfView, static_cast<float>(_windowWidth) / static_cast<float>(_windowHeight), _activeCamera->near, _activeCamera->far));
}
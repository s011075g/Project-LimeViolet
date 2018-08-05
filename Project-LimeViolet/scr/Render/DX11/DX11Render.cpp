#include "DX11Render.h"
#include "../../Common/resource.h"
#include "DX11VBOManager.h"
#include "DX11TextureManager.h"
#include "DX11Shader.h"
#include "DX11ConstantBuffers.h"

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
	_renderTargetView(nullptr), _depthStencilView(nullptr), _shaderManager(nullptr), _hWnd(nullptr)
{ }

DX11Render::~DX11Render()
{
	DX11Render::CleanUp();
	delete _shaderManager;
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
	//Swap chain
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

	HRESULT hr = S_OK;

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
	
	//Depth Buffer
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
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer));
	if (FAILED(hr))
		return hr;
	hr = _device->CreateRenderTargetView(pBackBuffer, nullptr, &_renderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_context->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(_windowWidth);
	vp.Height = static_cast<FLOAT>(_windowHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_context->RSSetViewports(1, &vp);

	if (FAILED(hr))
		return hr;

	_vboManager = new DX11VBOManager(_device);
	_textureManager = new DX11TextureManager(_device);
	_shaderManager = new DX11ShaderManager(_device);

	//TESTing
	D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_SOLID; //Solid objects
	wfdesc.CullMode = D3D11_CULL_NONE; //Culls the back away
	ID3D11RasterizerState* state;
	hr = _device->CreateRasterizerState(&wfdesc, &state);
	_context->RSSetState(state);
	state->Release();

	return hr;
}

DX11Shader* DX11Render::CreateShader(const char* fileLocation, int shaders) const
{
	if (!_shaderManager)
		return nullptr;
	std::string location(fileLocation);
	std::wstring file = std::wstring(location.begin(), location.end());
	ID3D11InputLayout* layout(nullptr);
	ID3D11VertexShader* vertex(nullptr);
	ID3D11HullShader* hull(nullptr);
	ID3D11DomainShader* domain(nullptr);
	ID3D11GeometryShader* geometry(nullptr);
	ID3D11PixelShader* pixel(nullptr);
	ID3D11Buffer* perDraw(nullptr),* perObject(nullptr);
	bool error = false;
	if(shaders & SHADER::VERTEX)
	{
		ID3DBlob* vertexBlob(nullptr);
		if (!FAILED(_shaderManager->CreateVertexShader(file.c_str(), vertexBlob, vertex)))
		{
			if (FAILED(_shaderManager->CreateInputLayout(vertexBlob, layout)))
				error = true;
		}
		else
			error = true;
	}
	if(shaders & SHADER::HULL)
		if (FAILED(_shaderManager->CreateHullShader(file.c_str(), hull)))
			error = true;
	if(shaders & SHADER::DOMAIN)
		if (FAILED(_shaderManager->CreateDomainShader(file.c_str(), domain)))
			error = true;
	if(shaders & SHADER::GEOMETRY)
		if (FAILED(_shaderManager->CreateGeometryShader(file.c_str(), geometry)))
			error = true;
	if (shaders & SHADER::PIXEL)
		if (FAILED(_shaderManager->CreatePixelShader(file.c_str(), pixel)))
			error = true;
	HRESULT hr = _shaderManager->CreateConstantBuffer(sizeof(PerDrawBuffer), perDraw);
	if (FAILED(hr))
		error = true;
	hr = _shaderManager->CreateConstantBuffer(sizeof(PerObjectBuffer), perObject);
	if (FAILED(hr))
		error = true;
	if(error)
	{
		if (layout)
			layout->Release();
		if (vertex)
			vertex->Release();
		if (hull)
			hull->Release();
		if (domain)
			domain->Release();
		if (geometry)
			geometry->Release();
		if (pixel)
			pixel->Release();
		if (perDraw)
			perDraw->Release();
		if (perObject)
			perObject->Release();
		return nullptr;
	}
	DX11Shader* result = new DX11Shader(layout, vertex, hull, domain, geometry, pixel, perDraw, perObject);
	return result;
}

void DX11Render::Update()
{
	if (PeekMessage(&_msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
	}
}

void DX11Render::DrawStart() const
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
	ID3D11ShaderResourceView* null = nullptr;
	_context->PSSetShaderResources(0, 1, &null);
	_context->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
	_context->ClearRenderTargetView(_renderTargetView, _activeCamera->clearColor.rgba);
	_context->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	PerDrawBuffer* buffer = new PerDrawBuffer();
	buffer->viewProjection = _view * _projection;

	_shaderManager->SetPerDrawBuffer(buffer);
}

void DX11Render::SetTextures(Material* material) const
{
	_shaderManager->SetTextureDiffuse(_context, static_cast<ID3D11ShaderResourceView*>(material->diffuseTexture));
	_shaderManager->SetTextureSpecular(_context, static_cast<ID3D11ShaderResourceView*>(material->specularTexture));
	_shaderManager->SetTextureNormal(_context, static_cast<ID3D11ShaderResourceView*>(material->normalTexture));
	_shaderManager->SetTextureOcclusion(_context, static_cast<ID3D11ShaderResourceView*>(material->occlusionTexture));
}

void DX11Render::DrawObject(TransformComponent* transform, RenderableMeshComponent* mesh, MaterialComponent* materials) const
{
	static const unsigned int stride = sizeof(ObjectVertex);
	static const unsigned int offset = 0;

	ID3D11Buffer* vertex = static_cast<ID3D11Buffer*>(mesh->geometry->GetVertexBuffer());
	_context->IASetVertexBuffers(0, 1, &vertex, &stride, &offset);
	DX11Shader* shader = static_cast<DX11Shader*>(materials->shader);
	_shaderManager->SetShader(_context, shader);
	for (size_t i = 0; i < materials->materials.size(); i++)
	{
		//Sets shaders and resources
		PerObjectBuffer buffer = {
			transform->transform.ToMatrix(), Float4(materials->materials[i]->diffuseColor.rgba),
			Float3(materials->materials[i]->specularColor.rgb), materials->materials[i]->specularPower
		};
		shader->SetPerObjectBuffer(_context, static_cast<void*>(&buffer));

		//SetTextures(materials->materials[i]);
		//Draw object
		_context->IASetIndexBuffer(static_cast<ID3D11Buffer*>(mesh->geometry->GetIndexBuffer()[i].first), DXGI_FORMAT_R16_UINT, 0);
		_context->DrawIndexed(mesh->geometry->GetIndexBuffer()[i].second, 0, 0);
	}
}

void DX11Render::DrawEnd() const
{
	_swapChain->Present(0, 0); //(1,0) = vsync
	_shaderManager->EndFrame();
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
}

#include <directxmath.h>

void DX11Render::UpdateViewMatrix() const
{
	DirectX::XMFLOAT3 eye(_activeCamera->eye.x, _activeCamera->eye.y, _activeCamera->eye.z);
	DirectX::XMFLOAT3 at(_activeCamera->at.x, _activeCamera->at.y, _activeCamera->at.z);
	DirectX::XMFLOAT3 up(_activeCamera->up.x, _activeCamera->up.y, _activeCamera->up.z);

	DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&_view.m), DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eye), DirectX::XMLoadFloat3(&at), DirectX::XMLoadFloat3(&up)));
}

void DX11Render::UpdateProjectionMatrix() const
{
	DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&_projection.m), DirectX::XMMatrixPerspectiveFovLH(_activeCamera->fieldOfView, static_cast<float>(_windowWidth) / static_cast<float>(_windowHeight), _activeCamera->nearPlane, _activeCamera->farPlane));
}
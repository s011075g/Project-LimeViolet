#include "DX11ShaderManager.h"
#include <d3dcompiler.h>
#include "../../Common/Utilities.h"

DX11ShaderManager::DX11ShaderManager(ID3D11Device* const& device)
	: _device(device), _diffuseTexture(nullptr), _specularTexture(nullptr), 
		_normalTexture(nullptr), _occlusionTexture(nullptr), _currentShader(nullptr),
		_currentPerDrawBuffer(nullptr)
{ }

DX11ShaderManager::~DX11ShaderManager()
{ }

HRESULT DX11ShaderManager::CreateVertexShader(const wchar_t* fileLocation, ID3DBlob*& blob, ID3D11VertexShader*& outShader) const
{
	void* outPtr;
	HRESULT hr = CreateShader(fileLocation, SHADER::VERTEX, "VS", "vs_4_0", blob, outPtr);
	outShader = static_cast<ID3D11VertexShader*>(outPtr);
	if (FAILED(hr))
		blob->Release();
	return hr;
}

HRESULT DX11ShaderManager::CreateHullShader(const wchar_t* fileLocation, ID3D11HullShader*& outShader) const
{
	void* outPtr;
	ID3DBlob* blob;
	HRESULT hr = CreateShader(fileLocation, SHADER::HULL, "HS", "hs_5_0", blob, outPtr);
	outShader = static_cast<ID3D11HullShader*>(outPtr);
	return hr;
}

HRESULT DX11ShaderManager::CreateDomainShader(const wchar_t* fileLocation, ID3D11DomainShader*& outShader) const
{
	void* outPtr;
	ID3DBlob* blob;
	HRESULT hr = CreateShader(fileLocation, SHADER::DOMAIN, "DS", "ds_5_0", blob, outPtr);
	outShader = static_cast<ID3D11DomainShader*>(outPtr);
	return hr;
}

HRESULT DX11ShaderManager::CreateGeometryShader(const wchar_t* fileLocation, ID3D11GeometryShader*& outShader) const
{
	void* outPtr;
	ID3DBlob* blob;
	HRESULT hr = CreateShader(fileLocation, SHADER::GEOMETRY, "GS", "gs_4_0", blob, outPtr);
	outShader = static_cast<ID3D11GeometryShader*>(outPtr);
	return hr;
}

HRESULT DX11ShaderManager::CreatePixelShader(const wchar_t* fileLocation, ID3D11PixelShader*& outShader) const
{
	void* outPtr;
	ID3DBlob* blob;
	HRESULT hr = CreateShader(fileLocation, SHADER::PIXEL, "PS", "ps_4_0", blob, outPtr);
	outShader = static_cast<ID3D11PixelShader*>(outPtr);
	return hr;
}

HRESULT DX11ShaderManager::CreateConstantBuffer(size_t size, ID3D11Buffer*& outBuffer) const
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = size;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	return _device->CreateBuffer(&desc, nullptr, &outBuffer);
}

void DX11ShaderManager::SetTextureDiffuse(ID3D11DeviceContext*const& context, ID3D11ShaderResourceView* ptr) const
{
	if(ptr == _diffuseTexture)
		return;
	context->PSSetShaderResources(0, 1, &ptr);
	_diffuseTexture = ptr;
}

void DX11ShaderManager::SetTextureSpecular(ID3D11DeviceContext* const& context, ID3D11ShaderResourceView* ptr) const
{
	if (ptr == _specularTexture)
		return;
	context->PSSetShaderResources(1, 1, &ptr);
	_specularTexture = ptr;
}

void DX11ShaderManager::SetTextureNormal(ID3D11DeviceContext* const& context, ID3D11ShaderResourceView* ptr) const
{
	if(ptr == _normalTexture)
		return;
	context->PSSetShaderResources(2, 1, &ptr);
	_normalTexture = ptr;
}

void DX11ShaderManager::SetTextureOcclusion(ID3D11DeviceContext* const& context, ID3D11ShaderResourceView* ptr) const
{
	if(ptr == _occlusionTexture)
		return;
	context->PSSetShaderResources(3, 1, &ptr);
	_occlusionTexture = ptr;
}

void DX11ShaderManager::SetShader(ID3D11DeviceContext* const& context, DX11Shader* shader) const
{
	if(_currentShader != shader)
	{
		_currentShader = shader;
		shader->SetShader(context);
		if(_currentPerDrawBuffer)
			shader->SetPerDrawBuffer(context, _currentPerDrawBuffer);
	}
}

void DX11ShaderManager::SetPerDrawBuffer(PerDrawBuffer* data) const
{
	if (_currentPerDrawBuffer)
		delete _currentPerDrawBuffer;
	_currentPerDrawBuffer = data;
}

void DX11ShaderManager::EndFrame() const
{
	_currentShader = nullptr;
}

HRESULT DX11ShaderManager::CreateInputLayout(ID3DBlob*& vertexBlob, ID3D11InputLayout*& outLayout) const
{
	if (!vertexBlob) 
		return S_FALSE;

	D3D11_INPUT_ELEMENT_DESC arr[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA , 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA , 0 }
	};

	HRESULT hr = _device->CreateInputLayout(arr, ARRAYSIZE(arr), vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), &outLayout);
	vertexBlob->Release();
	return hr;
}

HRESULT DX11ShaderManager::CreateShader(const wchar_t* fileLocation, const SHADER& shader, const LPCSTR& entryPoint, const LPCSTR& shaderModel, ID3DBlob*& outBlob, void*& outShader) const
{
	HRESULT hr = CompileShaderFromFile(fileLocation, entryPoint, shaderModel, outBlob);
	if(FAILED(hr))
	{
		Utilities::Write("Failed to compile shader", Utilities::EXTREME_LEVEL);
		return hr;
	}

	switch(shader)
	{
	case SHADER::VERTEX:
		{
			ID3D11VertexShader * shaderPtr;
			hr = _device->CreateVertexShader(outBlob->GetBufferPointer(), outBlob->GetBufferSize(), nullptr, &shaderPtr);
			outShader = shaderPtr; 
		}
		break;
	case SHADER::HULL:
		{
			ID3D11HullShader * shaderPtr;
			hr = _device->CreateHullShader(outBlob->GetBufferPointer(), outBlob->GetBufferSize(), nullptr, &shaderPtr);
			outShader = shaderPtr; 
			outBlob->Release();
		}
		break;
	case SHADER::DOMAIN:
		{
			ID3D11DomainShader * shaderPtr;
			hr = _device->CreateDomainShader(outBlob->GetBufferPointer(), outBlob->GetBufferSize(), nullptr, &shaderPtr);
			outShader = shaderPtr;
			outBlob->Release();
		}
		break;
	case SHADER::GEOMETRY:
		{
			ID3D11GeometryShader * shaderPtr;
			hr = _device->CreateGeometryShader(outBlob->GetBufferPointer(), outBlob->GetBufferSize(), nullptr, &shaderPtr);
			outShader = shaderPtr;
			outBlob->Release();
		}
		break;
	case SHADER::PIXEL:
		{
			ID3D11PixelShader * shaderPtr;
			hr = _device->CreatePixelShader(outBlob->GetBufferPointer(), outBlob->GetBufferSize(), nullptr, &shaderPtr);
			outShader = shaderPtr;
			outBlob->Release();
		}
		break;
	}

	return hr;
}

HRESULT DX11ShaderManager::CompileShaderFromFile(const WCHAR* fileName, const LPCSTR& entryPoint, const LPCSTR& shaderModel, ID3DBlob*& blobOut)
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* errorBlob(nullptr);
	HRESULT hr = D3DCompileFromFile(fileName, nullptr, nullptr, entryPoint, shaderModel, shaderFlags, 0, &blobOut, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
			errorBlob->Release();
		}
		return hr;
	}
	if (errorBlob)
		errorBlob->Release();
	return hr;
}

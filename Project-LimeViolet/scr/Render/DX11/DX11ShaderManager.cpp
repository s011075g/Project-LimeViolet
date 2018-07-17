#include "DX11ShaderManager.h"
#include <d3dcompiler.h>
#include "../../Common/Utilities.h"

DX11ShaderManager::DX11ShaderManager(ID3D11Device* const& device)
	: _device(device)
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
	HRESULT hr = CreateShader(fileLocation, SHADER::GEOMETRY, "PS", "ps_4_0", blob, outPtr);
	outShader = static_cast<ID3D11PixelShader*>(outPtr);
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

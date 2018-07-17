#pragma once
#include "../IShaderManager.h"
#include <d3d11.h>

class DX11ShaderManager : public IShaderManager
{
private:
		ID3D11Device* const & _device;
public:
	DX11ShaderManager(ID3D11Device* const& device);
	~DX11ShaderManager();

	HRESULT CreateVertexShader(const wchar_t* fileLocation, ID3DBlob*& blob, ID3D11VertexShader*& outShader) const;
	HRESULT CreateHullShader(const wchar_t* fileLocation, ID3D11HullShader*& outShader) const;
	HRESULT CreateDomainShader(const wchar_t* fileLocation, ID3D11DomainShader*& outShader) const;
	HRESULT CreateGeometryShader(const wchar_t* fileLocation, ID3D11GeometryShader*& outShader) const;
	HRESULT CreatePixelShader(const wchar_t* fileLocation, ID3D11PixelShader*& outShader) const;
private:
	enum class SHADER
	{
		VERTEX,
		HULL,
		DOMAIN,
		GEOMETRY,
		PIXEL
	};

	HRESULT CreateShader(const wchar_t* fileLocation, const SHADER& shader, const LPCSTR& entryPoint, const LPCSTR& shaderModel, ID3DBlob*& outBlob, void*& outShader) const;
	static HRESULT CompileShaderFromFile(const WCHAR* fileName, const LPCSTR& entryPoint, const LPCSTR& shaderModel, ID3DBlob*& outBlob);
};
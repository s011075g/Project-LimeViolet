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

	HRESULT CreateInputLayout(ID3DBlob*& vertexBlob, ID3D11InputLayout*& outLayout) const;

	HRESULT CreateVertexShader(const wchar_t* fileLocation, ID3DBlob*& blob, ID3D11VertexShader*& outShader) const;
	HRESULT CreateHullShader(const wchar_t* fileLocation, ID3D11HullShader*& outShader) const;
	HRESULT CreateDomainShader(const wchar_t* fileLocation, ID3D11DomainShader*& outShader) const;
	HRESULT CreateGeometryShader(const wchar_t* fileLocation, ID3D11GeometryShader*& outShader) const;
	HRESULT CreatePixelShader(const wchar_t* fileLocation, ID3D11PixelShader*& outShader) const;
	
	HRESULT CreateConstantBuffer(unsigned int& size, ID3D11Buffer*& outBuffer) const;

	void SetTextureDiffuse(ID3D11DeviceContext*const& context, ID3D11ShaderResourceView * ptr) const;
	void SetTextureSpecular(ID3D11DeviceContext*const& context, ID3D11ShaderResourceView * ptr) const;
	void SetTextureNormal(ID3D11DeviceContext*const& context, ID3D11ShaderResourceView * ptr) const;
	void SetTextureOcclusion(ID3D11DeviceContext*const& context, ID3D11ShaderResourceView * ptr) const;
private:
	enum class SHADER
	{
		VERTEX, HULL, DOMAIN, GEOMETRY, PIXEL
	};

	HRESULT CreateShader(const wchar_t* fileLocation, const SHADER& shader, const LPCSTR& entryPoint, const LPCSTR& shaderModel, ID3DBlob*& outBlob, void*& outShader) const;
	static HRESULT CompileShaderFromFile(const WCHAR* fileName, const LPCSTR& entryPoint, const LPCSTR& shaderModel, ID3DBlob*& outBlob);

	mutable ID3D11ShaderResourceView * _diffuseTexture;
	mutable ID3D11ShaderResourceView * _specularTexture;
	mutable ID3D11ShaderResourceView * _normalTexture;
	mutable ID3D11ShaderResourceView * _occlusionTexture;
};

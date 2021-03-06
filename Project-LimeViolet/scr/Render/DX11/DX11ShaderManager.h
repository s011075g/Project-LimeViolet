#pragma once
#include "../IShaderManager.h"
#include <d3d11.h>
#include "DX11Shader.h"
#include "DX11ConstantBuffers.h"
#undef DOMAIN
enum SHADER : int
{
	VERTEX = 1, HULL = 2, DOMAIN = 4, GEOMETRY = 8, PIXEL = 16
};

class DX11ShaderManager : public IShaderManager
{
	ID3D11Device* const & _device;
	//Current set data on the GPU so data doesn't have to transfer unless needed
	mutable ID3D11ShaderResourceView * _diffuseTexture;
	mutable ID3D11ShaderResourceView * _specularTexture;
	mutable ID3D11ShaderResourceView * _normalTexture;
	mutable ID3D11ShaderResourceView * _occlusionTexture;
	mutable DX11Shader* _currentShader;
	mutable PerDrawBuffer* _currentPerDrawBuffer;
public:
	DX11ShaderManager(ID3D11Device* const& device);
	~DX11ShaderManager();

	HRESULT CreateInputLayout(ID3DBlob*& vertexBlob, ID3D11InputLayout*& outLayout) const;

	HRESULT CreateVertexShader(const wchar_t* fileLocation, ID3DBlob*& blob, ID3D11VertexShader*& outShader) const;
	HRESULT CreateHullShader(const wchar_t* fileLocation, ID3D11HullShader*& outShader) const;
	HRESULT CreateDomainShader(const wchar_t* fileLocation, ID3D11DomainShader*& outShader) const;
	HRESULT CreateGeometryShader(const wchar_t* fileLocation, ID3D11GeometryShader*& outShader) const;
	HRESULT CreatePixelShader(const wchar_t* fileLocation, ID3D11PixelShader*& outShader) const;
	
	HRESULT CreateConstantBuffer(size_t size, ID3D11Buffer*& outBuffer) const;

	void SetTextureDiffuse(ID3D11DeviceContext*const& context, ID3D11ShaderResourceView * ptr) const;
	void SetTextureSpecular(ID3D11DeviceContext*const& context, ID3D11ShaderResourceView * ptr) const;
	void SetTextureNormal(ID3D11DeviceContext*const& context, ID3D11ShaderResourceView * ptr) const;
	void SetTextureOcclusion(ID3D11DeviceContext*const& context, ID3D11ShaderResourceView * ptr) const;

	void SetShader(ID3D11DeviceContext*const& context, DX11Shader* shader)const;
	void SetPerDrawBuffer(PerDrawBuffer* data) const;
	PerDrawBuffer* GetPerDrawBuffer() const;
	//Updates the values needed ready for the next frame 
	void EndFrame() const;
private:
	HRESULT CreateShader(const wchar_t* fileLocation, const SHADER& shader, const LPCSTR& entryPoint, const LPCSTR& shaderModel, ID3DBlob*& outBlob, void*& outShader) const;
	static HRESULT CompileShaderFromFile(const WCHAR* fileName, const LPCSTR& entryPoint, const LPCSTR& shaderModel, ID3DBlob*& outBlob);
};

#pragma once
#include <d3d11.h>
class DX11Shader
{
public:
	friend class DX11ShaderManager; //Allows access to the SetShader Function

	DX11Shader(ID3D11InputLayout *& layout, ID3D11VertexShader*& vertex, ID3D11HullShader* hull, ID3D11DomainShader* domain, ID3D11GeometryShader* geometry, ID3D11PixelShader*& pixel, ID3D11Buffer*& perDraw, ID3D11Buffer*& perObject);
	~DX11Shader();

	//PerDraw is registered to b0
	void SetPerDrawBuffer(ID3D11DeviceContext*const& context, void* data) const;
	//PerObject is registered to b1
	void SetPerObjectBuffer(ID3D11DeviceContext*const& context, void* data) const;
protected:
	void SetShader(ID3D11DeviceContext*const& context) const;
private:
	ID3D11InputLayout * _inputLayout;

	ID3D11VertexShader * _vertexShader;
	ID3D11HullShader * _hullShader;
	ID3D11DomainShader * _domainShader;
	ID3D11GeometryShader* _geometryShader;
	ID3D11PixelShader* _pixelShader;

	ID3D11Buffer* _perDrawBuffer;
	ID3D11Buffer* _perObjectBuffer;
};


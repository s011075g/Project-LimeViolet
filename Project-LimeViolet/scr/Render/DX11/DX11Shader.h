#pragma once
#include <d3d11.h>
class DX11Shader
{
public:
	DX11Shader(ID3D11VertexShader* vertex, ID3D11HullShader* hull, ID3D11DomainShader* domain, ID3D11GeometryShader* geometry, ID3D11PixelShader* pixel);
	~DX11Shader();

	void SetShader(ID3D11DeviceContext*& context) const;
private:
	ID3D11VertexShader * _vertexShader;
	ID3D11HullShader * _hullShader;
	ID3D11DomainShader * _domainShader;
	ID3D11GeometryShader* _geometryShader;
	ID3D11PixelShader* _pixelShader;
};


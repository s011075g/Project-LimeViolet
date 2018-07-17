#include "DX11Shader.h"



DX11Shader::DX11Shader(ID3D11VertexShader* vertex, ID3D11HullShader* hull, ID3D11DomainShader* domain, ID3D11GeometryShader* geometry, ID3D11PixelShader* pixel)
	: _vertexShader(vertex), _hullShader(hull), _domainShader(domain), _geometryShader(geometry), _pixelShader(pixel)
{ }

DX11Shader::~DX11Shader()
{
	if (_vertexShader)
		_vertexShader->Release();
	if (_hullShader)
		_hullShader->Release();
	if (_domainShader)
		_domainShader->Release();
	if (_geometryShader)
		_geometryShader->Release();
	if (_pixelShader)
		_pixelShader->Release();
}

void DX11Shader::SetShader(ID3D11DeviceContext*& context) const
{
	context->VSSetShader(_vertexShader, nullptr, 0);
	context->HSSetShader(_hullShader, nullptr, 0);
	context->DSSetShader(_domainShader, nullptr, 0);
	context->GSSetShader(_geometryShader, nullptr, 0);
	context->PSSetShader(_pixelShader, nullptr, 0);
}

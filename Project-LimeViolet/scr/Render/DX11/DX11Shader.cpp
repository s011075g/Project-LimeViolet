#include "DX11Shader.h"

ID3D11Buffer* DX11Shader::_currentPerDrawBuffer = nullptr;
ID3D11Buffer* DX11Shader::_currentPerObjectBuffer = nullptr;

DX11Shader::DX11Shader(ID3D11InputLayout*& layout, ID3D11VertexShader*& vertex, ID3D11HullShader* hull, ID3D11DomainShader* domain, ID3D11GeometryShader* geometry, ID3D11PixelShader*& pixel, ID3D11Buffer*& perDraw, ID3D11Buffer*& perObject)
	: _inputLayout(layout), _vertexShader(vertex), _hullShader(hull), _domainShader(domain), _geometryShader(geometry), _pixelShader(pixel), _perDrawBuffer(perDraw), _perObjectBuffer(perObject)
{ }

DX11Shader::~DX11Shader()
{
	if (_inputLayout)
		_inputLayout->Release();
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
	if (_perDrawBuffer)
		_perDrawBuffer->Release();
	if (_perObjectBuffer)
		_perObjectBuffer->Release();
}

void DX11Shader::SetShader(ID3D11DeviceContext*const& context) const
{
	context->IASetInputLayout(_inputLayout);
	context->VSSetShader(_vertexShader, nullptr, 0);
	context->HSSetShader(_hullShader, nullptr, 0);
	context->DSSetShader(_domainShader, nullptr, 0);
	context->GSSetShader(_geometryShader, nullptr, 0);
	context->PSSetShader(_pixelShader, nullptr, 0);
}

void DX11Shader::SetPerDrawBuffer(ID3D11DeviceContext*const& context, void* data) const
{
	if (_currentPerDrawBuffer != _perDrawBuffer)
	{
		_currentPerDrawBuffer = _perDrawBuffer;
		if (!_perDrawBuffer)
			return;
		if (_vertexShader)
			context->VSSetConstantBuffers(0, 1, &_perDrawBuffer);
		if (_hullShader)
			context->HSSetConstantBuffers(0, 1, &_perDrawBuffer);
		if (_domainShader)
			context->DSSetConstantBuffers(0, 1, &_perDrawBuffer);
		if (_geometryShader)
			context->GSSetConstantBuffers(0, 1, &_perDrawBuffer);
		if (_pixelShader)
			context->PSSetConstantBuffers(0, 1, &_perDrawBuffer);
	}
	context->UpdateSubresource(_perDrawBuffer, 0, nullptr, data, 0, 0);
}

void DX11Shader::SetPerObjectBuffer(ID3D11DeviceContext*const& context, void* data) const
{
	if (_currentPerObjectBuffer != _perObjectBuffer)
	{
		_currentPerObjectBuffer = _perObjectBuffer;
		if (!_perObjectBuffer)
			return;
		if (_vertexShader)
			context->VSSetConstantBuffers(1, 1, &_perObjectBuffer);
		if (_hullShader)
			context->HSSetConstantBuffers(1, 1, &_perObjectBuffer);
		if (_domainShader)
			context->DSSetConstantBuffers(1, 1, &_perObjectBuffer);
		if (_geometryShader)
			context->GSSetConstantBuffers(1, 1, &_perObjectBuffer);
		if (_pixelShader)
			context->PSSetConstantBuffers(1, 1, &_perObjectBuffer);
	}
	context->UpdateSubresource(_perObjectBuffer, 0, nullptr, data, 0, 0);
}

#pragma once
#include "IRender.h"
#include <d3d11_1.h>

class DX11Render : public IRender
{
private:
	D3D_DRIVER_TYPE _driverType;
	D3D_FEATURE_LEVEL _featureLevel;

	IDXGISwapChain* _swapChain;

	ID3D11Device* _device;
	ID3D11DeviceContext* _context;

	ID3D11RenderTargetView* _renderTargetView;
	ID3D11DepthStencilView* _depthStencilView;

	ID3D11RenderTargetView* _offScreenView;
	ID3D11ShaderResourceView*_offScreen; //Used for Preprocessing
public:
	DX11Render();
	~DX11Render();
	HRESULT InitWindow(const HINSTANCE hInstance, const int nCmdShow, RECT& rc) override;
	HRESULT InitRenderer() override;

	void Draw() override;
protected:
	void CleanUp() override;

	void UpdateViewMatrix() override;
	void UpdateProjectionMatrix() override;
};

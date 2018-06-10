#pragma once
#include "../IRender.h"
#include <d3d11_1.h>

class DX11Render : public IRender
{
private:
	MSG _msg;

	D3D_DRIVER_TYPE _driverType;
	D3D_FEATURE_LEVEL _featureLevel;

	ID3D11Device* _device;
	ID3D11DeviceContext* _context;

	IDXGISwapChain* _swapChain;

	ID3D11RenderTargetView* _renderTargetView;
	ID3D11DepthStencilView* _depthStencilView;

	ID3D11RenderTargetView* _offScreenView;
	ID3D11ShaderResourceView*_offScreen; //Used for Preprocessing

	//Window handler - might not be needed anymoe
	HWND _hWnd;
public:
	DX11Render();
	~DX11Render();
	HRESULT InitWindow(RECT& rc) override;
	HRESULT InitRenderer() override;

	void Update() override;
	void Draw() override;
	bool ShouldExit() override;
protected:
	void CleanUp() override;

	void UpdateViewMatrix() override;
	void UpdateProjectionMatrix() override;
};

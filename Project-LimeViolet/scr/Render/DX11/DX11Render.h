#pragma once
#include "../IRender.h"
#include <d3d11_1.h>
#include "DX11ShaderManager.h"

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

	DX11ShaderManager* _shaderManager;

	//Window handler - might not be needed anymoe
	HWND _hWnd;
public:
	DX11Render();
	~DX11Render();
	HRESULT InitWindow(RECT& rc, const char*& windowTitle) override;
	HRESULT InitRenderer() override;

	void Update() override;
	void DrawStart() const override;
	void DrawObject(TransformComponent* transform, RenderableMeshComponent* mesh, MaterialComponent* materials) const override;
	void DrawEnd() const override;
	bool ShouldExit() override;
protected:
	void CleanUp() override;

	void UpdateViewMatrix() const override;
	void UpdateProjectionMatrix() const override;
};

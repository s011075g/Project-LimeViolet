#include "IRender.h"
#include <complex>

IRender::IRender()
{ }

IRender::~IRender()
{ }

HRESULT IRender::InitWindow(const HINSTANCE hInstance, const int nCmdShow, RECT& rc)
{
	_hInst = hInstance;
	return 0;
}

void IRender::SetActiveCamera(Camera* camera)
{
	_activeCamera = camera;
}

Int2 IRender::GetWindowSize() const
{
	return Int2(_windowWidth, _windowHeight);
}

int IRender::GetWindowWidth() const
{
	return _windowWidth;
}

int IRender::GetWindowHeight() const
{
	return _windowHeight;
}

HWND IRender::GetWindowHandle() const
{
	return _hWnd;
}

void IRender::Update()
{
	UpdateViewMatrix();
}

void IRender::UpdateScreenSize(const int windowWidth, const int windowHeight)
{
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
	UpdateProjectionMatrix();
}

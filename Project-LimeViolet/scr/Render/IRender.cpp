#include "IRender.h"
#include <complex>

IRender::IRender()
	: _windowWidth(0), _windowHeight(0), _activeCamera(nullptr), _hInst(nullptr), _hWnd(nullptr), _vboManager(nullptr)
{
}

IRender::~IRender()
{
	if(_vboManager) delete _vboManager;
}

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

Geometry* IRender::LoadRawGeometry(RawGeometry*& geometry)
{
	if (!_vboManager) 
		return nullptr;
	Geometry* result = _vboManager->VBOGeometry(geometry);
	delete geometry;
	return result;
}

void IRender::FreeGeometry(Geometry*& geometry)
{
	_vboManager->DeleteVBO(geometry);
}

void IRender::UpdateScreenSize(const int windowWidth, const int windowHeight)
{
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
	UpdateProjectionMatrix();
}

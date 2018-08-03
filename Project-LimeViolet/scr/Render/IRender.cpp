#include "IRender.h"

IRender::IRender()
	: _windowWidth(0), _windowHeight(0), _activeCamera(nullptr), _vboManager(nullptr),
	  _textureManager(nullptr)
{ }

IRender::~IRender()
{
	if (_vboManager) delete _vboManager;
	if (_textureManager) delete _textureManager;
}

void IRender::SetActiveCamera(CameraComponent* camera)
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

Geometry* IRender::LoadRawGeometry(RawGeometry*& geometry)
{
	if (!_vboManager) 
		return nullptr;
	Geometry* result = _vboManager->VBOGeometry(geometry); 
	delete geometry;
	geometry = nullptr;
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

void IRender::UpdateProjectionMatrix()
{
	_projection = Float4x4::ProjectionMatrix(_activeCamera->fieldOfView, static_cast<float>(_windowWidth) / static_cast<float>(_windowHeight),_activeCamera->nearPlane, _activeCamera->farPlane);
}

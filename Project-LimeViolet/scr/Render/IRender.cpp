#include "IRender.h"

IRender::IRender()
	: _windowWidth(0), _windowHeight(0), _activeCamera(nullptr), _vboManager(nullptr),
	  _textureManager(nullptr)
{ }

IRender::~IRender()
{
	if (_vboManager) 
		delete _vboManager;
	if (_textureManager) 
		delete _textureManager;
}

void IRender::SetActiveCamera(CameraComponent* camera)
{
	_activeCamera = camera;
	UpdateProjectionMatrix(); //Here we can just calculate the projection when we're given a new camera 
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

Texture IRender::LoadTexture(const char* location) const
{
	if (!_textureManager) return nullptr;
	return _textureManager->GetTexture(location);
}

void IRender::UnloadTexture(const char* location) const
{
	if (!_textureManager) return;
	_textureManager->UnloadTexture(location);
}

void IRender::UpdateScreenSize(const int windowWidth, const int windowHeight)
{
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
	UpdateProjectionMatrix();
}

void IRender::UpdateProjectionMatrix() const
{
	_projection = Float4x4::ProjectionMatrix(_activeCamera->fieldOfView, static_cast<float>(_windowWidth) / static_cast<float>(_windowHeight),_activeCamera->nearPlane, _activeCamera->farPlane);
}

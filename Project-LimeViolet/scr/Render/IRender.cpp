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

HRESULT IRender::InitWindow(RECT& rc)
{
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

	for (auto m : result->GetMaterials())
	{
		if (auto t = m->GetTextureDiffuse())
			m->SetTextureDiffuse(_textureManager->GetTexture(static_cast<const char*>(t)));
		if (auto t = m->GetTextureNormal())
			m->SetTextureNormal(_textureManager->GetTexture(static_cast<const char*>(t)));
		if (auto t = m->GetTextureSpecular())
			m->SetTextureSpecular(_textureManager->GetTexture(static_cast<const char*>(t)));
	}

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
	//Default lefthanded
	_projection = Float4x4::ProjectionMatrix(_activeCamera->GetFOV(), static_cast<float>(_windowWidth) / static_cast<float>(_windowHeight),_activeCamera->GetNear(), _activeCamera->GetFar());
}

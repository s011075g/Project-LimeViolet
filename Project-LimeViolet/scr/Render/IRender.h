#pragma once
#include "../GameObject/Component/Camera.h"
#include <windows.h>
#include "IVBOManager.h"
#include "ITextureManager.h"

class IRender
{
protected:
	//Current matrices
	Float4x4 _view;
	Float4x4 _projection;
	//Screen size
	int _windowWidth, _windowHeight;
	//To hold the currently active Camera
	Camera * _activeCamera; //todo update for new component system

	IVBOManager* _vboManager;
	ITextureManager* _textureManager;
public:
	IRender();
	virtual ~IRender();

	virtual HRESULT InitWindow(RECT& rc, const char*& windowTitle) = 0;
	virtual HRESULT InitRenderer() = 0;

	void SetActiveCamera(Camera* camera);
	Int2 GetWindowSize() const;
	int GetWindowWidth() const;
	int GetWindowHeight() const;

	virtual void Update();
	virtual void Draw() = 0; //todo replace as this is for testing
	virtual bool ShouldExit() = 0;

	virtual Geometry* LoadRawGeometry(RawGeometry*& geometry);
	virtual void FreeGeometry(Geometry*& geometry);

	virtual void UpdateScreenSize(const int windowWidth, const int windowHeight);
protected:
	virtual void CleanUp() = 0;

	virtual void UpdateViewMatrix() = 0;
	virtual void UpdateProjectionMatrix();
};

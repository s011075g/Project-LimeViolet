#pragma once
#include <windows.h>
#include "IVBOManager.h"
#include "ITextureManager.h"
#include "../Components/CameraComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RenderableMeshComponent.h"
#include "../Components/MaterialComponent.h"

class IRender
{
protected:
	//Current matrices
	mutable Float4x4 _view;
	mutable Float4x4 _projection;
	//Screen size
	int _windowWidth, _windowHeight;
	//To hold the currently active Camera
	CameraComponent * _activeCamera; //todo update for new component system

	IVBOManager* _vboManager;
	ITextureManager* _textureManager;
public:
	IRender();
	virtual ~IRender();

	virtual HRESULT InitWindow(RECT& rc, const char*& windowTitle) = 0;
	virtual HRESULT InitRenderer() = 0;

	void SetActiveCamera(CameraComponent* camera);
	Int2 GetWindowSize() const;
	int GetWindowWidth() const;
	int GetWindowHeight() const;

	//Updates the window messages
	virtual void Update() = 0;

	virtual void DrawStart() const = 0;
	virtual void DrawObject(TransformComponent* transform, RenderableMeshComponent* mesh, MaterialComponent* materials) const = 0;
	virtual void DrawEnd() const = 0;

	virtual bool ShouldExit() = 0;

	virtual Geometry* LoadRawGeometry(RawGeometry*& geometry);
	virtual void FreeGeometry(Geometry*& geometry);

	virtual void UpdateScreenSize(const int windowWidth, const int windowHeight);
protected:
	virtual void CleanUp() = 0;

	virtual void UpdateViewMatrix() const = 0;
	virtual void UpdateProjectionMatrix() const;
};

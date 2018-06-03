#pragma once
#include "../GameObject/Component/Camera.h"

class IRender
{
protected:
	//Current matrices
	Float4x4 _view;
	Float4x4 _projection;
	//Screen size
	float _windowWidth, _windowHeight;
	//To hold the currently active Camera
	Camera * _activeCamera;
public:
	IRender();
	virtual ~IRender();

	void SetActiveCamera(Camera* camera);

	virtual void Update();

	virtual void UpdateScreenSize(float windowWidth, float windowHeight);
protected:
	virtual void UpdateViewMatrix() = 0;
	virtual void UpdateProjectionMatrix() = 0;
};
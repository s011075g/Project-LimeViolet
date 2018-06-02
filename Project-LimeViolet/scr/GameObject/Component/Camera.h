#pragma once
#include "IComponent.h"
#include "../../Color.h"
#include "../../Maths/MathStructs.h"

class Camera : public IComponent
{
private:
	Float4x4 _view;
	Float4x4 _projection;

	Float4 _eye;
	Float4 _at;
	Float4 _up;
	//What the background is cleared to
	Color _clearColor;
	float _fieldOfView;
	//Clipping plane
	float _near, _far;
public:
	Camera();
	~Camera();

	void Start() override;
	void Update() override;

	Float4x4 GetView() const;
	Float4x4 GetProjection() const;
	Float4x4 GetViewProjection() const;
};


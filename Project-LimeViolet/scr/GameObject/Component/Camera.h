#pragma once
#include "IComponent.h"
#include "../../Color.h"
#include "../../Maths/MathStructs.h"

class Camera : public IComponent
{
private:
	//Camera's up vector
	Float4 _up;
	//What the background is cleared to
	Color _clearColor;
	float _fieldOfView;
	//Clipping plane
	float _near, _far;
public:
	Camera(GameObject* gameObject);
	~Camera();

	void Start() override;
	void Update() override;

	void SetUp(Float3 vector);
	void SetFOV(float fov);

	float GetFOV() const;
	Float4 GetEye() const;
	Float4 GetAt() const;
	Float4 GetUp() const;
};


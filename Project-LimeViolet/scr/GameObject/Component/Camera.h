#pragma once
#include "IComponent.h"
#include "../../Maths/MathStructs.h"
#include "../../Common/Color.h"

class Camera : public IComponent
{
private:
	//Camera's up vector
	Float3 _up;
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
	void SetClippingPlanes(float near, float far);
	void SetClearColor(Color color);

	float GetFOV() const;
	float GetNear() const;
	float GetFar() const;
	Float3 GetEye() const;
	Float3 GetAt() const;
	Float3 GetUp() const;
	Color GetClearColor() const;
};


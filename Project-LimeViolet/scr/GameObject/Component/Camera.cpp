#include "Camera.h"
#include "../GameObject.h"

Camera::Camera(GameObject* gameObject)
	: IComponent(gameObject), _fieldOfView(10.0f), _near(0.1f), _far(0.2f)
{ }

Camera::~Camera()
{ }

void Camera::Start()
{ }

void Camera::Update()
{ }

void Camera::SetUp(const Float3 vector)
{
	_up = vector;
}

void Camera::SetFOV(float fov)
{
	_fieldOfView = fov;
}

void Camera::SetClippingPlanes(float near, float far)
{
	_near = near;
	_far = far;
}

void Camera::SetClearColor(Color4 color)
{
	_clearColor = color;
}

float Camera::GetFOV() const
{
	return _fieldOfView;
}

float Camera::GetNear() const
{
	return _near;
}

float Camera::GetFar() const
{
	return _far;
}

Float3 Camera::GetEye() const
{
	const Float3 direction(0.0f, 0.0f, 1.0f); //TODO Change depending on right or left handed systems
	return _gameObject->GetTransform()->GetRotation().RotateVector(direction);
}

Float3 Camera::GetAt() const
{
	return _gameObject->GetTransform()->GetPosition();
}

Float3 Camera::GetUp() const
{
	return _up;
}

Color4 Camera::GetClearColor() const
{
	return _clearColor;
}

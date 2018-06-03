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
	_up = Float4(vector, 0.0f);
}

void Camera::SetFOV(float fov)
{
	_fieldOfView = fov;
}

float Camera::GetFOV() const
{
	return _fieldOfView;
}

Float4 Camera::GetEye() const
{
	const Float3 direction(0.0f, 0.0f, 1.0f); //TODO Change depending on right or left handed systems
	return Float4(_gameObject->GetTransform()->GetRotation().RotateVector(direction), 1.0f);
}

Float4 Camera::GetAt() const
{
	return Float4(_gameObject->GetTransform()->GetPosition(), 1.0f);
}

Float4 Camera::GetUp() const
{
	return _up;
}

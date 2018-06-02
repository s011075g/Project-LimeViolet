#include "Camera.h"

Camera::Camera()
	: _fieldOfView(10.0f), _near(0.1f), _far(0.2f)
{ }

Camera::~Camera()
{ }

void Camera::Start()
{
	_eye = Float4(0.0f, 0.0f, 0.0f, 1.0f);
	_at  = Float4(0.0f, 0.0f, 0.0f, 1.0f);
	_up  = Float4(0.0f, 0.0f, 0.0f, 0.0f);
}

Float4x4 Camera::GetView() const
{
	return _view;
}

Float4x4 Camera::GetProjection() const
{
	return _projection;
}

Float4x4 Camera::GetViewProjection() const
{
	return _view * _projection;
}

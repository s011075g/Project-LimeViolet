#include "IRender.h"
#include <complex>

IRender::IRender()
{ }

IRender::~IRender()
{ }

void IRender::SetActiveCamera(Camera* camera)
{
	_activeCamera = camera;
}

void IRender::Update()
{
	UpdateViewMatrix();
}

void IRender::UpdateScreenSize(const float windowWidth, const float windowHeight)
{
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
	UpdateProjectionMatrix();
}
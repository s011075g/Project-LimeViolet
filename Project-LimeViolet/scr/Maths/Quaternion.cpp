#include "Quaternion.h"

#include <cmath>
#include <cfloat>

Quaternion::Quaternion()
	: _r(1), _i(0), _j(0), _k(0)
{ }

Quaternion::Quaternion(const Float4& rijk)
	: _r(rijk.x), _i(rijk.y), _j(rijk.z), _k(rijk.w)
{ }

Quaternion::Quaternion(const float& r, const float& i, const float& j, const float& k)
	: _r(r), _i(i), _j(j), _k(k)
{ }

Quaternion::Quaternion(const float& yaw, const float& pitch, const float& roll)
{
	const float cosYaw = std::cos(yaw * 0.5f);
	const float sinYaw = std::sin(yaw * 0.5f);
	const float cosPitch = std::cos(pitch * 0.5f);
	const float sinPitch = std::sin(pitch * 0.5f);
	const float cosRoll = std::cos(roll * 0.5f);
	const float sinRoll = std::sin(roll * 0.5f);

	_r = cosYaw * cosPitch * cosRoll - sinYaw * sinPitch * sinRoll;
	_i = sinYaw * cosPitch * cosRoll + cosYaw * sinPitch * sinRoll;
	_j = cosYaw * sinPitch * cosRoll - sinYaw * cosPitch * sinRoll;
	_k = sinYaw * sinPitch * cosRoll + cosYaw * cosPitch * sinRoll;
}

Quaternion::~Quaternion()
{ }

void Quaternion::Normalise()
{
	float d = _r * _r + _i * _i + _j * _j + _k * _k;

	if (d < FLT_EPSILON)
	{
		_r = 1.0f;
		return;
	}
	d = 1.0f / sqrtf(d);
	_r *= d;
	_i *= d;
	_j *= d;
	_k *= d;
}

void Quaternion::AddScaledVector(const Float3& v, float s)
{
	Quaternion q(0, v.x * s, v.y * s, v.z * s);
	q *= *this;
	_r += q._r * 0.5f;
	_i += q._i * 0.5f;
	_j += q._j * 0.5f;
	_k += q._k * 0.5f;
}

void Quaternion::RotateByVector(const Float3& v)
{
	const Quaternion q(0, v.x, v.y, v.z);
	*this *= q;
}

void Quaternion::operator*=(const Quaternion& r)
{
	const Quaternion q = *this; //Copy current values
	_r = q._r * r._r - q._i * r._i - q._j * r._j - q._k * r._k;
	_i = q._r * r._i + q._i * r._r + q._j * r._k - q._k * r._j;
	_j = q._r * r._j + q._j * r._r + q._k * r._i - q._i * r._k;
	_k = q._r * r._k + q._k * r._r + q._i * r._j - q._j * r._i;
}

Float3 Quaternion::RotateVector(const Float3& v) const
{
	const Float3 u(_i, _j, _k);
	const float dot = u.Dot(v);
	return u * dot * 2 + v * (_r * _r - dot) + v.Cross(u) * _r * 2;
}

Float3x3 Quaternion::GetRotationMatrix() const
{
	Float3x3 result;

	result.m11 = 1 - 2 * _j * _j - 2 * _k * _k;
	result.m12 = 2 * _i * _j - 2 * _r * _k;
	result.m13 = 2 * _i * _k + 2 * _r * _j;

	result.m21 = 2 * _i * _j + 2 * _r * _k;
	result.m22 = 1 - 2 * _i * _i - 2 * _k * _k;
	result.m23 = 2 * _j * _k - 2 * _r * _i;

	result.m31 = 2 * _i * _k - 2 * _r * _j;
	result.m32 = 2 * _j * _k + 2 * _r * _i;
	result.m33 = 1 - 2 * _i * _i - 2 * _j * _j;

	return result;
}

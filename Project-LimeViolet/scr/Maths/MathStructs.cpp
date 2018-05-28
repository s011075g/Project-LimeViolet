#include "MathStructs.h"

#include <complex>

constexpr Float2::Float2(const float& x, const float& y)
	: x(x), y(y)
{ }

constexpr Float3::Float3(const float& x, const float& y, const float& z)
	: x(x), y(y), z(z)
{ }

constexpr Float4::Float4(const float& x, const float& y, const float& z, const float& w)
	: x(x), y(y), z(z), w(w)
{ }

Float2& Float2::operator=(const Float2& r)
{
	x = r.x;
	y = r.y;
	return *this;
}

Float3& Float3::operator=(const Float3& r)
{
	x = r.x;
	y = r.y;
	z = r.z;
	return *this;
}

Float3 Float3::operator*(const Float3& r) const
{
	return Float3(x * r.x, y * r.y, z * r.z);
}

Float3 Float3::operator*(const float& r) const
{
	return Float3(x * r, y * r, z * r);
}

Float3 Float3::operator+(const Float3& r) const
{
	return Float3(x + r.x, y + r.y, z + r.z);
}

Float4& Float4::operator=(const Float4& r)
{
	x = r.x;
	y = r.y;
	z = r.z;
	w = r.w;
	return *this;
}

constexpr float Float2::Magnitude() const
{
	return x * x + y * y;
}

constexpr float Float2::MagnitudeSqrt() const
{
	return std::sqrt(x * x + y * y);
}

constexpr float Float3::Magnitude() const
{
	return x * x + y * y + z * z;
}

constexpr float Float3::MagnitudeSqrt() const
{
	return std::sqrt(x * x + y * y + z * z);
}

constexpr float Float4::Magnitude() const
{
	return x * x + y * y + z * z + w * w;
}

constexpr float Float4::MagnitudeSqrt() const
{
	return std::sqrt(x * x + y * y + z * z + w * w);
}

void Float2::Normalize()
{
	const float mag = MagnitudeSqrt();
	x /= mag;
	y /= mag;
}

void Float3::Normalize()
{
	const float mag = MagnitudeSqrt();
	x /= mag;
	y /= mag;
	z /= mag;
}

void Float4::Normalize()
{
	const float mag = MagnitudeSqrt();
	x /= mag;
	y /= mag;
	z /= mag;
	w /= mag;
}

constexpr float Float2::Dot(const Float2& r) const
{
	return x * r.x + y * r.y;
}

constexpr float Float3::Dot(const Float3& r) const
{
	return x * r.x + y * r.y + z * r.z;
}

constexpr float Float4::Dot(const Float4& r) const
{
	return x * r.x + y * r.y + z * r.z + w * r.w;
}

constexpr Float3 Float3::Cross(const Float3& r) const
{
	return Float3(y * r.z - z * r.y,
				  z * r.x - x * r.z,
				  x * r.y - y * r.x);
}

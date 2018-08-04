#include "MathStructs.h"
#include <complex>

Float2::Float2(const float x, const float y)
	: x(x), y(y)
{ }

Float3::Float3(const float x, const float y, const float z)
	: x(x), y(y), z(z)
{ }

Float3::Float3(float xyz[3])
	: x(xyz[0]), y(xyz[1]), z(xyz[2])
{ }

Float4::Float4(const float x, const float y, const float z, const float w)
	: x(x), y(y), z(z), w(w)
{ }

Float4::Float4(const Float3 xyz, const float w)
	: x(xyz.x), y(xyz.y), z(xyz.z), w(w)
{ }

Float4::Float4(float xyzw[4])
	: x(xyzw[0]), y(xyzw[1]), z(xyzw[2]), w(xyzw[3])
{ }

Float2& Float2::operator=(const Float2& r)
{
	x = r.x;
	y = r.y;
	return *this;
}

Float2 Float2::operator*(const Float2& r) const
{
	return Float2(x * r.x, y * r.y);
}

Float2& Float2::operator*=(const Float2& r)
{
	x *= r.x;
	y *= r.y;
	return *this;
}

Float2 Float2::operator*(const float& r) const
{
	return Float2(x * r, y * r);
}

Float2& Float2::operator*=(const float& r)
{
	x *= r;
	y *= r;
	return *this;
}

Float2 Float2::operator/(const Float2& r) const
{
	return Float2(x * r.x, y * r.y);
}

Float2& Float2::operator/=(const Float2& r)
{
	x /= r.x;
	y /= r.y;
	return *this;
}

Float2 Float2::operator/(const float& r) const
{
	return Float2(x / r, y / r);
}

Float2& Float2::operator/=(const float& r)
{
	x /= r;
	y /= r;
	return *this;
}

Float2 Float2::operator+(const Float2& r) const
{
	return Float2(x + r.x, y + r.y);
}

Float2& Float2::operator+=(const Float2& r)
{
	x += r.x;
	y += r.y;
	return *this;
}

Float2 Float2::operator-(const Float2& r) const
{
	return Float2(x -  r.x, y - r.y);
}

Float2& Float2::operator-=(const Float2& r)
{
	x -= r.x;
	y -= r.y;
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

Float3& Float3::operator*=(const Float3& r)
{
	x *= r.x;
	y *= r.y;
	z *= r.z;
	return *this;
}

Float3 Float3::operator*(const float& r) const
{
	return Float3(x * r, y * r, z * r);
}

Float3& Float3::operator*=(const float& r)
{
	x *= r;
	y *= r;
	z *= r;
	return *this;
}

Float3 Float3::operator/(const Float3& r) const
{
	return Float3(x / r.x, y / r.y, z / r.z);
}

Float3& Float3::operator/=(const Float3& r)
{
	x *= r.x;
	y *= r.y;
	z *= r.z;
	return *this;
}

Float3 Float3::operator/(const float& r) const
{
	return Float3(x / r, y / r, z / r);
}

Float3& Float3::operator/=(const float& r)
{
	x /= r;
	y /= r;
	z /= r;
	return *this;
}

Float3 Float3::operator+(const Float3& r) const
{
	return Float3(x + r.x, y + r.y, z + r.z);
}

Float3& Float3::operator+=(const Float3& r)
{
	x += r.x;
	y += r.y;
	z += r.z;
	return *this;
}

Float3 Float3::operator-(const Float3& r) const
{
	return Float3(x - r.x, y - r.y, z - r.z);
}

Float3& Float3::operator-=(const Float3& r)
{
	x -= r.x;
	y -= r.y;
	z -= r.z;
	return *this;
}

Float4& Float4::operator=(const Float4& r)
{
	x = r.x;
	y = r.y;
	z = r.z;
	w = r.w;
	return *this;
}

Float4 Float4::operator*(const Float4& r) const
{
	return Float4(x * r.x, y * r.y, z * r.z, w*  r.w);
}

Float4& Float4::operator*=(const Float4& r)
{
	x *= r.x;
	y *= r.y;
	z *= r.z;
	w *= r.w;
	return *this;
}

Float4 Float4::operator*(const float& r) const
{
	return Float4(x * r, y * r, z * r, w * r);
}

Float4& Float4::operator*=(const float& r)
{
	x *= r;
	y *= r;
	z *= r;
	w *= r;
	return *this;
}

Float4 Float4::operator/(const Float4& r) const
{
	return Float4(x / r.x, y / r.y, z / r.z, w / r.w);
}

Float4& Float4::operator/=(const Float4& r)
{
	x /= r.x;
	y /= r.y;
	z /= r.z;
	w /= r.w;
	return *this;
}

Float4 Float4::operator/(const float& r) const
{
	return Float4(x / r, y / r, z / r, w / r);
}

Float4& Float4::operator/=(const float& r)
{
	x /= r;
	y /= r;
	z /= r;
	w /= r;
	return *this;
}

Float4 Float4::operator+(const Float4& r) const
{
	return Float4(x + r.x, y + r.y, z + r.z, w + r.w);
}

Float4& Float4::operator+=(const Float4& r)
{
	x += r.x;
	y += r.y;
	z += r.z;
	w += r.w;
	return *this;
}

Float4 Float4::operator-(const Float4& r) const
{
	return Float4(x - r.x, y - r.y, z - r.z, w - r.w);
}

Float4& Float4::operator-=(const Float4& r)
{
	x -= r.x;
	y -= r.y;
	z -= r.z;
	w -= r.w;
	return *this;
}

float Float2::Magnitude() const
{
	return std::sqrt(x * x + y * y);
}

float Float2::MagnitudeSq() const
{
	return x * x + y * y;
}

float Float3::Magnitude() const
{
	return std::sqrt(x * x + y * y + z * z);
}

float Float3::MagnitudeSq() const
{
	return x * x + y * y + z * z;
}

float Float4::Magnitude() const
{
	return std::sqrt(x * x + y * y + z * z + w * w);
}

float Float4::MagnitudeSq() const
{
	return x * x + y * y + z * z + w * w;
}
#define tolerance 0.0001f

void Float2::Normalize()
{
	const float mag = Magnitude();
	if (mag < tolerance)
		throw;
	x /= mag;
	y /= mag;
}

void Float3::Normalize()
{
	const float mag = Magnitude();
	if (mag < tolerance)
		throw;
	x /= mag;
	y /= mag;
	z /= mag;
}

void Float4::Normalize()
{
	const float mag = Magnitude();
	if (mag < tolerance)
		throw;
	x /= mag;
	y /= mag;
	z /= mag;
	w /= mag;
}

float Float2::Dot(const Float2& r) const
{
	return x * r.x + y * r.y;
}

float Float3::Dot(const Float3& r) const
{
	return x * r.x + y * r.y + z * r.z;
}

float Float4::Dot(const Float4& r) const
{
	return x * r.x + y * r.y + z * r.z + w * r.w;
}

Int2::Int2(int x, int y)
	: x(x), y(y)
{ }


Float3 Float3::Cross(const Float3& r) const
{
	return Float3(y * r.z - z * r.y,
				  z * r.x - x * r.z,
				  x * r.y - y * r.x);
}

Float3x3::Float3x3(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33)
	: m11(m11), m12(m12), m13(m13), m21(m21), m22(m22), m23(m23), m31(m31), m32(m32), m33(m33)
{ }

float Float3x3::operator()(const size_t row, const size_t column) const
{
	return m[row][column];
}

float& Float3x3::operator()(const size_t row, const size_t column)
{
	return m[row][column];
}

Float4x4::Float4x4(float value)
	: m11(value), m12(value), m13(value), m14(value), m21(value), m22(value), m23(value), m24(value), m31(value), m32(value), m33(value), m34(value), m41(value), m42(value), m43(value), m44(value)
{ }

Float4x4::Float4x4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
	: m11(m11), m12(m12), m13(m13), m14(m14), m21(m21), m22(m22), m23(m23), m24(m24), m31(m31), m32(m32), m33(m33), m34(m34), m41(m41), m42(m42), m43(m43), m44(m44)
{ }

Float4x4::Float4x4(const Float3x3 m)
	: m11(m.m11), m12(m.m12), m13(m.m13), m14(0.0f), m21(m.m21), m22(m.m22), m23(m.m23), m24(0.0f), m31(m.m31), m32(m.m32), m33(m.m33), m34(0.0f), m41(0.0f), m42(0.0f), m43(0.0f), m44(0.0f)
{ }

float Float4x4::operator()(const size_t row, const size_t column) const
{
	return m[row][column];
}

float& Float4x4::operator()(const size_t row, const size_t column)
{
	return m[row][column];
}

Float4x4 Float4x4::operator*(const Float4x4& r) const
{
	Float4x4 result;
	result.m11 = m11 * r.m11 + m12 * r.m21 + m13 * r.m31 + m14 * r.m41;
	result.m12 = m11 * r.m12 + m12 * r.m22 + m13 * r.m32 + m14 * r.m42;
	result.m13 = m11 * r.m13 + m12 * r.m23 + m13 * r.m33 + m14 * r.m43;
	result.m14 = m11 * r.m14 + m12 * r.m24 + m13 * r.m34 + m14 * r.m44;

	result.m21 = m21 * r.m11 + m22 * r.m21 + m23 * r.m31 + m24 * r.m41;
	result.m22 = m21 * r.m12 + m22 * r.m22 + m23 * r.m32 + m24 * r.m42;
	result.m23 = m21 * r.m13 + m22 * r.m23 + m23 * r.m33 + m24 * r.m43;
	result.m24 = m21 * r.m14 + m22 * r.m24 + m23 * r.m34 + m24 * r.m44;

	result.m31 = m31 * r.m11 + m32 * r.m21 + m33 * r.m31 + m34 * r.m41;
	result.m32 = m31 * r.m12 + m32 * r.m22 + m33 * r.m32 + m34 * r.m42;
	result.m33 = m31 * r.m13 + m32 * r.m23 + m33 * r.m33 + m34 * r.m43;
	result.m34 = m31 * r.m14 + m32 * r.m24 + m33 * r.m34 + m34 * r.m44;

	result.m41 = m41 * r.m11 + m42 * r.m21 + m43 * r.m31 + m44 * r.m41;
	result.m42 = m41 * r.m12 + m42 * r.m22 + m43 * r.m32 + m44 * r.m42;
	result.m43 = m41 * r.m13 + m42 * r.m23 + m43 * r.m33 + m44 * r.m43;
	result.m44 = m41 * r.m14 + m42 * r.m24 + m43 * r.m34 + m44 * r.m44;
	return result;
}

Float4x4 Float4x4::ProjectionMatrix(const float fov, const float aspect, const float near, const float far, const bool leftHanded)
{
	Float4x4 result = Float4x4(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	float oneOverDepth = 1 / (far - near);

	result.m22 = 1 / tan(0.5f * fov);
	result.m11 = (leftHanded ? 1 : -1) * result.m22 / aspect;
	result.m33 = far * oneOverDepth;
	result.m43 = (-far * near) * oneOverDepth;
	result.m34 = 1;
	return result;
}

Float4x4 Float4x4::Identity()
{
	Float4x4 result;
	result.m11 = 1.0f;
	result.m22 = 1.0f;
	result.m33 = 1.0f;
	result.m44 = 1.0f;
	return result;
}

UShort3::UShort3(const unsigned short x, const unsigned short y, const unsigned short z)
	: x(x), y(y), z(z)
{ }

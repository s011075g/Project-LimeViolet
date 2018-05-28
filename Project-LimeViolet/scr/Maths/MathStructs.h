#pragma once
//Reference DirectXMath.h Copyright Microsoft Corporation

struct Float2
{
	float x, y;

	Float2() = default;
	constexpr Float2(const float& x, const float& y);

	Float2& operator= (const Float2& right);

	void Normalize();

	constexpr float Magnitude() const;
	constexpr float MagnitudeSqrt() const;

	constexpr float Dot(const Float2& right) const;
};

struct Float3
{
	float x, y, z;

	Float3() = default;
	constexpr Float3(const float& x, const float& y, const float& z);

	Float3& operator= (const Float3& right);

	Float3 operator* (const Float3& right) const;
	Float3 operator* (const float& right) const;

	Float3 operator+ (const Float3& right) const;

	void Normalize();

	constexpr float Magnitude() const;
	constexpr float MagnitudeSqrt() const;

	constexpr float Dot(const Float3& right) const;

	constexpr Float3 Cross(const Float3& right) const;
};

struct Float4
{
	float x, y, z, w;

	Float4() = default;
	constexpr Float4(const float& x, const float& y, const float& z, const float& w);

	Float4& operator= (const Float4& right);

	void Normalize();

	constexpr float Magnitude() const;
	constexpr float MagnitudeSqrt() const;

	constexpr float Dot(const Float4& right) const;
};

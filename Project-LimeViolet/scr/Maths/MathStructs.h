#pragma once
//Reference DirectXMath.h Copyright Microsoft Corporation

typedef float Float;

struct Float2
{
	float x, y;

	Float2() = default;
	Float2(float x, float y);

	Float2& operator= (const Float2& right);

	void Normalize();

	constexpr float Magnitude() const;
	float MagnitudeSqrt() const;

	float Dot(const Float2& right) const;
};

struct Float3
{
	float x, y, z;

	Float3() = default;
	Float3(float x, float y, float z);

	Float3& operator= (const Float3& right);

	Float3 operator* (const Float3& right) const;
	Float3 operator* (const float& right) const;

	Float3 operator+ (const Float3& right) const;
	Float3& operator+=(const Float3& right);

	Float3 operator- (const Float3& right) const;

	void Normalize();

	constexpr float Magnitude() const;
	float MagnitudeSqrt() const;

	float Dot(const Float3& right) const;

	Float3 Cross(const Float3& right) const;
};

struct Float4
{
	float x, y, z, w;

	Float4() = default;
	Float4(float x, float y, float z, float w);
	Float4(Float3 xyz, float w);

	Float4& operator= (const Float4& right);

	void Normalize();

	constexpr float Magnitude() const;
	float MagnitudeSqrt() const;

	float Dot(const Float4& right) const;
};

struct UShort3
{
	unsigned short x, y, z;
	UShort3() = default;
	UShort3(unsigned short x, unsigned short y, unsigned short z);
};

struct Float3x3
{
	union
    {
        struct
        {
            float m11, m12, m13;
            float m21, m22, m23;
            float m31, m32, m33;
        };
        float m[3][3];
    };

	Float3x3() = default;
	explicit Float3x3(float m11, float m12, float m13,
					  float m21, float m22, float m23,
					  float m31, float m32, float m33);

	float operator() (const size_t row, const size_t column) const;
	float& operator() (const size_t row, const size_t column);

};

struct Float4x4
{
	union
    {
        struct
        {
            float m11, m12, m13, m14;
            float m21, m22, m23, m24;
            float m31, m32, m33, m34;
            float m41, m42, m43, m44;
        };
        float m[4][4];
    };

	Float4x4() = default;
	explicit Float4x4(float m11, float m12, float m13, float m14,
					  float m21, float m22, float m23, float m24,
					  float m31, float m32, float m33, float m34,
					  float m41, float m42, float m43, float m44);
	explicit Float4x4(Float3x3 float3x3);

	float operator() (const size_t row, const size_t column) const;
	float& operator() (const size_t row, const size_t column);

	Float4x4 operator* (const Float4x4& right) const;

	//Returns an identity matrix
	static Float4x4 Identity();
};
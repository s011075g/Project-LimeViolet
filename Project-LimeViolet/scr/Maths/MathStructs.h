#pragma once
//Reference DirectXMath.h Copyright Microsoft Corporation

struct Float2
{
	union
	{
		struct 
		{
			float x, y;
		};
		float xy[2];
	};

	Float2() = default;
	Float2(float x, float y);

	Float2& operator= (const Float2& right);

	Float2 operator* (const Float2& right) const;
	Float2& operator*=(const Float2& right);

	Float2 operator* (const float& right) const;
	Float2& operator*=(const float &right);

	Float2 operator/ (const Float2& right) const;
	Float2& operator/=(const Float2 &right);

	Float2 operator/ (const float& right) const;
	Float2& operator/=(const float &right);

	Float2 operator+ (const Float2& right) const;
	Float2& operator+=(const Float2& right);

	Float2 operator- (const Float2& right) const;
	Float2& operator-=(const Float2& right);

	void Normalize();

	//Returns the length of the vector
	float Magnitude() const;
	//Returns the length of the vector squared
	float MagnitudeSq() const;

	float Dot(const Float2& right) const;
};

struct Float3
{
	union 
	{
		struct
		{
			float x, y, z;
		};
		float xyz[3];
	};

	Float3() = default;
	Float3(float x, float y, float z);
	explicit Float3(float xyz[3]);

	Float3& operator= (const Float3& right);

	Float3 operator* (const Float3& right) const;
	Float3& operator*=(const Float3& right);

	Float3 operator* (const float& right) const;
	Float3& operator*=(const float &right);

	Float3 operator/ (const Float3& right) const;
	Float3& operator/=(const Float3 &right);

	Float3 operator/ (const float& right) const;
	Float3& operator/=(const float &right);

	Float3 operator+ (const Float3& right) const;
	Float3& operator+=(const Float3& right);

	Float3 operator- (const Float3& right) const;
	Float3& operator-=(const Float3& right);

	void Normalize();

	//Returns the length of the vector
	float Magnitude() const;
	//Returns the length of the vector squared
	float MagnitudeSq() const;

	float Dot(const Float3& right) const;

	Float3 Cross(const Float3& right) const;
};

struct Float4
{
	union
	{
		struct
		{
			float x, y, z, w;
		};
		float xyzw[4];
	};

	Float4() = default;
	Float4(float x, float y, float z, float w);
	Float4(Float3 xyz, float w);
	explicit Float4(float xyzw[4]);

	Float4& operator= (const Float4& right);

	Float4 operator* (const Float4& right) const;
	Float4& operator*=(const Float4& right);

	Float4 operator* (const float& right) const;
	Float4& operator*=(const float &right);

	Float4 operator/ (const Float4& right) const;
	Float4& operator/=(const Float4 &right);

	Float4 operator/ (const float& right) const;
	Float4& operator/=(const float &right);

	Float4 operator+ (const Float4& right) const;
	Float4& operator+=(const Float4& right);

	Float4 operator- (const Float4& right) const;
	Float4& operator-=(const Float4& right);

	void Normalize();

	//Returns the length of the vector
	float Magnitude() const;
	//Returns the length of the vector squared
	float MagnitudeSq() const;

	float Dot(const Float4& right) const;
};

struct Int2
{
	union 
	{
		struct 
		{
			int x, y;
		};
		int xy[2];
	};
	Int2() = default;
	Int2(int x, int y);
};

struct UShort3
{
	union
	{
		struct
		{
			unsigned short x, y, z;
		};
		unsigned short xyz[3];
	};
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

	//Compute Projection Matrix;
	static Float4x4 ProjectionMatrix(float fov, float aspect, float near, float far, bool leftHanded = true);
	
	//Returns an identity matrix
	static Float4x4 Identity();
};
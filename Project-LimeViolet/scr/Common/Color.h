#pragma once
#include "../Maths/MathStructs.h"
struct Color4
{
	union
	{
		struct
		{
			float r, g, b, a;
		};
		float rgba[4];
	};
	Color4() = default;
	Color4(const float& r, const float& g, const float& b, const float& a);
	Color4(const Float4& color);
	Color4(const Float3& color, const float& transparency);
};

inline Color4::Color4(const float& r, const float& g, const float& b, const float& a)
	: r(r), g(g), b(b), a(a)
{ }

inline Color4::Color4(const Float4& color)
	: r(color.x), g(color.y), b(color.z), a(color.w)
{}

inline Color4::Color4(const Float3& color, const float& transparency)
	: r(color.x), g(color.y), b(color.z), a(transparency)
{ }

struct Color3
{
	union
	{
		struct
		{
			float r, g, b;
		};
		float rgb[3];
	};
	Color3() = default;
	Color3(const float& r, const float& g, const float& b);
	Color3(const Float3& color);
};

inline Color3::Color3(const float& r, const float& g, const float& b)
	: r(r), g(g), b(b)
{ }

inline Color3::Color3(const Float3& color)
	: r(color.x), g(color.y), b(color.z)
{ }

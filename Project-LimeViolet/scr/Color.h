#pragma once
struct Color
{
	union
	{
		struct
		{
			float r, g, b, a;
		};
		float rgba[4];
	};
	Color() = default;
};
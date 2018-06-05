#pragma once
//Holds 4 values, assigned to R,G,B,A
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
#pragma once
#include "MathStructs.h"

class Quaternion
{
private:
	float _r; //Real
	float _i, _j, _k; //Complex
public:
	Quaternion();
	Quaternion(const float& r, const float& i, const float& j, const float& k);
	Quaternion(const float& yaw, const float& pitch, const float& roll);
	
	~Quaternion();

	void Normalise();

	void AddScaledVector(const Float3& vector, float scale);
	
	void RotateByVector(const Float3& vector);

	void operator *=(const Quaternion& right);

	Float3 RotateVector(const Float3& vector) const;

	Float3x3 GetRotationMatrix() const;
};


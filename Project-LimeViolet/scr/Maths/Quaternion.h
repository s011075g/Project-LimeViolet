#pragma once
#include "MathStructs.h"

class Quaternion
{
private:
	float _r; //Real
	float _i, _j, _k; //Complex
public:
	Quaternion();
	explicit Quaternion(const Float4& rijk);
	Quaternion(const float& r, const float& i, const float& j, const float& k);
	Quaternion(const float& yaw, const float& pitch, const float& roll);
	
	~Quaternion();

	//Normalise the axis the Quaternion uses
	void Normalise();

	void AddScaledVector(const Float3& vector, float scale);

	//Rotate this quaternion by a vector
	void RotateByVector(const Float3& vector);

	//todo add in better math operators
	void operator *=(const Quaternion& right);

	//Rotate Vector by this Quaternion
	Float3 RotateVector(const Float3& vector) const;

	//Get a rotation matrix from the quaternion
	Float3x3 GetRotationMatrix() const;
};
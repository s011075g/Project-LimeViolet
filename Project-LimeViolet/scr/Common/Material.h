#pragma once
#include "../Maths/MathStructs.h"

struct MaterialValues //48 bytes, 4 byte alligned
{
	MaterialValues() = default;

	Float4 ambient; //16
	Float4 diffuse; //16
	Float3 specular; //12
	float specularPower; //4
};

class Material
{
private:
	MaterialValues * _material;
	void* _textureDiffuse;
	void* _textureNormal;
	void* _textureSpecular;
public:
	Material() = default;
	explicit Material(MaterialValues *& material);

	void SetTextureDiffuse(void * textre);
	void SetTextureNormal(void *textre);
	void SetTextureSpecular(void * textre);

	MaterialValues * GetMaterial() const;
	void * GetTextureDiffuse() const;
	void * GetTextureNormal() const;
	void * GetTextureSpecular() const;

};
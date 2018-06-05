#pragma once
#include "../Maths/MathStructs.h"

struct MaterialValues //48 bytes, 4 byte alligned
{
	MaterialValues() = default;

	Float4 ambient; //16
	Float3 diffuse; //12
	float transparency; //4
	Float3 specular; //12
	float specularPower; //4
};

class Material //Material holds information for what the triangles will look like once rendered
{
private:
	MaterialValues * _material; //Holds the colors for the material
	void* _textureDiffuse; 
	void* _textureNormal;
	void* _textureSpecular;
public:
	Material() = default;
	explicit Material(MaterialValues *& material);

	//Set a texture, possible for it to hold any pointer, so I can give it a char* which I can then take to load in that image and return a pointer in its place
	void SetTextureDiffuse(void * textre);
	void SetTextureNormal(void *textre);
	void SetTextureSpecular(void * textre);

	MaterialValues * GetMaterial() const;
	void * GetTextureDiffuse() const;
	void * GetTextureNormal() const;
	void * GetTextureSpecular() const;

};
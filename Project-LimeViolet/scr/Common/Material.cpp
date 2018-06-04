#include "Material.h"

Material::Material(MaterialValues *& material)
	: _material(material), _textureDiffuse(nullptr), _textureNormal(nullptr), _textureSpecular(nullptr)
{ }

void Material::SetTextureDiffuse(void * texture)
{
	_textureDiffuse = texture;
}

void Material::SetTextureNormal(void * texture)
{
	_textureNormal = texture;
}

void Material::SetTextureSpecular(void * texture)
{
	_textureSpecular = texture;
}

MaterialValues* Material::GetMaterial() const
{
	return _material;
}

void* Material::GetTextureDiffuse() const
{
	return _textureDiffuse;
}

void* Material::GetTextureNormal() const
{
	return _textureNormal;
}

void* Material::GetTextureSpecular() const
{
	return _textureSpecular;
}

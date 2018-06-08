#pragma once
class IShaderManager
{
public:
	IShaderManager();
	virtual ~IShaderManager();

	void LoadShader(const char*);
	void* GetShader() const;

};
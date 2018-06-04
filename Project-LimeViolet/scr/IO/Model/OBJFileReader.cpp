#include "OBJFileReader.h"
#include <fstream>
#include <string>

OBJFileReader::~OBJFileReader()
{ }

OBJFileReader* OBJFileReader::Instance()
{
	static OBJFileReader instance;
	return &instance;
}

void OBJFileReader::ReadFile(const char* fileLocation)
{
	if (_stricmp(&fileLocation[strlen(fileLocation) - 3], "obj") != 0)
		return;
	Obj* obj = LoadObj(fileLocation);
	if (!obj)
		return; //TODO add suitable exception  
	std::vector<Mtl*> materials;
	for (auto c : obj->materialPaths) //Load all MTL files
		materials.push_back(LoadMtl(c));
	
	//Pack Data in to VBO
	//Calculate Tangents if needed
	//Clean up
	delete obj;
	for (auto m : materials)
		delete m;
	return; //TODO return data ready for shader to push it to the VRAM
}

OBJFileReader::Obj* OBJFileReader::LoadObj(const char* fileLocation) const
{
	std::ifstream stream;
	stream.open(fileLocation);
	if(!stream.good())
		return nullptr;
	Obj* data(nullptr);
	try
	{
		std::string line;
		std::string location = std::string(fileLocation).substr(0, location.find_last_of("/") + 1);
		Float3 value;
		std::string material;
		data = new Obj();
		while (getline(stream, line))
		{
			if (line.empty())
				continue;
			const char c = line[0];
			if (c == '#' || c == '\n' || c == 'o' || c == ' ')
				continue;
			if (sscanf_s(line.c_str(), "v %f %f %f", &value.x, &value.y, &value.z) == 3)
				data->vertices.push_back(value);
			else if (sscanf_s(line.c_str(), "vn %f %f %f", &value.x, &value.y, &value.z) == 3)
				data->normals.push_back(value);
			else if (sscanf_s(line.c_str(), "vt %f %f", &value.x, &value.y) == 2)
				data->uvs.push_back(Float2(value.x, value.y));
			else if (c == 'f')
				ReadFace(line.c_str(), material.c_str(), data);
			else if (c == 'm' && line.substr(0, 6).compare("mtllib"))
				material = location + line.substr(8).c_str();
		}
	}
	catch(std::exception e)
	{
		delete data;
		stream.close(); //Safe closing of the file
		throw;
	}
	stream.close();
	return data;
}

OBJFileReader::Mtl* OBJFileReader::LoadMtl(const char* fileLocation) const
{
	std::ifstream stream;
	stream.open(fileLocation);
	if (!stream.good())
		return nullptr;
	Mtl* data(nullptr);
	try 
	{
		data = new Mtl();
		std::string line;
		const char* material(nullptr);
		Float3 value;
		while (getline(stream, line))
		{
			if (line.empty())
				continue;
			const char c = line[0];
			if (c == '#' || c == '\n' || c == ' ')
				continue;
			if (sscanf_s(line.c_str(), "Kd %f %f %f", value.x, value.y, value.z) == 3)
				data->diffuse[material] = value;
			else if (sscanf_s(line.c_str(), "Ka %f %f %f", value.x, value.y, value.z) == 3)
				data->ambient[material] = value;
			else if (sscanf_s(line.c_str(), "Ks %f %f %f", value.x, value.y, value.z) == 3)
				data->specular[material] = value;
			else if (sscanf_s(line.c_str(), "Ns %f", value.x) == 1)
				data->shine[material] = value.x;
			else if (sscanf_s(line.c_str(), "d %f", value.x) == 1)
				data->transparency[material] = value.x;
			else if (c == 'n' && line.substr(0, 6).compare("newmtl")) //The first compare is to decrease the amount of substrings needed
				material = line.substr(8).c_str();
		}
	}
	catch(std::exception e)
	{
		delete data;
		stream.close();
		throw;
	}
	stream.close();
	return data;
}

void OBJFileReader::ReadFace(const char* line, const char* material, Obj*& data)
{
	UShort3 vertex;
	UShort3 uv;
	UShort3 normal;
	if(sscanf_s(line, "f %hu %hu %hu", &vertex.x, &vertex.y, &vertex.z) == 3)
	{
		data->indicesVertices[material].push_back(vertex.x - 1);
		data->indicesVertices[material].push_back(vertex.y - 1);
		data->indicesVertices[material].push_back(vertex.z - 1);
	}
	else if (sscanf_s(line, "f %hu/%hu %hu/%hu %hu/%hu", &vertex.x, &uv.x, &vertex.y, &uv.y, &vertex.z, &uv.z) == 6)
	{
		data->indicesVertices[material].push_back(vertex.x - 1);
		data->indicesVertices[material].push_back(vertex.y - 1);
		data->indicesVertices[material].push_back(vertex.z - 1);
		data->indicesUvs[material].push_back(uv.x - 1);
		data->indicesUvs[material].push_back(uv.y - 1);
		data->indicesUvs[material].push_back(uv.z - 1);
	}
	else if (sscanf_s(line, "f %hu//%hu %hu//%hu %hu//%hu", &vertex.x, &normal.x, &vertex.y, &normal.y, &vertex.z, &normal.z) == 6)
	{
		data->indicesVertices[material].push_back(vertex.x - 1);
		data->indicesVertices[material].push_back(vertex.y - 1);
		data->indicesVertices[material].push_back(vertex.z - 1);
		data->indicesNormals[material].push_back(normal.x - 1);
		data->indicesNormals[material].push_back(normal.y - 1);
		data->indicesNormals[material].push_back(normal.z - 1);
	}
	else if (sscanf_s(line, "f %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu", &vertex.x, &uv.x, &normal.x, &vertex.y, &uv.y, &normal.y, &vertex.z, &uv.z, &normal.z) == 9)
	{
		data->indicesVertices[material].push_back(vertex.x - 1);
		data->indicesVertices[material].push_back(vertex.y - 1);
		data->indicesVertices[material].push_back(vertex.z - 1);
		data->indicesUvs[material].push_back(uv.x - 1);
		data->indicesUvs[material].push_back(uv.y - 1);
		data->indicesUvs[material].push_back(uv.z - 1);
		data->indicesNormals[material].push_back(normal.x - 1);
		data->indicesNormals[material].push_back(normal.y - 1);
		data->indicesNormals[material].push_back(normal.z - 1);
	}
}

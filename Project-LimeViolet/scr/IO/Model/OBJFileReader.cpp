#include "OBJFileReader.h"
#include <fstream>
#include <string>
#include <algorithm>
#include "../../Common/Material.h"

bool OBJFileReader::CompareCharValues::operator()(const char* left, const char* right) const
{
	return std::strcmp(left, right) < 0;
}

OBJFileReader::Packed::Packed(Float3 vertex, Float3 normal, Float2 uv)
	: vertex(vertex), normal(normal), uv(uv), tangent(Float4(0,0,0,0))
{ }

bool OBJFileReader::Packed::operator<(const Packed& right) const
{
	return memcmp(this, &right, sizeof(Packed)) > 0;
}

OBJFileReader* OBJFileReader::Instance()
{
	static OBJFileReader instance;
	return &instance;
}

Geometry* OBJFileReader::ReadFile(const char* fileLocation)
{
	if (_stricmp(&fileLocation[strlen(fileLocation) - 3], "obj") != 0)
		return nullptr;
	Obj* obj = LoadObj(fileLocation);
	if (!obj)
		return nullptr;
	std::vector<Mtl*> mtls;
	for (auto c : obj->materialPaths) //Load all MTL files
		mtls.push_back(LoadMtl(c));
	//Pack Data in single indices
	std::map<unsigned short, std::vector<ObjectVertex>> vertex;
	std::map<unsigned short, std::vector<unsigned short>> indices;
	std::vector<const char*> material;
	PackData(obj, vertex, indices, material);
	//Clean up
	delete obj;
	//Calculate Tangents if needed
	if(mtls.size() != 0)
	{
		bool tangents = false;
		for (auto m : mtls)
			if (m->materials.size() != 0)
			{
				tangents = true;
				break;
			}
		if(tangents)
			CalculateTangents(vertex, indices, material);
	}
	//Create Materials that fit
	std::vector<Material*> materials;
	for (auto i : mtls)
	{
		for (auto j : material)
		{
			auto data = i->materials.find(j);
			if(data == i->materials.end())
				break;
			material.erase(std::remove(material.begin(), material.end(), j), material.end());
			MaterialValues* values = new MaterialValues();
			values->ambient = Float4(i->materials[j].ambient, 0);
			values->diffuse = Float4(i->materials[j].diffuse, 0);
			values->specular = i->materials[j].specular;
			values->specularPower = i->materials[j].specularPower;
			materials.push_back(new Material(values));
			materials[materials.size() - 1]->SetTextureDiffuse(i->materials[j].fileLocationDiffuse);
			materials[materials.size() - 1]->SetTextureNormal(i->materials[j].fileLocationNormal);
			materials[materials.size() - 1]->SetTextureSpecular(i->materials[j].fileLocationSpecular);
		}
	} 
	if(materials.empty())
	{
		MaterialValues* values = new MaterialValues();
		values->ambient = Float4( 0, 0, 0, 0);
		values->diffuse = Float4( 0, 0, 0, 0);
		values->specular = Float3(0, 0, 0);
		values->specularPower = 0.0f;
		materials.push_back(new Material(values));
	}
	//Clean up
	for (auto m : mtls)
		delete m;
	for (auto m : material)
		delete m;
	return new Geometry(vertex, indices, materials);
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
				data->uvs.emplace_back(value.x, value.y);
			else if (c == 'f')
				ReadFace(line.c_str(), material.c_str(), data);
			else if (c == 'm' && line.substr(0, 6).compare("mtllib"))
				material = location + line.substr(8);
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
				data->materials[material].diffuse = value;
			else if (sscanf_s(line.c_str(), "Ka %f %f %f", value.x, value.y, value.z) == 3)
				data->materials[material].ambient = value;
			else if (sscanf_s(line.c_str(), "Ks %f %f %f", value.x, value.y, value.z) == 3)
				data->materials[material].specular = value;
			else if (sscanf_s(line.c_str(), "Ns %f", value.x) == 1)
				data->materials[material].specularPower = value.x;
			else if (sscanf_s(line.c_str(), "d %f", value.x) == 1)
				data->materials[material].transparency = value.x;
			else if (c == 'n' && line.substr(0, 6).compare("newmtl")) //The first compare is to decrease the amount of substrings needed
				material = line.substr(8).c_str(); //TODO add in texture path reading 
			else if (c == 'm')
			{
				if (line.substr(0, 6).compare("map_Kd"))
					data->materials[material].fileLocationDiffuse = (void *)line.substr(8).c_str();
				else if (line.substr(0, 6).compare("map_Ks"))
					data->materials[material].fileLocationSpecular = (void *)line.substr(8).c_str();
				else if (line.substr(0, 8).compare("map_bump"))
					data->materials[material].fileLocationSpecular = (void *)line.substr(10).c_str();
			}
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

void OBJFileReader::PackData(Obj* obj, std::map<unsigned short, std::vector<ObjectVertex>>& outVertex, 
	std::map<unsigned short, std::vector<unsigned short>>& outIndices, std::vector<const char*>& outMaterial)
{
	//Filling out the data set
	FillData(obj);
	//Sorting all in to one
	std::map<unsigned short, std::map<Packed, unsigned short>> map; //First map is for materials, second is for the vertices and their indice
	; //Orders the materials
	for (auto i : obj->indicesVertices)
	{
		outMaterial.push_back(i.first);
		for (auto j = 0; j < obj->indicesVertices[i.first].size(); j++)
		{
			Packed data = Packed(obj->vertices[obj->indicesVertices[i.first][j]],
								  obj->normals[obj->indicesNormals[i.first][j]],
								  obj->uvs[obj->indicesUvs[i.first][j]]);
			unsigned short index;
			if(!FindSameData(map[outMaterial.size()-1], data, index))
			{
				outVertex[outMaterial.size() - 1].push_back({data.vertex, data.uv, data.normal, data.tangent });
				index = outVertex.size() - 1;
				map[outMaterial.size() - 1][data] = index;
			}
			outIndices[outMaterial.size() - 1].push_back(index);
		}
	}
}

void OBJFileReader::CalculateTangents(std::map<unsigned short, std::vector<ObjectVertex>>& vertex, 
	std::map<unsigned short, std::vector<unsigned short>>& indices, std::vector<const char*> materials)
{
	//// http://www.terathon.com/code/tangent.html ////
	for (unsigned short i = 0; i < materials.size(); i++)
	{
		Float3* tan1 = new Float3[indices[i].size() * 2]{Float3()};
		Float3* tan2 = tan1 + indices[i].size();
		unsigned short x, y, z; //Moving out the for loops allows us not have to assign memory every iteration, but keeping the code readable is difficult
		for(auto j = 0; j < indices[i].size(); j += 3) //Triangles have 3 points
		{
			x = indices[i][j];
			y = indices[i][j + 1];
			z = indices[i][j + 2];

			const Float3& v1 = vertex[i][x].vertex;
			const Float3& v2 = vertex[i][y].vertex;
			const Float3& v3 = vertex[i][z].vertex;

			const Float2& w1 = vertex[i][x].uv;
			const Float2& w2 = vertex[i][y].uv;
			const Float2& w3 = vertex[i][z].uv;

			//The difference
			const Float3 g1(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
			const Float3 g2(v3.x - v1.x, v3.x - v1.y, v3.z - v1.z);

			const Float2 h1(w2.x - w1.x, w2.y - w1.y);
			const Float2 h2(w3.x - w1.x, w3.y - w1.y);

			//scale
			const float r = 1.0f / (h1.x * h2.y - h2.x * h1.y);

			//Tangent Direction
			Float3 sdir((h2.y * g1.x - h1.y * g2.x) * r, 
						(h2.y * g1.y - h1.y * g2.y) * r,
						(h2.y * g1.z - h1.y * g2.z) * r);
			Float3 tdir((h1.x * g2.x - g2.x * g1.x) * r,
						(h1.x * g2.y - g2.x * g1.y) * r,
						(h1.x * g2.z - g2.x * g1.z) * r);
			//Working the tangents out
			tan1[x] += sdir;
			tan1[y] += sdir;
			tan1[z] += sdir;
			tan2[x] += tdir;
			tan2[y] += tdir;
			tan2[z] += tdir;
		}
		Float3 tangent; 
		float w;
		for(auto j = 0; j < indices[i].size(); j++)
		{
			tangent = tan1[i] - vertex[i][j].normal * tan1[i].Dot(vertex[i][j].normal);
			w = vertex[i][j].normal.Cross(tan1[i]).Dot(tan2[i]) < 0 ? -1.0f : 1.0f;
			vertex[i][j].tangent = Float4(tangent, w);
		}
		delete[] tan1;
	}
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

void OBJFileReader::FillData(Obj*& obj)
{
	if (obj->uvs.size() == 0)
		obj->uvs.push_back(Float2(0, 0));
	if (obj->normals.size() == 0)
		obj->normals.push_back(Float3(0, 0, 0));
	for (auto i : obj->indicesVertices)
	{
		//UV
		for (int j = obj->indicesUvs[i.first].size(); j != i.second.size(); j++)
			obj->indicesUvs[i.first].push_back(0u);
		//Normals
		for (int j = obj->indicesNormals[i.first].size(); j != i.second.size(); j++)
			obj->indicesNormals[i.first].push_back(0u);
	}
}

bool OBJFileReader::FindSameData(std::map<Packed, unsigned short>& map, Packed& data, unsigned short& outIndex)
{
	const auto it = map.find(data);
	if (it == map.end())
		return false;
	outIndex = it->second;
	return true;
}

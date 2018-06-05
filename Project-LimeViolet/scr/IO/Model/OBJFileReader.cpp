#include "OBJFileReader.h"
#include <fstream>
#include <string>
#include <algorithm>
#include "../../Common/Material.h"

bool OBJFileReader::CompareCharValues::operator()(const char* left, const char* right) const
{
	return std::strcmp(left, right) < 0;
}

OBJFileReader::Obj::~Obj()
{
	for (auto i : materialPaths)
		delete i;
}

OBJFileReader::Packed::Packed(Float3 vertex, Float3 normal, Float2 uv)
	: vertex(vertex), normal(normal), uv(uv), tangent(Float4(0, 0, 0, 0))
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

Geometry* OBJFileReader::ReadFile(const char* fileLocation) const
{
	Obj* obj = LoadObj(fileLocation); //First loads the OBJ file in to memory
	if (!obj)
		return nullptr;
	std::vector<Mtl*> mtls; 
	//Loads all the MTL files, was going to be threaded, but during testing it was fasting having it on the same thread.  
	//As realistically,there will only be about 1 material file to read even if its possible to add more than 1
	LoadMtlFiles(obj->materialPaths, mtls); 
	//Pack Data in single indices
	std::map<unsigned short, std::vector<ObjectVertex>> vertex;
	std::map<unsigned short, std::vector<unsigned short>> indices;
	std::vector<const char*> material;
	PackData(obj, vertex, indices, material);
	//Clean up
	delete obj;
	//Calculate Tangents if needed
	if (mtls.size() != 0)
	{
		bool tangents = false; //Finds out if we need tangents calculated
		for(int i = 0; i < mtls.size(); i++)
			if(mtls[i]->isNormalMap)
			{
				tangents = true;
				break;
			}
		if (tangents) //Was tested to put on a new thread but it made it slower when testing
			CalculateTangents(vertex, indices, material);
	}
	//Create Materials that fit
	std::vector<Material*> materials;
	for(int i = 0; i < mtls.size(); i++)
		for (auto j : material)
		{
			auto data = mtls[i]->materials.find(j);
			if (data == mtls[i]->materials.end())
				break;
			material.erase(std::remove(material.begin(), material.end(), j), material.end());
			MaterialValues* values = new MaterialValues();
			values->ambient = Float4(mtls[i]->materials[j].ambient, 0);
			values->diffuse = mtls[i]->materials[j].diffuse;
			values->transparency = mtls[i]->materials[j].transparency;
			values->specular = mtls[i]->materials[j].specular;
			values->specularPower = mtls[i]->materials[j].specularPower;
			materials.push_back(new Material(values));
			materials[materials.size() - 1]->SetTextureDiffuse(mtls[i]->materials[j].fileLocationDiffuse);
			materials[materials.size() - 1]->SetTextureNormal(mtls[i]->materials[j].fileLocationNormal);
			materials[materials.size() - 1]->SetTextureSpecular(mtls[i]->materials[j].fileLocationSpecular);
		}
	if (materials.empty()) //If no names of the materials match, we set default values
	{
		for (int i = 0; i < material.size(); i++)
		{
			MaterialValues* values = new MaterialValues();
			values->ambient = Float4(0, 0, 0, 0);
			values->diffuse = Float3(0, 0, 0);
			values->transparency = 1.0f;
			values->specular = Float3(0, 0, 0);
			values->specularPower = 0.0f;
			materials.push_back(new Material(values));
		}
	}
	//Clean up
	for (int i = 0; i < mtls.size(); i++)
		delete mtls[i];
	for (int i = 0; i < material.size(); i++)
		delete material[i];
	return new Geometry(vertex, indices, materials); //return
}

OBJFileReader::Obj* OBJFileReader::LoadObj(const char* fileLocation) const
{
	std::ifstream stream; //Opens file in to a stream
	stream.open(fileLocation);
	if (!stream.good())
		return nullptr;
	Obj* data(nullptr);
	std::string line;
	try //Just encase any error happen so the file can be correctly closed
	{
		std::string location = std::string(fileLocation).substr(0, std::string(fileLocation).find_last_of("/") + 1);
		Float3 value;
		const char* material(nullptr);
		data = new Obj();
		while (getline(stream, line))
		{
			if (line.empty())
				continue;
			const char c = line[0]; 
			if (c == '#' || c == '\n' || c == 'o' || c == 'g' || c == ' ' || c == 's') //Skips these characters or continues to read information
				continue;
			if(sscanf_s(line.c_str(), "v %f %f %f", &value.x, &value.y, &value.z) == 3)
				data->vertices.push_back(value);
			else if (sscanf_s(line.c_str(), "vn %f %f %f", &value.x, &value.y, &value.z) == 3)
				data->normals.push_back(value);
			else if (sscanf_s(line.c_str(), "vt %f %f", &value.x, &value.y) == 2)
				data->uvs.emplace_back(value.x, value.y);
			else if (c == 'f') 
				ReadFace(line.c_str(), material, data);
			else if (c == 'm' && line.substr(0, 6).compare("mtllib") == 0)
			{
				std::string spath = location + line.substr(7);
				char* path = new char[spath.size() + 1]();
				strcpy_s(path, spath.size() + 1, spath.c_str());
				data->materialPaths.push_back(path);
			}
			else if (c =='u' && line.substr(0, 6).compare("usemtl") == 0)
			{
				char* mat = new char[line.substr(7).size() + 1]();
				strcpy_s(mat, line.substr(7).size() + 1, line.substr(7).c_str());
				material = mat;
			}
		}
	}
	catch (std::exception e)
	{
		delete data; //Clean up
		stream.close(); //Safe closing of the file
		throw;
	}
	stream.close();
	return data;
}

OBJFileReader::Mtl* OBJFileReader::LoadMtl(const char* fileLocation) const
{
	std::ifstream stream; //Opens file in to a stream
	stream.open(fileLocation); 
	if (!stream.good())
		return nullptr;
	Mtl* data(nullptr);
	try //Encase any errors happen while reading the file, it can safely close it.
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
			if (sscanf_s(line.c_str(), "Kd %f %f %f", &value.x, &value.y, &value.z) == 3)
				data->materials[material].diffuse = value;
			else if (sscanf_s(line.c_str(), "Ka %f %f %f", &value.x, &value.y, &value.z) == 3)
				data->materials[material].ambient = value;
			else if (sscanf_s(line.c_str(), "Ks %f %f %f", &value.x, &value.y, &value.z) == 3)
				data->materials[material].specular = value;
			else if (sscanf_s(line.c_str(), "Ns %f", &value.x) == 1)
				data->materials[material].specularPower = value.x;
			else if (sscanf_s(line.c_str(), "d %f", &value.x) == 1)
				data->materials[material].transparency = value.x;
			else if (c == 'n' && line.substr(0, 6).compare("newmtl") == 0) //The first compare is to decrease the amount of substrings needed
			{
				char* mat = new char[line.substr(7).size() + 1]();
				strcpy_s(mat, line.substr(7).size() + 1, line.substr(7).c_str());
				material = mat;
			}
			else if (c == 'm')
			{
				if (line.substr(0, 6).compare("map_Kd") == 0)
				{
					char* path = new char[line.substr(8).size() + 1]();
					strcpy_s(path, line.substr(8).size() + 1, line.substr(8).c_str());
					data->materials[material].fileLocationDiffuse = static_cast<void *>(path);
				}
				else if (line.substr(0, 6).compare("map_Ks") == 0)
				{
					char* path = new char[line.substr(8).size() + 1]();
					strcpy_s(path, line.substr(8).size() + 1, line.substr(8).c_str());
					data->materials[material].fileLocationSpecular = static_cast<void *>(path);
				}
				else if (line.substr(0, 8).compare("map_bump") == 0)
				{
					char* path = new char[line.substr(10).size() + 1]();
					strcpy_s(path, line.substr(10).size() + 1, line.substr(10).c_str());
					data->materials[material].fileLocationNormal = static_cast<void *>(path);
					data->isNormalMap = true;
				}
			}
		}
	}
	catch (std::exception e)
	{
		delete data; //clean up
		stream.close();
		throw;
	}
	stream.close();
	return data;
}

void OBJFileReader::PackData(Obj* obj, std::map<unsigned short, std::vector<ObjectVertex>>& outVertex,
	std::map<unsigned short, std::vector<unsigned short>>& outIndices, std::vector<const char*>& outMaterial)
{
	//Filling out the data set, so we don't happen to access memory that we don't use
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
			if (!FindSameData(map[static_cast<unsigned short>(outMaterial.size()) - 1u], data, index))
			{
				outVertex[static_cast<unsigned short>(outMaterial.size()) - 1u].push_back({ data.vertex, data.uv, data.normal, data.tangent });
				index = static_cast<unsigned short>(outVertex.size()) - 1;
				map[static_cast<unsigned short>(outMaterial.size()) - 1u][data] = index;
			}
			outIndices[static_cast<unsigned short>(outMaterial.size()) - 1u].push_back(index);
		}
	}
}

void OBJFileReader::CalculateTangents(std::map<unsigned short, std::vector<ObjectVertex>>& vertex,
	std::map<unsigned short, std::vector<unsigned short>>& indices, std::vector<const char*> materials)
{
	//// http://www.terathon.com/code/tangent.html was used as a base for this piece of code
	for (unsigned short i = 0; i < materials.size(); i++)
	{
		Float3* tan1 = new Float3[indices[i].size() * 2]{ Float3() }; //Just request memory twice the size so we can get a pointer half way
		Float3* tan2 = tan1 + indices[i].size();
		unsigned short x, y, z; //Moving out the for loops allows us not have to assign memory every iteration, but keeping the code readable is difficult
		for (auto j = 0; j < indices[i].size(); j += 3) //Triangles have 3 points
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
			//Adding the values together so we can later norlize them 
			tan1[x] += sdir;
			tan1[y] += sdir;
			tan1[z] += sdir;
			tan2[x] += tdir;
			tan2[y] += tdir;
			tan2[z] += tdir;
		}
		Float3 tangent;
		float w;
		for (auto j = 0; j < vertex[i].size(); j++)
		{
			tangent = tan1[i] - vertex[i][j].normal * tan1[i].Dot(vertex[i][j].normal);
			tangent.Normalize();
			w = vertex[i][j].normal.Cross(tan1[i]).Dot(tan2[i]) < 0 ? -1.0f : 1.0f;
			vertex[i][j].tangent = Float4(tangent, w);
		}
		delete[] tan1;
	}
}

//Used to load in all the Mtl files
void OBJFileReader::LoadMtlFiles(std::vector<const char*>& paths, std::vector<Mtl*> & mtls) const
{
	for (int i = 0; i < paths.size(); i++) //Load all MTL files
		mtls.push_back(LoadMtl(paths[i]));
}

//As OBJ files have different face formattings, this function was taken out to make the code more clean, we can also extend it to different faces, so we could support polygons than just triangles
void OBJFileReader::ReadFace(const char* line, const char* material, Obj*& data)
{
	UShort3 vertex;
	UShort3 uv;
	UShort3 normal;
	if (sscanf_s(line, "f %hu %hu %hu", &vertex.x, &vertex.y, &vertex.z) == 3)
	{
		data->indicesVertices[material].push_back(vertex.x - 1); //We subtract 1 so we can used the value to access an array element, as OBJ files indexing start at 1 not 0
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
	if (obj->uvs.empty()) //Sets a value we will be using as default for all the indices to access when we later use them
		obj->uvs.emplace_back(0.0f, 0.0f);
	if (obj->normals.empty())
		obj->normals.emplace_back(0.0f, 0.0f, 0.0f);
	for (auto i : obj->indicesVertices)
	{
		//UV
		for (auto j = obj->indicesUvs[i.first].size(); j != i.second.size(); j++)
			obj->indicesUvs[i.first].push_back(0u);
		//Normals
		for (auto j = obj->indicesNormals[i.first].size(); j != i.second.size(); j++)
			obj->indicesNormals[i.first].push_back(0u);
	}
}

//Used to find simular memory 
bool OBJFileReader::FindSameData(std::map<Packed, unsigned short>& map, Packed& data, unsigned short& outIndex)
{
	const auto it = map.find(data);
	if (it == map.end())
		return false;
	outIndex = it->second;
	return true;
}

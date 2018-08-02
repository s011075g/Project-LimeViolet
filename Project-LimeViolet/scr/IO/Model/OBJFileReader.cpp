#include "OBJFileReader.h"
#include <fstream>
#include <string>
#include <algorithm>

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

RawGeometry* OBJFileReader::ReadFile(const char* fileLocation, const bool calculateTangents)
{
	Obj* obj = LoadObj(fileLocation); //First loads the OBJ file in to memory
	if (!obj)
		return nullptr;
	//Pack Data in single indices
	std::vector<ObjectVertex> vertex;
	std::vector<std::vector<unsigned short>> indices;
	PackData(obj, vertex, indices);
	//Calculates Tangents if needed
	if (calculateTangents)
		CalculateTangents(vertex, indices, obj->materialCount);
	//Clean up
	delete obj;
	return new RawGeometry(vertex, indices); //return
}

OBJFileReader::Obj* OBJFileReader::LoadObj(const char* fileLocation)
{
	std::ifstream stream; //Opens file in to a stream
	stream.open(fileLocation, std::ios_base::in);
	if (!stream.good())
		return nullptr;
	Obj* data(nullptr);
	try //Just encase any error happen so the file can be correctly closed
	{
		std::string location = std::string(fileLocation).substr(0, std::string(fileLocation).find_last_of("/") + 1);
		Float3 value;
		int materialValue = -1;
		data = new Obj();
		std::string line;
		while (getline(stream, line))
		{
			if (line.empty())
				continue;
			const char c = line[0]; 
			if (c == '#' || c == '\n' || c == 'o' || c == 'g' || c == ' ' || c == 's') //Skips these characters or continues to read information
				continue;
			const char* ptr = line.c_str();
			if(sscanf_s(ptr, "v %f %f %f", &value.x, &value.y, &value.z) == 3)
				data->vertices.push_back(value);
			else if (sscanf_s(ptr, "vn %f %f %f", &value.x, &value.y, &value.z) == 3)
				data->normals.push_back(value);
			else if (sscanf_s(ptr, "vt %f %f", &value.x, &value.y) == 2)
				data->uvs.emplace_back(value.x, value.y);
			else if (c == 'f') 
				ReadFace(ptr, materialValue, data);
			else if (c == 'm' && line.substr(0, 6).compare("mtllib") == 0)
			{
				std::string spath = location + line.substr(7);
				char* path = new char[spath.size() + 1]();
				strcpy_s(path, spath.size() + 1, spath.c_str());
				data->materialPaths.push_back(path);
			}
			else if (c =='u' && line.substr(0, 6).compare("usemtl") == 0)
			{
				materialValue++;
				data->indicesVertices.push_back(std::vector<unsigned short>());
				data->indicesNormals.push_back(std::vector<unsigned short>());
				data->indicesUvs.push_back(std::vector<unsigned short>());
			}
		}
		data->materialCount = materialValue;
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

void OBJFileReader::PackData(Obj* obj, std::vector<ObjectVertex>& outVertex, std::vector<std::vector<unsigned short>>& outIndices)
{
	//Filling out the data set, so we don't happen to access memory that we don't use
	FillData(obj);
	//Sorting all in to one
	std::map<Packed, unsigned short> map; //First map is for materials, second is for the vertices and their indice

	outIndices.resize(obj->indicesVertices.size());
	for(size_t i = 0; i < obj->indicesVertices.size(); i++)
	{
		outIndices[i] = std::vector<unsigned short>();
		for (size_t j = 0; j < obj->indicesVertices[i].size(); j++)
		{
			Packed data = Packed(obj->vertices[obj->indicesVertices[i][j]],
									obj->normals[obj->indicesNormals[i][j]],
									obj->uvs[obj->indicesUvs[i][j]]);
			unsigned short index;
			if (!FindSameData(map, data, index))
			{
				outVertex.push_back({ data.vertex, data.uv, data.normal, data.tangent });
				index = static_cast<unsigned short>(outVertex.size()) - 1;
				map[data] = index;
			}
			outIndices[i].push_back(index);
		}
	}
}

void OBJFileReader::CalculateTangents(std::vector<ObjectVertex>& vertex, std::vector<std::vector<unsigned short>>& indices, const size_t materialCount)
{
	//// http://www.terathon.com/code/tangent.html was used as a base for this piece of code
	for (size_t i = 0; i < materialCount; i++)
	{
		Float3* tan1 = new Float3[indices[i].size() * 2]{ Float3() }; //Just request memory twice the size so we can get a pointer half way
		Float3* tan2 = tan1 + indices[i].size();
		unsigned short x, y, z; //Moving out the for loops allows us not have to assign memory every iteration, but keeping the code readable is difficult
		Float3 v1, v2, v3;
		Float2 w1, w2, w3;
		for (size_t j = 0; j < indices[i].size(); j += 3) //Triangles have 3 points
		{
			x = indices[i][j];
			y = indices[i][j + 1];
			z = indices[i][j + 2];

			v1 = vertex[x].vertex;
			v2 = vertex[y].vertex;
			v3 = vertex[z].vertex;

			w1 = vertex[x].uv;
			w2 = vertex[y].uv;
			w3 = vertex[z].uv;

			//The difference
			const Float3 g1(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
			const Float3 g2(v3.x - v1.x, v3.x - v1.y, v3.z - v1.z);

			const Float2 h1(w2.x - w1.x, w2.y - w1.y);
			const Float2 h2(w3.x - w1.x, w3.y - w1.y);

			//scale
			const float r = 1.0f / (h1.x * h2.y - h2.x * h1.y);

			//Tangent Direction
			const Float3 sdir((h2.y * g1.x - h1.y * g2.x) * r,
				(h2.y * g1.y - h1.y * g2.y) * r,
				(h2.y * g1.z - h1.y * g2.z) * r);
			const Float3 tdir((h1.x * g2.x - g2.x * g1.x) * r,
				(h1.x * g2.y - g2.x * g1.y) * r,
				(h1.x * g2.z - g2.x * g1.z) * r);
			//Adding the values together so we can later normalize them 
			tan1[x] += sdir;
			tan2[x] += tdir;
			tan1[y] += sdir;
			tan2[y] += tdir;
			tan1[z] += sdir;
			tan2[z] += tdir;
		}
		Float3 tangent;
		float w;
		for (size_t j = 0; j < vertex.size(); j++)
		{
			tangent = tan1[j] - vertex[j].normal * tan1[j].Dot(vertex[j].normal);
			tangent.Normalize();
			w = vertex[j].normal.Cross(tan1[i]).Dot(tan2[j]) < 0 ? -1.0f : 1.0f;
			vertex[j].tangent = Float4(tangent, w);
		}
		delete[] tan1;
	}
}

//As OBJ files have different face formattings, this function was taken out to make the code more clean, we can also extend it to different faces, so we could support polygons than just triangles
void OBJFileReader::ReadFace(const char* line, const int materialValue, Obj*& data)
{
	UShort3 vertex;
	UShort3 uv;
	UShort3 normal;
	//We subtract 1 so we can used the value to access an array element, as OBJ files indexing start at 1 not 0
	if (sscanf_s(line, "f %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu", &vertex.x, &uv.x, &normal.x, &vertex.y, &uv.y, &normal.y, &vertex.z, &uv.z, &normal.z) == 9)
	{
		data->indicesVertices[materialValue].push_back(vertex.x - 1);
		data->indicesVertices[materialValue].push_back(vertex.y - 1);
		data->indicesVertices[materialValue].push_back(vertex.z - 1);
		data->indicesUvs[materialValue].push_back(uv.x - 1);
		data->indicesUvs[materialValue].push_back(uv.y - 1);
		data->indicesUvs[materialValue].push_back(uv.z - 1);
		data->indicesNormals[materialValue].push_back(normal.x - 1);
		data->indicesNormals[materialValue].push_back(normal.y - 1);
		data->indicesNormals[materialValue].push_back(normal.z - 1);
	}
	else if (sscanf_s(line, "f %hu/%hu %hu/%hu %hu/%hu", &vertex.x, &uv.x, &vertex.y, &uv.y, &vertex.z, &uv.z) == 6)
	{
		data->indicesVertices[materialValue].push_back(vertex.x - 1);
		data->indicesVertices[materialValue].push_back(vertex.y - 1);
		data->indicesVertices[materialValue].push_back(vertex.z - 1);
		data->indicesUvs[materialValue].push_back(uv.x - 1);
		data->indicesUvs[materialValue].push_back(uv.y - 1);
		data->indicesUvs[materialValue].push_back(uv.z - 1);
	}
	else if (sscanf_s(line, "f %hu//%hu %hu//%hu %hu//%hu", &vertex.x, &normal.x, &vertex.y, &normal.y, &vertex.z, &normal.z) == 6)
	{
		data->indicesVertices[materialValue].push_back(vertex.x - 1);
		data->indicesVertices[materialValue].push_back(vertex.y - 1);
		data->indicesVertices[materialValue].push_back(vertex.z - 1);
		data->indicesNormals[materialValue].push_back(normal.x - 1);
		data->indicesNormals[materialValue].push_back(normal.y - 1);
		data->indicesNormals[materialValue].push_back(normal.z - 1);
	}
	else if (sscanf_s(line, "f %hu %hu %hu", &vertex.x, &vertex.y, &vertex.z) == 3)
	{
		data->indicesVertices[materialValue].push_back(vertex.x - 1); 
		data->indicesVertices[materialValue].push_back(vertex.y - 1);
		data->indicesVertices[materialValue].push_back(vertex.z - 1);
	}
}

void OBJFileReader::FillData(Obj*& obj)
{
	if (obj->uvs.empty()) //Sets a value we will be using as default for all the indices to access when we later use them
		obj->uvs.emplace_back(0.0f, 0.0f);
	if (obj->normals.empty())
		obj->normals.emplace_back(0.0f, 0.0f, 0.0f);
	for (size_t i = 0; i < obj->indicesVertices.size(); i++)
	{
		//UV
		for (auto j = obj->indicesUvs[i].size(); j != obj->indicesVertices[i].size(); j++)
			obj->indicesUvs[i].push_back(0u);
		//Normals
		for (auto j = obj->indicesNormals[i].size(); j != obj->indicesVertices[i].size(); j++)
			obj->indicesNormals[i].push_back(0u);
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

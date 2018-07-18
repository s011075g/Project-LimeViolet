#include "EntityReader.h"

EntityHandle EntityReader::ReadEntity(Json::Value value)
{
	if (!value["Transform"].empty())
	{
		//Transform
	}
	if(!value["Model"].empty())
	{
		//Model
	}
	return nullptr;
}

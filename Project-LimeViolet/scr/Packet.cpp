#include "Packet.h"

Packet::Packet(const char* type)
	: _type(type)
{ }

Packet::~Packet()
{
	delete[] _type;
}

const char * Packet::GetType() const
{
	return _type;
}

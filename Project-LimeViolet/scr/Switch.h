#pragma once
#include "Packet.h"
#include <map>
#include <list>

struct CmpStr
{
	bool operator()(const char const * left, const char const * right) const
	{
		return std::strcmp(left, right) < 0;
	}
};

class Switch
{
private:
	std::map<const char*, std::list<void(*)(Packet const *)>, CmpStr> _subscriptions;
public:
	explicit Switch();
	~Switch();
	void EventOccurred(const Packet const * event);
	void Subscribe(const char * event, void(* func)(Packet const *));
	void Unsubscribe(const char* event, void (* func)(Packet const*));
};

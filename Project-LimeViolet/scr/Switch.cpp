#include "Switch.h"

bool CmpStr::operator()(const char* left, const char* right) const
{
	return std::strcmp(left, right) < 0;
}

Switch::Switch()
{ }

Switch::~Switch()
{
	for (auto array = _subscriptions.begin(); array != _subscriptions.end(); ++array)
		delete[] array->first;
	_subscriptions.clear();
}

Switch const* Switch::Instance()
{
	static Switch instance = Switch();
	return &instance;
}

void Switch::EventOccurred(const Packet * event)
{
	for (std::_List_iterator<void(*)(Packet const*)>::value_type array = _subscriptions[event->GetType()].begin(); array != _subscriptions[event->GetType()].end(); ++array)
		array(event);
}

void Switch::Subscribe(const char* event, void(* func)(Packet const *))
{
	_subscriptions[event].push_back(func);
}

void Switch::Unsubscribe(const char* event, void(* func)(Packet const *))
{
	_subscriptions[event].remove(func);
}

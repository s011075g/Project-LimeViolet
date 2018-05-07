#include "Switch.h"

Switch::Switch()
{ }

Switch::~Switch()
{
	for (auto array = _subscriptions.begin(); array != _subscriptions.end(); ++array)
		delete[] array->first;
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

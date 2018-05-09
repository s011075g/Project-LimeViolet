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
}

Switch const* Switch::Instance()
{
	static Switch instance = Switch();
	return &instance;
}

void Switch::EventOccurred(Packet * event)
{
	for (auto array : _subscriptions[event->GetType()])
		array(event);
}

void Switch::Subscribe(const char* event, void(* func)(Packet *))
{
	_subscriptions[event].push_back(func);
}

void Switch::Unsubscribe(const char* event, void(* func)(Packet *))
{
	_subscriptions[event].remove(func);
}

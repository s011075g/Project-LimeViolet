#pragma once
#include "Packet.h"
#include <map>
#include <list>

/**
 * \brief Used to override the map operator to compare characters
 */
struct CmpStr
{
	bool operator()(const char const* left, const char const* right) const;
};

class Switch
{
private:
	std::map<const char*, std::list<void(*)(Packet const *)>, CmpStr> _subscriptions;
public:	
	~Switch();
	/**
	 * \brief As the switch is a singleton, must call this function to use its other functions
	 * \return It returns the static instance of the switch
	 */
	static Switch const * Instance();
	/**
	 * \brief Tells the switch an event occurred
	 * \param event The event that has occurred with details stored in the packet
	 */
	void EventOccurred(const Packet const * event);
	/**
	 * \brief Subscribe to an event, and calls the given function when the event occurs 
	 * \param event The event that is subscribed to
	 * \param func  The function called when the event occurs
	 */
	void Subscribe(const char * event, void(* func)(Packet const *));
	/**
	 * \brief Unsubscribe to an event thats been previously subscribed to
	 * \param event The event that is unsubscribed from
	 * \param func  The function was going to be called
	 */
	void Unsubscribe(const char* event, void (* func)(Packet const*));
private:
	/**
	 * \brief Private constructor as the class is a singleton. 
	 */
	Switch();
};

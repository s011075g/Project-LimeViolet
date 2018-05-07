#pragma once

class Packet
{
private:
	const char * _type;
public:
	/**
	 * \brief Constructor for a Packet which is used to inform that an event has occurred.
	 * \param type Type is the event that has occurred and what others subscribe to.
	 */
	explicit Packet(const char * type);
	virtual ~Packet();
	/**
	 * \brief Returns the event that has occurred.
	 * \return Returns const char* _type 
	 */
	const char * GetType() const;
};

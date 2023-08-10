#ifndef DESTINATION_PORT_NUMBER_H
#define DESTINATION_PORT_NUMBER_H


#include "ns3/ipv4-header.h"
#include "ns3/uinteger.h"
#include "filter-element.h"


namespace ns3 
{

/**
 * \brief A filter element that matches packets with a specific destination port number.
 */
class DestinationPortNumber : public FilterElement 
{
	public:
	  /**
	   * \brief Constructs a new DestinationPortNumber filter element.
	   *
	   * \param[in] value The port number to match against.
	   */
	  DestinationPortNumber(uint32_t value);

	  /**
	   * \brief Determines if the given packet's destination port matches the filter element's value.
	   *
	   * \param[in] p The packet to test.
	   *
	   * \returns true if the packet's destination port matches, false otherwise.
	   */
	  bool match(Ptr<Packet> p);

	private:
	  uint32_t m_value; ///< The port number to match against.
};

} // namespace ns3

#endif // DESTINATION_PORT_NUMBER_H

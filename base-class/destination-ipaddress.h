#ifndef DESTINATION_IP_ADDRESS_H
#define DESTINATION_IP_ADDRESS_H

#include "ns3/ipv4-address.h"
#include "ns3/packet.h"
#include "filter-element.h"

namespace ns3 
{

/**
 * \brief A filter element that matches packets with a specific destination IP address.
 */
class DestinationIPAddress : public FilterElement 
{
	public:
	  /**
	   * \brief Constructs a new DestinationIPAddress filter element.
	   *
	   * \param[in] value The IP address to match against.
	   */
	  DestinationIPAddress (Ipv4Address value);

	  /**
	   * \brief Determines if the given packet's destination IP address matches the filter element's value.
	   *
	   * \param[in] p The packet to test.
	   *
	   * \returns true if the packet's destination IP address matches, false otherwise.
	   */
	  bool match (Ptr<Packet> p);

	private:
	   Ipv4Address m_value; ///< The IP address to match against.
};

} // namespace ns3

#endif // DESTINATION_IP_ADDRESS_H

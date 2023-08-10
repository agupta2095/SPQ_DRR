#ifndef PROTOCOLNUMBER_H
#define PROTOCOLNUMBER_H

#include "ns3/ipv4-header.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"

#include "filter-element.h"

namespace ns3 
{

/**
 * \brief A filter element that matches the protocol number of a packet's IP header.
 */
class ProtocolNumber : public FilterElement 
{
	public:
	  /**
	   * \brief Constructs a ProtocolNumber filter element.
	   *
	   * \param[in] num The protocol number to match.
	   */
	  ProtocolNumber (uint32_t num);

	  /**
	   * \brief Determines if the given packet's protocol number matches the filter element's value.
	   *
	   * \param[in] p The packet to test.
	   *
	   * \returns true if the packet's protocol number matches the filter element's value, false otherwise.
	   */
	  bool match (Ptr<Packet> p);

	private:
	  uint32_t m_value;
};

} // namespace ns3

#endif // PROTOCOLNUMBER_H

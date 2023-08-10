#ifndef DESTINATION_MASK_H
#define DESTINATION_MASK_H


#include "ns3/ipv4-address.h"
#include "ns3/ipv4-header.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "filter-element.h"

namespace ns3 
{

/**
 * \brief A filter element that matches packets with a destination IP address that matches a given mask.
 */
class DestinationMask : public FilterElement 
{
	public:
	  /**
	   * \brief Constructs a new DestinationMask filter element.
	   *
	   * \param[in] value The mask to match against.
	   */
	  DestinationMask (Ipv4Mask value);

	  /**
	   * \brief Determines if the given packet's destination IP address matches the filter element's mask.
	   *
	   * \param[in] p The packet to test.
	   *
	   * \returns true if the packet's destination IP address matches the mask, false otherwise.
	   */
	  bool match (Ptr<Packet> p);

	private:
	  Ipv4Mask m_value; ///< The mask to match against.
};

} // namespace ns3

#endif // DESTINATION_MASK_H

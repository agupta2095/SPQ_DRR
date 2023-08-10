#ifndef SOURCE_PORT_NUMBER_H
#define SOURCE_PORT_NUMBER_H

#include "ns3/boolean.h"
#include "ns3/ipv4-header.h"
#include "ns3/pointer.h"
#include "ns3/ppp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/uinteger.h"
#include "ns3/udp-header.h"

#include "filter-element.h"

namespace ns3 {

/**
 * \ingroup traffic-control
 * \brief The SourcePortNumber class is used to match the source port number of a packet.
 */
class SourcePortNumber : public FilterElement
{
	public:
	  /**
	   * \brief Constructor that sets the source port number to match against.
	   *
	   * \param[in] num the source port number to match against.
	   */
	  SourcePortNumber (uint32_t num);

	  /**
	   * \brief The match function checks if the source port number of the given packet matches the port number to match against.
	   *
	   * \param[in] p the packet to match against.
	   *
	   * \returns true if the source port number of the packet matches the port number to match against, false otherwise.
	   */
	  bool match (Ptr<Packet> p);

	private:
	  uint32_t m_value; ///< The source port number to match against.
};

} // namespace ns3

#endif // SOURCE_PORT_NUMBER_H

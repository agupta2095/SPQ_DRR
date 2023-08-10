#ifndef FILTERELEMENT_H
#define FILTERELEMENT_H

#include "ns3/boolean.h"
#include "ns3/ipv4-header.h"
#include "ns3/pointer.h"
#include "ns3/ppp-header.h"
#include "ns3/packet.h"
#include "ns3/queue.h"
#include "ns3/tcp-header.h"
#include "ns3/uinteger.h"
#include "ns3/udp-header.h"

namespace ns3 
{

/**
 * \brief A virtual class representing a filter element.
 */
class FilterElement 
{
	public:
	  /**
	   * \brief Determines if the given packet matches the filter element.
	   *
	   * \param[in] p The packet to test.
	   *
	   * \returns true if the packet matches the filter element, false otherwise.
	   */
	  virtual bool match(Ptr<Packet> p) = 0;

	  /**
	   * \brief Destroys the filter element.
	   */
	  virtual ~FilterElement() {}
};

} // namespace ns3

#endif // FILTERELEMENT_H

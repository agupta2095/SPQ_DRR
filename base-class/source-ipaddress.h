#ifndef SOURCE_IPADDRESS_H
#define SOURCE_IPADDRESS_H

#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/pointer.h"
#include "ns3/ppp-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"
#include "filter-element.h"

namespace ns3 
{

/**
 * \ingroup packet-filter
 *
 * A filter element that matches packets based on their source IP address.
 */
class SourceIPAddress : public FilterElement 
{
  protected:
    ns3::Ipv4Address m_value; // IP address value to match against

  public:
    /**
     * Constructor.
     *
     * \param num IP address value to match against.
     */
    SourceIPAddress (ns3::Ipv4Address num);

    /**
     * Check if packet matches the filter element.
     *
     * \param p Packet to check.
     * \return True if packet matches the filter element, false otherwise.
     */
    bool match (Ptr<ns3::Packet> p);
};

} // namespace ns3

#endif //SOURCE_IPADDRESS_H


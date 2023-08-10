#ifndef SOURCE_MASK_H
#define SOURCE_MASK_H

#include "ns3/ipv4-address.h"
#include "ns3/ipv4-header.h"
#include "ns3/packet.h"
#include "ns3/packet-socket-address.h"
#include "ns3/tcp-header.h"
#include "ns3/uinteger.h"

#include "filter-element.h"

namespace ns3 
{

/**
 * \ingroup traffic-control
 * \brief The SourceMask class is used to match the source IPv4 address
 * of a packet against a given IPv4 mask.
 */
class SourceMask : public FilterElement 
{
    public:
      /**
       * \brief Constructor that sets the IPv4 mask to compare against.
       *
       * \param[in] num the IPv4 mask to compare against.
       */
      SourceMask (Ipv4Mask num);

      /**
       * \brief The match function checks if the source address of the given packet matches the IPv4 mask.
       *
       * \param[in] p the packet to match against.
       *
       * \returns true if the source address of the packet matches the IPv4 mask, false otherwise.
       */
      bool match (Ptr<Packet> p);

    private:
      Ipv4Mask m_value; ///< The IPv4 mask to compare against the source address of a packet.
};

} // namespace ns3

#endif // SOURCE_MASK_H

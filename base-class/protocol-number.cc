#include "ns3/uinteger.h"
#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include "protocol-number.h"

namespace ns3
{

ProtocolNumber::ProtocolNumber (uint32_t num)
{
	  m_value = num;
}


bool
ProtocolNumber::match (Ptr<ns3::Packet> p)
{
      Ptr<Packet> pCopy = p->Copy ();
      ns3::PppHeader ppp;
      pCopy->RemoveHeader (ppp);
      ns3::Ipv4Header ipv4Header;
      pCopy->RemoveHeader (ipv4Header);
      uint32_t protocol = ipv4Header.GetProtocol ();
      bool ans = (protocol == m_value);
      return ans;
}

} // ns3 namespace
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/pointer.h"
#include "ns3/packet.h"

#include "ns3/ppp-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"

#include "destination-ipaddress.h"

namespace ns3
{

DestinationIPAddress::DestinationIPAddress (Ipv4Address num)
{
	 m_value = num;
}


bool
DestinationIPAddress::match (Ptr<Packet> p)
{
	  Ptr<Packet> pCopy = p->Copy ();
    ns3::PppHeader ppp;
    pCopy->RemoveHeader (ppp);
    ns3::Ipv4Header ipv4Header;
    pCopy->RemoveHeader (ipv4Header);
    Ipv4Address address = ipv4Header.GetDestination ();
    if (address == m_value) 
    {
      return true;
    } 
    else 
    {
      return false;
    }
}


}

#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-address.h"
#include "ns3/tcp-header.h"
#include "ns3/packet-socket-address.h"

#include "destination-mask.h"

namespace ns3
{

DestinationMask::DestinationMask (ns3::Ipv4Mask num)
{
	 m_value = num;
}


bool
DestinationMask::match (Ptr<ns3::Packet> p)
{
    Ptr<Packet> pCopy = p->Copy ();
    ns3::PppHeader ppp;
    pCopy->RemoveHeader (ppp);
    ns3::Ipv4Header ipv4Header;
    pCopy->RemoveHeader (ipv4Header);
    ns3::Ipv4Mask dstMask (m_value);
    ns3::Ipv4Address dstAddr = ipv4Header.GetDestination ();
    if (dstMask.IsMatch (dstAddr, ns3::Ipv4Address::GetAny ())) 
    {
      return true;
    } 
    else 
    {
      return false;
    }
}


} //ns3 namespace


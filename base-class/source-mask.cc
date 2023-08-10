#include "ns3/uinteger.h"
#include "ns3/ipv4-header.h"
#include "ns3/packet.h"
#include "ns3/ipv4-address.h"
#include "ns3/tcp-header.h"
#include "ns3/packet-socket-address.h"

#include "source-mask.h"

namespace ns3
{

SourceMask::SourceMask (Ipv4Mask num)
{
	  m_value = num;
}


bool
SourceMask::match (Ptr<Packet> p)
{
    Ptr<Packet> pCopy = p->Copy ();
    ns3::PppHeader ppp;
    pCopy->RemoveHeader (ppp);
    ns3::Ipv4Header ipv4Header;
    pCopy->RemoveHeader (ipv4Header);
    ns3::Ipv4Mask srcMask (m_value);
    ns3::Ipv4Address srcAddr = ipv4Header.GetSource ();
    if (srcMask.IsMatch (srcAddr, ns3::Ipv4Address::GetAny ())) 
    {
        return true;
    } 
    else 
    {
        return false;
    }
}

}
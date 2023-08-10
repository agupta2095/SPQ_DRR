#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/pointer.h"

#include "ns3/ppp-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"
#include "ns3/packet-socket-address.h"

#include "source-portnumber.h"



namespace ns3
{

SourcePortNumber::SourcePortNumber (uint32_t num)
{
	 m_value = num;
}


bool
SourcePortNumber::match (Ptr<ns3::Packet> p)
{
 
    // Make a copy of the packet
    Ptr<Packet> pCopy = p->Copy ();
    ns3::PppHeader ppp;
    pCopy->RemoveHeader (ppp);
    ns3::Ipv4Header ipv4Header;
    pCopy->RemoveHeader (ipv4Header);

    if (ipv4Header.GetProtocol () != 6 && ipv4Header.GetProtocol () != 17) 
    {
        return false;
    }

    if (ipv4Header.GetProtocol() == 6) 
    {
      TcpHeader tcpHeader;
      pCopy->PeekHeader (tcpHeader);
      if (tcpHeader.GetSourcePort () == m_value) 
      {
          return true;
      }

    } 
    else if (ipv4Header.GetProtocol() == 17) 
    {
        UdpHeader udpHeader;
        pCopy->PeekHeader (udpHeader);

        if (udpHeader.GetSourcePort () == m_value) 
        {
            return true;
        }
    }
    return false;
}

} //ns3 namespace
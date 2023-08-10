#include "ns3/uinteger.h"
#include "ns3/ipv4-header.h"
#include "destination-portnumber.h"


namespace ns3
{

DestinationPortNumber::DestinationPortNumber (uint32_t num)
{
	 m_value = num;
}


bool
DestinationPortNumber::match (Ptr<Packet> p)
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

    if (ipv4Header.GetProtocol () == 6) 
    {
        TcpHeader tcpHeader;
        pCopy->PeekHeader (tcpHeader);
        if (tcpHeader.GetDestinationPort  () == m_value) 
        {
            return true;
        }
    } 
    else if (ipv4Header.GetProtocol () == 17) 
    {
        UdpHeader udpHeader;
        pCopy->PeekHeader(udpHeader);

        if (udpHeader.GetDestinationPort  () == m_value) 
        {
          return true;
        }
    }
    return false;
}


} //ns3 namespace
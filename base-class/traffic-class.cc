#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/pointer.h"
#include <queue>

#include "ns3/ppp-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"
#include "ns3/queue.h"

#include "traffic-class.h"

namespace ns3
{

TrafficClass::TrafficClass () 
{
}

void
TrafficClass::setPackets (uint32_t num)
{
	this->m_packets = num;
}

uint32_t
TrafficClass::getPackets ()
{
	return this->m_packets;
}

void
TrafficClass::setMaxPackets (uint32_t num)
{
	this->m_max_packets = num;
}

uint32_t
TrafficClass::getMaxPackets ()
{
	return this->m_max_packets;
}


void
TrafficClass::setPriorityLevel (uint32_t num)
{
	this->m_priority_level = num;
}

uint32_t
TrafficClass::getPriorityLevel ()
{
	return this->m_priority_level;
}


Ptr<ns3::Packet>
TrafficClass::dequeue() 
{
   if (m_queue.size () == 0) return nullptr;
   Ptr<ns3::Packet> packet = m_queue.front ();
   m_queue.pop ();
   return packet;
}

Ptr<ns3::Packet>
TrafficClass::peek () 
{
	if (m_queue.size () == 0) return nullptr;
   Ptr<ns3::Packet> packet = m_queue.front ();
   return packet;
}

bool
TrafficClass::enqueue (Ptr<ns3::Packet> p) 
{
   if (m_queue.size () == m_max_packets) 
   {
   	  return false;
   } 
   else 
   {
   	  m_queue.push (p);
   	  return true;
   }
}

bool
TrafficClass::match (Ptr<ns3::Packet> p)
{
	for (unsigned int i=0; i < filters.size (); i++)
	{
		if (filters[i]->match (p)) 
		{
			return true;
		}
	}
	
	return false;
}

void
TrafficClass::setQuantum (uint32_t num) 
{
	 this->m_quantum = num;
}

uint32_t
TrafficClass::getQuantum () 
{
    return this->m_quantum;
}

bool 
TrafficClass::empty() 
{
    return m_queue.empty();
}

}//ns3 namespace
#include "ns3/uinteger.h"
#include "ns3/ipv4-header.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/pointer.h"
#include "ns3/queue.h"
#include "ns3/packet.h"
#include <queue>

#include "filter.h"

namespace ns3
{

Filter::Filter()
{
}


void
Filter::add_element (ns3::FilterElement* filter_ele)
{
	m_elementVector.push_back(filter_ele);
}


bool
Filter::match (Ptr<ns3::Packet> p)
{
	for (unsigned int i=0; i< m_elementVector.size (); i++)
	{
		if (m_elementVector[i]->match (p) == false) 
		{
			return false;
		}
	}
	return true;
}


} // ns3 namespace
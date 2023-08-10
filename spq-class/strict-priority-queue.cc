/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */


#include "ns3/enum.h"
#include "ns3/queue.h"
#include "ns3/uinteger.h"
#include "ns3/log.h"
#include "ns3/boolean.h"
#include "ns3/pointer.h"
#include "ns3/simulator.h"

#include "ns3/ppp-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"

#include "strict-priority-queue.h"
#include "xml-parser.h"
#include <iostream>
#include <fstream> 


using namespace std;

namespace ns3 {
// NS_LOG_COMPONENT_DEFINE ("StrictPriorityQueue");
//LogComponentEnable ("StrictPriorityQueue", LOG_LEVEL_LOGIC);

std::ofstream outfile("debug.txt");
NS_OBJECT_ENSURE_REGISTERED (StrictPriorityQueue);

StrictPriorityQueue::StrictPriorityQueue () 
{  
}

StrictPriorityQueue::~StrictPriorityQueue () 
{
    //outfile<<"In Destructor of StrictPriorityQueue" << std::endl;
    for (auto tc: q_class) 
    {
        for (auto filter : tc->filters) 
        {
            for (auto filterElem : filter->getFilterElems ()) 
            {
                 delete filterElem;
            }
            delete filter;
        }
        delete tc;
    }
    q_class.clear ();
}


void
StrictPriorityQueue::createQueues (char* config_file) 
{
    XmlParserSPQ *parser = new XmlParserSPQ ();
    parser->parse_xml (config_file);
    const unordered_map<uint32_t, std::vector<ns3::Filter*> >& queueMap = parser->getQueueMap ();
    q_class.resize (queueMap.size ());
    for (auto it = queueMap.begin (); it != queueMap.end (); it = ++it) 
    {
        uint32_t priority_level = it->first;
        std::vector<ns3::Filter*> filtersVec = it->second;
        TrafficClass* tc = new TrafficClass ();
        tc->setFilters (filtersVec);
        tc->setPriorityLevel (priority_level);
        uint32_t maxPacketSize = parser->getMaxPacketSize (priority_level);
        tc->setMaxPackets (maxPacketSize);
        q_class[priority_level] = tc;
    }
    //outfile<<q_class.size();
    //outfile<<"Deleting SPQ XML Parser" << std::endl;
    delete parser;
}


TypeId
StrictPriorityQueue::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::StrictPriorityQueue")
        .SetParent<DiffServ> ()
        .AddConstructor<StrictPriorityQueue> ()
    ;
    return tid;
}


uint32_t
StrictPriorityQueue::Classify (Ptr<ns3::Packet> p) 
{
    //outfile <<"In Classify-----------------" << endl;
    //p->Print(outfile);
    //outfile << endl;
     for (unsigned int i = 0; i < q_class.size (); i++) 
     {
          if (q_class[i]->match (p) ) 
          {
              //outfile << "Classify " << i << endl;
              return i;
          }
     }
     return q_class.size () + 1;
}


uint32_t
StrictPriorityQueue::Schedule ()
{
    //outfile <<"In Schedule" << endl;
    for (unsigned int i = 0; i < q_class.size (); i++) 
    {
        Ptr<ns3::Packet> p = q_class[i]->peek ();
        if (p != nullptr) 
        {
            return i;
        }
    }
    return q_class.size () + 1;
}

Ptr<const ns3::Packet>
StrictPriorityQueue::Peek (void) const 
{
    for (unsigned int i = 0; i < q_class.size (); i++) 
    {
        Ptr<ns3::Packet> p = q_class[i]->peek ();
        if (p != nullptr) 
        {
            return p;
        }
    }
    return 0;
}


} // namespace ns3
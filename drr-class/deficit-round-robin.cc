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
#include "filter.h"
#include "filter-element.h"
#include "destination-portnumber.h"
#include "deficit-round-robin.h"
#include "xml-parser.h"

#include <iostream>
#include <fstream> 

std::ofstream outfile("debug.txt");
using namespace std;

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (DeficitRoundRobin);

/*DeficitRoundRobin::DeficitRoundRobin() {
  DestinationPortNumber *destPortNumberObj1 = new DestinationPortNumber(903);
  DestinationPortNumber *destPortNumberObj2 = new DestinationPortNumber(904);
  DestinationPortNumber *destPortNumberObj3 = new DestinationPortNumber(905); 
  Filter * filter1 = new Filter(); 
  filter1->add_element(destPortNumberObj1);
  Filter * filter2 = new Filter(); 
  filter2->add_element(destPortNumberObj2);
  Filter * filter3 = new Filter(); 
  filter3->add_element(destPortNumberObj3);
  
  std::vector<Filter*> filterVec1;
  filterVec1.push_back(filter1);
  std::vector<Filter*> filterVec2;
  filterVec2.push_back(filter2);
  std::vector<Filter*> filterVec3;
  filterVec3.push_back(filter3);
  TrafficClass * tc1 = new TrafficClass();
  tc1->setFilters(filterVec1);
  tc1->setQuantum(400);
  tc1->setMaxPackets(1000);
  q_class.push_back(tc1);
  TrafficClass * tc2 = new TrafficClass();
  tc2->setFilters(filterVec2);
  tc2->setQuantum(800);
  tc2->setMaxPackets(1000);
  q_class.push_back(tc2);
  TrafficClass * tc3 = new TrafficClass();
  tc3->setFilters(filterVec3);
  tc3->setQuantum(1200);
  tc3->setMaxPackets(1000);
  q_class.push_back(tc3);
  m_deficitCounter.resize(3);
  m_servicedQueues = 0;
  //m_activeList.resize(3);

}*/

DeficitRoundRobin::DeficitRoundRobin() {
}    

DeficitRoundRobin::~DeficitRoundRobin() {
  outfile<<"In Destructor of DeficitRoundRobin" << std::endl;
  for(auto tc: q_class) {
     for(auto filter : tc->filters) {
        for(auto filterElem : filter->getFilterElems()) {
           delete filterElem;
        }
        delete filter;
     }
     delete tc;
  }
  q_class.clear();
}

void
DeficitRoundRobin::createQueues (char* config_file) 
{
    q_class.clear (); 
    XmlParserDRR *parser = new XmlParserDRR ();
    parser->parse_xml (config_file);
    const std::vector<std::vector<ns3::Filter*> >& queueMap = parser->getQueueMap ();
    for (unsigned int i = 0; i < queueMap.size (); i++) 
    {
        std::vector<ns3::Filter*> filtersVec = queueMap[i];
        std::pair<uint32_t, uint32_t> quantumAndMaxPacket = parser->getQuantumAndMaxPacketSize (i);
        TrafficClass* tc = new TrafficClass ();
        if (quantumAndMaxPacket.first != 0) 
        {
            tc->setFilters (filtersVec);
            tc->setQuantum (quantumAndMaxPacket.first);
            tc->setMaxPackets (quantumAndMaxPacket.second);
            q_class.push_back (tc);
        }
    }
    //outfile<<q_class.size()<<endl;
    m_deficitCounter.resize (q_class.size ());
    m_servicedQueues = 0;
    delete parser;
}


TypeId
DeficitRoundRobin::GetTypeId (void) {
    static TypeId tid = TypeId ("ns3::DeficitRoundRobin")
        .SetParent<DiffServ>()
        .AddConstructor<DeficitRoundRobin> ()
        ;
    return tid;
}

bool
DeficitRoundRobin::DoEnqueue (Ptr<ns3::Packet> p) 
{
    //outfile<<"In Enqueue" << endl;
    p->Print(outfile);
    outfile << endl;
    uint32_t queueNum = Classify (p);
    //outfile<<"Priority Level " << priority_level << endl;
    if (queueNum < q_class.size ()) 
    {
       if (ExistsInActiveList (queueNum) == false) 
       {
          m_activeList.push (queueNum);
          m_deficitCounter[queueNum] = q_class[queueNum]->getQuantum ();
          m_activeListSet.insert (queueNum);
       }
       bool res = q_class[queueNum]->enqueue (p);
       if (res == false) 
       {
          outfile<<"DRR Queue full (at max packets) -- droppping pkt"<<endl;
          DropBeforeEnqueue (p);
       }
     return res;
    }
    //outfile <<"Packet enqueued" <<Simulator::Now ().GetSeconds ()<< endl;
    else 
    {
       return false;
    }
}


uint32_t
DeficitRoundRobin::Schedule () 
{
    outfile <<"   In Schedule method"<<endl;
    if (m_activeList.size ()> 0) 
    {
        for (unsigned int i = 0; i < m_activeList.size (); i++) 
        {
            int currQueue = m_activeList.front ();
            uint32_t curDc = 0;
            //One round completed, quantum values are added to deficit counter values for active queues
            if (m_servicedQueues == m_activeList.size ()) 
            {
                outfile << "Round Completed"<<endl;
                for (unsigned int i = 0; i< m_deficitCounter.size (); i++) 
                {
                    if(ExistsInActiveList (i)) 
                    {
                      m_deficitCounter[i] += q_class[i]->getQuantum ();
                    }
                }
                m_servicedQueues = 0;
            }  
            // Calculating the cur deficit counter
            curDc = m_deficitCounter[currQueue];
            
            Ptr<ns3::Packet> p = q_class[currQueue]->peek ();
            if (p != nullptr) 
            {   
                outfile<<"Packet not empty"<<endl;
                uint32_t packetSize =  p->GetSize ();
                if (packetSize <= curDc) 
                {
                    outfile <<"Dequeueing packet queue : " <<currQueue <<endl;
                    curDc = curDc - packetSize;
                    m_deficitCounter[currQueue] = curDc;
                    return currQueue;
                } 
                else 
                {
                    outfile<<"Packet size greater the current deficit counter "<<packetSize<<" "<<curDc<<endl;
                    m_servicedQueues++;
                    m_deficitCounter[currQueue] = curDc;
                    m_activeList.push (currQueue);
                    m_activeList.pop ();
                }   
            } 
            else 
            {
                outfile<<"Queue Empty: " << currQueue<<endl;
                m_activeList.pop ();
                m_deficitCounter[currQueue] = 0;
                m_activeListSet.erase (currQueue);
            }
       }
       outfile<<"Here after completing for loop"<<endl;
       return q_class.size () + 1;
    }  
    else 
    {
        outfile<<"Here m_activeList empty"<<endl;
        return q_class.size () + 1;
    } 
}

uint32_t
DeficitRoundRobin::Classify (Ptr<ns3::Packet> p) 
{
   //outfile <<"In Classify-----------------" << endl;
   for (unsigned int i = 0; i < q_class.size (); i++) {
       if (q_class[i]->match (p)) 
       {
          outfile << "Classify " << i << endl;
          return i;
       }
   }
   return q_class.size () + 1;
}


bool
DeficitRoundRobin::ExistsInActiveList (int queueNum) 
{
   return m_activeListSet.count (queueNum);
}

uint32_t 
DeficitRoundRobin::GetQueueToPeek () const
{
    for (unsigned int i = 0; i < m_activeList.size (); i++) 
    {
        int currQueue = m_activeList.front ();
        if (!q_class[currQueue]->empty ()) 
        {
            return currQueue;
        }
    }
    return q_class.size () + 1;
}


Ptr<const ns3::Packet>
DeficitRoundRobin::Peek (void) const 
{
   uint32_t queueNum = GetQueueToPeek ();
   if (queueNum < q_class.size ())
   {
       return q_class[queueNum]->peek ();
   }
   return 0;
   
}


} // namespace ns3

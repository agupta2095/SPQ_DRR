/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "diff-serv.h"

namespace ns3 {

DiffServ::DiffServ()
{

}

DiffServ::~DiffServ()
{

}

bool
DiffServ::Enqueue(Ptr<Packet> p)
{
    return DoEnqueue(p);
}

Ptr<Packet>
DiffServ::Dequeue(void)
{
    return DoDequeue();
}

bool
DiffServ::DoEnqueue(Ptr<Packet> p)
{
    uint32_t queueNum = Classify (p);

    bool res = q_class[queueNum]->enqueue (p);
    if (res == false) 
    {
        DropBeforeEnqueue (p) ;
    }
    return res;
}

Ptr<Packet>
DiffServ::DoDequeue(void)
{
    uint32_t queueNum = Schedule ();
    if (queueNum < q_class.size ()) 
    {
        Ptr<ns3::Packet> p = q_class[queueNum]->dequeue ();
        return p;
    }
    return 0;
}


Ptr<ns3::Packet> 
DiffServ::Remove (void)  
{  
    return 0;
}

} // namespace ns3
/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef DIFF_SERV_CLASS_H
#define DIFF_SERV_CLASS_H

#include <queue>
#include "ns3/packet.h"
#include "ns3/queue.h"
#include "ns3/nstime.h"
#include <vector>

#include "traffic-class.h"

namespace ns3 
{

/**
 * \brief Base class for differentiable services queues.
 *
 * This class provides the common interface for queues implementing
 * differentiable services functionalities.
 */
class DiffServ : public Queue<ns3::Packet>
{

    public:
        std::vector<TrafficClass*> q_class; /**< Vector of TrafficClass pointers. */

        /**
         * \brief Enqueue a packet in the queue.
         * \param p the packet to enqueue
         * \returns true if the packet was successfully enqueued, false otherwise
         *
         * This method calls DoEnqueue for the actual enqueueing of the packet.
        */
        virtual bool Enqueue(Ptr<ns3::Packet> p);

        /**
          * \brief Dequeue a packet from the queue.
          * \returns the dequeued packet, or null if the queue is empty
          *
          * This method calls DoDequeue for the actual dequeueing of the packet.
        */
        virtual Ptr<ns3::Packet> Dequeue(void);

        /**
          * \brief Perform the actual packet enqueue.
          * \param p the packet to enqueue
          * \returns true if the packet was successfully enqueued, false otherwise
          *
          * This method should be implemented by the derived classes to actually
          * perform the enqueue of the packet. This method is called by Enqueue.
        */
        virtual bool DoEnqueue(Ptr<ns3::Packet> p);

        /**
          * \brief Perform the actual packet dequeue.
          * \returns the dequeued packet, or null if the queue is empty
          *
          * This method should be implemented by the derived classes to actually
          * perform the dequeue of the packet. This method is called by Dequeue.
        */
        virtual Ptr<ns3::Packet> DoDequeue(void);

        /**
          * \brief Peek at the next packet in the queue.
          * \returns the next packet in the queue, or null if the queue is empty
          *
          * This method returns the packet from the next scheduled queue.
        */
        virtual Ptr<const ns3::Packet> Peek(void) const = 0;

        /**
          * \brief Schedule the packets in the queue.
          * \returns the number of packets scheduled
          *
          * This method should be implemented by the derived classes to actually
          * schedule the packets in the queue.
        */
        virtual uint32_t Schedule(void) = 0;

        /**
          * \brief Classify the packet into the corresponding traffic class.
          * \param p the packet to classify
          * \returns the traffic class index
          *
          * This method should be implemented by the derived classes to actually
          * classify the packet into the corresponding traffic class.
        */
        virtual uint32_t Classify(Ptr<ns3::Packet> p) = 0 ;

        /**
          * \brief This method is pure virtual function in Queue class. 
          *  Derived class need to call DoRemove() with position of the packet to be removed.
          * \returns 0 for current implementation
          *
        */
        virtual Ptr<ns3::Packet> Remove(void);


        //NS_LOG_TEMPLATE_DECLARE;

        DiffServ();
        ~DiffServ();
};

}

#endif /* DIFF_SERV_CLASS_H */

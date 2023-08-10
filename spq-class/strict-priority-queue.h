#ifndef STRICT_PRIORITY_QUEUE_H
#define STRICT_PRIORITY_QUEUE_H

#include <queue> // standard library header files first
#include <vector>

#include "ns3/packet.h" // ns-3 header files next
#include "ns3/queue.h"
#include "diff-serv.h" // custom header files last

namespace ns3 
{

/**
 * \brief A strict priority queue implementation for DiffServ.
 *
 * This class represents a strict priority queue which can be used to implement
 * Differentiated Services (DiffServ) in a network simulator. It is derived from
 * the DiffServ base class and overrides its methods. 
 */
class StrictPriorityQueue : public DiffServ 
{
    public:
        /**
         * \brief Get the TypeId.
         * \return the object TypeId
         */
        static TypeId GetTypeId (void);

        /**
         * \brief Create queues based on the configuration file.
         *
         * This method creates queues based on the configuration file passed as input.
         *
         * \param config_file the name of the configuration file
         */
        void createQueues(char* config_file); 

        /**
         * \brief Default constructor.
         */
        StrictPriorityQueue();

        /**
         * \brief Default destructor.
         */
        ~StrictPriorityQueue();

    private:
        /**
         * \brief Classify the packet.
         *
         * This method classifies the packet.
         *
         * \param p the packet to classify
         * \return the classification of the packet
         */
        uint32_t Classify (Ptr<ns3::Packet> p);

        /**
         * \brief Schedule packets for transmission.
         *
         * This method schedules packets for transmission.
         *
         * \return the number of packets scheduled
         */
        uint32_t Schedule();

        /**
         * \brief Peek at the next packet in the queue.
         *
         * This method peeks at the next packet in the queue.
         *
         * \return the packet peeked
         */
        Ptr<const ns3::Packet> Peek (void) const;

};

} // namespace ns3

#endif /* STRICT_PRIORITY_QUEUE_H */

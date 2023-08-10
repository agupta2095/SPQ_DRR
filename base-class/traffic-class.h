#ifndef TRAFFICCLASS_H
#define TRAFFICCLASS_H
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/pointer.h"
#include <queue>
#include <vector>
#include "ns3/ppp-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"
#include "ns3/queue.h"
#include "ns3/nstime.h"
#include "filter.h"

namespace ns3
{

/**
 * TrafficClass class represents a traffic class with a priority level, a max packet limit, a quantum, and filters for packet matching.
 * Packets are stored in a queue and dequeued based on the scheduling algorithm.
 */
class TrafficClass
{
	public:
		// Class variables
		uint32_t m_packets; /// Number of packets in the queue 
		uint32_t m_max_packets; ///< Maximum number of packets that can be enqueued 
		uint32_t m_quantum; /// Quantum value for the scheduling algorithm 
		uint32_t m_priority_level; /// Priority level of the traffic class 
		std::queue<Ptr<ns3::Packet> > m_queue; /// Queue for storing packets 
		std::vector<ns3::Filter*> filters; /// Filters for packet matching 

	public:

		/**
		\ingroup trafficcontrol
		\brief Constructor for the TrafficClass class
		Initializes a TrafficClass object.
		*/
		TrafficClass();
		// Class methods
		/**
		 * Set the number of packets in the queue.
		 *
		 * \param num Number of packets
		 */
		void setPackets (uint32_t num);

		/**
		 * Get the number of packets in the queue.
		 *
		 * \return Number of packets
		 */
		uint32_t getPackets ();

		/**
		 * Set the maximum number of packets that can be enqueued.
		 *
		 * \param num Maximum number of packets
		 */
		void setMaxPackets (uint32_t num);

		/**
		 * Get the maximum number of packets that can be enqueued.
		 *
		 * \return Maximum number of packets
		 */
		uint32_t getMaxPackets ();
		
		/**
		 * Set the priority level of the traffic class.
		 *
		 * \param num Priority level
		 */
		void setPriorityLevel (uint32_t num);

		/**
		 * Get the priority level of the traffic class.
		 *
		 * \return Priority level
		 */
		uint32_t getPriorityLevel ();
		
		/**
		 * Set the filters for packet matching.
		 *
		 * \param filterVec Vector of filters
		 */
		void setFilters (std::vector<ns3::Filter*>& filterVec) {filters = filterVec;}

		/**
		 * Match a packet with the filters of the traffic class.
		 *
		 * \param p Packet to be matched
		 * \return True if the packet matches the filters, false otherwise
		 */
		bool match (Ptr<ns3::Packet> p);
		
		/**
		 * Dequeue a packet from the traffic class queue.
		 *
		 * \return Pointer to the dequeued packet
		 */
		Ptr<ns3::Packet> dequeue ();

		/**
		 * Peek at the next packet in the traffic class queue without dequeuing it.
		 *
		 * \return Pointer to the next packet in the queue
		 */
		Ptr<ns3::Packet> peek ();

		/**
		 * Enqueue a packet in the traffic class queue.
		 *
		 * \param packet Pointer to the packet to be enqueued
		 * \return True if the packet was successfully enqueued, false if the queue is full
		 */
		bool enqueue (Ptr<ns3::Packet> packet);

        /**
         * Set the quantum value for the scheduling algorithm.
         *
         * \param num Quantum value
         */
        void setQuantum (uint32_t num);

        /**
         * Get the quantum value for the scheduling algorithm.
         *
         * \return Quantum value
         */
        uint32_t getQuantum ();

        /**
         *  Check if the queue is empty or not.
         * 
         * \return bool value
         */
        bool empty();
		
};

}

#endif //TRAFFIC_CLASS
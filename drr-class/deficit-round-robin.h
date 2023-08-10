#ifndef DEFICIT_ROUND_ROBIN_H
#define DEFICIT_ROUND_ROBIN_H

#include <queue>
#include <vector>
#include <unordered_set>
#include "ns3/packet.h"
#include "ns3/queue.h"
#include "ns3/ipv4-header.h"
#include "diff-serv.h"


namespace ns3 
{

/**
 * \brief Class implements Deficit Round Robin QoS mechanism
 *
 * This class inherits from DiffServ class and provides implementation of Schedule(), Classify(), DoEnqueue() methods.
 * Deficit Round Robin is a fair queueing mechanism that aims to allocate bandwidth to all flows while giving larger 
 * allocation to flows with larger quantum vale
 */

class DeficitRoundRobin : public DiffServ 
{
    public:
      /**
       * \brief Get the TypeId.
       *
       * \return TypeId object.
       */
      static TypeId GetTypeId (void);

      /**
       * \brief Create queues from the configuration file.
       *
       * \param config_file Configuration file.
       */
      void createQueues (char* config_file);

      /**
       * \brief Constructor.
       */
      DeficitRoundRobin ();

      /**
       * \brief Destructor.
       */
      ~DeficitRoundRobin ();

    private:
      /**
       * \brief Enqueue the packet to the queue.
       *
       * \param p Packet to enqueue.
       *
       * \return True if enqueue is successful, false otherwise.
       */
      bool DoEnqueue (Ptr<ns3::Packet> p) override;

      /**
       * \brief Schedule the packet from the queue.
       *
       * \return The index of the queue to serve.
       */
      uint32_t Schedule ();

      /**
       * \brief Classify the packet to the respective queue.
       *
       * \param p Packet to classify.
       *
       * \return The index of the queue to serve.
       */
      uint32_t Classify (Ptr<ns3::Packet> p) ;

      /**
       * \brief Check if the queue exists in the active queue list.
       *
       * \param queueNum Queue number to check.
       *
       * \return True if the queue exists, false otherwise.
       */
      bool ExistsInActiveList (int queueNum);

      /**
       * \brief Get the first packet from the scheduled queue without dequeuing it.
       *
       * \return Packet from the queue.
       */
      Ptr<const ns3::Packet> Peek (void) const;
      

      /**
       *  \brief Get the queue number to peek packet from 
       * 
       *  \return queue number.
       */
      uint32_t GetQueueToPeek () const;

      unsigned int m_servicedQueues; ///< Number of queues serviced in a round.
      std::unordered_set<int> m_activeListSet; ///< Set of active queues, i.e., non-empty queues.
      std::queue<int> m_activeList; ///< Queue to pop and push active queue numbers.
      std::vector<int> m_deficitCounter; ///< Deficit counter of all queues.
};

} // namespace ns3

#endif /* DEFICIT_ROUND_ROBIN_H */

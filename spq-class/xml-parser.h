#ifndef XML_PARSER_H
#define XML_PARSER_H

#include "tinyxml2.h"
#include <iostream>
#include <unordered_map>
#include "filter.h"

using namespace tinyxml2;
using namespace std;

namespace ns3 
{

/**
 * \brief Class to parse XML files for Strict Priority Queueing
 */
class XmlParserSPQ 
{
    public:
        /** \brief Parse the XML file and populate the queueMap and priorityMaxPacketsMap */
        int parse_xml (const char* filename);
        
        /** \brief Get the map of queue priority vs vector of filter */
        const unordered_map<uint32_t, std::vector<ns3::Filter*> >& getQueueMap () const {return m_queueMap;}
        
        /** \brief Get the maximum packet size for a given priority */
        uint32_t getMaxPacketSize (uint32_t priority);

    private:
        // Map of queue priority vs vector of filter
        unordered_map<uint32_t, std::vector<ns3::Filter*> > m_queueMap;
        unordered_map<uint32_t, uint32_t> m_priorityMaxPacketsMap;
        
        /** \brief Add the filter vector to the queueMap for the given priority */
        void addToQueue (uint32_t priority, std::vector<ns3::Filter*>& filterVec) {m_queueMap.insert (std::make_pair (priority, filterVec));}
        
        /** \brief Add the maximum packet size for the given priority */
        void addToPacketSizeMap (uint32_t priority, uint32_t maxPacketSize) 
        {
            m_priorityMaxPacketsMap.insert (std::make_pair (priority, maxPacketSize));
        }       
};

/**
 * \brief Class to parse XML files for Deficit Round Robin
 */
class XmlParserDRR 
{
    public:
        /** \brief Parse the XML file and populate the queueMap and quantumAndMaxPacketsMap */
        int parse_xml (const char* filename);
        
        /** \brief Get the queueMap */
        const std::vector<std::vector<ns3::Filter*> >& getQueueMap () const {return m_queueMap;}
        
        /** \brief Get the quantum and maximum packet size for a given queue number */
        std::pair<uint32_t, uint32_t> getQuantumAndMaxPacketSize (uint32_t queueNum);

    private:
        // Map of queue priority vs vector of filter
        std::vector<std::vector<ns3::Filter*> > m_queueMap;
        unordered_map<uint32_t, std::pair<uint32_t, uint32_t>> m_quantumAndMaxPacketsMap;
        
        /** \brief Add the filter vector to the queueMap */
        void addToQueue (std::vector<ns3::Filter*>& filterVec) {m_queueMap.push_back (filterVec);}
        
        /** \brief Add the maximum packet size and quantum for the given queue number */
        void addToPacketSizeMap (uint32_t queueNum, uint32_t maxPacketSize, uint32_t quantum) 
        {
            m_quantumAndMaxPacketsMap.insert (std::make_pair (queueNum, std::make_pair (quantum, maxPacketSize)));
        }
};

}

#endif /* XML_PARSER_H */
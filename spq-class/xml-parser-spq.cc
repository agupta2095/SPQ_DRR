#include "tinyxml2.h"
#include <iostream>
#include "xml-parser.h"
#include "filter.h"
#include "filter-element.h"
#include "source-ipaddress.h"
#include "destination-ipaddress.h"
#include "source-portnumber.h"
#include "protocol-number.h"
#include "destination-portnumber.h"
#include "source-mask.h"
#include "destination-mask.h"
#include <unordered_map>
#include <string>

using namespace tinyxml2;
using namespace std;

namespace ns3 {

int
XmlParserSPQ::parse_xml (const char* filename) 
{
  
    XMLDocument doc;
    doc.LoadFile (filename);

    // Check for errors
    if (doc.ErrorID () != 0) 
    {
        std::cout << "Error loading XML file: " << doc.ErrorStr () << std::endl;
        return 1;
    }

    XMLElement *acllist = doc.FirstChildElement ("acllist");
    unordered_map<std::string, Filter*> filterMap;

    while (acllist != nullptr) 
    {
        // First acl
        XMLElement *acl = acllist->FirstChildElement ("acl");
    
        while (acl != nullptr) 
        {
            Filter * filter = new Filter ();
            const char *aclId = acl->Attribute ("id");
            std::cout << "ACL ID: " << aclId << std::endl;

            XMLElement *entry = acl->FirstChildElement ("entry");
            while (entry != nullptr) 
            {
                XMLElement *sourceAddress = entry->FirstChildElement ("sourceaddress");
                if (sourceAddress != nullptr) 
                {
                   const char *sourceAddressText = sourceAddress->GetText ();
                   std::cout << "     Source Address: " << sourceAddressText << std::endl;
                   // Convert the text-based source address to an ns3::Ipv4Address
                   ns3::Ipv4Address sourceIpAddress (sourceAddressText);
                   SourceIPAddress * sourceIPObj = new SourceIPAddress (sourceIpAddress);
                   filter->add_element (sourceIPObj);
                   
                }

                XMLElement *sourceMask = entry->FirstChildElement ("sourcemask");
                if (sourceMask != nullptr) 
                {
                   const char *sourceMaskText = sourceMask->GetText ();
                   std::cout << "     Source Mask: " << sourceMaskText << std::endl;
                   // Convert the string to an Ipv4Mask object
                   ns3::Ipv4Mask sourceIpv4Mask = ns3::Ipv4Mask (sourceMaskText);
                   // Create a new SourceMask object using the Ipv4Mask object
                   SourceMask *sourceMaskObj = new SourceMask (sourceIpv4Mask);
                   filter->add_element (sourceMaskObj);
                }

                XMLElement *sourcePortNumber = entry->FirstChildElement ("sourceportnumber");
                if (sourcePortNumber != nullptr) 
                {
                    const char *sourcePortNumberText = sourcePortNumber->GetText ();
                    std::cout << "     Source Port Number: " << sourcePortNumberText << std::endl;
                    // Convert the string to a uint32_t value
                    std::stringstream ss (sourcePortNumberText);
                    uint32_t sourcePortNumberValue;
                    ss >> sourcePortNumberValue;

                    // Create a new SourcePortNumber object using the uint32_t value
                    SourcePortNumber *sourcePortNumberObj = new SourcePortNumber (sourcePortNumberValue);
                    filter->add_element (sourcePortNumberObj);
                }

                XMLElement *destinationAddress = entry->FirstChildElement ("destinationaddress");
                if (destinationAddress != nullptr) 
                {
                   const char *destinationAddressText = destinationAddress->GetText ();
                   std::cout << "     Destination Address: " << destinationAddressText << std::endl;
                   ns3::Ipv4Address destIpAddress (destinationAddressText);
                   DestinationIPAddress * destIPObj = new DestinationIPAddress (destIpAddress);
                   filter->add_element (destIPObj);
                }

                XMLElement *destinationPortNumber = entry->FirstChildElement ("destinationportnumber");
                if (destinationPortNumber != nullptr) 
                {
                    const char *destinationPortNumberText = destinationPortNumber->GetText ();
                    std::cout << "     Destination Port Number: " << destinationPortNumberText << std::endl;
                    std::stringstream ss (destinationPortNumberText);
                    uint32_t destinationPortNumberValue;
                    ss >> destinationPortNumberValue;
                    DestinationPortNumber *destPortNumberObj = new DestinationPortNumber (destinationPortNumberValue);
                    filter->add_element (destPortNumberObj);
                }

                XMLElement *destinationMask = entry->FirstChildElement ("destinationmask");
                if (destinationMask != nullptr) 
                {
                    const char *destinationMaskText = destinationMask->GetText ();
                    std::cout << "     Destination Mask: " << destinationMaskText << std::endl;
                    ns3::Ipv4Mask destIpv4Mask = ns3::Ipv4Mask (destinationMaskText);
                    // Create a new SourceMask object using the Ipv4Mask object
                    DestinationMask *destMaskObj = new DestinationMask (destIpv4Mask);
                    filter->add_element (destMaskObj);
                }

                XMLElement *protocol = entry->FirstChildElement ("protocol");
                if (protocol != nullptr) 
                {
                    const char *protocolText = protocol->GetText ();
                    std::cout << "     Protocol: " << protocolText << std::endl;
                    std::stringstream ss (protocolText);
                    uint32_t protocol;
                    ss >> protocol;

                    ProtocolNumber *protocolObj = new ProtocolNumber (protocol);
                    filter->add_element (protocolObj);

                }

                entry = entry->NextSiblingElement ("entry");
            }
            std::string aclIdStr = aclId;
            filterMap.insert(std::make_pair (aclIdStr, filter));

            acl = acl->NextSiblingElement ("acl");
        }
        acllist = acllist->NextSiblingElement ("acllist");
    }

    // Class list
    XMLElement *classList = doc.FirstChildElement ("classlist");

    while(classList != nullptr) 
    {
        // First class
        XMLElement *cls = classList->FirstChildElement ("class");
        while (cls != nullptr) 
        {
            const char *classId = cls->Attribute ("id");
            const char *aclIdStr = cls->Attribute ("aclid");
            std::cout << "Class ID: " << classId << std::endl;
            //There can be multiple aclIds, as a Queue can have a vector of filter elements
              // Split the value into individual values using a space delimiter
            std::vector<std::string> aclIdValues;
            if (aclIdStr != nullptr) 
            {           
                std::istringstream iss (aclIdStr);
                std::string value;
                while (std::getline (iss, value, ' ')) 
                {
                    aclIdValues.push_back (value);
                    std::cout<<"    ACL ID String: " << value << std::endl;
                }
            }

            XMLElement *maxQueueSizeElem = cls->FirstChildElement ("maxQueueSize");
            tinyxml2::XMLElement* priorityElem = cls->FirstChildElement ("priority");

            // check if all elements exist before getting their text
            if (classId && priorityElem)
            {
                //Default Queue Size = 1000;
                unsigned int maxQueueSizeInt = 1000;
                if (maxQueueSizeElem != nullptr) 
                {
                    maxQueueSizeInt = std::stoi (maxQueueSizeElem->GetText ());
                }
                uint32_t maxQueueSizeVal = static_cast<uint32_t> (maxQueueSizeInt);

                unsigned int priorityInt = 0;
                
                priorityInt = std::stoi (priorityElem->GetText ());
                uint32_t priority = static_cast<uint32_t>(priorityInt);

                std::vector<ns3::Filter*> filterVec;
                for (std::string aclid : aclIdValues) 
                {
                        if (filterMap.count (aclid)) 
                        {
                           filterVec.push_back (filterMap.at (aclid));
                        } 
                        else 
                        {
                            cout << "No such filter with acl id: "<< aclid << std::endl;
                        }
                }
                addToQueue (priority, filterVec);

                addToPacketSizeMap (priority, maxQueueSizeVal);               
                std::cout << "      Max Queue Size: " << maxQueueSizeVal << std::endl;
                std::cout << "      Priority: " << priority << std::endl;
            } 
            else 
            {
                 std::cout << "Priority Level queue class: " << classId << " not specified properly" << std::endl;
            }
            cls = cls->NextSiblingElement ("class");
        }
        classList = classList->NextSiblingElement ("classlist");
    }

    return 0;
}


uint32_t
XmlParserSPQ::getMaxPacketSize (uint32_t priority) 
{
    if (m_priorityMaxPacketsMap.count (priority) != 0) 
    {
        return m_priorityMaxPacketsMap.at (priority);
    } 
    else 
    {
        return 1000;
    }
}

  
} //ns3 namespace




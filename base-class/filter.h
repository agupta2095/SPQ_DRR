#ifndef FILTER_H
#define FILTER_H

#include "ns3/boolean.h"
#include "ns3/ipv4-header.h"
#include "ns3/pointer.h"
#include "ns3/ppp-header.h"
#include "ns3/packet.h"
#include "ns3/tcp-header.h"
#include "ns3/uinteger.h"
#include "ns3/udp-header.h"
#include "filter-element.h"
#include <queue>
#include <vector>

namespace ns3 
{

/**
 * \brief A class that represents a filter consisting of multiple filter elements.
 */
class Filter 
{
	public:
	  /**
	   * \brief Constructs a new Filter object.
	   */
	  Filter ();

	  /**
	   * \brief Adds a filter element to the filter.
	   *
	   * \param[in] element The filter element to add.
	   */
	  void add_element (FilterElement* element);

	  /**
	   * \brief Determines if the given packet matches the filter.
	   *
	   * \param[in] p The packet to test.
	   *
	   * \returns true if the packet matches the filter, false otherwise.
	   */
	  bool match (Ptr<Packet> p);

	  /**
	   * \brief Returns a reference to the filter elements vector.
	   *
	   * \returns a reference to the filter elements vector.
	   */
	  std::vector<FilterElement*>& getFilterElems () { return m_elementVector; }

	private:
	  std::vector<FilterElement*> m_elementVector; ///< The vector of filter elements.
};

} // namespace ns3

#endif // FILTER_H

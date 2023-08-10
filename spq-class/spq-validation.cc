#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/traffic-control-module.h"

#include "ns3/drop-tail-queue.h"
#include "ns3/queue.h"
#include "ns3/traffic-control-module.h"
#include "ns3/pcap-file-wrapper.h"

#include "strict-priority-queue.h"
#include "xml-parser.h"

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE("ScratchSimulator");

int
main (int argc, char* argv[])
{

    // Check if a filename argument was provided
    if (argc < 2) 
    {
       cout << "Usage: " << argv[0] << " <config file>" << endl;
       return 1;
    }

    // open the file
    ifstream config_file (argv[1]);
    if (!config_file) 
    {
       cout << "Failed to open file " << argv[1] << endl;
       return 1;
    }

    GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));


    ns3::Packet::EnablePrinting ();

    // Create nodes
    NodeContainer nodes;
    nodes.Create (3);


    // Setup point-to-point devices and channels for AB link
    PointToPointHelper pointToPointAB;
    pointToPointAB.SetDeviceAttribute ("DataRate", StringValue ("40Mbps"));
    pointToPointAB.SetChannelAttribute ("Delay", StringValue ("2ms"));
    NetDeviceContainer devicesAB = pointToPointAB.Install (nodes.Get (0), nodes.Get (1));
    
  
    // Setup point-to-point devices and channels for BC link
    PointToPointHelper pointToPointBC;
    pointToPointBC.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
    pointToPointBC.SetChannelAttribute ("Delay", StringValue ("2ms"));
    NetDeviceContainer devicesBC = pointToPointBC.Install (nodes.Get (1), nodes.Get (2));


    ObjectFactory m_queueFactory ("ns3::StrictPriorityQueue");

    Ptr<DiffServ> queueN1 = m_queueFactory.Create<DiffServ> ();
    Ptr<PointToPointNetDevice> devN1 = DynamicCast<PointToPointNetDevice> (devicesBC.Get (0));
    Ptr<StrictPriorityQueue> spqQueueN1 = StaticCast<StrictPriorityQueue> (queueN1);
    spqQueueN1->createQueues (argv[1]);
    devN1->SetQueue (queueN1); 

    // Install Internet stack
    InternetStackHelper stack;
    stack.Install (nodes);
    

    // Assign IP addresses
    Ipv4AddressHelper address;
    address.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfacesAB = address.Assign (devicesAB);
    address.SetBase ("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer interfacesBC = address.Assign (devicesBC);
    
    
    // Enable routing
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    // Create Applications A and B
    uint16_t sinkPort1 = 912;
    uint16_t sinkPort2 = 913;

    PacketSinkHelper sinkHelper1 ("ns3::UdpSocketFactory", InetSocketAddress (interfacesBC.GetAddress (1), sinkPort1));
    ApplicationContainer sinkApps1 = sinkHelper1.Install (nodes.Get (2));
    sinkApps1.Start (Seconds (0));
    sinkApps1.Stop (Seconds (40));

    PacketSinkHelper sinkHelper2 ("ns3::UdpSocketFactory", InetSocketAddress (interfacesBC.GetAddress (1), sinkPort2));
    ApplicationContainer sinkApps2 = sinkHelper2.Install (nodes.Get (2));
    sinkApps2.Start (Seconds (0));
    sinkApps2.Stop (Seconds (40));
 
    const Address appAAddress (InetSocketAddress (interfacesBC.GetAddress (1), sinkPort1));
    const Address appBAddress (InetSocketAddress (interfacesBC.GetAddress (1), sinkPort2));
    // Create UDP source applications
    UdpClientHelper sourceA (appAAddress);
    sourceA.SetAttribute ("MaxPackets", UintegerValue (100000));
    sourceA.SetAttribute ("Interval", TimeValue (Seconds  (0.0004)));
    sourceA.SetAttribute ("PacketSize", UintegerValue (1060));

    UdpClientHelper sourceB (appBAddress);
    sourceB.SetAttribute ("MaxPackets", UintegerValue (100000));
    sourceB.SetAttribute ("Interval", TimeValue (Seconds (0.0004)));
    sourceB.SetAttribute ("PacketSize", UintegerValue (1060));

    // Set application A to start after application B
    ApplicationContainer sourceAppsB = sourceB.Install (nodes.Get(0));
    sourceAppsB.Start (Seconds (0));
    sourceAppsB.Stop (Seconds (40));

    ApplicationContainer sourceAppsA = sourceA.Install (nodes.Get(0));
    sourceAppsA.Start (Seconds (12));
    sourceAppsA.Stop (Seconds (33));

    
    // Get the net devices connected to node 1
    pointToPointAB.EnablePcap ("Node1_AB", devicesAB.Get (1), false);
    pointToPointBC.EnablePcap ("Node1_BC", devicesBC.Get (0), false);


    // Run the simulator
    Simulator::Stop (Seconds (40));
    Simulator::Run ();
    
   
    Simulator::Destroy ();

    return 0;
}



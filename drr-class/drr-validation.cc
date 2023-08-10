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
#include "deficit-round-robin.h"


using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE("DRRValidator");

int main(int argc, char* argv[]) 
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
    

    // Default configuration
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

    // Install DRR queue
    ObjectFactory m_queueFactory ("ns3::DeficitRoundRobin");
    Ptr<DiffServ> queueN1 = m_queueFactory.Create<DiffServ> ();
    Ptr<DeficitRoundRobin> drrQueueN1 = StaticCast<DeficitRoundRobin> (queueN1);
    drrQueueN1->createQueues (argv[1]);

    Ptr<PointToPointNetDevice> devN1 = DynamicCast<PointToPointNetDevice> (devicesBC.Get(0));
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
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // Create Applications A, B, and C
    uint16_t sinkPort1 = 903;
    uint16_t sinkPort2 = 904;
    uint16_t sinkPort3 = 905;

    // Create packet sinks
    PacketSinkHelper packetSinkHelper1
            ("ns3::UdpSocketFactory", InetSocketAddress (interfacesBC.GetAddress (1), sinkPort1));
    PacketSinkHelper packetSinkHelper2
            ("ns3::UdpSocketFactory", InetSocketAddress (interfacesBC.GetAddress (1), sinkPort2));
    PacketSinkHelper packetSinkHelper3
            ("ns3::UdpSocketFactory", InetSocketAddress (interfacesBC.GetAddress (1), sinkPort3));

    ApplicationContainer sinkApps1 = packetSinkHelper1.Install (nodes.Get (2));
    sinkApps1.Start (Seconds (0));
    sinkApps1.Stop (Seconds (30));

    ApplicationContainer sinkApps2 = packetSinkHelper2.Install (nodes.Get (2));
    sinkApps2.Start (Seconds (0));
    sinkApps2.Stop (Seconds (30));

    ApplicationContainer sinkApps3 = packetSinkHelper3.Install (nodes.Get (2));
    sinkApps3.Start (Seconds (0));
    sinkApps3.Stop (Seconds (30));

    // Create bulk send applications
    const Address appAAddress (InetSocketAddress (interfacesBC.GetAddress (1), sinkPort1));
    const Address appBAddress (InetSocketAddress (interfacesBC.GetAddress (1), sinkPort2));
    const Address appCAddress (InetSocketAddress (interfacesBC.GetAddress (1), sinkPort3));

    UdpClientHelper sourceA (appAAddress);
    sourceA.SetAttribute ("MaxPackets", UintegerValue (100000));
    sourceA.SetAttribute ("Interval", TimeValue (Seconds (0.0005)));
    sourceA.SetAttribute ("PacketSize", UintegerValue (670));

    UdpClientHelper sourceB (appBAddress);
    sourceB.SetAttribute ("MaxPackets", UintegerValue (100000));
    sourceB.SetAttribute ("Interval", TimeValue (Seconds (0.0005)));
    sourceB.SetAttribute ("PacketSize", UintegerValue (670));
   
    UdpClientHelper sourceC (appCAddress);
    sourceC.SetAttribute ("MaxPackets", UintegerValue (100000));
    sourceC.SetAttribute ("Interval", TimeValue (Seconds (0.0005)));
    sourceC.SetAttribute ("PacketSize", UintegerValue (670));



    // Start all applications together
    ApplicationContainer sourceAppsB = sourceB.Install (nodes.Get (0));
    sourceAppsB.Start (Seconds (0));
    sourceAppsB.Stop (Seconds (30));

    ApplicationContainer sourceAppsA = sourceA.Install (nodes.Get (0));
    sourceAppsA.Start (Seconds (0));
    sourceAppsA.Stop (Seconds (30));

    ApplicationContainer sourceAppsC = sourceC.Install (nodes.Get (0));
    sourceAppsC.Start (Seconds (0));
    sourceAppsC.Stop (Seconds (30));



    // Get the net devices connected to node 1
    pointToPointAB.EnablePcap ("NodeDrr1_AB", devicesAB.Get (1), false);
    pointToPointBC.EnablePcap ("NodeDrr1_BC", devicesBC.Get (0), false);


    // Run the simulator
    Simulator::Stop(Seconds(30));
    Simulator::Run ();

    // Clean up
    Simulator::Destroy ();

    return 0;
}
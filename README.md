# SPQ_DRR
implement two quality-of-service (QoS) mechanisms: SPQ and DRR in ns-3 network simulator

# Differentiated Services Implementation in ns-3

This project aims to implement a set of differentiated services using the ns-3 simulation environment. The project involves creating a base class that contains common structures among the differentiated services and implementing two quality-of-service (QoS) mechanisms: SPQ and DRR. Finally, the implemented services will be validated and verified using ns-3 simulation runs.

## Developers

- Developer 1: [Akanksha Gupta]
- Developer 2: [YaoMin Zhang]

## Requirements

To use this project, you will need the following:

- ns-3 simulation environment version 3.36 or later
- C++ compiler (GCC or Clang). Developers have used gcc 9.5.0 version to compile the code.
- Python3 version 3.10 or above

## Prerequisites

Before starting to use this project, you need to have ns-3.x installed on your system. You can follow the instructions provided on the official ns-3 website or use the following commands:

### Method 1: Steps to install ns-3.38
1. wget https://www.nsnam.org/releases/ns-allinone-3.38.tar.bz2
2. tar xjf ns-allinone-3.38.tar.bz2
3. cd ns-allinone-3.38/ns-3.38
4. ./ns3 build
To verify installation
 1. ./test.py --no-build
 2. ./ns3 run first


## Installation and Configuration

To use this project, follow these steps:

1. Extract all the files and folder from the zip file :- Project2.zip
2. Make two directories in the scratch folder located under ns-3.38 folder. One for SPQ verification and validation and other for DRR verification and validation
3. Copy all the files from base-class, tinyxml2 and spq-class in the folder for SPQ verification.
4. Similarly, copy all the files from base-class, tinyxml2 and drr-class in the folder for DRR verification.

Note:-
1. To set the network filter properties and queue characteristics for the QoS mechanisms 
2. For the purpose of this project, we have created a 3 node topology, and provided configuration file to to set the network filter properties and queue characteristics for the QoS mechanisms
3. The files are located in spq-class and drr-class folders and named spq-config.xml and drr-config.xml respectively
4. For the SPQ configuration, the queue with priroity value 0 is the highest priority queue.


## Building and Running the Project

1. To build and run the project go to ns3.3x directory, use the following command
   For SPQ:-
   ./ns3 run "scratch/<spq folder name>/spq-validation.cc <full path to spq config file>"
   For DRR:-
   ./ns3 run "scratch/<drr folder name>/drr-validation.cc <full path to drr config file>"

2. The simulation will generate two pcap files
   1. One for capturing the simulation before applying QoS discipline and other after applying QoS discipline.
   2. The pre_SPQ and pre_DRR PCAP files are captured on the second node of netdevice AB
   3. The post_SPQ and post_DRR PCAP files are captured on the first node of netdevice BC   	

3. User can analyze the pcap files using Wireshark

## Wireshark I/O Graphs
  1. Open Wireshark and select File -> Open from the menu.
  2. Browse to the directory where the pcap files are located and select the desired file.
  3. Click on the Statistics menu and select I/O Graphs.
  4. Configure the I/O graph settings, such as the Y axis factor(1) and the graph style (line), colors, and add the following display filters:-
     For SPQ:-
     1. udp.dstport == 912 (port with high priorty)
     2. udp.dstport == 913 (port with low priority)
     For DRR:-
     1. udp.dstport == 903 (port with quantum value 400)
     2. udp.dstport == 904 (port with quantum value 800)
     3. udp.dstport == 905 (port with quantum value 1200)


## Configuration File
 To specify the filter elements the user can use the following tags:-
  1.  Destination IP Address : <destinationaddress></destinationaddress>>
  2.  Destination Port Number : <destinationportnumber></destinationportnumber>
  3.  Destination Mask : <destinationmask></destinationmask>
  4.  Protocol : <protocol></protocol>
  5.  Source IP Address : <sourceaddress></sourceaddress>
  6.  Source Port Number : <sourceportnumber></sourceportnumber>
  7. Source Mask :  <sourcemask></sourcemask>
  
## Project Implementation
For detailed information on the project implementation, please refer to the report.pdf file included in this repository. This report contains a comprehensive overview of the project, including a description of the base class, the implementation details for the SPQ and DRR QoS mechanisms, and the validation and verification process.

The implementation of the project is done in C++ using the ns-3 network simulation framework. The implementation includes the creation of a base class that provides a common interface for implementing differentiated services, as well as the implementation of the SPQ and DRR QoS mechanisms.

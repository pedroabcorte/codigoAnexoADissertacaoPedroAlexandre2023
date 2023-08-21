#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/netanim-module.h"
#include "ns3/flow-monitor-module.h"
#include <random>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("experiment_v6");

int
main (int argc, char *argv[])
{
  bool verbose = true; // Decide whether logging info should be printed in the Terminal or not
  bool tracing = true; // Decide whether simulation traces should be saved to files or not
  double globalStopTime = 10; // The stop time used everywhere

  uint32_t apNodeCount = 10; // Number of Access Point nodes
  uint32_t staNodeCount = 3; // Number of Station per AP node

  if (verbose)
    {
      // Log UDP Echo Client data in the Terminal
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);

      // Log UDP Echo Server data in the Terminal
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

  // Create the container for all of the nodes ===========================================================
  NodeContainer apNodes;
  NodeContainer staNodes[apNodeCount];

  apNodes.Create (apNodeCount);
  for (uint32_t i = 0; i < apNodeCount; i++)
    {
      staNodes[i].Create (staNodeCount);
    }

  // Create all the helpers needed for networking (they won't be used right away) ========================
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default (); // Channel
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default (); // Physical layer
  WifiHelper wifi; // General WiFi helper
  WifiMacHelper mac; // MAC protocol
  /* The SSID is not created here, as it is not necessary */
  MobilityHelper mobility; // Generic Mobility (needed for wireless networks)
  InternetStackHelper stack; // Internet stack helper
  Ipv4AddressHelper address; // IP address helper

  /// Set up the basic parameters for PHY and WiFi
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager"); // Adaptive Advanced Routing and Forwarding

  // Create the container for all net devices (part of the nodes) ========================================
  NetDeviceContainer apNetDevices[apNodeCount];
  NetDeviceContainer staNetDevices[apNodeCount];

  // Install the devices in the nodes within the network =================================================
  for (uint32_t i = 0; i < apNodeCount; i++)
    {
      /// Create the SSID for this loop
      std::ostringstream oss;
      oss << "SSiD-" << i;
      Ssid ssid = Ssid (oss.str ());

      /// Update the phy channel to avoid Network Number Confusion
      phy.SetChannel (channel.Create ()); // Set the PHY channel while creating it

      /// Set one AP node to currentSsid
      mac.SetType ("ns3::ApWifiMac", "Ssid", SsidValue (ssid));
      apNetDevices[i] = wifi.Install (phy, mac, apNodes.Get (i));

      /// Set its Station nodes to the same currentSsid
      mac.SetType ("ns3::StaWifiMac", "Ssid", SsidValue (ssid), "ActiveProbing",
                   BooleanValue (false));
      staNetDevices[i] = wifi.Install (phy, mac, staNodes[i]);
    }

  // Install the mobility aspect of the nodes ============================================================
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator", "Z", DoubleValue (1), "MinX",
                                 DoubleValue (0.0), "MinY", DoubleValue (0.0), "DeltaX",
                                 DoubleValue (5.0), "DeltaY", DoubleValue (10.0), "GridWidth",
                                 UintegerValue (3), "LayoutType", StringValue ("RowFirst"));
  mobility.Install (apNodes);
  for (uint32_t i = 0; i < apNodeCount; i++)
    {
      mobility.Install (staNodes[i]);
    }

  // Set up the internet stack ===========================================================================
  stack.Install (apNodes);
  for (uint32_t i = 0; i < apNodeCount; i++)
    {
      stack.Install (staNodes[i]);
    }

  // Assign addresses to the nodes =======================================================================
  Ipv4InterfaceContainer wifiApInterface[apNodeCount];
  Ipv4InterfaceContainer wifiStaInterface[apNodeCount];

  /// Create IPs for this loop (the 6th digit will be altered per loop)
  char base_ip[16];
  uint8_t sect_1 = 10;
  uint8_t sect_2 = 1;
  uint8_t sect_3 = 1;
  uint8_t sect_4 = 0;

  for (uint32_t i = 0; i < apNodeCount; i++)
    {
      /// Update the base_ip to use the 4 sectors
      snprintf (base_ip, 16, "%d.%d.%d.%d", sect_1, sect_2, sect_3, sect_4);

      /// Update the address helper's base IP
      address.SetBase (base_ip, "255.255.255.0");

      /// Assign IP addresses to the i-th group nodes
      wifiApInterface[i] = address.Assign (apNetDevices[i]); // The i-th contains one AP node
      wifiStaInterface[i] =
          address.Assign (staNetDevices[i]); // The i-th contains staNodeCount Station nodes

      /// Update sect_3 so that base IP is different next loop
      sect_3++;
    }

  // Create the UDP Echo application and all the necessary parts =========================================
  //// PS.: A lot of these cannot be created inside a FOR loop, there is no default constructor. Check apNodeCount.

  /// Create the servers on the AP nodes
  UdpEchoServerHelper echoServer_0 (9);
  UdpEchoServerHelper echoServer_1 (9);
  UdpEchoServerHelper echoServer_2 (9);
  UdpEchoServerHelper echoServer_3 (9);
  UdpEchoServerHelper echoServer_4 (9);
  UdpEchoServerHelper echoServer_5 (9);
  UdpEchoServerHelper echoServer_6 (9);
  UdpEchoServerHelper echoServer_7 (9);
  UdpEchoServerHelper echoServer_8 (9);
  UdpEchoServerHelper echoServer_9 (9);

  ApplicationContainer serverApps[apNodeCount];
  serverApps[0] = echoServer_0.Install (apNodes.Get (0));
  serverApps[1] = echoServer_1.Install (apNodes.Get (1));
  serverApps[2] = echoServer_2.Install (apNodes.Get (2));
  serverApps[3] = echoServer_3.Install (apNodes.Get (3));
  serverApps[4] = echoServer_4.Install (apNodes.Get (4));
  serverApps[5] = echoServer_5.Install (apNodes.Get (5));
  serverApps[6] = echoServer_6.Install (apNodes.Get (6));
  serverApps[7] = echoServer_7.Install (apNodes.Get (7));
  serverApps[8] = echoServer_8.Install (apNodes.Get (8));
  serverApps[9] = echoServer_9.Install (apNodes.Get (9));

  for (uint32_t i = 0; i < apNodeCount; i++)
    {
      serverApps[i].Start (Seconds (1));
      serverApps[i].Stop (Seconds (globalStopTime));
    }

  /// Create the clients on the Station nodes
  UdpEchoClientHelper echoClient_0 (wifiApInterface[0].GetAddress (0), 9);
  UdpEchoClientHelper echoClient_1 (wifiApInterface[1].GetAddress (0), 9);
  UdpEchoClientHelper echoClient_2 (wifiApInterface[2].GetAddress (0), 9);
  UdpEchoClientHelper echoClient_3 (wifiApInterface[3].GetAddress (0), 9);
  UdpEchoClientHelper echoClient_4 (wifiApInterface[4].GetAddress (0), 9);
  UdpEchoClientHelper echoClient_5 (wifiApInterface[5].GetAddress (0), 9);
  UdpEchoClientHelper echoClient_6 (wifiApInterface[6].GetAddress (0), 9);
  UdpEchoClientHelper echoClient_7 (wifiApInterface[7].GetAddress (0), 9);
  UdpEchoClientHelper echoClient_8 (wifiApInterface[8].GetAddress (0), 9);
  UdpEchoClientHelper echoClient_9 (wifiApInterface[9].GetAddress (0), 9);

  UintegerValue maxPackets = 3;
  TimeValue interval = Seconds (1);
  UintegerValue packetSize = 64;

  echoClient_0.SetAttribute ("MaxPackets", maxPackets);
  echoClient_0.SetAttribute ("Interval", interval);
  echoClient_0.SetAttribute ("PacketSize", packetSize);

  echoClient_1.SetAttribute ("MaxPackets", maxPackets);
  echoClient_1.SetAttribute ("Interval", interval);
  echoClient_1.SetAttribute ("PacketSize", packetSize);

  echoClient_2.SetAttribute ("MaxPackets", maxPackets);
  echoClient_2.SetAttribute ("Interval", interval);
  echoClient_2.SetAttribute ("PacketSize", packetSize);

  echoClient_3.SetAttribute ("MaxPackets", maxPackets);
  echoClient_3.SetAttribute ("Interval", interval);
  echoClient_3.SetAttribute ("PacketSize", packetSize);

  echoClient_4.SetAttribute ("MaxPackets", maxPackets);
  echoClient_4.SetAttribute ("Interval", interval);
  echoClient_4.SetAttribute ("PacketSize", packetSize);

  echoClient_5.SetAttribute ("MaxPackets", maxPackets);
  echoClient_5.SetAttribute ("Interval", interval);
  echoClient_5.SetAttribute ("PacketSize", packetSize);

  echoClient_6.SetAttribute ("MaxPackets", maxPackets);
  echoClient_6.SetAttribute ("Interval", interval);
  echoClient_6.SetAttribute ("PacketSize", packetSize);

  echoClient_7.SetAttribute ("MaxPackets", maxPackets);
  echoClient_7.SetAttribute ("Interval", interval);
  echoClient_7.SetAttribute ("PacketSize", packetSize);

  echoClient_8.SetAttribute ("MaxPackets", maxPackets);
  echoClient_8.SetAttribute ("Interval", interval);
  echoClient_8.SetAttribute ("PacketSize", packetSize);

  echoClient_9.SetAttribute ("MaxPackets", maxPackets);
  echoClient_9.SetAttribute ("Interval", interval);
  echoClient_9.SetAttribute ("PacketSize", packetSize);

  /// Create the client apps (these also cannot be created insider a FOR loop)
  ApplicationContainer clientApps[apNodeCount];
  double startTimeClientApp = 2;

  clientApps[0] = echoClient_0.Install (staNodes[0]);
  clientApps[1] = echoClient_1.Install (staNodes[1]);
  clientApps[2] = echoClient_2.Install (staNodes[2]);
  clientApps[3] = echoClient_3.Install (staNodes[3]);
  clientApps[4] = echoClient_4.Install (staNodes[4]);
  clientApps[5] = echoClient_5.Install (staNodes[5]);
  clientApps[6] = echoClient_6.Install (staNodes[6]);
  clientApps[7] = echoClient_7.Install (staNodes[7]);
  clientApps[8] = echoClient_8.Install (staNodes[8]);
  clientApps[9] = echoClient_9.Install (staNodes[9]);

  for (uint32_t i = 0; i < apNodeCount; i++)
    {
      clientApps[i].Start (Seconds (startTimeClientApp));
      clientApps[i].Stop (Seconds (globalStopTime));
    }

  // Populate the IPv4 routing tables for this network ===================================================
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  // Force the Simulator to stop if the given time is reached ============================================
  Simulator::Stop (Seconds (globalStopTime));

  // Final steps =========================================================================================
  if (tracing == true)
    {
      /// Wireshark tracing (using PCAP files)
      for (uint32_t i = 0; i < apNodeCount; i++)
        {
          phy.EnablePcap ("experiment_v6_PCAP_APnode_" + std::to_string (i),
                          apNetDevices[i].Get (0));
        }

      /// Create a XML (NetAnim) file for the simulation
      AnimationInterface anim ("experiment_v6_NetAnim.xml");
      anim.SetMaxPktsPerTraceFile (1000000); // One million

      /// Set the base coordinates for the animation
      double x = 20;
      double y = x;

      /// Create the tools to add the Station nodes to the animation randomly around the AP Node
      double random_x = 0;
      double random_y = 0;
      double horizontal_lower_bound = x - 5;
      double horizontal_upper_bound = x + 5;
      double vertical_lower_bound = y - 10;
      double vertical_upper_bound = y + 10;
      /*std::uniform_real_distribution<double> horizontal_urd (horizontal_lower_bound,
                                                             horizontal_upper_bound);
      std::uniform_real_distribution<double> vertical_urd (vertical_lower_bound,
                                                           vertical_upper_bound);*/
      std::default_random_engine dre;

      /// Add the nodes to the animation
      for (uint32_t i = 0; i < apNodeCount; i++)
        {
          anim.SetConstantPosition (apNodes.Get (i), x, y);

          // Create the distributions here to take updated values each loop
          std::uniform_real_distribution<double> horizontal_urd (horizontal_lower_bound,
                                                                 horizontal_upper_bound);
          std::uniform_real_distribution<double> vertical_urd (vertical_lower_bound,
                                                               vertical_upper_bound);

          for (uint32_t j = 0; j < staNodeCount; j++)
            {
              // Randomize (x, y)
              random_x = horizontal_urd (dre);
              random_y = vertical_urd (dre);

              // Add the Station Nodes from the i-th AP node to the animation
              anim.SetConstantPosition (staNodes[i].Get (j), random_x, random_y);
            }

          // Update all the variables for the next group
          x = x + 30;
          y = x;

          horizontal_lower_bound = x - 5;
          horizontal_upper_bound = x + 5;
          vertical_lower_bound = y - 10;
          vertical_upper_bound = y + 10;
        }

      /// Set up the Flow Monitor and its helper
      Ptr<FlowMonitor> flowMonitor;
      FlowMonitorHelper flowHelper;
      flowMonitor = flowHelper.InstallAll ();

      /// Change the Simulator stop time to allow for some cleanup
      /// time because of the FlowMonitor
      /// PS.: the apps containers should remain at their normal stop time,
      ///      only the Simulator has to stop later
      Simulator::Stop (Seconds (globalStopTime + 3));
      Simulator::Run ();

      flowMonitor->SerializeToXmlFile ("flow_monitoring.xml", true, true);

      Simulator::Destroy ();
    }
  else
    {
      /// Run and then destroy the simulator
      Simulator::Run ();
      Simulator::Destroy ();
    }

  return 0;
}
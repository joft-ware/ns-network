#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"

#include <iostream>
using namespace ns3;
NS_LOG_COMPONENT_DEFINE("week4");

int 
main(int argc, char*argv[])
{
	LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
	LogComponentEnable("UdpEchoClientApplication", LOG_PREFIX_TIME);
	LogComponentEnable("UdpEchoClientApplication", LOG_PREFIX_FUNC);
	LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
	LogComponentEnable("UdpEchoServerApplication", LOG_PREFIX_TIME);
	LogComponentEnable("UdpEchoServerApplication", LOG_PREFIX_FUNC);
	
	NodeContainer nodes;
	nodes.Create(3);

	PointToPointHelper p2p1, p2p2;
	p2p1.SetDeviceAttribute("DataRate", StringValue("10Mbps"));
	p2p1.SetChannelAttribute("Delay", StringValue("5ms"));

	p2p2.SetDeviceAttribute("DataRate", StringValue("0.1Mbps"));
	p2p2.SetChannelAttribute("Delay", StringValue("10ms"));

	NetDeviceContainer devices1, devices2;
	devices1 = p2p1.Install(nodes.Get(0),nodes.Get(1));
	devices2 = p2p2.Install(nodes.Get(1),nodes.Get(2));

	InternetStackHelper stack;
	stack.Install(nodes);

	Ipv4AddressHelper addr1;
	addr1.SetBase("10.1.1.0", "255.255.255.0");

	Ipv4InterfaceContainer interfaces1 = addr1.Assign(devices1);
	
	UdpEchoClientHelper echoClient1(interfaces1.GetAddress(1), 9);
	echoClient1.SetAttribute("MaxPackets", UintegerValue(1500));
	echoClient1.SetAttribute("Interval", TimeValue(Seconds(0.001)));
	echoClient1.SetAttribute("PacketSize", UintegerValue(1050));

	ApplicationContainer clientApps1;
	clientApps1.Add(echoClient1.Install(nodes.Get(0)));
	clientApps1.Start(Seconds(2.0));
	clientApps1.Stop(Seconds(4.0));

	UdpEchoServerHelper echoServer1(9);
	ApplicationContainer serverApps1(echoServer1.Install(nodes.Get(1)));
	serverApps1.Start(Seconds(1.0));
	serverApps1.Stop(Seconds(5.0));

	Ipv4AddressHelper addr2;
	addr2.SetBase("10.1.2.0", "255.255.255.0");

	Ipv4InterfaceContainer interfaces2 = addr2.Assign(devices2);
	
	UdpEchoClientHelper echoClient2(interfaces2.GetAddress(1), 9);
	echoClient2.SetAttribute("MaxPackets", UintegerValue(1500));
	echoClient2.SetAttribute("Interval", TimeValue(Seconds(0.01)));
	echoClient2.SetAttribute("PacketSize", UintegerValue(1050));

	ApplicationContainer clientApps2;
	clientApps2.Add(echoClient2.Install(nodes.Get(1)));
	clientApps2.Start(Seconds(2.0));
	clientApps2.Stop(Seconds(4.0));

	UdpEchoServerHelper echoServer2(9);
	ApplicationContainer serverApps2(echoServer2.Install(nodes.Get(2)));
	serverApps2.Start(Seconds(1.0));
	serverApps2.Stop(Seconds(5.0));

	Simulator::Run();
	Simulator::Stop(Seconds(5.0));
	Simulator::Destroy();

	return 0;
}

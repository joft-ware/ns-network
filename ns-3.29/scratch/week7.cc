#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"

#include <iostream>
using namespace ns3;

int 
main(int argc, char*argv[])
{
	CommandLine cmd;
	std::string dr = "10Mbps", dl = "10us";

	cmd.AddValue("DataRate", "DataRate.", dr);
	cmd.AddValue("Delay", "Delay.", dl);
	cmd.Parse(argc,argv);
	
	NodeContainer nodes;
	nodes.Create(2);

	PointToPointHelper p2p;
	p2p.SetDeviceAttribute("DataRate", StringValue(dr));
	p2p.SetChannelAttribute("Delay", StringValue(dl));

	NetDeviceContainer devices1;
	devices1 = p2p.Install(nodes); 

	InternetStackHelper stack1;
	stack1.Install(nodes);

	Ipv4AddressHelper addr1;
	addr1.SetBase("10.1.1.0", "255.255.255.0");

	Ipv4InterfaceContainer interfaces1 = addr1.Assign(devices1);
	
	UdpClientHelper Client1(interfaces1.GetAddress(1), 9);
	Client1.SetAttribute("MaxPackets", UintegerValue(10000));
	Client1.SetAttribute("Interval", TimeValue(Seconds(0.001)));
	Client1.SetAttribute("PacketSize", UintegerValue(1024));

	ApplicationContainer clientApps1;
	clientApps1.Add(Client1.Install(nodes.Get(0)));
	clientApps1.Start(Seconds(1.0));
	clientApps1.Stop(Seconds(10.0));

	UdpServerHelper Server1(9);
	ApplicationContainer serverApps1(Server1.Install(nodes.Get(1)));
	serverApps1.Start(Seconds(0.5));
	serverApps1.Stop(Seconds(10.5));

	p2p.EnablePcapAll("2017311005");
	Simulator::Run();
	Simulator::Stop(Seconds(11.0));
	Simulator::Destroy();
	return 0;
}

---
layout: post
title:  "Networking - Part 3"
date:   2025-08-11 06:21:40 +0530
categories: Networking
tags: ["Computer Networking"]
---

Hey everyone! Welcome back to the third part of networking. In this blog, I will be covering the requirements (related to networking) that will be needed to get a clear understanding of the DevOps. So, let's start... 

#### Switching

It is how a `network switch` moves data between devices inside the same local network.

Let's understand switching in a simpler way:
- Each network device has a unique MAC address.
- When a switch gets a data, it looks for the MAC address of the destination.
- It checks its MAC address table (like a contacts list) to know which port that MAC address is connected to.
- It then sends the data to that device only, (not to every device)
- If it doesn't know the destination MAC, it broadcasts (temporarily) to everyone. 


##### The main difference between `switch` and `hub` is -
Hub broadcasts the data to all connected devices while switch boradcasts the data selectively through MAC-IP address table.



#### Some important commands

`ip link` : It shows information about your network interfaces at the  `data-link` layer (of the OSI model). It does not show any IP addresses (both IPv4 and IPv6), only MAC addresses details.

`ip addr` : It is similar to `ip link` but it also shows information about the **IPs assigned to each interface**.
It does not remove the old address, but will be reset after reboot if the changes are not made in network configuration files.

`ip addr add 192.168.8.10/24 dev eth0` : This command adds `192.168.8.10` with a subnet of 24 to the `eth0` interface.


#### Routing
Routing is the way of deciding the best path for the data packets to move between networks, based on the routing tables. In switching, the movement is within the same network. 

So, `switching` is like moving between rooms inside the same building while `routing` is like finding the best road to another building across town.

![Router](/assets/Networking/routing.png)

Let's understand this diagram. 

I want to send data from device B to device C. Just connecting the router between the network will make it work ?? 

The answer is No. Because router is just a device and there can be many other such devices, and in that case it will be more difficult for device B to  send data to device C. So, here comes the solution, `gateway`. We configure the systems with the gateway or a route. We'll come back to the solution once we understand gateway and routes clearly.

We can think like in this way, if a network is a room then gateway is like a door to the other networks.

#### Gateway
It is a specific node (IP address) that a device uses as the exit point from its own network to reach another network.

The gateway acts as a route to any network outside your own local network. 
To see the existing routing configuration on your system, use command `route` or `ip route`.

![Router](/assets/Networking/routing_2.png)

This is the routing information you can see in the above image. 
- `Destination` means the network for which this route is defined.
- `Genmask` means subnet here
- `Iface` means the network interface, (`wlp1s0` for wifi, `docker0` for docker-bridge)
- `Flags` shows about the status of gateway. `U` means the route is UP, `G` means route uses a gateway

##### What does `0.0.0.0` mean ?
It means there is no need of any gateway. Send the data directly to destination that is local and reachable.

So, now how can we send the data from device B to C? The solution is, we have to add a route to the IP routing table. 
`ip route add 192.168.2.0/24 via 192.168.1.1` this commands add a route with a destination network `192.168.2.0/24` and gateway `192.168.1.1` on dev ice B. Similarly, we need to add a route on device C too.
`ip route add 192.168.1.0/24 via 192.168.2.1`

Now, the connection is established, device B and C can communicate with each other.

Let's look at the another case.

![Router](/assets/Networking/routing_3.png)

I want to setup a connection between device A and C. Your answer will be adding the route on device A and device C, as we did in the previous case.
- `ip route add 192.168.2.0/24 via 192.168.1.6` on device A
- `ip route add 192.168.1.0/24 via 192.168.2.6` on device B

But, is it sufficient to establish the connection between them? The answer is No. In the figure, B is connected to both the devices A and C via `eth0` and `eth1` network interfaces. Due to security reasons, packets are not forwarded from one interface to another interface, and this setting is generally set as default in linux systems. 

`cat /proc/sys/net/ipv4/ip_forward`- with this command we can see if forwarding the packets is allowed if the value is set to 1, and if set to 0, means that forwarding the packets in not allowed.

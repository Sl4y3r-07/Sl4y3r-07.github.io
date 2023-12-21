---
layout: post
title:  "Networking - Part 2"
date:   2023-12-15 06:21:40 +0530
categories: Networking
tags: ["Computer Networking"]
---

Welcome to the other part of Computer Networking. In this part, we'll be exploring the protocols involved in networking.

### HTTP 

HTTP stands for ```Hypertext Tranfer Protocol```. It can be defined as the foundation of communication on the web. It is an application layer protocol, used to transfer information between devices. This protocol involves various request methods and corresponding responses to communicate between clients and servers.

**Request Methods**
 -   ```GET```: Requests data from a specified resource.
 -   ```POST```: Submits data to be processed to a specified resource.
 -   ```PUT```: Updates a resource or creates a new one if it doesn't exist.
 -   ```DELETE```: Deletes the specified resource.
 -   ```PATCH```: Partially updates a resource.
 -   ```HEAD```: Requests the headers of a resource, similar to GET but without the body.
 -   ```OPTIONS```: Queries the server for available methods on a resource.

**Responses**

The server responds to these request methods with status codes, indicating the success or failure of the request.

-    ```1xx```: Informational - Request received, continuing process.
 -   ```2xx```: Success - Request successfully received, understood, and accepted.
  -  ```3xx```: Redirection - Further action needed to complete the request.
   - ```4xx```: Client Error - Request contains bad syntax or cannot be fulfilled.
   - ```5xx```: Server Error - Server failed to fulfill a valid request.

Example: ```502```: Bad Gateway, ```500```: Internal Server Error, ```200```: Successful GET request, ```201```: Successful POST request, ```404```:URL can't be found, ```102```:Processing, ```301```:Moved Permanently, ```305```:Use Proxy

### SMTP

SMTP stands for ```Simple Mail Transport Protocol```. It is a standard protocol used for sending and receiving emails over the internet. SMTP works between mail servers to handle the sending of emails from the sender's email client to the recipient's email server. It operates on TCP ```port 25``` by default. To add more security, modern SMTP servers use ```SSL``` (Secure Shell) or ```TLS``` (Transport Layer Security) to encrypt the communication and to prevent unauthorized access or interception of emails..

**How does an SMTP server work?**
 - When we send an email from our email client (like Gmail, Outlook, Yahoo etc.), our email client communicates with our email provider's SMTP server.

 - Our email client formats the email according to SMTP standards (including the sender's address, recipient's address, subject, message body, and attachments).

 - The email client establishes a connection to the SMTP server using the SMTP protocol on port 25. It sends the formatted email to the SMTP server.

 - The SMTP server processes the email, performs checks (such as domain validation), and decides whether to relay the message to another server or deliver it locally (if the recipient is on the same server).

 - If the recipient is on a different domain, the SMTP server might forward the email to another SMTP server responsible for handling emails for that domain. This process continues until the email reaches the recipient's SMTP server.

 - The recipient's SMTP server receives the email, performs further checks, and places it in the recipient's mailbox or forwards it to the recipient's email client for retrieval.
 
 
### POP/IMAP 

These are two standard protocols used by email clients to retrieve emails from a mail server. ```POP``` stands for ```Post Office Protocol```. It is an older protocol that downloads the emails from the server and stores them locally on the device. ```IMAP``` stands for ```Internet Message Access Protocol``` that works in contrast to POP. It keeps emails stored on the server. IMAP is ideal for users who access their emails from multiple devices (such as phones, tablets, and computers) and want their emails to be synchronized across all devices. 

![smtp](/assets/Networking/smtp.jpg)

### DHCP

DHCP stands for ```Dynamic Host Configiration Protocol```. This protocol automates the assignment of IP addresses and other network configuration to devices in the network especially in the larger network where manually assigning addresses to each device would be impractical. Let's understand the how it works :-

When a device connects to a network, it sends out a broadcast message to a DHCP server on the network ```requesting an IP address```. It manages a pool of available IP addresses and configuration settings. The DHCP server selects an available IP address from its pool and assigns it to the requesting device. This assignment is temporary and is known as a lease. The lease includes a ```lease duration```, specifying how long the IP address is valid for that device. It may also provide additional network configuration information, such as the subnet mask, default gateway, DNS servers, and more. Once the IP address and configuration settings are assigned, DHCP server sends an acknowledgment to the requesting device, confirming the lease.

The device can also request a ```lease renewal``` from the DHCP server to continue using the same IP address. If the DHCP server approves the renewal, the lease duration is extended.

![dhcp](/assets/Networking/dhcp.jpg)

### FTP

FTP stands for ```File Transfer Protocol```, a standard network protocol for transferring files between server and client on a computer network. It works on a client-server model where client conencts to server to perform file transfers.

![ftp](/assets/Networking/ftp.png)

In the above image, I connected to a ```FTP server``` by username and password. By ```ls``` command, I listed all the files present on the server (There are some Organic Reactions files of my JEE time :p). Similarly, with ```send``` I can send files and with ```get```, I can download the files from the server. There are other commands also related to FTP server. 

Traditional FTP doesn’t encrypt data during transmission, which may cause security risks. Secure versions like FTPS (FTP Secure) and SFTP (SSH File Transfer Protocol) add encryption for secure data transfer.


### ARP 

ARP stands for ```Address Resolution Protocol```, that operates at 2nd layer (Data Link Layer). It is used for mapping an IP address to a physical machine address, such as a MAC address in a local network. Let's understand how ARP works :-

When a device wants to communicate with another device on the same local network and knows the IP address but not the MAC address of the destination, it uses ARP to find the MAC address associated with the IP address. The requesting device sends an ```ARP request``` packet containing the target IP address across the network. The device that has the corresponding IP address in its ARP cache responds with an ```ARP reply``` packet, providing its MAC address and after receiving the reply, the requesting device stores this mapping in its ```ARP cache``` or table. This cache helps speed up future communications by avoiding the need for ARP requests for frequently accessed IP addresses.However, ARP cache size is limited by design, so it is purged regularly to free up space.

![arp](/assets/Networking/arp.jpg)


### BGP/OSPF

BGP stands for ```Border Gateway Protocol```  and OSPF stands for ```Open Shortest Path First```. Both are routing protocols but they operate in different ways.
BGP is an ```exterior gateway protocol``` (EGP) primarily used to exchange routing information between different autonomous systems (AS) on the internet. 

What is **autonomous system**? It is a collection of IP networks and routers under a common administration sharing a single routing policy.

The purpose of BGP is to enable routers within different autonomous systems to share information about the best available paths for reaching specific destinations across the internet. It depends on mutliple factors such as network policies, path attributes, and the number of autonomous systems a route passes through. 


OSPF is an ```interior gateway protocol``` (IGP) designed for use within an autonomous system. Its purpose is to determine the best route path within an autonomous system. It is easily configured than BGP which is lot more complex. It also supports faster convergence which helps in effiient routing.

![bgp](/assets/Networking/bgp.png)


### TCP

TCP stands for ```Transmission Control Protocol```, a connection oriented protocol i.e it establishes a connection between sender and reciever before exchanging data. It ensures reliable, ordered, and error-checked delivery of data packets. TCP guarantees the delivery of data by using ```acknowledgments and retransmissions```. It also include flow control mechanisms to manage the rate of data transmission between devices. 

 ```Transmission```: It refers to the process of sending data from one device to another across a network. Data is broken down into smaller units called packets or frames
 
 ```Acknowledgments (ACK)```: After sending data, the sender expects confirmation from the receiver that the data arrived intact. This acknowledgment serves as confirmation that the data was successfully received. If the sender does not receive an acknowledgment within a specified time frame or detects packet loss, it retransmits the data to ensure reliable delivery.

There is also another flag called ```SYN``` which stands for ```Synchronize```. When a device wants to establish a TCP connection with another device, it sends a TCP packet with the SYN flag set in the header. This packet contains an initial sequence number, which is used to start the conversation and synchronize sequence numbers between the sender and receiver.

 ```Sequence numbers``` are unique identifiers assigned to TCP segments to maintain the order and integrity of data transmitted over a connection. They help in reassembling packets in the correct order at the receiver's end. Each segment of data sent via TCP contains a sequence number, allowing the receiver to arrange and reconstruct the data in the correct order.
 
 When a TCP connection is initiated, the SYN flag is set in the first packet sent by the initiating device. This packet includes an initial sequence number. The receiving device responds with its own SYN packet, acknowledging the connection request and also including its own initial sequence number. So the SYN flag is used to synchronize sequence numbers between devices initiating a TCP connection. It marks the beginning of a ```three-way handshake``` in TCP, allowing devices to exchange sequence numbers and establish a reliable connection before data transmission begins.
 
 The ```three-way handshake``` is a fundamental process in the TCP for establishing a reliable connection between two devices over a network. Data transmission can begin after the completion of the three-way handshake.


The three steps involved in the TCP three-way handshake are:

    SYN:
        The initiating device sends a TCP segment with the SYN flag set to the receiving device to initiate a connection request. This packet includes an initial sequence number chosen by the client to start the conversation.

    SYN-ACK:
        Upon receiving the SYN segment, if the server is available and willing to establish a connection, it responds with a TCP segment. This segment has both the SYN and ACK flags set, acknowledging the receipt of the client's SYN packet and also indicating its own readiness to establish the connection.
        The server also chooses its own initial sequence number for the conversation.

    ACK:
        Finally, the client responds to the server's SYN-ACK packet with an ACK packet. This packet acknowledges the receipt of the server's SYN-ACK segment. At this point, the connection is established and both devices have synchronized sequence numbers.
        

 

### UDP
It stands for ```User Datagram Protocol```, a connection-less protocol i.e. it does not establish a direct connection for data exchange. It sends data packets, called ```datagrams```, without establishing a connection or checking whether the receiver is ready or exists. Unlike TCP, it does not have an error checking mechanism which may result in corruption and loss of data packets. UDP has ```lower overhead``` compared to TCP because it lacks the extensive error-checking and flow control mechanisms. This makes it faster and more efficient for real-time applications. It is used where speed and efficiency is prioritized over reliability like video streaming, gaming, DNS lookups and cases where small amount of loss of data is acceptable. 









===================
= Names
===================

Nathaniel Glikman	nmg116
Stephen Scott		sms699

===================
= Implementation
===================

* Setting Up Interface

To set up the interfaces for each host and the routes they take, we just had to use the ```ip addr add``` command. When using that command, we must first specify the host we want to set up, so for instance
for h1 we want to start the command as ```h1 ip addr add```. Then, every host has a specific IP address, so we add the IP address after. We then need to specify the device its on, and each host has it's
own device, including the router host. We add ```dev h1-eth0``` for h1, and so on. For r1, we add ```dev r1-eth1``` and so on for each ethernet connection. Doing this sets up the IP addresses for each device
that we can then use to set up the routes.

* Setting Up Default Routes

To set up the default routes, we need to add 0.0.0.0 as the default gateway for each regular host. To do this, we use the command ```<host> route add default gw <device IP>```. We specify the host first,
and then add the default gateway, which is what ```deafult gw``` is. All that's left is to specify the device IP, which we know from the specifications and what we did before.

* Setting Up Per-Destination Routes

To set up the per=destination routes, we do it all from r1. We need to use ```ip route add``` and specify a connection between two different IPs. Each device on r1 has a different IP that connects to a
different host IP. Thus, our command needs to be ```r1 ip route add <host IP> via <r1-ethN IP>``` The via part makes the connection from r1 go through the specific host.

=====================
= Issues
=====================

There are no known issues with the commands. When using ping and traceroute, it works as intended.

=======================
= Problems
=======================

Problems that were had when doing this project was figuring out the correct commands to use. There was a lot of googling involved, as mininet and the commands were mostly unknown at the time. However,
with time, we understood how the commands work, and were able to set up the routes correctly.

===============
= Learned
===============

We learned how to effectively work in a Linux environment. Learning the ip suite of commands is useful for debugging connections, and we can use this new found information in the future.

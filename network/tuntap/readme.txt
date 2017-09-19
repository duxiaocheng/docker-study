###############
For tun demo
###############

Step 1:
------
# ip tuntap add tun0 mode tun
# ifconfig tun0 0.0.0.0 up
# ip -d link show tun0
22: tun0: <NO-CARRIER,POINTOPOINT,MULTICAST,NOARP,UP> mtu 1500 qdisc pfifo_fast state DOWN mode DEFAULT qlen 500
    link/none  promiscuity 0 
    tun addrgenmode eui64

Step 2:
------
# route add 10.10.10.1 dev tun0
# route -n
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
0.0.0.0         10.9.245.222    0.0.0.0         UG    100    0        0 eth0
10.9.245.192    0.0.0.0         255.255.255.224 U     100    0        0 eth0
10.10.10.1      0.0.0.0         255.255.255.255 UH    0      0        0 tun0

Step 3:
------
# ./tun-demo tun0
Open tun/tap device: tun0 success...
TUN name is tun0
Read 84 bytes from tun/tap device
0x458f39f0: 45 00 00 54 68 3c 40 00 40 01 be 92 0a 09 f5 c6  E..Th<@.@.......
0x458f3a00: 0a 0a 0a 01 08 00 73 8d 23 8f 00 01 00 ce c0 59  ......s.#......Y
0x458f3a10: 00 00 00 00 e0 e7 00 00 00 00 00 00 10 11 12 13  ................
0x458f3a20: 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f 20 21 22 23  ............ !"#
0x458f3a30: 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f 30 31 32 33  $%&'()*+,-./0123
0x458f3a40: 34 35 36 37 00 00 00 00 00 00 00 00 00 00 00 00  4567............
Write 84 bytes to tun/tap device

# ping 10.10.10.1 
PING 10.10.10.1 (10.10.10.1) 56(84) bytes of data.
64 bytes from 10.10.10.1: icmp_seq=1 ttl=64 time=0.133 ms
64 bytes from 10.10.10.1: icmp_seq=2 ttl=64 time=0.155 ms
64 bytes from 10.10.10.1: icmp_seq=3 ttl=64 time=0.158 ms

# tcpdump -i tun0 -n
tcpdump: WARNING: tun0: no IPv4 address assigned
tcpdump: verbose output suppressed, use -v or -vv for full protocol decode
listening on tun0, link-type RAW (Raw IP), capture size 65535 bytes
07:58:21.064288 IP 10.9.245.198 > 10.10.10.1: ICMP echo request, id 9103, seq 30, length 64
07:58:21.064419 IP 10.10.10.1 > 10.9.245.198: ICMP echo reply, id 9103, seq 30, length 64

-------------------
When we ping the IP address 10.10.10.1, the packet will be sent to tun0 interface.
Meanwhile, user-space program tun-demo has attached itself to tun0.
So, it can receive this packet. And it makes a new ICMP reply pakcet,
and then it writes this new packet to tun0 char dev.

###############
For tap demo
###############

Step 1:
------
# ip tuntap add tap0 mode tap
# ifconfig tap0 0.0.0.0 up
# ip -d link show tap0
28: tap0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc pfifo_fast state DOWN mode DEFAULT qlen 1000
    link/ether 22:91:6b:a9:6a:dd brd ff:ff:ff:ff:ff:ff promiscuity 0 
    tun addrgenmode eui64

Step 2:
------
# route add 10.10.10.1 dev tap0
# arp -s 10.10.10.1 22:91:6b:a9:6a:dd
# arp
Address                  HWtype  HWaddress           Flags Mask            Iface
10.10.10.1               ether   22:91:6b:a9:6a:dd   CM                    tap0

Set the static arp, otherwise the arp message will be sent out firstly. 

Step 3:
------
# ./tun-demo tap0
Open tun/tap device: tap0 success...
TUN name is tap0
Read 98 bytes from tun/tap device
0xfb36eef0: 22 91 6b a9 6a dd 22 91 6b a9 6a dd 08 00 45 00  ".k.j.".k.j...E.
0xfb36ef00: 00 54 9d 6f 40 00 40 01 89 5f 0a 09 f5 c6 0a 0a  .T.o@.@.._......
0xfb36ef10: 0a 01 08 00 32 71 42 83 00 01 61 d0 c0 59 00 00  ....2qB...a..Y..
0xfb36ef20: 00 00 a2 0d 00 00 00 00 00 00 10 11 12 13 14 15  ................
0xfb36ef30: 16 17 18 19 1a 1b 1c 1d 1e 1f 20 21 22 23 24 25  .......... !"#$%
0xfb36ef40: 26 27 28 29 2a 2b 2c 2d 2e 2f 30 31 32 33 34 35  &'()*+,-./012345
0xfb36ef50: 36 37 00 00 00 00 00 00 00 00 00 00 00 00 00 00  67..............
Ether type: 0x0800
Write 98 bytes to tun/tap device

# ping 10.10.10.1




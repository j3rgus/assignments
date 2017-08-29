#!/bin/bash

#Rezervovane adresy + lokalne adresy. Dal som sem len par pre priklad, snad to nevadi.
RES_ADDRS="0.0.0.0/8 10.0.0.0/8 127.0.0.0/8 172.16.0.0/12 192.0.0.0/29 224.0.0.0/4"

#Vymazem aktualnu tabulku (default tab filter)
iptables -F
iptables -X

#Nastavim default policy na DROP
iptables -P INPUT DROP
iptables -P OUTPUT DROP
iptables -P FORWARD DROP

#Dovolim pristup na lo
iptables -A INPUT -i lo -j ACCEPT
iptables -A OUTPUT -o lo -j ACCEPT

#Dovolim ping zvnutra von
iptables -A OUTPUT -p icmp --icmp-type 8 -j ACCEPT
iptables -A INPUT -p icmp --icmp-type 0 -j ACCEPT

#Dovolim ping zvonku dnu
iptables -A INPUT -p icmp --icmp-type 8 -j ACCEPT
iptables -A OUTPUT -p icmp --icmp-type 0 -j ACCEPT

#Dovolim traceroute (predpokladam, ze v tomto pripade funguje tak, ze sa neustale zvysuje port)
iptables -A OUTPUT -o eth0 -p udp --sport 32769:65535 --dport 33434:33523 -m state --state NEW -j ACCEPT
iptables -A INPUT -i eth0 -p udp --sport 33434:33523 --dport 32769:65535 -m state --state NEW -j ACCEPT

#Dovolim webovu prevadzku (HTTP)
iptables -A INPUT -p tcp -m tcp --sport 80 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 80 -j ACCEPT

#Dovolim webovu prevadzku (HTTPS)
iptables -A INPUT -p tcp -m tcp --sport 443 -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 443 -j ACCEPT

#Dovolim DNS requests
iptables -A OUTPUT -p udp -o eth0 --dport 53 -j ACCEPT
iptables -A INPUT -p udp -i eth0 --sport 53 -j ACCEPT

#Dovolim FTP spojenia na port 21, prichadzajuce aj odchadzajuce
iptables -A INPUT  -p tcp -m tcp --dport 21 -m conntrack --ctstate ESTABLISHED -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 21 -m conntrack --ctstate NEW,ESTABLISHED -j ACCEPT

#Dovolim FTP port 20 pre aktivne spojenie
iptables -A INPUT -p tcp -m tcp --dport 20 -m conntrack --ctstate ESTABLISHED,RELATED -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --dport 20 -m conntrack --ctstate ESTABLISHED -j ACCEPT

#Dovolim pasivne spojenie
iptables -A INPUT -p tcp -m tcp --sport 1024: --dport 1024:  -m conntrack --ctstate ESTABLISHED -j ACCEPT
iptables -A OUTPUT -p tcp -m tcp --sport 1024: --dport 1024:  -m conntrack --ctstate ESTABLISHED,RELATED -j ACCEPT

#Otvorim porty pre Sambu
iptables -A INPUT -i eth0 -p udp --dport 137:138 -j ACCEPT
iptables -A OUTPUT -o eth0 -p udp --sport 137:138 -j ACCEPT
iptables -A INPUT -i eth0 -p tcp --dport 139 -j ACCEPT
iptables -A OUTPUT -o eth0 -p tcp --sport 139 -j ACCEPT
iptables -A INPUT -i eth0 -p tcp --dport 445 -j ACCEPT
iptables -A OUTPUT -o eth0 -p tcp --sport 445 -j ACCEPT

#Dovolim ssh len z 147.251.48.155
iptables -A INPUT -i eth0 -p tcp -s 147.251.48.155 --dport 22 -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A OUTPUT -o eth0 -p tcp --sport 22 -m state --state ESTABLISHED -j ACCEPT

#Logujem IP spoofing
for ip in $RES_ADDRS
do
  iptables -A INPUT -i eth0 -s $ip -j LOG --log-prefix "SPOOF Adress: "
done


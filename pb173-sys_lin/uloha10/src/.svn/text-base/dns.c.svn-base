#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <pcap.h>
#include <time.h>
#include <errno.h>
#include <resolv.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/udp.h>

#define FILTER_STRING	"udp and port 53"
#define BUF_LEN			256

char errbuf[PCAP_ERRBUF_SIZE];

void print_rr_sec(ns_msg *nmsg, ns_sect sect)
{
	char buf[BUF_LEN];
	ns_rr rr;
	int i, ret, count;

	errno = 0;
	count = ns_msg_count(*nmsg, sect);
	for (i = 0; i < count; i++) {
		ret = ns_parserr(nmsg, sect, i, &rr);
		if (ret == -1) {
			fprintf(stderr, "ns_parserr: %s", strerror(errno));
			continue;
		}

		printf("%s, type %s, class %s", ns_rr_name(rr),
					p_type(ns_rr_type(rr)), p_class(ns_rr_class(rr)));

		if (sect == ns_s_an) {
			printf(", ttl %s, address %s", p_time(ns_rr_ttl(rr)),
						inet_ntop(AF_INET, ns_rr_rdata(rr), buf, BUF_LEN - 1));
		}

		printf("\n");
	}
}

int main(int argc, char *argv[])
{
	const u_char *packet;
	char buf[BUF_LEN];
	pcap_t *pcap;
	struct bpf_program fp;
	struct pcap_pkthdr header;
	struct ether_header *ehdr;
	struct ip *iphdr;
	struct ip6_hdr *ip6hdr;
	struct udphdr *uhdr;
	ns_msg nmsg;
	bpf_u_int32 netp;
	bpf_u_int32 maskp;
	int ret;

	if (argc < 2) {
		fprintf(stderr, "Need interface as a parameter!\n");
		exit(1);
	}

	ret = pcap_lookupnet(argv[1], &netp, &maskp, errbuf);
	if (ret == -1) {
		fprintf(stderr, "%s\n", errbuf);
		exit(1);
	}

	pcap = pcap_open_live(argv[1], 2000, 0, 0, errbuf);
	if (pcap == NULL) {
		fprintf(stderr, "%s\n", errbuf);
		exit(1);
	}

	ret = pcap_datalink(pcap);
	if (ret != DLT_EN10MB) {
		fprintf(stderr, "Interface %s doesn't provide Ethernet headers!\n", argv[1]);
		exit(1);
	}

	ret = pcap_compile(pcap, &fp, FILTER_STRING, 0, netp);
	if (ret == -1) {
		pcap_perror(pcap, "pcap_compile");
		exit(1);
	}

	ret = pcap_setfilter(pcap, &fp);
	if (ret == -1) {
		pcap_perror(pcap, "pcap_setfilter");
		exit(1);
	}

	while (1) {
		packet = pcap_next(pcap, &header);
		printf("Packet captured at %s", ctime((time_t *) &header.ts.tv_sec));

		printf("L2:\n");
		ehdr = (struct ether_header *) packet;
		printf("\tSRC MAC: %s\n", ether_ntoa((struct ether_addr *) ehdr->ether_shost));
		printf("\tDST MAC: %s\n", ether_ntoa((struct ether_addr *) ehdr->ether_dhost));
		packet += ETHER_HDR_LEN;

		printf("L3:\n");
		if (ehdr->ether_type == ETHERTYPE_IPV6) {
			ip6hdr = (struct ip6_hdr *) packet;
			printf("\tSRC IP: %s\n", inet_ntop(AF_INET6, (struct in6_addr *) &ip6hdr->ip6_src, buf, BUF_LEN-1));
			printf("\tSRC IP: %s\n", inet_ntop(AF_INET6, (struct in6_addr *) &ip6hdr->ip6_dst, buf, BUF_LEN-1));
			printf("\tHop Limit: %u\n", ip6hdr->ip6_ctlun.ip6_un1.ip6_un1_hlim);
			packet += sizeof(struct ip6_hdr);
		} else {
			iphdr = (struct ip *) packet;
			printf("\tSRC IP: %s\n", inet_ntop(AF_INET, (struct in_addr *) &iphdr->ip_src, buf, BUF_LEN-1));
			printf("\tDST IP: %s\n", inet_ntop(AF_INET, (struct in_addr *) &iphdr->ip_dst, buf, BUF_LEN-1));
			printf("\tTTL: %u\n", iphdr->ip_ttl);
			packet += sizeof(struct ip);
		}

		printf("L4:\n");
		uhdr = (struct udphdr *) packet;
		printf("\tSRC PORT: %u\n", ntohs(uhdr->source));
		printf("\tDST PORT: %u\n", ntohs(uhdr->dest));
		packet += sizeof(struct udphdr);

		errno = 0;
		printf("DNS:\n");
		ret = ns_initparse(packet, header.caplen - (packet - (u_char *) ehdr) , &nmsg);
		if (ret == -1) {
			fprintf(stderr, "ns_initparse: %s\n", strerror(errno));
			continue;
		}

		printf("\tTransaction ID: 0x%04x\n", ns_msg_id(nmsg));
		printf("\tMessage is a %s\n", (ns_msg_getflag(nmsg, ns_f_qr)) ? "response" : "query");
		printf("\tQuestions: %u\n", ns_msg_count(nmsg, ns_s_qd));
		printf("\tAnswer RRs: %u\n", ns_msg_count(nmsg, ns_s_an));

		printf("\tQueries:\n");
		print_rr_sec(&nmsg, ns_s_qd);
		printf("\tAnswers:\n");
		print_rr_sec(&nmsg, ns_s_an);
		printf("\n");
	}

	pcap_close(pcap);
	return 0;
}

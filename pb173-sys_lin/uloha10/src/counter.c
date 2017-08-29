#include <stdio.h>
#include <signal.h>
#include <pcap.h>

int end;

void sig_handle(int sig)
{
	if (sig == SIGINT)
		end = 1;
}

int main(int argc, char *argv[])
{
	int count = 0;
	char errbuf[PCAP_ERRBUF_SIZE];
	struct pcap_pkthdr h;
	struct sigaction sa;
	pcap_t *pcap;

	if (argc < 2) {
		fprintf(stderr, "Usage: ./counter <if>\n");
		return 1;
	}

	sa.sa_handler = sig_handle;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	if (sigaction(SIGINT, &sa, NULL) == -1)
		perror("Error occured");

	pcap = pcap_open_live(argv[1], 2000, 0, 0, errbuf);
	if (pcap == NULL)
		fprintf(stderr, "%s\n", errbuf);

	while (1) {
		pcap_next(pcap, &h);
		count++;
		if (end == 1) {
			printf("%d\n", count);
			break;
		}
	}

	pcap_close(pcap);
	return 0;
}

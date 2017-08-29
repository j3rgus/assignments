#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* define what parameters are acceptable */
#define ARGUMENTS "hV"

/* define program version */
#define VERSION "Using getopt example v0.1"

/* Print how to use this program */
void usage(char* progname)
{
	printf("Usage: %s [-hV]\n\n", progname);
	printf(" -h     Show this help\n");
	printf(" -V     Show program version\n");
	printf("\nReport %s bugs to mymail@mydomain.org\n", progname);
}

/* main work */
int main(int argc, char* argv[])
{
	int c;
	char buffer[10];

	/* parse given parameters */
	while ((c = getopt(argc, argv, ARGUMENTS)) != -1) {
		switch (c) {
		case 'h': /* Show help */
			usage(argv[0]);
			return 0;

		case 'V': /* Show version */
			printf("%s\n", VERSION);
			return EXIT_SUCCESS;

		default: /* Something strange - show user what to do and exit */
			usage(argv[0]);
			return EXIT_FAILURE;
		}
	}

	/* do some nice (or faulty?) work */
	printf("Say something: ");
	scanf("%s", buffer);
	printf("You said \"%s\"\n", buffer);

	/* say Good Bye */
	return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

extern char **environ;

/*
 *  Vypis Usage a skonci
 */
void usage(void)
{
	printf("Usage: ./printenv [OPTION]... [VARIABLE]...\n");
	printf("Print the values of the specified environment VARIABLE(s).\n");
	printf("If no VARIABLE is specified, print name and value pairs for them all.\n\n");
	printf("  -0, --null     end each output line with 0 byte rather than newline\n");
	printf("      --help     show this message \n");
	printf("      --version  print information about version\n\n");
	exit(0);
}


/*
 *  Vypis verziu a skonci.
 */
void version(void)
{
	printf("\nPrintenv v1.0\n");
	printf("Written by J. Lysy\n\n");
	exit(0);
}

/*
 *  Vypis vsetky premenne prostredia
 *	null_end - 0-nemen vystup, 1-vypis \n na koniec premennej
 */
void print_env(int null_end)
{
	char **vars = environ;

	while (*vars != 0) {
		printf("%s", *vars);
		if (!null_end)
			putchar('\n');
		vars++;
	}
}

int main(int argc, char **argv)
{
	int c;
	int option_index;
	int null_flag = 0, help_flag = 0, ver_flag = 0;
	struct option options[] = {
		{"null", no_argument, 0, '0'},
		{"help", no_argument, &help_flag, 1},
		{"version", no_argument, &ver_flag, 1},
		{0, 0, 0, 0}
	};

	while (1) {
		c = getopt_long(argc, argv, "0", options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 0:
			break;
		case '0':
			null_flag = 1;
			break;
		default:
			usage();
		}
	}

	if (help_flag)
		usage();

	if (ver_flag)
		version();

	if (optind >= argc)
		print_env(null_flag);
	else {
		while (optind < argc) {
			printf("%s", getenv(argv[optind]));
			if (!null_flag)
				putchar('\n');
			optind++;
		}
	}

	return 0;
}

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>

int help;
int ver;

struct option options[] = {
	{"group", no_argument, 0, 'g'},
	{"groups", no_argument, 0, 'G'},
	{"name", no_argument, 0, 'r'},
	{"user", no_argument, 0, 'u'},
	{"help", no_argument, &help, 1},
	{"version", no_argument, &ver, 1},
	{0, 0, 0, 0}
};

void usage(void)
{
	printf("Pouzitie ./id [PREPINAC] [MENO UZIATELA]\n\n"
			"-g, --group     print only the effective group ID\n"
			"-G, --groups    print all group IDs\n"
			"-n, --name      print a name instead of a number, for -ugG\n"
			"-u, --user      print only the effective user ID\n"
			"      --help     zobrazí tohto pomocníka a skončí\n"
			"      --version  vypíše informácie o verzii a skončí\n\n");
	exit(0);
}

void version(void)
{
	printf("./id version 1.00\nBy J. Lysy\n\n");
	exit(0);
}

/*
 *	Function printgr prints all groups
 *	for given user. Set nam to 1 to print
 *	names as well.
 */

void printgr(struct passwd *usr, int nam)
{
	struct group *group;
	gid_t *groups;
	int ngrps, ret;
	int i;

	ngrps = sysconf(_SC_NGROUPS_MAX);
	groups = malloc(ngrps * sizeof(groups));
	if (groups == NULL) {
		perror("malloc");
		exit(0);
	}

	ret = getgrouplist(usr->pw_name, usr->pw_gid, groups, &ngrps);
	if (ret == -1) {
		printf("getgrouplist returned -1\n");
		exit(1);
	}

	for (i = 0; i < ngrps; i++) {
		group = getgrgid(groups[i]);
		printf("%d", group->gr_gid);
		if (nam)
			printf("(%s)", group->gr_name);
		putchar(' ');
	}
	putchar('\n');
	free(groups);
}

int main(int argc, char *argv[])
{
	struct passwd *user_info;
	struct group *group_info;
	int c, opt_index;
	int group = 0, groups = 0, name = 0, user = 0;

	while (1) {
		c = getopt_long(argc, argv, "gGnu", options, &opt_index);
		if (c == -1)
			break;

		switch (c) {
		case 0:
			break;
		case 'g':
			group = 1;
			break;
		case 'G':
			groups = 1;
			break;
		case 'n':
			name = 1;
			break;
		case 'u':
			user = 1;
			break;
		default:
			usage();
		}
	}

	if ((group && groups) || (group && user) || (groups && user)) {
		printf("Use one of the ugG switch!\n");
		return 1;
	}

	if (optind < argc) {
		user_info = getpwnam(argv[optind]);
		group_info = getgrnam(argv[optind]);

	} else {
		user_info = getpwuid(getuid());
		group_info = getgrgid(getuid());
	}

	if (!user_info || !group_info) {
		printf("User not found!\n");
		return 1;
	}

	if (group) {
		if (name)
			printf("%s\n", group_info->gr_name);
		else
			printf("%u\n", group_info->gr_gid);
		return 0;
	}

	if (user) {
		if (name)
			printf("%s\n", user_info->pw_name);
		else
			printf("%u\n", user_info->pw_uid);
		return 0;
	}

	if (groups) {
		printgr(user_info, name);
		return 0;
	}

	printf("uid=%u(%s) gid=%u(%s) skupiny=", user_info->pw_uid, user_info->pw_name,
			group_info->gr_gid, group_info->gr_name);
	printgr(user_info, 1);

	return 0;
}

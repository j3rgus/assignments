#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <string.h>

#include "debug.h"

/* Shared for all treads as read-only */
const char *pattern;
int level;

void usage(void)
{
	printf("Usage: ./psearch -p <patern> -d <dir> [-l <1-3>]\n");
	printf("Use level of output (1 for default):\n");
	printf("    1 for printing number of all matches only\n");
	printf("    2 includes number of matches on each line\n");
	printf("    3 includes printing of all locations of matches\n");
	exit(1);
}

void *read_file(void *filename)
{
	char *fname = (char *) filename;
	char *mem;
	char *line;
	char *saveptr;
	char *ptr;
	struct stat st;
	int fd;
	int ret, iline;
	int matches = 0, lmatches = 0;
	int *th_ret;

	fd = open(fname, O_RDONLY);
	if (fd == -1) {
		perror(fname);
		pthread_exit(NULL);
	}

	ret = fstat(fd, &st);
	if (ret == -1) {
		perror(fname);
		pthread_exit(NULL);
	}

	mem = malloc(st.st_size + 1);
	if (!mem)
		pthread_exit(NULL);

	ret = read(fd, mem, st.st_size);
	close(fd);

	mem[st.st_size] = 0;

	iline = 1;
	line = strtok_r(mem, "\n", &saveptr);
	ptr = line;

	while (line != NULL) {
		ptr = strcasestr(ptr, pattern);
		if (ptr) {
			if (level >= 3)
				printf("%s:%d:%d Found match\n", fname, iline, ptr - line + 1);
			matches++;
			lmatches++;
			ptr++;
			continue;
		}

		if (level >= 2)
			printf("-> %s:%d Found %d matches\n", fname, iline, lmatches);

		line = strtok_r(NULL, "\n", &saveptr);
		ptr = line;
		lmatches = 0;
		iline++;
	}

	free(mem);

	th_ret = malloc(sizeof(int));
	if (!th_ret)
		pthread_exit(NULL);

	*th_ret = matches;
	pthread_exit((void *) th_ret);
}

int main(int argc, char *argv[])
{
	char **filenames;
	char **temp;
	char *string = NULL;
	char *dirname = NULL;
	struct stat st;
	struct dirent *entry;
	DIR *dir;
	pthread_t *threads;
	int len, i;
	int opt, ret;
	int numfiles = 0;
	int step = 64;
	int *matches;

	while (1) {
		opt = getopt(argc, argv, "l:p:d:");
		if (opt == -1)
			break;

		switch (opt) {
		case 'p':
			string = optarg;
			break;
		case 'd':
			dirname = optarg;
			break;
		case 'l':
			level = atoi(optarg);
			break;
		default:
			usage();
		}
	}

	if (!string || !dirname)
		usage();

	/* Set searching pattern into global variable */
	pattern = string;

	/* Open directory and read all the files */
	dir = opendir(dirname);
	printerr_ex((dir == NULL), "opendir", 1);

	filenames = malloc(sizeof(char *) + step);
	if (!filenames)
		exit(2);

	while ((entry = readdir(dir)) != NULL) {
		if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
			continue;

		/* Reallocate and do all stuff around */
		if (numfiles >= step) {
			step <<= 1;
			temp = realloc(filenames, sizeof(char *) * step);
			if (!temp)
				exit(2);
			else
				filenames = temp;
		}

		/* Add name to target directory's path */
		len = strlen(dirname) + strlen(entry->d_name) + 2;

		filenames[numfiles] = malloc(len);
		if (!filenames[numfiles])
			exit(2);

		sprintf(filenames[numfiles], "%s/%s", dirname, entry->d_name);

		/* Omit folders and potential error */
		ret = stat(filenames[numfiles], &st);
		if ((ret == -1) || (S_ISDIR(st.st_mode))) {
			free(filenames[numfiles]);
			continue;
		}

		numfiles++;
	}

	ret = closedir(dir);
	printerr_ex((ret == -1), "closedir", 1);

	/* Create exactly one thread for one file(name) */
	threads = malloc(sizeof(pthread_t) * numfiles);
	if (!threads)
		exit(2);

	/* Run all threads */
	for (i = 0; i < numfiles; i++)
		pthread_create(&threads[i], NULL, read_file, filenames[i]);

	/* Wait till all threads exit and free all memory */
	for (i = 0; i < numfiles; i++) {
		pthread_join(threads[i], (void **) &matches);
		if (matches)
			printf("--> %s Found %d matches\n", filenames[i], *matches);

		free(filenames[i]);
		free(matches);
	}

	free(filenames);
	free(threads);

	return 0;
}

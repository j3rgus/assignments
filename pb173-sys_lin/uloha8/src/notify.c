#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <dirent.h>
#include <sys/inotify.h>
#include <sys/stat.h>

#include "debug.h"

#define CMD_MAX_LEN		512
#define PATH_LEN		256
#define FILENAME_LEN	128
#define FULLPATH_LEN	(PATH_LEN + FILENAME_LEN)
#define NWATCHES		100
#define WBUF_SIZE		(NWATCHES * sizeof(struct inotify_event))

struct pair {
	int wd;
	char *path;
};

volatile int end;

/* Global variables */
struct pair watch_pair[NWATCHES];
char src_prefix[PATH_LEN];
char dst_prefix[PATH_LEN];

/* Function prototypes */
void usage(void);
void watch_dirs(int ifd, char *path);
void handle_event(int ifd, struct inotify_event *e);
void syncdir(char *src, char *dst);
int find_pair(int wd);
int save_pair(int wd, char *path);
int delete_pair(int wd);

/* Signal handler */
void sig_handle(int sig)
{
	if (sig == SIGTERM)
		end = 1;
}

/*---------------- MAIN ----------------*/
int main(int argc, char *argv[])
{
	unsigned char buf[WBUF_SIZE];
	unsigned char *p;
	char *pret;
	struct inotify_event *event;
	int ifd;
	int nread, ret;
	struct sigaction sa;

	if (argc < 3)
		usage();

	/* Simply copy directory */
	syncdir(argv[1], argv[2]);

	/* Make sure that we use absolute paths */
	pret = realpath(argv[1], src_prefix);
	printerr_ex((pret == NULL), "realpath (src)", 1);

	pret = realpath(argv[2], dst_prefix);
	printerr_ex((pret == NULL), "realpath (dst)", 1);

	sa.sa_handler = sig_handle;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	if (sigaction(SIGTERM, &sa, NULL) == -1)
		perror("sigaction");

	/* Run daemon and inotify */
	ret = daemon(0, 0);
	printerr_ex((ret == -1), "daemon", 1);

	ifd = inotify_init();
	printerr_ex((ifd == -1), "inotofy_init", 1);

	watch_dirs(ifd, src_prefix);

	while (1) {
		nread = read(ifd, buf, WBUF_SIZE);
		if ((nread < 0) && (errno & EINTR))
			if (end == 1)
				break;

		for (p = buf; p < buf + nread; ) {
			event = (struct inotify_event *) p;
			handle_event(ifd, event);
			p += sizeof(struct inotify_event) + event->len;
		}

		syncdir(src_prefix, dst_prefix);
	}

	close(ifd);

	return 0;
}

/*---------------- Function definitions ----------------*/
void usage(void)
{
	printf("Usage: ./notify <SRC_DIR> <DST_DIR>\n");
	exit(1);
}

void watch_dirs(int ifd, char *path)
{
	DIR *p_dir;
	struct dirent *p_dirent;
	struct stat st;
	char full_path[PATH_LEN];
	int wd;

	p_dir = opendir(path);
	if (p_dir == NULL)
		return;

	wd = inotify_add_watch(ifd, path, IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO | IN_ISDIR | IN_DELETE_SELF);
	printerr_ex((wd == -1), "inotify_add_watch", 1);

	save_pair(wd, path);

	while ((p_dirent = readdir(p_dir)) != NULL) {
		snprintf(full_path, PATH_LEN-1, "%s/%s", path, p_dirent->d_name);
		stat(full_path, &st);
		if (S_ISDIR(st.st_mode)) {
			if (strcmp(p_dirent->d_name, ".") && strcmp(p_dirent->d_name, ".."))
				watch_dirs(ifd, full_path);
		}
	}

	closedir(p_dir);
}

void handle_event(int ifd, struct inotify_event *e)
{
	char *src;

	src = watch_pair[find_pair(e->wd)].path;

	if (e->mask & IN_ISDIR) {
		if (e->mask & IN_CREATE)
			watch_dirs(ifd, src);
	}

	if (e->mask & IN_ISDIR) {
		if (e->mask & IN_DELETE_SELF) {
			delete_pair(e->wd);
			inotify_rm_watch(ifd, e->wd);
		}
	}
}

void syncdir(char *src, char *dst)
{
	char scmd[CMD_MAX_LEN];
	int ret;

	snprintf(scmd, CMD_MAX_LEN - 1, "rsync \"%s/\" --delete -aq \"%s\"", src, dst);

	ret = system(scmd);
	printerr_ex((ret < 0), scmd, 1);
}

int find_pair(int wd)
{
	int i;
	int ret = -1;

	for (i = 0; i < NWATCHES; i++)
		if (watch_pair[i].wd == wd) {
			ret = i;
			break;
		}

	return ret;
}

int save_pair(int wd, char *path)
{
	int idx;

	idx = find_pair(0);
	if (idx < 0)
		return idx;

	watch_pair[idx].wd = wd;
	watch_pair[idx].path = strdup(path);

	return 0;
}

int delete_pair(int wd)
{
	int idx;

	idx = find_pair(wd);
	if (idx < 0)
		return idx;

	watch_pair[idx].wd = 0;
	free(watch_pair[idx].path);
	watch_pair[idx].path = NULL;

	return 0;
}


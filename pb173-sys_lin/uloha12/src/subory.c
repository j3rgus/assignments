#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <libintl.h>

#define _(STRING) gettext(STRING)

int main(int argc, char *argv[])
{
	char *ptr;
	int num;

	ptr = setlocale(LC_ALL, "");
	if (ptr == NULL) {
		printf(_("Can't set locale!\n"));
		return 1;
	}

	bindtextdomain("file", "locale");
	textdomain("file");

	if (argc < 2) {
		printf(_("You need to specify number of files!\n"));
		return 1;
	}

	num = atoi(argv[1]);
	if (num <= 0)
		printf(_("No change!\n"));
	else
		printf(ngettext("%d file deleted!\n", "%d files deleted!\n", num), num);

	return 0;
}

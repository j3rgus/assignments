#include <stdio.h>
#include <locale.h>
#include <libintl.h>

#define _(STRING) gettext(STRING)

int main(void)
{
	char *ptr;

	if ((ptr = setlocale(LC_ALL, "")) == NULL) {
		printf(_("Can't set locale!\n"));
		return 1;
	}

	bindtextdomain("hello", "locale");
	textdomain("hello");

	printf(_("Hello, World!\n"));
	return 0;
}

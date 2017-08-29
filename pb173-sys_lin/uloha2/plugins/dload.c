#include <stdio.h>
#include <dlfcn.h>

int main(void)
{
	void *h_handle;
	void *b_handle;
	void (*hello)(void);
	void (*bye)(void);
	char *error;
	int c;

	h_handle = dlopen("./hello.so.1.0.0", RTLD_LAZY);
	if (!h_handle) {
		fputs(dlerror(), stderr);
		return 1;
	}

	b_handle = dlopen("./bye.so.1.0.0", RTLD_LAZY);
	if (!b_handle) {
		fputs(dlerror(), stderr);
		return 1;
	}

	hello = dlsym(h_handle, "hello");
	error = dlerror();
	if (error != NULL)  {
		fputs(error, stderr);
		return 1;
	}

	bye = dlsym(b_handle, "hello");
	error = dlerror();
	if (error != NULL)  {
		fputs(error, stderr);
		return 1;
	}

	printf("Type '0' to call hello from hello.\n");
	printf("Type '1' to call hello from bye.\n");
	printf("Type 'q' to quit.\n");

	while ((c = getchar()) != 'q') {
		if (c == '0')
			hello();

		if (c == '1')
			bye();
	}

	dlclose(h_handle);
	dlclose(b_handle);
	return 0;
}

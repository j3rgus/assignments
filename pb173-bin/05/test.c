#include <stdio.h>

typedef int		my_intarr[16];
typedef char	*my_pchar;
typedef struct my_string {
	char *buf;
	int len;
} my_string_t;

my_pchar p_c;
my_intarr arr_i;
my_string_t s_str;
int i;
long li;
char buf[10];
int *p_int;

char fun1(void)
{
	return 0;
}

int fun2(int a, int b)
{
	return a + b;
}

void fun3(void)
{
	printf("Hello");
	void fun4(void)
	{
		printf(" world!\n");
	}
	fun4();
}

int main(void)
{
	fun3();
	return 0;
}

#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <asm/vsyscall.h>

#define TEST_COUNT	100000

extern void *vdso_sym(const char *version, const char *name);
extern void vdso_init_from_sysinfo_ehdr(uintptr_t base);
extern void vdso_init_from_auxv(void *auxv);

int strcmp(const char *a, const char *b)
{
	/* This implementation is buggy: it never returns -1. */
	while (*a || *b) {
		if (*a != *b)
			return 1;
		if (*a == 0 || *b == 0)
			return 1;
		a++;
		b++;
	}

	return 0;
}

static size_t strlen(char *str)
{
	size_t len = 0;

	while (*str++ != '\0')
		len++;

	return len;
}

static __attribute((noinline, noclone)) long syscall(unsigned int number, ...)
{
	long ret;
#ifdef __x86_64__
	asm volatile(
		"movq %%rsi, %%rdi\n"
		"movq %%rdx, %%rsi\n"
		"movq %%rcx, %%rdx\n"
		"movq %%r8, %%r10\n"
		"movq %%r9, %%r8\n"
		"movq 8(%%rsp),%%r9\n"
		"syscall"
		: "=a" (ret)
		: "a" (number)
		: "r10", "rdi", "rsi", "rdx", "r8"
		);
#else
	asm volatile(
		"movl 8(%%esp), %%ebx\n"
		"movl 12(%%esp), %%ecx\n"
		"movl 16(%%esp), %%edx\n"
		"movl 20(%%esp), %%esi\n"
		"movl 24(%%esp), %%edi\n"
		"movl 28(%%esp), %%ebp\n"
		"int $0x80"
		: "=a" (ret)
		: "a" (number)
		: "ebx", "ecx", "edx", "esi", "edi", "ebp"
		);
#endif
	return ret;
}

static char *itoa(long num, char *buf)
{
	char *p = buf;
	long tmp;

	if (num < 0) {
		*p++ = '-';
		num = ~num + 1;
	}

	tmp = num;
	do {
		tmp /= 10;
		p++;
	} while (tmp);

	*p-- = '\0';
	do {
		*p-- = (num % 10) + '0';
		num /= 10;
	} while (num);

	return buf;
}

void print_time(struct timespec *tms, struct timespec *tme)
{
	long double time;
	char buf[32];

	time = tme->tv_sec - tms->tv_sec;
	time += (tme->tv_nsec - tms->tv_nsec) / 1000000000.0;

	itoa((long) time, buf);
	syscall(__NR_write, 1, buf, strlen(buf));

	syscall(__NR_write, 1, ".", 1);

	itoa(time * 1000000000L, buf);
	syscall(__NR_write, 1, buf, strlen(buf));
}

__attribute__((externally_visible)) void c_main(void **stack)
{
	int i;
	void *pgtime;
	struct timespec tm_start, tm_end;
	struct timeval tv;
	struct timezone tz;
	long argc = (long) *stack;

	stack += argc + 2;

	while (*stack)
		stack++;
	stack++;

	vdso_init_from_auxv((void *)stack);

	/* Test syscall */
	syscall(__NR_clock_gettime, CLOCK_MONOTONIC, &tm_start);
	for (i = 0; i < TEST_COUNT; i++)
		syscall(__NR_gettimeofday, &tv, &tz);
	syscall(__NR_clock_gettime, CLOCK_MONOTONIC, &tm_end);

	syscall(__NR_write, 1, "Syscall: ", 9);
	print_time(&tm_start, &tm_end);

	/* Test vsyscall */
	pgtime = (void *) VSYSCALL_ADDR(__NR_vgettimeofday);
	syscall(__NR_clock_gettime, CLOCK_MONOTONIC, &tm_start);
	for (i = 0; i < TEST_COUNT; i++)
		((int (*)(struct timeval *, struct timezone *))pgtime)(&tv, &tz);
	syscall(__NR_clock_gettime, CLOCK_MONOTONIC, &tm_end);

	syscall(__NR_write, 1, "\nVsyscall: ", 11);
	print_time(&tm_start, &tm_end);

	/* Test vdso */
	pgtime = vdso_sym("LINUX_2.6", "__vdso_gettimeofday");
	syscall(__NR_clock_gettime, CLOCK_MONOTONIC, &tm_start);
	for (i = 0; i < TEST_COUNT; i++)
		((int (*)(struct timeval *, struct timezone *))pgtime)(&tv, &tz);
	syscall(__NR_clock_gettime, CLOCK_MONOTONIC, &tm_end);

	syscall(__NR_write, 1, "\nvdso: ", 7);
	print_time(&tm_start, &tm_end);

	syscall(__NR_exit, 0);
}

asm (
	".text\n"
	".global _start\n"
	".type _start,@function\n"
	"_start:\n\t"
	"mov %rsp,%rdi\n\t"
	"jmp c_main"
);

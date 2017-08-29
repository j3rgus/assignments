#include <stdio.h>
#include <check.h>
#include <time.h>

int is_mature(const struct tm *born)
{
	struct tm *now;
	time_t tim;

	time(&tim);
	now = localtime(&tim);
	return ((now->tm_year - born->tm_year) < 18 ? 0 : 1);
}

START_TEST(Mature)
{
	struct tm young = { .tm_year = 97 };
	struct tm old = { .tm_year = 91 };

	ck_assert_int_eq(is_mature(&young), 0);
	ck_assert_int_eq(is_mature(&old), 1);
	ck_assert_int_eq(is_mature(&young), 1);
}
END_TEST

int main(void)
{
	Suite *s = suite_create("MTest");
	TCase *test = tcase_create("MTest");
	SRunner *sr = srunner_create(s);
	int n;

	suite_add_tcase(s, test);
	tcase_add_test(test, Mature);

	srunner_run_all(sr, CK_VERBOSE);
	n = srunner_ntests_failed(sr);

	printf("Num of fails: %d\n", n);

	srunner_free(sr);
	return 0;
}

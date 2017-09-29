unsigned int compute_old(unsigned int a)
{
	return a + 1;
}
asm(".symver compute_old,compute@");

unsigned int compute_new(unsigned int a)
{
	return (a > 0) ? a * compute_new(a-1) : 1;
}
asm(".symver compute_new,compute@@VERS_1.1");

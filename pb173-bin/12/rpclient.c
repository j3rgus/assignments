#include <stdio.h>
#include <string.h>
#include <rpc/rpc.h>
#include <rpc/pmap_clnt.h>
#include <rpc/xdr.h>
#include <time.h>

#define chck_stat(stat) if (stat != RPC_SUCCESS) \
						clnt_perror(client, "clnt_call");
#define FLOAT 0x80

struct result {
	long lres;
	double fres;
};

struct long_args {
	long a;
	long b;
};

struct double_args {
	double a;
	double b;
};

int xdr_2long(XDR *xdrsp, struct long_args *largs)
{
	if (!xdr_long(xdrsp, &largs->a))
		return 0;

	if (!xdr_long(xdrsp, &largs->b))
		return 0;

	return 1;
}

int xdr_2double(XDR *xdrsp, struct double_args *fargs)
{
	if (!xdr_double(xdrsp, &fargs->a))
		return 0;

	if (!xdr_double(xdrsp, &fargs->b))
		return 0;

	return 1;
}

int main(void)
{
	char buf1[16], buf2[16], buf3[16];
	char op, flg;
	CLIENT *client;
	long mem;
	enum clnt_stat clnt_stat;
	struct  result output;
	struct timeval tm = {1, 0};
	struct long_args largs;
	struct double_args fargs;

	client = clnt_create("localhost", 100070, 1, "tcp");
	if (client == NULL) {
		printf("clnt_create error");
		return 1;
	}

	while (1) {
		scanf("%s", buf1);
		if (!strcmp(buf1, "mpr")) {
			clnt_call(client, 'C', (xdrproc_t) xdr_void, NULL,
					(xdrproc_t) xdr_long, (caddr_t) &mem, tm);
			printf("%ld\n", mem);
			goto cont;
		}

		op = buf1[0];
		if (op != 'M')
			scanf("%s", buf2);

		scanf("%s", buf3);

		if (op == 'M') {
			sscanf(buf3, "%ld", &mem);
			clnt_stat = clnt_call(client, (unsigned char)op,
						(xdrproc_t) xdr_long, (caddr_t) &mem,
						(xdrproc_t) xdr_void, NULL, tm);
			chck_stat(clnt_stat);
			goto cont;
		}

		/* Povedzme, ze c. 8888 je konst. pre MEM */
		if (!strcmp(buf2, "M"))
			strcpy(buf2, "8888");
		if (!strcmp(buf3, "M"))
			strcpy(buf3, "8888");

		flg = buf1[1];

		if (flg == 'F') {
			sscanf(buf2, "%lf", &fargs.a);
			sscanf(buf3, "%lf", &fargs.b);
			clnt_stat = clnt_call(client, op | FLOAT, (xdrproc_t) xdr_2double,
						(caddr_t) &fargs, (xdrproc_t) xdr_double,
						(caddr_t) &output.fres, tm);
			chck_stat(clnt_stat);
			printf("%lf\n", output.fres);
		} else {
			sscanf(buf2, "%ld", &largs.a);
			sscanf(buf3, "%ld", &largs.b);
			clnt_stat = clnt_call(client, op, (xdrproc_t) xdr_2long,
						(caddr_t) &largs, (xdrproc_t) xdr_long,
						(caddr_t) &output.lres, tm);
			chck_stat(clnt_stat);
			printf("%ld\n", output.lres);
		}
cont:
		printf("ok\n");
	}

	return 0;
}

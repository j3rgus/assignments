#include <rpc/rpc.h>
#include <rpc/pmap_clnt.h>

#define decode_err(stat)	if (!stat) { printf("Can't decode"); \
							svcerr_decode(xprt); }
#define reply_err(stat)		if (!stat) printf("Can't reply");
#define FLOAT 0x80

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

void dispatch(struct svc_req *req, SVCXPRT *xprt)
{
	unsigned char op = req->rq_proc;
	static long mem;
	struct long_args largs;
	struct double_args fargs;
	long ret;
	double fret;

	if (op == 'C') {
		ret = svc_sendreply(xprt, (xdrproc_t) xdr_long, (caddr_t) &mem);
		reply_err(ret);
		return;
	}

	if (op == 'M') {
		ret = svc_getargs(xprt, (xdrproc_t) xdr_long, (caddr_t) &mem);
		decode_err(ret);
		ret = svc_sendreply(xprt, (xdrproc_t) xdr_void, NULL);
		reply_err(ret);
		return;
	}

	if (op & FLOAT) {
		ret = svc_getargs(xprt, (xdrproc_t) xdr_2double, (caddr_t) &fargs);
		decode_err(ret);
	} else {
		ret = svc_getargs(xprt, (xdrproc_t) xdr_2long, (caddr_t) &largs);
		decode_err(ret);
		largs.a = (largs.a == 8888) ? mem : largs.a;
		largs.b = (largs.b == 8888) ? mem : largs.b;
	}

	switch (op) {
	case '+':
		ret = largs.a + largs.b;
		break;
	case '-':
		ret = largs.a - largs.b;
		break;
	case '*':
		ret = largs.a * largs.b;
		break;
	case '/':
		ret = largs.a / largs.b;
		break;
	case ('+' | FLOAT):
		fret = fargs.a + fargs.b;
		break;
	case ('-' | FLOAT):
		fret = fargs.a - fargs.b;
		break;
	case ('*' | FLOAT):
		fret = fargs.a * fargs.b;
		break;
	case ('/' | FLOAT):
		fret = fargs.a / fargs.b;
		break;
	default:
		svcerr_noproc(xprt);
		return;
	}

	if (op & FLOAT) {
		ret = svc_sendreply(xprt, (xdrproc_t) xdr_double, (caddr_t) &fret);
		reply_err(ret);
	} else {
		ret = svc_sendreply(xprt, (xdrproc_t) xdr_long, (caddr_t) &ret);
		reply_err(ret);
	}
}

int main(void)
{
	int ret;
	SVCXPRT *xprt;

	xprt = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (xprt == NULL) {
		printf("svctcp_create error\n");
		return 1;
	}

	ret = pmap_unset(100070, 1);
	if (!ret) {
		printf("pmap_unset error\n");
		return 1;
	}

	ret = svc_register(xprt, 100070, 1, dispatch, IPPROTO_TCP);
	if (!ret) {
		printf("svc_register error\n");
		return 1;
	}
	svc_run();

	return 0;
}

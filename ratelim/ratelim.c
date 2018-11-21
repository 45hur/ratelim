#define C_MOD_RATELIM "\x07""ratelim"

#include "log.h"
#include "program.h"
#include "ratelim.h"

#ifndef NOKRES

#include <arpa/inet.h>

int getip(struct kr_request *request, char *address)
{
	if (!request->qsource.addr) {
		debugLog("\"%s\":\"%s\"", "error", "no source address");

		return -1;
	}

	const struct sockaddr *res = request->qsource.addr;
	struct ip_addr origin = { 0 };
	bool ipv4 = true;
	switch (res->sa_family)
	{
	case AF_INET:
	{
		struct sockaddr_in *addr_in = (struct sockaddr_in *)res;
		inet_ntop(AF_INET, &(addr_in->sin_addr), address, INET_ADDRSTRLEN);
		origin.family = AF_INET;
		memcpy(&origin.ipv4_sin_addr, &(addr_in->sin_addr), 4);
		break;
	}
	case AF_INET6:
	{
		struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)res;
		inet_ntop(AF_INET6, &(addr_in6->sin6_addr), address, INET6_ADDRSTRLEN);
		origin.family = AF_INET6;
		memcpy(&origin.ipv6_sin_addr, &(addr_in6->sin6_addr), 16);
		ipv4 = false;
		break;
	}
	default:
	{
		debugLog("\"%s\":\"%s\"", "error", "qsource invalid");

		return -1;
	}
	}

	return 0;
}

int begin(kr_layer_t *ctx)
{
	debugLog("\"%s\":\"%s\"", "message", "begin");

	return ctx->state;
}

int consume(kr_layer_t *ctx, knot_pkt_t *pkt)
{
	debugLog("\"%s\":\"%s\"", "message", "consume");

	return ctx->state;
}

int produce(kr_layer_t *ctx, knot_pkt_t *pkt)
{
	debugLog("\"%s\":\"%s\"", "message", "produce");

	return ctx->state;
}

int finish(kr_layer_t *ctx)
{
	debugLog("\"%s\":\"%s\"", "message", "finish");

	struct kr_request *request = (struct kr_request *)ctx->req;
	struct kr_rplan *rplan = &request->rplan;
	char address[256] = { 0 };
	int err = 0;

	if ((err = getip(request, (char *)address)) != 0)
	{
		return err;
	}

	increment(address);

	if (isblocked(address) == 0)
	{
		ctx->state = KR_STATE_FAIL;
		return ctx->state;
	}

	return ctx->state;
}

KR_EXPORT 
const kr_layer_api_t *ratelim_layer(struct kr_module *module) {
	static kr_layer_api_t _layer = {
			.begin = &begin,
			.consume = &consume,
			.produce = &produce,
			.finish = &finish,
	};

	_layer.data = module;
	return &_layer;
}

KR_EXPORT 
int ratelim_init(struct kr_module *module)
{
	pthread_t thr_id;
	int err = 0;

	void *args = NULL;
	if ((err = create(&args)) != 0)
		return kr_error(err);

	module->data = (void *)args;

	return kr_ok();
}

KR_EXPORT 
int ratelim_deinit(struct kr_module *module)
{
	int err = 0;
	if ((err = destroy((void *)module->data)) != 0)
		return kr_error(err);

	return kr_ok();
}

KR_MODULE_EXPORT(ratelim)

#endif
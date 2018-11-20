#define C_MOD_RATELIM "\x07""ratelim"

#include "log.h"
#include "main.h"
#include "ratelim.h"

#ifndef NOKRES

void* observe(void *arg)
{
	debugLog("\"%s\":\"%s\"", "message", "observe");

	while (true)
	{
		vectorPrint();
		sleep(10000);
	}

	return NULL;
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

	if (!request->qsource.addr) {
		debugLog("\"%s\":\"%s\"", "error", "no source address");

		return ctx->state;
	}

	const struct sockaddr *res = request->qsource.addr;
	char *req_addr = NULL;
	struct ip_addr origin = { 0 };
	bool ipv4 = true;
	switch (res->sa_family) 
	{
		case AF_INET:
		{
			struct sockaddr_in *addr_in = (struct sockaddr_in *)res;
			req_addr = malloc(INET_ADDRSTRLEN);
			inet_ntop(AF_INET, &(addr_in->sin_addr), req_addr, INET_ADDRSTRLEN);
			origin.family = AF_INET;
			memcpy(&origin.ipv4_sin_addr, &(addr_in->sin_addr), 4);
			break;
		}
		case AF_INET6:
		{
			struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)res;
			req_addr = malloc(INET6_ADDRSTRLEN);
			inet_ntop(AF_INET6, &(addr_in6->sin6_addr), req_addr, INET6_ADDRSTRLEN);
			origin.family = AF_INET6;
			memcpy(&origin.ipv6_sin_addr, &(addr_in6->sin6_addr), 16);
			ipv4 = false;
			break;
		}
		default:
		{
			debugLog("\"%s\":\"%s\"", "error", "qsource invalid");
			break;
		}
	}

	vectorIncrement(req_addr);

	if (req_addr)
	{
		free(req_addr);
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
	create();

	pthread_t thr_id;
	int err = 0;

	if ((err = create()) != 0)
		return kr_error(err);

	if ((err = pthread_create(&thr_id, NULL, &observe, NULL)) != 0)
		return kr_error(err);

	module->data = (void *)thr_id;

	return kr_ok();
}

KR_EXPORT 
int ratelim_deinit(struct kr_module *module)
{
	int err = 0;
	if ((err = destroy()) != 0)
		return kr_error(err);

	void *res = NULL;
	pthread_t thr_id = (pthread_t)module->data;
	if ((err = pthread_join(thr_id, res)) != 0)
		return kr_error(errno);

	return kr_ok();
}

KR_MODULE_EXPORT(ratelim)

#endif
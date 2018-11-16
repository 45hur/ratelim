#define C_MOD_RATELIM "\x07""ratelim"

#include "main.h"
#include "ratelim.h"

#ifndef NOKRES

/* Convenience macro to declare module API. */

static void* observe(void *arg)
{
	/* ... do some observing ... */
	logDebug("\"message\":\"observe\"");

	return NULL;
}

static int load(struct kr_module *module, const char *path)
{
	logDebug("\"message\":\"load\"");

	return kr_ok();
}

static int consume(kr_layer_t *ctx, knot_pkt_t *pkt)
{
	logDebug("\"message\":\"consume\"");

	return ctx->state;
}

static int produce(kr_layer_t *ctx, knot_pkt_t *pkt)
{
	logDebug("\"message\":\"produce\"");

	return ctx->state;
}

static int finish(kr_layer_t *ctx)
{
	logDebug("\"message\":\"finish\"");

	return ctx->state;
}

KR_EXPORT const kr_layer_api_t *rank_layer(struct kr_module *module) {
	static kr_layer_api_t _layer = {
			.consume = &consume,
			.produce = &produce,
			.finish = &finish,
	};
	logDebug("\"message\":\"rank_layer\"");

	_layer.data = module;
	return &_layer;
}

KR_EXPORT int ratelim_init(struct kr_module *module)
{
	logDebug("\"message\":\"init\"");

	pthread_t thr_id;
	int err = 0;

	if ((err = create()) != 0)
		return kr_error(err);

	if ((err = pthread_create(&thr_id, NULL, &observe, NULL)) != 0)
		return kr_error(err);

	module->data = (void *)thr_id;

	return err;
}

KR_EXPORT int ratelim_deinit(struct kr_module *module)
{
	logDebug("\"message\":\"deinit\"");

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

#ifdef NOKRES
int main()
{
	int err = 0;
	if ((err = create()) != 0)
	{
		debugLog("error in create()");
		return err;
	}

	usage();
	while (userInput());

	if ((err = destroy()) != 0)
	{
		debugLog("error in destroy()");
		return err;
	}

	return err;
}
#endif
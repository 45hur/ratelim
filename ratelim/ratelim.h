#ifndef NOKRES

#ifndef RATELIM_RATELIM_H
#define RATELIM_RATELIM_H

#include <libknot/packet/pkt.h>

#include "lib/module.h"
#include "lib/layer.h"

void* observe(void *arg);
int begin(kr_layer_t *ctx);
int consume(kr_layer_t *ctx, knot_pkt_t *pkt);
int produce(kr_layer_t *ctx, knot_pkt_t *pkt);
int finish(kr_layer_t *ctx);

#endif

#endif
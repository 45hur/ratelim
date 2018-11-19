ratelim_CFLAGS := -fvisibility=hidden -fPIC
ratelim_LDFLAGS := -Wl,-undefined -Wl,dynamic_lookup
ratelim_SOURCES := \
	modules/ratelim/ratelim.c \
	modules/ratelim/log.c
ratelim_DEPEND := $(libkres)
ratelim_LIBS := $(contrib_TARGET) $(libkres_TARGET) $(libkres_LIBS) 
$(call make_c_module,ratelim)

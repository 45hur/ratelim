ratelim_CFLAGS := -fvisibility=hidden -fPIC
ratelim_SOURCES :=  modules/ratelim/log.c modules/ratelim/main.c modules/ratelim/ratelim.c
ratelim_DEPEND := $(libkres)
ratelim_LIBS := $(contrib_TARGET) $(libkres_TARGET) $(libkres_LIBS) 
$(call make_c_module,ratelim)

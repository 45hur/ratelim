FROM alpine:edge
MAINTAINER Knot Resolver team <knot-resolver-users@lists.nic.cz>

# Environment
ENV BUILD_PKGS build-base automake autoconf libtool pkgconfig git luajit-dev libuv-dev gnutls-dev jansson-dev userspace-rcu-dev curl vim bsd-compat-headers patch
ENV RUN_PKGS luajit libuv gnutls jansson bash libstdc++ lua5.1-cqueues lua5.1-http lua5.1-sec lua5.1-socket
ENV BUILD_IGNORE gmp nettle jansson gnutls lua libuv cmocka
ENV PKG_CONFIG_PATH /usr/local/lib/pkgconfig
ENV CFLAGS -O2 -ftree-vectorize -fstack-protector -g
ENV LDFLAGS -Wl,--as-needed

# Expose port
EXPOSE 53/UDP 53/TCP 853/TCP 8053/TCP

# Select entrypoint
WORKDIR /data
COPY "config.docker" "/data"
#COPY "root.keys" "/data"

#COPY "hashtable.dump" "/data"
CMD ["/usr/local/sbin/kresd", "-f", "2", "-c", "/data/config.docker"]

# Install dependencies and sources
RUN apk -U upgrade && apk add -t lua5.1-compat5.3 lua5.1-compat53 && \
apk --update add ${RUN_PKGS} && \
apk add --virtual build-dep ${BUILD_PKGS} && \
git clone -b 2.6 https://github.com/CZ-NIC/knot.git /tmp/dns && \
git clone -b 2.4 --depth 1 --recurse-submodules=modules/policy/lua-aho-corasick \
https://gitlab.labs.nic.cz/knot/knot-resolver.git /tmp/build && \
git clone https://github.com/Elctro/ratelim.git /tmp/ratelim && \
cp -a /tmp/ratelim/. /tmp/build/modules/ && \
cp -a /tmp/dns/src/knot/. /tmp/build/knot/ && \
cd /tmp/build/modules && \
patch -i modules.mk.patch modules.mk && \
cd /tmp/build && \
./scripts/bootstrap-depends.sh /usr/local && \
make -j4 install && \
# Trim down the image
cd / && \
apk del --purge build-dep && \
rm -rf /var/cache/apk/* /tmp/build
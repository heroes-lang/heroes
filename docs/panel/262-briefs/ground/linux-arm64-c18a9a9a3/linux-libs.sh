#!/bin/sh
# Panel 262's repair: what the Linux arm64 image holds of the ffi-pragmatist's libraries (the critic's C21, re-read).
cd /w
{
echo "## headers"
for h in boost/version.hpp openssl/async.h openssl/opensslv.h uv.h curl/curl.h pth.h coro.h libucontext/libucontext.h; do
  f=$(find /usr/include /usr/local/include -path "*$h" 2>/dev/null | head -1); echo "$h: ${f:-absent}"; done
grep -h 'define OPENSSL_VERSION_TEXT\|define OPENSSL_FULL_VERSION_STR' /usr/include/openssl/opensslv.h /usr/include/*/openssl/opensslv.h 2>/dev/null | head -2
grep -h 'define LIBCURL_VERSION ' /usr/include/curl/curlver.h /usr/include/*/curl/curlver.h 2>/dev/null | head -1
echo "## shared objects"
for l in libboost_context libboost_coroutine libboost_fiber libcrypto libuv libcurl libpth libnpth; do
  f=$(find / -xdev -name "$l*.so*" 2>/dev/null | head -2 | tr '\n' ' '); echo "$l: ${f:-absent}"; done
echo "## dpkg"
dpkg -l 2>/dev/null | awk '$1 == "ii" && ($2 ~ /boost|libssl|libuv|libcurl|pth|coro|ucontext/) {print $2, $3}'
echo "## time"
ls -l /usr/bin/time 2>&1
} > L-libs.txt 2>&1

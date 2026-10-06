#!/bin/bash
# Panel 178 ffi-pragmatist: every function the census headers declare whose name
# says it constructs a value (…_init, …_Init, …emptyset), with its first
# parameter's type: the world's list of "types with a constructor".
H="stdio.h stdlib.h time.h math.h sys/socket.h netinet/in.h arpa/inet.h netdb.h sys/un.h sys/utsname.h sys/stat.h sys/statvfs.h sys/time.h sys/resource.h dirent.h pthread.h signal.h termios.h poll.h sys/select.h fcntl.h unistd.h pwd.h grp.h glob.h regex.h setjmp.h locale.h ifaddrs.h net/if.h sys/mount.h sqlite3.h curl/curl.h openssl/sha.h openssl/hmac.h wchar.h"
TU=/tmp/ctors-tu-$$.c
: > $TU
for h in $H; do
  if echo "#include <$h>" | clang -x c -fsyntax-only "$@" - >/dev/null 2>&1; then echo "#include <$h>" >> $TU; else echo "MISSING $h" >&2; fi
done
clang -x c -fsyntax-only "$@" -Xclang -ast-dump $TU 2>/dev/null | sed 's/\x1b\[[0-9;]*m//g' \
 | grep -E '^\|-FunctionDecl|^`-FunctionDecl' \
 | grep -oE " [A-Za-z_0-9]*(_init|_Init|emptyset|_initialize)[A-Za-z_0-9]* '[^']*'" | sort -u
rm -f $TU

#!/bin/bash
# Critic, panel 178: every RecordDecl and FieldDecl line of the census translation unit, indentation kept.
HEADERS="stdio.h stdlib.h time.h math.h sys/socket.h netinet/in.h arpa/inet.h netdb.h sys/un.h sys/utsname.h sys/stat.h sys/statvfs.h sys/time.h sys/resource.h dirent.h pthread.h signal.h termios.h poll.h sys/select.h fcntl.h unistd.h pwd.h grp.h glob.h regex.h setjmp.h locale.h ifaddrs.h net/if.h sys/mount.h sqlite3.h curl/curl.h openssl/sha.h openssl/hmac.h"
T=$(mktemp -d)
{ for h in $HEADERS; do if echo "#include <$h>" | clang -x c -std=gnu11 -fsyntax-only - >/dev/null 2>&1; then echo "#include <$h>"; fi; done; } > $T/tu.c
clang -x c -std=gnu11 -fsigned-char -fsyntax-only -Xclang -ast-dump $T/tu.c 2>/dev/null | sed 's/\x1b\[[0-9;]*m//g' | grep -E 'RecordDecl|FieldDecl'

#!/bin/bash
# Panel 178 census: every C array field of a struct or union, as clang's AST sees
# it after macro expansion, over one translation unit including the headers below.
# Output: size <TAB> record <TAB> field <TAB> type, one line per distinct field.
# Usage: census.sh <extra clang flags...>
HEADERS="stdio.h stdlib.h time.h math.h sys/socket.h netinet/in.h arpa/inet.h netdb.h sys/un.h sys/utsname.h sys/stat.h sys/statvfs.h sys/time.h sys/resource.h dirent.h pthread.h signal.h termios.h poll.h sys/select.h fcntl.h unistd.h pwd.h grp.h glob.h regex.h setjmp.h locale.h ifaddrs.h net/if.h sys/mount.h sqlite3.h curl/curl.h openssl/sha.h openssl/hmac.h"
{
  for h in $HEADERS; do
    if echo "#include <$h>" | clang -x c -fsyntax-only "$@" - >/dev/null 2>&1; then echo "#include <$h>"; else echo "MISSING $h" >&2; fi
  done
} > /tmp/census-tu.c
clang -x c -fsyntax-only "$@" -Xclang -ast-dump /tmp/census-tu.c 2>/dev/null \
 | sed 's/\x1b\[[0-9;]*m//g' \
 | awk '
   /RecordDecl .* (struct|union) [A-Za-z_0-9]+ definition/ { for (i = 1; i <= NF; i++) if ($i == "struct" || $i == "union") rec = $(i + 1) }
   /FieldDecl/ && /\[[0-9]+\]/ {
     match($0, /\[[0-9]+\]/); n = substr($0, RSTART + 1, RLENGTH - 2)
     split($0, q, "\047"); nm = $0; sub(/.* col:[0-9]+ (referenced )?/, "", nm); sub(/ .*/, "", nm)
     print n "\t" rec "\t" nm "\t" q[2]
   }' \
 | sort -u

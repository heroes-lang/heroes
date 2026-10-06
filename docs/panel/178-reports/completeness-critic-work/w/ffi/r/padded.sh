#!/bin/bash
# Every struct the census headers define that clang pads (-Wpadded, forced by
# a sizeof of each), public ones (no leading underscore) listed.
H="stdio.h stdlib.h time.h math.h sys/socket.h netinet/in.h arpa/inet.h netdb.h sys/un.h sys/utsname.h sys/stat.h sys/statvfs.h sys/time.h sys/resource.h dirent.h pthread.h signal.h termios.h poll.h sys/select.h fcntl.h unistd.h pwd.h grp.h glob.h regex.h setjmp.h locale.h ifaddrs.h net/if.h sys/mount.h sqlite3.h curl/curl.h openssl/sha.h openssl/hmac.h"
TU=/tmp/pad-$$.c; : > $TU
for h in $H; do echo "#include <$h>" | clang -x c -fsyntax-only "$@" - >/dev/null 2>&1 && echo "#include <$h>" >> $TU; done
clang -x c -fsyntax-only "$@" -Xclang -ast-dump $TU 2>/dev/null | sed 's/\x1b\[[0-9;]*m//g' \
 | grep -oE 'RecordDecl .* struct [A-Za-z_][A-Za-z_0-9]* definition' | grep -oE 'struct [A-Za-z_][A-Za-z_0-9]*' | sort -u > /tmp/prec-$$.txt
i=0; while read kw name; do i=$((i+1)); echo "unsigned long s$i = sizeof($kw $name);"; done < /tmp/prec-$$.txt >> $TU
clang -x c -fsyntax-only -Wsystem-headers -Wpadded "$@" $TU 2>&1 | grep -oE "padding (struct|size of) '[^']*' with [0-9]+ bytes?" | sed -E "s/padding (struct|size of) 'struct ([^']*)' with ([0-9]+) bytes?/\2 \3/" \
 | awk '{pad[$1]+=$2} END {for (k in pad) print pad[k], k}' | sort -k2 > /tmp/padded-$$.txt
echo "structs defined: $(wc -l < /tmp/prec-$$.txt); padded: $(wc -l < /tmp/padded-$$.txt); public padded: $(grep -vc ' _' /tmp/padded-$$.txt)"
grep -v ' _' /tmp/padded-$$.txt | awk '{printf "%s(%s) ", $2, $1} END {print ""}'
rm -f $TU /tmp/prec-$$.txt /tmp/padded-$$.txt

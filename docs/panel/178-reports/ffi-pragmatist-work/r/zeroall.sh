#!/bin/bash
# For every struct/union the census headers DEFINE, emit R0's two candidate
# spellings, `(T){0}` and memset, under the emitter's flags plus -Wextra, and
# count what clang says. Usage: zeroall.sh <flags file> <extra clang flags...>
FLAGS=$(cat "$1"); shift
H="stdio.h stdlib.h time.h math.h sys/socket.h netinet/in.h arpa/inet.h netdb.h sys/un.h sys/utsname.h sys/stat.h sys/statvfs.h sys/time.h sys/resource.h dirent.h pthread.h signal.h termios.h poll.h sys/select.h fcntl.h unistd.h pwd.h grp.h glob.h regex.h setjmp.h locale.h ifaddrs.h net/if.h sys/mount.h sqlite3.h curl/curl.h openssl/sha.h openssl/hmac.h"
TU=/tmp/zeroall-$$.c; : > $TU
for h in $H; do echo "#include <$h>" | clang -x c -fsyntax-only "$@" - >/dev/null 2>&1 && echo "#include <$h>" >> $TU; done
clang -x c -fsyntax-only "$@" -Xclang -ast-dump $TU 2>/dev/null | sed 's/\x1b\[[0-9;]*m//g' \
 | grep -oE 'RecordDecl .* (struct|union) [A-Za-z_][A-Za-z_0-9]* definition' | grep -oE '(struct|union) [A-Za-z_][A-Za-z_0-9]*' | sort -u > /tmp/recs-$$.txt
echo "records defined: $(wc -l < /tmp/recs-$$.txt)"
for form in brace memset; do
  { cat $TU; echo '#include <string.h>'; i=0; while read kw name; do i=$((i+1));
      if [ $form = brace ]; then echo "void z$i(void) { $kw $name v; v = ($kw $name){0}; (void)v; }";
      else echo "void z$i(void) { $kw $name v; memset(&v, 0, sizeof v); (void)v; }"; fi; done < /tmp/recs-$$.txt; } > /tmp/z-$form-$$.c
  out=$(clang -x c $FLAGS -Wextra -fsyntax-only "$@" /tmp/z-$form-$$.c 2>&1)
  echo "$form: errors $(echo "$out" | grep -c 'error:'), warnings $(echo "$out" | grep -c 'warning:')"
  echo "$out" | grep -E 'error:|warning:' | sed 's/.*\(error\|warning\): //' | sort | uniq -c | sort -rn | head -5
done
rm -f $TU /tmp/recs-$$.txt /tmp/z-*-$$.c

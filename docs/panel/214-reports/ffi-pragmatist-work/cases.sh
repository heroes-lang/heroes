#!/bin/bash
# Panel 214, the ffi-pragmatist: each case's generated unit compiled as
# written (A), with the compiler's prefix read after the groups' headers and
# clang's own <stdbool.h>/<stddef.h> before them (H), and with the group's
# header replaced by the prototype its Heroes declaration gives (I). Prints
# clang's exit, its first error's place and its first note's place.
# Usage: cases.sh <dir holding runtime/ and x/>
R="$1"; X="$R/x"
FLAGS="-std=gnu11 -D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS -Wall -Werror=return-type -Werror=uninitialized -Werror=format -Werror=conditional-uninitialized -fno-strict-aliasing -fno-delete-null-pointer-checks -Werror=shorten-64-to-32 -Werror=sign-conversion -Werror=incompatible-pointer-types-discards-qualifiers -Werror=incompatible-pointer-types -fsigned-char -fno-optimize-sibling-calls -Wno-infinite-recursion -fdiagnostics-show-note-include-stack -Werror=macro-redefined -O0 -g"
printf '#include <stdbool.h>\n#include <stddef.h>\n' > "$X/h2pre.h"
for n in hs i64 e_fabs imp e_std j_typo w2053a w2053b; do
  u="$X/$n.c"
  hdr=$(sed -n 's/^#include <\([^>]*\.h\)>$/\1/p' "$u" | grep -v -E '^(heroes_unit|hero_os|heroes_runtime)\.h$' | head -1)
  proto="int32_t eight(void);"; [ "$n" = i64 ] && proto="int64_t seven(void);"
  awk 'NR<=6 && ($0=="#include \"heroes_unit.h\"" || $0=="#include <heroes_unit.h>" || $0=="#include <hero_os.h>" || $0=="#include <heroes_runtime.h>") { pre = pre $0 "\n"; next }
       $0=="#include \"heroes_standard.h\"" && !done { printf "%s", pre; done=1 }
       { print }' "$u" | sed '1a\
#include "h2pre.h"
' > "$X/$n.H.c"
  sed "s|^#include <$hdr>\$|$proto|" "$u" > "$X/$n.I.c"
  for v in A H I; do
    f="$X/$n.c"; [ $v != A ] && f="$X/$n.$v.c"
    clang $FLAGS -fsyntax-only -I "$R/runtime" -I "$X" "$f" > "$X/$n.$v.txt" 2>&1; e=$?
    err=$(grep -m1 -E ': (fatal )?error:' "$X/$n.$v.txt" | sed -E 's#^.*[/\\]([^/\\]+:[0-9]+):[0-9]+: (fatal )?error: (.{0,70}).*#\1 \3#')
    note=$(awk '/: (fatal )?error:/{s=1} s && /: note:/{print; exit}' "$X/$n.$v.txt" | sed -E 's#^.*[/\\]([^/\\]+:[0-9]+):[0-9]+: note: (.{0,40}).*#\1 \2#')
    echo "$n $v exit=$e | err: ${err:--} | note: ${note:--}"
  done
done

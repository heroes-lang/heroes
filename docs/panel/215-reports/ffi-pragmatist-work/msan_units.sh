#!/bin/bash
# ffi-pragmatist, panel 215: build a Heroes program with a compiler made from the seed in this
# container, then recompile the very units `heroes build` wrote, one per module, at four levels,
# plain and under MemorySanitizer: the program's own output plain, and MSan's reports with the
# .hero and .h lines they stand on. A unit may be replaced by a hand-edited one (a route's C):
# Usage (in the container): msan_units.sh <folder under /src/build> <main.hero> [unit.c=replacement.c ...]
set -u
dir=$1; shift; main=$1; shift
mkdir -p /tmp/m && cd /tmp/m
[ -d runtime ] || cp -r /src/runtime /src/seed . ; [ -x /cache/hc ] || clang -O1 -I runtime seed/heroes.c runtime/runtime.c -o /cache/hc 2>/dev/null || { echo "compiler build failed"; exit 1; }; cp /cache/hc hc
export HEROES_RUNTIME=/tmp/m/runtime
rm -rf w && mkdir w && cp -r /src/build/$dir/* w/ 2>/dev/null; cd w; rm -rf build
../hc build $main -o plain > build.txt 2>&1 || { echo "$main: build failed"; head -20 build.txt; exit 1; }
units=$(ls build/tu-*/*.c)
[ -n "${PATCH:-}" ] && { perl -0pi /src/build/routes/$PATCH build/tu-*/main.c build/tu-*/rec.c || { echo "patch failed"; exit 1; }; echo "patched by $PATCH"; }
for r in "$@"; do u=${r%%=*}; s=${r#*=}; t=$(ls build/tu-*/$u); cp $s $t; echo "replaced $t by $s"; done
words="-std=gnu11 -D_USE_MATH_DEFINES -fno-strict-aliasing -fno-delete-null-pointer-checks -fsigned-char -fno-optimize-sibling-calls -Wno-infinite-recursion -Werror=incompatible-pointer-types -g -I /tmp/m/runtime -I ."
echo "leg $(uname -m), $(clang --version | head -1); $main; units: $(echo $units | wc -w)"
for O in -O0 -O1 -O2 -O3; do
  clang $words $O $units /tmp/m/runtime/runtime.c -o q$O -lm ${LIBS:-} 2> qcc$O.txt || { echo "$O plain: cc failed"; head -5 qcc$O.txt; continue; }
  echo "$O plain: $(./q$O 2>&1 | tr -cd '[:print:]\n' | grep -oE '(eq|write|send|memcmp|sha|n) .*' | tr '\n' '|')"
  clang $words $O -fsanitize=memory -fsanitize-memory-track-origins=2 -fsanitize-recover=memory $units /tmp/m/runtime/runtime.c -o p$O -lm ${LIBS:-} 2> cc$O.txt || { echo "$O: cc failed"; head -5 cc$O.txt; continue; }
  MSAN_OPTIONS=halt_on_error=0 timeout 60 ./p$O > out$O.txt 2> m$O.txt
  at=$(grep -A6 "use-of-uninitialized-value" m$O.txt | grep -oE "[a-z_0-9]+\.(hero|h):[0-9]+(:[0-9]+)?" | grep -v runtime | sort | uniq -c | tr -s ' ' | tr '\n' ';')
  echo "$O msan: $(grep -c 'WARNING: MemorySanitizer' m$O.txt) reports; frames: ${at:-none}"
  [ -n "${KEEP:-}" ] && cp m$O.txt $KEEP/${PATCH:-none}-m$O.txt
done

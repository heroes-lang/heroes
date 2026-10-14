#!/bin/bash
# Critic, panel 215 pass 2, in the x86-64 container (the copy at /src, read-only):
# 1. the mechanism: small.hero under MSan with today's runtime, and with the
#    lend region re-poisoned after hero_lend_local's memset;
# 2. the values: the ffi-pragmatist's two probes built plain (no sanitiser),
#    each binary run three times, at -O0 (heroes build's default) and -O2.
set -u
mkdir -p /tmp/m && cd /tmp/m && cp -r /src/runtime /src/seed . && cp -r /src/build/mech/* .
git_runtime=/src/runtime
# today's runtime, without the prototype's hero_pad_zero (the copy is patched): the seed's own
clang -O1 -I runtime seed/heroes.c runtime/runtime.c -o hc 2>/dev/null || { echo "compiler build failed"; exit 1; }
export HEROES_RUNTIME=/tmp/m/runtime
cp -r runtime runtime_poison
perl -0pi -e 's/(    memset\(local, 0, size\);\n)/$1#if defined(__has_feature)\n#if __has_feature(memory_sanitizer)\n    { extern void __msan_poison(const volatile void *, unsigned long); __msan_poison(local, size); }\n#endif\n#endif\n/' runtime_poison/parts/lend.c
grep -c "__msan_poison(local" runtime_poison/parts/lend.c
F="-std=gnu11 -D_USE_MATH_DEFINES -fno-strict-aliasing -fno-delete-null-pointer-checks -fsigned-char -fno-optimize-sibling-calls -Wno-infinite-recursion -D_GNU_SOURCE -g"
echo "leg $(uname -m), $(clang --version | head -1)"
./hc build small.hero --emit-c -o small.c > /dev/null 2>&1 || echo "emit failed"
for rt in runtime runtime_poison; do
  for O in -O0 -O1 -O2 -O3 "-O2 -fno-inline"; do
    tag=$(echo "$O" | tr -d ' ')
    clang $F $O -fsanitize=memory -fsanitize-recover=memory -I $rt -I . small.c $rt/runtime.c -o s$tag -lm 2> cc.txt || { echo "$rt $O cc failed"; head -3 cc.txt; continue; }
    MSAN_OPTIONS=halt_on_error=0 timeout 60 ./s$tag > /dev/null 2> m.txt
    lines=$(grep -A4 "use-of-uninitialized-value" m.txt | grep -oE "small\.hero:[0-9]+" | sort | uniq -c | tr -s ' ' | tr '\n' ';')
    echo "$rt small.hero $O: $(grep -c 'WARNING: MemorySanitizer' m.txt) reports at:${lines:- none}"
  done
done
W="-std=gnu11 -D_USE_MATH_DEFINES -fno-strict-aliasing -fno-delete-null-pointer-checks -fsigned-char -fno-optimize-sibling-calls -Wno-infinite-recursion -g -I /tmp/m/runtime -I ."
for d in q1m main; do
  cd /tmp/m/$d && rm -rf build && ../hc build main.hero -o plain > b.txt 2>&1 || { echo "$d build failed"; head -5 b.txt; cd ..; continue; }
  units=$(ls build/tu-*/*.c)
  for O in -O0 -O2; do
    clang $W $O $units /tmp/m/runtime/runtime.c -o v$O -lm ${LIBS:-} 2> vcc.txt || { echo "$d $O cc failed"; head -3 vcc.txt; continue; }
    for i in 1 2 3; do echo "$d $O plain run $i: $(./v$O 2>&1 | tr -cd '[:print:]\n' | grep -oE '(eq|memcmp|sha) .*|^[0-9]+ [0-9-]+ [0-9]+$' | tr '\n' '|')"; done
  done
  cd /tmp/m
done

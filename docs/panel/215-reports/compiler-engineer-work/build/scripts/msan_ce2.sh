#!/bin/bash
# Compiler-engineer, panel 215 build: in a container, the copy mounted read-only
# at /src. Today = the seed with 7e5c02048's runtime; g = route G recursive,
# built from its own emitted C (build/g2c/heroes.c) with the copy's runtime.
# 1. every probe under MemorySanitizer at five levels; 2. the value probes
# built plain by `heroes build -O0` and `-O2`, each binary run three times.
set -u
mkdir -p /tmp/m && cd /tmp/m
cp -r /src/runtime runtime_g; cp -r /src/build/rt_today/runtime runtime_t; cp -r /src/seed .
cp /src/build/g2c/heroes.c g2.c; cp /src/build/padc/*.hero /src/build/padc/*.h /src/build/padc/*.expected .
cp -r /src/build/mech/main main
clang -O1 -I runtime_t seed/heroes.c runtime_t/runtime.c -o hc_today 2>/dev/null || { echo "today build failed"; exit 1; }
clang -O1 -I runtime_g g2.c runtime_g/runtime.c -o hc_g 2>/dev/null || { echo "route build failed"; exit 1; }
F="-std=gnu11 -D_USE_MATH_DEFINES -Wall -Werror=return-type -Werror=uninitialized -fno-strict-aliasing -fno-delete-null-pointer-checks -fsigned-char -fno-optimize-sibling-calls -Wno-infinite-recursion -D_GNU_SOURCE -g"
echo "leg $(uname -m), $(clang --version | head -1)"
for comp in today g; do
  if [ $comp = today ]; then RT=runtime_t; else RT=runtime_g; fi
  export HEROES_RUNTIME=/tmp/m/$RT
  for prog in "$@"; do
    b=${prog%.hero}_$comp
    ./hc_$comp build $prog --emit-c -o $b.c > $b.emit.txt 2>&1 || { echo "$comp $prog: emit failed"; tail -5 $b.emit.txt; continue; }
    echo "$comp $prog: hero_pad_zero lines $(grep -c 'hero_pad_zero(' $b.c)"
    for O in -O0 -O1 -O2 -O3 "-O2 -fno-inline"; do
      tag=$(echo "$O" | tr -d ' ')
      clang $F $O -fsanitize=memory -fsanitize-recover=memory -I $RT -I . $b.c $RT/runtime.c -o $b$tag -lm 2> $b$tag.cc.txt || { echo "$comp $prog $O: cc failed"; tail -3 $b$tag.cc.txt; continue; }
      MSAN_OPTIONS=halt_on_error=0 timeout 120 ./$b$tag > $b$tag.out 2> $b$tag.msan.txt
      code=$?
      lines=$(grep -A4 "use-of-uninitialized-value" $b$tag.msan.txt | grep -oE "[a-z_.0-9-]+\.hero:[0-9]+" | sort | uniq -c | tr -s ' ' | tr '\n' ';')
      same=""
      if [ -f ${prog%.hero}.expected ]; then cmp -s $b$tag.out ${prog%.hero}.expected && same=" expected:same" || same=" expected:DIFFERS"; fi
      echo "$comp $prog $O: exit $code, $(grep -c 'WARNING: MemorySanitizer' $b$tag.msan.txt) reports at:${lines:- none}; printed: $(tail -n1 $b$tag.out | tr -cd '[:print:]' | head -c 120)$same"
    done
  done
done
echo "-- values, no sanitizer, heroes build -O0 (the default) and -O2, three runs each"
for comp in today g; do
  if [ $comp = today ]; then RT=runtime_t; else RT=runtime_g; fi
  export HEROES_RUNTIME=/tmp/m/$RT
  for O in -O0 -O2; do
    ./hc_$comp build dv.hero $O -o dv_$comp$O > dvb.txt 2>&1 || { echo "$comp dv $O build failed"; head -3 dvb.txt; continue; }
    echo "$comp dv.hero heroes build $O: $(./dv_$comp$O) | $(./dv_$comp$O) | $(./dv_$comp$O)"
  done
  for O in -O0 -O2; do
    (cd main && rm -rf build && ../hc_$comp build main.hero $O -o v_$comp$O > b.txt 2>&1) || { echo "$comp main $O build failed"; head -5 main/b.txt; continue; }
    for i in 1 2 3; do echo "$comp main.hero heroes build $O run $i: $(./main/v_$comp$O 2>&1 | tr -cd '[:print:]\n' | grep -oE '(eq|memcmp|sha) .*' | tr '\n' '|')"; done
  done
done

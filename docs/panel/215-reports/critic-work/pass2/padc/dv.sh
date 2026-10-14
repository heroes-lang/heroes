#!/bin/bash
# Critic, panel 215 pass 2: dv.hero built plain by today's compiler and the prototype, -O0 and -O2, three runs each.
set -u
mkdir -p /tmp/m && cd /tmp/m && cp -r /src/runtime /src/seed . && cp /src/build/g2c/heroes.c g2.c && cp /src/build/padc/dv.hero /src/build/padc/dv.h .
export HEROES_RUNTIME=/tmp/m/runtime
clang -O1 -I runtime seed/heroes.c runtime/runtime.c -o hc_today 2>/dev/null && clang -O1 -I runtime g2.c runtime/runtime.c -o hc_g 2>/dev/null || { echo "compiler build failed"; exit 1; }
echo "leg $(uname -m), $(clang --version | head -1)"
for comp in today g; do
  for O in -O0 -O2; do
    ./hc_$comp build dv.hero $O -o dv_$comp$O > b.txt 2>&1 || { echo "$comp $O build failed"; head -3 b.txt; continue; }
    echo "$comp heroes build $O: $(./dv_$comp$O) | $(./dv_$comp$O) | $(./dv_$comp$O)"
  done
done

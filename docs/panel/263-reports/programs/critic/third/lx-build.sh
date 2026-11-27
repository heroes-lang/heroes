#!/bin/bash
# lx-build.sh: inside c263c, the seed's compiler and stage 2 of /s/lx/bpp then /s/lx/base.
set -u
export TMPDIR=/s/tmp-lx; mkdir -p $TMPDIR
for t in bpp base; do
  cd /s/lx/$t || exit 2
  echo "== $t seed $(date +%T)"; time clang -I runtime seed/heroes.c runtime/runtime.c -o heroes 2>&1 | tail -3
  echo "== $t stage 2 $(date +%T)"; time ./heroes build selfhost/main.hero -o heroes-next 2>&1 | tail -4
  ./heroes-next --version
done
echo LX-BUILT

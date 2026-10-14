#!/bin/bash
# Critic, panel 215 pass 2: the union probes under today, the sound G (the
# prototype), the naive G (declared spans) and the gap macro (ffi-pragmatist).
F="-std=gnu11 -fno-strict-aliasing -fno-delete-null-pointer-checks -fsigned-char -D_GNU_SOURCE ${OPT:--O2} -g"
R=../../runtime
for p in "uni struct s kind fd" "unimid struct m kind fd tail"; do
  set -- $p; b=$1; shift; ct="$1 $2"; shift 2
  python3 -I patch_g.py naive ${b}_today.c ${b}_naive.c "$ct" "$@"
  python3 -I patch_g.py gaps ${b}_today.c ${b}_gaps.c "$ct" "$@"
  for v in today g naive gaps; do
    clang $F -I $R -I . ${b}_$v.c $R/runtime.c -o ${b}_$v.bin -lm 2>${b}_$v.cc.txt || { echo "$b $v cc failed"; head -3 ${b}_$v.cc.txt; continue; }
    printf "%-7s %-6s prints %s\n" $b $v "$(./${b}_$v.bin)"
  done
done

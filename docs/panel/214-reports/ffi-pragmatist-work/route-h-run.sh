#!/bin/bash
# Route H, compiled: every tracked emission holding a group, as written (A)
# and with the compiler's prefix moved to just before heroes_standard.h (H).
W=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-panel-214/ffi-pragmatist
cd $W
OUT=$W/build/h
FLAGS="-std=gnu11 -D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS -Wall -Werror=return-type -Werror=uninitialized -Werror=format -Werror=conditional-uninitialized -fno-strict-aliasing -fno-delete-null-pointer-checks -Werror=shorten-64-to-32 -Werror=sign-conversion -Werror=incompatible-pointer-types-discards-qualifiers -Werror=incompatible-pointer-types -fsigned-char -fno-optimize-sibling-calls -Wno-infinite-recursion -fdiagnostics-show-note-include-stack -Werror=macro-redefined -O0 -g"
: > $OUT/table.txt
for f in tests/emission/*.c; do
  grep -q 'heroes_guard_open.h' "$f" || continue
  name=$(basename "$f" .c)
  src=$(sed -n '1s/.*from \(.*\)\. Do not edit.*/\1/p' "$f")
  dir=$(dirname "$src")
  h="$OUT/units/$name.c"; mkdir -p $OUT/units
  awk '
    NR<=6 && ($0=="#include \"heroes_unit.h\"" || $0=="#include <heroes_unit.h>" || $0=="#include <hero_os.h>" || $0=="#include <heroes_runtime.h>") { pre = pre $0 "\n"; next }
    $0=="#include \"heroes_standard.h\"" && !done { printf "%s", pre; done=1 }
    { print }
  ' "$f" > "$h"
  clang $FLAGS -fsyntax-only -I runtime -I "$dir" -I /opt/homebrew/include "$f" > $OUT/units/$name.a.txt 2>&1; a=$?
  clang $FLAGS -fsyntax-only -I runtime -I "$dir" -I /opt/homebrew/include "$h" > $OUT/units/$name.h.txt 2>&1; b=$?
  echo "$a $b $name" >> $OUT/table.txt
done

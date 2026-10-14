#!/bin/bash
# Route H's two softer variants: clang's own <stdbool.h> and <stddef.h> kept
# before the groups (h2), and those plus the ten integer typedefs (h3); the
# runtime's own declarations after the groups in both.
W=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-panel-214/ffi-pragmatist
cd $W
FLAGS="-std=gnu11 -D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS -Wall -Werror=return-type -Werror=uninitialized -Werror=format -Werror=conditional-uninitialized -fno-strict-aliasing -fno-delete-null-pointer-checks -Werror=shorten-64-to-32 -Werror=sign-conversion -Werror=incompatible-pointer-types-discards-qualifiers -Werror=incompatible-pointer-types -fsigned-char -fno-optimize-sibling-calls -Wno-infinite-recursion -fdiagnostics-show-note-include-stack -Werror=macro-redefined -O0 -g"
: > build/h/table23.txt
while read a b name; do
  [ "$a" = 0 ] || continue
  f=tests/emission/$name.c; src=$(sed -n '1s/.*from \(.*\)\. Do not edit.*/\1/p' "$f"); dir=$(dirname "$src")
  for v in h2 h3; do sed "1a\\
#include \"${v}pre.h\"
" build/h/units/$name.c > build/h/units/$name.$v.c; done
  clang $FLAGS -fsyntax-only -I build/h -I runtime -I "$dir" -I /opt/homebrew/include build/h/units/$name.h2.c > build/h/units/$name.h2.txt 2>&1; x=$?
  clang $FLAGS -fsyntax-only -I build/h -I runtime -I "$dir" -I /opt/homebrew/include build/h/units/$name.h3.c > build/h/units/$name.h3.txt 2>&1; y=$?
  echo "$x $y $name" >> build/h/table23.txt
done < build/h/table.txt

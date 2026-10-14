#!/bin/bash
# Route G: the hs unit's refusal as SARIF and as clang's serialized
# diagnostics. Usage: g.sh <dir holding runtime/ and x/> <tag>
R="$1"; X="$R/x"; T="$2"
FLAGS="-std=gnu11 -D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS -Wall -Werror=return-type -Werror=uninitialized -Werror=format -Werror=conditional-uninitialized -fno-strict-aliasing -fno-delete-null-pointer-checks -Werror=shorten-64-to-32 -Werror=sign-conversion -Werror=incompatible-pointer-types-discards-qualifiers -Werror=incompatible-pointer-types -fsigned-char -fno-optimize-sibling-calls -Wno-infinite-recursion -fdiagnostics-show-note-include-stack -Werror=macro-redefined -O0 -g"
for n in hs e_std; do
  clang $FLAGS -fsyntax-only -fdiagnostics-format=sarif -I "$R/runtime" -I "$X" "$X/$n.c" > "$X/$n.$T.sarif" 2>&1; echo "$n sarif exit=$?"
  clang $FLAGS -fsyntax-only --serialize-diagnostics "$X/$n.$T.dia" -I "$R/runtime" -I "$X" "$X/$n.c" > /dev/null 2>&1; echo "$n dia exit=$? bytes=$(wc -c < "$X/$n.$T.dia")"
done

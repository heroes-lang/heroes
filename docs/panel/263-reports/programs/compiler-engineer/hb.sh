#!/bin/bash
# hb.sh <c file> <runtime dir> <level O0|O2> <out> [extra clang words...]: a hand build of an emission, the
# compiler's own words (selfhost/cli/flags.hero:136-154, frame words, level, debug word).
set -u
c=$1; rt=$2; lv=$3; out=$4; shift 4
dbg=-g; [ "$lv" = O2 ] && dbg=-gline-tables-only
W=(-std=gnu11 -D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS -Wall -Werror=return-type -Werror=uninitialized -Werror=format
   -Werror=conditional-uninitialized -fno-strict-aliasing -fno-delete-null-pointer-checks -Werror=shorten-64-to-32
   -Werror=sign-conversion -Werror=incompatible-pointer-types-discards-qualifiers -Werror=incompatible-pointer-types
   -fsigned-char -fno-optimize-sibling-calls -Wno-infinite-recursion -Werror=macro-redefined)
if [ "$(uname)" = Linux ]; then W+=(-fstack-clash-protection); fi
W+=(-fno-omit-frame-pointer -$lv $dbg)
L=(-lm); if [ "$(uname)" = Linux ]; then L=(-lm -lpthread -ldl -rdynamic); fi
timeout -k 5 600 clang "${W[@]}" "$@" -I "$rt" -I "$(dirname "$c")" "$c" "$rt/runtime.c" -o "$out" "${L[@]}"

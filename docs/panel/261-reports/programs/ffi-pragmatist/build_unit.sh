#!/bin/sh
# Panel 261, the ffi-pragmatist: compile one hand-edited unit with the words
# `heroes build` compiled the emitted unit with (the unit's .rsp under build/
# words/, read from that build), and link it with the runtime's and the
# library's objects of the same build. Run from the program's directory.
# usage: sh ../build_unit.sh <unit.c> <out> <unit-words.rsp> <link-words.rsp> [extra link words...]
set -e
unit=$1; out=$2; words=$3; link=$4; shift 4
clang @"$words" -c "$unit" -o "$out.o"
objs=$(grep '\.o"$' "$link" | grep -v '/tu-[0-9a-f]*/[a-z]*-[0-9a-f]*\.o"$' | tr -d '"')
lib=$(grep '/library-[0-9a-f]*\.o"$' "$link" | tr -d '"')
flags=$(grep -v '\.o"$' "$link" | tr -d '"' | tr '\n' ' ')
clang $flags "$out.o" $objs $lib "$@" -o "$out"

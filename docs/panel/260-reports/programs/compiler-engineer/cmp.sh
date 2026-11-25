#!/bin/bash
# cmp.sh: what the emitted C owes the change (`.claude/rules/verification.md` § The formatter's probe: identical
# except the line `hero_program_sources`). (1) The base's compiler and the route's each emit the base's own
# `selfhost/`, which no group of says `layer`; (2) each of print, clock, audio and ui, as written for the base and
# declared for the route, emitted from a folder of its own under one file name, so the `#line`s agree; then `cmp`
# of the two with that one line taken out of each.
S=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/260-compiler-engineer-scratch
o=$S/cmp; rm -rf $o; mkdir -p $o
strip() { grep -v 'hero_program_sources(void) {' "$1" > "$1.cut"; }
cd $S/base-src
/usr/bin/time -p $S/base-o2/heroes build selfhost/main.hero --emit-c > $o/self-base.c 2> $o/self-base.err; echo "self base exit $? $(wc -c < $o/self-base.c)"
/usr/bin/time -p $S/mine-o2/heroes build selfhost/main.hero --emit-c > $o/self-mine.c 2> $o/self-mine.err; echo "self route exit $? $(wc -c < $o/self-mine.c)"
strip $o/self-base.c; strip $o/self-mine.c
cmp $o/self-base.c.cut $o/self-mine.c.cut && echo "self: identical but hero_program_sources"
for p in print clock audio ui; do
  mkdir -p $o/$p-base $o/$p-mine; cp $S/progs/$p.hero $o/$p-base/main.hero; cp $S/progs-declared/$p.hero $o/$p-mine/main.hero
  (cd $o/$p-base && $S/base-o2/heroes build main.hero --emit-c > main.c 2> err.txt; echo "$p base exit $?")
  (cd $o/$p-mine && $S/mine-o2/heroes build main.hero --emit-c > main.c 2> err.txt; echo "$p route exit $?")
  strip $o/$p-base/main.c; strip $o/$p-mine/main.c
  cmp $o/$p-base/main.c.cut $o/$p-mine/main.c.cut && echo "$p: identical but hero_program_sources ($(wc -c < $o/$p-base/main.c) bytes)"
done

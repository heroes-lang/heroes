#!/bin/bash
# ident.sh: the emitted C of stage 2 (before) and of the (c2) compiler (after), the compiler's own and every example,
# compared with the `hero_program_sources` line taken out (.claude/rules/verification.md, the formatter's probe).
set -u
S=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/263-compiler-engineer-scratch
COPY=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/263-compiler-engineer
export TMPDIR=$S
cd $COPY
for root in selfhost/main.hero examples/*/main.hero; do
  name=$(echo $root | tr '/' '_')
  HEROES_RUNTIME=$COPY/runtime timeout -k 5 900 ./heroes-next build $root --emit-c -o $S/ident/$name.before.c > $S/ident/$name.before.log 2>&1; eb=$?
  HEROES_RUNTIME=$S/rt-c timeout -k 5 900 $S/heroes-c2 build $root --emit-c -o $S/ident/$name.after.c > $S/ident/$name.after.log 2>&1; ea=$?
  if [ $eb -ne 0 ] || [ $ea -ne 0 ]; then echo "$root: emit exits $eb / $ea"; continue; fi
  if cmp -s <(grep -v 'hero_program_sources(void) {' $S/ident/$name.before.c) <(grep -v 'hero_program_sources(void) {' $S/ident/$name.after.c); then
    echo "$root: identical ($(wc -c < $S/ident/$name.before.c) bytes)"
  else
    echo "$root: DIFFERS, $(grep -c hero_stack_check $S/ident/$name.after.c) checks, $(grep -c hero_thread_guard $S/ident/$name.after.c) guards"
  fi
  rm -f $S/ident/$name.before.c $S/ident/$name.after.c
done

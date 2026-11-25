#!/bin/bash
# build.sh: the new base's compiler (c3885b81b) built as ground/build.sh built 80e2e39ad's:
# stage 1 = clang on the base's seed; stage 1 emits the base's selfhost/ as C; clang -O2 of that C beside the base's runtime.
S=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/panel260-repair-scratch
cd $S/src || exit 2
echo "stage1 start $(date '+%H:%M:%S') $(uptime | sed 's/.*averages: //')"
/usr/bin/time -p clang -I runtime seed/heroes.c runtime/runtime.c -o $S/stage1 > $S/stage1.log 2>&1; echo "stage1 exit $? $(date '+%H:%M:%S')"
/usr/bin/time -p $S/stage1 build selfhost/main.hero --emit-c > $S/main.c 2> $S/emit.err; echo "emit exit $? $(wc -c < $S/main.c) $(date '+%H:%M:%S')"
mkdir -p $S/main-o2 && cp -R $S/src/runtime $S/main-o2/runtime
cd $S/main-o2 && /usr/bin/time -p clang -O2 -I runtime $S/main.c runtime/runtime.c -o heroes.new > $S/o2.log 2>&1 && mv heroes.new heroes; echo "o2 exit $? $(date '+%H:%M:%S') $(uptime | sed 's/.*averages: //')"
echo BUILD-DONE

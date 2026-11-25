#!/bin/bash
# mac-nm.sh: linux-nm.sh's method on this Mac: every C member of main's runtime/platform compiled
# alone with main's -O0 words, nm of each object (Darwin's leading `_` dropped), closure.py.
S=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/panel260-repair-scratch; out=$S/mac-nm.txt
echo "# $(date '+%Y-%m-%d %H:%M:%S') $(clang --version | head -1) load $(sysctl -n vm.loadavg)" > $out
rm -rf $S/macnm && mkdir -p $S/macnm/o && cd $S/src
WORDS="-std=gnu11 -D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS -Wall -Werror=return-type -Werror=uninitialized -Werror=format -Werror=conditional-uninitialized -fno-strict-aliasing -fno-delete-null-pointer-checks -Werror=shorten-64-to-32 -Werror=sign-conversion -Werror=incompatible-pointer-types-discards-qualifiers -Werror=incompatible-pointer-types -fsigned-char -fno-optimize-sibling-calls -Wno-infinite-recursion -fdiagnostics-show-note-include-stack -Werror=macro-redefined -O0 -g"
M=$(grep -o '"platform/[a-z_0-9]*\.c"' runtime/runtime.c | tr -d '"')
n=0; bad=0
for m in $M; do b=$(basename $m .c); n=$((n+1)); clang $WORDS -c runtime/$m -I runtime -o $S/macnm/o/$b.o 2> $S/macnm/o/$b.err || { bad=$((bad+1)); echo "did not compile: $b" >> $out; }; done
echo "members $n, did not compile $bad" >> $out
for f in $S/macnm/o/*.o; do b=$(basename $f); nm $f | awk -v m=$b 'NF>=2 {t=$(NF-1); s=$NF; sub(/^_/, "", s); if (t=="U") print "U", m, s; else if (t ~ /^[A-Z]$/) print "D", m, s}'; done > $S/nm-mac.txt
python3 -I $S/closure.py $S/src/runtime $S/nm-mac.txt hero_time.h hero_ui.h hero_net.h hero_audio.h > $S/closure-mac.txt 2>&1; echo "closure exit $?" >> $out
tail -6 $S/closure-mac.txt >> $out

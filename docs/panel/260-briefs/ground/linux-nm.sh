#!/bin/bash
# linux-nm.sh, inside heroes-linux-arm64:latest with this folder at /w: every C member of main's
# runtime/platform compiled alone with main's -O0 words (selfhost/cli/flags.hero `flags()`, -O0 -g),
# each object's symbols read by nm, then closure.py (the lane's py/closure.py, unchanged).
W=/w; out=$W/linux-nm.txt
echo "# $(date '+%Y-%m-%d %H:%M:%S') $(clang --version | head -1) nproc=$(nproc) load $(cut -d' ' -f1-3 /proc/loadavg)" > $out
rm -rf /x && mkdir -p /x/o && cp -R $W/src/runtime /x/runtime && cd /x
WORDS="-std=gnu11 -D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS -Wall -Werror=return-type -Werror=uninitialized -Werror=format -Werror=conditional-uninitialized -fno-strict-aliasing -fno-delete-null-pointer-checks -Werror=shorten-64-to-32 -Werror=sign-conversion -Werror=incompatible-pointer-types-discards-qualifiers -Werror=incompatible-pointer-types -fsigned-char -fno-optimize-sibling-calls -Wno-infinite-recursion -fdiagnostics-show-note-include-stack -Werror=macro-redefined -O0 -g"
M=$(grep -o '"platform/[a-z_0-9]*\.c"' runtime/runtime.c | tr -d '"')
n=0; bad=0
for m in $M; do b=$(basename $m .c); n=$((n+1)); clang $WORDS -c runtime/$m -I runtime -o /x/o/$b.o 2> /x/o/$b.err || { bad=$((bad+1)); echo "did not compile: $b $(head -c 200 /x/o/$b.err)" >> $out; }; done
echo "members $n, did not compile $bad" >> $out
for f in /x/o/*.o; do b=$(basename $f); nm $f | awk -v m=$b 'NF>=2 {t=$(NF-1); s=$NF; if (t=="U") print "U", m, s; else if (t ~ /^[A-Z]$/) print "D", m, s}'; done > $W/nm-linux.txt
python3 -I $W/closure.py /x/runtime $W/nm-linux.txt hero_time.h hero_ui.h hero_net.h hero_audio.h > $W/closure-linux.txt 2>&1; echo "closure exit $?" >> $out
tail -6 $W/closure-linux.txt >> $out

#!/bin/sh
# Panel 215 re-run (spec-warden's draft): one reader folder, one TSV row.
# usage: judge.sh <heroes> <reader-folder> <kit-folder>
# Builds a2, u and c with the compiler named, runs each binary under a 20 s
# timeout with its output cut at 4096 bytes, and compares the whole output.
# The c row's hints are the by-value signatures carrying `struct counter` in
# the emitted C; a copy by `=` leaves none, so a person reads every c.hero.
H=$1; d=$2; kit=$3
cd "$d" || exit 2
cp "$kit/union.h" "$kit/counter.h" .
res() { # name expected-output
  f=$1
  [ -f "$f.hero" ] || { echo "missing"; return; }
  if ! "$H" build "$f.hero" -o "$f.bin" > "$f.build.txt" 2>&1; then
    echo "build:$(grep -m1 -oE 'error\[[a-z_]+\]' "$f.build.txt" | sed 's/error\[//; s/\]//')"; return
  fi
  ( timeout 20 "./$f.bin"; echo $? > "$f.rc" ) 2>&1 | head -c 4096 > "$f.out"
  rc=$(cat "$f.rc")
  if [ "$rc" = 0 ] && [ "$(cat "$f.out")" = "$(printf "$2")" ]; then echo ok; else echo "run:$rc:$(tr '\n' '/' < "$f.out" | cut -c1-40)"; fi
}
a2=$(res a2 '2'); u=$(res u 'Darwin'); c=$(res c '6\n4\n5')
form=other
grep -qE '^[[:space:]]+record [A-Za-z_0-9]+ .*partial' a2.hero 2>/dev/null && form=partial
grep -qE '^[[:space:]]+(fd|handle):' a2.hero 2>/dev/null && form=flat
grep -qE '^[[:space:]]+via:' a2.hero 2>/dev/null && form=typed
u8=no; grep -qE '(sysname|nodename|release|version|machine): u8\[' u.hero 2>/dev/null && u8=yes
hints=-
if [ -f c.hero ] && "$H" build c.hero --emit-c > c.c 2> c.emit.txt; then
  ret=$(grep -oE '^struct counter h_[A-Za-z0-9_]+\(' c.c | sort -u | sed 's/^struct counter //; s/($//' | tr '\n' ',')
  byval=$(grep -oE '^[A-Za-z_][A-Za-z0-9_ ]* h_[A-Za-z0-9_]+\([^)]*struct counter h[0-9]' c.c | grep -oE 'h_[A-Za-z0-9_]+\(' | sort -u | tr -d '(' | tr '\n' ',')
  hints="ret=${ret:--};byval=${byval:--}"
fi
printf '%s\ta2=%s\tform=%s\tu=%s\tu8=%s\tc=%s\t%s\n' "$(basename "$d")" "$a2" "$form" "$u" "$u8" "$c" "$hints"

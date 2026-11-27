#!/bin/bash
# c1.sh <compiler> <dir with .hero and headers> <program> <runtime> <outdir>: emit <program>.hero, put
# `hero_stack_check(<who>)` after every `hero_thread_guard(<who>)` (route (c1): the address-taken set), build plain
# and --sanitize at -O0 and -O2 by hb.sh.
set -u
C=$1; D=$2; P=$3; RT=$4; O=$5
mkdir -p "$O"
cp "$D"/*.h "$O"/ 2>/dev/null
( cd "$D" && "$C" build "$P.hero" --emit-c -o "$O/$P-emit.c" ) || { echo "emit $P failed"; exit 1; }
sed -E 's/hero_thread_guard\(("[^"]*")\);/hero_thread_guard(\1); hero_stack_check(\1);/' "$O/$P-emit.c" > "$O/$P-c1.c"
echo "$P: $(grep -c 'hero_stack_check(' "$O/$P-c1.c") checked lines"
H=$(cd "$(dirname "$0")" && pwd)
for lv in O0 O2; do
  "$H/hb.sh" "$O/$P-c1.c" "$RT" $lv "$O/$P-$lv" > "$O/build-$P-$lv.txt" 2>&1 || echo "build $P $lv failed"
  "$H/hb.sh" "$O/$P-c1.c" "$RT" $lv "$O/$P-san-$lv" -fsanitize=address,undefined -fno-sanitize=function > "$O/build-$P-san-$lv.txt" 2>&1 || echo "build $P san $lv failed"
done

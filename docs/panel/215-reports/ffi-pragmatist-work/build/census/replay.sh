#!/bin/sh
# Panel 215, the ffi-pragmatist's build of route C: replay, one at a time, the
# clang runs the route added to the corpus's cold --emit-c (group "route"), or
# the layout check's own screen and dump runs in the same directories ("today").
cd "$(dirname "$0")/e-route/build" || exit 2
for d in layout-*; do
  ls $d/locks-said-*.txt >/dev/null 2>&1 || continue
  idr=$(grep -l -- '-ferror-limit=0' $d/*.rsp | head -1)
  dur=$(grep -l -- '-fdump-record-layouts' $d/*.rsp | head -1)
  if [ "$1" = route ]; then
    for u in $d/locks-*.c; do clang @"$idr" "$u" >/dev/null 2>&1; done
    for t in $d/locks-dump-*.txt; do [ -e "$t" ] || continue; g=${t##*/locks-dump-}; clang @"$dur" "$d/dump-${g%.txt}.c" >/dev/null 2>&1; done
  else
    for u in $d/screen-*.c; do clang @"$idr" "$u" >/dev/null 2>&1; done
    for t in $d/clang-dump-*.txt; do [ -e "$t" ] || continue; g=${t##*-}; clang @"$dur" "$d/dump-${g%.txt}.c" >/dev/null 2>&1; done
    for u in $d/check-*.c; do [ -e "$u" ] || continue; clang @"$idr" "$u" >/dev/null 2>&1; done
  fi
done

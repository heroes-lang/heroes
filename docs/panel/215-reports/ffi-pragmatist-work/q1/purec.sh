#!/bin/bash
# ffi-pragmatist, panel 215: pure C under MemorySanitizer, two translation units, every level.
set -u
mkdir -p /tmp/p && cd /tmp/p && cp /w/purec_a.c /w/purec_b.c .
echo "leg $(uname -m), $(clang --version | head -1)"
for O in -O0 -O1 -O2 -O3; do
  clang -g $O -fsanitize=memory -fsanitize-recover=memory purec_a.c purec_b.c -o p$O 2>cc$O.txt || { cat cc$O.txt; continue; }
  MSAN_OPTIONS=halt_on_error=0 ./p$O > /dev/null 2> m$O.txt
  at=$(grep -A4 "use-of-uninitialized-value" m$O.txt | grep -oE "purec_[ab]\.c:[0-9]+" | sort | uniq -c | tr '\n' ' ')
  echo "$O: $(grep -c 'WARNING: MemorySanitizer' m$O.txt) reports; frames: ${at:-none}"
done

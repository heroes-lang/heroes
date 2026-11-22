#!/bin/bash
# Panel 255 critic, second reading: builds each named shape plain and
# --sanitize with compiler $1 against the runtime folder $2 (HEROES_RUNTIME),
# in a work folder $3, runs each binary 3 times under timeout 20, and prints
# one line per distinct outcome: route, shape, mode, count, exit, stdout, the
# first refusal/ASan line, and where the coroutine's stack was.
H=$1; RT=$2; W=$3; shift 3
route=$(basename "$RT")
export HEROES_RUNTIME="$RT"
mkdir -p "$W"; cp "$(dirname "$0")"/*.h "$W"/; for n in "$@"; do cp "$(dirname "$0")/$n.hero" "$W"/; done
cd "$W" || exit 1
for n in "$@"; do
  for mode in plain sanitize; do
    flag=""; [ "$mode" = sanitize ] && flag="--sanitize"
    if ! "$H" build "$n.hero" $flag -o "bin-$n-$mode" > "bin-$n-$mode.build" 2>&1; then
      echo "$route $n $mode BUILD-FAILED $(head -c 300 "bin-$n-$mode.build" | tr '\n' ' ')"; continue
    fi
    for i in 1 2 3; do
      timeout 20 "./bin-$n-$mode" > o.txt 2> e.txt < /dev/null; code=$?
      echo "exit=$code | $(tr '\n' '|' < o.txt | head -c 90) | $(grep -m1 -E 'panic|ERROR: AddressSanitizer|SEGV|refus|had not been seen|no record' e.txt | cut -c1-170) | $(grep -m1 '^where' e.txt)"
    done | sort | uniq -c | sed "s/^ */$route $n $mode /"
  done
done

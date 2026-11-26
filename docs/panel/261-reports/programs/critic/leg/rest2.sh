#!/bin/sh
# The critic, panel 261's second reading: every case the compiler-engineer
# kept, my three shapes and the ffi shapes, under one compiler.
#   sh cases.sh <compiler> <root holding docs/panel/261-reports/programs/compiler-engineer, mine, ffi> <out>
# For each program: `check`, then where it checks `build` and the binary under
# `timeout 60`, output bounded by `head -c`; `--permissive` for the two
# control-arm cases. One file per program in <out>.
set -u
H=$1; R=$2; O=$3
mkdir -p "$O"
one() { # <dir> <file> <tag> [build flags]
    d=$1; f=$2; t=$3; shift 3
    ( cd "$d" || exit 1
      { echo "== check $f"; { "$H" check "$f" 2>&1; echo "check exit $?"; } | head -c 6000
        if "$H" check "$f" >/dev/null 2>&1 || [ "$#" -gt 0 ]; then
          rm -f "$t.bin"
          "$H" build "$f" "$@" -o "$t.bin" > "$t.build.log" 2>&1; b=$?
          echo "build $* exit $b"; [ $b -eq 0 ] || head -c 3000 "$t.build.log"
          if [ -x "$t.bin" ]; then { timeout 60 "./$t.bin" 2>&1; echo "run exit $?"; } | head -c 20000; fi
        fi
      } > "$O/$t.out" 2>&1 )
}
C=$R/docs/panel/261-reports/programs/compiler-engineer
one "$C/router_b" main.hero ce.router_b
echo done > "$O/DONE3"

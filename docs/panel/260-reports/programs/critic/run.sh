#!/bin/bash
# run.sh <label> <compiler-abs> <src-dir> <undeclared|declared|asis> [levels...]
# Every program of <src-dir> (each folder's main.hero, and each top-level *.hero of a `families/` folder), copied into
# a folder of its own variant (one shared build/ per variant), built at each level, run under `timeout 20` (4 KiB
# kept), its layer families read by `nm` (global T), then `check` and `--emit-c` of it. `declared` writes ` layer`
# after the header of every group (as the compiler-engineer's shapes.sh does); `asis` and `undeclared` change nothing.
# A folder holding `lib/now.c` gets `libnow.a` built against the compiler's runtime, and `--library lib --include lib`.
set -u
L=$1; C=$2; R=$3; V=$4; shift 4
levels=${*:-plain}
S=${OUT:-/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/260-critic-scratch/r2}
RT=$(dirname "$C")/runtime
d=$S/t/$L-$V; rm -rf "$d"; mkdir -p "$S/t"; cp -R "$R" "$d"
[ "$V" = declared ] && find "$d" -name '*.hero' -exec perl -0pi -e 's/^(extern "[^"]+")/$1 layer/mg' {} +
for lib in "$d"/*/lib; do [ -f "$lib/now.c" ] && ( cd "$lib" && clang -c -I . -I "$RT" now.c -o now.o && ar rcs libnow.a now.o ); done
out=$S/res-$L-$V.txt
echo "# $L $V $C levels[$levels] $(date '+%H:%M:%S') load $( (sysctl -n vm.loadavg 2>/dev/null || cut -d' ' -f1-3 /proc/loadavg) | tr -d '{}')" > "$out"
fams='acl|activation|audio|crypto|disk|ipc|keyring|locale|net|odbc|poll|printing|proc|procs|proxy|scm|serial|service|stream|sys|syslog|term|time|tls|ui|users|volumes|watch|xattr'
progs=$(cd "$d" && { ls -d */main.hero 2>/dev/null; ls families/*.hero 2>/dev/null; } )
for lv in $levels; do
  case $lv in plain) words="";; *) words=$lv;; esac
  for p in ${ONLY:-$progs}; do
    [ -f "$d/$p" ] || continue
    dir=$(dirname "$p"); base=$(basename "$p")
    n=$(echo "$p" | sed 's|/|-|g; s|\.hero$||')$(echo "$lv" | tr -d ' -')
    extra=""; [ -d "$d/$dir/lib" ] && extra="--library lib --include lib"
    ( cd "$d/$dir" && "$C" build "$base" $words $extra -o "$d/bin-$n" > "$d/$n.out" 2> "$d/$n.err" ); code=$?
    ran=""; fam=""
    if [ $code = 0 ] && [ -x "$d/bin-$n" ]; then
      ran=$(cd "$d/$dir" && timeout 20 "$d/bin-$n" 2>&1 < /dev/null | head -c 4096 | head -4 | tr '\n' '|')
      fam=$(nm "$d/bin-$n" 2>/dev/null | grep -oE " T _?hero_($fams)_" | sed 's/.*hero_//; s/_$//' | sort | uniq -c | awk '{printf "%s %s, ", $2, $1}')
    fi
    echo "$p $lv exit=$code | $(grep -m1 -E 'error|undefined|Undefined' "$d/$n.err" | cut -c1-260) | ran: ${ran:0:140} | members: ${fam:-none}" >> "$out"
  done
done
if [ -z "${NOCHECK:-}" ]; then
for p in ${ONLY:-$progs}; do
  [ -f "$d/$p" ] || continue
  dir=$(dirname "$p"); base=$(basename "$p"); n=$(echo "$p" | sed 's|/|-|g; s|\.hero$||')
  extra=""; [ -d "$d/$dir/lib" ] && extra="--library lib --include lib"
  ( cd "$d/$dir" && "$C" check "$base" > "$d/$n.check.out" 2> "$d/$n.check.err" ); cc=$?
  ( cd "$d/$dir" && "$C" build "$base" $extra --emit-c > "$d/$n.c" 2> "$d/$n.emit.err" ); ec=$?
  echo "$p check exit=$cc | $(cat "$d/$n.check.err" "$d/$n.check.out" | grep -m1 -E 'error' | cut -c1-200) || emit-c exit=$ec | $(grep -m1 -E 'error' "$d/$n.emit.err" | cut -c1-200)" >> "$out"
done
fi
echo "# layer archives under this variant: $(find "$d" -name 'layer-*.a' | wc -l | tr -d ' '); done $(date '+%H:%M:%S')" >> "$out"

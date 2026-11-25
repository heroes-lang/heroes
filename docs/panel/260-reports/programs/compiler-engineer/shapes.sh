#!/bin/bash
# shapes.sh <label> <compiler-abs> <undeclared|declared> [levels...]: every shape of the briefs' `shapes/` and
# `programs/`, built in a copy of its own (one shared build/ per label and variant), at each level given
# (default: plain, -O2, --sanitize). `undeclared` is the shapes as written over the base's Rebel library;
# `declared` adds ` layer` after the header of every group and points the catalogues at this copy's library,
# which land.py declared. Each line: shape level exit, the first error line, what the binary printed first
# (run under `timeout 20`, 4 KiB kept), the layer's families its symbols hold (`nm`), and whether the shared
# build/ holds any layer archive. Then `heroes check` and `--emit-c` of each shape, exit and first error.
S=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/260-compiler-engineer-scratch
B=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/panel260/docs/panel/260-briefs
l=$1; c=$2; v=$3; shift 3
levels=${*:-plain -O2 --sanitize}
d=$S/t/shapes-$l-$v; rm -rf $d; mkdir -p $S/t; cp -R $B/shapes $d; cp $B/programs/*.hero $d/
if [ "$v" = declared ]; then
  lib="../../../../260-compiler-engineer/rebel/library/0.1.0/files"
  find $d -name '*.hero' -exec perl -0pi -e 's/^(extern "[^"]+")/$1 layer/mg' {} +
else
  lib="../../../base-rebel/rebel/library/0.1.0/files"
fi
for t in $d/rebel/heroes.toml $d/face/heroes.toml; do perl -pi -e "s|^path = \".*\"|path = \"$lib\"|" $t; done
out=$S/shapes-$l-$v.txt; echo "# $l $v $c $(date '+%H:%M:%S') load $(sysctl -n vm.loadavg)" > $out
list="print.hero clock.hero audio.hero ui.hero usemod/main.hero ownhdr/main.hero owndecl/main.hero rebel/reach_hero_time.hero rebel/reach_hero_audio.hero rebel/reach_hero_net.hero constonly/main.hero platformhdr/main.hero twofam/main.hero testonly/main.hero face/main.hero $(ls $B/shapes/*.hero | xargs -n1 basename)"
cd $d
for lv in $levels; do
  case $lv in plain) words="";; *) words=$lv;; esac
  for p in ${ONLY:-$list}; do
    [ -f "$p" ] || continue
    n=$(echo "$p" | sed 's|/|-|g; s|\.hero$||')$(echo "$lv" | tr -d ' -')
    ( cd $(dirname $p) && "$c" build $(basename $p) $words -o $d/bin-$n > $d/$n.out 2> $d/$n.err ); code=$?
    ran=""; fam=""
    if [ $code = 0 ] && [ -x $d/bin-$n ]; then
      ran=$(cd $(dirname $p) && timeout 20 $d/bin-$n 2>&1 | head -c 4096 | head -3 | tr '\n' '|')
      fam=$(nm $d/bin-$n 2>/dev/null | grep -oE ' T _?hero_(acl|activation|audio|crypto|disk|ipc|keyring|locale|net|odbc|poll|printing|proc|procs|proxy|scm|serial|service|stream|sys|syslog|term|time|tls|ui|users|volumes|watch|xattr)_' | sed 's/.*hero_//; s/_$//' | sort | uniq -c | awk '{printf "%s %s, ", $2, $1}')
    fi
    archives=$(ls $d/build $d/*/build 2>/dev/null | grep -c '^layer-.*\.a$')
    echo "$p $lv exit=$code | $(grep -m1 -E 'error|undefined|Undefined' $d/$n.err | cut -c1-220) | ran: ${ran:0:120} | members: ${fam:-none} | layer archives in build/: $archives" >> $out
  done
done
for p in ${ONLY:-$list}; do
  [ -f "$p" ] || continue
  n=$(echo "$p" | sed 's|/|-|g; s|\.hero$||')
  ( cd $(dirname $p) && "$c" check $(basename $p) > $d/$n.check.out 2> $d/$n.check.err ); cc=$?
  ( cd $(dirname $p) && "$c" build $(basename $p) --emit-c > $d/$n.c 2> $d/$n.emit.err ); ec=$?
  echo "$p check exit=$cc | $(grep -m1 -E 'error' $d/$n.check.err $d/$n.check.out | cut -c1-200) || emit-c exit=$ec | $(grep -m1 -E 'error' $d/$n.emit.err | cut -c1-200)" >> $out
done
echo "# layer archives anywhere under this variant's folders: $(find $d -name 'layer-*.a' | wc -l | tr -d ' ')" >> $out
echo "# done $(date '+%H:%M:%S') load $(sysctl -n vm.loadavg)" >> $out

#!/bin/bash
# run-linux.sh, inside heroes-linux-arm64 with this seat's `linux/` folder mounted at /w: each compiler built from
# its C with clang -O2 beside its runtime (as the lane's run.sh built them), then every shape under the route,
# undeclared over the base's library and declared over the route's, then clang's calls for print(1) under each
# compiler, then the timing table: print (old, base, route), clock, audio, ui (base as written, route declared),
# -O0, then -O2 and --sanitize; warm and cold, 4 rounds interleaved, round 1 discarded by the reader. In phases
# ($PHASE: build, shapes, time; all three where it is empty), the compilers kept on the mount under /w/c so a later
# container times them; every build folder stands in the container's own /x.
set -u
W=/w; out=$W/results.txt; X=/x; C=$W/c; mkdir -p $X $C
PHASE=${PHASE:-build shapes time}
echo "# $(date '+%Y-%m-%d %H:%M:%S') phases: $PHASE; $(uptime) $(clang --version | head -1) nproc=$(nproc)" >> "$out"
case " $PHASE " in *" build "*)
for c in ${COMPILERS:-old base mine}; do
  rt=runtime; [ "$c" = old ] && rt=runtime-old
  mkdir -p $C/$c && rm -rf $C/$c/runtime && cp -R $W/$rt $C/$c/runtime
  ( cd $C/$c && { time -p clang -O2 -I runtime $W/$c.c runtime/runtime.c -o heroes -lm -lpthread -ldl > build.txt 2>&1 ; } 2> time.txt ); echo "built $c: $? $(tr '\n' ' ' < $C/$c/time.txt) $(tail -2 $C/$c/build.txt | tr '\n' ' ') $($C/$c/heroes --version 2>&1 | tr '\n' ' ')" >> "$out"
done
;; esac
for c in ${COMPILERS:-old base mine}; do mkdir -p $X/$c && rm -rf $X/$c/runtime && cp -R $C/$c/runtime $X/$c/runtime && cp $C/$c/heroes $X/$c/heroes; done

shapes() { # variant level
  local v=$1 lv=$2 d=$X/shapes-$1; local words=""; [ "$lv" != plain ] && words=$lv
  if [ ! -d $d ]; then
    cp -R $W/shapes $d; cp $W/progs/*.hero $d/
    if [ "$v" = declared ]; then lib=$W/lib-declared; find $d -name '*.hero' -exec perl -0pi -e 's/^(extern "[^"]+")/$1 layer/mg' {} +; else lib=$W/lib-undeclared; fi
    for t in $d/rebel/heroes.toml $d/face/heroes.toml; do perl -pi -e "s|^path = \".*\"|path = \"$lib\"|" $t; done
  fi
  cd $d
  for p in print.hero clock.hero audio.hero ui.hero usemod/main.hero ownhdr/main.hero owndecl/main.hero rebel/reach_hero_time.hero rebel/reach_hero_audio.hero rebel/reach_hero_net.hero constonly/main.hero platformhdr/main.hero twofam/main.hero testonly/main.hero face/main.hero $(cd $W/shapes && ls *.hero); do
    [ -f "$p" ] || continue
    n=$(echo "$p" | sed 's|/|-|g; s|\.hero$||')$(echo "$lv" | tr -d ' -')
    ( cd $(dirname $p) && $X/mine/heroes build $(basename $p) $words -o $d/bin-$n > $d/$n.out 2> $d/$n.err ); code=$?
    ran=""; fam=""
    if [ $code = 0 ] && [ -x $d/bin-$n ]; then
      ran=$(cd $(dirname $p) && timeout 20 $d/bin-$n 2>&1 | head -c 4096 | head -3 | tr '\n' '|')
      fam=$(nm $d/bin-$n 2>/dev/null | grep -oE ' T hero_(acl|activation|audio|crypto|disk|ipc|keyring|locale|net|odbc|poll|printing|proc|procs|proxy|scm|serial|service|stream|sys|syslog|term|time|tls|ui|users|volumes|watch|xattr)_' | sed 's/.*hero_//; s/_$//' | sort | uniq -c | awk '{printf "%s %s, ", $2, $1}')
    fi
    echo "linux $v $p $lv exit=$code | $(grep -m1 -E 'error|undefined' $d/$n.err | cut -c1-200) | ran: ${ran:0:100} | members: ${fam:-none} | layer archives: $(find $d -name 'layer-*.a' | wc -l)" >> "$out"
  done
}
case " $PHASE " in *" shapes "*)
for lv in ${SHAPE_LEVELS:-plain -O2 --sanitize}; do shapes undeclared $lv; shapes declared $lv; done
[ -n "${NOCALLS:-}" ] && { echo "# done $(date) (no calls)" >> "$out"; exit 0; }

# clang's calls for a warm print(1) and clock under each compiler: a wrapper on PATH logs each call whole.
mkdir -p $X/wrap; printf '#!/bin/bash\ns=$(date +%%s.%%N); /usr/bin/clang "$@"; c=$?; e=$(date +%%s.%%N); echo "$(echo "$e - $s" | bc) $c $*" >> /x/wrap/log.txt; exit $c\n' > $X/wrap/clang; chmod +x $X/wrap/clang
for spec in old=progs/print base=progs/print mine=progs/print base=progs/clock mine=progs-declared/clock; do
  c=${spec%%=*}; p=${spec#*=}; d=$X/calls-$c-$(basename $p); rm -rf $d; mkdir -p $d; cd $d
  $X/$c/heroes build $W/$p.hero -o bin > /dev/null 2>&1; $X/$c/heroes build $W/$p.hero -o bin > /dev/null 2>&1
  rm -f $X/wrap/log.txt; touch $X/wrap/log.txt
  PATH=$X/wrap:$PATH $X/$c/heroes build $W/$p.hero -o bin > /dev/null 2>&1; code=$?
  echo "calls $c $p exit=$code clang calls $(wc -l < $X/wrap/log.txt), naming platform/ $(grep -c 'platform/' $X/wrap/log.txt), with -M $(grep -cE ' -M( |$)' $X/wrap/log.txt), seconds $(awk '{s+=$1} END {print s}' $X/wrap/log.txt), layer archives $(ls build 2>/dev/null | grep -c '^layer-.*\.a$')" >> "$out"
  cp $X/wrap/log.txt $W/calls-$c-$(basename $p).log
done
;; esac

one() { # name level label compiler program folder mode round words
  cd "$6" || return
  o=$( { time -p "$4" build "$5" ${9:-} -o "$6/bin" > "$6/out.txt" 2> "$6/err.txt"; } 2>&1 ); c=$?
  echo "time $1 $2 $3 $7 $8 $(echo "$o" | awk '/^real/{r=$2} /^user/{u=$2} /^sys/{s=$2} END{print r, u, s}') exit=$c load=$(cut -d' ' -f1 /proc/loadavg)" >> "$out"
}
table() { # name level words specs...
  local pn=$1 lv=$2 words=$3; shift 3
  for spec in "$@"; do l=${spec%%=*}; p=${spec#*=}; f=$X/t/$pn-$lv-$l-warm; rm -rf $f; mkdir -p $f; one $pn $lv $l $X/$l/heroes $W/$p $f prime 0 "$words"; done
  for r in 1 2 3 4; do
    for spec in "$@"; do l=${spec%%=*}; p=${spec#*=}; one $pn $lv $l $X/$l/heroes $W/$p $X/t/$pn-$lv-$l-warm warm $r "$words"; done
    for spec in "$@"; do l=${spec%%=*}; p=${spec#*=}; f=$X/t/$pn-$lv-$l-cold; rm -rf $f; mkdir -p $f; one $pn $lv $l $X/$l/heroes $W/$p $f cold $r "$words"; done
  done
}
case " $PHASE " in *" time "*)
for lv in ${LEVELS:-O0 O2 sanitize}; do
  words=""; [ $lv = O2 ] && words=-O2; [ $lv = sanitize ] && words=--sanitize
  table print $lv "$words" old=progs/print.hero base=progs/print.hero mine=progs/print.hero
  for pn in ${PROGS:-clock audio ui}; do table $pn $lv "$words" base=progs/$pn.hero mine=progs-declared/$pn.hero; done
done
;; esac
echo "# done $(date '+%Y-%m-%d %H:%M:%S') $(uptime)" >> "$out"

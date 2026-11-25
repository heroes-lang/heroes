#!/bin/bash
# critic-linux.sh, inside a Linux image with this folder mounted at /w: after ce-run-linux.sh's build phase has put
# old, base and mine (the compiler-engineer's route) under /w/c, run (PHASE words, all where empty):
#   mine   - the critic's shapes under the base (as written) and the route (as written, and declared: mine-d/)
#   doors  - the ffi-pragmatist's doors and family programs under the route, undeclared and declared; three under
#            the base for the same container's comparison
#   calls  - clang's calls for a warm print(1) under the route, logged whole
#   touch  - an undeclared print(1) under the route, warm, then one member file of a private runtime copy edited
# Build folders live in the container's own /x; result files are copied to /w/out.
set -u
W=/w; X=/x; mkdir -p $X/out $W/out
PHASE=${PHASE:-mine doors calls touch}
for c in old base mine; do [ -x $W/c/$c/heroes ] && { mkdir -p $X/$c; rm -rf $X/$c/runtime; cp -R $W/c/$c/runtime $X/$c/runtime; cp $W/c/$c/heroes $X/$c/heroes; }; done
echo "# critic-linux $(date '+%F %T') phases[$PHASE] $(uname -m) $(cut -d' ' -f1-3 /proc/loadavg) $(clang --version | head -1)" >> $W/out/log.txt
case " $PHASE " in *" mine "*)
  OUT=$X/out bash $W/run.sh base-mine $X/base/heroes $W/mine asis plain
  OUT=$X/out bash $W/run.sh route-mine $X/mine/heroes $W/mine asis plain -O2
  OUT=$X/out bash $W/run.sh route-mined $X/mine/heroes $W/mine-d asis plain -O2
;; esac
case " $PHASE " in *" doors "*)
  OUT=$X/out bash $W/run.sh route-ffi $X/mine/heroes $W/ffi undeclared plain
  OUT=$X/out bash $W/run.sh route-ffi $X/mine/heroes $W/ffi declared plain
  ONLY="linklib/main.hero linklibhdr/main.hero owndeclwrong/main.hero owndeclswift/main.hero" OUT=$X/out bash $W/run.sh base-ffi $X/base/heroes $W/ffi asis plain
;; esac
case " $PHASE " in *" calls "*)
  mkdir -p $X/wrap; printf '#!/bin/bash\ns=$(date +%%s.%%N); /usr/bin/clang "$@"; c=$?; e=$(date +%%s.%%N); echo "$(awk -v s=$s -v e=$e "BEGIN{printf \\"%%.3f\\", e-s}") $c $*" >> /x/wrap/log.txt; exit $c\n' > $X/wrap/clang; chmod +x $X/wrap/clang
  for c in old mine base; do
    d=$X/calls-$c; rm -rf $d; mkdir -p $d; cd $d; cp $W/progs/print.hero .
    $X/$c/heroes build print.hero -o bin > /dev/null 2>&1; $X/$c/heroes build print.hero -o bin > /dev/null 2>&1
    rm -f $X/wrap/log.txt; touch $X/wrap/log.txt
    PATH=$X/wrap:$PATH $X/$c/heroes build print.hero -o bin > /dev/null 2>&1; code=$?
    echo "calls $c print warm exit=$code clang calls $(wc -l < $X/wrap/log.txt), naming platform/ $(grep -c 'platform/' $X/wrap/log.txt), with -M $(grep -cE ' -M( |$)' $X/wrap/log.txt), seconds $(awk '{s+=$1} END {print s}' $X/wrap/log.txt), layer archives $(ls build 2>/dev/null | grep -c '^layer-'), members in bin $(nm bin | grep -cE ' T hero_(net|time|audio|ui|poll|stream|disk|crypto)_')" >> $X/out/calls.txt
    cp $X/wrap/log.txt $X/out/calls-$c-print.log
  done
;; esac
case " $PHASE " in *" touch "*)
  # A private copy of the route's compiler and runtime: print warm, then a comment appended to one member's .c, then
  # print again: does an undeclared program's build compile anything again?
  T=$X/touch; rm -rf $T; mkdir -p $T/p; cp -R $X/mine/runtime $T/runtime; cp $X/mine/heroes $T/heroes; cp $W/progs/print.hero $T/p/
  mkdir -p $X/wrap; [ -x $X/wrap/clang ] || { echo "no wrapper" >> $X/out/touch.txt; }
  cd $T/p; $T/heroes build print.hero -o bin > /dev/null 2>&1; $T/heroes build print.hero -o bin > /dev/null 2>&1
  rm -f $X/wrap/log.txt; touch $X/wrap/log.txt; PATH=$X/wrap:$PATH $T/heroes build print.hero -o bin > /dev/null 2>&1
  echo "touch before: exit=$? calls $(wc -l < $X/wrap/log.txt) $(awk '{s+=$1} END {print s}' $X/wrap/log.txt)s; build/ $(ls build | tr '\n' ' ')" >> $X/out/touch.txt
  echo "/* a comment appended by the critic */" >> $T/runtime/platform/hero_audio.c
  rm -f $X/wrap/log.txt; touch $X/wrap/log.txt; PATH=$X/wrap:$PATH $T/heroes build print.hero -o bin > /dev/null 2>&1
  echo "touch after hero_audio.c edited: exit=$? calls $(wc -l < $X/wrap/log.txt) $(awk '{s+=$1} END {print s}' $X/wrap/log.txt)s; compiling: $(grep -oE '[^ ]*runtime\.c|[^ ]*\.hero\.c|-c [^ ]*' $X/wrap/log.txt | tr '\n' ' ' | cut -c1-300); build/ $(ls build | tr '\n' ' ')" >> $X/out/touch.txt
  cp $X/wrap/log.txt $X/out/touch-after.log
;; esac
cp $X/out/*.txt $X/out/*.log $W/out/ 2>/dev/null
echo "# critic-linux done $(date '+%F %T') $(cut -d' ' -f1-3 /proc/loadavg)" >> $W/out/log.txt

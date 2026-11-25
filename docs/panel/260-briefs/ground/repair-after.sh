#!/bin/bash
# after.sh: the shapes under the base's compiler, the members each probe's binary took, the probes run, the
# t1 reference built and run, all at c3885b81b on this Mac.
S=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/panel260-repair-scratch
H=$S/main-o2/heroes
bash $S/shapes.sh main $H
d=$S/t/shapes-main; out=$S/members-taken.txt
echo "# members taken, nm of each binary, $(date '+%H:%M:%S') load $(sysctl -n vm.loadavg)" > $out
for b in constonly-main platformhdr-main twofam-main testonly-main face-main print clock audio ui usemod-main ownhdr-main owndecl-main the-platform-layer-ends-a-wait-at-its-sockets-deadline; do
  f=$d/bin-$b; [ -f $f ] || { echo "$b: no binary" >> $out; continue; }
  line="$b:"
  for fam in time net disk audio ui crypto stream poll procs stop sys users activation; do
    n=$(nm $f 2>/dev/null | grep -c " T _hero_${fam}_"); [ "$n" -gt 0 ] && line="$line $fam $n"
  done
  echo "$line" >> $out
done
echo "# runs, each under timeout 20, stdout cut at 4 KiB" >> $out
for b in constonly-main platformhdr-main twofam-main testonly-main face-main; do
  ( cd $d && timeout 20 ./bin-$b 2>&1 | head -c 4096 > $d/run-$b.txt ); echo "$b run exit=${PIPESTATUS[0]} $(tr '\n' ' ' < $d/run-$b.txt)" >> $out
done
( cd $d/testonly && $H test clockmod.hero > $d/test-out.txt 2> $d/test-err.txt ); echo "testonly heroes test clockmod.hero exit=$? $(tail -1 $d/test-out.txt) $(head -1 $d/test-err.txt)" >> $out
r=$S/ref; ro=$S/ref-results.txt
echo "# t1 reference at c3885b81b, $($H --version | tail -1), this Mac, $(date '+%H:%M:%S')" > $ro
for j in w1 w2 w3 w4; do
  ( cd $r/$j && $H build main.hero -o bin > build-out.txt 2> build-err.txt ); c=$?
  if [ $c -eq 0 ]; then ( cd $r/$j && timeout 20 ./bin > run-out.txt 2> run-err.txt ); rc=$?; else rc=-; fi
  echo "$j build exit=$c $(head -1 $r/$j/build-err.txt) run exit=$rc prints: $(tr '\n' ' ' < $r/$j/run-out.txt 2>/dev/null)" >> $ro
done
echo AFTER-DONE

#!/bin/bash
# Critic, panel 178 reader test: compile and run one reader's three programs with its variant's compiler(s).
# usage: judge.sh <reader-dir>
set -u
R=/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/07a257f7-6606-4944-b558-0be3c97d5c11/scratchpad/178-completeness-critic
export HEROES_RUNTIME=$R/runtime
d=$1; id=$(basename $d); v=${id%-*}
case $v in I) comp=$R/heroes; comp2=$R/bin/ce-heroes-091 ;; II) comp=$R/bin/ce-heroes-z; comp2= ;; III) comp=$R/bin/ce-heroes-r1; comp2= ;; IV) comp=$R/bin/heroes-al; comp2= ;; esac
cd $d && cp $R/w/readers/app.h . && python3 $R/w/readers/extract.py .
for t in 1 2 3; do
  for c in $comp $comp2; do
    $c check t$t.hero > t$t.$(basename $c).check.txt 2>&1; ck=$?
    ( timeout 20 $c run t$t.hero > t$t.$(basename $c).run.txt 2>&1 ); rn=$?
    first=$(grep -m1 -oE 'error\[[a-z_]+\]|panic: [a-z ]+' t$t.$(basename $c).check.txt t$t.$(basename $c).run.txt | head -1 | sed 's/.*://')
    out=$(grep -vE '^\s*$|warning|^ +\||^ +[0-9]+ \||generated|^\s+\^' t$t.$(basename $c).run.txt | tail -2 | tr '\n' '|' | cut -c1-60)
    echo -e "$id\tt$t\t$(basename $c)\tcheck=$ck\trun=$rn\t${first:-}\t$out"
  done
done
echo -e "$id\tmutex\t$(cat mutex.txt)"

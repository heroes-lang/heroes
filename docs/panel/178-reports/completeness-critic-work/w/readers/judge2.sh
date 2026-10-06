#!/bin/bash
# Critic, panel 178 reader test: one TSV row per reader. Raw = as written; norm = `u8[` read as `i8[` (the char-sign guess
# taken out) and, under II, `record X zero tag Y` read as `record X tag Y zero` (the prototype's placement).
set -u
R=/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/07a257f7-6606-4944-b558-0be3c97d5c11/scratchpad/178-completeness-critic
export HEROES_RUNTIME=$R/runtime
d=$1; id=$(basename $d); v=${id%-*}
case $v in V) comp=$R/bin/ce-heroes-r1 ;; I) comp=$R/heroes ;; II) comp=$R/bin/ce-heroes-z ;; III) comp=$R/bin/ce-heroes-r1 ;; IV) comp=$R/bin/heroes-al ;; esac
cd $d && cp $R/w/readers/app.h . && python3 $R/w/readers/extract.py .
res() { # file expect -> ok / check:<code> / run:<exit>:<code>
  $comp check $1 > $1.ck 2>&1; ck=$?
  if [ $ck -ne 0 ]; then echo "check:$(grep -m1 -oE 'error\[[a-z_]+\]' $1.ck | sed 's/error\[//; s/\]//')"; return; fi
  ( timeout 20 $comp run $1 > $1.run 2>&1 ); rn=$?
  if [ $rn -eq 0 ] && grep -q "$2" $1.run; then echo ok; return; fi
  echo "run:$rn:$(grep -m1 -oE 'error\[[a-z_]+\]|panic: [a-z ]+' $1.run | sed 's/error\[//; s/\]//' | tr ' ' '_')"
}
norm() { sed -E 's/u8\[/i8[/g; s/^( *record [A-Za-z_]+) zero tag ([A-Za-z_]+)/\1 tag \2 zero/' $1 > n$1; }
norm t1.hero; norm t3.hero
t1=$(res t1.hero Darwin); t1n=$(res nt1.hero Darwin); t2=$(res t2.hero connect); t3=$(res t3.hero .); t3n=$(res nt3.hero .)
form=$(grep -qE 'rest: zero' t1.hero && echo rest || (grep -qE '\[0; *256\]' t1.hero && echo repeat || (grep -q partial t1.hero && echo partial-literal || echo literal)))
claim=$(grep -E '^ *record pthread_mutex_t|record [A-Za-z_]+ tag _opaque' t3.hero | grep -qw zero && echo yes || echo no)
noinit=$(grep -q 'pthread_mutex_init(' t3.hero && (grep -E '^\s*[a-z_]+ = pthread_mutex_init|pthread_mutex_init\(@' t3.hero >/dev/null && echo no || echo no) || echo yes)
char=$(grep -qE '(sysname|__opaque|sun_path): u8\[' t1.hero t3.hero t2.hero && echo u8 || echo i8)
echo -e "$id\t$v\t$form\t$char\tt1=$t1\tt1n=$t1n\tt2=$t2\tt3=$t3\tt3n=$t3n\tclaim=$claim\tnoinit=$noinit"

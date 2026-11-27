set -u
# panel 263, compiler-engineer, Linux arm64: the copy at /w/copy, the scratch at /w/scr; everything written under /w/scr/lx
cd /w/scr/lx
uptime
export TMPDIR=/w/scr/lx/tmp; mkdir -p $TMPDIR
start=$(date +%s); clang -I /w/copy/runtime /w/copy/seed/heroes.c /w/copy/runtime/runtime.c -o /w/scr/lx/heroes || exit 3; echo "seed build $(( $(date +%s) - start )) s"
/w/scr/lx/heroes --version
rm -rf proto proto-cc; cp -R /w/scr/proto proto; rm -rf proto/build; cp -R /w/scr/proto-cc proto-cc; rm -rf proto-cc/build
start=$(date +%s)
bash /w/copy/docs/panel/263-briefs/reproducer/run.sh /w/scr/lx/heroes /w/scr/lx/rs 3 20 2>&1 | grep -v "line 30:"
echo "run.sh $(( $(date +%s) - start )) s"
echo "== malloc by CO_N, base (seed)"
for n in 1600 2000 3000 10000 20000 30000 50000; do printf "base malloc-O0 n$n: "; { CO_N=$n timeout -k 5 20 rs/malloc-O0 2>&1; echo "exit $?"; } | head -c 2000 | tr '\n' '|' | cut -c1-140; echo; done
for n in 9600 9800 10000 14000; do printf "base malloc-O2 n$n: "; { CO_N=$n timeout -k 5 20 rs/malloc-O2 2>&1; echo "exit $?"; } | head -c 2000 | tr '\n' '|' | cut -c1-140; echo; done
echo "== route c1 builds and runs"
for p in malloc canary static thread guarded bare; do bash /w/scr/c1.sh /w/scr/lx/heroes /w/scr/lx/proto $p /w/scr/rt-c /w/scr/lx/c1; done
grep -l "error" /w/scr/lx/c1/build-*.txt | head -3
bash /w/scr/runc1.sh /w/scr/lx/c1 3 2>&1 | grep -v "runc1.sh: line"
echo "== counting canaries, base and c1"
mkdir -p cc-base
cd proto-cc; for p in canary4 canary6; do for lv in O0 O2; do /w/scr/lx/heroes build $p.hero -$lv -o ../cc-base/$p-$lv > /dev/null 2>&1 || echo "base $p $lv failed"; /w/scr/lx/heroes build $p.hero -$lv --sanitize -o ../cc-base/$p-san-$lv > /dev/null 2>&1 || echo "base san $p $lv failed"; done; done; cd ..
for p in canary4 canary6; do bash /w/scr/c1.sh /w/scr/lx/heroes /w/scr/lx/proto-cc $p /w/scr/rt-c /w/scr/lx/cc-c1; done
bash /w/scr/count.sh /w/scr/lx/cc-base base 2>&1 | grep -v "count.sh: line"
bash /w/scr/count.sh /w/scr/lx/cc-c1 c1 2>&1 | grep -v "count.sh: line"
uptime
echo "== stage 2"
start=$(date +%s); cd /w/copy && /w/scr/lx/heroes build selfhost/main.hero -o /w/scr/lx/heroes-next > /w/scr/lx/stage2.txt 2>&1; echo "stage 2 exit $? $(( $(date +%s) - start )) s"; cd /w/scr/lx
/w/scr/lx/heroes-next --version
bash /w/copy/docs/panel/263-briefs/reproducer/run.sh /w/scr/lx/heroes-next /w/scr/lx/rs2 1 20 2>&1 | grep -v "line 30:" | head -40
uptime

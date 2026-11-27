set -u
cd /w/scr/lx
uptime
export TMPDIR=/w/scr/lx/tmp; mkdir -p $TMPDIR
start=$(date +%s); clang -I /w/copy/runtime /w/copy/seed/heroes.c /w/copy/runtime/runtime.c -o /w/scr/lx/heroes || exit 3; echo "seed build $(( $(date +%s) - start )) s"
/w/scr/lx/heroes --version
export HEROES_RUNTIME=/w/copy/runtime
start=$(date +%s)
bash /w/copy/docs/panel/263-briefs/reproducer/run.sh /w/scr/lx/heroes /w/scr/lx/rs 1 20
echo "run.sh $(( $(date +%s) - start )) s"
mkdir -p maps && cp /w/copy/docs/panel/263-briefs/reproducer/malloc.hero maps/ && cp /w/copy/docs/panel/263-briefs/reproducer/maps-co.h maps/co.h
/w/scr/lx/heroes build maps/malloc.hero -O2 -o maps/malloc-O2 > maps/build.txt 2>&1 || echo "maps build failed"
for b in rs/malloc-O2 rs/static-O2 maps/malloc-O2; do echo "== $b"; { CO_SAY=1 CO_MAPS=1 CO_N=1 timeout -k 5 20 /w/scr/lx/$b 2>&1; echo "exit $?"; } | head -c 6000 | cut -c1-150; done
echo "== adj.c"; clang -O0 /w/copy/docs/panel/263-briefs/reproducer/adj.c -o adj && ./adj
for n in 1 2 3; do for b in static-O2 static-O0; do printf "%s %s: " $b $n; { timeout -k 5 20 /w/scr/lx/rs/$b 2>&1; echo "exit $?"; } | head -c 4096 | tr '\n' '|' | cut -c1-160; echo; done; done
uptime

set -u
cd /w/scr/lx
export TMPDIR=/w/scr/lx/tmp HEROES_RUNTIME=/w/copy/runtime
H=/w/scr/lx/heroes-next
uptime
echo "== (iii) a 64 KiB malloc stack (the clerk's co.h, CO_STACK changed)"
rm -rf s64; mkdir -p s64; cp /w/scr/proto/malloc.hero s64/; sed 's/#define CO_STACK ((size_t)256 << 10)/#define CO_STACK ((size_t)64 << 10)/' /w/scr/proto/co.h > s64/co.h
grep -c "64 << 10" s64/co.h
for lv in O0 O2; do $H build s64/malloc.hero -$lv -o s64/m-$lv > /dev/null 2>&1 || echo "build $lv failed"; done
for lv in O0 O2; do for n in 100 300 1000 1500 1900 2000 2100 2500 3000 10000 1000000; do for i in 1 2; do printf "64k malloc-$lv n%-8s #$i " $n; { CO_SAY=1 CO_N=$n timeout -k 5 20 s64/m-$lv 2>&1; echo "exit $?"; } | head -c 3000 | grep -v "^/w" | tr '\n' '|' | cut -c1-200; echo; done; done; done
echo "== the counting canary in the heap: CO_KB 32 (a 64 KiB block), base, c1 (c2's set for this program), b"
for d in cc-base cc-c1 bb; do for b in canary4-O2 canary4-O0; do for n in 100 500 1000 2000 10000; do printf "%-7s %s n%-6s " $d $b $n; { CO_KB=32 CO_N=$n timeout -k 5 20 $d/$b 2>&1; echo "exit $?"; } | head -c 3000 | grep -E "^co:|^panic|^depth|^exit|ERROR" | sed -E 's/ \(a stack check[^)]*\)//; s/of the 32768 below the stack were written, the lowest/below of 32768, lowest/' | tr '\n' '|' | cut -c1-170; echo; done; done; done
echo "== shape D"
rm -rf dd; mkdir -p dd; cp -R /w/scr/proto-d pd; rm -rf pd/build
(cd pd && $H build dshape.hero --emit-c -o ../dd/dshape-emit.c > /dev/null 2>&1) || echo "emit dshape failed"
python3 /w/scr/addcheck.py dd/dshape-emit.c dd/dshape-c2.c dshape.enter
cp pd/co.h dd/
for lv in O0 O2; do (cd pd && $H build dshape.hero -$lv -o ../dd/base-$lv > /dev/null 2>&1) || echo "base $lv fail"; bash /w/scr/hb.sh dd/dshape-c2.c /w/scr/rt-c $lv dd/c2-$lv > /dev/null 2>&1 || echo "c2 $lv fail"; done
bash /w/scr/b.sh $H /w/scr/lx/pd dshape /w/scr/rt-b /w/scr/lx/dd/bsrc > /dev/null; for lv in O0 O2; do cp dd/bsrc/dshape-$lv dd/b-$lv; done
for v in base c2 b; do for lv in O0 O2; do for n in 1000 2500 3000 4000 6000 100000; do printf "%-5s %s n%-7s " $v $lv $n; { CO_N=$n timeout -k 5 20 dd/$v-$lv 2>&1; echo "exit $?"; } | head -c 4000 | grep -E "^co:|^panic|^depth|^exit|ran on|ERROR" | sed -E 's/of the 262144 below the stack were written, the lowest/below, lowest/' | tr '\n' '|' | cut -c1-160; echo; done; done; done
echo "== costs, by the program's own clock (CLOCK_MONOTONIC), base / c2 / b each built by hb.sh from the stage 2 emission"
rm -rf cb; mkdir -p cb; cp -R /w/scr/cost cs; rm -rf cs/build
for p in cost_cb cost_qsort cost_calls cost_rec; do
  (cd cs && $H build $p.hero --emit-c -o ../cb/$p-emit.c > /dev/null 2>&1) || echo "emit $p failed"
  set=$(grep "^$p " /w/scr/costbin/c2-sets.txt | cut -d' ' -f2-)
  python3 /w/scr/addcheck.py cb/$p-emit.c cb/$p-c2.c $set
  cp cs/cost.h cb/
  for lv in O2 O0; do bash /w/scr/hb.sh cb/$p-emit.c /w/copy/runtime $lv cb/$p-base-$lv > /dev/null 2>&1 || echo "base $p $lv"; bash /w/scr/hb.sh cb/$p-c2.c /w/scr/rt-c $lv cb/$p-c2-$lv > /dev/null 2>&1 || echo "c2 $p $lv"; done
  bash /w/scr/b.sh $H /w/scr/lx/cs $p /w/scr/rt-b /w/scr/lx/cb/bsrc > /dev/null; for lv in O2 O0; do cp cb/bsrc/$p-$lv cb/$p-b-$lv; done
done
uptime
for p in cost_cb cost_qsort cost_calls cost_rec; do for lv in O2 O0; do for i in 1 2 3 4 5 6; do for side in base c2 b; do
  ns=$( { cb/$p-$side-$lv > cb/ans.txt; } 2>&1 | awk '/^ns / {print $2}'); echo "costrun $p $lv $side $i $ns $(cat cb/ans.txt)"; done; done; done; done
uptime
echo "== the check's code in Linux arm64, c2 cost_rec -O2 down"
(llvm-objdump -d --no-show-raw-insn cb/cost_rec-c2-O2 2>/dev/null || objdump -d cb/cost_rec-c2-O2) | awk '/<h_costrec_down>:/{on=1} on{print; n++} n>24{exit}'
(llvm-objdump -d --no-show-raw-insn cb/cost_rec-base-O2 2>/dev/null || objdump -d cb/cost_rec-base-O2) | awk '/<h_costrec_down>:/{on=1} on{print; n++} n>14{exit}'

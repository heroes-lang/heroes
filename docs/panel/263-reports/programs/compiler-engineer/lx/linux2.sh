set -u
cd /w/scr/lx
export TMPDIR=/w/scr/lx/tmp HEROES_RUNTIME=/w/copy/runtime
H=/w/scr/lx/heroes-next
uptime; $H --version
rm -rf proto proto-cc proto-helper; for d in proto proto-cc proto-helper; do cp -R /w/scr/$d $d; rm -rf $d/build; done
echo "== run.sh on Linux stage 2"
bash /w/copy/docs/panel/263-briefs/reproducer/run.sh $H /w/scr/lx/rs2 3 20 2>&1 | grep -v "line 30:"
echo "== malloc by CO_N, base (stage 2)"
for n in 1600 2000 3000 10000 20000 30000 50000; do printf "base malloc-O0 n$n: "; { CO_N=$n timeout -k 5 20 rs2/malloc-O0 2>&1; echo "exit $?"; } | head -c 2000 | tr '\n' '|' | cut -c1-140; echo; done
for n in 9600 9800 10000 14000; do printf "base malloc-O2 n$n: "; { CO_N=$n timeout -k 5 20 rs2/malloc-O2 2>&1; echo "exit $?"; } | head -c 2000 | tr '\n' '|' | cut -c1-140; echo; done
echo "== route c1 builds and runs"
for p in malloc canary static thread guarded bare; do bash /w/scr/c1.sh $H /w/scr/lx/proto $p /w/scr/rt-c /w/scr/lx/c1; done
bash /w/scr/runc1.sh /w/scr/lx/c1 3 2>&1 | grep -v "runc1.sh: line"
echo "== counting canaries, base and c1"
mkdir -p cc-base
(cd proto-cc; for p in canary4 canary6; do for lv in O0 O2; do $H build $p.hero -$lv -o ../cc-base/$p-$lv > /dev/null 2>&1 || echo "base $p $lv failed"; $H build $p.hero -$lv --sanitize -o ../cc-base/$p-san-$lv > /dev/null 2>&1 || echo "base san $p $lv failed"; done; done)
for p in canary4 canary6; do bash /w/scr/c1.sh $H /w/scr/lx/proto-cc $p /w/scr/rt-c /w/scr/lx/cc-c1; done
bash /w/scr/count.sh /w/scr/lx/cc-base base 2>&1 | grep -v "count.sh: line" > cc-base.txt
bash /w/scr/count.sh /w/scr/lx/cc-c1 c1 2>&1 | grep -v "count.sh: line" > cc-c1.txt
paste -d'\n' cc-base.txt cc-c1.txt | awk -f /w/scr/join.awk
echo "== helper: base, c1, c2, c3"
bash /w/scr/c1.sh $H /w/scr/lx/proto-helper helper /w/scr/rt-c /w/scr/lx/hp
sed -E 's/^(int64_t h_helper_walk\(int64_t h0_n\) \{)/\1 hero_stack_check("helper.walk");/' hp/helper-c1.c > hp/helper-c2.c
sed -E 's/^(int64_t h_helper_setup\(void\) \{)/\1 hero_stack_check("helper.setup");/; s/^(void h_helper_main\(void\) \{)/\1 hero_stack_check("helper.main");/' hp/helper-c2.c > hp/helper-c3.c
for v in c2 c3; do for lv in O0 O2; do bash /w/scr/hb.sh hp/helper-$v.c /w/scr/rt-c $lv hp/helper-$v-$lv > /dev/null 2>&1 || echo "$v $lv failed"; bash /w/scr/hb.sh hp/helper-$v.c /w/scr/rt-c $lv hp/helper-$v-san-$lv -fsanitize=address,undefined -fno-sanitize=function > /dev/null 2>&1 || echo "$v san $lv failed"; done; done
(cd proto-helper && for lv in O0 O2; do $H build helper.hero -$lv -o ../hp/helper-base-$lv >/dev/null 2>&1; $H build helper.hero -$lv --sanitize -o ../hp/helper-base-san-$lv >/dev/null 2>&1; done)
for lv in O0 O2; do for b in helper-base-$lv helper-base-san-$lv helper-$lv helper-san-$lv helper-c2-$lv helper-c2-san-$lv helper-c3-$lv; do for n in 1000 10000 30000; do printf "%-20s n%-6s " $b $n; { CO_N=$n timeout -k 5 20 hp/$b 2>&1; echo "exit $?"; } | head -c 6000 | grep -E "^co:|^panic|^depth|^exit|ran on|ERROR" | sed -E 's/ \(a stack check[^)]*\)//; s/of the 262144 below the stack were written, the lowest/below, lowest/; s/==[0-9]+==ERROR: AddressSanitizer: stack-overflow.*/asan stack-overflow/' | tr '\n' '|' | cut -c1-150; echo; done; done; done
echo "== route b"
for p in canary4 canary6; do bash /w/scr/b.sh $H /w/scr/lx/proto-cc $p /w/scr/rt-b /w/scr/lx/bb; done
bash /w/scr/b.sh $H /w/scr/lx/proto-helper helper /w/scr/rt-b /w/scr/lx/bb
for b in canary4-O2 canary4-O0 canary6-O2 canary6-O0 canary4-san-O2 canary4-san-O0 helper-O2 helper-O0; do for n in 1000 9000 100000 300000 10000000; do printf "%-16s n%-8s " $b $n; { CO_N=$n timeout -k 5 30 bb/$b 2>&1; echo "exit $?"; } | head -c 6000 | grep -E "^co:|^panic|^depth|^exit|ran on|ERROR" | sed -E 's/of the 262144 below the stack were written, the lowest/below, lowest/; s/==[0-9]+==ERROR: AddressSanitizer: stack-overflow.*/asan stack-overflow/' | tr '\n' '|' | cut -c1-170; echo; done; done
mkdir -p lv; (cd proto-cc && for lv in O0 O2; do $H build canary4.hero -$lv -o ../lv/base-$lv >/dev/null 2>&1; $H build canary4.hero -$lv --sanitize -o ../lv/base-san-$lv >/dev/null 2>&1; done)
for b in lv/base-O0 lv/base-O2 bb/canary4-O0 bb/canary4-O2 lv/base-san-O2 bb/canary4-san-O2; do first0=""; for leave in 64 128 192 256 320 384 448 512 640 768 896 1024 1280 1536 2048 3072 4096; do w=$(CO_N=0 CO_LEAVE=$leave timeout -k 5 10 $b 2>&1 | grep -oE "[0-9]+ bytes of" | head -1 | cut -d' ' -f1); if [ "$w" = "0" ] && [ -z "$first0" ]; then first0=$leave; fi; done; echo "== $b: the smallest room with 0 bytes below: $first0"; done
echo "== forced unwind across the switch"
cp /w/scr/fu/fu.c fu.c
for co in 0 1; do for s in 0 1; do for c in 0 1; do for t in 0 1; do [ $s = 0 ] && [ $c = 1 ] && continue; clang -O0 -g -fexceptions -DCO=$co -DSWITCH=$s -DCFI=$c -DSTOP=$t fu.c -o fu-$co$s$c$t 2>&1 | grep error | head -3; printf "linux: "; timeout -k 5 10 ./fu-$co$s$c$t; echo "  exit $?"; done; done; done; done
uptime
echo "== 199j's cases and the switch family, Linux stage 2"
cd /w/copy
for set in fixedbugs-2871 fixedbugs-27 fixedbugs-28 fixedbugs-26; do echo "-- $set"; start=$(date +%s); timeout -k 5 3000 $H run tests/harness/main.hero -- $H run $set > /w/scr/lx/harness-$set.txt 2>&1; echo "exit $? $(( $(date +%s) - start )) s"; grep -E "^FAIL|passed|failed" /w/scr/lx/harness-$set.txt | head -20; done
uptime

set -u
# panel 263, compiler-engineer: the Linux costs again on a quiet machine (the binaries linux3.sh built), then linux4.sh
cd /w/scr/lx
echo "== vm.max_map_count $(cat /proc/sys/vm/max_map_count)"; echo "== quiet re-run"; uptime; cat /proc/loadavg
for p in cost_cb cost_qsort cost_calls cost_rec; do for lv in O2 O0; do for i in 1 2 3 4 5 6; do for side in base c2 b; do
  ns=$( { cb/$p-$side-$lv > cb/ans.txt; } 2>&1 | awk '/^ns / {print $2}'); echo "costrun $p $lv $side $i $ns $(cat cb/ans.txt)"; done; done; done; done
uptime; cat /proc/loadavg
bash /w/scr/lx/linux4.sh

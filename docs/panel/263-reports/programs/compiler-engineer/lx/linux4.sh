set -u
# panel 263, compiler-engineer: fixedbugs-27 and fixedbugs-28 on the Linux stage 2, one harness process a case, four
# at a time; every red re-run alone before it is read (.claude/rules/verification.md, a shared cache's false red).
export TMPDIR=/w/scr/lx/tmp HEROES_RUNTIME=/w/copy/runtime
H=/w/scr/lx/heroes-next
cd /w/copy
uptime
mkdir -p /w/scr/lx/h4
ls tests/golden/run | grep -E "\.hero$" | grep -E -- "fixedbugs-2[78]" | sed 's/\.hero$//' > /w/scr/lx/h4/cases.txt
wc -l < /w/scr/lx/h4/cases.txt
$H build tests/harness/main.hero -o /w/scr/lx/h4/harness > /w/scr/lx/h4/harness-build.txt 2>&1 || echo "harness build failed"
start=$(date +%s)
xargs -P 4 -I{} sh -c 'timeout -k 5 1200 /w/scr/lx/h4/harness '"$H"' run {} > /w/scr/lx/h4/{}.txt 2>&1; echo "{} $?"' < /w/scr/lx/h4/cases.txt > /w/scr/lx/h4/exits.txt
echo "parallel pass $(( $(date +%s) - start )) s"
grep -v " 0$" /w/scr/lx/h4/exits.txt | while read -r c e; do
  timeout -k 5 1200 /w/scr/lx/h4/harness $H run $c > /w/scr/lx/h4/$c.alone.txt 2>&1; echo "alone $c $e -> $?"
  grep -E "^FAIL|failed" /w/scr/lx/h4/$c.alone.txt | head -4
done
grep -h "passed" /w/scr/lx/h4/fixedbugs-*.txt | grep -v alone | awk '{p += $(NF-3); f += $(NF-1)} END {print "sum: " p " passed, " f " failed (the cases files)"}'
uptime

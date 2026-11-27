#!/bin/bash
# depth.sh <binary> [CO_KB]: the deepest alternation n that ends at exit 0 with 0 bytes below C's stack, by bisection
# between 1 and 20,000 (a run past it is told, hangs or writes below; each probe one run, timeout -k 5 20).
b=$1; kb=${2:-256}
ok() { out=$(CO_KB=$kb CO_N=$1 timeout -k 5 20 "$b" 2>&1 | head -c 4000); rc=$?
  echo "$out" | grep -q "^depth reached" || return 1
  echo "$out" | grep -qE "co: [a-z ]+: 0 bytes" || return 1; return 0; }
lo=1; hi=20000
while [ $((hi - lo)) -gt 1 ]; do mid=$(( (lo + hi) / 2 )); if ok $mid; then lo=$mid; else hi=$mid; fi; done
echo "$(basename $b) CO_KB=$kb: deepest right with 0 bytes below n=$lo ($(( kb * 1024 / lo )) bytes of C's stack a level)"

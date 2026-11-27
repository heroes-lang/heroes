#!/bin/bash
# leave.sh <binary> <CO_N>: the smallest CO_LEAVE (room left on C's 256 KiB stack when C calls the callback) with which
# the run ends at exit 0 with the answer and 0 bytes below; by bisection between 16 and 65536.
b=$1; n=$2
ok() { out=$(CO_LEAVE=$1 CO_N=$n timeout -k 5 20 "$b" 2>&1 | head -c 4000); echo "$out" | grep -q "^depth reached" || return 1
  echo "$out" | grep -qE "co: [a-z ]+: 0 bytes" || return 1; return 0; }
lo=16; hi=65536
ok $hi || { echo "$(basename $b) CO_N=$n: not right even at $hi"; exit 0; }
while [ $((hi - lo)) -gt 8 ]; do mid=$(( (lo + hi) / 2 )); if ok $mid; then hi=$mid; else lo=$mid; fi; done
echo "$(basename $b) CO_N=$n: smallest room right with 0 bytes below about $hi (wrong or refused at $lo): $(CO_LEAVE=$lo CO_N=$n timeout -k 5 20 $b 2>&1 | grep -oE 'co: [a-z ]+: [0-9]+ bytes|panic: [a-z ]+: C called back [a-z.]+ with [0-9]+ bytes' | head -1)"

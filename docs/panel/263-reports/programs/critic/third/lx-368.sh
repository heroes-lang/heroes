#!/bin/bash
# lx-368.sh: inside c263c, who writes the 368 bytes below C's stack when (b'')'s reserve refuses the alternation at
# --sanitize -O0 (CO_N=1000): the run, then lldb with ASLR off, a write watchpoint at the lowest byte the count names.
set -u
export TMPDIR=/s/tmp-lx; mkdir -p /s/ox/w368; cd /s/ox/w368
B=/s/bin; b=/s/ox/alt/alt-bpp-sO0
$B/r1.sh $b CO_N=1000
out=$(CO_SAY=1 CO_N=1000 lldb --batch -o run $b 2>&1 | head -c 20000)
echo "$out" | grep -E "^co: kind|co: at the stop|panic:" | head -4
stack=$(echo "$out" | sed -nE 's/^co: kind [0-9]+ stack \[(0x[0-9a-f]+), .*/\1/p' | head -1)
low=$(echo "$out" | sed -nE 's/.*the lowest ([0-9]+) below it.*/\1/p' | head -1)
[ -z "$stack" ] || [ -z "$low" ] && { echo "no address read"; exit 0; }
addr=$(printf '0x%x' $(( stack - low )))
echo "stack $stack, lowest written $low below it: watching $addr"
CO_N=1000 timeout -k 5 300 lldb --batch -o "br set -n makecontext" -o run -o "watchpoint set expression -w write -s 8 -- $addr" -o "br delete 1" -o continue -o "bt 25" -o "register read sp pc" -o continue -o "bt 25" $b 2>&1 | grep -vE "^\s*$" | head -150

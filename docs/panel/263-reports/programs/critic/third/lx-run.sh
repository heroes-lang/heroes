#!/bin/bash
# lx-run.sh: inside c263c, the shapes with the base and (b'') compilers built in the image (/s/lx/<side>), into /s/ox.
set -u
export TMPDIR=/s/tmp-lx; mkdir -p $TMPDIR /s/ox
B=/s/bin
for side in base bpp; do
  C=/s/lx/$side/heroes-next; R=/s/lx/$side/runtime
  for spec in "csh/csh:O0 O2 sO2 sO0" "alt/alt:O0 O2 sO2 sO0" "dshape/dshape:O0 O2" "canary/canary4:O0 O2" "canary/canary6:O2" "helper/helper:O2" "td/td:O0 O2 sO2 sO0"; do
    prog=${spec%%:*}; tags=${spec#*:}; d=${prog%/*}; p=${prog#*/}; mkdir -p /s/ox/$d
    $B/b4.sh $C $R /s/p/$d $p /s/ox/$d/$p-$side $tags
  done
done
echo LX-PROGRAMS-BUILT
for side in base bpp; do for lv in O0 O2 sO2 sO0; do for i in 1 2 3; do $B/r1.sh /s/ox/csh/csh-$side-$lv; done; done; done
for side in base bpp; do for lv in O2 O0 sO2 sO0; do $B/depth.sh /s/ox/alt/alt-$side-$lv 256; done; done
for side in base bpp; do for lv in O2 O0; do for n in 0 1; do $B/leave.sh /s/ox/alt/alt-$side-$lv $n; done; done; done
for side in base bpp; do for n in 1000 2000 3000 100000; do $B/r1.sh /s/ox/alt/alt-$side-sO0 CO_N=$n; done; done
for side in base bpp; do for n in 2500 4000 6000; do $B/r1.sh /s/ox/dshape/dshape-$side-O0 CO_N=$n; done; for i in 1 2 3; do $B/r1.sh /s/ox/dshape/dshape-$side-O2 CO_N=100000; done; done
for side in base bpp; do for n in 9000 10000 100000; do $B/r1.sh /s/ox/canary/canary4-$side-O2 CO_N=$n; $B/r1.sh /s/ox/canary/canary6-$side-O2 CO_N=$n; done
  for n in 1500 2000; do $B/r1.sh /s/ox/canary/canary4-$side-O0 CO_N=$n; done; for n in 10000 30000; do $B/r1.sh /s/ox/helper/helper-$side-O2 CO_N=$n; done
  for lv in O2 O0 sO2 sO0; do $B/r1.sh /s/ox/td/td-$side-$lv; done; done
mkdir -p /s/ox/proto && cp /s/o/proto/alt-p1.c /s/o/proto/alt-p1c.c /s/o/proto/alt-bpp.c /s/p/alt/co.h /s/ox/proto/
cd /s/ox/proto && for v in bpp p1 p1c; do $B/hb2.sh alt-$v.c /s/rt-p O2 alt-$v-O2 > alt-$v-O2.build.txt 2>&1 || echo "hb2 $v failed"; $B/depth.sh /s/ox/proto/alt-$v-O2 256; for n in 0 1; do $B/leave.sh /s/ox/proto/alt-$v-O2 $n; done; done
cd /s
left=$(pgrep -f "/s/ox/" || true); [ -n "$left" ] && echo "STILL RUNNING: $left"
echo LX-RUN-DONE

#!/bin/bash
# Panel 178 compiler-engineer: one Linux leg. Builds the seed, then the prototype, then runs the probes.
set -u
mkdir -p /tmp/b && cd /tmp/b && cp -r /src/seed /src/runtime /src/selfhost . && mkdir -p w && cp -r /src/w/* w/ && cp /src/docs/panel/178-briefs/sunpath_linux_ascii.hero /src/docs/panel/178-briefs/elem_min.* w/
export HEROES_RUNTIME=/tmp/b/runtime
uname -m
time clang -O2 -I runtime seed/heroes.c runtime/runtime.c -o heroes-seed 2>&1 | tail -3
time ./heroes-seed build selfhost/main.hero -o heroes-r1 2>&1 | grep -v 'warning\|^ \|generated' | tail -4
cd w
for f in elem_min.hero sunpath_linux_ascii.hero uname_r1_linux.hero sunpath_r1_linux.hero kinds.hero shapes091.hero; do
  ../heroes-seed run $f > o.txt 2> e.txt; s=$?
  ../heroes-r1 run $f > o2.txt 2> e2.txt; r=$?
  echo "$f  seed run=$s [$(tr '\n' '|' < o.txt)] $(grep -E 'panic|error\[' e.txt | head -1)  ||  prototype run=$r [$(tr '\n' '|' < o2.txt)] $(grep -E 'panic|error\[' e2.txt | head -1)"
done
../heroes-r1 build uname_r1_linux.hero --emit-c 2>/dev/null | grep 'struct utsname){'
../heroes-r1 build sunpath_r1_linux.hero --emit-c 2>/dev/null | grep 'struct sockaddr_un){'
cd ref && for f in own fn case order notlast restfield; do ../../heroes-r1 check $f.hero > /dev/null 2>&1; echo "refusal $f check=$?"; done

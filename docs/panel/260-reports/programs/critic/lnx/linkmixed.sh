#!/bin/bash
# linkmixed.sh, in the arm64 image: door 3 beside a declared group, under the route (c-arm64/mine), plain; then the
# response file's order and the linker's words.
X=/x; mkdir -p $X/mine $X/out; cp -R /w/c-arm64/mine/runtime $X/mine/runtime; cp /w/c-arm64/mine/heroes $X/mine/heroes
OUT=$X/out bash /w/run.sh route-extra2 $X/mine/heroes /w/extra2 asis plain
d=$X/out/t/route-extra2-asis/linkmixed
{ echo "## build.err"; cat $X/out/t/route-extra2-asis/linkmixed-mainplain.err | head -30; echo "## link words"; cat $d/build/*/link-words.rsp 2>/dev/null | tr ' ' '\n' | grep -E 'layer-|\.o"?$|-l|group' | tr '\n' ' '; echo; } > $X/out/linkmixed-detail.txt
mkdir -p /w/out-linkmixed; cp $X/out/res-*.txt $X/out/linkmixed-detail.txt /w/out-linkmixed/

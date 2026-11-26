#!/bin/sh
# Panel 261, the ffi-pragmatist: the leg's later steps, run by run_leg.sh from
# /work with <out dir> <stage 2>. sqlite3's rtree geometry callback, and the
# shapes beside the bindings (`shapes/`, when present).
OUT=$1; H=$2
words_of() { grep -lx '"-Werror"' build/words/*.rsp | xargs grep -l "$1" 2>/dev/null | head -1; }
plain_words() { for f in $(grep -lx '"-Werror"' build/words/*.rsp); do grep -q fsanitize "$f" || { echo "$f"; return; }; done; }
link_of() { for f in build/*/link-words.rsp; do if [ "$1" = san ]; then grep -q fsanitize "$f" && { echo "$f"; return; }; else grep -q fsanitize "$f" || { echo "$f"; return; }; fi; done; }
compile_and_run() {
    sh /work/build_unit.sh "$2" "$1.bin" "$3" "$4" > "$OUT/$5.build.out" 2>&1
    echo "build exit $?" >> "$OUT/$5.build.out"
    if [ -x "$1.bin" ]; then timeout 60 "./$1.bin" > "$OUT/$5.run.out" 2>&1; echo "run exit $?" >> "$OUT/$5.run.out"; fi
}
mkdir -p /work/sqlite && cd /work/sqlite
cp /prog/sqlite/geom_body.hero /prog/sqlite/geom_reads.hero /prog/sqlite/make_geom.py .
$H check geom_reads.hero > "$OUT/geom.reads.check.out" 2>&1; echo "exit $?" >> "$OUT/geom.reads.check.out"
$H build geom_body.hero -o body.bin > "$OUT/geom.body.build.out" 2>&1; echo "exit $?" >> "$OUT/geom.body.build.out"
./body.bin > "$OUT/geom.body.run.out" 2>&1; echo "run exit $?" >> "$OUT/geom.body.run.out"
cp build/tu-*/geombody.c geom_body.unit.c
python3 -I make_geom.py geom_body.unit.c geom_proposal.c > "$OUT/geom.variants.out" 2>&1
python3 -I make_geom.py geom_body.unit.c geom_proposal_null.c null >> "$OUT/geom.variants.out" 2>&1
W=$(plain_words); L=$(link_of plain)
compile_and_run geom geom_proposal.c "$W" "$L" geom
compile_and_run geom_null geom_proposal_null.c "$W" "$L" geom.null
$H build geom_body.hero --sanitize -o body.san.bin > "$OUT/geom.body.san.build.out" 2>&1; echo "exit $?" >> "$OUT/geom.body.san.build.out"
WS=$(words_of fsanitize); LS=$(link_of san)
compile_and_run geom_san geom_proposal.c "$WS" "$LS" geom.san
if [ -f /prog/shapes/leg_shapes.sh ]; then mkdir -p /work/shapes && cd /work/shapes && sh /prog/shapes/leg_shapes.sh "$OUT" "$H"; fi

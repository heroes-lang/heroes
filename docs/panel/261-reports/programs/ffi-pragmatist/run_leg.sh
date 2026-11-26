#!/bin/sh
# Panel 261, the ffi-pragmatist: one Linux leg, run inside a container of
# `heroes-linux-arm64:latest` or `heroes-linux:latest`:
#
#   docker run --rm --pull never --name p261-ffi \
#     -v <copy>/build-linux/base.tar:/src/base.tar:ro \
#     -v <copy>/docs/panel/261-reports/programs/ffi-pragmatist:/prog \
#     <image> sh /prog/run_leg.sh <label>
#
# `base.tar` is `git archive df92f9281 selfhost runtime seed vendor spec`.
# Builds the seed's compiler and stage 2 there, then each binding's programs
# with stage 2, the proposal's C by the same edit scripts as this Mac's, every
# unit compiled with the words that leg's own build used (`build_unit.sh`).
# Writes everything under /prog/linux-<label>/. Works in /work, inside the
# container, which `--rm` removes.
set -u
A=$1
OUT=/prog/linux-$A
rm -rf "$OUT"
mkdir -p "$OUT" /work
cd /work && tar xf /src/base.tar
{ uname -m; clang --version | head -1; } > "$OUT/machine.txt"
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes > "$OUT/seed.log" 2>&1
echo "seed exit $?" >> "$OUT/seed.log"
./heroes --version >> "$OUT/seed.log" 2>&1
./heroes build selfhost/main.hero -o heroes-next > "$OUT/stage2.log" 2>&1
echo "stage2 exit $?" >> "$OUT/stage2.log"
./heroes-next --version >> "$OUT/stage2.log" 2>&1
H=/work/heroes-next
cp /prog/build_unit.sh /work/build_unit.sh

# The words of the newest unit compile and link of a build (`-Werror` marks a
# unit's; `fsanitize` the sanitized build's), read after each build.
words_of() { grep -lx '"-Werror"' build/words/*.rsp | xargs grep -l "$1" 2>/dev/null | head -1; }
plain_words() { for f in $(grep -lx '"-Werror"' build/words/*.rsp); do grep -q fsanitize "$f" || { echo "$f"; return; }; done; }
link_of() { for f in build/*/link-words.rsp; do if [ "$1" = san ]; then grep -q fsanitize "$f" && { echo "$f"; return; }; else grep -q fsanitize "$f" || { echo "$f"; return; }; fi; done; }

compile_and_run() { # <name> <c> <words> <link> <label>
    sh /work/build_unit.sh "$2" "$1.bin" "$3" "$4" > "$OUT/$5.build.out" 2>&1
    echo "build exit $?" >> "$OUT/$5.build.out"
    if [ -x "$1.bin" ]; then
        timeout 60 "./$1.bin" > "$OUT/$5.run.out" 2>&1
        echo "run exit $?" >> "$OUT/$5.run.out"
    fi
}

# nftw
mkdir -p /work/nftw && cd /work/nftw
cp /prog/nftw/visit_body.hero /prog/nftw/visit_reads.hero /prog/nftw/nftw_today.hero /prog/nftw/nftw_unit.h /prog/nftw/make_variants.py .
cp -r /prog/nftw/tree .
$H check visit_reads.hero > "$OUT/nftw.visit_reads.check.out" 2>&1; echo "exit $?" >> "$OUT/nftw.visit_reads.check.out"
$H build nftw_today.hero -o today.bin > "$OUT/nftw.today.build.out" 2>&1; echo "exit $?" >> "$OUT/nftw.today.build.out"
$H build visit_body.hero -o body.bin > "$OUT/nftw.body.build.out" 2>&1; echo "exit $?" >> "$OUT/nftw.body.build.out"
./body.bin > "$OUT/nftw.body.run.out" 2>&1; echo "run exit $?" >> "$OUT/nftw.body.run.out"
cp build/tu-*/visitbody.c visit_body.unit.c
python3 -I make_variants.py > "$OUT/nftw.variants.out" 2>&1
W=$(plain_words); L=$(link_of plain)
echo "words $W link $L" >> "$OUT/nftw.variants.out"
for v in B A C D E; do compile_and_run "nftw_$v" "nftw_proposal_$v.c" "$W" "$L" "nftw.$v"; done
$H build visit_body.hero --sanitize -o body.san.bin > "$OUT/nftw.body.san.build.out" 2>&1; echo "exit $?" >> "$OUT/nftw.body.san.build.out"
WS=$(words_of fsanitize); LS=$(link_of san)
echo "san words $WS link $LS" >> "$OUT/nftw.variants.out"
for v in B E; do compile_and_run "nftw_${v}_san" "nftw_proposal_$v.c" "$WS" "$LS" "nftw.$v.san"; done
cp nftw_proposal_B.c "$OUT/nftw_proposal_B.linux.c"

# dl_iterate_phdr
mkdir -p /work/phdr && cd /work/phdr
cp /prog/phdr/phdr_body.hero /prog/phdr/phdr_reads.hero /prog/phdr/phdr_unit.h /prog/phdr/make_phdr.py .
$H check phdr_reads.hero > "$OUT/phdr.reads.check.out" 2>&1; echo "exit $?" >> "$OUT/phdr.reads.check.out"
$H build phdr_body.hero -o body.bin > "$OUT/phdr.body.build.out" 2>&1; echo "exit $?" >> "$OUT/phdr.body.build.out"
./body.bin > "$OUT/phdr.body.run.out" 2>&1; echo "run exit $?" >> "$OUT/phdr.body.run.out"
cp build/tu-*/phdrbody.c phdr_body.unit.c
python3 -I make_phdr.py phdr_body.unit.c > "$OUT/phdr.variants.out" 2>&1
W=$(plain_words); L=$(link_of plain)
for v in at lent; do compile_and_run "phdr_$v" "phdr_$v.c" "$W" "$L" "phdr.$v"; done
$H build phdr_body.hero --sanitize -o body.san.bin > "$OUT/phdr.body.san.build.out" 2>&1; echo "exit $?" >> "$OUT/phdr.body.san.build.out"
WS=$(words_of fsanitize); LS=$(link_of san)
compile_and_run phdr_at_san phdr_at.c "$WS" "$LS" phdr.at.san
cp phdr_at.c "$OUT/phdr_at.linux.c"

# whatever else a later step adds runs from /prog/leg_extra.sh, if present
if [ -f /prog/leg_extra.sh ]; then cd /work && sh /prog/leg_extra.sh "$OUT" "$H"; fi
echo "leg done" > "$OUT/done.txt"

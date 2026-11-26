#!/bin/sh
# Panel 261, the ffi-pragmatist: the steps written after a leg's first run
# (`values/leg_values.sh`: the value with an `@` slot, route F, the reuse
# shape, the census's samples), run alone in a container of the same image,
# into the same /prog/linux-<label>/ without erasing the first run:
#
#   docker run --rm --pull never --name p261-ffi \
#     -v <copy>/build-linux/base.tar:/src/base.tar:ro \
#     -v <copy>/docs/panel/261-reports/programs/ffi-pragmatist:/prog \
#     <image> sh /prog/run_later.sh <label>
set -u
A=$1
OUT=/prog/linux-$A
mkdir -p "$OUT" /work
cd /work && tar xf /src/base.tar
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes > "$OUT/later.seed.log" 2>&1
echo "seed exit $?" >> "$OUT/later.seed.log"
./heroes build selfhost/main.hero -o heroes-next > "$OUT/later.stage2.log" 2>&1
echo "stage2 exit $?" >> "$OUT/later.stage2.log"
./heroes-next --version >> "$OUT/later.stage2.log" 2>&1
cp /prog/build_unit.sh /work/build_unit.sh
mkdir -p /work/values && cd /work/values && sh /prog/values/leg_values.sh "$OUT" /work/heroes-next
echo "later done" > "$OUT/later.done.txt"

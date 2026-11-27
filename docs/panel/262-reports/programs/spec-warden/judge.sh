#!/bin/sh
# For each text named: the base's spec with the text applied and the instrument changed, then the
# `spec` suite and the instrument's own tests, then both files restored and compared by shasum.
# From the copy's root: sh judge.sh <out dir> <N>...
set -u
OUT=$1
shift
R=docs/panel/262-reports
P=$R/programs/spec-warden
mkdir -p "$OUT"
cp spec/heroes-spec.md tmp/spec.orig
cp tests/harness/spec_refusals.hero tmp/spec_refusals.orig
before=$(shasum -a 256 spec/heroes-spec.md tests/harness/spec_refusals.hero)
for n in "$@"; do
    cp "$P/applied/base-w$n.md" spec/heroes-spec.md
    if [ -f "$P/instr/spec_refusals-w$n.hero" ]; then cp "$P/instr/spec_refusals-w$n.hero" tests/harness/spec_refusals.hero; fi
    ./heroes-next run tests/harness/main.hero -- ./heroes-next spec > "$OUT/spec-w$n.txt" 2>&1
    echo "exit $?" >> "$OUT/spec-w$n.txt"
    ./heroes-next test tests/harness/spec_refusals.hero > "$OUT/instr-test-w$n.txt" 2>&1
    echo "exit $?" >> "$OUT/instr-test-w$n.txt"
    cp tmp/spec.orig spec/heroes-spec.md
    cp tmp/spec_refusals.orig tests/harness/spec_refusals.hero
    after=$(shasum -a 256 spec/heroes-spec.md tests/harness/spec_refusals.hero)
    [ "$before" = "$after" ] || { echo "w$n: RESTORE FAILED"; exit 1; }
    echo "w$n: spec $(grep -E 'spec: [0-9]' "$OUT/spec-w$n.txt" | tail -1 | sed 's/^ *//'); failing: $(grep -oE 'FAIL spec/[a-z_-]+' "$OUT/spec-w$n.txt" | sed 's/FAIL //' | sort -u | tr '\n' ' '); instrument tests: $(tail -2 "$OUT/instr-test-w$n.txt" | tr '\n' ' ')"
done
echo "restored: $after"

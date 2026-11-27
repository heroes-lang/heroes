#!/bin/sh
# w2 as finally worded: judge.sh's two runs, then the net's own tests whole, the text and the
# instrument applied; both files restored and compared by shasum. From the copy's root.
set -u
OUT=$1
P=docs/panel/262-reports/programs/spec-warden
sh $P/judge.sh "$OUT" 2
before=$(shasum -a 256 spec/heroes-spec.md tests/harness/spec_refusals.hero)
cp spec/heroes-spec.md tmp/spec.orig2
cp tests/harness/spec_refusals.hero tmp/spec_refusals.orig2
cp $P/applied/base-w2.md spec/heroes-spec.md
cp $P/instr/spec_refusals-w2.hero tests/harness/spec_refusals.hero
./heroes-next test tests/harness/main.hero > "$OUT/net-own-tests-w2.txt" 2>&1
echo "exit $?" >> "$OUT/net-own-tests-w2.txt"
cp tmp/spec.orig2 spec/heroes-spec.md
cp tmp/spec_refusals.orig2 tests/harness/spec_refusals.hero
after=$(shasum -a 256 spec/heroes-spec.md tests/harness/spec_refusals.hero)
[ "$before" = "$after" ] || { echo "RESTORE FAILED"; exit 1; }
echo "net's own tests, w2: $(tail -3 "$OUT/net-own-tests-w2.txt" | tr '\n' ' ')"
echo "restored: $after"

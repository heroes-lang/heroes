#!/bin/bash
# Panel 178 spec-warden: price every draft on the real instrument, restoring the pristine spec after each.
cd /private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/07a257f7-6606-4944-b558-0be3c97d5c11/scratchpad/178-spec-warden/
. /Users/joseph/Temp/heroes/heroes-lang/.env
out=work/prices.tsv
echo -e "draft\tlegacy\tcl100k\treal\trefresh_exit\tdigest" > $out
for d in baseline $(python3 work/drafts.py list); do
  cp work/pristine-spec.md spec/heroes-spec.md
  if [ "$d" != baseline ]; then python3 work/drafts.py $d work/pristine-spec.md spec/heroes-spec.md || { echo "apply failed $d"; continue; }; fi
  off=$(./heroes measure spec/heroes-spec.md 2>&1)
  leg=$(echo "$off" | awk '/claude-legacy/ {print $2}')
  cl=$(echo "$off" | awk '/cl100k_base/ {print $2}')
  ref=$(./heroes measure spec/heroes-spec.md --refresh 2>/dev/null); rc=$?
  real=$(echo "$ref" | awk '/SPEC_REAL_TOKENS/ {getline; print $1}')
  dig=$(echo "$ref" | awk '/SPEC_DIGEST/ {getline; gsub(/"/,""); print $1}')
  echo -e "$d\t$leg\t$cl\t$real\t$rc\t$dig" >> $out
  cp work/pristine-spec.md spec/heroes-spec.md
done
shasum -a 256 spec/heroes-spec.md work/pristine-spec.md

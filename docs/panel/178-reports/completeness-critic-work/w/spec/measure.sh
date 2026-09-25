#!/bin/bash
# Critic, panel 178: price each draft on the real instrument, restoring the pristine spec after each.
cd /private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/07a257f7-6606-4944-b558-0be3c97d5c11/scratchpad/178-completeness-critic/
set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env; set +a
out=w/spec/prices.tsv
echo -e "draft\tcl100k\treal\trefresh_exit\tdigest" > $out
for d in baseline $(python3 w/spec/drafts.py list); do
  cp w/spec/pristine-spec.md spec/heroes-spec.md
  if [ "$d" != baseline ]; then python3 w/spec/drafts.py $d w/spec/pristine-spec.md spec/heroes-spec.md || { echo "apply failed $d"; continue; }; fi
  off=$(./heroes measure spec/heroes-spec.md 2>&1)
  cl=$(echo "$off" | awk '/cl100k_base/ {print $2}')
  ref=$(./heroes measure spec/heroes-spec.md --refresh 2>&1); rc=$?
  echo "$ref" > w/spec/refresh-$d.txt
  real=$(echo "$ref" | awk '/SPEC_REAL_TOKENS/ {getline; print $1}')
  dig=$(echo "$ref" | awk '/SPEC_DIGEST/ {getline; gsub(/"/,""); print $1}')
  echo -e "$d\t$cl\t$real\t$rc\t$dig" >> $out
  cp w/spec/pristine-spec.md spec/heroes-spec.md
done
shasum -a 256 spec/heroes-spec.md w/spec/pristine-spec.md

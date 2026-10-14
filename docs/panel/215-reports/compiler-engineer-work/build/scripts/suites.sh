#!/bin/bash
# The suites with the route's compiler, each into its own file, read whole after.
R=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-panel-215/ce-build
cd $R
export TMPDIR=$R/build
for s in "$@"; do
  echo "=== $s start $(date +%H:%M:%S)"
  ./build/heroes-g run tests/harness/main.hero -- ./build/heroes-g $s > build/suite-$s.txt 2>&1
  echo "=== $s exit $? end $(date +%H:%M:%S)"
done

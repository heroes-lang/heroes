#!/bin/bash
# Cold --emit-c of the compiler: each run from an empty build directory.
R=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-panel-215/ce-build
export TMPDIR=$R/build
for i in 3 4; do
  for comp in today g; do
    d=$R/build/cold2-$comp-$i; rm -rf $d; mkdir -p $d; cd $d
    if [ $comp = today ]; then H=$R/build/heroes-today; export HEROES_RUNTIME=$R/build/rt_today/runtime; else H=$R/build/heroes-g; export HEROES_RUNTIME=$R/runtime; fi
    echo "== $comp run $i, load before: $(uptime | sed 's/.*averages: //')"
    /usr/bin/time -p $H build $R/selfhost/main.hero --emit-c -o out.c 2>&1 | tail -4
    echo "   load after: $(uptime | sed 's/.*averages: //'); bytes $(wc -c < out.c)"
  done
done
cmp $R/build/cold2-today-3/out.c $R/build/cold2-g-3/out.c && echo "today and route emit the same compiler C"

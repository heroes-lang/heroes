#!/bin/bash
# base-check.sh, inside the container: the base's compiler on the shapes the route's declared builds refused on
# Linux, as written (undeclared, over the base's library): its exit and first error, for each.
W=/w; out=$W/results.txt; X=/x; C=$W/c
mkdir -p $X/base && cp -R $C/base/runtime $X/base/runtime && cp $C/base/heroes $X/base/heroes
d=$X/shapes-base; rm -rf $d; cp -R $W/shapes $d; cp $W/progs/*.hero $d/
for t in $d/rebel/heroes.toml $d/face/heroes.toml; do perl -pi -e "s|^path = \".*\"|path = \"$W/lib-undeclared\"|" $t; done
cd $d
for p in audio.hero ui.hero platformhdr/main.hero rebel/reach_hero_audio.hero face/main.hero clock.hero; do
  n=$(echo "$p" | sed 's|/|-|g; s|\.hero$||')
  ( cd $(dirname $p) && $X/base/heroes build $(basename $p) -o $d/bin-$n > $d/$n.out 2> $d/$n.err ); code=$?
  echo "linux base $p plain exit=$code | $(grep -m1 -E 'error|undefined' $d/$n.err | cut -c1-220)" >> "$out"
  grep -m3 -E 'undefined reference|undefined symbol' $d/$n.err | cut -c1-160 | sed 's/^/    /' >> "$out"
done
# and the route's, declared, over its library, with the names its link missed
mkdir -p $X/mine && cp -R $C/mine/runtime $X/mine/runtime && cp $C/mine/heroes $X/mine/heroes
e=$X/shapes-mine; rm -rf $e; cp -R $W/shapes $e; cp $W/progs/*.hero $e/
find $e -name '*.hero' -exec perl -0pi -e 's/^(extern "[^"]+")/$1 layer/mg' {} +
for t in $e/rebel/heroes.toml $e/face/heroes.toml; do perl -pi -e "s|^path = \".*\"|path = \"$W/lib-declared\"|" $t; done
cd $e
for p in audio.hero ui.hero platformhdr/main.hero rebel/reach_hero_audio.hero face/main.hero clock.hero; do
  n=$(echo "$p" | sed 's|/|-|g; s|\.hero$||')
  ( cd $(dirname $p) && $X/mine/heroes build $(basename $p) -o $e/bin-$n > $e/$n.out 2> $e/$n.err ); code=$?
  echo "linux route-declared $p plain exit=$code | $(grep -m1 -E 'error|undefined' $e/$n.err | cut -c1-220)" >> "$out"
  grep -m3 -E 'undefined reference|undefined symbol' $e/$n.err | cut -c1-160 | sed 's/^/    /' >> "$out"
done

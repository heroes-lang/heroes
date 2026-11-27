#!/bin/bash
# b4.sh <compiler> <runtime dir> <program dir> <program> <out prefix> [tags]: build <dir>/<program>.hero with
# HEROES_RUNTIME=<runtime dir> at the run form's builds (O0, O2, sO2, sO0; default all four) into <out prefix>-<tag>.
set -u
C=$1; R=$2; D=$3; P=$4; O=$5; shift 5
tags=${*:-O0 O2 sO2 sO0}
for tag in $tags; do
  case $tag in O0) w="-O0";; O2) w="-O2";; sO2) w="--sanitize -O2";; sO0) w="--sanitize -O0";; esac
  ( cd "$D" && HEROES_RUNTIME="$R" timeout -k 5 900 "$C" build "$P.hero" $w -o "$O-$tag" ) > "$O-$tag.build.txt" 2>&1 || echo "build $P $tag failed: $(tail -3 "$O-$tag.build.txt" | tr '\n' ' ')"
done

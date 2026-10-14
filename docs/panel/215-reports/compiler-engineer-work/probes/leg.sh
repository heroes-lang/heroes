#!/bin/bash
# usage: leg.sh <image> <out file> [g2]   (run from the copy's root)
P=(ctlprobe.hero small.hero small_rz.hero cret.hero rzpad.hero fieldlend.hero uni.hero ffi-rest-zero-a-nested-group-record.hero ffi-rest-zero-a-partial-record.hero ffi-rest-zero-a-record-of-long-arrays.hero ffi-rest-zero-a-union-none-of-whose-members-is-named.hero ffi-rest-zero-a-union-with-one-member-named.hero)
E=(); [ "${3:-}" = g2 ] && E=(-e COMP=g2.c)
docker run --rm "${E[@]}" -v "$PWD":/src:ro "$1" bash /src/build/padg/msan_ce.sh padg "${P[@]}" > "$2" 2>&1
echo "exit $?" >> "$2"

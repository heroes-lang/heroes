#!/bin/sh
# The critic, panel 261's second reading: one program emitted by the base's
# stage 2 and by the route's, the provenance line taken out of each, `cmp`.
#   sh one.sh <base> <route> <file.hero> <out dir>
B=$1; R=$2; F=$3; O=$4
n=$(echo "$F" | tr '/' '_')
d=$(dirname "$F"); f=$(basename "$F")
( cd "$d" && "$B" build "$f" --emit-c 2>/dev/null | grep -v 'hero_program_sources(void) {' > "$O/$n.base.c"; echo $? > "$O/$n.base.rc" )
( cd "$d" && "$R" build "$f" --emit-c 2>/dev/null | grep -v 'hero_program_sources(void) {' > "$O/$n.route.c"; echo $? > "$O/$n.route.rc" )
if cmp -s "$O/$n.base.c" "$O/$n.route.c"; then echo "SAME $(wc -c < "$O/$n.base.c") $F"; rm -f "$O/$n.base.c" "$O/$n.route.c"; else echo "DIFF $(wc -c < "$O/$n.base.c") $(wc -c < "$O/$n.route.c") $F"; fi

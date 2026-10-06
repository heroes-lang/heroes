#!/bin/bash
# Panel 178: every *_INITIALIZER macro the census headers define, after expansion,
# and whether its expansion is all zeros. A non-zero one names a struct whose
# all-zero value the platform does not promise is valid.
TU=/tmp/census-tu.c
[ -f $TU ] || { echo "run census.sh first" >&2; exit 1; }
clang -x c -E -dM "$@" $TU 2>/dev/null | awk '$2 ~ /_INITIALIZER$/ {print $2}' | sort -u | while read m; do
  v=$(printf '#include "%s"\n%s\n' "$TU" "$m" | clang -x c -E -P "$@" - 2>/dev/null | tail -1 | tr -s ' ')
  if echo "$v" | grep -qE '[1-9]|0x[0-9a-fA-F]*[1-9a-fA-F]'; then z=nonzero; else z=zero; fi
  echo "$z	$m	$v"
done

#!/bin/bash
# usage: census.sh <compiler> <mode> <file>   mode: check | emit
# prints: file exit diagnostic-codes
f="$3"
if [ "$2" = check ]; then out=$(timeout 300 "$1" check "$f" 2>&1 >/dev/null); code=$?
else out=$(timeout 300 "$1" build "$f" --emit-c 2>&1 >/dev/null); code=$?; fi
codes=$(printf '%s\n' "$out" | grep -oE '^(error|warning)\[[a-z_]+\]' | sort | uniq -c | tr -s ' ' | tr '\n' ';')
echo "$f $code $codes"

#!/bin/bash
# usage: census.sh <compiler> <file.hero>   prints: file exit codes...
f="$2"
out=$("$1" check "$f" 2>&1); code=$?
codes=$(printf '%s\n' "$out" | grep -oE '^(error|warning)\[[a-z_]+\]' | sort | uniq -c | tr -s ' ' | tr '\n' ';')
echo "$f $code $codes"

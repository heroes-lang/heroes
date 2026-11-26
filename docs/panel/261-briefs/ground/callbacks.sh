#!/bin/sh
# Every function-typed parameter inside a `group` or `extern` block of the tree (panel 261's repair, 2026-11-26, at
# the base df92f9281; the critic's command of its § 5, written down). Run from the repository's root; prints
# `path:line: text` for each `(function(` line inside a block, a block being the lines after a `group` or `extern`
# head up to the next line that starts in column one. Its output at the base is callbacks.txt.
for root in rebel/library selfhost examples rebel/examples; do
    grep -rlE '^(group|extern) ' "$root" --include='*.hero' 2>/dev/null | while read -r f; do
        awk -v F="$f" '/^(group|extern) /{inb=1; next} /^[^ \t#]/ && !/^$/ {inb=0} inb && /\(function\(/ {print F":"NR": "$0}' "$f"
    done
done

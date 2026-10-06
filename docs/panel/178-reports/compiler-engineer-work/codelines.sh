#!/bin/bash
# The layout suite's unit (tests/harness/suite_layout.hero code_lines): non-blank lines outside `test` blocks.
for f in "$@"; do
awk '
/^test "/ { inside = 1; next }
{ if (inside && length($0) > 0 && $0 !~ /^[ \t]/ && $0 !~ /^#/) inside = 0 }
{ t = $0; gsub(/[ \t\r]/, "", t); if (!inside && length(t) > 0) n++ }
END { printf "%s %d\n", FILENAME, n }' "$f"
done

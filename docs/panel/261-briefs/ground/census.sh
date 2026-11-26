#!/bin/sh
# The census of Rebel's HTTP programs (panel 261's clerk, 2026-11-26). Run from the repository's root.
# For each example that uses <rebel>/web or <rebel>/http: files, lines, the router's adds,
# the routes found, and the dispatches by name or by path.
cd rebel/examples || exit 2
printf "%-9s %5s %6s %6s %5s %6s %5s %8s %8s %8s %9s\n" program files lines useweb usehttp addcall add_any routecall ifname== found.nm path==
for d in */; do
  d=${d%/}
  w=$(grep -rl '^use <rebel>/web$' "$d" --include='*.hero' | wc -l | tr -d ' ')
  h=$(grep -rl '^use <rebel>/http$' "$d" --include='*.hero' | wc -l | tr -d ' ')
  [ "$w$h" = "00" ] && continue
  printf "%-9s %5s %6s %6s %5s %6s %5s %8s %8s %8s %9s\n" "$d" \
    "$(find "$d" -name '*.hero' | wc -l | tr -d ' ')" \
    "$(find "$d" -name '*.hero' -exec cat {} + | wc -l | tr -d ' ')" \
    "$w" "$h" \
    "$(grep -rhE '(^|[^_a-z])add\(@' "$d" --include='*.hero' | grep -v '^ *#' | wc -l | tr -d ' ')" \
    "$(grep -rhE 'add_any\(@' "$d" --include='*.hero' | grep -v '^ *#' | wc -l | tr -d ' ')" \
    "$(grep -rhE 'web\.route\(' "$d" --include='*.hero' | grep -v '^ *#' | wc -l | tr -d ' ')" \
    "$(grep -rhE 'if name ==' "$d" --include='*.hero' | grep -v '^ *#' | wc -l | tr -d ' ')" \
    "$(grep -rhE 'found\.name ==|\.name == routes\.' "$d" --include='*.hero' | grep -v '^ *#' | wc -l | tr -d ' ')" \
    "$(grep -rhE 'path ==|path\.starts_with|starts_with\(.*path|url\.path' "$d" --include='*.hero' | grep -v '^ *#' | wc -l | tr -d ' ')"
done

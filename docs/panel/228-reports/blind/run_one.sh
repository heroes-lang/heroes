#!/bin/sh
# One blind session of panel 228, folder $1, in the trunk's build/b. The key is loaded by the caller.
n=$1
root=/Users/joseph/Temp/heroes/heroes-lang/build
d=$root/b/$n
cd "$d" || exit 3
GIT_CEILING_DIRECTORIES=$root/b claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Answer in English." \
  --bare --no-session-persistence --system-prompt "You work in the directory $d. You read its files with the Read tool, by absolute path." \
  --model claude-haiku-5-5 --restricted --safe-mode --strict-mcp-config \
  --tools "Read" --allowedTools "Read" \
  --disallowedTools "Bash,Write,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
  --max-budget-usd 0.035 --output-format stream-json --verbose > "$root/b-out/$n.jsonl" 2> "$root/b-out/$n.err"
echo "$n exit=$?" >> "$root/b-out/exits.txt"

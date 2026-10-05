#!/bin/bash
# Panel 192's fifth blind arm, D: five sessions, one after another, each in its
# own folder of 192-llm-ergonomist-2/ (outside any git tree, no CLAUDE.md above),
# the command of panel 192's blind seat with the model it used. The funded bound is
# the 1.0965 USD left of the sitting's 5: each session's --max-budget-usd is the
# smaller of 0.25 and what is left less 0.03, read from the runs' own
# total_cost_usd, and no session starts under 0.12 left.
set -u
S=/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad
E=$S/192-llm-ergonomist-2
A=$S/192-arm-d
LEFT=1.0965
for n in 1 2 3 4 5; do
  d=$E/d$n
  [ -e "$d" ] && { echo "refused: $d exists"; exit 2; }
  CAP=$(python3 -c "l=$LEFT; print(f'{min(0.25, l - 0.03):.4f}' if l - 0.03 >= 0.12 else 'stop')")
  [ "$CAP" = stop ] && { echo "stopped before d$n: $LEFT USD left"; break; }
  mkdir -p "$d" && cp $A/spec-d.md "$d/spec.md" && cp $A/brief-d.md "$d/brief.md"
  echo "d$n start $(date +%H:%M:%S) cap $CAP left $LEFT"
  (cd "$d" && claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." \
    --model claude-opus-5-5 \
    --restricted --safe-mode --strict-mcp-config \
    --tools "Read,Write" --allowedTools "Read,Write" \
    --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
    --max-budget-usd "$CAP" --output-format json > run.json 2> run.err)
  code=$?
  COST=$(python3 -c "import json; print(json.load(open('$d/run.json')).get('total_cost_usd', 0))" 2>/dev/null || echo 0)
  LEFT=$(python3 -c "print(f'{$LEFT - $COST:.4f}')")
  echo "d$n exit $code $(date +%H:%M:%S) cost $COST left $LEFT; c.hero $( [ -f $d/c.hero ] && echo yes || echo no ), report.md $( [ -f $d/report.md ] && echo yes || echo no )"
done
echo "arm D done $(date +%H:%M:%S), left $LEFT USD of 1.0965"

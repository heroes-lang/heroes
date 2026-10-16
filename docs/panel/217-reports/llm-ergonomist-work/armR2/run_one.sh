#!/bin/sh
# One blind reader of panel 217, run in its own folder: panel 215's launcher
# (docs/panel/215-reports/llm-ergonomist-work-2/run_one.sh) with the model pinned.
cd "$1" || exit 2
claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write your answers here. Answer in English." --model claude-opus-5-5 --bare --restricted --strict-mcp-config --tools "Read,Write" --allowedTools "Read,Write" --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" --max-budget-usd 0.7 --output-format json > run.json 2> run.err
echo "$1 exit $?"

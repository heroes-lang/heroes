# Panel 184, the llm-ergonomist probe: what a fresh session receives

Run by the coordinator at 16:56 on 2026-09-30, from `<scratchpad>/rdr/p0/`, with `claude -p "Read brief.md in this directory and follow it exactly. Write report.md here. Answer in English." --restricted --safe-mode --strict-mcp-config --allowedTools "Read,Write" --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent" --max-budget-usd 0.5 --output-format json`; the CLI reported `total_cost_usd` 0.3405935, 5 turns, `is_error` false. Its brief is `docs/panel/184-briefs/blind/brief-probe.md`. The session's report follows, verbatim.

# Session self-check

## 1. Working directory

As my session was told it:

`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/rdr/p0`

A separate scratchpad directory was also given for temporary files:
`/private/tmp/claude-501/-private-tmp-claude-501--Users-joseph-Temp-heroes-heroes-lang-edfda945-b355-427c-a48e-2113b1040b67-scratchpad-rdr-p0/224a677b-c2d4-4d77-b24f-2fe25bceb288/scratchpad`

## 2. Other instructions and documents received

1. **System prompt / operating instructions** — the base agent instructions (harness behavior, delivering work, corrections, etc.). First line: `You are a Claude agent, built on Anthropic's Claude Agent SDK.`
2. **System reminder with user context** — a `<system-reminder>` block carrying my user's email for identification only. First line: `As you answer the user's questions, you can use the following context:`
3. **Environment block** — working directory, git status, platform, shell, OS, scratchpad path. First line: `# Environment`
4. **Deferred-tools notice** — names of tools whose schemas must be fetched with `ToolSearch` before use. First line: `The following deferred tools are now available via ToolSearch. Their schemas are NOT loaded — calling them directly will fail with InputValidationError. Use ToolSearch with query "select:<name>[,<name>...]" to load tool schemas before calling them:`
5. **Skills listing** — the skills invocable via the `Skill` tool. First line: `The following skills are available for use with the Skill tool:`
6. **Model/date/budget footer** — model identity (`Opus 5 (1M context)`, `claude-opus-5[1m]`), a USD budget line (`USD budget: $0/$0.5; $0.5 remaining`) and `Today's date is 2026-09-30.`
7. **Language preference** — only the one in the user's own message: "Answer in English." (Also in `brief.md`.)

**None** of the following were received: a memory file, project rules, a `CLAUDE.md`, an output style, or a list of extra directories I may use.

## 3. Tools

Directly available: `Artifact`, `ListAgents`, `Read`, `ReportFindings`, `ScheduleWakeup`, `ShareOnboardingGuide`, `Skill`, `ToolSearch`, `Write`.

Deferred (name only, schema must be fetched first): `ArtifactComments`, `ArtifactData`, `CronDelete`, `CronList`, `DesignSync`, `EnterWorktree`, `ExitWorktree`, `NotebookEdit`, `PushNotification`, `SendMessage`, `TaskOutput`, `TaskStop`.

Notably absent: no shell/`Bash`, no `Edit`, no `Grep`/`Glob`, no web fetch or search.

## 4. Read of `.../scratchpad/p184/launch.md`

**Refused.** The error was:

> /private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/p184/launch.md is outside /private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/rdr/p0; --restricted confines the file tools to the working directory.

## 5. Read of `/Users/joseph/Temp/heroes/heroes-lang/CLAUDE.md`

**Refused.** The error was:

> /Users/joseph/Temp/heroes/heroes-lang/CLAUDE.md is outside /private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/rdr/p0; --restricted confines the file tools to the working directory.

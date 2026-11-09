# Panel 239's blind seat: written, sized, unrun

Written 2026-11-09 by the coordinator **at the synthesis, after the seats**, because what it measures
is the words the seats drafted; **unrun**: it is a paid run (`claude -p` sessions), and this
sitting's coordinator was told to start none. The author allowed 10 USD more for paid runs at panel
237's ratification (`git show 5d9e832b2`, 2026-11-08); whether any of it remains for this seat, or a
new yes is owed, is the author's, asked once through the coordinator.

## What it measures

design.md §4.17's promise for the two new refusals: whether a model holding only the specification,
the program and the compiler's words writes the repair its author meant, on the first try. And the
spec-warden's claim (`239-reports/spec-warden.md` § 9): that a message leaning on words the
specification never uses (*unit*, *linker*, *link line*, an archive's *member*) costs a reader who has
only the specification. Three arms per task, label-stripped in the session's folder (each holds one
`task.md`):

- **today**: what today's compiler says (2363: nothing, the program builds and dies; 2364: the false
  `ffi_field_type` and its `guess` fix);
- **seat**: the words the seat's prototype printed (2363: the ffi-pragmatist's v4 words, its paths
  shortened to `lib/`; 2364: the compiler-engineer's `ffi_record_two_layouts`, verbatim from
  `out-agree-mac/s2364_own_main.err`);
- **synthesis**: the same facts in the specification's words, as R6 of the synthesis asks the landing
  to write them (the coordinator's drafts).

Tasks: `task-2363-<arm>.md` (`sig_sorted`: the author meant `1` and `30`), `task-2364-<arm>.md`
(`own`: the author meant `9`). A third question, whether a reader of the specification alone predicts
that `dup_sorted` and `pw_first` build and what they print, is the spec-warden's § 3b table answered
by argument; it is left out on cost.

## Grading, registered before any session

Each answer's files are copied into a fresh folder beside the task's other files and built with
**the landing round's compiler** (the one holding both repairs), on this Mac:

- **right**: it builds at exit 0 and prints what the author meant (`1` then `30`; `9`), with no file
  of a library's C rewritten into another library's meaning (`pick_w.c`'s `pick` may be made
  `static` or removed, not renamed into `liba`'s);
- **wrong, refused**: the landing compiler refuses it;
- **wrong, silent**: it builds and prints anything else, or dies.

The decision rule: an arm whose words give fewer `right` answers than another arm's by two or more
of six is the worse; the synthesis's arm is adopted for the landing unless the seat's arm beats it so.

## The run, and its size

Two sessions per task and arm, **12 sessions**, each in a folder of its own under
`p239-scratch/blind239/<task>-<arm>-<n>/` (inside the repository's root and ignored by git, as panel
237's `build/b237/seat/` was), holding `brief.md`, `heroes-spec.md` (the specification at the
sitting's commit) and `task.md`; run with git blind (`GIT_CEILING_DIRECTORIES` set to the folder's
parent) and the key loaded from the trunk's `.env`, never printed, by the `/panel` skill's command:

```
claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." \
  --bare --restricted --safe-mode --strict-mcp-config \
  --tools "Read,Write" --allowedTools "Read,Write" \
  --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
  --max-budget-usd 0.45 --output-format json > run.json 2> run.err
```

**Bound: 0.45 USD a session, 5.40 USD for the twelve**, the runner stopping once the reported sum
passes it. Panel 237's one blind session reported 0.3635 USD (`237-reports/llm-ergonomist.md`), so
about 4.40 USD is the inference for twelve, not a measurement. The model: `claude-opus-5-5` where
the CLI supports it, else the one it runs, said in the report.

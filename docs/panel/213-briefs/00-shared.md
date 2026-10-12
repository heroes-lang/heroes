# Panel 213, the shared brief: how a stopped Heroes program shows Heroes values

Written 2026-10-12 to 13 by the coordinator. **Every number, path and count below
is followed by the command that produced it, run while this brief was written**
(`.claude/skills/panel/SKILL.md` § 2, CL-077); a sentence that says the tree
lacks something names what was searched. Read your own brief after this one.

## The sitting

M-typed-inspection's opening sitting, GitHub issue 939 (`gh issue view 939`),
convened without asking: the author's answers of 2026-10-12 asked for it once
for the milestone (`gh issue view 2029`). It is a full panel (CLAUDE.md § 4:
the tool surface is a trigger; `.claude/skills/panel/SKILL.md` § Two lanes).

**The proposal, as a question with its routes** (the routes are the coordinator's
list and not the option set; a route nobody listed is worth more than a vote):
a program built at `-O0` and stopped in a debugger, lldb directly or the editor
extension's lldb-dap, should show what the author wrote: `p` and not `h4_p`, a
record's fields `x` and `y` and not `f_x` and `f_y`, a `[T]` and a `{K: V}` with
their contents and not an opaque header pointer, a `T?` with its one live arm, a
variant as its case and payload, and a frame without the emitted C's temporaries.
Routes:

- **A, nothing**: the status quo; the author reads C names.
- **B, a formatter module the build emits beside the binary** (Python for lldb,
  loaded with `command script import`), which nobody types.
- **C, formatters embedded in the binary**, in a section the debugger loads by
  itself (whether lldb on this Mac, on the Linux image and through lldb-dap
  supports that, and in what format, is a question, not a premise).
- **D, a `heroes` surface that starts the debugger with the formatters**: a
  flag such as `heroes run --debug`, or a verb (`.claude/rules/cli-surface.md`
  admits a new verb only as a proven overload).
- **E, names a debugger reads with no formatter**: the C the compiler writes,
  or its debug information, carrying the author's names directly.
- **F, the frame's temporaries**: hidden by the emitter (scope, struct,
  artificial marks) or filtered by the debugger side.
- **More routes the critic named at pass 1** (`213-reports/critic-briefs.md`):
  **G**, one fixed formatter per toolchain rather than per build, keyed on the
  runtime's C types and versioned by `HERO_RUNTIME_ABI`; **H**, emitted C types
  that carry the shape, so the debug information alone shows it; **I**, a type
  table emitted into the binary that one generic formatter reads, with no ABI
  move; **J**, the editor's side alone (lldb-dap's settings or `extension.js`);
  **B without Python**, lldb's summary strings and synthetic children (on
  Windows `lldb.exe` dies for want of a Python DLL); and **gdb** on Linux.

**What the sitting must rule** (issue 939 and the milestone page,
`docs/roadmap/milestones/M-typed-inspection.md`): whether CLAUDE.md § 10's
*never a script* forbids a formatter the BUILD writes and nobody types; the
surface (B, C, D, E, a combination, or A); whether the frame filter needs the
`$`-slot distinction kept past the mangler (a slot table beside the binary is
the page's name for it, and a sitting's question); Windows; and how the editor
extension consumes it. Not this milestone's (the page): `print(p)` on a record,
and calling a generated renderer inside the stopped process.

## The tree, frozen

- The sitting's tree: worktree `.claude/worktrees/lane-panel-213`, branch
  `lane-panel-213`, at `0b8307daf` (`git log --oneline -1`), `git status`
  clean but for this folder (`git status --short`). It is the milestone's lane
  (`lane-typed-inspection`) at its step 3 plus the trunk's panel evidence.
- **Your copy**: `git worktree add --detach .claude/worktrees/scratch-panel-213/<your seat> 0b8307daf`
  from the repository's root, then inside it `clang -I runtime seed/heroes.c
  runtime/runtime.c -o heroes` (seconds; never another seat's compiler or the
  trunk's) and `mkdir -p build`. You read, build and run only there; you write
  your report into the sitting's tree at
  `.claude/worktrees/lane-panel-213/docs/panel/213-reports/<your seat>.md`, as
  you go and not only at the end. Nothing is written outside the repository's
  root (CLAUDE.md § Hard stops); `.claude/worktrees/` is inside it and ignored.
- Rebuilding the compiler from `selfhost/` in your copy:
  `./heroes build selfhost/main.hero -o build/heroes-next`, about a minute
  (`.claude/skills/panel/SKILL.md` § 2's measurement of 2026-09-24).
- **No paid run** (no `claude -p`, no API call, no cloud run), **no credential
  printed** (`gh auth status` if you must know, never `gh auth token`), **no
  GitHub write**, **no push**, no `git add -A`, no `git commit -a`, no
  `git stash`. Never pipe a suite into `head` or `tail` (the guard refuses it):
  write it to a file under your `build/` and read it.

## The evidence, measured

- **The debugger here**: `lldb --version` printed `lldb-2103.0.34.103` (this
  Mac, Darwin arm64, Xcode's lldb). The Linux images: `docker images` lists
  `heroes-linux-arm64:latest`; its lldb is unmeasured in this brief.
- **The before-table and the census**: `docs/measurements/042-a-stopped-frame-shows-the-c-names-and-of-its-locals-the-author-wrote-fewer-than-one-in-ten.md`,
  measured on this Mac on 2026-10-12 by the milestone's step 3, with its
  transcripts in the `.tar.xz` beside it (§ 8 says how to put them back). Its
  counts are that measurement's and not this brief's: a seat that rests a
  conclusion on one re-runs § 5's method in its own copy.
- **The mangler**: `selfhost/emit/mangle.hero` is 265 lines (`wc -l`); its
  `slot(index, name)` (lines 147 to 177, read) returns `"h" + index + "_" +`
  the name with every non-word character made `_` and leading `_` stripped, so
  a lowering's `$i` in slot 5 becomes `h5_i` and the `$` is gone.
- **The runtime's shapes**: `runtime/heroes_runtime.h` is 825 lines (`wc -l`);
  `struct HeroDesc` at line 486 holds `size`, `copy`, `drop`, `eq`, `hash` and
  no name; `HeroArrayHeader` at 535 to 541 holds `refcount`, `len`, `cap`,
  `elem` (a `const HeroDesc *`), the elements in-line after it;
  `HeroMapHeader` at 755 to 764 holds `refcount`, `len`, `cap`, `key`, `val`
  and three byte offsets, `states`, `keys`, `vals` (`grep -n` and read).
- **A user type's descriptor is a symbol of the binary**: built tonight,
  `record Room` in module `rooms` at `-O0` (`heroes build rooms.hero -O0 -o
  rooms-bin` in a scratch folder), `nm rooms-bin` lists `_h_rooms_Room_desc`
  (`s`, a local data symbol) beside `_h_rooms_Room_desc_copy`, `_drop`, `_eq`,
  and the runtime's `_hero_desc_array`, `_hero_desc_bool`, `_hero_desc_f32`
  and so on. Issue 939 wrote `_h_desc_Room_desc` on 2026-09-06; the name is
  module-qualified today.
- **The 2026-09-06 prototype is not in the tree**: `git grep -l -E
  "lldb\.SB|SBValue|SBType|__lldb_init_module|type summary add|type synthetic add"`
  over everything but `archive/` returned nothing. What issue 939 says it did
  (read `len` from the header, resolve `elem` to the descriptor's symbol, print
  `"ada"` and `"grace"`) is the issue's report, unrun tonight.
- **What a build writes today**: `heroes build locals.hero -O0 -o locals-bin`
  in a scratch folder wrote `locals-bin` and, under `build/`, the runtime's and
  the units' objects with their key files and two response files (`find .
  -newer` after the build); nothing else beside the binary, **and no dSYM**:
  the variables' debug information stays in the objects under `build/`, which
  the binary names by absolute path (`nm -ap prog | grep OSO`), so a debugger's
  auto-load from a dSYM has nothing to load from today.
- **The command's surface**: `selfhost/cli/table.hero` lines 98 to 115 (read):
  `build` takes `--dump-ir`, `--emit-c`, `-o`, `--include`, `--library`,
  `--sanitize`, `-O0` (its default) and `-O2`; `run` takes `-o`, `--include`,
  `--library`, `--sanitize`, `-O0` and `-O2` (its default). `heroes help build`
  prints the whole usage, not `build`'s alone. `selfhost/cli/flags.hero`
  (493 lines) gives `-O0` the debug word `-g` and every other level
  `-gline-tables-only` (`debug_words`, lines 169 to 172, read), so a `run` that
  showed variables would have to build at `-O0`. CLAUDE.md § 10: *Any new capability is a
  `heroes` subcommand or flag. Never a second binary, never a script, never a
  Makefile.* The stopping rule is `.claude/rules/cli-surface.md`.
- **The instrument**: the `lldb` suite (`tests/harness/suite_lldb.hero`, its
  cases in `tests/golden/lldb/`) read `lldb: 4 passed, 0 failed` on this Mac
  tonight (`./heroes run tests/harness/main.hero -- ./heroes lldb`); its
  `locals` case pins today's C names on purpose.
- **The editor**: `editors/vscode/extension.js` resolves a `heroes` debug
  configuration by building at `-O0` and returning an `lldb-dap` one
  (read). `editors/README.md` § Debugging says the variables show the C names
  until this milestone and that it improves then with no change in the
  extension: that is the README's premise, unrun, and a question for the sitting.
- **Windows**: `.github/workflows/ci.yml` records at line 487 that LLVM's
  `lldb.exe` exits `0xC0000135` before running a command (`grep -n`).
  `selfhost/cli/doctor.hero` (402 lines) judges each row as `Check(name, ok:
  bool, detail)`, two outcomes (`grep -n`); whether a row for an absent but
  optional tool needs a third outcome, or can be a row shown on some platforms
  only as some rows already are, is a question (critic, pass 1).
- **Issue 939's own refusals are the issue's view, not this brief's**: its
  notes refuse a `name` field on the descriptor ("the linker already holds the
  name", which holds for an unstripped binary only: after `strip`, `nm` finds no
  `Room_desc`, the critic's measure) and route A "on §12". Weigh them, do not
  inherit them.

## What every report holds

The structure your agent definition gives (`.claude/agents/<seat>.md`), plus:
the route you would adopt and why, the route you refuse and the program or
fact that refuses it; every number with its command; a **prediction** naming the
instrument that scores it and the milestone; and what you could not run.

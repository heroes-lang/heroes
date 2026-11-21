# Panel 253, compiler-engineer

Read `00-shared.md` first, whole: the frozen commit (`181555e9a32d0b18878f9f10e48d2eadd939c285`), the six
questions with their robust and conservative resolutions, the measurements, the seats and the rules.
You judge the ceiling (design.md §1.1, §1.7, Part 5: `grep -n -E '^### 1\.(1|7) |^## Part 5'
docs/design.md`) and what each route costs the compiler and the runtime, in lines and where they land.
Veto on a core construct or a breached ceiling.

**Your folder** is `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/253-compiler-engineer/`,
made by you as `00-shared.md` § Where each seat works shows; binaries and containers carry `ce`. **Your
report** is `.claude/worktrees/rebel-round/docs/panel/253-reports/compiler-engineer.md`, written as you go,
in `.claude/agents/compiler-engineer.md`'s structure, **one block per question you sit for**: 2, 4, 5, 6,
and in 1 the compiler's two homes of the Zen and `selfhost/library_source.hero`. **The route you would
adopt you build in your copy and run on the cases you name** (`SKILL.md:274-277`); stage 2 read 136 s for
the writer at load 11 and 130 s for the critic at load 12 to 14, so plan 150 to 300 s per rebuild (a plan,
never a measurement: § No wall clock below). One build or run of yours at a time. No `heroes test` of the
compiler or of a whole package.

**The sitting is whole, six questions, and you work in this order** (the coordinator's decision (a),
`00-shared.md` § One sitting, six questions): **each question's block written whole in your report before
the next begins**, so a stop leaves whole blocks; **the spec-facing halves first**, which the spec-warden
and the blind seat's rows wait on: question 4 whole (the name, the channel, a run of nothing but skips),
then question 2's refusals and golden form (its items 1 and 3), then question 6's refusal over a `path`
entry and the `path` question (its items 1, first half, and 6); **then** the rest of 2 and 6 and question
1's two homes; **question 5 last**. **Where to stop when the clock runs short, cut in this order**: (a)
question 5's (W) built on this Mac only, its Linux arm64 rows left to the ffi-pragmatist's question 5.1,
and (P) priced by `--emit-c | wc -c` and one program's instructions retired, not the compiler's own
build; (b) question 6's index publication and its `fetch` costs; (c) question 2's widening to `heroes
check` (its item 5), counted only if the verb is built. Say in your report's first lines which cuts you
took.

**No wall clock** (the coordinator's decision (b)): five agents share this Mac (load 5.5 to 18 while these
briefs were written and repaired, `uptime`), and CLAUDE.md § Verification says *"while a clock runs the
machine stays still"*, so no seconds you take now survive the ratio rule. Every cost you report is
**instructions retired** (`/usr/bin/time -l <command>`, its `instructions retired` line, best of N stated,
the first discarded) and **bytes** (`ls -l`, `wc -c`, `du -k`); a seconds figure you want is named **owed
to a still machine**.

## Question 2: `heroes doc`

**Where it would land, read at the frozen commit** (`wc -l`): the argv table `selfhost/cli/table.hero`
(310; 16 `Command(` rows at 187-202), the verbs `selfhost/cli/verbs.hero` (376), the operand kinds
(`.file_operand`, `.package_operand`, ...; `selfhost/cli/file_operand.hero`, `argv.hero`); the signature
printer `selfhost/print/signature.hero` (155) and the formatter `selfhost/print/fmt.hero` (1,848), which keep
comments (the probe holds it); the label rule the call is derived by, `selfhost/check/type_fit.hero` (169,
its `shared`, which `tests/harness/package_card.hero:11-19` cites); a package operand's face found through
the catalogue (`selfhost/module/catalogue.hero` 487, `reached.hero` 40). The card writer it would replace
or sit beside: `tests/harness/package_card.hero` (452), `package_cards.hero` (115), `module_docs.hero`
(212), `package_judge.hero`.

**What you are asked**:

1. **Price and build the robust verb** in your copy: `heroes doc <module.hero>` printing the module doc,
   then per declaration its signature as `fmt` prints it, its comment, the call derived by the checker's
   label rule (never a second reader's), and its examples (the documented `test` blocks titled
   `<name>:`), **the same bytes every run**; exit 1 with a diagnostic for a parameter line naming no
   parameter, two out of order, and an example titled with no function of the module (each a code, its
   `Fix` tagged `certain` or `guess` by `.claude/rules/diagnostics-and-goldens.md`); and `heroes doc
   <rebel>/json` from a program's folder through its catalogue. Say how many lines in which files, and
   whether it is core or a reader of what the frontend already holds.
2. **Run it over Rebel's faces** (`packages/rebel/0.1.0/files`, 149 faces by `bw/tools/doc_census.py`)
   twice and `cmp` the outputs; count the largest face's cost in instructions retired (`/usr/bin/time
   -l`, best of N stated, the first discarded) and its output's bytes; count what its refusals would
   refuse, against the writer's reader of lines (642 functions whose parameter lines disagree with their
   signature, 626 of them with parameters and no parameter line at all, 929 with no example,
   `bw/q2/doc-census.txt`, the script repaired after the critic found it reading a function type's `->`
   as a bracket): say where the two readers differ and which is right. **No check reads Rebel's faces
   today** (`00-shared.md` § Question 2): § A7's *"check of the net"* is one to write, under the verb or
   beside it.
3. **What a golden form `doc` owes**: `tests/golden/doc/` beside the 14 groups (`ls tests/golden | wc
   -l`), the harness's `forms` list and `suite_golden.hero`'s shape, `canonical` and `annotations` over
   its cases, a determinism case; and the site's claims (`site/src/lib/claims.ts:264` counts the
   `Command(` rows; four pages say *sixteen verbs*, `00-shared.md` § Question 2).
4. **The conservative**, no verb and the card writer grown into the reference's printer: its lines, and
   what it cannot do that the verb can (a package outside this repository).
5. **Whether a refusal belongs to `heroes doc` alone or also to `heroes check`**: a comment's shape
   refused by the build is a wider class; say what it would refuse in `selfhost/` and `examples/` today
   (`heroes-next check` over them with your prototype's rule on, counted).

## Question 4: a skipped outcome

**Where it lands**: `heroes test`'s loop, `selfhost/cli/verbs.hero:175-207` (each test a process of the
built binary, `ok` where `one.started && one.code == 0`, else `FAIL`, 188-192; the count lines 198-207);
the test binary's entry, `runtime/parts/panic.c:144` (`hero_test_index`) and `selfhost/emit/c_main.hero`
(214); the built-ins' table `selfhost/inventory.hero` (370); `builtin_name_taken` in
`selfhost/resolve/top.hero` (577) and `resolve/decls.hero`; panel 251's finding that a built-in's name is
taken everywhere (task 2551, `OPEN`: *"The specification says that a built-in's name is taken
everywhere"*).

**What you are asked**:

1. **Build the robust route** (`bw/arms/spec-skip-r.diff`, +59 vendored): `skip(reason: str)` inside a
   `test` block ends it as skipped; `heroes test` prints a line naming the title and the reason, counts it
   apart (*"3 tests, 1 failed, 1 skipped"*), exits 0 where nothing failed; `skip` outside a `test` block is
   refused. **Choose and justify how the test process tells the runner**, by something no assert, abort,
   signal or `exit(code:)` of the program can produce (today `exit(code:)` takes 0 to 255, spec line 362,
   and an abort is 134, `00-shared.md` § Question 5): measure that your channel cannot be forged by a
   program's own `exit`.
2. **The name**: `skip` taken by a built-in refuses 7 declarations (`00-shared.md` § Question 4: one in
   `selfhost/parse/braces.hero`, one in `examples/markdown/inline.hero`, five in Rebel, among them
   `io.hero`'s public `skip`) and an unread number of bindings (112 lines by a rough grep). Count them by
   the compiler (`heroes-next check` over the trees with your built-in on), and price `skip_test` or
   another spelling against it.
3. **What a run where every test skipped is**: green, or a failure as the harness's floor makes it
   (`tests/harness/report.hero:49-53`)? Build what you would adopt and show both outputs.
4. **Run the 106 tests that read `ok` today without running their subject** (`bw/q4/`): pick five of
   them, rewrite each with `skip` in your copy, and show `heroes test` naming them skipped; one of
   `mongodb.hero`'s silent 51 among them.
5. **The conservative** costs no compiler line: say so, or say what it does cost.

## Question 5: the stack of calls at an abort

**What stands**: `runtime/parts/panic.c` (159: `hero_panic` 58, `hero_panic_where` declared at 69), its
body in `runtime/parts/place.c:100` (387 lines), the stack guard's walk `parts/stack.c` (1,379: `hero_stack_symbol` 585, `hero_stack_names` 862,
*"Windows has no walk"* 858), `parts/symbols.c` (192, Linux's own symbol table), the place the compiler
passes, `selfhost/emit/stop_place.hero` (238), panel 230's R1 (`docs/panel/230-*.md:110-135`) and the
library's caller place (`:120-123`). The compile flags `selfhost/cli/flags.hero:133-153` carry
`-fno-optimize-sibling-calls` (150) and no `-fno-omit-frame-pointer`. The measured outputs:
`00-shared.md` § Question 5 (`bw/q5/deep.hero`, `deepmust.hero`, `rebel/main.hero`).

**What you are asked**:

**What the brief settled for you** (`00-shared.md` § Question 5, the critic's findings 7 and 8, re-run by
the writer): this Mac's clang driver keeps frame records at `-O2` for `arm64-apple-macos` and
`aarch64-linux-gnu` (`-mframe-pointer=non-leaf`) and drops them for `x86_64-linux-gnu` and
`x86_64-pc-windows-msvc` (`none`), `flags.hero` passing no `-fno-omit-frame-pointer`; and every build
already carries line tables (`debug_words`, `flags.hero:172-175`: `-g` at `-O0`, `-gline-tables-only`
otherwise), which on Darwin stay in the build's objects (no `__debug` section in the binary, a debug map of
`N_OSO` entries, no `.dSYM`).

1. **Price and build (W)**, the frame walk at every abort through `hero_panic_where`, naming each Heroes
   frame by the guard's symbol lookup and **its `.hero` line**: say where the line comes from (a table the
   emitter writes per call site, keyed how; or the line tables every build carries, read how and by what,
   on Darwin from the objects of `build/`), and **build it on this Mac only** (its Linux arm64 rows are the
   ffi-pragmatist's question 5.1); run `bw/q5/deep.hero`, `deepmust.hero` and `rebel/main.hero` at `-O0`
   and `-O2` and show the output. An inlined frame at `-O2`: shown, or lost, measured. What
   `-fno-omit-frame-pointer` costs where it is needed (x86-64), in bytes of the emitted binary and
   instructions retired of one program, priced.
2. **Price (P)**, the caller's place passed to every call that can reach an abort (panel 230's library
   route made general): the emitted C's size (`--emit-c | wc -c`, with and without) and one program's
   instructions retired on its happy path (`/usr/bin/time -l`, best of N stated, the first discarded),
   not the compiler's own build.
2a. **Price (A)**, the critic's route (`00-shared.md` § Question 5): the runtime writes the frames' return
   addresses alone, allocating nothing, and `heroes run` and `heroes test`, the program's parent, resolve
   them against the binary and its build's line tables; what a binary run alone then shows.
3. **Price (B)**, the program's call into a package alone: the boundary as the compiler knows it
   (`module/reached.hero`), the change to `stop_place.hero`.
4. **The rules every route keeps** (panel 230's, carried): the first line and the place written from a
   fixed buffer before anything allocates; the stop final (`panic.c:34-57`); defect 2587, `OPEN`, *"A
   runtime panic never says its line while another thread is blocked writing stdout"*: does the walk
   make it worse?
5. **Windows**: what (W) needs there (dbghelp and a PDB, `stack.c:1231`, `:1300-1301`), owed to the box;
   what (P) and (B) need (nothing platform-specific, by construction): say which you would adopt knowing
   the box is not yours today.

## Question 6: the umbrella's placement

**What stands**: `heroes add` (`selfhost/fetch/add.hero` 469, `add_wanted.hero`), `heroes fetch`
(`fetch/fetch_all.hero` 276, `placing.hero` 71, `listing_read.hero` 92), the catalogue's reader
(`selfhost/module/catalogue.hero` 487, `sums.hero` 99, `placed.hero` 239, `unplaced.hero` 182), the operand
`.package_operand` (*"a name from the index, a git address with --tag, or a folder with --path"*,
`table.hero:194`); panel 250's measurements of M (`docs/panel/250-reports/compiler-engineer.md`, carried
there: *"+294 code lines and +389 lines of compiler in 12 files"*, *"a correct `use` refused, 17 of 17"*,
*"placement a function of the order the verbs ran"*).

**What you are asked**:

1. **Build the robust spelling** (`bw/arms/spec-umb-r.diff`, +134 vendored): a catalogue entry's
   `packages` list; `heroes add rebel/csv` adding a package to it; `heroes fetch` placing the listed
   packages and their Rebel closure, each file held to the version's listing; a `use <rebel>/x` of a
   package not listed refused with a `certain` fix naming `heroes add rebel/x`. Rebel is not in the index
   (`packages/rebel/` holds no `versions` and no `list`, `ls`): publish it in your copy's index as
   `packages/core/` is published (`packages/core/versions`, `0.6.0/list`), say how you made its listing,
   and fetch from it.
2. **Re-measure 250's two findings under the list**: a correct `use` refused (which programs, of which
   set), and placement depending on the order the verbs ran; and the falsifier of
   `.claude/rules/cli-surface.md` (deleting a name from `packages`, mistyping one) as a `check` golden
   case each.
3. **The cost**: lines and files of compiler, and what the programs place under W and under the list:
   **the 161 programs of `examples-rebel/`**, real ones (their distribution is `bw/q6/examples-rebel.txt`,
   one row each in `examples-rebel-programs.txt`), and the five sets of `00-shared.md` § Question 6 table,
   which are the writer's inventions; for each, files and bytes placed and `heroes fetch`'s instructions
   retired, cold (no seconds); `bw/tools/closure.py` is yours to read and re-run.
4. **The routes between**: (M') placement computed from the `use` lines with no list, at the package
   grain, against 250's findings; (E) one index entry per package at one version the index enforces, and
   what `use <rebel>/csv` would then name. Price what you do not build.
5. **What panel 242's R2 and panel 250's R1 still bind** after decision 6 (*"The compiler does not use
   Rebel to write itself"*): your reading, with the ruler's sentence (`docs/design.md:578-579`).
6. **A `path` entry** (the critic's finding 14): does `packages` bind one? `examples-rebel/heroes.toml` is
   one `path` entry for 161 programs whose `use <rebel>/` lines name 163 distinct paths: build both
   readings over it (every face listed; the entry exempt) and say which you adopt and what the other
   refuses or leaves unchecked.

## Question 1, for the compiler's part

The Zen's law 18 stands in `selfhost/cli/doctor.hero:254` (its test at `:287`) and in the seed
(`seed/heroes.c:365`); say what changing it costs (the seed refreshed only where the two-stage build goes
red or a release closes, `.claude/rules/verification.md` § The batch) and whether the decision touches
`selfhost/library_source.hero` (panel 097's condition 5; `wc -l` it and say what it holds).

## What you are asked to say

Your definition's structure per question; every cost with its file and line count; the route you would
adopt built and run, with its commands and outputs in your folder; what you did not build and why; every
premise of these briefs you found false, first.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/253-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out.

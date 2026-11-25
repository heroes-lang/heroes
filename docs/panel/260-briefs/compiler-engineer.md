# Panel 260, the compiler-engineer's brief

Read `docs/panel/260-briefs/00-shared.md` first, whole: the author's principle (§ 1), the questions (§ 2), the tree and
the language today (§ 3), the cost measured (§ 4), the guards (§ 5), the shapes (§ 6) and the routes as questions (§ 7).
Then, if the coordinator's prompt says it exists, the critic's first reading (`docs/panel/260-reports/critic-briefs.md`),
which may have repaired a sentence here. Your seat judges **implementation cost, soundness and what the compiler owes**
(design.md §1.1, §1.7, §1.12, Part 5, reached by `grep -n`; `.claude/rules/generated-c.md`). You have a veto, on
soundness, and it yields to the most robust resolution. Your files are `selfhost/` and `runtime/`, never `crates/` (not
in the tree; the Rust bootstrap is the tag `m-bootstrap-archive`).

**Your copy** is `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/260-compiler-engineer`, a detached worktree the
coordinator made at the base `00-shared.md` names, **`c3885b81b`** (re-frozen after the critic's first reading; what moved
since `80e2e39ad` is `00-shared.md`'s opening and § 4.5). Build there from the seed, then stage 2 (`00-shared.md` § 10:
the seed predates steps 184a to 184c and 182c, so stage 1 is not the base's compiler); time with a compiler built from
its emitted C by `clang -O2`, as `ground/build.sh` does, never with stage 2's own build against the seeds' `-O2` ones. A runtime you
change is a copy of `runtime/` beside the compiler that reads it, never the copy's own `runtime/` edited under a build
another process reads. **You hold this Mac and Linux arm64** (`heroes-linux-arm64:latest`, one container at a time,
`--rm`, named `p260-compiler-engineer`, removed when idle).

## The code, with its size (`wc -l` at the base `c3885b81b`; every line cited below printed there with `sed -n <n>p`)

| file | lines | what it holds for this question |
|---|---|---|
| `selfhost/cli/include_stands.hero`, `include_lines.hero`, `include_told.hero` | 366, 212, 99 | **step 182c's walk, new since `80e2e39ad`** (`00-shared.md` § 4.5): `stopped` (93) walks, before any clang, the runtime's opening and each group's header to any depth as clang's search finds each name (`search_of` 178, which asks `package_reads.toolchain_own`, a `clang -E -v` over an empty file, `package_reads.hero:88`); `include_lines` reads `#include`, `#include_next`, `#import`, `#embed`, `__has_include` and a `#define`d include, a block under `#if 0` passed over and every other `#if` read as true; called from `shadowed.checked` (`shadowed.hero` 493 lines, the call at 120), which `produce.hero:219` asks |
| `selfhost/cli/header_needs.hero`, `header_bodies.hero` | 441, 321 | the bodies' reading, **asked only after a link fails** (its doc, lines 1 to 45): today's answer to an undeclared program's failed link, `ffi_missing_link` *"no group in this program says which library has it"* |
| `selfhost/emit/extern_refs.hero` | 94 | a `used` reference held to each C function a module's unit calls through a group (its lines 1 to 16; panel 202's R2, defect 562), so a link takes what a reached module's bindings name, not what the program calls: measured (`00-shared.md` § 6.2), a used module whose clock binding is called only in a `test` block makes a program that never reads the clock take `hero_time` (`shapes/testonly/`), and `time.mark()` alone takes `hero_disk` and `hero_net` (`shapes/face/`) |
| `selfhost/check/runtime_header.hero` | 97 | **the precedent at `check`**: a group naming `heroes_runtime.h` refused at its own string, reading no header (panel 232's R6, defect 2256) |
| `selfhost/cli/runtime_bound.hero` | 98 | **the precedent at `build`**: a binding of a name the runtime's header declares, refused whatever header its group names, a header of the program's own that includes it among them, read from the header's C words (`own_names.reserved`, `c_words.used`) |
| `selfhost/parse/group.hero`, `selfhost/emit/externs.hero` | 275, 349 | the group and its header (`externs.unquoted` 70, `is_own` 94, `headers` 116) |
| `selfhost/cli/runtime_key.hero` | 400 | `layer_members` (107), `swift_members` (129), `made_of` (69): the list of the layer's members, read from `runtime.c` |
| `selfhost/cli/layer_reach.hero` | 283 | `for_build` (49), the one door a build asks; `headers_of` (72), the groups' header names; `reached` (97), the Swift closure; its doc (20 to 23) names what it cannot see |
| `selfhost/cli/layer.hero` | 587 | `archive` (71), `compiled` (127), `reaches` (183); `defined_by` (255) and `taken` (305) read an archive with `nm`; R6's own test at 330, defect 443's at 465, 2625's at 422, 2489's at 576 |
| `selfhost/cli/reached_each.hero`, `opening.hero` | 252, 409 | defect 2289's question, for the members and for each unit (`opening.reached` 171) |
| `selfhost/cli/produce.hero`, `assemble.hero`, `value_run.hero` | 525, 341, 315 | the call at `produce:156`, the archive on the link at `assemble:263`, the value probe's own link at `value_run:194` |
| `selfhost/cli/link.hero`, `link_report.hero` | 855, 359 | `link_objects` (50); the undefined names in each linker's words (`references` 77) |
| `selfhost/cli/supplied*.hero` | 145 to 457 (twelve files: `supplied_versions.hero` 145, `supplied_windows.hero` 150, `supplied.hero` 357, `supplied_judged.hero` 457) | the link asked who supplied a name, only where the program names a library (`supplied.asked`, 148) |

## What you are asked to settle, in this order

1. **The declaration and its check, built.** Choose the home you would adopt among `00-shared.md` § 7.1's (or another),
   and build it in your copy: the form (parsed, carried, printed by the formatter: `.claude/rules/diagnostics-and-goldens.md`
   § A new surface form lands in every tool that reads the language), the check, and the build that follows it. Say:
   - **what `heroes check` of a package's own module can know**: the list of the layer's headers is the runtime's today
     (`runtime_key.layer_members` through the toolchain); `check/runtime_header.hero` judges a header by its name alone
     and reads none. Does the checker read the runtime's list, or does the language carry a rule a name obeys?
   - **what *binds the layer* means for the check**: `00-shared.md` § 6.2 reads the candidates from the files and runs
     none of them as a check: the group's header name (the 54 files of § 3.1); what each unit's opening reaches (§ 6.2's
     **ten** programs, which bind only their own header's `static` functions whose bodies call the layer, the tenth two
     includes deep); a bound name a member's header declares, whatever header the group names, as
     `cli/runtime_bound.hero` reads the runtime's names (it sees `owndecl` and none of the ten); the names a group's
     header bodies call (`cli/header_needs.hero`, `header_bodies.hero`, today read **only after a link fails**); the
     link's missing names; **step 182c's walk**, already built and run before the first clang of every build
     (`00-shared.md` § 4.5), which reads every `#if` but `#if 0` as true; the undefined names of the program's own
     objects read by `nm` before the link (`layer.defined_by` and `layer.taken` already read an archive so). Build the
     one or the pair you would adopt, run it on every shape of `shapes/` (the critic's `constonly`, `platformhdr`,
     `testonly`, `face` and `twofam` among them, on which the readings disagree), and say at which verb each part is
     told (`check` for the writer, `build` for what only the build knows; `heroes test` of a floor, `run`, `--emit-c`
     and the value probe answered the same way).
   - **an undeclared program that reaches the layer anyway, with no fallback**: what it is told, at which line, and that
     it is never *internal error: linking failed* at exit 2, **nor `cli/header_needs.hero`'s *"no group in this program
     says which library has it"***, which today's code tells such a program after its failed link (route (a)'s
     prototype printed it for `ownhdr` and `platformhdr`): the new diagnostic is asked before that reader. And that
     **an undeclared program asks nothing of the layer and links nothing of it**, `print(1)` at `old`'s cost, warm and
     cold (`00-shared.md` § 4.2's table at `80e2e39ad`, § 4.5's at the base). **That target counts step 182c's walk**
     (`00-shared.md` § 4.5): the walk is
     not the layer's, every build pays it whatever the layer, and robustness ranks above speed (CLAUDE.md
     § Precedence), so state your row twice, the whole build's ratio to `old` and the same with the walk's measured
     share taken out; an undeclared program meets the target when what stands above `old` is the walk alone, and
     the clang log of `ground/repair-calls.sh` shows no call over the members (its count line cannot see a `-M`,
     `00-shared.md` § 4.1, so read the log) and `nm` of the binary no member's symbol.
   - **whether the declaration travels**: a face whose floor declares; a program that imports a declared package and
     declares nothing; a package importing a declared package. Counted (`00-shared.md` § 3.1): following `use` lines
     alone, **78 of Rebel's 115 faces and 735 of its 1,914 modules reach a member floor**, `toml` and `csv` among them
     through `io/hero_stream`, and **30 of `rebel/examples`' 43 programs with a catalogue** do, none binding a member's
     header itself; so if the declaration travels by `use` and a declared build pays R6 whole, a program reading a TOML
     file pays the layer. Price that. And **a declaration that binds nothing** (§ 7.4), `shapes/constonly/` (a group
     naming `hero_net.h` that binds only a constant, which takes no member) among its cases.
   - **the diagnostic** (§ 7.3): its code, words, line, fix and its annotated golden.
2. **How a declared build pays.** R6 whole (every member asked and archived, the linker choosing) is the principle's
   default; say whether (a) guarded **without the lane's fallback** (which the author refuses), (d), (P2) or (f)
   (`00-shared.md` § 7.5) should be adopted for a declared build now or left for later, and price each against R6 on
   `clock`, `audio` and `ui` (`programs/`). A route that changes which members a declared link may take must close
   § 6.1's calls by a test that fails when one is missed, on every leg the CI runs.
3. **The guards, by their tests** (`00-shared.md` § 5), on what you built: run them, say which moved and why; R6's own
   test at `layer.hero:330` is rewritten by the principle (an undeclared link carries no layer): say what replaces it.
   A guard lost is a refusal, never a price.
4. **What the landing owes the tree**, counted: the 32 Rebel floors, the 20 `run` goldens that name a member's header,
   **the ten** that reach one through their own header (`00-shared.md` § 6.2: `ground/own-headers.sh` reads one include
   deep and finds nine; `shapes/own_deep.py`, to any depth, finds the tenth,
   `the-platform-layer-ends-a-wait-at-its-sockets-deadline.hero`), `examples/`'s one,
   `tests/harness/rebel_apps_run.hero`, and the compiler's own tests that build the layer (`grep -n 'test "'
   selfhost/cli/layer*.hero`). Nothing is kept for the old spelling (CLAUDE.md § 2).
5. **The timing table** for what you would adopt, built: `print`, `clock`, `audio`, `ui` (`programs/`), at `-O0`, `-O2`
   and `--sanitize`, warm and cold, three kept runs after a discarded one, interleaved with `old`
   (`.claude/worktrees/rebel-scratch/perf/old/heroes`, read only; **it builds `print` alone**: run by the repair on this
   Mac, `clock` exit 1, `ffi_missing_link` on `hero_time_monotonic_ns`, `audio` and `ui` exit 1, `ffi_missing_header`,
   so `old`'s ratio exists for `print` only) and the base's compiler, on this Mac and in the arm64
   image (there, `old` built from `perf/old/seed/heroes.c` inside the container, as the lane's `run.sh` did); the load
   stated; a run whose `real` stands far above `user` plus `sys` discarded with the reason; the ratios stand. And
   `heroes run tests/harness/latency.hero -- compare`, the contract's instrument for the compiler's own speed: say
   whether the check moves it.
6. **What the emitted C and the seed owe**: say so by `cmp` of `--emit-c` of the compiler, with and without your change,
   the line `hero_program_sources` left out (`.claude/rules/verification.md` § The formatter's probe states that
   exception); and whether the seed must learn the new form before `selfhost/` may use it (the compiler's groups name
   `hero_os.h` 23 times and `hero_compiler.h` 10, the runtime's, and no member's header: `grep -rhoE '^extern
   "hero_[a-z_]+\.h"' selfhost | sort | uniq -c`).
7. **The spec sentence** the home you adopt owes, for the spec-warden to price: say what a reader must be told.

**A route that does not build is not adopted** (`.claude/skills/panel/SKILL.md` § 3c). Keep every program and script you
write in `docs/panel/260-reports/programs/compiler-engineer/` of your copy, sources only, so the critic can run them on
the other platform.

## The report

`docs/panel/260-reports/compiler-engineer.md` of your copy, written as you go and copied to the trunk's same path when you
finish or stop: your verdict (approve, approve with a condition, object, veto), the section it rests on, the home and the
check you would adopt and their measured cost, the table of shapes (shape, platform, the base it ran on, undeclared and
declared, plain, `-O2` and `--sanitize`, exit and the first line of stderr), the guards' tests and what moved, **what a
veto would compel**, what the conservative resolution would have been, every place the shared brief was false or silent,
and one falsifiable prediction with the measurement at which it is checkable. Say what you ran and what you read, apart.

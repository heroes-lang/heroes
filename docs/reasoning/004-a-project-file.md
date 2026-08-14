# 004 — a project file, against the homes that already exist

**Origin.** 2026-08-14 · reasoning session · «Allora ragione sulla possibilità di
introdurre un file di progetto come il file che usa Rust cargo. toml specificare
cose utili al compilatore» — asked the evening `examples/sdl/` landed, hours after
two decisions about *where a header is* went into the record pointing opposite
ways. Read: `design.md` §1.0, §1.2, §1.3, §1.6, §1.11, §1.12, §3.5, §4.1, §4.19,
Part 6, Part 7 item 4, Part 10 steps 14/15/18 · `CLAUDE.md` §4, §6, §10, §11, §13,
§14 · `spec/heroes-spec.md` · `crates/heroes/src/{syntax/externs.rs,modules/mod.rs}` ·
`crates/heroes-cli/src/{cli.rs,cli/table.rs,commands/{compile.rs,toolchain.rs,libraries.rs}}` ·
`docs/panel/{036,047,049,050,055}` · `docs/ROADMAP.md` · `docs/debrief/{DECIDE,SCHEDULED}.md` ·
`DESIGN-LOG.md` · `main` at `eac3e5b`. **No file of code, spec or design modified.**

## The question

"A project file" is one word for at least nine different jobs, and Heroes has
already answered most of them somewhere else. So the session's first job is not
yes or no but **what the word names**, and then, for each part, *what already
houses it* — because a manifest can only take a job from whoever holds it now.

This is note `003`'s method, and it is used here for the same reason: answering
"Heroes has no project file because config files are bad" would be wrong about
seven of the nine, and unresponsive about the other two.

## What the artifacts say

### The nine jobs, and who holds each one today

| # | what a `Cargo.toml` carries | where Heroes puts it | the rule that puts it there |
|---|---|---|---|
| 1 | `[package]` — name, version, licence | **nowhere**; no publishable unit exists | — |
| 2 | `[dependencies]` | `use geom` reads `geom.hero` **beside the file** (`modules/mod.rs:126`) | `design.md` §3.5:759 — *"a v1 'dependency' is a link flag declared in the source next to the `extern` that needs it"* |
| 3 | native linking (`links`, `build.rs`) | `link "sqlite3"` / `package "raylib"` in the group head | `design.md` §4.19; panels 036, 050 |
| 4 | search paths `-I` / `-L` | `--include`/`--library`, plus `CPATH`, `LIBRARY_PATH`, `PKG_CONFIG_PATH` | panel 055; `machine_locked_path`, `syntax/externs.rs:110-136` |
| 5 | `[profile]` — opt level, debug | per-verb defaults (`build` −O0, `run` −O2, `test` −O0) and `-O0`/`-O2`/`--sanitize` | `CLAUDE.md` §10 |
| 6 | `[[bin]]`, entry point | *"the file you compile holds `main`"*; one file operand, maximum (`cli.rs:226-228`) | `design.md` §4.1:773 |
| 7 | `[workspace]`, module paths | beside the file: no path, no search list, no hierarchy | `design.md` Part 7 item 4:2397 — *"no package hierarchy"* |
| 8 | `[target.'cfg(…)']` | `runtime/hero_os.h`; `package` is **one spelling per platform** | panel 049 — the platform axis refused at +144, with a veto |
| 9 | toolchain pinning | `FLAGS` names `-std=gnu11` (`toolchain.rs:73-118`); clang pinned at the fixpoint (`design.md`:2707) | panel 047 — *"`rust-toolchain.toml` pins the Rust, `FLAGS` names the C"* |

**The right-hand column is the finding.** For seven of the nine, what stands
against a manifest is not a preference about file formats: it is that the fact
already has a home and the home has a stated reason. Rows 1 and 7 are the
exceptions — they have no home at all, and both belong to the same unbuilt
milestone (§ Rows 1 and 7 below).

### Principle 0 does not fund it, and the fixpoint is the test

`design.md`:120-124 puts the burden on any proposed form: on the closure list, or
a measured Part 11 effect, or a §1-derived argument the panel accepts —
*"Neither → it waits, regardless of elegance."* The closure list
(`design.md`:134-141) names `modules`, `file I/O`, `args()`, `exit(code)` and
**spawning a process**. It names no manifest, no build description, no dependency
record.

The sharpest form of the test is the fixpoint itself. §3.5:763 retires
`cargo build`/`cargo test` at M-selfhost-fixpoint, after which *"`heroes` is the
only command for compiler development too"*, and `design.md`:142 says that after
the archive `heroes build` and `heroes run` **are Heroes programs**. So the
question has a concrete form: to build the compiler with the compiler, is a
project file needed? Everything that invocation requires is already located
without one — one root `.hero` with siblings by `use`, one whole-program `.c`
(Part 10 step 14), the runtime found by `$HEROES_RUNTIME` then `runtime/` then an
ancestor of the executable (`design.md`:660-668), and clang named in `FLAGS`.

That is not a verdict; it is the artifact reading. The verdict is panel 056's.

### §3.5 is the question's home, and it fixes a verb whose object has no name

`design.md` §3.5 is where the tool surface lives, and it says two things that bear
directly on this. First, the refusal it already carries (`:753`): *"**A second
binary never exists**; neither does a Makefile, a script, or a separate
formatter/test-runner/LSP/package tool."* A project file is none of those four —
it is not a binary, not a script, and a manifest is not a Makefile, which is a
program. **The nearest existing refusal does not reach the thing.**

Second, and this is the genuine gap (`:760`): *"No package manager exists before
modules do; when it arrives it will be `heroes add`/`heroes fetch` — inside the
same binary."* The sentence fixes the **verb** and leaves the **object**
unnamed. `heroes add sqlite3` must write what it learned somewhere, and no
artifact in this repository says where. That is the one place in the record where
a project file has an already-promised job.

### Two decisions of 2026-08-14 point opposite ways about one fact

Both landed on the day the question was asked, hours apart, and both are about
*where a header is*.

**`machine_locked_path`** (`syntax/externs.rs:110-136`, panel 055) refuses an
absolute path written in the program, and its message states the principle:

> `{text}` names a path, and a {what} in a group head says **what the machine
> has, not where this machine keeps it** — the next machine keeps it elsewhere

**`--include`/`--library`** (`cli/table.rs:26-45`, `DESIGN-LOG.md`:298) landed on
the opposite argument, recorded verbatim in the code that implements it:

> an environment variable is **not an artifact a program can carry**: asked for a
> program that binds SDL2, a model produces something that cannot be built, and
> the missing half is an unversioned shell line.

**The two are not obviously compatible, and the second is only half discharged by
what shipped.** `--include /opt/homebrew/include` typed at a shell is not an
artifact a program carries either: it is the same unversioned shell line, moved
from the environment into argv. What the flags did buy is real and is not this —
the fact became **nameable in the tool's own vocabulary**, so it enters the cache
key (`Search::key`, `commands/compile.rs:79-95`, closing a staleness defect the
prototype had) and a diagnostic can point at it (`ffi_missing_header`). What they
did not buy is that the fact travels **with the repository**. The only artifact
that would is a file in it — and that is what `machine_locked_path` refuses, for a
reason that applies to a committed manifest word for word.

Stated as a falsifiable claim rather than a justification (`CLAUDE.md` §11): *a
committed file carrying `/opt/homebrew/include` fails on the second machine in
exactly the way a group head carrying it would.* What would refute it is a
project-file design whose contents are **not** machine paths, or one that is
deliberately not committed — at which point it is `CPATH` with a filename, and the
comparison to make is against the environment channel, not against the flags.

### A project file was not on panel 055's ballot

Panel 055 (`docs/panel/055-where-a-header-is.md`) put four options: **A** a path in
the group head · **B** nothing in the language, the machine is asked · **C**
`--include`/`--library` on the argv table · **D** wait, because `package` may have
met the need. A per-project file is none of them.

The sitting's own reason for refusing A is the one that makes the gap visible. The
llm-ergonomist vetoed A on locality: *"one clang invocation means one `-I` set, so
a path in group 1's head changes how group 2's header resolves."* That is an
argument that a search path is a **whole-program fact wearing a per-group
spelling** — and it says nothing against a whole-program home. Of the three
whole-program homes that exist, the environment was adopted, the flags were
queued and then landed, and the third was never written down. The header comment
of `examples/sdl/main.hero` names the shape of the hole from the other side:
*"There was no third door."*

### §6's Nim rule does not reach it, and should not be cited as if it did

`CLAUDE.md` §6 ends *"a separate package binary: never"*. It is a bare rule: never
argued, never panel-tested, and cited by exactly one artifact
(`docs/ROADMAP.md`:171, for *"never a second binary"*). Nim's config files —
`nim.cfg`, `config.nims` — are not named anywhere in this repository, and neither
is `nimble`. Whatever §6 forbids, a file is not a binary, and the refusal a
reader would reach for first is not available.

### Rows 1 and 7 belong to a milestone with no warrant

The two jobs with no home are package identity and module hierarchy, and both are
M-package-manager's. `docs/ROADMAP.md` puts it at **order 6**, after the fixpoint
and after M-separate-compilation, marked `scheduled, no warrant` — and the file
insists the mark is load-bearing: *"a place in this table is not a warrant"*
(`:60-63`). `SCHEDULED.md`:37 holds the matching open question — *"Do
subdirectories ever arrive, and in what spelling?"* — deferred to that same
milestone, with the unresolved half named as the qualifier rather than the
separator.

So the two rows a manifest would genuinely serve are both behind the fixpoint, and
one of them is the reason §3.5's verb has no object.

## What was settled

1. **"Project file" names nine jobs, and seven already have homes with stated
   reasons.** The answer to the question as asked is not a preference about
   configuration files; it is an inventory.
2. **Principle 0 does not fund it.** It is not on the closure list, and the
   fixpoint invocation — the strictest form of the compiler-need test — is
   locatable without one.
3. **The nearest refusals do not reach it.** §3.5's *"never a Makefile, a script,
   a second binary"* does not cover a manifest; §6's *"a separate package binary"*
   does not either. Nothing in Part 6 or Part 7 mentions one, and the record holds
   no per-project configuration artifact of any kind. Grepped over `design.md`,
   `spec/`, `CLAUDE.md`, `DESIGN-LOG.md` and `docs/`: `heroes.toml`, `project file`
   and `build config` have **zero** hits; `manifest` has **one**, and it is Zig's
   trailing *test-file* manifest (`DESIGN-LOG.md`:75); `Cargo.toml` has **one**, as
   precedent about Rust's inferred closed world (`docs/panel/034`:197). Neither is
   about a project file for Heroes.
4. **§3.5 promises `heroes add`/`heroes fetch` and names no object.** That is the
   one already-promised job, and it sits at ROADMAP order 6 with no warrant.
5. **The argument that bought `--include`/`--library` is half discharged.** A flag
   is not an artifact a program carries any more than an environment variable is.
   The record now holds two decisions from the same day that pull against each
   other, and neither cites the other.

Nothing above is a decision. This session amends no design file, and the verdict
belongs to panel 056, convened from this note in the same session.

## What stayed open → where it was handed off

- **The question itself** — should a per-project file exist, and what may it carry
  — with the ballot drawn to repair panel 055's missing option: **A** a Part 6 row
  · **B** examined and unplaced (panel 039's precedent) · **C** a minimal file now,
  search paths only · **D** deferred to M-package-manager and named there.
  → **`docs/panel/056`**, convened in this session; ratification →
  `docs/debrief/DECIDE.md`
- **Whether `--include`/`--library` fully discharge "an artifact a program can
  carry"**, and whether that argument and `machine_locked_path`'s reason can both
  stand as written. Two decisions of 2026-08-14, neither citing the other.
  → `docs/panel/056` · `docs/debrief/DECIDE.md`
- **`CLAUDE.md` §6's *"a separate package binary: never"* is a bare rule** — no
  argument, no panel, one citation. It should either acquire a reason or stop
  being load-bearing. → `docs/debrief/DECIDE.md`
- **Whether a session that reasons its way to a panel may also leave a note.**
  `docs/reasoning/README.md` § What is and is not a reasoning session says a
  `/panel` session does not qualify *"because each already has its artifact"*. This
  file and `docs/panel/056` hold different things — the inventory and the tension
  here, the proposal and the verdict there — and the README does not currently
  legalise the pair. → `docs/debrief/DECIDE.md`
- **Three stale entries found while verifying** and reported rather than repaired,
  because none is part of the question asked: `DECIDE.md`:182 and :184 are unticked
  though `DESIGN-LOG.md`:298-299 records both decided; `docs/ROADMAP.md` § Status
  and § Done stop at `M-program-corpus` while tag `m-binding-fidelity` and
  `docs/journal/017` exist; `docs/panel/020`:5 still reads `provisional — author
  ratification pending` against its own line 461. → `docs/debrief/DECIDE.md`

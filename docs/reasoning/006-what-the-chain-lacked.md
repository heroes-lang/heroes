# 006 — what the chain lacked

**Origin.** 2026-09-03 · reasoning session · «Allora adesso prima di procedere
facciamo un grosso ragionamento sulla roadmap e anche sulle cose in decide e
scheduled. Vediamo se ha senso ribaltare i prossimi step, rimescolarli … valutiamo
se ci sono degli step che non abbiamo considerato finora, ma che sono importanti
… nuove feature del linguaggio … nuovi tool a supporto del linguaggio o la
pubblicazione ad esempio su brew … il deploy del linguaggio … c'è un esempio la
reflection … un runtime forte … poi io accetto le voci che tu mi dici che mancano
e poi le riordiniamo tutte … in un ordine molto logico, molto sensato». Read:
`spec/heroes-spec.md` in full · design.md Part 2, §3.5, Part 6, Part 7, Part 8,
Part 9, Part 10, Part 11 · `docs/ROADMAP.md` whole · `docs/work/DECIDE.md`,
`docs/work/SCHEDULED.md` · `DESIGN-LOG.md` tail · `docs/panel/OPEN-QUESTIONS.md`,
`docs/panel/018` · `docs/reasoning/003`, `004`, `005` · `docs/measurements/003`,
`007` · `runtime/parts/panic.c`, `stack.c`, `alloc.c` · `selfhost/emit/ctype.hero`,
`selfhost/check/ffi.hero` · `tests/harness/suite_records.hero`,
`tests/harness/main.hero` · `.claude/skills/step/SKILL.md` · `heroes --help`,
`heroes --version`, `heroes measure`, `git status`, `git log`, `git tag`. **No
file of code, spec or design modified.** The session's records — seven chain
rows, the reorder, seven `SCHEDULED.md` items and three amended, one DESIGN-LOG
line, `ROWS` 48 → 55 — are the commit before this note's, on `005`'s precedent.

## The question

"Does the order hold, and what is missing" is two questions, and the second has
the four heads the author named: forms of the language (reflection was the
example), tools around it, a stronger runtime, and the way a stranger installs
it. The method is `003`'s and `004`'s: for every candidate, find what already
houses it and the rule that already rules on it, and only then ask whether it
needs a row. Sixteen candidates were met that way. Seven became rows and nine did
not, and the nine are recorded with the rule that refused them so that they are
not proposed again as if they were new.

## What the artifacts say

### The order had three faults, and each is a measurement

- **The language's only scheduled ruling sat after the code it would shape.**
  M-interpolation-verdict stood at row 41, behind M-core-packages,
  M-web-framework, M-qbe-backend, M-lsp-server and M-vscode-extension. Its own
  ordering paragraph (`docs/ROADMAP.md` § M-interpolation-verdict, *"The cost of
  ordering it here"*) weighed one consumer — the books, which a late form makes
  rewrite chapters in two languages — and not the other: the packages and the
  framework are Heroes written against the spec, and the only body of that kind
  today, the compiler, is **51,788** lines in **178** modules (`find selfhost
  -name '*.hero'`, 2026-09-03). A form that lands after them is a form they were
  written without.
- **Closures had no row.** design.md Part 7 item 1 (`:2441-2443`) has read *"v1.5,
  immediately after the first running program"* since the day it was written; the
  first running program was M-scalars-run, 2026-08-04, and the fixpoint that
  makes Part 7 admissible at all was 2026-08-18. Meanwhile the ROADMAP's
  M-web-framework entry planned middleware *"as a chain of functions, because v1
  has no closures"*, and `selfhost/emit/ctype.hero:375-380` emits a function value
  as a bare C pointer for the same stated reason — a design built around an
  absence that no sitting had ever judged.
- **The manager stood before the packages it would distribute.** M-package-manager
  was row 35 and M-core-packages 36, with the same evening's reason *"`heroes
  fetch` is what makes a package a thing you distribute"*. But the packages' own
  step 13 was `heroes fetch` under the root, and question (ii) of their opening
  sitting decides where a package lives — which is what `fetch` has to know. One
  distributes what exists.

Two smaller faults were found on the way and repaired in the same commit: the
sections under § The milestones did not run in the table's order
(M-isolated-threads stood above M-corpus-depth after the latter was moved ahead
of it), and the preamble's second sentence still said *"the seven closed ones"*
where the headings were eight — its first sentence had been corrected by the
session that scheduled the packages, hours earlier, and the second had not.

### Sixteen candidates, and what already ruled on each

| candidate | what the record already said | outcome |
|---|---|---|
| closures, inline blocks | Part 7 items 1, 12; Part 8 wart 1 (`:2608`); panel 013's watch-list condition (`OPEN-QUESTIONS.md:33-35`): a capturing closure is a record plus a pointer, and the type must know at the C boundary | row 35, **M-closures-verdict** — a decision, not a feature |
| string interpolation | already a row, at 41 | moved to 36 |
| reflection | three mentions, no ruling: `docs/panel/018:99` (*"new semantics with no compiler need"*), `docs/reasoning/003:89` (C4, under Part 6's Ruby row), `docs/reasoning/005:194` (FastAPI *"needs reflection Heroes has not got"*). The Ruby row (`design.md:2395`) refuses run-time dispatch; the emitter already walks a record's fields to generate `eq` and `hash` (CLAUDE.md §7). `examples/json/` is 671 lines; 20 renderers are hand-written across `examples/` | row 37, **M-reflection-verdict** — two questions, run time and compile-time derivation |
| the rest of Part 7 — `alias`, doctests, traits, variant constructors as values, `raw`, `private`, symmetric variants — plus raw strings and printing without a newline | items 5, 6, 8, 9, 11, 14, 16; `OPEN-QUESTIONS.md:74-79`; Part 8 warts 15, 16. No milestone, no date | row 38, **M-deferral-ledger** — a dated verdict or return condition per item |
| conditional compilation, C-width vocabulary (item 10) | questions (vi) and (v) of M-core-packages' opening sitting | no row: already housed |
| `heroes doc` | once in the record, as Part 6's promise (`design.md:2400`, the literate-source row); CLAUDE.md §10's stopping rule; `docs/measurements/003` rider 3 refused `outline` and `explain` on it | row 42, **M-doc-generator**, scheduled, no warrant, **accepted against the recommendation** — the rule is its opening's first question |
| a panic that says where | `runtime/parts/panic.c:21-25` prints `panic: <msg>` and aborts; `stack.c:202-213` already names the function through `dladdr` on POSIX; the emitted C carries `#line` | row 43, **M-panic-location**, soundness lane, §1.12 |
| metrics 2 and 4 | never run (`docs/measurements/007:24-25`; the gate's checklist); tasks author-written (panel 011, decision 2026-08-24) | row 44, **M-thesis-harness** — the instrument, over the packages' HTTP client; a seed of tasks at its opening by author decision |
| the incremental frontend | a `SCHEDULED.md` item asking *which milestone does it*; `heroes check` on the compiler ~8 s (2026-08-26) | homed in **M-lsp-server**: a server re-checks on every save |
| installing with one line | `git clone` plus one clang line (`site/src/html/index.html:135-137`); `heroes --version` is `heroes 0.0.1`; every tag is a milestone's name | row 50, **M-install-channels** — tap, manifest, flake, image, all from the seed, plus a version scheme |
| prebuilt binaries | `heroes` invokes clang on every build (`heroes doctor` checks for it) | none: a binary alone is a decoy; the channels depend on a C toolchain instead |
| a REPL | Part 6's interpreter shapes (`:2395`, `:2397`); `heroes run` is the dev loop (§3.5) | none |
| a web playground | Part 2 (`:590-592`) and Part 9 (`:2746-2751`): wasm breaks the FFI premise | none |
| cross-compilation | CLAUDE.md § Commands: the three platforms are measured on real machines before a commit; `design.md:726` keeps `zig cc` as a note | none |
| typed debugging | Part 2 (`:594-596`), a stated v1 non-goal; lldb-dap composes with the DWARF the `#line`s already produce (§ M-vscode-extension) | none now; admissible after v1, not urgent |
| out-of-memory | `runtime/parts/alloc.c:75-79`: `malloc` failure is a clean `hero_panic("out of memory")` | nothing owed |

### What *"un runtime forte"* resolves to

Read against the runtime's eighteen parts, the phrase names three things that
already have homes and one that did not. Threads are M-isolated-threads, open.
The services a program reaches for — `os`, `io`, `time`, `net` — are
M-core-packages' packages, over C. Leaks are `hero_runtime_check_leaks()`, and
out-of-memory is a clean stop. What had no home was the one abort that said
nothing about where, and that is M-panic-location.

### One question the packages' manager did not know it owed

Pinning what `fetch` brings — Go's `go.sum`, Cargo's lock file — is a per-project
file, and CLAUDE.md §10 admits no fourth input class; panel 056's three return
conditions are the only amendment path. Neither M-package-manager's entry nor
`005` had asked how a fetched package is fixed to a version without such a file.
The question is now in that entry, for its sitting.

## What was settled

The order, rows 33–51, and the reason for every position — in `docs/ROADMAP.md`
§ Who scheduled what and in the DESIGN-LOG line of the same evening. The
author's decisions, each put with a recommendation: all seven rows accepted, one
against the recommendation; the packages before the manager; the four rulings on
the language immediately after the two open milestones and before the packages;
a seed of metric-2 tasks written at M-thesis-harness' opening, the bulk staying
at M-guide-book. Two placements were the session's own and stand unless refused:
M-qbe-backend after the tools, because every ruling above it may move the IR;
the incremental frontend at M-lsp-server. Spec headroom the evening this was
decided: **378** tokens (`heroes measure`, 3718 of 4096) — the budget the four
rulings will have to share.

## What stayed open

- **The stopping rule for `heroes doc`** → M-doc-generator's opening, as its first
  question (`SCHEDULED.md`).
- **Pinning without a per-project file** → M-package-manager's sitting (ROADMAP
  entry).
- **The C-ABI boundary of a capturing closure**, and the count of one-line
  helpers passed as values, which nobody has taken → M-closures-verdict's
  opening (`SCHEDULED.md`).
- **Whether compile-time derivation is a form or a refusal** → M-reflection-verdict,
  before `encoding/json` is written.
- **A version scheme** → M-install-channels, written together with the gate's
  compatibility paragraph.
- **How many seed tasks, and which** → the author, at M-thesis-harness' opening.

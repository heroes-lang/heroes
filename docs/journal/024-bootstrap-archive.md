# 024 — M-bootstrap-archive: the third language dies

**Closed 2026-08-19**, tag `m-bootstrap-archive`.

## Goal

design.md:82 ends the fixpoint paragraph with a claim the repository cannot yet
make: *"the final picture is Heroes → C → native binary, with **no third
language anywhere**."* There are 4,719 lines of Rust tests and a whole Rust
compiler in `crates/`, and the milestone's job is to make that sentence true —
`crates/` → `archive/bootstrap-rs/`.

**The move is the last commit, not the first**, and panel 085 R4/R5 said why: five
things die with the bootstrap and each one has to have a successor *before* the
directory moves.

| what dies | successor | step |
|---|---|---|
| `differential.rs` — its oracle **is** the second compiler | `tests/emission/`, 142 blessed emissions | 1 |
| `heroes measure` — the §1.6 budget's only enforcer | `selfhost/measure_*.hero` | 2 |
| `measure/gate.rs` + `measure/spec.rs` — 435 lines, all `cfg(test)` | `tests/harness/suite_spec.hero` | 3 |
| `heroes mutate` — the thesis's PRIMARY metric, run on every commit | `selfhost/mutate_*.hero` | 4 |
| CI's four cargo steps — the workflow becomes empty | the seed leg, clang and nothing else | 5 |

## What surprised

**The whole C, not a hash, and the seed had already decided it.** Step 1's oracle
stores 5.4 MB of generated C — 142 files, 162,878 lines — where a manifest of 142
hashes would catch exactly the same changes for 142 lines. The argument that
settled it is not size but *what a failure can say*: after the archive there is no
second compiler to ask, so a hash reports "something moved" and nothing else,
while stored bytes make `git diff` the explanation. The repository was already
paying this price in the largest possible denomination — `seed/heroes.c` is 22 MB
of generated C for **one** program — so the decision had a precedent with four
times the cost.

**A language that cannot spell a byte forced a better tokeniser.** The Rust
`measure` decodes every BPE token from base64 and keys its table on the bytes.
Heroes cannot: `s[i]` gives a `u8` and nothing turns one back into a `str`, so a
decoded table has nowhere to live. The port keys the table on the spelling it
arrives in and encodes the **lookup** instead — and the result is not a
compromise: both instruments agree with the bootstrap to the token on the real
spec (3440 and 3512), and the three published vectors match through both
compilers. What made it sound was measurable rather than arguable: both vendored
tables spell every token in padded standard base64, which is what `spelled`
writes.

**`_` is forbidden on a variant, so thirteen mutation operators name twenty-one
expression forms each.** That reads like a cost and is the opposite. The Rust has
`_ => continue` in the same place — and `ast.hero`'s own note records that this is
where the lexer's `nullptr` defect came from. In the port, the day a twenty-second
expression form arrives, every operator stops compiling instead of quietly not
measuring it.

**The net was still holding the bootstrap's hand.** Nine of the harness's own
self-tests named `./target/debug/heroes` as a literal — in the net built
specifically to survive that path's deletion, and `suite_golden.hero`'s stub
compilers exist with a comment saying exactly why that is wrong. They now ask the
machine (`shell.some_compiler`: `$HEROES_COMPILER`, then the seed-built compiler,
then the bootstrap), which is the archive's own order.

## What broke and why

**`panic: string slice splits a character`, on the spec itself** (step 3). The
first draft of `spec_text.hero` walked bytes and sliced them one at a time, which
is the obvious port of Rust's `char_indices` — and the spec's em dash is three
bytes. Slicing one of them aborts, by design: `slice` refuses to split a
character (spec:148) while indexing never does. The module now takes `[str]` from
`chars()` where the Rust takes `&str`. What makes this a *good* failure is that
the alternative is a silent miscount, and `tests/harness/strings.hero`'s module
doc had already written the warning for this exact shape — the rule existed and
the new file did not read it.

**A comment that nothing had ever executed** (step 4). `edits_typo.rs` says the
one-character slip turns `total` into `totl`. It does not: the code drops index
`len / 2`, which is the second `t`, so it produces **`toal`** — measured on *both*
compilers with a corpus built for the purpose. `totl` is the illustration
design.md:1027 and `harness/mutations/operators.md` use for the class, one index
over and equally plausible, so the class was right and only the sentence was
wrong. The comment is corrected and the code is not: changing the code would move
every number in `docs/measurements/002` and `007`, the operator is data, and the
illustration is prose. CLAUDE.md §11's shape, found by writing the function a
second time in another language.

**A new file over §11's ceiling, caught by the check written for it** (step 4).
`mutate_edits.hero` reached 338 code lines against the 300 ceiling.
`suite_layout.hero` holds seventeen *existing* files to the length they measure
today — a stop on growth rather than an approval — and a brand-new file has no
business on that list. The split was real rather than arithmetic: everything that
answers a question about **one node or one span** went to `mutate_sites.hero`, and
it had to happen anyway, because `mutate_typo` and `mutate_edits` each needed one
function from the other and Heroes refuses module cycles.

## The sitting: where a ledger lives when its file goes away

Panel 086 was convened for one sentence and found a class. `gate.rs`'s doc comment
held the **SPEC_TOKENS ledger** — 38 rows, one per spec amendment, each naming the
delta and what paid for it — and two design.md Part 1 sentences named that file as
its *live* home. Three seats, three measurements, one answer:

- the ledger is **48,996 bytes**, max row 3,379 — 81% of the file it lived in;
- it measures **12,757 tokens, 3.6× the spec it guards**, which is what killed the
  option of moving it into the harness module that enforces the number: CLAUDE.md
  §11's relaxation for tests rests on *"a case is read one at a time"*, and this is
  not a case, it is a wall;
- five of six comparable projects split figure from history (CPython's
  `Misc/stable_abi.toml`, Go's `api/go1.*.txt`, Rust's `tidy`, TeX's
  `errorlog.tex`, Linux's checkpatch), and **leaving the citation pointing into an
  archive is the failure Linux built `make refcheckdocs` to catch**.

And all three seats named the same missing mechanism, independently: *nothing had
ever checked that the ledger's newest row and the enforced constant agree.* The
coupling was **adjacency** — two lines apart in one file. The warden supplied the
base rate for what adjacency is worth here: `REGISTRY_TOKENS` drifted +85 across
eight milestones with no commit naming a delta.

So the ledger became `docs/measurements/010-spec-budget-ledger.md` and the check
became real: row count, newest figure, and today's measurement, all three or red.
The sitting also narrowed a sentence rather than repealing it — *"a figure that
lives in one place cannot die in another"* is about **that paragraph's** prose, so
the rule is that **a number may live in a second place only if it is checked or
dated**. A row is dated. A pin is checked.

## What the new check found in its first hour

`records/citations` — panel 086 R7, the anchored dead-citation rule — went green on
the day it landed and **that was the defect**. It anchored on *existing* directory
names, so the moment `crates/` became `archive/bootstrap-rs/`, every citation of
`crates/heroes/src/measure/gate.rs` stopped being a path claim: the rule was
self-adjusting in the wrong direction, and it reported six passes over exactly the
citations it was built for. A file extension is a fact about the token that
survives the deletion of everything around it, so the rule now accepts either
anchor — and with that, it found **nine dead citations that predate the archive**:
`emit/mangle.rs`, `ir/print.rs`, `types/holes.rs`, `ir/verify.rs`, `ir/mono.rs` and
`emit/ffi.rs` in design.md and CLAUDE.md, written relative to a source root the
reader was assumed to be standing in. None of them ever resolved from the
repository root. Nobody had noticed, because nothing had looked.

## What landed, and what carried forward

**Closed 2026-08-19, tag `m-bootstrap-archive`. There is no third language.**

    clang -I runtime seed/heroes.c runtime/runtime.c -o heroes    3.4 s
    ./heroes test selfhost/main.hero                              482 tests
    ./heroes run tests/harness/main.hero -- ./heroes               838 checks

`crates/` → `archive/bootstrap-rs/`, and design.md:82's *"no third language
anywhere"* is now a fact rather than a plan. What the milestone had to build first,
because the archive would otherwise have made it unobservable:

| landed | measured |
|---|---|
| `tests/emission/` — the differential's successor | 142 programs, 5.4 MB, **both compilers green against the same bytes** |
| `heroes measure` in Heroes | 3440 / 3512 / spread 72 — **identical to the bootstrap's**, three published vectors matching |
| `suite_spec.hero` — the §1.6 gate | 8 checks, green against both compilers |
| `heroes mutate` in Heroes | 538 mutants, 512 (96%) / 414 (78%) — **byte-identical output**, `--survivors` included |
| the seed, regenerated with both verbs inside it | 22,025,792 bytes, **fixpoint byte-identical**, 15m41s |
| CI without cargo | four steps deleted, four added, none of them Rust |
| `docs/measurements/010` + the lock | 38 rows, three-way agreement checked |
| `records/citations` | 47 anchored occurrences, 9 dead ones found and repaired |

**The port is 37,137 lines of Heroes across 153 files** (30,569 before the first
test block), against 25,482 non-test lines of Rust it replaces.

**What carried forward.** Two decisions are the author's and are in `DECIDE.md`
with proposed wordings: CLAUDE.md §4's panel trigger still names the archived tree
(and its brace form is the one citation shape the new check cannot read), and **the
Windows CI leg now covers the toolchain and nothing else** — it ran the cargo
steps, and the Heroes steps cannot replace them because `selfhost/cli_io.hero`
binds `unistd.h`. That leg prints a warning on every run rather than staying
quietly green, because a job that tests nothing is what §9 calls a decoration.
One item goes to `SCHEDULED.md`: the `ORDER:` marker gap panel 086 R4 measured —
4 of the bootstrap's 13 marks have no counterpart in the port, so the live
inventory under-reports until they are marked.

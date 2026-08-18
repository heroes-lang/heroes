# 024 — M-bootstrap-archive: the third language dies

**Open.** Sections grow per step; the closing block is appended at the close.

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

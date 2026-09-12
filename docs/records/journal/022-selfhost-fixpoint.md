# 022 — M-selfhost-fixpoint: the seed, and four defects the fixpoint could not see

Closed 2026-08-18, tag `m-selfhost-fixpoint`.

## Goal

Two things, in the order the ROADMAP puts them: **the seed**, so that a clean
checkout with nothing but a C compiler can build a working compiler, and then the
archive of `crates/heroes` — *"the third language dies here"*.

The first landed. `seed/heroes.c` is 21,008,434 bytes of C, one clang line builds
it in 3.7 seconds, and the compiler it produces re-emits that same file byte for
byte. The second **did not land, and the reason is the milestone's real content**:
the archive turned out to be sitting on a defect that made the self-hosted
compiler unusable outside this repository, and on the discovery that the emitted C
of the two compilers had never actually been compared.

## What surprised

**The port had reversed a ratified panel decision, and the reversal was invisible
from the only place anyone ran it.** `selfhost/main.hero:45` read the standard
library from `crates/heroes/src/library/source.hero` at run time, through
`.default("")` — while the Rust it replaces is `include_str!`, and
`source.hero`'s own header states panel 028 R3's ruling: *"there is no discovery,
no `$HEROES_LIBRARY`, and no decoy"*. Inside the repository the path resolves and
everything works. One directory away, the library is silently empty and `range`,
`map`, `filter`, `fold`, `read_file` and `args` are refused **on the author's own
line** with `builtin_shape`, exit 1. The bootstrap accepts the same program at
exit 0. A compiler that only works in its own source tree is not a compiler, and
the archive would have made that permanent.

**A differential that compares streams cannot see an artifact.** M-selfhost-port's
instrument was five commands over 257 inputs on both streams — 2,570 byte-exact
comparisons, and it found three defects in the *bootstrap*. What it cannot reach
is `--emit-c -o file`, which writes an artifact rather than a stream. So the one
output the finish line is *about* had been compared for exactly one input:
`selfhost/main.hero`. Everything the compiler's own source does not happen to
contain went uncompared — and `selfhost/` has no fixed-array field, no hyphen in a
module name and no float literal. Measured the first time anyone asked: **26 of
127 programs byte-identical**. After three repairs, 127 of 127.

**A note the port wrote to itself, unpaid, is the same shape as the note the
bootstrap wrote to the port.** Last milestone's final obstacle was three
`// ORDER:` marks the bootstrap had left for the port. This milestone's was
`# PORT note: the base spelling routes through storageless.fixed_text when that
module lands`. The module landed. The call site did not change. And the note's
closing clause — *"the same bytes for every case the tests cover today"* — was
true only because **no test covered it**, which is a premise that expires in
silence while the sentence around it still reads as correct.

**C's `&&` does not protect an ill-formed operand, and that cost a whole exit
code.** A field assertion asked a member's sign with `((__typeof__(place))-1 <
0)`. On a member the header declares as `char[1024]` that expression is not
false — it is *ill-formed*, and C type-checks every operand of `&&` whether or not
an earlier one is false. So clang emitted an error that was **not this assertion
failing**, the FFI error mapping could not recognise it, and the compiler blamed
itself for the author's `extern`. Two candidate escapes were measured and both are
closed: neither `__builtin_choose_expr` nor `_Generic` protects an unselected
operand from type-checking on this clang.

**The panel's most useful verdicts were the ones that contradicted the brief.**
The brief priced the seed's storage on one commit and got the arithmetic exactly
backwards: git delta-compresses plain text and cannot delta a compressed stream,
so gzip saves 0.20 MiB once and costs 1 to 2.4 MiB per refresh — 2.74 MiB raw
against 5.78 MiB gzipped by the third stored copy. And the ROADMAP's own citation
of ancestors was wrong: Go 1.4 is hand-written C that anyone can **patch**, which
is why Go could respin its "frozen" seed twice; generated C cannot be patched at
all, so the refresh trigger has to be executable rather than remembered. The real
ancestors are Nim's `csources` and Pascal-P4's 1976 porting kit.

**And two of a judge's own findings did not reproduce.** The seat that compiles
reported a working `readdir_r` binding and a reachability-dependent FFI check;
four attempts today were blocked before either could be observed, because a group
record with a `char[1024]` member **cannot be constructed** — a fixed array is
built with a literal of exactly its length. The record says *could not reproduce*
rather than carrying the claim, and the question underneath it is the better one.

## What broke and why

| symptom | cause |
|---|---|
| `range` refused on the author's line, one directory away from the repository | the port read the standard library from `crates/` at run time, through `.default("")`, where the bootstrap embeds it — panel 028 R3 reversed in silence |
| `HEROES_RUNTIME=<dir>` did nothing under the port, while `--help` promised it | the port read a *file* of that name, on the note "the language has no getenv" — which is one `extern "stdlib.h"` away |
| `use of undeclared identifier 't20'`, `heroes build` exit 2, on three ordinary programs | `emit_container.read_element` never routed the subscript base through `storageless.fixed_text`; a fixed array is never declared, so the fallback's temporary does not exist |
| 81 of 127 programs named `while-loop.c` in a `#line`, a file nothing creates | the port passed `stem_of` where the bootstrap passes `module_of`, eleven lines under a comment that already said which one it is |
| 19 programs spelled `2.5` as `2.5` where the bootstrap writes `0x1.4p+1` | the port carried the literal's text because it cannot parse a decimal — and C's `atof` can, which the filed item's three options had not considered |
| `x: f64 @ 1_0.5` printed **0.0** at exit 0 | `parse::<f64>().unwrap_or(0.0)` under the comment "the parse cannot fail: the lexer already accepted the shape"; the lexer accepts `_` on both sides of the point and Rust's parser accepts none |
| `d_name: i8` against `char d_name[1024]` was `internal error`, exit 2 | the sign conjunct negated the member's own type, which is ill-formed rather than false on an array — and `&&` type-checks every operand regardless |
| `call to undeclared function 'HERO_C_UNSIGNED'` in the first repair | that macro's preamble is emitted only where an `extern` **function** needs it, and a struct-only group emits none |

And one of mine, recorded because a milestone that lists only its wins is not a
record: the panel's working-tree freeze was **broken during the sitting** — not by
a judge, by a concurrent session committing to `site/` — and the brief's file
count expired 39 minutes after it was written, which two judges noticed
independently. The number was true when written. That is exactly the failure mode
the rule about premises describes, arriving inside a document written to enforce
it.

## What landed, and what carried forward

`seed/heroes.c` is a release artifact **in git**, raw, 21,008,434 bytes, costing
**2.50 MiB** of pack (the repository went 6.56 → 9.06 MiB, measured before and
after). `crates/heroes-cli/tests/seed.rs` builds it from `git archive HEAD` — a
clean checkout, not the working tree — with the one documented command, checks its
`HERO_RUNTIME_ABI` stamp against the runtime's as numbers, and runs a program
through it end to end: 6.7 seconds. `crates/heroes-cli/tests/differential.rs`
compares the two compilers' emitted C over 140 programs in 19.5 seconds, and it
was watched failing before it was trusted.

**The archive did not happen and now has a milestone chain of its own**:
`M-harness-port` (the net in Heroes — 4,279 Rust lines measured, 2,500–3,760
Heroes lines predicted, no compiler lines and no new surface) and then
`M-bootstrap-archive`, whose first row is a repair rather than a move, because the
differential dies with the bootstrap and its expectation *is* the bootstrap.

Carried forward in `DECIDE.md`: panel 085's ratification, the mirror of the
field-assertion hole (still exit 2, three candidate repairs, and the obvious route
measured closed), the two dirent findings that could not be reproduced, the `5.`
lexing question, and metric 2 — whose own words made it due at this milestone and
which is blocked on author-written tasks by design.

One correction to the record, measured rather than recalled: `selfhost/` has **447
test blocks**, not the 27,230 this journal's predecessor and the ROADMAP both
carry. `heroes test selfhost/main.hero` prints `447 tests, all passed` in 10.3
seconds. The live status is corrected; the dated record is not rewritten.

# 021 — M-selfhost-port: the port, and the fixpoint it was aiming at

Closed 2026-08-17, tag `m-selfhost-port`.

## Goal

Carry `crates/heroes` and `crates/heroes-cli` — 48,706 lines of Rust — into
Heroes under `selfhost/`, file by file, with the goldens and the corpus as the
net. The measure of done is not "the files exist": it is the **fixpoint**, the
generated C from the two compilers compared byte for byte, because that is the
one test a port cannot pass by accident.

It landed: **34,812 lines across 143 modules, 27,230 tests**, and

    373f454482831a9fc23942b8675cfc68e450a61cb49f34f45b7dbeac81173e63  gen1b.c  (bootstrap, Rust)
    373f454482831a9fc23942b8675cfc68e450a61cb49f34f45b7dbeac81173e63  gen3.c   (heroes, in Heroes)
    373f454482831a9fc23942b8675cfc68e450a61cb49f34f45b7dbeac81173e63  gen4.c   (the stage-2 compiler)

20,886,539 bytes, 724,245 lines, three compilers, one hash. Apple clang
21.0.0, arm64-apple-darwin25.5.0.

## What surprised

**The port refusing its own source was the most productive event of the
milestone.** `heroes-selfhost check selfhost/checker.hero` answered `record Ast
contains itself` — a cycle that does not exist — and that one false diagnostic
opened a vein of five defects in the port and four in the bootstrap. A compiler
pointed at its own code is a corpus no test suite reproduces: 34,812 lines
holding every shape the language has, in the proportions real code has them.

**One trap accounted for five of the port's defects, and it is a shape the Rust
cannot have.** A `continue` in a match arm is a loop jump, so it skips a
hand-maintained index at the bottom of the loop — where Rust writes
`.iter().enumerate()` and cannot. It cost a false `no_size`, every C type name
under the previous declaration's index, a mono table read from the wrong
declaration, misindexed extern assertions, and a verifier that refused **every
program containing a `.must()`** by counting only the aborts.

**A differential against the bootstrap is a stronger instrument than a test
suite, and it is nearly free.** Five commands over 257 corpus inputs on both
streams is 2,570 comparisons, and it found what 27,230 unit tests and 573
bootstrap tests did not — including three defects in the *bootstrap*: a caret
that swallowed a trailing comment at three sites adjacent to a repair made five
days earlier for statements only. One of them underlined the `#~` annotation
that §9 requires precisely to check that caret.

**The last thing between the port and the fixpoint was a note the bootstrap had
written to it.** Three map walks reached emitted output through `keys()`, which
the spec says gives no order and measurably does not (50, 3, 97, 12 in gives
`97 50 3 12` out). Each site carried `// ORDER: ascending TyId — so the Heroes
port owes an explicit sort`. Unpaid, they produced 20.8 MB of C whose *lines*
matched exactly and whose *sequence* did not, on 3,330 of 724,197 lines.

**Cyclic Rust module groups become one Heroes module, and the language decides
where the seams are.** Fifteen knots, the largest `ir_lower` at 1,685 lines and
the `fmt` knot at 865 from nine Rust files, each carrying the map CLAUDE.md §11
requires — because `module_cycle` fires on the `use` edge whatever it carries,
so a recursive-descent grammar cannot be split at all.

## What broke and why

| symptom | cause |
|---|---|
| `record Ast contains itself` on a file with a function above two same-typed fields | `.function_decl => continue` skipped `collect`'s own index; the shifted `from` collided with the resolver's correct `to` |
| every program with a `.must()` refused as ill-formed | `ir_verify` counted only aborts, so `index + 1` was 1 and the check fired on any block with more than one instruction |
| the author's unclosed `(` reported inside the library at exit 2 | the port lexed the *concatenation*, so `open_brackets` and the indent level crossed a file boundary and the one eof landed past the root |
| a file's last declaration reported at the library's line 26 | `here_or` was a stub that discarded its fallback for the whole port |
| one trailing blank line on every artifact | `print` adds exactly one newline and every blob already ends with one; on `--emit-c` that byte is the fixpoint |
| `sort(ps)` underlined 46 columns for 8 (bootstrap) | `previous_significant_span`'s repair reached statements and not the call, the method, or a fixed-array type beside them |
| 3,330 sequence differences in the emitted C | three `keys()` walks the bootstrap had marked as owing an explicit sort |

And two measurement errors of mine, recorded because a milestone that lists
only its wins is not a record: a hole-report defect announced that did not
exist (my own `head -3`, shifted by a real blank-line defect), and this
fixpoint first compared against a **stale** emission from before the version
constant moved, turning 0 differences into 40,238. Both are §1's shape — the
instrument lied and I read it out loud before checking it.

## What landed, and what carried forward

`selfhost/` is the whole compiler: lexer, parser, resolver, checker, IR with its
verifier, monomorphisation, the ownership pass, the C backend, the FFI
assertion family and its error mapping, discovery, diagnostics, the printer and
the formatter, and the CLI — 143 modules, 27,230 tests, zero `PORT-DEBT`. The
bootstrap keeps its 573 tests green and gained four repairs the port found.

**The fixpoint is measured, verified three ways and stable.** What
M-selfhost-fixpoint still owes is not a measurement: the seed test (B.c built
from a clean checkout with nothing but a C compiler) and then the archive of
`crates/heroes`, in that order, because the row states the gate in absolute
terms. Queued for the author, with the numbers above as the brief.

Carried forward in `DECIDE.md`: the self-compilation cost (298 s for the
frontend over 143 modules, 616 s to emit — §13's named case, routed to a panel
with the profile still owed), the four one-line provisional answers the CLI
needed (the library's text, `HEROES_RUNTIME` as a file, the compiler
fingerprint, the word width), the float-literal divergence, and the
`continue`-as-no-op spec question this milestone paid for six times.

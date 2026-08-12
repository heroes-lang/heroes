# M-header-constants — the number leaves the file

## Goal

An author's question, asked while reading `examples/curl/main.hero`: *why must
constants that are read from outside be set by hand — can `extern` not carry them
too? It is important, full stop.*

The record had already answered it and then forgotten. Panel 018, ratified
2026-08-04, listed the spelling twice — *"the extern-global spelling is committed
at the FFI milestone"*, and in its watch list *"`extern constant name: type` or
equivalent — at the FFI milestone at the latest"* — and its ffi-pragmatist
predicted, as prediction 4, that *"the SQLite binding needs an extern-global
line"*. M-ffi-ladder closed with five constants copied by hand and the prediction
unscored. A commitment with a deadline had passed its deadline in silence, which
is the same failure mode this language is built to make loud.

Two things were on the record beside it, and neither had been noticed. §4.19
promised the capability in a sentence that was **false** — *"macros, `inline`
functions and `#define` constants are reachable because the C compiler sees the
real header"*, which is true of the C compiler and false of the author, who had no
form to name one. And the compiler's own library paid for the gap:
`library/source.hero` compared a status against `0`, `1` and `2` because it could
not name `HERO_OS_OK`, and `runtime/hero_os.h` says why those numbers are small —
*"small integers rather than errno"*, chosen so a Heroes reader could carry them
across by hand.

Delivered: a `constant` inside an `extern` group, with no body, whose value is the
header's. Five commits, one panel, spec 2560 → 2588.

## What surprised

**The measurement came before the cure, and it said 5 of 267.** The instrument
did not exist: `heroes mutate` had eleven operators and none touched the digits of
a number, so metric 3 could not see a mistranscribed constant at all. The twelfth,
`typo-digit`, moves the last digit of a `constant`'s value by one, and on
`examples/` it found **five sites, zero killed** — all five owned by a C header,
with no sixth site anywhere. That is the number the panel judged.

Then the panel's own spec-warden pre-registered a prediction and it was **scored
inside the same sitting**, which has not happened before in this record: widen the
operator to every int literal and the class is **267 sites**, of which five have an
authority. So this milestone reaches 1.9% of the digit class, and the other 262 are
unreachable by anything — `print(9)` becoming `print(8)` is a different program,
not a wrong one. The claim is smaller than it first looked and it is true, which is
better.

**The AST answer is the opposite of the obvious one.** A sixth `DeclKind` looks
safer by this project's own method: a new variant breaks every exhaustive `match`
at compile time. The compiler-engineer built it and found the one place that is
not a `match` — `ir/exprs.rs`'s `is_constant` is a `matches!`, so a new variant
would leave it answering *false* and lower a header constant to `Op::FuncRef`, a
function pointer where an `int` is wanted, silently. The variant that breaks more
call sites is worse at the single site that decides correctness. `DeclKind::Constant`
was widened instead, and the six sites that kept compiling were read one at a time.

**Two judges met at the same fact from opposite ends.** One was compiling C, the
other writing Rust, and both landed on the accessor's name: unmangled, `int64_t
SQLITE_OK(void) { return SQLITE_OK; }` has the macro eat its own definition. So
CLAUDE.md §7's rule that `extern` names pass through unmangled gains its first
exception, and the reason is exact — a **linker** name must survive the mangler, a
**preprocessor** name must never appear. Same rule, opposite consequence, because
the two names are consumed by different tools.

**A judge given only the specification found a silent error in the
specification.** Asked about constants, the llm-ergonomist kept pointing at a line
that has nothing to do with them: `function sqlite3_open(path: cstr, out: ptr)`,
four lines above the rule *"a C out-parameter is an `@` parameter"*. Its diagnosis
of the consequence was wrong and the truth is worse. Copied verbatim into a
program, that example compiles clean, links, runs and **exits 0** printing
`rc: 21` — `SQLITE_MISUSE`: the address of the handle was never passed, no database
was opened, and the program told the shell it had succeeded. The document's one FFI
example taught a program that silently does nothing.

## What broke and why

**`emit/ffi.rs` was reading clang's echo of the source line.** A misspelled
`extern` name reported `` `sqlite3_openn` does not return `int");` `` — the
garbage is text from the echoed line, which repeats the assertion's own message
verbatim and so matched the marker. CLAUDE.md §7's claim that the mapping *"reads
only the assertion messages this emitter itself writes"* was therefore **already
false**, and the diagnosis was wrong anyway: the name does not exist, and
correcting the result type cannot fix that.

The cause is the shape this project keeps paying for. It worked by **ordering** —
clang prints the message before the echo, and the duplicate was dropped by span —
which is a premise about another tool's output doing the work of a fact about the
line in hand. The repair is the fact: a line is one of ours only if it also says
`static assertion failed`. A name no header declares became its own diagnostic,
carrying clang's own typo correction as a `guess`, never `certain`, because it is a
search over the header's names and not a fact about this program.

**Three walks were asking `FnKind` a question only the declaration can answer.**
An `extern function` lowers to `FnKind::Extern`; an `extern constant` lowers to
`FnKind::Constant`. Every filter written as `kind == Extern` encoded *"only a
function comes from a header"*, and its failure is silent: the `#include`
disappears and clang blames the compiler for a name it was never given. The
`#include` walk, the `link` walk and the IR verifier's bodyless rule now read the
declaration. `tests/golden/run/ffi-constant.hero` carries a second group holding
constants and no function, which is the shape that catches it.

**Two silent acceptances, found by reading the sites that did not break.**
`--dump-scopes` printed `constant` for both kinds where it distinguishes `extern
sqrt` from `function main`; `--dump-ast` printed a constant with no group head at
all while a signature carries one. Both now print the header, from one writer.

**A pre-existing `fmt` defect, fixed before the examples moved onto it.** A
comment above a group's *first* member was being moved down onto the second,
because a first member's comment rules key on the head's line — they must, or
§4.1's blank-line rule fires on a gap the head created — and nothing then flushed
the comments in between. Reproduced with functions alone. It mattered because the
migration moves a four-line comment into exactly that position, and the
compiler-engineer's prediction was that it would move unless this was fixed first.

## What landed, and what carried forward

The form: `constant NAME: type` as a group member, no body, and one rule — inside
a group a declaration is a **signature, not a definition**, so `function` gives up
its code and `constant` gives up its value. Three lines of generated C per
constant: the existing `HERO_RET_*` assertion asked of a token instead of a call
(17 of 18 real header constants pass it unchanged), a `__builtin_constant_p`
assertion so a C *object* is refused, and a mangled accessor.

Refusing the object case appears to be new. Nim, Swift and Go all accept naming
`errno` or `stdout`; Zig refuses it by accident, as a comptime failure its own
tracker calls a bug. Heroes refuses it as a rule, because a zero-argument accessor
over `(*__error())` returns whatever it holds at the time of the call, which is the
mutable global §4.2 forbids arriving through the back door.

Two of the seven boundary types are refused in the checker — `str`, because this
runtime builds every one of them, and `()`, because a constant names a value.
`bool` deliberately is **not**: no C11 header constant has type `_Bool`, but that
is a premise about the world, and the per-constant assertion asks the token instead.

The corpus: five copied numbers gone, `typo-digit` at **0 sites**, and the four-line
comment explaining `CURLOPTTYPE_STRINGPOINT + 2` gone with the number it explained.
The compiler's own library names `HERO_OS_OK` and `HERO_OS_NOT_FOUND`, so
renumbering them in the header cannot leave the Heroes side behind.

Carried forward: **the spec-warden's objection, un-withdrawn.** Its own withdrawal
condition was that header constants turn out to be the majority of the
killed-nothing digit class, and they are 5 of 267. What carried the decision is the
author's instruction, and the panel file says so rather than manufacturing a
consensus. Its other two points were adopted rather than answered — the wording
(v-B, 7 tokens below the sitting's own draft) and the repair of design.md's false
sentence, which was owed whatever funded it.

Also carried: `heroes mutate` does not check that the base program compiles, so
pointed at a corpus that does not it reports a perfect defence — measured at 7 of 7
"killed" on `library/`, where the pristine file does not type-check at all because
the library *is* the built-ins. And the general form of the spec test: only the FFI
example is compiled from the document, because most of the others are fragments.

# Two sittings refused two routes, and the answer had been spoken all along

2026-09-15. Panels 151 and 152, both on `docs/work/DEFECTS.md` 037, and the
second convened three seats under CL-023 because the first two had ruled.

## Both routes were refused, and one of them was mine

**Panel 151** refused Route B — let the compiler LEARN a `tag`'s spelling from
clang — with two vetoes reached separately. The ground is one sentence: **the
emitter's probe IS §4.19's instrument**, it works by writing the AUTHOR's word
and letting clang disagree with the header, and a probe generated FROM the header
can no longer disagree with it. Measured: `record Db tag sqlite3_stmt` gives
**two** `error[ffi_parameter_type]` today and **zero** under that route.

Its premise was false three ways. `handles.c_spelling` has **zero callers**,
proved on the compiler translated to C. The existing clang dump never asks about
a handle. And the emitter finishes before the ask.

**Panel 152** refused the narrowing those seats had compiled — ask clang ONE BIT,
does this word need `struct` — on a measurement the historian predicted without a
shell and the coordinator ran: `addrinfoo *p;` errors and **`struct addrinfoo
*p;` compiles at exit 0**, so a misspelled tag gives the same bit as a correct
struct-only one.

## The answer was already spoken

**Clang distinguishes all three cases by message**: *must use 'struct' tag*,
*unknown type name*, and silence. `emit/ffi_tag.hero` has read the first string
since panel 074 and already builds a Heroes diagnostic with a `certain` fix.

**One comparison was throwing it away.** `emit/ffi_lookup.hero` matched the
declaration's Heroes NAME where clang's message carries C's, so `record AI tag
addrinfo` was looked up under a name no declaration holds. **A failing build
already printed the right message five times.**

## What that closes, and what it does not

A binding that made the compiler blame itself now refuses honestly:
`error[ffi_tag_needs_struct]`, caret on the author's line, saying the tag is
RIGHT and this compiler cannot write it. A misspelled tag stays refused. A record
with no tag keeps its `certain` fix, unchanged.

**The diagnostic offers no fix and that is the point.** Its sibling offers one
because a record with no tag can be given one; here the tag is already correct
and the repair does not exist, so a fix would be what
`.claude/rules/diagnostics-and-goldens.md` refuses: one that leaves the defect
standing. **The blessed emission pins the wrong C in bytes** — `addrinfo *`, five
times — so the day the capability lands, a suite says so.

## The precedent was unanimous and nobody had asked it

Six independent systems make the author write `struct addrinfo`: Nim, cgo, Zig,
CMake, Meson, autoconf. **Nim's own posix module does it thirty-two times in one
file**, and CLAUDE.md § 6 binds this language to Nim's surface. cgo refuses a
misspelled name rather than guessing. And **.NET's `ExactSpelling`, the one tool
that ever supplied a spelling an author omitted, was dropped by its successor**,
whose code-fix makes the author choose.

The instrument itself is not novel: autoconf moved header checks onto the
compiler across three releases, and cgo infers C name kinds from the presence or
absence of an error on a numbered line. **Reading clang's disagreement is that
instrument. Generating a probe to guess is not.**

## Two defects a seat that reads only the specification found

**042.** § 13 cannot bind a struct that is both READ and POINTED AT: the fielded
record and the handle record would both need one tag, and two records may not
name one tag. `getaddrinfo` has no writable binding at all, and `struct stat`
under `lstat` is the same shape.

**043.** The specification's only FFI example carries an acquire-and-release pair
with **neither** mark, and teaches the omission its own section forbids. Priced
here at **+7 vendored**, and not landed, because spec text owes a payment and the
one removal available is already allocated.

## A correction to this record

The ROADMAP, a commit body, panel 149's verdict and a log entry all said the
specification's correction could not land for want of an API key. **That was
false**: `.env` carries it. It was a negative claim nobody searched for.

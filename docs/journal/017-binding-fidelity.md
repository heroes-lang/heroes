# 017 — Binding fidelity: what an `extern` accepts

The milestone that started as a footnote in another one. `M-program-corpus`
closed, the Windows leg went green, and the last thing that went red on it was a
golden that had been correct on two platforms for a day.

## The goal

§4.19's guarantee, as the spec stated it, was: *"clang checks every signature and
constant against that header, so a wrong FFI type is a compile error."* The
milestone's goal was to find out whether that sentence is true.

It is half true. It is true about **result types**, which a `_Static_assert` over
a `_Generic` has checked since M-ffi-ladder, and it is false about **every
parameter**. Two judges falsified it independently with running programs:
`putchar(c: i64)` given `4294967361` prints `A` at exit 0, and a C `int
narrow(int)` given `4294967301` prints `5` — silently, under this project's exact
compile flags.

So the goal became the parameter side: make a wrong parameter a compile error on
the author's own line, and find out what the C vocabulary still cannot spell.

## What surprised

**The mechanism that checks results is the reason it cannot check parameters.**
`_Generic`'s controlling expression is not evaluated (C11 6.5.1.1p3) — which is
exactly what makes the return check free — and an unevaluated expression gets no
conversion diagnostics. Measured both ways: with a folding constant and with a
non-folding one, `-Wshorten-64-to-32` says nothing inside a `_Generic`. The check
could not be had by turning a flag on. It had to be built.

What it took was a **never-called function** whose parameters are the *declared*
types and whose body is the call. A parameter cannot be constant-folded, so the
conversion is the one a real call would make; `#line` puts the verdict on the
author's `extern` line; `__attribute__((unused)) static` keeps it out of the
binary and out of the warning count.

**42% of what anybody binds takes a parameter that is not 64 bits** — 267 of 643
bindable entry points across `sqlite3.h`, `curl/curl.h` and `raylib.h`. The gap
was not a curiosity.

**Two of the milestone's own premises were false, and the panel killed both.**

The first was mine, carried into panel 052's proposal from panel 051: *"19 raylib
entry points take a `float` and are silently wrong today."* They are not. A C
`float` parameter bound `f64` is **bit-exact** — clang converts against the real
prototype, and `0.10000000149011612` is not a bug, it is what a `float` holds.
Two judges measured it separately and got identical bits. The `f32` feature the
milestone was convened to add turned out not to be needed by the case that
motivated it.

The second was the historian's, and it was a good mistake to make: C makes calling
a variadic through a fixed prototype undefined (6.7.6.3p15), Apple's ARM64 puts
variadic arguments on the stack while fixed ones go in registers, and three
shipped projects — Erlang/OTP, Terra, CPython — bled on exactly that, on exactly
that target. So `examples/curl/main.hero`, which declares `curl_easy_setopt`'s
third argument as fixed, should be broken.

It is not, and the reason is a rule this project already had: **§4.19 refuses to
re-declare an `extern`'s signature.** The emitter writes `#include <curl/curl.h>`
and calls through curl's own variadic prototype. The Heroes declaration never
reaches C at all. The ffi-pragmatist measured both sides — the real prototype
returns `66` and the URL intact, a re-declared fixed prototype returns
`8393946705` and an empty URL on arm64 and the *correct* answer on x86-64 — and
left a standing veto on any variadic implementation that re-declares. A defect
that is silently correct on one CI leg and garbage on another is the exact class
this language exists to kill.

## What broke and why

**The Windows leg found four things and none of them were Windows.** `M_PI` hidden
by `_USE_MATH_DEFINES`, a third libc reproducing panel 047's class. `link "m"`
resolving to `m.lib`, which does not exist, because the maths functions are in the
C runtime — answered by `in_the_c_runtime`, which drops the `-l` names a platform
already carries, precedented in Zig's `isLibCLibName`. A module's *name* built with
`Path::join`, so one output named one directory in two spellings. And the milestone
tables in `milestones.rs`, written with `/` and compared against a native path.

Each was a fact about this project that had only ever been checked on one machine.

**One assertion's premise expired mid-milestone, in the loud direction.**
`gate.rs` asserted `!out.c.contains("_labs")` to mean *"an extern must not be
mangled"*. That rested on a premise about the world — nothing else in the unit
will end in `_labs` — which died the moment `extern_probe.rs` began emitting
`hero_ffi_probe_labs`. It went red on a correct change instead of green on a wrong
one, which is luck, not design; it now asserts the mangler's own shape.

**The gate nearly rested on a premise too.** The first design read *"a `.hero`
path means the author's error"*. Panel 052's historian named the hole — this
emitter lowers an author's call site under that author's own `#line`, so a
narrowing the **compiler** emitted would be blamed on the reader — and named the
shipped answer: `cmd/cgo/gcc.go` gates on synthetic per-probe `#line` filenames
and says why in its own source, *"we used to look at specific warning or error
messages here, but that tied the behavior too closely to specific versions of the
compilers."* The gate that landed asks the narrower question: is an `extern`
**declared at exactly this file and line**?

## What landed, and what carried forward

Landed: `emit/extern_probe.rs`, one probe per `extern` with a parameter;
`ffi_parameter_type`, the fifth member of CLAUDE.md §7's named exception, at exit 1
with the author's own parameter name and the Heroes width to write;
`-Werror=shorten-64-to-32` and `-Werror=sign-conversion`, both measured to fire on
FFI code and nothing else; the spec's false sentence repaired at +36, stating the
asymmetry that was measured rather than assumed — **a result may be wider than
C's, and a parameter may not**; `examples/sqlite/` moved to the header's own
widths in two edits and no shim.

Carried forward, and all of it is in `DECIDE.md`: **struct-by-value**, which is
356 of 1159 entry points and the whole remaining wall; a **rename**, so one C name
can carry two arities, which is what variadics actually need since the ABI is
already right; the **variadic slot's width**, which nothing checks — `int -1`
reads `-1` on arm64 and `4294967295` on x86-64; and **`f32`**, whose shape is not
settled because the case that motivated it does not need it and the case that does
is a `float *` out-parameter, which wants a typed pointer rather than a float type.

The row in design.md Part 7 that refused sized integers now carries its second
correction in two milestones. Its named falsifier — `strlen` — has been produced
and compiles; what survives is the sentence it closes with, and panel 052 measured
that from the other side. `emit/ffi.rs` can propose `i32` for a C `int` and can
propose nothing at all for a C `long`, which is 64 bits here and 32 on Windows.
A Heroes width is a number. A C width is a question about the machine.

# 049 — The platform axis: a feature that does not unblock the thing it was convened for

**Status** `provisional — author ratification pending`
**Convened** 2026-08-14, by author instruction · **Lane** full panel, five judges

## Why it was convened

The author ruled, after panel 048: *"given the importance of the FFI in this
project I do not mind if it costs me spec tokens — do the best, most robust and
general solution, and introduce a concept of OS/platform to handle the linking of
frameworks or DLLs or `.so`s. These aspects are central; spend the tokens needed
but make them work fully."*

The convening premise was panel 048's own sentence: **§4.19's ladder rung 5,
raylib, is unbindable on Darwin because `link` cannot spell `-framework`.**

## The proposal, verbatim

> `extern "raylib.h" link "raylib"` keeps its meaning — that library everywhere.
> Inside the group, `link <platform> [framework] "a", "b"` adds libraries on that
> platform only. Platforms: `macos`, `linux`, `windows`. `framework` is macOS's
> own kind.

Measured before any judge saw it: **+144** with a worked example, +79 prose only,
against 2983 and a ceiling of 4096.

## The verdict table

| judge | verdict | the measurement that decided it |
|---|---|---|
| **llm-ergonomist** | **approve P**, **veto Q** | converged on P's exact syntax *unprompted*, before being shown it; vetoed the rival on locality |
| **spec-warden** | **VETO** | Principle 0: the §1.0 payment does not exist. Priced seven wordings, +28 to +144 |
| **compiler-engineer** | **object** | linked a raylib program **with zero frameworks**; ~410 lines; `fmt` would print two heads for one group |
| **ffi-pragmatist** | **object**, no ABI veto | **opened a raylib window from `.hero` source today**, head line unchanged, no framework, no platform token |
| **historian** | **object** | **0 of 11** systems put a platform keyword inside the link construct |

## The premise was false, and the document had already said so

Three judges measured it from three directions, and the synthesis a fourth.

**design.md:2056–2061, ratified at M-ffi-ladder, in the very section the proposal
cites:**

> *"Measured before it was needed: the homebrew **dylib** links with `-I`/`-L` and
> **no framework at all**; the static archive needs five. So what the group head
> lacks is **search paths**, not a framework keyword — and both are paths,
> validated not to begin with `-`, so the form stays closed rather than becoming
> an arbitrary-flag hole."*

**The ffi-pragmatist climbed the rung.** With the head line unchanged —
`extern "raylib.h" link "raylib"` — plus `-I/opt/homebrew/include
-L/opt/homebrew/lib`, a Heroes program **opened a window, drew, and exited 0**
today. Not one framework token was consulted.

**And what `heroes build` actually says on a raylib group:**

    internal error: compiling the generated C failed:
    build/…/ray.c:5:10: fatal error: 'raylib.h' file not found

Exit **2**. It dies at the *header path*, one directory before the linker exists
as a question. Every framework token in the proposal is unreachable.

**The spec-warden's independent route**: `pkg-config --libs raylib` returns
`-L… -lraylib`, no frameworks; in a clean environment the `-L` is the blocker;
with it, `otool -L` names libraylib and libSystem and nothing else.

## A measurement error of the synthesis's own, and it is the third this week

The synthesis reported to the author that *"`clang -lraylib` alone links a real
raylib program on this Mac, exit 0, zero frameworks"*. True in that shell and
**false in a clean one**: this machine exports `LIBRARY_PATH=:/opt/homebrew/lib`,
so `-L` was being supplied invisibly. Under `env -i`: `ld: library 'raylib' not
found`. The ffi-pragmatist caught it by measuring under `env -i` and named it for
what it is — CLAUDE.md §11's premise about the world, dying silently.

The conclusion survives and is better supported: frameworks are not the blocker,
**and neither is `-l` alone**. It is `-I` and `-L`.

## What the proposal actually earns, and what it misses

**Earns: one row.** The ffi-pragmatist verified the three inversions end to end.
`iconv` is *"the one thing today's language genuinely cannot say, and the proposal
says it"* — `-liconv` is required on Darwin and **fails** on glibc, where the
symbol is in libc. `rt` and `pthread` are expressible too. That is the honest
credit: one row, not a ladder rung.

**Misses, first: the axis is wrong for its own motivating cases.** Panel 048
measured the divergences per **libc** — `strptime` hidden by glibc and shown by
musl; `pthread_create` moving at glibc **2.34** — and glibc and musl are both
`linux`. FreeBSD, which the same sitting probed, has no name in the enum.

**Misses, second, and this is the largest finding of the sitting.** Rung 5's own
text promises *"real struct passing and framework linking"*. **383 of raylib's 600
`RLAPI` declarations pass or return a struct by value.** `ClearBackground(Color)`:

    error: passing 'int64_t' to parameter of incompatible type 'Color'

Heroes has seven boundary types and none of them is a struct. `DrawFPS(int, int)`
is the one draw call that escapes, which is why the judge's working program uses
it. **No platform token, on any platform, changes this by one line.** The wall at
rung 5 is the type vocabulary, and it was not on this sitting's agenda at all.

## `windows` cannot be promised today

The ffi-pragmatist cross-compiled the real runtime for `x86_64-windows-gnu`:
**18 errors, all in `runtime/parts/f64.c` lines 56–93** — `locale_t`,
`newlocale`, `uselocale`, the POSIX-2008 locale code this very repository patched
for Linux hours earlier. mingw has no `xlocale`. And `-l<name>` does not mean one
thing on Windows: the MSVC ABI wants `.lib`, the mingw ABI resolves `.dll.a`, and
zig's mingw searches neither in the order GNU `ld` does. CI has two legs.

The warden priced `windows` at **2 tokens** and said the right thing about it:
strike it on Principle 0 and §11, **not** on tokens, and let nobody claim a saving
there.

**The honest precedent is Rust's tier policy** (historian, sourced): Rust names
dozens of targets it does not test — and says so, per target, in writing. *"Tier
3: Rust provides no guarantees about tier 3 targets; they exist in the codebase,
but may or may not build."* If `windows` ever enters this grammar, it enters with
a written tier or not at all.

## The precedent runs against the shape, not against the idea

The historian sourced eleven systems. **Zero put a platform keyword inside the
link construct.**

- **Go** — the closest analogue, and the only one that puts the condition in the
  source — reuses its *general* build-constraint vocabulary:
  `// #cgo darwin,arm64 LDFLAGS: -framework CoreFoundation`. The `#cgo` line is
  not a private dialect.
- **clang module maps** spell `link framework "Cocoa"` with **no platform word**,
  and defer to a separate `requires macos` on the module.
- **Cabal** does not condition at all: `frameworks:` is *"ignored on all other
  platforms"*.
- **Rust** refuses: `kind = "framework"` is *"only valid for macOS targets"* and
  E0455 is a **compile error**; the escape is the general `cfg_attr`.

**And Go's chronology is the slope, dated.** `#cgo LDFLAGS` shipped
platform-blind on 1 Feb 2011 (CL 3921043); within ten months Go had a general
platform algebra which the flag directive then absorbed; then CVE-2018-6574
forced a flag allow-list, because a free-form flag string in a source file was a
remote-code-execution vector. The historian's line is the one to keep: *"the
proposal's defence — `link` is the only place platform names may appear — is
precisely the state Go was in on 1 Feb 2011, and it did not hold."*

**Rust escaped the slope only because `cfg` came first** — 0.3 in July 2012,
frameworks in 0.6 in April 2013. Heroes has no `cfg` and §13 forbids one. That is
good, *and* it means `link macos …` is the thin end with nothing behind it.

**The compiler-engineer found the structural reason anyway**, which is worth more
than a policy: `Emitted` has exactly **one** channel from a declaration to the
toolchain that is not the emitted bytes, and `link` is it. A `function foo()
macos` would have to travel through the bytes, which are under the double-emit
determinism test and five golden snapshots. But it is a premise about the
pipeline's shape and would expire the day `compile "shim.c"` lands — so it is
worth what its guard is worth, and the guard is one fixture: *the emitted C is a
function of the source alone, never of the host*, asserted byte-identical on both
CI jobs.

## The option the sitting did not consider

**pkg-config**, and the historian is right that it needed answering. raylib ships
`raylib.pc`; `pkg-config --libs raylib` returns the frameworks on Darwin and
`-lGL -lX11 -lm -ldl` on Linux **from one universal spelling**. Go made it a
first-class cgo verb. CMake's `find_library` does the same trick a different way,
resolving one spelling to `-framework` on Apple.

CLAUDE.md §10 forbids a second binary and a script, which is very likely the
refusal — but **§12 holds a refusal to a feature's burden of proof**, so it must
be written down with the program or compiler fact that makes it wrong, not left
implied. That is owed whatever else happens.

## The formatter would have broken

The compiler-engineer traced it and it is worse than the convening brief guessed.
`printer/fmt.rs:70` keys a group run on *(header, link)* and `fmt_extern.rs`
rebuilds the head from **any one member**. `link` lines inside the block give two
outcomes and both are defects: the parser stamps as it goes and `fmt` prints
**two head lines for one group** — verbatim the failure `printer/tests/externs.rs`
was written to forbid — or `fmt` hoists them, which contradicts its own stated
rule, *"**Source order, not a canonical order.** … reordering is where a formatter
starts deleting the author's remarks."* And link order **matters** to a linker,
reproduced. This repository fixed a formatter that changed program meaning nine
hours ago.

## Resolution — provisional, author ratification pending

**The proposal does not land.** One veto (spec-warden, Principle 0), three
objections, and the single approval came from the judge that was never asked
whether the feature unblocks anything — it was asked whether the *syntax* reads,
and it does.

**Landed tonight, at zero spec tokens:** `ffi_missing_header`. A header this
program names and this machine does not have was `internal error:` at **exit 2**,
the compiler blaming itself for a missing `-I` on the author's machine. It is now
a diagnostic on the author's line at exit 1, gated on the group that named the
header — the sibling of panel 048's `ffi_missing_link`, and the fifth member of
§7's named exception.

It does not repair the gap. The language still cannot **say** where a header is.
It repairs the blame.

**Queued for the author, in the order the measurements rank them:**

1. **Search paths** — the measured blocker, what design.md:2060 already names,
   priced at **+28** against the proposal's +144. The design question the sitting
   did *not* settle and must not be settled at speed: an absolute path in a source
   file is **more** machine-dependent than a platform token, which is the same
   §1.3 objection the warden raised against the platform axis. `from "<prefix>"`
   expanding to `include/`+`lib/` is a world-premise; two explicit clauses are
   facts but freeze one machine's layout into the program.
2. **pkg-config's refusal**, written with its falsifier per §12.
3. **Struct-by-value at the FFI boundary** — 383 of 600 raylib entry points, the
   actual wall at rung 5, and untouched by everything above.
4. **`framework`**, when it comes: untagged must be an **error** (Rust E0455), and
   the vocabulary should be clang's split or Cabal's inertness rather than a
   link-local platform word.
5. **`iconv`** is the platform axis's one genuine witness. If the axis lands, it
   lands for that row and it must be honest that the real axis is the **libc**.
6. **`windows`** — struck until the runtime builds under a Windows target and CI
   has a third leg, or admitted with a written Rust-style tier.

## Predictions to score

| # | judge | prediction | checkable at |
|---|---|---|---|
| 1 | ffi-pragmatist | `examples/raylib/main.hero` under the proposal fails on `macos-14` at `'raylib.h' file not found`, **exit 2**, before `ld` runs — no framework token consulted. Delete every platform token, add `-I`/`-L`, and it builds, opens a window, exits 0 | **first half now scored: the diagnostic makes it exit 1, and the header is still the wall.** Second half scored locally by the judge |
| 2 | ffi-pragmatist | `zig cc --target=x86_64-windows-gnu runtime/runtime.c` reports **18 errors**, all `locale_t`/`newlocale`/`uselocale` in `parts/f64.c`. No FFI-surface change moves that number | a compile, today |
| 3 | spec-warden | with the platform concept landed and no search path, `heroes build` on a raylib group still fails at the header on both Darwin and `macos-14` — **zero lines of progress on rung 5** | CI |
| 4 | compiler-engineer | at rung 5's close, `examples/raylib/main.hero` builds and runs on the macos-14 job with the whole `link macos framework …` line **deleted**, and the only clause it needs that the language lacks is an include search path | `corpus.rs` on CI |
| 5 | compiler-engineer | if the clause lands as proposed, `printer/fmt_extern.rs` exceeds **140** lines (today 95) and `toolchain.rs` exceeds **360** (today 333) | `wc -l` |
| 6 | historian | the proposal's own example does not link: raylib's own wiki gives **five** frameworks for the static archive and the example names **two** | a compile on the Darwin leg |
| 7 | historian | a test asserting every platform token in the corpus is one some CI leg compiles **fails the day `windows` is written**, absent a tier note | `cargo test` |

## The record this sitting has to repair

`docs/panel/048`'s sentence — *"§4.19's ladder rung 5, raylib, is currently
unbindable on Darwin"* — is **falsified**, and it was the sentence that convened
this panel. design.md:2058 was right a milestone earlier. Panel 048 is a dated
record and is not rewritten (CLAUDE.md §14); the correction is appended there and
carried here, which is what the two files are for.

**Four measurement errors are on this sitting's record**, three of them
self-reported by the judge or synthesis that made them: the ffi-pragmatist's
`zig cc` matrix in 048 (auto-linked libm), the synthesis's shell-quoting artifact
in 047, its `LIBRARY_PATH` contamination here, and panel 048's framework claim.
Every one was found by measuring again rather than by arguing, which is the only
reason the resolution is trustworthy at all.

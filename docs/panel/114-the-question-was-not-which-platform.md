# Panel 114 — the question was not which platform

**Convened** 2026-09-06 by author instruction, over the coordinator's twice-stated
recommendation to leave the question where it was already scheduled. Full five
seats: it changes surface syntax, it costs spec tokens, and a diagnostic class is
in play. **The author also named a preference** — *a new symbol, something like
the dollar `$`* — and an acceptance criterion, given the same hour: *in the end I
want a real net on the threads, not skipped by Windows or anything else.*

**Frozen at `b0d8d927`**, one snapshot with a built compiler copied per judge.
The ergonomist received `spec/heroes-spec.md` and four sample programs and
nothing else.

## The verdicts

| seat | verdict | the measurement that carried it |
|---|---|---|
| **compiler-engineer** | **veto** of any shape putting parsed-but-unchecked text in a `.hero` file | `heroes mutate` walks the AST, so a dead arm is a mutation site and every mutant there counts SURVIVED |
| **spec-warden** | **veto** of all four token-costing shapes; approve the program-supplied header | every shape fits the budget, so the veto is Principle 0 and not arithmetic |
| **ffi-pragmatist** | **object**; veto of shape A as sampled; veto of any resolution that hides an unverified `extern` | **0 of 15** `extern` groups in the tree need a platform word |
| **llm-ergonomist** | **veto** of `$` inside a body; the placement of `$` on a declaration is right, the rules are not | shape A **cannot gate a header**, so it does not solve the problem it was drawn for |
| **historian** | **object to the SET** (advisory) | nobody has ever removed a conditional-compilation feature after shipping it |

## What the brief got wrong, and it is three things

**The convener wrote all three.** They are here because CLAUDE.md §1 says a brief's
errors belong in the record beside its findings.

1. **"CI compares `seed/heroes.c` on every leg" is false.** `.github/workflows/ci.yml:604`
   reads `if: runner.os == 'Linux'` — **one leg**. Found by the spec-warden and
   confirmed by the ffi seat independently. The conclusion the brief drew from it
   survives and gets stronger: that one step is the only place host-independent
   emission is verified rather than assumed.
2. **"Panel 049's return condition is met, so the door is open" overstates what was
   met.** The spec-warden — who wrote that veto — corrected it: condition 6
   unblocks **the word `windows`**, not the feature. The veto's ground was §1.0's
   missing payment plus §1.3, and condition 6 touches neither.
3. **The brief called "Heroes has nothing" a gap. It is the artifact of three
   rulings**, and CLAUDE.md §1 says to grep for the ruling behind a silence
   *before* convening on it. `DESIGN-LOG.md:407` (2026-08-24, `unistd.h` in the
   seed), `DESIGN-LOG.md:455` (2026-08-26, `hero_word_bits`), `DESIGN-LOG.md:485`
   and panel 097 (2026-08-30, the filesystem arm). `selfhost/cli/io.hero:22` says
   it in the repository's own words: *"The route out is not a platform arm — this
   language has no `#if`, by design — but `hero_write_err` in
   `runtime/parts/os.c` … The platform question lives in the runtime's C, the one
   file in this project that is allowed to know what machine it is on."* **This
   sitting was convened on a silence that was a decision.**

## The shape the brief did not list, and it works today

The **ergonomist** found it by reading the spec cold, with no access to the
compiler: § FFI says *"A group names its header."* It does not say a **system**
header. So a program ships its own header with the `#ifdef` inside, and the
platform choice is made in C, where `#ifdef` already exists and clang checks it on
each machine.

**Measured by the coordinator on two platforms the same hour, then re-run
independently by the compiler and ffi seats:**

```c
/* nap.h, shipped beside main.hero */
#ifdef _WIN32
#include <windows.h>
static inline long long nap_ms(long long ms) { Sleep((unsigned long)ms); return ms; }
#else
#include <unistd.h>
static inline long long nap_ms(long long ms) { usleep((unsigned)(ms * 1000)); return ms; }
#endif
```

macOS `slept 5 ms on whatever machine this is`, exit 0. Windows under
clang/MSVC, the same line, exit 0. The include path already carries the compiled
file's directory — `selfhost/cli/units.hero:132` and `selfhost/cli/pointee.hero:283`,
both `["-I", source_dir]`, found by the compiler seat.

**And the emitted C carries no platform word at all**: `--emit-c` on that program
gives **0** occurrences of `_WIN32`, `usleep`, `Sleep` or `__APPLE__`, and names
`#include <nap.h>`. The divergence happens inside clang's preprocessor, *after*
this compiler has emitted, so `cmp seed/heroes.c` is untouched.

## Why a language form does not land, and none of the reasons is the symbol

**It would make the author's own criterion read as met while it was not.** Under
shape A the `extern` group stays at column 0, so on Windows `#include <pthread.h>`
produces `error[ffi_missing_header]` — **one of the three strings
`tests/harness/shell.hero:450` matches to SKIP a case**. The ten thread examples
would be skipped in silence, and `suite_corpus.hero:152`'s floor
(`skipped * 2 > programs.len()`) does not fire: ten skips of fifty-six is twenty
against fifty-six. Found by the ffi seat, confirmed by the coordinator.

**It would make Part 11's mutation score platform-dependent.**
`selfhost/mutate/score.hero:6-8`: a mutant the compiler rejects is KILLED, one it
accepts is SURVIVED — *"a silent wrong program"*. Every operator in
`selfhost/mutate/edits.hero` takes `a: ast.Ast`, so `mutate` walks the **tree**,
and a parsed-but-unchecked arm **is** a mutation site. Every mutant made there is
accepted and counted SURVIVED, and none of them is a silent wrong program. That
biases metric 3 downward, per machine, against `ci.yml:555`'s *"three machines,
three architectures, one table"*. This is the compiler seat's and it is new: no
document covers it.

**And it would make 200 irreplaceable oracle files platform-specific.**
`tests/emission/` holds 200 blessed files, 10 MB, which
`tests/harness/suite_emission.hero:16` calls *"what two independent
implementations agreed on, byte for byte"* — and the second implementation is
archived. The net runs on all three legs with no `if:`. One corpus program using
an in-language form turns those files into one platform's, and there is no way
back. Also the compiler seat's, also new.

**§1.7 says it is core, not sugar**, and the distinction is exact: every existing
sugar translates a form the checker already judged, while `$if`'s inactive arm is
a subtree the checker is **told to skip**. The form disappears before the IR not
because lowering translated it but because the checker was instructed not to look,
and those are opposite facts wearing the same shape.

**The blast radius, measured by probe edit** (`./heroes check selfhost/main.hero`,
4.87 s baseline) — and it contains a warning for any future spelling:

| probe | forced call sites | files |
|---|---|---|
| shape B as a new `ast.DeclKind` case | **113** | **47** |
| shape A as a new `ast.StmtKind` case | 22 | 9 |
| shape B as a **field** on `record Decl` | **22** | **5** |
| `$` as a new `token.TokenKind` | 3 | 3 |

**If a declaration-level form is ever taken, it is a field and never a kind** — 5×
the cost for the same feature.

## The symbol, and the author's instinct was better sourced than the brief assumed

The coordinator asked the historian to report a failed search. **It is not one.**
Three living families spell conditional compilation with `$`: Turbo Pascal and
Delphi (`{$IFDEF}`), Oracle PL/SQL since 10gR2 in 2005 (`$IF … $THEN`), and **V**
— *a small statically typed compiled language that emits C*, with `$if linux { }`
at statement level and an entire `$`-prefixed compile-time sublanguage. The
author's preference landed on the spelling chosen by the language closest to this
one's architecture.

**`$` is also free here**, measured: `error[unexpected_character]` today, zero hits
in `selfhost/lexer.hero`, `scan.hero`, `token.hero`, and the mangler drops it
safely (`emit/mangle.hero:136`, `:214`). Four comments claim *"`$` is not in the
language"*; only `ir/print.hero:15` is load-bearing and **it is already false**,
since the diagnostic above prints one. Cost: four comment repairs, no mechanism.

**The one collision worth the author's attention**: CLAUDE.md §6 says Heroes copies
Nim's surface, and in Nim `$` is the **stringify** operator, its general spelling
of `toString`. A reader arriving from Nim reads `$x` as *make this a string*.

## The one-directional finding

**Nothing the historian could source ever removed a conditional-compilation
feature once it shipped.** The whole recorded history is narrowing, and it takes
years. D shipped `version` deliberately without `&&`, `||` or `!`. Rust took from
2020-11-04 to 2024-07-25 to add a typo check, whose motivating example is
`#[cfg(feature = "widnows")]` — compiles clean, deletes the function in silence —
and fenced `#[cfg]` out of `core` with a repository lint. Go rewrote its
constraint syntax after cataloguing five broken constraints in public
repositories. Odin took constraints out of comments and still ships two
mechanisms that disagree about the thing that matters. The Linux kernel, the
largest `#ifdef` user alive, tells contributors not to use it in `.c` files.
**Ada, Java and Oberon omitted it deliberately and held.**

For a spec with 266 tokens of headroom: **the form you choose is the form you
keep.**

## Resolution — provisional, author ratification pending

CLAUDE.md §4: the most robust and complete resolution, never the cheapest and
never a compromise, with the conservative one recorded.

**R1 — No conditional-compilation form enters the language.** Three vetoes, from
three different grounds — §1.7 (core, not sugar), Principle 0 (§1.0 and §1.3), and
non-locality — and none of them is the budget: every shape fits the 266 tokens.
The mechanism the author asked for already exists at zero cost, measured on two
platforms by three parties.

**R2 — A program may name a header it ships beside itself, and the spec says so at
a net cost of +1 token.** The removal is named and priced: delete from § Strings
*"`+` on `str` copies both sides — a concatenation loop is quadratic; `join` and
`repeat` build in one pass"* (**−31**), because its whole content is performance
advice and **§13 says performance is never a justification**. What replaces it
earns its place under §1.4 — it prevents a hallucination rather than a slow loop:

> The header may be one the program ships beside its own file, so a difference
> between machines is written in C, where `#ifdef` already exists.

Measured together: **3831, net +1.** The warden recommends also closing the
question — *"This language has no conditional compilation of its own"* — which
lands the trade at **3853, net +23**, and the synthesis takes it: it is the
sentence that stops a reader hunting for a `#if` every other language they have
seen has.

**R3 — Three instruments are owed, and R2 does not land without them.** Each was
found by running the shape rather than by reading it, and each is zero spec
tokens.
- **A header the program SHIPS and a header the MACHINE lacks must be told
  apart.** Today both are `error[ffi_missing_header]`, which
  `tests/harness/shell.hero:450` matches to skip a case — so this shape's failure
  mode is a silent skip, on the criterion whose words are *skipped by nothing*.
  Harness change.
- **The compiler reports which `extern` declarations it did NOT verify on this
  machine.** The ffi seat compiled the proof: a wrong return type in the live arm
  is a `_Static_assert` failure, and **a wrong return type and a wrong parameter
  type in the dead arm is clang exit 0 and a running binary**. §4.19 promises a
  wrong FFI signature is a compile error; half that promise is silently unkept on
  any one machine, and this is the line that makes it visible. This is the ffi
  seat's veto and it binds every two-arm shape equally.
- **A fifth `declaration()` class**, because a syntax error in the author's own
  shipped header is `exit 2, internal error: compiling the generated C failed` —
  the compiler blaming itself for a mistake in a file the author wrote. §7's four
  named exceptions do not cover it. Writing R2's sentence into the spec without
  this would advertise a road that ends in an internal error.

**R4 — The acceptance criterion funds nothing here, and that is what settles the
sitting.** `runtime/parts/thread.c`'s guard refuses a Heroes function on any
thread the program did not start, **whatever spelling selected the arm** — run by
the warden, `panic: main.worker ran on a thread this program did not start`, exit
134 on macOS where `$if` would have chosen the POSIX arm. So the ten thread
examples run under none of the shapes until that question is answered elsewhere,
and no shape may spend the criterion as currency.

**R5 — The axis is the libc, not the platform, and the record already said so.**
The ffi seat enumerated from the tree: **15 `extern` groups, 62 member
declarations, 0 needing a platform word**; 5 needing a does-this-header-exist
answer, all five already served by `ffi_missing_header` (panel 049's zero-token
landing) and `package` (panel 050). `DESIGN-LOG.md`, 2026-08-14: *"049 vetoed a
platform axis in the language; the same problem is answered here in the driver,
where it is a fact about the machine that can be measured on the machine, rather
than a word the author has to write into a program that then means nothing on the
machines it does not name."* And panel 049's own condition 5 says the real axis is
the libc — with witnesses a platform word cannot express: `strptime` hidden by
glibc and shown by musl, `pthread_create` moving at glibc 2.34, **and both are
`linux`**.

**R6 — `compile "shim.c"` was deferred, not vetoed, and its acceptance test has
arrived.** Panel 036:275 — *"not refused on their merits but deferred for want of
an acceptance test. They wait on a rung that needs a shim."* The rung came, and
the finding is that a header of `static inline` functions needs no `compile` at
all. That deferral can be closed.

**R7 — If the author takes the form anyway, these are its terms**, recorded so the
decision is not re-derived. It stands **in front of a declaration and never inside
a body** — shape A cannot gate a header and so cannot solve the problem it was
drawn for, and inside a body it makes a binding's type a fact about the compiling
host. It is a **field on `record Decl`**, never a `DeclKind` case (22 sites against
113). The tag set is **closed and enumerated in the spec**, so an unlisted tag is a
compile error rather than a dead branch. A name reachable from ungated code is
**declared under every tag**. And **all arms of one name share one signature, byte
for byte** — without which one name is `()?` on one machine and `()` on another,
four lines apart, and the caller's line cannot be written correctly for both. The
spelling is a **word**, not `$if`: `spec:22` says *"Every top-level line starts
with its kind"*, and a `$` at column 0 breaks a sentence the spec states as
universal.

**What the conservative resolution would have been**: adopt nothing, leave the
question at M-core-packages, and let the next sitting re-derive it. It changes
least. It also leaves the mechanism that already works undocumented — so the next
reader invents a form that is not there, which is the error this whole sitting was
made of.

## Predictions to score at M-isolated-threads close

1. **spec-warden**: with no form in the language, **0 thread examples skipped on
   all three legs**, and `grep -c '\$if\|#if' examples/**/*.hero` is **0**.
   Falsified if any thread example is skipped on any leg, in which case the
   file-name shape lands at **+84**, already drafted and measured.
2. **spec-warden**: the sentence lands at **3831** with a named removal — the first
   in this class — scored by `heroes measure` in the commit that spends it.
3. **compiler-engineer**: if a `$if` form ever lands and one corpus program uses
   it, a Windows mutation run reports a score **≥ 1 point below** the Linux score
   over the same corpus, falsifying `ci.yml:555`.
4. **ffi-pragmatist**: `examples/sqlite/` needs no conditional compilation under
   any shape on all three legs — it builds and runs here in **0.35 s** today.
5. **ffi-pragmatist**: `link "pthread"` fails on the Windows box with `LNK1104`
   until `in_the_c_runtime` gains the name.
6. **llm-ergonomist**: under a body-scoped form, **≥ 60%** of generated samples
   leave the `extern` group ungated at column 0 — as the convener's own sample
   did — and every one is red on the platform its arm was written for.

## Author's verdict

**RATIFIED 2026-09-06 by the author, in full**, in these words: *"all right, I
understand — you have convinced me."* The sitting had been convened by the same
author, over the coordinator's twice-stated recommendation, and its answer is the
opposite of the instruction that opened it — so this line is the record of a mind
changed by measurement rather than of a plan carried out.

**What convinced was not the argument.** It was `M-isolated-threads` step 6,
committed forty minutes after the last seat reported: the runtime grew
`hero_thread_spawn` and a Heroes function ran on a real thread on macOS, on
Windows and on Debian, one source, one answer, **with no conditional compilation
anywhere in it**. The platform arm went where every other one in this project
already lives. The question the sitting was convened on — *should the language be
able to say which machine it is on* — was answered in practice by the most
platform-dependent feature there is not needing it.

**R7 is therefore not spent, and it is not deleted either.** If a case appears
that the runtime cannot absorb, the terms for a `$` form are written down and
priced, and the historian's finding stands beside them: Turbo Pascal, Oracle
PL/SQL and V — a C-emitting statically typed language — all spell it `$`, the
lexer here has the symbol free, and the one collision is that Nim, whose surface
CLAUDE.md §6 copies, spells `toString` with it.

**One question came back the same hour and it is filed rather than answered
here**: *if in future I want a graphics library wrapping three different libraries
underneath, how will I do it?* The thin case is answered and measured — one header
the package ships, `#ifdef` inside, which is how SDL itself is built and how
`examples/sdl/main.hero` already binds it with no platform word. The real case
wants a `.c` file the compiler builds, which is `compile`, which panel 036
**deferred rather than vetoed** *"for want of an acceptance test"*. That question
is that test, and it is an open item in `docs/work/DECIDE.md`.

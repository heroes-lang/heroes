# Panel 208, completeness critic

One pass after the three reports (compiler-engineer, ffi-pragmatist,
historian). No verdict of my own. Started 16:00:33 by `date`. Tree: my copy of
`391628b6` under `.claude/worktrees/scratch-b15/208-critic/tree/`; my own C in
`208-critic/pcheck/`, `TMPDIR` in `208-critic/tmp/`. I built no compiler (the
compiler-engineer's seed build read `real 656.38` on this machine; there was
no time for one). Every command I ran is named beside its result; every
other number is a seat's, cited to its report.

## 1. Framing facts in the briefs that are false

**F1. design.md `:3744`.** At `391628b6`, `grep -n -i 'no warning level'
docs/design.md` gives **line 3800**, *"Note also that this language has no
warning level: a diagnostic is exit 1 or nothing"*. Both building seats
caught it (compiler-engineer § 7, ffi-pragmatist § 4); the historian repeats
`:3744` (its § Routes, (N)). The synthesis cites `:3800` at `391628b6`.

**F2. "Zig's `-fallow-deprecated` is the shape" of (F).** The historian
(§ Zig, URLs github.com/ziglang/zig/pull/22898 and the 0.14.0 release notes)
reports PR #22898 merged 2025-02-27 and **reverted 2025-02-28**, absent from
0.14.0, the proposal open again on Codeberg as #36703. If that holds, (F)'s
cited precedent never shipped. *Unrun by me*: I fetched no page; it rests on
the historian's fetches, which it says were rendered by a summarising model.

**F3. "A system header's deprecated function did not warn, unexplained" and
its two candidate causes.** Both building seats measured the cause
independently and agree: **neither clang's system-header rule nor the guard's
region**, but `_FORTIFY_SOURCE=2` on by default on this Mac making `sprintf`
a function-like macro that expands to `__builtin___sprintf_chk`, which
carries no `deprecated` attribute (compiler-engineer § 1, § 1b; ffi-pragmatist
§ 3, `clang -E` of the unit, and the same unit warning under
`-D_FORTIFY_SOURCE=0`). Two independent derivations of one cause; I did not
re-run it. The historian's lead (`_POSIX_C_SOURCE` guards the attribute) is
true of the header (`_stdio.h:277`, ffi-pragmatist § 3b) but is not the
cause: the unit does not define it, and the call warns under fortify off.

**F4. `__attribute__((warning(...)))` in the list of deprecation shapes.**
It is not one: its group is `-Wattribute-warning`, a code-generation
diagnostic, silent under `-fsyntax-only` (ffi-pragmatist § 2) and at `-O2`
when the call is inlined (compiler-engineer § 1c, `m.c`). Any route that
treats it as a deprecation gives one program two verdicts by optimisation
level.

**F5. "No sentence of `spec/` or `docs/design.md` names `deprecated`" is
true, and the silence is not empty.** `grep -n -i -c deprecat docs/design.md
spec/heroes-spec.md` gives 0 and 0. But the brief's own instruction, grep
`docs/panel/` and `issues/` for it, was carried out by no seat for the
sitting's question (the compiler-engineer: *"I searched design.md and the
spec, not other trees"*; the ffi-pragmatist grepped `issues/` for two
beside-findings only). I ran `grep -rl -i deprecat docs/panel/ issues/` in
my tree and read the hits. Two are rulings on this question:

- **Panel 205, ratified** (`issues/2026-10/10/2026-10-10-0340-panel-205-ratify-*.md`,
  its box `- [x]` in my tree and on the trunk): its critic's finding beside,
  carried into R5 as defect 571, calls a program binding `<ucontext.h>`'s
  deprecated `getcontext` **"a correct program"**, and classes clang's
  warning on it `blocking` as *a clang warning on a correct program*
  (`docs/panel/205-*.md:108-111`, `:187-189`). So the record already holds
  that a program binding a deprecated name is correct. The brief's open
  question (*which one the program IS*) was answered by a ratified sitting
  for one shape, a binding; whether that answer extends to a CALL is the
  question 205 did not ask.
- **The `run` goldens of defect 396** (lane b13-land-buf, 2026-10-07):
  `tests/golden/run/fixedbugs-396-openssl.h` says in its own comment
  *"OpenSSL 3 marks `SHA256_Init` and its two siblings deprecated, and a
  clang warning on a correct program fails the `warnings` suite, so this
  header asks OpenSSL not to mark them"*, and defines
  `OPENSSL_SUPPRESS_DEPRECATED 1` in a first-group header. Three `run`
  goldens bind `SHA256_Init`, `SHA256_Update`, `SHA256_Final` through it
  (`grep -n 'SHA256' tests/golden/run/fixedbugs-396-sha256-final-fills-its-digest.hero`).
  So **the ffi-pragmatist's way out under (P) is already the repository's
  practice**, written a week ago, and the same comment calls a program
  calling `SHA256_Init` *a correct program*. Neither seat cites it.

(Other hits are unrelated: precedents of other languages deprecating things,
panels 058, 089, 090, 109, 113, 131, 143, 163, 192; panel 177 item 7 rules
that *"the two deprecated one-parameter OpenSSL adders bind as `consumes`"*,
`EVP_PKEY_asn1_add0` and `EVP_PKEY_meth_add0` by its critic's report, line
123: a ratified sitting writing the binding rule FOR two deprecated
functions. Under (R) or (P) every binding that rule describes is refused at
its use or its line, unless a switch is set.)

## 2. Claims asserted and not measured, and what I settled

**C1. The compiler-engineer: (B), refusing at the binding, "misses what has
no probe today, a function with no parameter (`version()`, `RSA_new()`) and a
constant, unless probes are added" (§ 6, "unbuilt").** **Falsified for
`RSA_new()`.** The emitted unit names a parameterless function in the
return-type static assert: the compiler-engineer's own
`shapes/build/tu-3e6dd69a6517ea46/s14opensslbound.c:49` reads
`_Static_assert(HERO_RET_PTR(RSA_new()), "heroes-ffi-return RSA_new ptr");`,
inside 571's QUIET (line 26). I copied that unit to `pcheck/s14.c`, turned
line 26's `ignored` into `error` (`sed`, `pcheck/s14P.c`) and compiled it,
`clang -std=gnu11 -Wall -fsigned-char -fsyntax-only -I tree/runtime
$(pkg-config --cflags openssl) s14P.c`: **exit 1, `s14P.c:49:29: error:
'RSA_new' is deprecated [-Werror,-Wdeprecated-declarations]`**. So a
binding-site refusal sees a never-called parameterless function. The
ffi-pragmatist's claim that (P) is *uniform* (§ 4, "the probe ... and the
static asserts") holds on this shape, which its own route table did not
carry (its six units hold no parameterless function). What the run also
shows and nobody priced: **the error is located at `s14P.c:49`, a line of
the compiler's with no `#line` to the `.hero` member**, so (P) needs a reader
that maps a static-assert line back to its `extern` member, as the c-boundary
class's members do; the ffi-pragmatist's measured location for `sprintf`
(`call.hero:8:116`) came from the probe line, which has one. Constants and
records under (P): the ffi-pragmatist measured `OLD_LIMIT` refused at exit 1
(§ 4 table); the record shape under (P) is unrun by anyone.

**C2. The ffi-pragmatist, § 2 table: `unavailable`, "today's verdict: exit 1
already".** Contradicted by its own § 3b (*"exits 2 with internal error"*)
and by the compiler-engineer's s6 (exit 2, *internal error: compiling the
generated C failed*). The § 2 cell is clang's verdict, not Heroes'. Filed
since as defect 588 (`git show --stat 87794631`).

**C3. The ffi-pragmatist: the library switch is "the way out"
(`_POSIX_C_SOURCE` for `mktemp`).** Asserted as costless; it is not. A
first-group header defining a feature-test macro changes every later
header of the unit. Measured in plain C (`pcheck/posix.c`,
`#define _POSIX_C_SOURCE 200809L` then `<stdlib.h> <string.h> <stdio.h>` and a
call of `strlcpy` and `arc4random`, `clang -std=gnu11 -Wall -fsyntax-only`):
**exit 1, `call to undeclared library function 'strlcpy'` and `call to
undeclared function 'arc4random'`**. So on macOS the way out for `mktemp`
withdraws BSD names the same program may bind beside it. Whether Heroes'
first-group switch reaches every group of the unit, and so refuses those
bindings as undeclared, is unrun (I built no compiler); spec § 13 and panel
205's R3 are where it is ruled. `OPENSSL_SUPPRESS_DEPRECATED` has no such
cost measured either way.

**C4. Memory safety (§ 1.12): does refusing a deprecated name close a class?**
Nobody measured it; both building seats and the historian name `sprintf`,
`mktemp`, `tmpnam` as plausible mistakes. Counted on the ffi-pragmatist's
own lists (`grep -cw <name> 208-ffi-pragmatist/c/mac_deprecated.txt` and
`linux_deprecated.txt`): `strcpy` 0 and 0, `strcat` 0 and 0, `strncpy` 0 and
0, `memcpy` 0 and 0, `scanf` 0 and 0; `sprintf` 1 and 0, `vsprintf` 1 and 0,
`gets` 1 and 0. So **deprecation marks no buffer-writing function on Linux
and three of eight on macOS, and `strcpy` nowhere**: refusing what a header
marks deprecated closes no memory-safety class on any platform; it is advice
made into a verdict. (The instrument takes each name's address, which a
function-like macro does not hide, so the zeros are not fortify's blind
spot.) Whether a Heroes program can even overrun through a bound `sprintf`
(what the boundary lets a `[u8]` or `cstr` argument be) is unrun here.

**C5. The compiler-engineer's (S2) silences `-Wattribute-warning` (§ 9).**
The ffi-pragmatist says the opposite must not happen: *"glibc's fortify uses
it for provable buffer overruns ... silencing it with deprecation would hide
a real bug"* (§ 4, left open), itself marked *a question rather than a
measurement*. Neither side built a glibc fortify overrun. This is the one
place robustness (precedence rank 3) is at stake in the sitting, and it is
unrun: whether Heroes' Linux units define `_FORTIFY_SOURCE` at all (Debian's
clang does not by default; the compile words are
`-std=gnu11 -Wall -fsigned-char`, ffi-pragmatist § 2), and whether a
`__warnattr` call is always followed by a `__chk_fail` abort at run time, so
the warning's loss is a loss of advice and not of the guard.
**Settled in part, 16:05.** In the Linux image, `docker run --rm
heroes-linux-arm64:latest sh -c 'echo | clang -dM -E - | grep -i fortify'`
and the same with `-O2` print **nothing**: Debian's clang defines no
`_FORTIFY_SOURCE` at either level, and `grep -n -E '"-D' selfhost/cli/flags.hero`
in my tree names only `-D_USE_MATH_DEFINES` and `-D_CRT_SECURE_NO_WARNINGS`
(lines 125-126). glibc's `__warnattr` headers exist in the image (7 files,
`grep -ln '__warnattr\|__warndecl' /usr/include/*/bits/*.h ...`:
`poll2.h`, `select-decl.h`, `socket2.h`, `stdio2-decl.h`, `stdlib.h`,
`unistd-decl.h`, `wchar2-decl.h`) but are inert without `_FORTIFY_SOURCE`.
On this Mac's SDK one header uses the attribute at all
(`grep -rlE '__warning__|attribute__\(\(warning|__warnattr' $(xcrun --show-sdk-path)/usr/include | wc -l`
gives **1**, `curl/typecheck-gcc.h`), and `curl.h:3221-3224` includes it only
for `__GNUC__` 4.3 or later, where clang reports 4.2 (`clang -dM -E`). So by
default **S2's silencing of `-Wattribute-warning` hides nothing glibc or the
SDK would say in a Heroes unit today**; it would hide glibc's provable
overruns in a program whose own first-group header defines
`_FORTIFY_SOURCE` (an inference from the headers; such a program unbuilt).
The ffi-pragmatist's objection stands for that program only, and the
synthesis should say whether S2's ignore is scoped to exclude it.

**C6. "Neither route slows the compiler" (compiler-engineer § 4)** is
measured in instructions retired and reads within noise; sound. (R2) and
(S2) are not costed, and their `heroes test` is unrun (§ 9 says so).

**C7. "0 of the 21 `examples/` files move" (both)** is measured. It is not
the whole of the tree's programs: `tests/golden/run/` holds three that bind
deprecated OpenSSL names behind a switch (F5), and the `warnings` suite
(`tests/harness/suite_warnings.hero:1`, *"The generated C compiles with NO
warnings at all, over the goldens and examples"*) is the instrument that
would see a deprecated use without one. **No seat ran `warnings`, `run` or
the census of `check` against (S), (R) or (P)**; the compiler-engineer says
so (§ 3, *Not run*).

## 3. The contradictions, and which are checkable

**X1, the central one: (S) by the compiler-engineer and the historian
against (P) by the ffi-pragmatist.** Judged against the grounds the
coordinator named:

- *The thesis, every plausible LLM mistake a compile error.* (P) rests on
  the premise that binding `SHA256_Init`, `sprintf`, `mktemp` is a mistake a
  model makes. **Unmeasured**: the sitting runs no blind seat and no paid
  run (the shared brief says what that gives up), and every seat says so.
  It is the one fact that would flip the compiler-engineer's own
  recommendation (its § 7, *what would make it wrong*).
- *Robustness, §1.12.* C4: refusing a deprecated name closes no memory
  class on Linux and leaves `strcpy` everywhere; (P) is not a robustness
  route, so rank 3 does not lift it over rank 4. C5 is the only robustness
  question, and it cuts against S2's silencing of `-Wattribute-warning`
  only for a program that defines `_FORTIFY_SOURCE` itself (measured: no
  default unit on either platform activates the attribute), not against
  (S) on the deprecation groups.
- *A correct program refused.* Two records say these programs are correct:
  panel 205 (ratified) for a `getcontext` binding, defect 396's golden header
  for a `SHA256_Init` call (F5). (P) refuses both unless the switch is set,
  and `getcontext` on macOS has no switch and no replacement (ffi-pragmatist
  § 2). By the class list that is `blocking` on the day (P) lands, unless the
  sitting rules a deprecated binding incorrect, which overturns 205's
  wording: the synthesis must say which.
- *A verdict that differs by platform and by SDK.* Measured by the
  ffi-pragmatist: the platforms disagree on 4 of 13 rows; the SDK's
  `sqlite3.h` marks 13 functions, Debian's 0; `availability(macos,
  deprecated=99.0)` is silent today, so the verdict also moves with the
  deployment target and the SDK installed, on one platform (the macOS 13
  `sprintf` story the historian cites). Under (P) a program refused on the
  CI's macOS leg builds on Linux. Its own prediction says so.

Checkable now, and checked: the parameterless-function half of the dispute
(C1, for (P)). Not checkable in the time: (P) on a record in a signature,
(P)'s location mapping from a static-assert line, and both routes against
`warnings`, `run` and the census.

**X2. Exactness of (R).** Both building seats measured that (R) at the use
accepts `sprintf` (fortify macro) and a deprecated constant read through the
accessor, and refuses `twice`. Agreed; (R) as worded in the brief does not
exist as built. The compiler-engineer prices the exact form (bare name asked
at each call, `(void)sizeof(&(X))`, measured to warn in `ctest/n.c`); the
ffi-pragmatist calls the exact form (P). These are two different rules,
*uses* against *bindings*, and the compiler-engineer names it as the ruling
(R) owes. A binding never called (s2, s11, s14) is accepted by one and
refused by the other.

**X3. `-Wdeprecated-pragma`.** Both found it outside 571's QUIET; filed as
defect 587. Agreed.

**X4. A raw warning at a compiler line on a deprecated record** (the
compiler-engineer's s3, `s3record.c:67:12`, `struct old_pair t2;` in `main`'s
prologue, which 571's `names_a_group_type` misses). Only one seat found it;
`grep -rl -E 'names_a_group_type|old_pair|prologue.*deprecat' issues/` on the
trunk gives nothing, so it is **unfiled** (the s6 and s8 findings were filed
as 588 and 587). It is 571's class, `blocking`, under every route but (R)/(P)
where the record is refused first.

## 4. Routes nobody listed, and the question not asked

**G1. Go's split (the historian): the build silent, the advice a separate
`heroes` question.** Under `.claude/rules/cli-surface.md:18-20` it enters
only if the fixpoint, the golden harness or the Part 11 harness must type
it, or it has a measured Part 11 effect: none of the four is true today
(nothing types it; no effect measured). Its shape would be a subcommand
(*a different question*, a different artifact class, lines 22-23), and
panel 179's `heroes probe` entered by the author's decision with the rule
*not stretched to cover it* (lines 31-38). So: **not admissible by the
stopping rule; admissible only as the author's decision, as `probe` was.**
Its cost is unpriced. It is the only route on the table that keeps the
header's advice AND the one-verdict build.

**G2. Split by whose header it is.** Nobody listed: refuse a deprecated
use where the declaration lives in a header of the program's own tree (the
reproducer's `dep.h`, which the program's author deprecated), silence it
where it lives in a system or package header (advice from a third party
whose verdict moves with platform and SDK). The note clang prints names the
declaration's file (*"note naming `./dep.h:2:16`"*, the shared brief; the
compiler-engineer's R reader already reads clang's text), and panel 205's R2
already distinguishes headers by where they were found. It answers X1's
platform objection and keeps `twice` refused. Unbuilt and unpriced.

**G3. Apple's own per-language refusal.** The historian read that
`sprintf` carries `__swift_unavailable("Use snprintf instead.")`
unconditionally (`_stdio.h:275-280`): the SDK already says which names it
refuses to a newer language. A route reading `availability(swift,
unavailable)` as the header's refusal for Heroes is macOS-only and unbuilt;
listed for completeness.

**The question the sitting should have asked and did not**: *is a
deprecated C name, bound or called, a mistake in the program, or advice
about it?* Every seat's verdict turns on it, the class list cannot decide it
(both outcomes are `blocking`), and the record had half an answer (panel
205: correct, for a binding; defect 396's golden: correct, for a call) that
no seat cited. A second question follows from it: **may one program's
verdict depend on the SDK installed?** No sentence I found answers it
(searched: `grep -n -i deprecat` over design.md and the spec, 0 and 0; and
`grep -n -i -E 'same verdict|one verdict|verdict.{0,40}platform|platform.{0,40}verdict|every platform' docs/design.md`,
one hit, line 3042, about marks, not platforms). A question, not a
finding: other wordings may exist.

## 5. What would change the resolution, most important first

1. **The record already calls these programs correct** (F5): panel 205,
   ratified, for a `getcontext` binding; defect 396's `run` golden header
   for a `SHA256_Init` call, with the switch in a first group already the
   practice. (P) or (R) refuses what the record names correct, so the
   synthesis either rules a deprecated use incorrect, saying it amends
   205's wording, or takes the `blocking` it creates.
2. **Deprecation is not a memory-safety instrument** (C4): `strcpy`,
   `strcat`, `memcpy` are marked on neither platform, `sprintf` only on
   macOS; refusing closes no class, so §1.12 does not lift (P) above
   Principle 0, and the thesis argument for (P) is unmeasured (no blind
   seat). By default nothing S2 silences carries a glibc or SDK overrun
   warning (C5, measured); a program defining `_FORTIFY_SOURCE` itself is
   the one exception.
3. **(P) is more exact than the compiler-engineer priced** (C1, measured:
   the static assert refuses a never-called `RSA_new()` under an `error`
   region), but its refusal of a parameterless function lands at a
   compiler line with no `#line`, an unpriced reader; and its way out
   `_POSIX_C_SOURCE` withdraws `strlcpy` and `arc4random` from the same
   unit on macOS (C3, measured in C).
4. **Two routes nobody listed** keep the advice without a per-platform
   verdict: Go's split (G1, admissible only by the author's decision, as
   `heroes probe` was), and the split by whose header it is (G2: refuse
   where the program's own header deprecates, silence a system or package
   header's). And one beside-defect is unfiled: s3's raw warning at
   `main`'s prologue temporary (X4).

Finished 16:06 by `date`.

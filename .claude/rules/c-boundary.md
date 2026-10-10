---
paths:
  - "runtime/**"
  - "examples/**"
  - "selfhost/emit/ffi*"
  - "selfhost/parse/group.hero"
---

# The C boundary

Home of CLAUDE.md § 12's FFI half and § 7's clang-failure classes, since
2026-09-07. What each rule cost to learn is in `docs/records/contract/case-law.md`,
cited as `CL-NNN`.

## Why this boundary outranks the rest

Everything comes from C (design.md §1.11), so everything a program touches
arrives through §4.19, and this is where the language's own guarantees stop.
Robustness is a **goal of the language** and it wins here first: a Heroes program
must not segfault and must not corrupt memory (CL-012).

The author's instruction is that the FFI be **complete and bug-proof** (CL-028).
Complete, because a library Heroes cannot bind is a library the author must leave
C code around for. Bug-proof, because a binding is the one place where a Heroes
program can reach an address nobody checked.

## What the compiler already shuts

A null `cstr` reaching a C function is `panic: a null cstr was passed to a C
function`, exit 134, because `guard_arguments` wraps every `cstr` argument
on its way out (measured twice on 2026-08-24). `to_str` on a null `cstr` is a
clean abort in the runtime.

**The function's name was wrong here until 2026-09-15**, and it is corrected
rather than annotated because a rule file is read for its citations: this line
said `guard_cstr_arguments` and the function is `guard_arguments`,
`selfhost/emit/ops.hero:244`, called at `:121`. Panel 154's compiler-engineer
found it while pricing a symmetric guard for handles, and the stale name had
already been copied out of here into that sitting's own briefs and into defect
045's entry — which is what a citation nobody greps costs.

**Say this rather than the danger it replaced.** A contract that states a danger
the compiler has already closed funds the wrong decision next time, and it
nearly did once: a token payment read as *deleting a warning* when the compiler
had become loud in both directions (CL-028).

## A clang failure that the author's own extern caused

A clang failure is normally exit 2 and says the **compiler** is wrong. One class
is exit 1 with a diagnostic on the `.hero` line, and it has twelve members
(panel 036, widened by panel 048, CL-008; the fifth by panel 166; the sixth by
defect 360, 2026-10-06, with no sitting, on defect 060's precedent: who is
blamed moves, the line between accepted and refused does not; the seventh,
eighth and ninth by panel 202, ratified 2026-10-10, its R2; the tenth by its
R3, the same day; the eleventh by panel 208, ratified 2026-10-10, its R2,
which counted it the seventh on the trunk's list of that hour; the twelfth
by defects 591 and 592, 2026-10-11, with no sitting, on defect 360's
precedent):

- a result type the header refutes, and a record's constant whose header value
  C will not build as that record, judged by the accessor's own declaration at
  the constant's line, told `ffi_constant_type` with no new code (defect 094,
  lane b12-ffi13, repaired at `41c5d1b4`, 2026-10-06);
- a `constant` that is not one;
- a name the header does not have;
- a symbol the **linker** cannot find because the group named no `link`;
- **a `ptr` lend C would write through, rooted at a name the program may not
  write** — added 2026-09-19 (panel 166, defect 065). The lend crosses as
  `const void *` from a `=` binding or a by-value parameter, and clang's own
  qualifier error against the header's parameter is the verdict;
  `emit/ffi_lend.hero` reads it into `field_lend_written` on the lend. This is
  the one member that points at a **call** rather than at a declaration.
- **a header a group names, or one it includes, that clang refuses on its
  own**: added 2026-10-06 (defect 360, batch 12, repaired at `e0dc8780`). Told
  `ffi_header_refused` at exit 1 on the group's string with clang's located
  words, narrowed by the include stack as `ffi_missing_header` is; until then
  it was *internal error: compiling the generated C failed* at exit 2, the
  author's header blamed on the compiler.
- **two headers that each compile alone, refused together** in one unit:
  added 2026-10-10 (panel 202; defects 538, 550, 557 and 561, repaired in
  batch 18). Told `ffi_header_refused` on the group whose header sorts first,
  both headers and both lines named, a file one of them includes named with
  the header it arrives through (`cli/headers_together.hero`); a header that
  compiles alone and fails after an earlier one, with no note in it, is
  found by compiling the two alone (`cli/header_alone.hero`). Each module's
  unit reads its own groups' headers, so the two are one module's, or
  `heroes test`'s one unit's.
- **one C name two modules' headers declare two ways**, external in both:
  added 2026-10-10 (panel 202's C1, defect 555). Told
  `ffi_declared_two_ways` before the link, both files and lines and clang's
  canonical types named, a `static` definition passed over
  (`cli/two_ways.hero`); a name no group binds is held to it too, the
  sitting's widening, which refused no tracked program the bound names do
  not.
- **one C name the units of two modules would each define**: added
  2026-10-10 (panel 202's C2, defect 556). Told `ffi_defined_twice` before the
  link on every platform, a tentative definition included, the files, lines
  and modules named (`cli/defined_twice.hero`); until then the link said
  *duplicate symbol* at exit 2, and a tentative one linked on this Mac and
  not on Linux.
- **the one file `--emit-c` writes, where it cannot mean the program the
  units build**: added 2026-10-10 (panel 202's R3, defect 453). Told
  `ffi_one_file` at exit 1, nothing written, where a module's unit reads
  other headers than the file and clang refuses the file, naming the two
  modules and their headers, or where a module's binding has another C type,
  another macro or another dump in it (`cli/one_file.hero`); until then the
  file was written at exit 0, clang never reading it.
- **a name its header marks `unavailable`**, a function, a constant or a
  record, called or only bound: added 2026-10-10 (panel 208's R2, defect
  588). Clang refuses every use of it, the compiler's own probe of the binding
  included, and `emit/ffi_unavailable.hero` reads that error, wherever in the
  unit clang names it, into `ffi_unavailable` on the binding, once, the
  header's own words in a note; until then it was *internal error: compiling
  the generated C failed* at exit 2. A name its header marks deprecated is
  not a member: it binds and builds silent (design.md §4.19, panel 208's R1).
- **a linker name a header gives a C name, by `#pragma redefine_extname` or
  an `__asm__` label, that the linker cannot find**: added 2026-10-11
  (defects 591 and 592, lane b20-pragma). Read on the link's failure path
  from clang's dump of the module's header list, followed by the unit's own
  `heroes_standard.h` and `main` (`cli/renamed.hero`, `AsmLabelAttr`): the
  label of a function a group binds is told `ffi_missing_link` on the
  binding, the file and line clang gave it named; the label of a name the C
  this compiler writes uses, a runtime function read before the groups, a
  function of `<math.h>` read after them, or `main` renamed away, is told
  `ffi_header_refused` on the group's header. C has no way to take a label
  back (measured on this Mac: a second pragma does not replace it, a label
  written after it is *conflicting asm label*), so the program is told rather
  than protected; until then *internal error: linking failed* at exit 2.
  A label is the symbol as written, with no platform prefix, so
  `redefine_extname labs llabs` fails on Darwin and links in the Linux arm64
  image, where `llabs` is the symbol's own spelling.

**The narrowing is `declaration()`, not whose text it is.** Every class recovers
a name and asks whether *this program* declared it `extern`, so a symbol nobody
declared stays exit 2 and the compiler's fault. The fifth member recovers the
lend from the IR at the line clang names, which is the same question asked of
the call: a qualifier error at a line where no immutable lend is passed to C
stays exit 2.

## The instruments

`--sanitize` adds `-fsanitize=address,undefined`, which catches use-after-free
and double-free. **Leaks are caught by `hero_runtime_check_leaks()`**, because
ASan's leak detector does not exist on Darwin arm64.

**Since 2026-10-07 the `run` suite also sanitises at `-O0`, gives both
sanitised runs ASan's free fill, and runs the `-O0` binary under Guard Malloc
on Darwin** (defects 390 and 321, lane b14-cli): ASan at `-O2` missed a stack
overwrite `-O0` catches, and a C library reading the bytes of a string already
freed was seen by no leg. LeakSanitizer runs in the Linux arm64 container too
(measured that day, a 77-byte leak; `.claude/rules/platforms.md`).

**A program that declares an `extern` runs its Linux leg under `--sanitize`**
(CL-055). LeakSanitizer exists on that leg and on no other, so a leak in a C
binding is invisible on this Mac in all three configurations. The rule is narrow
on purpose: a program without an `extern` cannot leak from the C side, because
its own allocations are counted on every platform.

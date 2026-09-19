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
is exit 1 with a diagnostic on the `.hero` line, and it has five members
(panel 036, widened by panel 048, CL-008; the fifth by panel 166):

- a result type the header refutes;
- a `constant` that is not one;
- a name the header does not have;
- a symbol the **linker** cannot find because the group named no `link`;
- **a `ptr` lend C would write through, rooted at a name the program may not
  write** — added 2026-09-19 (panel 166, defect 065). The lend crosses as
  `const void *` from a `=` binding or a by-value parameter, and clang's own
  qualifier error against the header's parameter is the verdict;
  `emit/ffi_lend.hero` reads it into `field_lend_written` on the lend. This is
  the one member that points at a **call** rather than at a declaration.

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

**A program that declares an `extern` runs its Linux leg under `--sanitize`**
(CL-055). LeakSanitizer exists on that leg and on no other, so a leak in a C
binding is invisible on this Mac in all three configurations. The rule is narrow
on purpose: a program without an `extern` cannot leak from the C side, because
its own allocations are counted on every platform.

---
paths:
  - "runtime/**"
  - "examples/**"
  - "selfhost/emit/ffi*"
  - "selfhost/parse/group.hero"
---

# The C boundary

Home of CLAUDE.md § 12's FFI half and § 7's clang-failure classes, since
2026-09-07. What each rule cost to learn is in `docs/contract/case-law.md`,
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
function`, exit 134, because `guard_cstr_arguments` wraps every `cstr` argument
on its way out (measured twice on 2026-08-24). `to_str` on a null `cstr` is a
clean abort in the runtime.

**Say this rather than the danger it replaced.** A contract that states a danger
the compiler has already closed funds the wrong decision next time, and it
nearly did once: a token payment read as *deleting a warning* when the compiler
had become loud in both directions (CL-028).

## A clang failure that the author's own extern caused

A clang failure is normally exit 2 and says the **compiler** is wrong. One class
is exit 1 with a diagnostic on the `.hero` line, and it has four members
(panel 036, widened by panel 048, CL-008):

- a result type the header refutes;
- a `constant` that is not one;
- a name the header does not have;
- a symbol the **linker** cannot find because the group named no `link`.

**The narrowing is `declaration()`, not whose text it is.** Every class recovers
a name and asks whether *this program* declared it `extern`, so a symbol nobody
declared stays exit 2 and the compiler's fault.

## The instruments

`--sanitize` adds `-fsanitize=address,undefined`, which catches use-after-free
and double-free. **Leaks are caught by `hero_runtime_check_leaks()`**, because
ASan's leak detector does not exist on Darwin arm64.

**A program that declares an `extern` runs its Linux leg under `--sanitize`**
(CL-055). LeakSanitizer exists on that leg and on no other, so a leak in a C
binding is invisible on this Mac in all three configurations. The rule is narrow
on purpose: a program without an `extern` cannot leak from the C side, because
its own allocations are counted on every platform.

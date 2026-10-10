# Panel 204, completeness critic, first pass

Copied by the coordinator at 01:29 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 204, completeness critic, first pass

I read the briefs from 01:17 to 01:29 on 2026-10-10 (`date`), and resumed at 01:27:59 after the coordinator's process ended. The compiler in my copy had finished building at 01:17 and was intact (`heroes 0.2.0`, Apple clang 21.0.0), so I did not rebuild it.

- **Tree:** the frozen tree `635e8f67`, rsynced to `.claude/worktrees/scratch-b15/critic-204/tree/` without `.claude/worktrees`.
- **Cases:** copied to `critic-204/cases/`, their `build/` folders and binaries removed before I re-ran them.
- **My own shapes:** under `critic-204/shapes/` and `critic-204/verify/`.
- **Report file:** the Write tool refused `report-pass1.md` ("Subagents should return findings as text"). This reply is the whole report and nothing is on disk; the coordinator should copy it into `docs/panel/204-reports/`.

I give no verdict on the question.

## The brief's three cases, re-run: all three reproduce

- **`cfgone`:**
  - `main.hero` builds at 0 and prints `3` `50`.
  - `main2.hero` builds at 0 and prints `3` `10`, with clang's `warning: 'LIMIT' macro redefined [-Wmacro-redefined]` on stderr, in `run` too.
  - `run` agrees with `build` in both orders.
- **`jpeg`:**
  - `ab.hero` builds at 0 and prints `1`.
  - `ba.hero` exits 1 in `build`, `run` and `build --emit-c`, with the brief's message word for word.
  - `check ba.hero` exits 0.
- **`one`:** `build` exits 1 with `ffi_header_refused`, and the note says *whichever comes first*, as the brief quotes.

## Repairs to the briefs, each from a command I ran

1. **Today the include order is not the order the groups are written in, and the brief assumes it is.**
   - **The code:** `selfhost/emit/externs.hero` `headers()` (lines 101-133) walks `p.functions` first and the record declarations second. A group holding only `record`s therefore has its header emitted after every group with a function or a constant, wherever it is written.
   - **`shapes/recfirst/rec.hero`:** `extern "stdio.h"` holds only `record CFile tag FILE` and is written above `extern "jpeglib.h" package "libjpeg"`, the order the brief calls the one that works. `check` exits 0. `build` exits 1 with `ffi_header_refused`, *unknown type name 'FILE'*, and the same false advice.
   - **`recfn.hero`:** the same program with a `function puts` added to the stdio group builds at 0 and prints 1.
   - **`con.hero`:** a stdio group holding only a constant, written first, builds and prints `-1`. Constants keep their place.
   - **Emitted C:** `verify/ro.hero` is written `time.h` (record only) then `stdlib.h`; `--emit-c` writes `<stdlib.h>` before `<time.h>`.
   - **Over the 107 files:** 20 of them have an emitted order different from the written one. This comes from an awk reading of the source (`critic-204/order.awk`), not from each file's emitted C, so it is an inference.
   - **What becomes false:**
     - *The program needs the `stdio.h` group first* holds only when that group holds a function or a constant.
     - Panel 091's *the author's only lever on include order is group order* is false for record-only groups.
     - E1 (*C, which reads a file's headers in order*) is false on today's compiler for `rec.hero`.

2. **Every unit starts with a hidden group of the compiler's own, and the brief does not say so.**
   - **The unit's layout** (`recfn.c` lines 2-9), in order:
     - `heroes_runtime.h`, which includes `stdbool.h`, `stddef.h` and `stdint.h` (`runtime/heroes_runtime.h:54-56`);
     - `<math.h>` and `<hero_os.h>` (`SEEDS`, `externs.hero:79`);
     - `heroes_guard_open.h`, then the groups' headers, then `heroes_guard_close.h`.
   - **Why `ba` fails at `FILE`, not `size_t`:** `ba` is refused at *FILE* (line 984) and not at *size_t* (line 855) only because that prefix supplies `size_t`. `#include <jpeglib.h>` alone gives 8 errors, *unknown type name 'size_t'*. After `stddef.h` it gives 2, *FILE* (`shapes/j1.c`, `j2.c`).
   - **A header that fails alone builds in Heroes:** `shapes/zeroth/needsz.h` uses `size_t` and `HUGE_VAL` with no include. It gives 3 errors compiled alone, yet it builds and runs in Heroes (`42`, `inf`). Per-group units (route f) would refuse it unless they keep the prefix.
   - **A documented switch cannot be placed first** (`shapes/sw/`). `cfg.h` defines `_POSIX_C_SOURCE 200112L`.
     - In plain C, that header before `<string.h>` hides `strlcpy` (`call to undeclared function`, exit 1). After `<math.h>` it does not (exit 0).
     - In Heroes, `extern "cfg.h"` written first, then a `string.h` group binding `strlcpy`, builds.
     - The switch has no effect and no group order can express it. `_GNU_SOURCE` on Linux is unrun.
   - **A configuration-only header cannot be named on its own:** a group with no member is refused, `expected_extern_block` (`shapes/sw/empty.hero`).

3. **The sentence panel 091 said was owed was filed, and the issue is still open.**
   - The brief's *never filed as an issue* rests on a grep whose words miss it: the issue writes *`#include` order*, with a backtick between the two words.
   - The file is `issues/2026-09/07/2026-09-07-0000-four-repairs-to-design-md-that-ride-its-opening-sitting-4-19.md`: `kind: task`, `milestone: M-core-packages` (scheduled, ROADMAP row 64), `- [ ]`.
   - Its text: *§4.19 owes one sentence: a group's `#include` order is load-bearing ... nothing stops a later pass from reordering or thinning that list*.
   - I found it with `grep -rln -i -E 'jpeglib|libjpeg|thinned|reordered' issues`.

4. **Panel 202's condition for this sitting is not met, and the brief does not say so.**
   - Panel 202's R4 sends defect 563 to a sitting *once R3's instrument exists*.
   - At `635e8f67`, defects 453 and 560 are still open (`- [ ]`). R3's comparison of dumps is unbuilt, and lane b18-ffi sits at `86189b2b` with nothing committed.
   - The brief should say the sitting convenes before that condition, on the author's *zero defects*. It should also say that a prototype of the comparison route amounts to building R3's instrument.

5. **`cfgone/main2` also prints a clang warning on a correct program.** The brief omits it, while defect 563's own line states it. Under § Bounded discovery a clang warning on a correct program is `blocking`, so it is part of the case.

6. **The carried census matches its file, but it measures a different unit, and failure only.**
   - **Mac:** `mac/unordered.tsv` reads 12,066 / 16 / 8, as the brief says (12,090 is 156 choose 2).
   - **Linux:** `linux/unordered.tsv` (2,775 pairs) reads 2,683 / 4 / **88**. The brief gives the 4 and drops the 88 pairs that fail in both orders.
   - **A different unit:** `census/pair.sh` compiles the two headers alone at `-std=gnu11`, which is the build's own flag (`cli/flags.hero:111`), but without the prefix and guard from repair 2.
   - **Failure only:** it does not count pairs that compile in both orders and mean different things, which is `cfgone`'s class.

7. **The emitter already drops a repeated header.** `headers()` keeps only a header's first occurrence. `shapes/dup/bab.hero` names `a.h`, `b.h`, then `a.h` again (`a.h` has no include guard). `<a.h>` is emitted once, and the program prints `3 50 3`. So panel 091's *never reordered or thinned* is false today twice: repair 1 and this one.

8. **Smaller points.**
   - The brief's second grep also matches the panel 202 ratification issue and this sitting's own `00-shared.md`.
   - Checked and true:
     - the 107 of 3,195 and its split by folder (my list is identical to `two-headers.txt`);
     - `spec:113`;
     - panel 091 lines 224-227;
     - E1's text and its +12 (`202-reports/spec-warden.md:15,56`);
     - ABI 30;
     - `seed/`, `selfhost/` and `runtime/` identical between `86189b2b` and `635e8f67`;
     - the three lanes exist;
     - `heroes fmt` keeps the groups' order.

## The shapes beside the cases

- **`one` in both orders:** swapped (`shapes/one2`), it still fails `build` with the same words, including *whichever comes first*. The message is placed on the `a.h` group (now line 4) and says *the other group names `b.h` at line 1*. So the message orders the headers by bytes, not as written.
- **A header needed by one two groups below:** `stdio.h`, `math.h`, `jpeglib.h` builds. Reversed, it is refused with the same message.
- **`jpeglib.h` alone** (`three/alone.hero`) gets the same words. A message naming *the group to move above* has nothing to name here; it would have to name a header to add. Clang does not name that header in one run: after `stddef.h` it says *unknown type name 'FILE'* with no note, and `-fmodules` adds nothing.
- **Three headers where two fixed orders agree and the module still depends on order** (`shapes/mpq/`):
  - `p.h` and `q.h` each `#define K 5`; `m.h` has `#ifndef K` / `#define K 9`.
  - The written order `p m q` and its reverse `q m p` both print `5 5 105`.
  - `m p q`, which is also the byte-sorted order, prints `9 5 105` with `-Wmacro-redefined`.
  - So comparing the written order with its reverse passes a module that depends on order. A canonical sort changes this program's value at exit 0.
- **A function bound in one group but declared only by another group's header** builds today. In `shapes/dup/bab2.hero`, `third` is bound under `b.h` and defined in `a.h`, and the program prints `3 10 3`. Per-group units would refuse it.

## Routes nobody listed, and what would have to be true for each

1. **The group names its prerequisites:** `extern "jpeglib.h" after "stdio.h"`, or an ordered list in one group head. This needs a grammar change to § 13 accepted under Principle 0, and no cycles between groups' prerequisites. It gives `cfgone` no answer.
2. **A header of the program's own is the only way to state an order**, and the messages draft it for the author. This needs the probes to accept members declared by a header that the group's header includes (unrun), and the drafted header to be a `guess` fix, never `certain`.
3. **On a failure only, search the module's other groups for the missing header:** *n − 1* extra clang runs, then tell the author *move the `stdio.h` group above*. It says nothing for `alone`, which has no other group.
4. **Make the hidden prefix a stated rule, or move it after the groups** so a switch written first really is first. This needs the runtime's own C to still compile after a group's header has defined a feature macro, and a decision on `needsz.h`-shaped headers, which build today.
5. **Emit in written order, record-only groups included.** This is the only repair that makes E1 true. It needs no tracked program to depend on record-only groups coming last; the 20 files are where to look.
6. **Refuse a module whose preprocessed meaning differs across a bounded set of orders** (`-E -dM` and the dumps). The set has to be chosen so the `mpq` shape cannot pass, which no fixed pair of orders does.

## Questions the sitting should ask and does not

1. Is the order it rules on the written one, the emitted one, or neither (repair 1)?
2. Is the hidden prefix part of the order, and may an author put anything before it? What does § 13 owe a reader about it?
3. What should `jpeglib.h` alone be told, when there is no group to move?
4. How many installed header pairs compile in both orders but mean different things, measured on the unit a program is really compiled in?
5. Does a route that changes a program's value (a canonical sort turns `cfgone/main2` into `3 50` at exit 0) count as breaking that program?
6. Does the sitting close the open M-core-packages task (repair 3), and does panel 202's R3 condition (repair 4) still bind?
7. Does `test` read one module's groups in the same order as `build`?

## What I did not run

- Docker, Linux, `_GNU_SOURCE` on glibc, or the Windows box.
- `heroes test` on any case, or `--emit-c` on `cfgone`.
- The emitted C of the 20 files my awk flags. Only `verify/ro.hero` is confirmed in emitted C.
- Whether the probe units carry the prefix and the same order as the program's unit. `emit/macro_guard.hero`'s comment says they *include the groups alone*.
- Whether the 51 of 107 files that a byte sort would reorder (my awk's count) change exit code or output.
- No paid run of any kind.

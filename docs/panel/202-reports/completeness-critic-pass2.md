# Panel 202, completeness critic, second pass

Copied by the coordinator at 00:10 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

The cheap claims are reproduced, three new shapes are measured, and one blind reading was scored wrongly. The biggest gaps are four: route (c) has no defined `--emit-c`; it needs a spec sentence nobody drafted; the cheap repair for the probes is ruled out by a measurement; and (c)'s `test` change makes `onedef` worse unless C2 lands first.

I built my own compiler in three stages in `.claude/worktrees/scratch-b15/202-critic2/tree/`; it printed `heroes 0.2.0` at 00:06:29. Cases are in `202-critic2/cases/<name>/`, outputs in `out/`, and `202-critic2/run.sh` runs every verb on a case. Load was about 10, so no duration was taken. Nothing else was written.

## Findings

**1. fpat64 and fprec reproduce (synthesis question 2).**
- `fpat64`: check 0, build 0, prints `6 4294967299`, test 1. `fpat64ctl` is refused at build, exit 1. `fprec`: build 1.
- New sibling `fpwide`: `fill(@x: i32)` bound to `void fill(long *x)`, so C writes 8 bytes into a 4-byte value.
  - It builds at exit 0 and the binary aborts with exit 134: *"C reached past the local `y`… changed nothing beside it"*.
  - Its control without the conflict (`fpwidectl`) is refused at build, exit 1, `ffi_parameter_type`.
  - So the memory guard held on this one case. But a compile error is owed and arrives as a run-time abort, and the abort's advice (`counted_by … lent`) is the wrong fix: the right one is `i64`.
- Classification: all three are `blocking` (a wrong value, a correct program refused, a compile error told only at run time). They share one cause, the probes reading every header in one unit.
- I read `cli/pointee.hero:200-204`: it returns `ok()` when that dump unit fails to compile. The engineer's cause holds.

**2. New shape `skew`: a macro in one module's header changes another module's record, as the probes see it.**
- `a.h` holds `#define WIDE 1`; `b.h` defines `struct S` as `{long a; int b;}` under `#ifdef WIDE`, otherwise `{int a; int b;}`.
- Read on its own module's header, `a: i32` is correct.

| case | `use` order | build | test |
|---|---|---|---|
| `skew32` (`a: i32`) | left, right | 1, *"`S.a` is `long`… not `i32`"* | 1 |
| `skew32sw` | right, left | 1, ***"`S.a` is `int`… a signed 32-bit integer, not `i32`"*** | 0 |
| `skew64` (`a: i64`) | left, right | 1, ***"`long`, a signed 64-bit integer, not `i64`"*** | 0 |
| `skew64sw` | right, left | 1, *"`int`… not `i64`"* (true) | 1 |

- No declaration of `S` builds in either order, and two of the four messages contradict themselves. `test`'s verdict flips with the `use` order.
- **This rules out the cheap form of C0** ("probe the whole program first, per module only when that fails"): `skew`'s combined probe compiles fine and still answers wrong. C0 has to be per module every time.
- My guess at the mechanism, not measured: the verdict comes from one unit and the type name in the message from another.

**3. The blind seat's scoring misreports d3.**
- The scoring says both readers "rebind `right.hero` to `a.h`". In fact d3-a wrote `use left` and called `left.twice(...)`. On my build that is refused by `check` at exit 1, `extern_across_modules`.
- d3-b's repair passes every verb, and its `--emit-c` file compiles and prints `6 8`.
- So today's 538 message led 2 of 2 readers to edit a correct program, and 1 of 2 edits does not compile.
- Both readers also argued that the note's second remedy ("a header of your own that declares only what that group binds") cannot work for `fp`, because any declaration of `twice` conflicts with the other header's `static inline` definition. Unrun as a program.

**4. Under (c), `--emit-c` means three incompatible things to three seats.**
- Measured today: `--emit-c` exits 0 for `fp`, `fpat64` and `clash`, and clang refuses each written file (*conflicting types for 'twice'*). For `onedef` the written file compiles and prints `6 8` while `build` exits 2.
- The spec-warden's P1 predicts that `fp`'s compiled `--emit-c` file prints the per-module values. One C file cannot do that (C11 6.7p4; measured above).
- The engineer's C4 says panel 200 R1's compile catches `fp`, so `--emit-c` would exit 1. That contradicts his own "(c) prototype: 0 everywhere" row; `rc.summary` shows the prototype never compiled the file.
- The seed is one file from `--emit-c` (`seed/README.md:93`).
- So the synthesis must choose one of three:
  - `--emit-c` refuses with a true message, keeping R1 and a one-file seed (selfhost's headers share a unit);
  - several files, which changes the bootstrap;
  - reverse R1, which is ratified.

**5. The ffi-pragmatist's condition 2 contradicts its own approval of (c).**
- It asks that `clash` be refused at `check` by comparing parameter types across groups. `fp`'s groups bind `twice(x: i64)` and `twice(x: i32)`, so that comparison refuses `fp`, the program (c) exists to accept.
- Telling `static` from external linkage takes clang, and `check` asks clang nothing. The engineer's C1 at `build`, and the historian's "where a linker stands", are the consistent reading.

**6. `onedef`/`extdef`: the order of landing matters.**
- The engineer's `rc.summary` reads `onedef` test=2, against 0 in `base.summary`. So (c)'s `test` change shipped without C2 makes `onedef` worse.
- My build's linker message names the objects `left-<hash>.o` and `right-<hash>.o`. So exit 1 naming both modules is possible without the AST dump, but the wording differs per linker: ld64 measured, GNU ld and link.exe unrun. C2 via `-ast-dump=json` does not depend on the platform.

**7. The smallest first landing that removes the two wrong values and the two exit 2s, inferred from 1, 2 and 6 and unbuilt:**
- C0 per module (pointee, layout, `compiling.probe`), with C1 and C2 riding the same per-module dumps (C1 needs each declaration's storage class), at `build`/`run` only.
- `test` stays as one unit for now, telling its true messages, and 453 waits.
- (c)'s `test` half (with the root shim, so `main` does not run before each test) and C4 come second.
- The C0+C1+C2 subset is unpriced; the 200 to 350 lines cover all of (c).
- A one-condition stopgap: make pointee refuse at `:204` instead of returning `ok()`, as layout already does for `fprec`. It trades `fpat64`'s wrong value for refusing the correct `fpat` (the engineer's case). Unrun.

**8. Question 3: (c) does not make § 4 true for `cfgone`.**
- `cfgone` is one module, so under (c) its two headers still share a unit and their order still moves the value.
- The spec-warden's 0-token approval is for per-group isolation, route (f) ("only holds if groups are isolated per group, not per module"), which the engineer prices high and rejects. The synthesis cannot adopt (c) "at 0 tokens" on the spec-warden's word.
- For `macro3` (two modules), (c) aligns `build` and `test` by construction. It was unrun under the prototype (not among `rc.summary`'s cases), and `--emit-c`'s one file still computes 400 (point 4).
- So under (c), `one` and `cfgone` are owed one of three: a refusal, a sentence, or (f).

**9. Question 4: naming 538's class.**
- Proposed wording: *two headers one unit must read together, each of which clang compiles alone, refused together*. It stays `ffi_header_refused`, exit 1, on the group whose header sorts first.
- With (g), add "or a header one of them includes".
- Under (c) with C0, it shrinks to one module (538's own `one`). Today the probes also print it at `build` (`fprec`, `fpwidectl`).
- The note's "whichever comes first" is false (the ffi-pragmatist's `rlmath2`, `tasn1b`).

**10. Question 5: the blind seat against each route.** No arm compared two spec texts, so nothing it measured is specific to one route.
- **d1:** 2 of 2 readers predict `test` exits 0, and d1-b writes "`main` is not run by `heroes test`".
  - Against (a): contradicted, it refuses at `check`.
  - Against (b): contradicted, `test` and `build` refuse.
  - Against (c): agrees, but only with the test shim the prototype lacks.
  - Against (d): `test` still refuses.
- **d2:** d2-a predicts `build` refused with `check` 0, which is (c)+C1 or (b). d2-b foresees the wrong value as C's undefined behaviour. Neither predicts `check` refusing (d2-a gives that 30%).
- **d4:** both readers make `c.h` `static inline`. d4-b rejects "bind it from one module" as fragile, which is relevant to what C2's note recommends.

## Routes nobody built

- A canonical header order inside one module (sorted by bytes, as 538's message already orders them). It would make § 4 literally true at 0 tokens. The cost is programs whose headers need the author's order; measure it on the 107 tracked files that name two or more headers in one module.
- An order-free detector for one module: compile the unit in both orders and compare the preprocessed tokens.
- `--emit-c` refusing a program that cannot be one C file, with a true message (point 4).
- A census of tracked roots where two modules bind one C name with different Heroes types. It decides whether an interim Heroes-level refusal would hurt anyone.
- Running `skew` under (c) with C0. My prediction: `skew32` builds in both orders and `skew64` is refused truthfully. Unrun.

## Questions not asked

- The sentence (c) needs for `one` and `cfgone` ("a module's headers compile together") was never drafted or priced. Every draft is whole-program or per-group.
- What C2's note says, and whether a header that defines an external symbol may be named by more than one module.
- The historian's condition 1 ("fusing refuses nothing") is met by the engineer's tracked-tree census (0 of 1,323) and refuted by the ffi-pragmatist's installed-header census (18 failing pairs on Mac, 92 on Linux). Which population counts?
- The ffi-pragmatist's `c99` case, where `build` exits 1 and `run` exits 0, under C1.
- Windows (link.exe's duplicate-symbol wording, raylib × windows.h): unrun by every seat.

Not run by me: the (c) prototype, Linux, Windows, any suite, any duration.

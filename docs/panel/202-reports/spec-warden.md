# Panel 202, spec-warden

Copied by the coordinator at 23:21 on 2026-10-09 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

**Panel 202, spec-warden's report**

I ran from 23:06 to 23:20 (`date`) on my copy of `46b80b82`, with the compiler built in three stages. I grepped design.md §1.6 (`:255-256`): the ceiling is 10240 on `claude-opus-5`, and the payment rule is unconditional (`:311-314`).

- `verdict`: **veto** on every sentence a one-unit route needs: U1, U1s, U2, U3, U4 for routes (a) and (b), and D1 for route (d). **Object** to E1, C1, C3 and O1. **Approve** 0 tokens for the route where each group's binding is compiled with its own header alone, in every verb and inside one module too. The real deltas below are provisional; the 0 is exact.
- `section`: §1.0 (Principle 0), §1.2, §1.3 (it names macros among the non-local constructs to reject), §1.6, and spec § 4 `:113`. CLAUDE.md §12 (spec beats compiler).
- `spec_token_delta`: `./heroes measure spec/heroes-spec.md` reads legacy 7348, cl100k 7479, maximum 7479. That is a lower bound, not the reader's tokeniser. The pinned real count is **9847**. With the 60-token FFI floor, **333 tokens are spendable**, and panel 203, sitting beside this one, spends from the same 333.
  - The recommended route is +0.
  - Draft deltas on the vendored maximum, **lower bounds in those words**: U1 +19, U1s +14, U2 +12, U3 +11, U4 +31, D1 +21, E1 +12, C1 +9, C2 +12, C3 +20, O1 +6.
  - At the document's ratio of 1.317, U1s is about +18 real and U4 about +41. That is an inference: no `--refresh` was run.
- `removal`: none is needed for the 0-token route. Every draft has nothing to remove in exchange, and that is a problem.
- `needed_for_self_hosting`: no. Stages 2 and 3 built `selfhost/main.hero` module by module tonight.
- `argument`: The compiler does not need a one-unit sentence, and the sentence serves no thesis effect. A reader cannot see inside a header, so knowing that headers share one C file changes nothing they write, and §1.2 gets nothing back. The short wordings are also false by omission: `cfg` and `macro3` compile together and mean something else (50 for 10, 12 for 8). The only true wording, U4, states that a macro reaches across groups, which §1.3 refuses by name. And with no sentence, route (b) refuses `fp`, a program the spec accepts.
- `prediction`:
  - P1: under the recommended route, the spec stays at 7479 vendored and 9847 real. `cfgone`, `cfg`, `macro3`, `one` and `fp` then print per-module build's values in `build`, `run`, `test` and the compiled `--emit-c` artifact, in both orders.
  - P2: if route (b) lands instead, `heroes build probes/cfg/main.hero` prints `3 50` at exit 0 with no diagnostic.
  - P3: only if U1s is overridden in, its real delta is +16 to +22 at the landing's `--refresh`. Above 24, the wording is re-argued.
  - P4: in the critic's B2 blind repair task, readers with U1s in the prompt and readers without it choose the same repair in at least 3 of 4 pairs.
- `condition`: I would lift the veto for any one of these:
  - a closure-list program or a measured Part 11 effect that needs one unit;
  - the compiler-engineer measuring per-group isolation as unbuildable (for by-value group records or `constant`s), with the sitting adopting an order-free detector that refuses `cfg` and `macro3` in both orders. Then a refusal sentence is owed for truth, and U1s with P3 would be its payment;
  - a blind test where U1s changes a reader's binding in 2 or more of 4 sessions.

**Findings: the spec is already false today.** Probes are under `scratch-b15/202-spec-warden/probes/`, re-run on the stage-3 compiler.
- **§ 4's "Declaration order never matters" is falsified by `build` at exit 0.** An `Extern` is a `Declaration` in § 4's grammar. In `cfgone`, one module binds `a.h`, which has `#define LIMIT 100`, and `b.h`, which has `#ifndef LIMIT #define LIMIT 10` and `cap`. In the order a, b it prints `3 50`. Swapped, it prints `3 10`, with a clang `-Wmacro-redefined` warning on a correct program. `run` also prints `3 50`.
- **`test` and the `--emit-c` artifact compute values `build` does not.**
  - `cfg` (two modules) builds and runs to `3 10`. Under `test`, a correct test fails, `left: 50 right: 10`. The `--emit-c` artifact exits 0, and plain clang compiles it to a program printing `3 50`.
  - `macro3` (a macro `twice` beside a function `twice`) builds to `9 8`, but `test` computes 12 for 8.
  - Under panel 200's R1 this would compile with no diagnostic: the file written would be the file clang read, and still a different program.
- **`one` (538's own shape) is refused at `build` exit 1** while the spec says nothing that refuses it. So the 0-token route only holds if groups are isolated per group, not per module.
- I searched `issues/` and `docs/panel/` for `macro-redefined|macro redefined|include order|header order|order of two groups` and found no defect matching these shapes.

**Precedents (repair 13).**
- The § 13 tag sentence (`:394-395`) governs Heroes text the reader writes, so the reader can act on it.
- `contracts.agree` (`selfhost/checker.hero:85`, decision `issues/2026-09/25/2026-09-25-1152-one-c-function-one-contract.md`) is the nearer precedent: one C name declared in two files, a whole-program rule, and it landed with no spec sentence and no ledger row.
- Class 6 (defect 360) also landed with no sentence.
- So a `clash` refusal is owed for robustness, but no sentence is: header disagreement is not text the reader writes.

**Is 538's message a seventh class (repair 5)?** By its text, yes. Class 6 says *refuses on its own*, and 538 tells a pair whose headers each compile alone. The line between accepted and refused did not move, so defect 060's precedent covers landing it without a sitting. But `c-boundary.md:47` no longer describes what it tells, and this sitting should name it. Either way it costs 0 spec tokens.

**The drafts, whole** (the changed lines; full copies in `drafts/<name>.md`):
- U1s (§ 13 `:348`): `…when the symbols need one; a program's headers must compile together, in one C file.`
- U1: `…when the symbols need one. Every header a program names is compiled in one C file, so they must compile together.`
- U2: `A group names its header, compiled in one C file with every other group's, and `link` a library…`
- U3: `…two records may not name one tag but `void`, and two groups may not name headers that cannot compile together.`
- U4: U1 plus `, and a macro one defines reaches the ones after it.`
- C1 (`:395`): `…one tag but `void`, nor two headers declare one function two ways.`
- C2 (`:348`): `…when the symbols need one; two headers may not declare one C function two ways.`
- C3: `…one tag but `void`. Two groups declaring one function agree where they share a type, and their headers declare it alike.`
- E1 (`:113`): `Declaration order never matters but to C, which reads a file's headers in order;` (this also rewraps the next line).
- D1 (§ 12): `A program asked for its tests is compiled as one C file, so its headers must compile together.`
- O1 (§ 1): `…of the file you compile, and their order never matters.`

**What I could not run:**
- `--refresh`, per the brief, so every real delta is an inference.
- The `spec` suite on any draft, since none is recommended.
- The blind readers, which are the coordinator's.
- Per-group isolation itself, and its cost for group records passed by value and for `constant`s; those are the compiler-engineer's.

Files are in /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/202-spec-warden/:
- notes.txt
- scripts/drafts.py
- scripts/drafts2.py
- drafts/ (each draft and its `.measure` output)
- probes/cfgone, probes/cfg, probes/macro3, probes/cfgx, probes/one

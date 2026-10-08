# Panel 199, completeness critic, second pass

Copied by the coordinator at 00:33 on 2026-10-09 (`date`) from the critic's
reply, verbatim apart from this header: a subagent's Write of a report file is
refused, and its running notes are
`.claude/worktrees/scratch-b15/199-critic/notes.txt` (ignored by git).

---

I ran this from 00:15 to 00:32 CEST on 2026-10-09 (`date`) in `.claude/worktrees/scratch-b15/199-critic/tree`. That is a copy of 56def9b4 with `.git` removed, its compiler built from the seed with Apple clang 21.0.0. I copied in the engineer's four prototype files (identical by `cmp`) and built them as `heroes-proto`; it reproduces his table. My notes are `199-critic/notes.txt`. Every binary that might not end ran under `timeout 10`, its output through `head -c 4096`. Afterwards `pgrep -fl build/` shows two processes, both another session's (their working directory is `scratch-b15/emit/s501`). No paid run.

## 1. R3 is the cheap reading, and plausible endless programs pass it
R3 treats a call to any function declaration as a way out, the library's functions included (`self_call.hero:131-139`). The library holds `map`, `filter`, `fold`, `range`, `args` and `read_file`. Each program below gets `check` exit 0 from the prototype, then aborts 134 at both `-O0` and `-O2`:
- **`count(xs)` returning `xs.filter(is_even).count()`**: p3's mistake in its idiomatic form. It ends in `stack exhausted`.
- **`sum(xs)` returning `xs.map(double).sum()`**: it ends in `integer overflow`.
- **`fact(n)` returning `n * fact(pred(n))`**: p2's missing base case, with a helper. It ends in `stack exhausted`.

Clang warns, correctly, on all three.

The only correct program I found that R3 refuses is an endless function nobody calls (`deadfn`: prints 1, exit 0). So the engineer's condition should be worded per function ("a function that, when called, returns or ends the program"), not per program.

## 2. A route nobody listed: R4
R4 is R1 plus one rule: a call is a way out only when the callee may end the program. "May end" is followed through calls: a written end, an extern, a call through a function value or a field, or a callee that itself may end. Function arguments passed to the library's `map`, `filter` and the rest are followed too.
- **It refuses** the three programs above and `mixed`.
- **It spares** `calleeexit`, `callback`, `fieldcb`, `serve` and `hidden2`.
- **Its census is bounded, by inference.** R3's refusals are a subset of R4's, which are a subset of R1's. R1 and R3 each moved the same 2 of 2,980 tracked files (the engineer's first session, carried, not re-run), so R4 moves those same 2.
- **It is unbuilt.**

## 3. Contradictions, settled here
- **(F) and defect 507.** The engineer says `serve` builds clean after (F); the spec-warden says `hidden2` still warns after it. Both are right. With the engineer's edit, measured on clang 21.0.0 and 22.1.8, `serve` is clean and `hidden2` still warns at `8:39`. Route (K) on `hidden2` exits 1. So (F) does not close 507's class.
- **The shared brief's claim that (B) answers question 2 by construction** is false. `--dump-ir p2.hero` shows `sub! $t2, $t3` as one instruction with no branch, as the engineer said.
- **The spec-warden's sentence D1rnw** ("a function whose every path reaches a call of itself by name is a compile error") is false under R3, because `fact(pred(n))` gets `check` 0. Since the spec beats the compiler, D1rnw and R3 cannot both land.
- **The historian's inference about rustc** holds. rustc 1.90.0 warns on p2. It also warns falsely on the `hidden2` shape (the program prints 1 2 3 and exits 0) and correctly on `mixed`. So the shipping precedent carries 507's false positive, as a warning.

## 4. The question the sitting did not ask
What does unbounded recursion **mean** in Heroes: an abort (spec `:280`, route G) or a loop (route N)? Today it is both, depending on the level. `yes` (print, then call itself) runs at `-O2` until something stops it (12.9 MB in 10 s in the engineer's run), and aborts at `-O0`. Under N, `yes` becomes a correct program, and R3 refusing it is a correct program refused (`blocking`), unless the language rules that a loop is written `while true`. The rule's class depends on this answer.

## 5. The blind experiment carries no weight on adoption
- **No room to differ.** Variant A scored 3 of 3 and so did B, with one reading per cell.
- **No null control.** Each brief states the program's intent, so the fix can be worked out with no output at all. Whether either transcript mattered is unmeasured.
- **A was curated.** p3-A's last line is the coordinator's narration, which no command prints.
- **It measures the wrong step.** It measures repair after the mistake is shown; the thesis turns on whether the mistake is caught, which the probes measure.

What it does show: no B reader was misled, and `r2-m` hesitated over the note's wording.

A free partial answer to question 7: I extracted the 194 model-written programs (252 functions) from `docs/panel/*-reports/llm-ergonomist*.md`, panels 147 to 199. Only 2 functions call themselves, both this sitting's repaired `fact`. No verdict moved and none got `endless_recursion`. A null control would cost one session (0.15 to 0.18 USD each, measured). The approval named at most seven sessions and six ran; whether the seventh may be used for this is for the coordinator and the author.

## 6. Defect 507: what the synthesis must decide
- **Where clang's warning is true and false.** True on `forever`, p1, p3, `gen`, `yes`, `mixed` and the three programs of §1. False on `serve` and `hidden2`. (F) repairs `serve` only.
- **The `warnings` suite.** It holds every golden to zero clang warnings at both levels, so `hidden2` as a `run` golden would be red under any route that keeps the warning.
- **(C) and (K) are out**, because they refuse `hidden2`, a correct program.
- **One route closes the class:** `-Wno-infinite-recursion`, with the checker as the only witness. That drops clang's true warning on R3's misses, which is a further reason to build R4 before silencing it.
- **(F) becomes independent**, with its own cost: 17 blessed emissions plus the seed (the engineer's count).

## 7. Defect 508: routes G, L, N
- **G.** At `-O2` it turns the silent hang (exit 124) into an abort (134) on `forever` and p3 (engineer), `pingpong`, `fnvalue` and `gen` (spec-warden), and `yes` (here).
  - Its costs: a correct tail recursion 10,000,000 deep now aborts at `-O2` (it already aborts at `-O0`).
  - On the compiler checking itself (`check selfhost/main.hero`) instructions rise 0.11%, from 18,724.2M to 18,745.5M (seed built by hand at `-O2`, medians of three).
  - The compiler built with the flag builds the compiler (exit 0) and emits byte-identical C, 41,254,610 bytes.
  - Spec cost: 0 tokens.
- **L.** +17 to +18 tokens on the spec's `maximum` row (the spec-warden's drafts D4b and D4a), and one program keeps two meanings.
- **N**, by hand with `musttail`. The 10,000,000-deep recursion prints its result at `-O0`, but `forever` at `-O0` runs silently until `timeout` (124). Every program the rule misses would then hang at every level. A portable form, the emitter turning a self tail call into a jump, is unlisted and unbuilt.

G is the robust choice unless the sitting answers §4 with "a loop".

## What I could not run
- R4's code, cost and census; the census bound in §2 is an inference from carried numbers.
- G and N on clang 18.1.3 and 23.1.1, on Linux and Windows, and the limits of `musttail` there.
- G through `heroes build`'s own flag list, and (F) through `heroes build`.
- The null-control and generation readings, both paid.
- The first-try answers behind measurement 040, which lived in the emptied scratchpad.
- `heroes run` on any endless program, which the bounds rule forbids.

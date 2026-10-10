# Panel 206, spec-warden

Copied by the coordinator at 09:22 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 206, spec-warden

Clock: 05:04:55 to about 05:09, then the session limit stopped me. Resumed at 09:16:59 and finished at 09:22 (`date` both times). I worked in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/206-spec-warden/`. `tree/` is an rsync of `lane-panel-206` (e0aeb991) without `.claude/worktrees`. I removed its `.git` at once and ran no git in it.

- **Compilers**:
  - `heroes` is built from the seed.
  - `heroes-round` is built from `selfhost/` (real 140.80 s, user 90.21 s, on a loaded machine).
  - `heroes-trunk` is a copy of the trunk's binary, built 00:26.
- **Files**:
  - running notes: `notes.txt`
  - probe script: `probe.sh` (check, then build, then the binary under `timeout 10`)
  - every shape on the round: `probes/round-all.txt`
  - the drafts: `drafts/`
  - their prices: `drafts/prices.txt`

- **`verdict`**: **approve, provisional**, for the round's abort landing as a repair, `rep` included, at **0 spec tokens** (draft D0). **Object** to every refusal draft as things stand. No veto.
  - If the sitting takes the refusal, **F7e** is the one sentence I would let land with it.
  - **Refused**:
    - F2b: ambiguous on `s22` and `s23`.
    - F7a and F7c: false on `v02`, because "constants" takes in a group's constant, whose value only clang knows.
    - F2a: silent on division by zero and on the shift count.
  - **Object**: C4a and R11a.

- **`section`**:
  - design.md §1.6: the payment rule, unconditional at every level.
  - §1.2: the cost formula.
  - §1.0: Principle 0.
  - CLAUDE.md § 12: spec beats compiler.
  - design.md does not cover a check that evaluates expressions only to refuse them. Its compile-time evaluation paragraph (Part 6, the one starting "Compile-time evaluation (`comptime`…") and panel 039's C1 row ("fold … an optimisation with no measured need") are about features and optimisation. I say so explicitly.

- **`spec_token_delta`**: vendored tables (claude-legacy / cl100k), **lower bounds**, no `--refresh`.
  - **The frozen spec** reads 7384 / 7518, and 9889 real as recorded and not stale. Headroom is 351. Net of the 60-token FFI floor, 291 are spendable. All three numbers are `heroes-round measure` output.
  - **Base** = frozen + panel 204's F2 and G1m + panel 205's H2f. The texts come from the diffs in the 205 warden's drafts. It reads **7426 / 7561, +42/+43**, which matches the briefs' +33/+34 and +9/+9.
  - **Real headroom after 204 and 205 land is about 234.** This is an inference: about +57 real for their +43 vendored, at the document's ratio of 9889 / 7518.

  Deltas against that base:

  | draft | delta (vendored) |
  |---|---|
  | D0 (no sentence) | 0 |
  | F2a | +13/+13 |
  | F2b | +10/+10 |
  | F7a | +13/+13 |
  | F7b | +16/+16 |
  | F7c | +13/+13 |
  | F7d | +14/+14 |
  | **F7e** | **+14/+14** |
  | F7f | +18/+18 |
  | F7g | +13/+13 |
  | F7h | +14/+14 |
  | C4a | +6/+6 |
  | R11a | +8/+8 |

  F7e would cost about +18 real. That is an inference, not a count.

- **`removal`**:
  - D0 owes none.
  - For F7e: **nothing, and that is a problem.** I found no redundant clause in § 2 or § 7. Merging the sentence into § 7 (F7g, F7h) saves at most 1 token, and F7g drops the shift. F7e can only be paid by P2 or P3.

- **`needed_for_self_hosting`**: **no**. The round compiles `selfhost/main.hero`, and the compiler holds no such refusal.

- **`argument`**: On the round, today's spec already predicts five of the six cases at zero tokens. Each literal takes `u8` and fits (§ 2). Each operator aborts on overflow, left to right (§ 7). So `sum`, `s22`, `s23` and `const` abort and `t02` is refused, all as measured. The trunk's refusals were 564 itself: it also refused `repeat("-", 3 - 1)` and `y: u8 = 2 + 3`. `rep` keeps panel 054's purpose. `-1` and an `i64` count stay refused, and `0 - 1` aborts at the subtraction, the path 054 gave a computed count. A refusal makes four of those predictions false unless F7e (+14/+14, a lower bound) lands. Its benefit comes from the diagnostic, not the sentence, and no Part 11 measurement shows the class occurs.

- **`prediction`**:
  - **P1.** If F7e lands, the landing's `--refresh` reads it at **+16 to +21 real**. Instrument: `heroes measure`. Scored at the landing.
  - **P2.** A per-step refusal route under F7e, judged by the census of tracked `.hero` files (`heroes check`), moves **0 files of `examples/` or `selfhost/`** from accepted to refused. Only `tests/golden/` cases written for 564, 577 and 054 move. Scored at the route's landing. Even one example or compiler file moving weakens my objection.
  - **P3.** This one is an observation that pays nothing: its instrument is a paid blind run, which I did not run. On a task asking for an all-ones `u64` mask or an unsigned sentinel, at least 1 of 4 blind readers of today's spec writes `0 - 1` at an unsigned width. The idiom is common in the corpus: 75 lines in 28 files of `examples/` and 93 lines in 38 files of `selfhost/`, by a grep, which is not a census.

- **`condition`**: I move to **approve the refusal with F7e** if all three of these hold:
  - (i) A route is built that works step by step, at every width `i64` included (F7e is false on `s17` otherwise). It reads only written constants, so `v02` stays an abort. It leaves calls alone, so `m2` and `t10` stay aborts (panel 203 R3).
  - (ii) F7e is run true on that route against p206 and the 58 shapes.
  - (iii) P3 or P2 measures the class in real first tries or real code.

  A refusal that lands **with no sentence** I object to outright. § 7 would then describe a run that the compiler refuses, and CLAUDE.md § 12 makes that the compiler's defect.

## The drafts, whole

Each draft replaces one sentence of the base:
- § 7's drafts replace *Overflow aborts at every width. Integer division by zero aborts.*
- § 2's drafts replace *Every base writes a value, so a literal must fit its type.*

- **D0**: no change.
- **F2a** (§ 2): *Every base writes a value, so a literal must fit its type, and so must each step of arithmetic on literals and constants alone.*
- **F2b** (§ 2): *Every base writes a value, so a literal must fit its type, and so must arithmetic on literals and constants alone.*
- **F7a**: *Overflow aborts at every width, and so does integer division by zero; where literals and constants alone abort, the program is refused.*
- **F7b**: *Overflow aborts at every width, at each operator, and integer division by zero aborts: a compile error where literals and constants alone reach it.*
- **F7c**: *Overflow aborts at every width. Integer division by zero aborts. Either is a compile error where literals and constants alone compute it.*
- **F7d**: *Overflow aborts at every width. Integer division by zero aborts. Either is a compile error where literals and written constants alone compute it.*
- **F7e**: *Overflow aborts at every width. Integer division by zero aborts. An abort that literals and written constants alone compute is a compile error.*
  - "An abort" also covers the shift count, a unary minus, `%`, and the `i64` default.
  - It is true only on a step-by-step route. A route judging the final value would leave `s23` aborting while F7e says it is refused.
  - A final-value semantics would need the emitter to fold expressions, which is panel 039's C1, and would change what § 7 means.
- **F7f**: F7e with *, at each operator* after *every width*.
- **F7g**: *Overflow and integer division by zero abort, at every width; where literals and written constants alone compute one, it is a compile error.*
- **F7h**: *Overflow aborts at every width, and so does integer division by zero; an abort literals and written constants alone compute is a compile error.*
- **C4a** (§ 4): *…and a written body computes over literals and other constants, each time it is read.* This rests on the critic's reading that a constant is emitted as a C function; that is carried, I did not read the C myself. It saves no rewrite.
- **R11a** (§ 11): *`repeat(s, n)` (`n` a `u64`)*. This is a gap older than this sitting: the spec never states the count's type, and panel 120 refused built-in signatures as a class.

## What a reader predicts, against what was run

"Round" and "trunk" are runs: check, then build, then the binary. Exit 134 is *panic: integer overflow*, with no source position, even under `heroes run`.

| case | today's spec (D0) predicts | round | trunk | F7e predicts |
|---|---|---|---|---|
| `sum` | builds, aborts | 134 | `type_mismatch` | refused |
| `rep` | **undetermined**: § 11 gives no type for `n`; read as `i64`, the spec says nothing about a negative count | 134 | `type_mismatch` | refused only if `n` is known to be `u64` (with R11a: refused) |
| `const` | aborts; when is unstated, and § 4's *computes* may read as compile time | 134 | `type_mismatch` | refused |
| `s22` | aborts (`255 + 1` comes first) | 134 | `type_mismatch` | refused |
| `s23` | aborts (`2 - 3` comes first) | 134 | `type_mismatch` | refused |
| `t02` | refused, 300 cannot fit (§ 2) | `int_out_of_range` | `type_mismatch` | refused |

- **D0 is true on the round** for `sum`, `s22`, `s23` and `t02`, and for `const` apart from when it aborts.
- **D0 plus R11a** would also predict `rep` (abort) correctly.
- **F7e is false on the round** for 5 of 6 cases. It would be true on a refusal route, but none is built.

Four more runs bear on `rep`:
- `repeat("-", 3 - 1)` prints `--` on the round and is **refused `type_mismatch` on the trunk**, so the trunk refused a correct program.
- `repeat("-", -1)` is `int_out_of_range` on both.
- `n: i64` passed as the count is `type_mismatch` on both.
- `u64` operands `w - c` abort 134 on both.

**One observation outside the question**: `3 << 63` (`s07`) prints `-9223372036854775808` at exit 0 on the round. A reader of § 7's *Overflow aborts at every width* may predict an abort. The spec does not say whether a shift overflows.

## What I did not run

- **`--refresh`**, and any paid run. Every delta above is a vendored lower bound, and every real figure except the recorded 9889 is an inference.
- **The compiler-engineer's route**: none was built. Its notes stop at 05:06 and its `bin/` holds only `round`. So no draft was run against a refusal, and "true on its route" is unrun.
- **Any refusal prototype**, the census, the blind readings, `-O2`.
- **The trunk on all 58 shapes.** I ran it on the 6 p206 cases (`lit` aside, which only the round ran), 1 critic shape and 5 cases of my own.
- **The `spec` suite** on any draft.
- **Reading the emitted C for a constant**: carried from the critic.

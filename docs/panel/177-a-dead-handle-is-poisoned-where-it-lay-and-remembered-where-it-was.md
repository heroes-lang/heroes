# Panel 177 — a dead handle is poisoned where it lay and remembered where it was

2026-09-24, M-agreed-retention, at HEAD `521c5e02`. **Full panel**, five seats
and a completeness critic. Briefs and reproducers: `docs/panel/177-briefs/`.
Reports: `docs/panel/177-reports/`; the llm-ergonomist's, the historian's, the
ffi-pragmatist's and the critic's were written out by the coordinator from their
final messages, the harness having refused their files, with the one mechanical
change each header names.

**The author's words that bind this sitting**, given in conversation on
2026-09-23 and recorded in English as meant: *proceed, always favouring the most
solid solutions*, and *pay all the tokens, without economising*. Neither lifts
design.md §1.6's payment rule.

**Procedure.** Every seat worked in its own `git archive` of HEAD, built from the
seed; the blind seat read a copy of the spec and its brief outside the tree.
Four seats and the critic stalled on a stream watchdog and were resumed from their
transcripts; the Mac was in clamshell sleep from 10:27:48 to 13:57:59 (`pmset -g
log`), and the four stalls were reported as it woke. **Windows is unrun for every
row of this sitting**: the box was unreachable until 15:54.

**Two errors in the shared brief and one in the critic's, found by seats.** The
shared brief said R *catches the 088 shape*: false, measured by the
compiler-engineer and the ffi-pragmatist independently on three legs — the
reproducer's next `calloc` returns the freed address, so the dead handle is live
again when R asks. That sentence was the coordinator's inference, written unrun.
The label-stripped sentences II and III handed to the llm-ergonomist are false of
the routes they stood for (spec-warden). And the critic's brief called T's
`borrows` clear unpriced; it was already in the ffi seat's emulation, which its
report did not say (critic).

## The two questions

**Q1, defects 077 and 088**: a handle whose life a consuming call ended is still
a value the program holds, and nothing checks a read of it. Both are shapes of
design.md Part 8 wart 20's class, the copy that outlives its release; 077 adds an
address C has handed out again. **Q2, panel 176's item 9**: the reader test and
price of the transfer's releaser, the success clause and the reference.

## The verdict table

| | compiler-engineer | ffi-pragmatist | spec-warden | llm-ergonomist | historian |
|---|---|---|---|---|---|
| **verdict** | approve P and M-must (a route nobody listed) · object M-may, R, S · no veto | approve P and T (a route nobody listed) · **veto R and S as stated** | approve the adopted text at its price, β, D+ · object α, R, P until `==` is ruled | object the common text as written · approve α (preferred), β, II, III · no veto | approve M (with a join rule), P, S, A · object R, G, D |
| **section** | §1.1, §1.7, Part 5, §4.4 | §1.11, §1.12, §4.19 | §1.6, §1.2, §1.0 | the spec alone | precedent |
| **cost** | P +96 −9 selfhost, +12 runtime, ABI 22→23, +0.5 ns a call; M-must +329, +1-2 % check time; β +133 −13 | no C byte changes under P, T, β or `retains`; S changes C-visible types | the adopted text with β: **8675** real (+314) | — | — |
| **prediction** | P + M-must + β: selfhost +500 to +700, check moves exactly two of 510 | R aborts `jsonc_read`; S breaks the verify callback; P leaves `pair_escape` at 0; the success clause runs 0 / 134 / 134 | 8675 ± 8; the reader binds both success values right first try | β no worse than α on polarity; III misses the brief's program on a leg | P aborts all five; one line of copy returns each row to today |
| **condition** | object P if a correct program needs `==` on a dead handle | lift the R veto if R records borrowed handles (that is T) | approve P once `==` is defined | approve the common text once a transfer has a receiver and an out-of-set release aborts | drop the join-rule condition on a shipped check that ignores branches |

## What the sitting measured

**R and S are out.** R refused 6 of 8 correct programs over real json-c, SQLite
and OpenSSL on three legs, because a borrowed handle is correct exactly where the
set does not hold it, and it misses 088 itself (ffi seat, compiler-engineer). S
makes a handle two words: today's callback emission becomes a clang error, a
per-type trampoline calls the wrong callback at exit 0, and a group record's field
is C's struct (ffi seat; the critic re-ran its C). The historian's precedents for
S all own their handle table or unwrap in development only.

**P catches the binding, T catches the copy, and neither alone is enough.** P —
the place a consuming call read its argument from is overwritten with a dead
value, and a handle crossing into C is checked — aborts all five reproducers
before C on three legs, fields and elements included, `pair_escape` too
(compiler-engineer, critic; the ffi seat's contrary row describes its own
emulation). It misses every copy made before the call (compiler-engineer's E13,
`sqlite_copies` 3 of 4). T — the runtime remembers the addresses consuming calls
ended, clearing a mark when C hands the address back — catches those copies,
refuses none of the ffi seat's eight correct programs **with its `borrows` clear**
(without it, real OpenSSL's `ossl_pump` aborts), and costs +3.4 ns a handle
argument and about 18-21 MB per million released addresses never reused (critic,
run; nobody else timed it). **Under `--sanitize`, T closes the copy-and-reuse
case too**, because ASan's quarantine keeps the address from coming back: real
SQLite's `sqlite_copy_reuse` aborts before C under `C --sanitize`, where the base
compiler under `--sanitize` runs 0 (critic).

**P as prototyped is worse than today in one place, and the sitting found it
late.** A callback's RESULT crosses into C unchecked, so a dead value returned to
C is written through: exit 0 under the composite on three legs and under its
`--sanitize` on Darwin, where the base compiler's `--sanitize` reports
`heap-use-after-free` (critic, `cb_return_poison64`). P is as sound as the list of
crossings it checks, and nobody had enumerated it.

**T's callback clear launders.** A mark cleared at the entry of a function C can
call back also clears when Heroes calls it directly, because a function value is
its C address: a stale copy handed to such a function reads freed memory at exit 0
on three legs (critic, `cb_launder`). The clear belongs in a thunk at the address
handed to C — argued, unbuilt.

**M-must is the compile-time half, and only intraprocedurally.** A binding whose
life ended on every path to a read is refused there: `check` 1 on the five
reproducers, no correct program refused over the 510 files and seven flow shapes,
two double-release goldens moved (compiler-engineer, critic). **With its
program-wide `@` summary its verdict is not local** — one line is refused or
accepted depending on a callee's body in another module (critic, `xmod`), panel
176's ground for refusing the call-site rule. Intraprocedural (`Cloc`), it still
refuses four of five at check and moves the helper case to P at run time. M-may
refuses two correct programs whose correctness is a value; M-must lets the
ergonomist's buggy loop and forgotten `return` through to P, which aborts them at
run time. §4.4's sentence about mandatory initialisation stays true; what M-must
falsifies are the checker's own comments and wart 20's text.

**`--sanitize` is not the remedy for a real library.** ASan checks reads only in
instrumented code: the four `sqlite_copies` shapes exit 0 under `--sanitize`, and
`ossl_alias_null` is *SEGV on unknown address* inside `libcrypto` (ffi seat;
reproduced by the coordinator on Darwin and Linux arm64). Wart 20's *"The remedy
today is `--sanitize`, which names the line"* is true of its own reproducer and
false of handles given to real libraries.

**The success clause belongs on the result, and must govern every end.** Four
seats and the critic's runs put it on the result: one condition per function is
where SAL and Clang's `TRY_ACQUIRE` put it, no function in the ffi seat's census
transfers two parameters under different values, and a clause on a `void`
function or two disagreeing parameters are spellings only α can write. It must
cover `consumes` — real SQLite's `sqlite3_close` returns `SQLITE_BUSY` and leaves
the connection open, 0 three of three with the clause (compiler-engineer, critic)
— and `retains`, since `X509_up_ref` returns 0 on failure (spec-warden,
historian), and never an acquisition, since `sqlite3_open` hands a handle back on
error. **No priced spelling says success is a non-null result**, which
`BIO_new_fp`, `curl_slist_append`, `OCSP_request_add0_id` and `realloc` need
(critic). The llm-ergonomist preferred α on polarity, and its prediction P1 is
where that is scored.

**V1c has a hole a first-time reader writes.** `fclose(stream: File transfers
pclose)` on a `popen` stream passes the releaser check and nothing calls `pclose`,
0 three of three on V1c's emulation (ergonomist, critic). The ergonomist's repair
as worded — a call with no other handle parameter cannot transfer — refuses
`BIO_new_fp`, whose receiver is its result, and forces it back to the `consumes`
that runs silent (critic).

## The resolution — `provisional — author ratification pending`

1. **A dead handle is poisoned where it lay (P).** A call that ends a life
   overwrites the place its argument was read from — a binding, an `@` cell's
   field, an element — with a dead value, **when the life ended**: when the set's
   count for that address reached zero, so a second reference released through
   the same name is not refused. The dead value is an address on a page the
   runtime maps with no access, so a write through it faults. **Every crossing of
   a handle into C is checked**: arguments with their fields and elements,
   callback results, and `@ … borrows` cells; a dead value aborts before C. A dead
   value may not be read in Heroes either: `==` on one aborts.
2. **A dead address is remembered where it was (T).** The set keeps the addresses
   whose life ended, and a handle reaching C at one aborts before C. The mark
   clears when C hands the address back: an `acquires`, a `borrows` hand-back, or
   a handle C passes to a callback — **in a thunk at the address handed to C,
   never at the function's entry**. The price, +3.4 ns a handle argument and the
   memory of the dead set, is stated and paid: robustness ranks above speed
   (CLAUDE.md § Precedence).
3. **The compile-time half is M-must, intraprocedural.** A read of a binding
   whose life ended on every path to it is an error at that read, decided inside
   the function. No program-wide summary: its verdict would move with a callee's
   body. The checker's comments that state there is no flow analysis, and wart
   20, are amended in the landing's commit.
4. **What stays a documented limit, narrower than today**: a copy made before the
   call that reaches C after C has handed its address out again, outside
   `--sanitize`; and a handle C keeps inside its own struct across the release.
   § 13 says so, and design.md Part 8 wart 20 is rewritten to say it, with its
   `--sanitize` sentence corrected. **Defects 077 and 088 close with items 1 to
   3**, their reproducers refused at check or before C.
5. **R, S and G are refused**, R and S on the ffi seat's vetoes. **A stays
   refused on cost**; Part 6's row is corrected where it says *nobody has*: SPARK
   ships a move for every type holding a pointer (historian), so what remains open
   is the price here.
6. **The success clause is on the result**, `-> i32 when 0`, and governs every
   end, transfer and reference the call makes, never an acquisition. **A handle
   result that comes back null means the call failed**, without a clause, when
   the call transfers into that result. α is refused. A success range (`> 0`) is
   a question, not a form.
7. **A transfer needs a receiver**: another handle parameter of the call, or the
   handle the call acquires as its result. A call with neither cannot transfer,
   so `fclose(stream: File transfers pclose)` is a compile error, and `BIO_new_fp`
   stays a transfer. The releaser stays checked against the set (V1c). The two
   deprecated one-parameter OpenSSL adders bind as `consumes`.
8. **`retains`** on a result or a parameter keeps the live set and takes the
   result's `when` on a status result (`X509_up_ref … -> i32 when 1`).
9. **§ 13 states the two sentences the reader found missing**: a release by a
   function outside the handle's releasers aborts before C, and a handle given
   back more often than it was taken aborts (panel 176 item 1, and the
   ergonomist's H11 and H16).
10. **The landing measures before it lands**, and each of these is a condition,
    not a hope: the thunk against `cb_launder` and `cb_reuse`; the callback-result
    check against `cb_return_poison64`; `retains` through one name under P and
    M-must against the ergonomist's A3; the null-result rule against
    `slist_fail`; and the whole text on the real instrument, paid by predictions
    it registers.

**What the vetoes compel**: R and S out. **What conservative would have been**
(CL-040): P alone, +96 lines, the five reproducers refused. **Refused**: it misses
every copy (`sqlite_copies` 3 of 4) and, with callback results unchecked, turns a
use-after-free ASan catches today into a write it cannot see.

## Found in what ships

**Defect 090**, the ffi-pragmatist's: a use-after-free inside C reached through a
stale copy is reported as *a handle or `ptr` holding `nullptr` reached C* on
Darwin, after the program printed that the handle was not null, and dies at 139
with nothing on both Linux legs — reproduced by the coordinator, three of three,
filed with this sitting. Two library facts, recorded and not ours: json-c 0.19's
`json_object_put` returns the opposite of what its header says, and
`CMS_add0_cert` frees an equal certificate it reports as added. And one stale
number in the process: `.claude/skills/panel/SKILL.md` says a rebuild from
`selfhost/` is about twenty minutes, and the compiler-engineer measured 67-72 s
from a seed built at `-O2`; the skills are amended by the author, so it is queued
with this sitting.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | P + M-must + β: `selfhost/` +500 to +700, `runtime/` +20 to +40, `check` moves exactly two of 510, `corpus` 55 / 0 — **the runtime half is void as stated**, since T is adopted beside it; the rest stands | the landing |
| compiler-engineer | if R lands, `ffi-borrows-owes-nothing` goes red — **void**, R refused | — |
| ffi-pragmatist | R aborts `jsonc_read`; S breaks `ossl_verify_cb` — **void**, both refused | — |
| ffi-pragmatist | P leaves `pair_escape` at 0 — **FALSIFIED in the sitting**: the compiler-engineer's P aborts it, 134 on three legs (critic) | scored here |
| ffi-pragmatist | the success clause: `jsonc_success` 0 at n=1, 134 at n=2, 134 at n=0 with the value wrong — **held on the critic's composite**, to re-score on the landed build | the landing |
| spec-warden | P1 8675 ± 8 — **void as stated**, the landed text adds items 1-3, 6, 7 and 9; P2 the reader binds `json_object_object_add` with 0 and `X509_up_ref` with 1 first try; P3 panel 176's critic's four programs run 0 / 134 / 0 / 0 | the landing's reader test, then M-thesis-harness; the landing |
| spec-warden | P4: under I the reader expects 088 stopped, or D+'s +20 is unpaid — **scored in the sitting by the critic: unpaid**, the reader expected it to reach C | scored here |
| llm-ergonomist | P1 β no worse than α on polarity, 20 fresh readers; P3 at least 15 of 20 first attempts free one name twice under II; P4 at least 1 of 20 edits the binding in an A4 task, 0 of 20 with a receiver | M-thesis-harness |
| llm-ergonomist | P2 III misses 088 on at least one leg — **HELD** on all three POSIX legs (compiler-engineer, ffi seat: R does not abort it) | scored here |
| historian | P aborts all five reproducers before C on every leg — **HELD** on the three POSIX legs, Windows unrun; one line of copy returns each row to today under P — **HELD** for P alone (compiler-engineer's E13) | scored here |
| panel 176's llm-ergonomist | task 1 first-try ≤60 % under X and ≥90 % under V1 — one reader cannot give a rate; **carried to M-thesis-harness**, as panel 176 named | M-thesis-harness |

## Author's verdict

*Pending: `docs/work/DECIDE.md` carries this sitting as `panel 177`. Work
proceeds on the provisional resolution: route E's lane merges, defect 090 is
filed, then the landing of panels 176 and 177 in steps.*

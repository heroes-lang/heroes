# Panel 154 — report of the llm-ergonomist

**verdict** — A: **object** (dominated by B, not unsound) · B: **adopt** · C: **object** (reintroduces Task 1 by omission) · D: **object** (silent hole on the commonest C handle; declined veto, see §Locality) · E: **refuse** (inverted: forbids the correct null, permits the dangerous one) · F (named sixth route, result-side mark): **adopt as queued follow-up, not now**

No veto. All five routes are local under the rule as I hold it; my objection to D is overloading, which is a weaker complaint than non-locality, and I say so rather than borrow the veto's force for it.

---

## experiment

### Task 1 — the cost of silence

What I, reading only that binding, expected:

1. `Node` is spelled as a thing, not as a maybe-thing. The language has a distinct spelling for absence (`nullptr`), so `-> Node` reads to me as **"a node"**. I expected `node_find` to either return a node or not return.
2. `borrows` told me about *ownership* and I read it as also telling me about *presence* — the library keeps one, so there is one. That inference is wrong, and nothing on the page corrects it. This is the whole defect: the only mark on the result line talks about lifetime, which primes a reader to believe lifetime is the only question a result has.
3. I expected the program to either abort with a message naming `p` of `node_value`, or fail to compile.

What I did **not** expect, and what the description says happens: exit 0 with a fabricated integer. That is a plausible mistake that compiles — the enemy case.

Where I would have looked for the warning, in order:

- **line 3**, `function node_find(key: i64) -> Node borrows` — my first look, because the *defect* is here: the binding asserts more than the header supports. No route A–E puts a diagnostic here.
- **line 7**, `print(node_value(p: found))` — my second look, and the only place any of A–E speaks. This is the right place to *stop*, because it is where undefined behaviour begins, but the message will be pointing two lines downstream of the lie.

Note the two failures are separable and only one is on the table: (i) *this result may be absent* (binding-side, unsayable today), (ii) *this parameter must be present* (call-side, what A–E address). A–E convert (ii) from silent to loud. None of them makes (i) sayable, so **none of them makes Task 1 a compile error.** The honest claim for the winner is: *silently wrong answer becomes loud abort at the call.*

### Task 2 — the five rules, written out

The reader's real job: `getaddrinfo`/`freeaddrinfo` (null accepted), `fopen`/`fclose` (null **not** accepted — `fclose(NULL)` is undefined), `pthread_create` (null `attr` **is** the documented way to ask for defaults), `node_value` (null not accepted). I wrote the group four ways; `fclose` and `pthread_create` are the two shapes that separate the routes.

**Route A — check everything, no word.**

```
extern "netdb.h"
    record AI tag addrinfo
    function getaddrinfo(host: Str, service: Str) -> AI acquires freeaddrinfo
    function freeaddrinfo(ai: AI consumes)

extern "node.h"
    record Node tag node
    function node_value(p: Node) -> i64
```
New words: **0**. Goes wrong at the *call*, not the binding: `freeaddrinfo(ai: ai)` on a cleanup path after a failed `getaddrinfo` aborts, where C was fine. The program must be restructured with a hand guard. Loud, first run, right line. **Local: yes.**

**Route B — check everything, opt out on the rare case.**

```
extern "netdb.h"
    record AI tag addrinfo
    function getaddrinfo(host: Str, service: Str) -> AI acquires freeaddrinfo
    function freeaddrinfo(ai: AI consumes accepts_null)

extern "stdio.h"
    record File tag _IO_FILE
    function fclose(f: File consumes)                  # correct: no accepts_null

extern "pthread.h"
    record PthreadAttr tag pthread_attr_t
    function pthread_create(attr: PthreadAttr borrows accepts_null) -> i64

extern "node.h"
    function node_value(p: Node) -> i64                 # correct: checked
```
New words: **1**, and it lands on the minority of parameters, so most bindings and most spec examples are byte-identical to today. Goes wrong at `function freeaddrinfo(ai: AI consumes)` by **omission** → abort at the cleanup call → fix is one word at the line the message names. The only silent residue is an **affirmative** false claim, `function fclose(f: File consumes accepts_null)`, which requires the reader to assert something about the header rather than forget something. **Local: yes** — the call's check is decided by the declaration the call names.

**Route C — check nothing, opt in on the common case.**

```
extern "netdb.h"
    function freeaddrinfo(ai: AI consumes)              # unchanged
extern "stdio.h"
    function fclose(f: File consumes needs_value)
extern "node.h"
    function node_value(p: Node needs_value) -> i64
```
New words: **1**, on the majority of parameters — every handle parameter of every binding in the language now carries it, which inflates every program and every example and multiplies the places to omit it. Goes wrong at `function node_value(p: Node) -> i64` by omission, and the consequence is **Task 1 unchanged, verbatim, exit 0**. **Local: yes**, and useless: locality does not help a mark whose absence is invisible.

**Route D — no new word, `consumes` implies null-tolerant.**

```
extern "netdb.h"
    function freeaddrinfo(ai: AI consumes)              # unchecked — correct by luck
extern "stdio.h"
    function fclose(f: File consumes)                   # unchecked — WRONG, silent UB
extern "pthread.h"
    function pthread_create(attr: PthreadAttr borrows) -> i64   # checked — WRONG, false abort
extern "node.h"
    function node_value(p: Node) -> i64                 # checked — correct
```
New words: **0**, but **one new meaning on a word the reader already knows**, which is worse than a new word: an unfamiliar token makes a reader stop and look it up; a familiar token does not. Wrong in **both** directions on the two lines above — `fclose` silently, `pthread_create` loudly — because "ends this value's life" and "tolerates absence" are empirically independent properties of C functions. **Local: yes** (see §Locality).

**Route E — compile-time, literal `nullptr` only.**

```
    freeaddrinfo(ai: nullptr)      # REFUSED — this is the one call that was correct
    node_value(p: found)           # ACCEPTED — this is Task 1, still exit 0
```
New words: **0**. Goes wrong on both lines, in opposite directions. Worst property: the refusal is trivially defeated by `x = nullptr; freeaddrinfo(ai: x)`, so it *teaches laundering* — a model that learns to route a null through a variable to silence the compiler has learned the exact habit that makes every later check unreachable. **Local: yes.** Closes nothing.

### Task 3 — ranking

**B > A > D > E > C.**

- **C: 8 in 10 models** will omit `needs_value` on at least one parameter of a six-function binding, and the omission is a **silent wrong answer**. Nothing on the page prompts the word; the header does not contain it.
- **D: 9 in 10 models** will not know the rule is in play at all (no unfamiliar token appears), and **5 in 10** will ship a binding whose null admissibility is wrong — roughly half silent (`fclose`-shaped consumers: `fclose`, `closedir`, `sqlite3_finalize`-adjacent), half a false abort (`borrows` parameters documented as taking NULL for defaults).
- **E: 10 in 10 models** will pass Task 1's program unchanged; **3 in 10** will meet the false refusal on `freeaddrinfo(ai: nullptr)` and **1 in 10** of those will launder it through a variable.
- **A: 0 in 10** produce a silent wrong answer; **3 in 10** produce a program that aborts on a cleanup path C would have accepted, found on the first run.
- **B: 0 in 10** produce a silent wrong answer by omission; **3 in 10** omit `accepts_null` and get A's abort, then fix it by adding one word at the named line; **1 in 10** assert `accepts_null` falsely, which needs an affirmative misreading.

I adopt **B**. It is A plus an escape hatch, so it dominates A at a cost of one word; the safe behaviour is the default, the unsafe behaviour requires an act, and both the omission and the act are visible on a single line. D beats E only because D closes the motivating case at all; that ordering is the least stable thing in this report.

**F, the sixth route, named and queued:** the Task 1 defect is on the **result**. `function node_find(key: i64) -> Node borrows maybe_null` plus a required discrimination before use is the only thing that turns Task 1 into a compile error. It is a second new word and a change to how a handle-typed local reads, which is a different proposal; B is the correct thing to land first and F is what B leaves owed. Do not let B's adoption be recorded as closing the null-handle class.

### Locality

I decline the veto on all five. The rule I hold is: *decidable from the line plus the declarations that line names, no other file open.* Under D, `function fclose(f: File consumes)` decides its own checking from its own line, and the call `fclose(f: h)` decides from the declaration it names. That is local. My complaint is that one word carries two independent properties, which is an ergonomic defect, not a soundness one, and I will not dress it as a veto. Under F the call site reads a local binding two lines up, which is ordinary type locality — also not a veto, but it is the route where I would want to re-test locality once its syntax is concrete.

---

## hesitation_points

1. **Syntax of a multi-parameter extern and mark order** (`AI consumes accepts_null` vs `accepts_null consumes`; `-> AI acquires freeaddrinfo` placement). I guessed. Wrong guess → **compile error**. Good. But B must fix an order and show it, or 10 in 10 will guess and half will guess differently.
2. **Whether `freeaddrinfo` accepts NULL.** The brief told me; a model reads a man page. This is the one irreducible guess in every route. Under **B** a wrong guess in the cautious direction (omit the word) → **loud abort**; under **C** and **D** a wrong guess in the dangerous direction → **silent UB**. This single asymmetry is the verdict.
3. **Whether `borrows` implies presence.** I guessed yes in Task 1 and was wrong. Today → **silently different program**. Under B → loud abort. Under F → compile error.
4. **Whether the check fires on a handle that came from `nullptr` via a variable.** I assumed dataflow-independent, i.e. every handle argument at every call. If B is implemented as E-plus-marks (literals only) it buys nothing; the spec text must say *every call, whatever the argument's provenance*, or readers will build E's laundering habit by accident.
5. **What the abort says.** I assumed it names the parameter, the extern function, the header, and the word that would permit null. If it does not, B's 3-in-10 false-abort cohort spends a guessing round trip and some of them will "fix" it by deleting the call. This is a condition of my adopt, not a nicety.
6. **Whether `accepts_null` is legal on a result.** I assumed not, and route F is why the question arises. Wrong guess → compile error if the spec forbids it; **silent no-op if the spec ignores unknown positions.** Say which.

## argument

Task 1 is the thesis failing: a plausible omission compiles and prints a wrong answer. Every route but E closes it at run time; none closes it at compile time, so B's honest claim is *silent-wrong becomes loud-abort*, and F is what remains owed. B is A plus an escape hatch, so it dominates A: safe default, one-word opt-out on the rarer case, omission fails loudly at the line naming the fix. C inverts this — the mark is needed on most parameters, and forgetting it reproduces Task 1 verbatim. D spends no word but overloads `consumes`; a new meaning on a known word does not announce itself, so `fclose` goes unchecked and `pthread_create`'s null `attr` falsely aborts. E refuses the one correct null, permits the dangerous one, and teaches laundering.

## prediction

On a fixed set of five null-handle programs — Task 1's verbatim; `getaddrinfo` failing then `freeaddrinfo` on the cleanup path; `fopen` failing then `fclose`; `pthread_create` with `attr: nullptr`; `freeaddrinfo(ai: nullptr)` written literally — generated first-try from the spec alone:

- Under **B**: silent-wrong-answer rate **0/5** (today: at least 1/5, Task 1, and it is 1/5 under C and 2/5 under D from the `fclose` and `node_value` shapes). False-abort rate **≤ 1/5**, and every false abort is repaired by inserting exactly **one token** at the line the message names — measurable as the diff size of the fix.
- Under **D** specifically: the `fclose` program exits 0 with a wrong answer or a crash on at least one optimisation level. If it does not, my object to D weakens.

Command that scores it: add those five as annotated cases plus goldens, then `./heroes run tests/harness/main.hero -- ./heroes` naming that suite, and score each program on *exit code and whether a diagnostic names the offending parameter*. The annotation is what makes this falsifiable — a snapshot can be regenerated, an annotation cannot be invented.

## condition

I move **B → object** and **A → adopt** if the harness shows that a model, given B's spec text, writes `accepts_null` where the header does not support it in **more than 1 in 10** bindings — that is, if the affirmative-false-claim rate exceeds the omission rate, the escape hatch costs more than it buys and the no-word version is safer.

I move **D → veto** if the `consumes`-implies-null rule turns out to require consulting anything beyond the parameter's own line (for instance, if `consumes` on a *result* or an inherited group-level default participates). As stated in the brief it does not, and I hold the veto.

I move **E → adopt-as-supplement** only if it is added on top of B as a compile-time refusal of `nullptr` at a checked parameter — refusing the literal *before* run time is free once B exists, and the laundering hazard disappears when the run-time check is there to catch the laundered value.

I withdraw the "none of these closes Task 1" framing if someone shows me spec text under which the **result** line `-> Node borrows` already carries a presence claim. I looked for one in what I was given and found none; that is a statement about what I was handed, not about the language.
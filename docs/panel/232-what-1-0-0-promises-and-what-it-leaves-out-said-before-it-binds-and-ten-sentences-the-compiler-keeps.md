# Panel 232 — what `1.0.0` promises and what it leaves out, said before it binds, and ten sentences the compiler keeps

2026-10-29, at M-compatibility-promise (row 74), in the lane `compat-promise`. **Full panel**: the
five seats and a completeness critic, which read the briefs before any seat started (34 findings,
`232-reports/critic-briefs.md`, every one repaired before the first seat) and the reports after
(21 findings, `232-reports/critic-reports.md`). **Status: `provisional — author ratification
pending`**, queued as decision https://github.com/heroes-lang/heroes/issues/2262.

Convened on decision 2217's *A* (the order of evaluation), task 2245 (three sentences of the
specification the compiler does not keep: `<<`, `==` on a handle given back, a stack exhausted
without recursion), task 2250 (§ 11's `exit`, whose step 1 landed before the sitting), task 1066
(the paragraph `1.0.0` rests on), and two questions the milestone owes: whether `1.0.0` promises a
diagnostic's code, and the compatibility suite's corpus. By the author's answers of that day: the
full panel; the blind seat within 12 USD (**1.5590 USD** spent, 80 sessions); and the frame no seat
argued past, written into CLAUDE.md § 2 the same afternoon: **before `1.0.0` no form enters to keep
an older program compiling**, and the promise starts at `v1.0.0`. The briefs are `232-briefs/`
(committed before any seat, fe4f42de9; the blind seat registered before its first session,
5b90ee119), the reports `232-reports/`, the evidence `232-reports/evidence.tar.xz`
(`evidence.md`).

The seats found seven defects running the shapes beside the sentences, each reproduced by the
coordinator with the lane's compiler (`heroes 0.5.0`, sources `3e3c95dc246b73b3`) and filed the same
evening: **2253** (a frame larger than the stack guard's 1 MiB window dies at 139, nothing on
stderr), **2254** (`must(x)`, `default(x, v)` and `is_err(x)` pass `check` and are refused at build),
**2255** (a C object of another type at a given-back address stopped as a stale copy), **2256** (a
program with no C of its own frees a string it holds by binding `heroes_runtime.h`), **2257** (libc's
`signal` bound from Heroes runs Heroes code after an abort and ends at exit 0), **2258** (`==` on a
dead handle aborts naming no place, `adjacent`) and **2259** (a runtime of another release told
`internal error` at exit 2 by every build and `ok` by `doctor`). Six are `blocking`, so this
milestone is not tagged before they close.

## The proposal, as the seats received it

`232-briefs/proposal.md`, verbatim from its frame on:

> **A frame no seat argues past** (the author's rule of 2026-10-29, CLAUDE.md § 2): before `1.0.0`
> no form enters to keep an older program compiling. Each item is a sentence that says what the
> language already does, a sentence the compiler does not keep and one of the two repaired, or a
> promise about `1.0.0` itself.
>
> 1. **The order of evaluation** (decision 2217's *A*). One true sentence, or as few as stay true,
>    for what the compiler does: an expression's parts left to right as written, a write's two
>    phases, the built-ins that call a function value; and whether § 9 owes a direct call's labels
>    the declared order.
> 2. **`<<` and `>>`** (task 2245, 1): § 7 states what design.md § 4.14 rules, the wrap and the sign
>    copied; or an abort on a lost set bit, which amends § 4.14 and answers its reason.
> 3. **`==` on a copy of a handle given back** (task 2245, 2): § 13's clause and the runtime made to
>    agree, one way or the other.
> 4. **A stack exhausted without recursion** (task 2245, 3): *a call chain deeper than the stack
>    aborts*, tested on the shapes beside it first.
> 5. **§ 11's `exit`** (task 2250): whether the sentence says a literal is refused, and what of its
>    edges.
> 6. **The paragraph `1.0.0` rests on** (task 1066): its words, its home, what it excludes, whether
>    it promises one way, and the deployment model.
> 7. **Whether `1.0.0` promises a diagnostic's code**, and not its words.
> 8. **The compatibility suite's corpus** from `v1.0.0`.

And the paragraph, the coordinator's draft as `232-briefs/paragraph-draft.md` holds it after the
critic's first pass:

> **What 1.0.0 promises.** A program that compiles under Heroes 1.0.0 compiles under every later
> 1.x compiler, and does what the specification says it does. Three things are outside it: a
> program that depends on a defect of the compiler, which a 1.x release may repair; what the
> specification leaves open, such as the order of a map's keys; and an error in the specification
> itself, which a 1.x release may correct. The promise covers the language the specification
> describes, the functions it names, and those of `hero_os.h`.
>
> **It runs one way.** A program cannot say which version of the language it was written for, so a
> newer compiler keeps an older program, and an older compiler is never asked to read a newer one.
>
> **A Heroes program stops rather than guesses.** There are no exceptions: an error a program does
> not handle ends it at once, saying why and where, and no Heroes code can catch that stop. A
> program meant to run for a long time runs under something that starts it again.
>
> **2.0.0 may break this promise**, and says which programs it breaks.

## What was measured

Every number below is a command's, run by the seat that reports it; where the coordinator re-ran
one, it says so.

- **compiler-engineer** (`232-reports/compiler-engineer.md`): the lowering's order read call by
  call in `selfhost/ir/flatten.hero` and run on the seat's own programs (`ce/`) beside the
  critic's, every one of the critic's printed lines reproduced; the write's check
  placed by four programs (`a_value_grows` runs, `b_value_shrinks` needs the late check); the abort
  route for `<<` **built** over three generations (stage 2 and 3 emit byte-identical C) and run on
  twelve shapes, the compiler's tests (1,768, 1 failed) and the net's (474, 14 failed, against 474
  passed unchanged), its instructions retired counted at `-O0` and `-O2`; the stack's shapes
  generated and run, and a 12-line repair of `runtime/parts/stack.c` **built** and run on them and
  on the five recursion fixtures; `exit`'s edges rerun; the codes a diagnostic can carry read out
  of `selfhost/` (285 against measurement 060's 242); five corpora at `v0.5.0` checked by today's
  compiler.
- **ffi-pragmatist** (`232-reports/ffi-pragmatist.md`): the handle shapes at the frozen head,
  plain, `-O2` and `--sanitize`; route (a) for item 3 **built** in a runtime copy and costed
  (instructions per `==`, the `run` form under it); `hero_os.h`'s 57 functions and
  `heroes_runtime.h`'s 165 counted from clang's AST and their binders found; five runtime copies
  stepping the ABI stamp or one declaration; nine versions of a third-party header under one
  compiler; the stop caught by a handler bound from Heroes, and the one-line reset **built**; a
  result's ownership changed under an unchanged declaration; a name a later runtime adds against a
  program's own header.
- **spec-warden** (`232-reports/spec-warden.md`): predictions for the blind seat registered before
  its first session; 43 drafts priced on the instrument, each applied to a pristine copy and
  measured offline and with `--refresh` (44 calls of the token-counting endpoint), every
  combination equal to the sum of its parts; the recommended set run through the `spec` and
  `unseen` suites; the 16 rows of its table of evaluation-order, shift and `exit` shapes run on its
  own compiler.
- **historian** (`232-reports/historian.md`): 11 web searches and about 60 page reads, each
  quotation marked `verified`, the searches that found nothing listed with what they looked for.
- **the blind seat** (`232-reports/llm-ergonomist.md`, the coordinator's reading of its sessions):
  80 sessions of 4 tasks × 2 specifications × 2 models × 5, each a `claude -p` with the `Read` tool
  alone in its own folder, scored by `score.py` before the key was opened; 0 of 80 read outside
  their folder.
- **the critic** (`232-reports/critic-briefs.md`, `232-reports/critic-reports.md`): the first pass checked every number, path and negative sentence
of the briefs; the second ran its probes in its own worktree (`critic-scratch/second/`): the
`.default` shapes, the thesis rules counted at every tag, the inventory's names at two tags and
nineteen foreign words, the handle read after its end, one frame at `-O0` and `-O2`, `check`
against `build` over the 371 `unsupported` cases and the corpus's 21 programs that bind C, the
sealed key joined with every answer, a stop inside a bound `hero_os.h` function, one 4 MiB frame's
assembly for four targets, the codes a program can receive, the command's rows at each tag; and five
of the historian's quotations re-read on the raw pages, every one holding.

## The verdict table

| seat | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| **compiler-engineer** | **no veto**: 1 approve a two-phase sentence, object to one line; 2 object to the abort route, approve § 4.14's rule as the sentence; 4 approve *a call chain deeper than the stack aborts* and file the crash it does not keep; 5 approve a corrected § 11; 7 object to promising codes on the 242, approve the `"schema"` route once a list exists; 8 approve `examples/` plus the thesis references, object to `run/` and `selfhost/` | design.md §1.1, §1.7, §1.12, §4.14, §4.17 | item 1, 0 lines; item 2's abort route 37 lines, 174 bytes of C per `<<`, about 2 instructions per `<<` at `-O2` unmasked and 20 at `-O0`, 14 of the net's 474 tests red; item 4, 12 lines of `stack.c` on Darwin (run), Linux unpriced; item 7, a list of 285 codes, unbuilt; item 8, a second corpus, unbuilt | `bigframe18b` reads 139 with an empty stderr at every commit until `stack.c` changes, 134 `stack exhausted in bigframe18b.go` with the witness; the Linux legs stay red with the witness alone | item 2: readers measured predicting an abort for `5 << 62` and no wrap-reliant `<<` in written programs; item 1: a write whose value lends the write's root refused; item 7: the list exists and the net holds it |
| **ffi-pragmatist** | **no veto**: item 3 approve route (b), the clause narrowed, object to route (a); item 6 **object** to the draft's C-boundary sentence and to *no Heroes code can catch that stop* | design.md §1.11, §1.12, §4.10, §4.19, §4.20, Part 7 item 2 (decision 2228), Part 6's *Exceptions*; spec § 13 against Part 8 wart 20 | (b) +4 vendored, no C, no golden; (a) +121 instructions and a global mutex per handle `==` once any handle is given back, the `run` form 599 passed and 1 failed; the C half of the paragraph 122 vendored; refusing a program's `heroes_runtime.h` refuses two `check` goldens and one frozen recovery copy, none of `examples/`; reserving the runtime's name forms reaches 24 golden headers, none of `examples/` | (b) moves no emission and no golden; (a) moves the one pinned program and aborts `s9b` on every leg (unrun off this Mac); `c4` exits 0 on the CI's Linux legs and 134 once `hero_abort` resets `SIGABRT` (unrun off this Mac) | (a) acceptable once 2255 is repaired and its price paid without a global lock; item 6 approved when the paragraph promises `hero_os.h` as names, types and ownership with the runtime's name forms reserved, puts `heroes_runtime.h` outside, excludes a C library the program binds, and makes the stop final or narrows the sentence |
| **spec-warden** | **object, no veto** on items 1 to 5: E1 + M10a + B4r + L1, S3, H1, K1, X4; **veto** on item 6 in the specification, approve the README | design.md §1.6, §1.2; CLAUDE.md § 2; `.claude/rules/spec-shape.md` | the recommended set 10,343 → **10,426 real (+83)**, 7,890 → 7,956 vendored; headroom 5,898 with the FFI floor; the consolidation block 378 of 1,500; the paragraph in the specification +335 real, as one sentence +31 | the set lands at 10,426 real if nothing else lands first; the blind effects O ≤ 1, S ≥ 5, X ≥ 7, W ≥ 3 of 10 | 2253 filed before item 4's sentence lands (done); W's effect measured before item 1's write clause is called paid |
| **historian** (advisory) | approve | — | 11 searches, about 60 pages, every precedent cited `verified` | — | a language that promised one way for a whole major version and never added a declaration; one that kept a promise on its operating-system functions; a trap on a lost set bit that still gives the sign bit a value; C# reconciling its standard and Roslyn; a language promising its diagnostics' words |
| **llm-ergonomist** (the blind seat; **the coordinator's reading**) | the tables of its report: X the one sentence that moves what a reader predicts | — | 1.5590 USD, 80 sessions | none registered beyond the spec-warden's | — |

## Predictions to score

| seat | prediction | scored |
|---|---|---|
| spec-warden | blind O: P 10 of 10 | **hit**, 10 |
| spec-warden | blind O: T 9 of 10 (a miss below 8) | **missed**, 6 (8 on the post hoc reading) |
| spec-warden | blind S: P 9 (a miss below 7) | **missed**, 5 (10 post hoc) |
| spec-warden | blind S: T 1 (a miss above 3) | **hit**, 3 (9 post hoc) |
| spec-warden | blind X: P 9 (a miss below 8) | **missed**, 5 (10 post hoc) |
| spec-warden | blind X: T 0 (a miss above 1) | **hit**, 0 |
| spec-warden | blind W: P 9 (a miss below 8) | **hit**, 8 (10 post hoc) |
| spec-warden | blind W: T 4 (a miss above 6) | **hit**, 5 (10 post hoc) |
| spec-warden | the effects, P minus T: O ≤ 1, S ≥ 5, X ≥ 7, W ≥ 3 | O **missed** (+4), S **missed** (+2), X **missed** (+5), W **hit** (+3); post hoc O +2, S +1, X +10, W 0 |
| spec-warden | the recommended set lands at 10,426 real if nothing else lands first | **lapsed**: the set was replaced after the critic's pass (R1 to R5) |
| coordinator, from the spec-warden's parts | the synthesis's set, P1 left for the gate, lands at 10,497 real (the sum of its measured parts, which the seat found exact) if nothing else lands first | the landing |
| spec-warden | a blind re-run of W with E1 + M10a within 1 of E0's P count; a task printing inside `xs.any(f)` at most 6 of 10 under T and at least 9 under B4r (about 4.20 USD, 30 sessions) | the milestone's close, the run inside the author's 12 USD |
| spec-warden | `k2` exits 139 with nothing on stderr on Darwin arm64 until the window is repaired | 2253's repair gate |
| compiler-engineer | a `run/` case of `bigframe18b` reads 139 and an empty stderr until `stack.c` changes, then 134 `stack exhausted in bigframe18b.go`; the CI's Linux legs stay red with the Darwin witness alone | 2253's repair gate and the push after it |
| ffi-pragmatist | route (b) changes no emission and no golden | the landing |
| ffi-pragmatist | `c4` exits 0 on the CI's two Linux legs, and 134 once `hero_abort` resets `SIGABRT` | 2257's repair and the push after it |

## Disagreements, stated plainly

The critic's findings, numbered R1 to R21 in its report, are cited here as **C1 to C21**, the R
being this file's resolutions.

1. **Item 1, one line or several.** The spec-warden recommends E1, *An expression's parts are
   evaluated left to right as written*, as true of every shape it ran; the compiler-engineer
   objects to one line, `.default(v)` evaluating `v` only when the receiver is an error. C1
   checked both: `m["a"].default(xs[len(args()) + 4])` prints `1` and exits 0, the read never
   made, and design.md Part 5's sugar table rules `.default()` a branch; read with § 9's *`x.f(y)`
   is sugar for `f(x, y)`*, E1 predicts the abort the program never meets. **For the
   compiler-engineer**: E1 lands only with D1 beside it.
2. **Item 1, the write's index.** The spec-warden's M10a says an index in a place written with `@`
   waits for the value; C15 (a) ran `ys[zs[noisy("inner", 5)]] @ noisy("value", 1)`, which aborts
   at the read `zs[5]` before the value, as every read does. **M10c replaces M10a.**
3. **Item 1, the labels.** L1 (−3) widens a clause inside § 9's bullet on two parameters of one
   type, while the rule binds every call (C15 (b): `add2(b: "xy", a: 1)`, no such pair, refused
   with four diagnostics, now defect 2261). **L2, *Labels keep the declared order*, replaces L1.**
4. **Item 4, K0 or K1.** The compiler-engineer approves *a call chain deeper than the stack aborts*,
   the spec-warden *running out of stack aborts*. One call whose frame passes the stack is no chain,
   and a spawned thread's stack is its own (C17): **K1**. Both wordings are open on Linux until
   defect 2253's repair holds there (C10).
5. **Item 3, route (a) or (b).** The ffi-pragmatist approves (b) and objects to (a); the
   spec-warden finds either free. (a) aborts `s9b`, two live handles compared, through defect 2255's
   cause, and costs 121 instructions and a global mutex per handle `==` to guard a comparison that
   reads no memory: **(b)**, with C4's compile-time half added.
6. **Item 6, its home.** The spec-warden vetoes the paragraph in the specification on Principle 0
   (+335 real, no measured effect on a program written from it); no seat argued for the
   specification. **The README, at M-publication-gate**, which the milestone's page already names
   as where the paragraph is published.
7. **Item 6, *It runs one way*.** A fact the document's silence already rules (the spec-warden) or
   a choice every precedent read later reversed (the historian, Go 1.21, Rust's editions, Nim's
   `--useVersion`); C16 checked the first half (the specification holds no word for a version) and
   asked the sitting to rule the second. **Ruled a rule of `1.x`** (R7).
8. **The blind seat's two readings.** C7 joined the sealed key with every answer: all 20 of task
   W's answers predicted the program's lines, and the registered +3 is three Sonnet answers that
   stopped writing a caveat (Sonnet wrote prose after correct lines in 19 of 40 answers, Haiku in
   6). **Ruled**: a reader's prediction is the answer's lines, the post hoc reading; a caveat
   dropped is a model's habit and pays no sentence. Item 1's clauses are paid by the promise
   (decision 2217's *A*: a program that depends on an unwritten order is promised nothing), item 2's
   by correcting a sentence that is false of `<<` today, item 5's by the measured effect, the one
   both readings agree on (X: 0 of 10 under T, 10 of 10 under P).
9. **Item 7, how strong.** The compiler-engineer's route promises a code through `"schema"`, so a
   `1.x` may rename one whenever the schema moves; the historian's pages promise more (Go's *values
   should not be changed*, Swift's *stable identifier*). C21 named the gap. **For the stronger**
   (R9).

## The resolution adopted, provisionally

**Robust, and neither the cheapest route nor a compromise between the seats.** Twelve parts. **One
veto stood, the spec-warden's on the paragraph in the specification, and it is honoured**: R6 keeps
the paragraph out of the specification. No seat vetoed anything else. Every
sentence of the specification below was priced on the instrument by the spec-warden after the
critic's pass (`232-reports/spec-warden.md` § The synthesis's wordings, priced): the eleven as one
document, **10,343 → 10,556 real (+213)**, 7,890 → 8,054 vendored, the headroom 5,768 with the FFI
floor, the consolidation block 508 of 1,500, the set equal to the sum of its parts to the token,
`spec` 27 and 4 (the four record pins a landing rewrites) and `unseen` 3 and 0. P1, the reserved
prefixes (+59 real), lands at M-publication-gate with the refusal that makes it true (R6), so this
milestone lands ten: **10,497 real (+154)** by that sum, measured again on the instrument when it
lands. K1 lands with defect 2253's repair and H3 with defect 2255's, a sentence landing when the
compiler keeps it.

**R1. Item 1, the order of evaluation, in its homes.** § 7, E1: *An expression's parts are
evaluated left to right as written.* § 6's table, D1: `.default(v)` *extract or fall back; `v` is
evaluated only for an error*. § 10, M10c: *An out-of-bounds index or slice aborts, the index of a
place written with `@` only after its value or the call's other arguments, and so does a slice that
splits a character.* § 11, B4r: the six built-ins written in Heroes call their function in order,
`find`, `any` and `all` stopping at the first element that decides. § 9, L2: *Labels keep the
declared order.* The check at a write's place stays after the value: it is the memory guard, not a
placement (the compiler-engineer's `a_value_grows` runs correct today and `b_value_shrinks` needs
it), and a `lent` argument's place is checked at once, as a read (the spec-warden's row). design.md
Part 5 already rules `.default()` a branch and stays. **Defect 2254 is repaired** so that
`default(x, v)` is the same branch.

**R2. Item 2, `<<` and `>>`.** § 7, S6: *Overflow aborts at every width; `<<` drops the bits it
shifts out, and `>>` copies the sign.* design.md § 4.14 stands. **The abort on a lost set bit is
refused, built and measured**: 37 lines over three generations, byte-identical, and it aborts `-1
<< 1`, exactly −2, every left shift of a negative, the mask idiom `~(-1 << 64 - k)` and the net's
own xorshift and carry adder, 14 of its 474 tests; none of Java, Go, Rust, Swift and C# traps on a
lost bit, and the two exact shifts read (Zig's `@shlExact`, Rust's `shl_exact`) refuse the sign bit
as well, so the route has no precedent among the pages read.

**R3. Item 3, a handle given back.** § 13's sentence becomes H3: *Giving one back ends it: reading
it where every path has ended it is a compile error; elsewhere handing it to C again or `==` on it
aborts, and so does handing C a copy made before, unless C has since handed its address out again
or a million other lives have ended.* The copy is stopped where it crosses, as design.md Part 8 wart
20 says and the compiler does; the compile-time half is `handle_used_after_end` (C4). **Defect 2255
is repaired**: every handle C hands the program, a plain result, an `@` cell and a field among them,
clears its address's mark, which is what *unless C has since handed its address out again* already
says. Route (a) refused (disagreement 5).

**R4. Item 4, the stack.** § 9, K1: *running out of stack aborts.* It lands with **defect 2253
repaired on Darwin and on Linux**: Darwin by the probe's witness the compiler-engineer built, Linux
by what its lane measures there (C10: on both Linux targets clang moves `sp` past a 4 MiB frame in
one instruction, probing nothing, so a frame past the guard may write rather than die;
`-fstack-clash-protection` in `flags()` turns the jump into probes), Windows by the CI's leg after
the push. How deep a program may call and how much stack it holds stay the machine's and the
compiler's (C5: one source completes under `heroes run`'s `-O2` and dies under `heroes build`'s
`-O0`), said by the paragraph (R6).

**R5. Item 5, `exit`, and the code a narrowing fails with.** § 11, X4: *(ends the program; a code
outside 0 to 255 aborts, one written as a number is refused)*, the one sentence the blind seat
measured moving every reader. § 11, N1: *`to_i8` … `to_u64` give a `T?`, because the number may
not fit, failing `does_not_fit`* (C11: the one code eight built-ins fail with is named nowhere while
§ 6 promises codes stable). **Defect 2260 is repaired**: the abort of a computed status names
`exit`, never a `.must()` the program did not write.

**R6. The paragraph `1.0.0` rests on**, in these words, kept on M-compatibility-promise's page until
M-publication-gate publishes them in `README.md` in place of the `0.x` sentence at lines 131 to 133,
and in no sentence of the specification (the spec-warden's veto):

> **What 1.0.0 promises.** A program that compiles under Heroes 1.0.0 compiles under every later
> 1.x compiler, and does what the specification says it does. Outside it: a program that relies on
> a defect, the compiler doing other than the specification says, which a 1.x release may repair;
> a repair that keeps a program from crashing or corrupting memory, which may refuse or stop a
> program that did; what the specification leaves open, such as the order of a map's keys, how
> deep a program may call and how much memory it may hold; and an error in the specification
> itself, which a 1.x release may correct.
>
> **What it covers.** The language the specification describes and the functions it names; the
> functions `hero_os.h` declares at 1.0.0, their names, their types and what each result hands its
> caller, which a 1.x release may add to but never removes, narrows or changes; and the `heroes`
> command's exit codes (0 clean, 1 the input has a diagnostic, 2 the tool could not run), its
> diagnostics' codes and `heroes check --json`'s schema, never their words. Any other C a program
> binds is outside it: its header is read again at every build, so a change in a name, a type or a
> record's fields stops the program compiling where it binds it, and what no header shows is that
> library's to promise. So is a built program meeting a newer shared library, the runtime's own
> `heroes_runtime.h`, which a program may not bind, and every name the compiler's C takes, `h_`,
> `hero_`, `Hero`, `HERO_` and `HEROES_`, which a program's own C may not declare.
>
> **It runs one way.** A program declares no version: a newer 1.x compiler reads an older 1.x
> program, and a change of meaning, a new refusal, a new built-in or a new reserved word waits for
> 2.0.0.
>
> **A Heroes program stops rather than guesses.** There are no exceptions: an error a program does
> not handle ends it at once, saying why and where, with a status that is not 0, and nothing
> catches that stop. A program meant to run for a long time runs under something that starts it
> again.
>
> **The history stays.** `main` and the published tags are never rewritten again.
>
> **2.0.0 may break this promise**, and says which programs it breaks.

Each sentence is made true before it binds: *nothing catches that stop* by **defect 2257's
repair** (the stop made final, one line in `hero_abort`; systemd's `on-failure` restarts on a
status that is not 0 or a signal, the deployment model's own reason); *saying why and where* by
**defect 2258's repair** and the task for every stop raised inside a bound `hero_os.h` function
(C8: `hero_thread_join` twice says why and not where; 99 `hero_panic(` sites against 6
`hero_panic_at(`); *which a program may not bind* by **defect 2256's repair**, a program's `extern
"heroes_runtime.h"` refused once the three declarations Heroes sources bind move to `hero_os.h`;
*may not declare* by the reservation's enforcement, the emitter's own `f_<field>` and `t<N>` moved
under `h_` first; *what each result hands its caller* by `hero_os.h`'s twelve `HeroStr` results
stated where a check reads them and its 21 functions no program outside the compiler binds moved out
or frozen knowingly; the codes' half by item 7's list (R9). Those not repaired in this milestone are
tasks of M-publication-gate, the last gate before `1.0.0`, filed with this sitting.

**R7. What a `1.x` may change: rule (A), provisionally, and the question to the author.** A 1.x
refuses no program its 1.0.0 compiled except under the four exclusions; a new thesis rule, built-in,
reserved or foreign word waits for the next major release, which says which programs it breaks; and
*It runs one way* is a rule of `1.x`, not a fact of the day (C16). The measured cost (C2, C3): 13
thesis rules joined in the 52 days from `v0.1.0`, six of them filed as defect repairs, and five
names were taken from programs in the same days; under (A) every one of those after `1.0.0` would
wait for `2.0.0`. **(B)**, the alternative put to the author: a new thesis rule, defined by the
compiler's own list (`is_thesis_rule`, what `--permissive` drops), as a fifth exclusion, so the
thesis loop keeps refusing inside `1.x` and the promise is weaker by exactly that list. **Refused
outright** (C9 (b)): `--permissive` offered as the way an older program keeps compiling, a mode kept
for compatibility, which the author's rule refuses before `1.0.0` and R7 after it. Nothing landed
in this milestone differs between (A) and (B).

**R8. Task 2237, now.** The eight place-less runtime entry points kept beside their `_at` twins
(C9 (a)) serve no seed: today's calls none of them, and `v0.5.0`'s cannot compile against today's
runtime at all (`'31 == 30': heroes_runtime.h is from another compiler`). They are a deprecated alias
kept for an older program's sake, which the author's rule refuses; **the rule reaches the runtime's
C names**, and 2237 is done in this milestone, not after a release.

**R9. Item 7, a diagnostic's code.** `1.0.0` promises the code and never the words, as Go's type
checker and Swift's diagnostic groups do and more than the `"schema"` route the compiler-engineer
priced (C21: under it a `1.x` may rename a code whenever `"schema"` moves): **a `1.x` renames no
code and withdraws none, and a mistake it told with one code it tells with the same code**; a new
code may be added. design.md § 4.17 gains that sentence beside its rule for `"schema"`, which keeps
its own (a member's meaning or a member leaving). Before `1.0.0` a list of the codes the compiler
can hand a diagnostic (285 by the compiler-engineer's reading, 43 of them pinned by no annotation)
is held by the net, so that a code vanishing fails and a code added is told; a task of
M-publication-gate. Measurement 060's 242 is not the promise's list.

**R10. Item 8, the compatibility suite.** Its corpus is `examples/` and the ten reference programs
of `tests/thesis/`, both as each release tag holds them; `run/` (380 of its 563 files are repairs'
witnesses, 129 expected outputs pin an abort's words) and `selfhost/` are refused. **It asks `build
--emit-c`, not `check`** (C6: `check` reads no header, and 371 of 371 `unsupported` cases pass it
while `build` refuses them), skipping a program whose header the machine lacks as the `unsupported`
form does. Running the 66 corpus programs that hold an expected output, the promise's *does what
the specification says*, is a task of M-publication-gate; before `v1.0.0` the suite tells, from it
judges.

**R11. The release number from `1.0.0`** (C19): `.claude/rules/records.md` § Release tags reads
forward, `Y` moving with any change of the specification and `Z` with none, and gains one sentence:
`X` moves only with a release that breaks this promise and says which programs it breaks.

**R12. A map's order, put to the author** (C20). `keys(m)` gives the keys *in no order* (§ 10),
while the runtime's seed is fixed (panel 006) so that the compiler's own output is reproducible, and
every run of one build gives one order: a program that relies on it works until a release changes
the hash, then breaks in silence. Provisionally the order stays outside the promise as the
paragraph says. Before `1.0.0`, where nothing is promised, the author chooses among promising an
order (Python 3.7's insertion order, a change of the runtime's representation), breaking reliance
early (Go 1's order varied per run, which panel 006's reproducibility refuses for the compiler's
own maps), or today's (unpromised, fixed per build); the recommendation is in the sitting's decision
issue.

## What the conservative resolution would have been, so the author can choose it

The spec-warden's recommended set as it stood (E1, M10a, B4r, L1, S3, H1, K1, X4, +83 real); the
paragraph's draft repaired only where a seat showed it false; (B) for R7; task 2237 after a
release; the suite on `check`. It was not adopted because it leaves `.default(v)` predicted wrong by
every reader of E1 with § 9 (C1), the write's index sentence wrong on a nested read (C15), a
compile error described as an abort (C4), the promise's instrument blind to every program that
binds C (C6), the promise with no clause for the next robustness repair (C14), and an old spelling
kept beside the new against the author's rule (C9).

## Prior art, per resolution

The historian's report (`232-reports/historian.md`), every page marked `verified` there and
**re-read by the coordinator on 2026-10-29**, by `curl` with the tags taken off where the page was
too long to read whole:

- **R1**, *reached first by Go*: [The Go Programming Language Specification, *Assignment
  statements*](https://go.dev/ref/spec) (language version go1.27): *"The assignment proceeds in two
  phases. First, the operands of index expressions and pointer indirections (including implicit
  pointer indirections in selectors) on the left and the expressions on the right are all evaluated
  in the usual order. Second, the assignments are carried out in left-to-right order."*
- **R1**, *reached first by Java*: [JLS SE 21 §15.26.1](https://docs.oracle.com/javase/specs/jls/se21/html/jls-15.html):
  for an array access on the left, *"First, the array reference subexpression ... is evaluated"*,
  then *"the index subexpression"*, then *"the right-hand operand"*, and only then *"if the value of
  the index subexpression is less than zero, or greater than or equal to the length of the array,
  then no assignment occurs and an ArrayIndexOutOfBoundsException is thrown"*.
- **R2**, *reached first by Go*: [the specification, *Integer overflow*](https://go.dev/ref/spec):
  *"For signed integers, the operations +, -, \*, /, and << may legally overflow and the resulting
  value exists and is deterministically defined by the signed integer representation, the
  operation, and its operands. Overflow does not cause a run-time panic."*
- **R2**, *reached first by Java*: [JLS SE 21 §15.19](https://docs.oracle.com/javase/specs/jls/se21/html/jls-15.html):
  *"The value of n << s is n left-shifted s bit positions; this is equivalent (even if overflow
  occurs) to multiplication by two to the power s. The value of n >> s is n right-shifted s bit
  positions with sign-extension."*
- **R6 and R7**, *taken from Go 1*: [Go 1 and the Future of Go Programs](https://go.dev/doc/go1compat):
  *"It is intended that programs written to the Go 1 specification will continue to compile and run
  correctly, unchanged, over the lifetime of that specification"*, and its exclusions, among them
  *Security* (*"A security issue in the specification or implementation may come to light whose
  resolution requires breaking compatibility"*), *Unspecified behavior*, *Specification errors* and
  *Bugs* (*"If a compiler or library has a bug that violates the specification, a program that
  depends on the buggy behavior may break if the bug is fixed"*).
- **R6**, *reached first by Go*, the same page: *"It is impossible to guarantee long-term
  compatibility with operating system interfaces, which are changed by outside parties. The syscall
  package is therefore outside the purview of the guarantees made here."*
- **R6**, *reached first by Erlang/OTP*: [Supervisor Behaviour](https://www.erlang.org/doc/system/sup_princ.html):
  *"A supervisor is responsible for starting, stopping, and monitoring its child processes. The basic
  idea of a supervisor is that it is to keep its child processes alive by restarting them when
  necessary."*
- **R6**, *reached first by systemd*: [systemd.service](https://www.freedesktop.org/software/systemd/man/latest/systemd.service.html):
  *"Setting this to on-failure is the recommended choice for long-running services"*, and *"the
  service will be restarted when the process exits with a non-zero exit code, is terminated by a
  signal"*.
- **R9**, *reached first by Swift*: [SE-0443](https://github.com/swiftlang/swift-evolution/blob/main/proposals/0443-warning-control-flags.md)
  (implemented in Swift 6.1): *"A diagnostic group is a stable identifier for an error or warning.
  ... diagnostics within the compiler may change, but we need to provide stable user-facing
  identifiers for them."*
- **R9**, *reached first by Go's type checker*: [`internal/types/errors/codes.go`](https://github.com/golang/go/blob/master/src/internal/types/errors/codes.go):
  *"Error code values should not be changed: add new codes at the end."*

R3, R4, R5, R8, R10 and R11 rest on no precedent.

## What the seats could not run

- **Off this Mac**: the stack's shapes on Linux and Windows (C10 measured the Linux assembly, not a
  run); `c4` on the CI's Linux legs; `s9` over glibc's free list; route (a) anywhere but here.
- **Readers of the adopted words**: the blind seat read P (E0, S0, X0), not E1 with D1, M10c, S6 or
  X4, and items 3 and 4 had no reader (C7). The spec-warden's 30 further sessions (about 4.20 USD)
  to score its predictions on B4r and on W under E1 with M10a were **not run**, by the author's
  instruction of that evening to aggregate and finish in hours; those two predictions stay
  unscored.
- **The critic's blind task for K0 against K1** (C17, 20 sessions, at most 2.80 USD): not run, by
  the same instruction; K1 is adopted on what it covers, one frame and a thread's own stack, not on a
  reader.
- **The paragraph's real price in `README.md`**: `--refresh` refuses every path but the
  specification and the contract, so its vendored count alone stands (+270 on the README's 4,540).
- **Whether every one of the 285 codes is reachable**: five run, ten guesses drew a neighbour
  (the compiler-engineer's 7.1).

## Found beside the sitting

Nine defects, each reproduced by the coordinator with the lane's compiler and filed the same
evening: 2253 to 2259 (above), **2260** (a computed `exit` status outside 0 to 255 aborts naming a
`.must()` the program never wrote) and **2261** (two swapped labels told as four mistakes), the last two `adjacent` and found by
the critic. With 2211, 2212, 2242, 2251 and 2252, open before the sitting, they are the batch this
milestone closes: fourteen defects, every one open.

## Process, recorded against this sitting

- The critic read the briefs before any seat (34 findings, repaired, fe4f42de9); the blind seat was
  registered before its first session (5b90ee119).
- The ffi-pragmatist was stopped mid-report at about 21:16 by the session's five-hour limit and
  resumed at 21:19, the session moved to another account, from its own report; nothing it had
  written was lost. It had edited lines of its own report in place before then and listed every such
  edit underneath at 21:21.
- The compiler-engineer corrected its own 4.2 (the output it first said was lost) and two line
  citations, underneath; the critic corrected its own F10 and F23 (C18, C13) the same way.
- The critic wrote temporary files to `/tmp` and removed them in the same loop, and said so in its
  report: a breach of its binding, nothing kept.
- The historian's fetches left one PDF and eight folders of rendered pages under the session's own
  folder of `~/.claude/projects/`, outside the repository, unasked; whether to remove them is the
  author's, asked that evening.
- Two peer sessions were told of the first filing; both messages expired unread, the sessions
  holding them for an approval no one gave.
- The coordinator read a `pmset` listing cut by `head -5` as Amphetamine's absence and took a
  `caffeinate` assertion the machine did not need; the author corrected it, and it was stopped.

## Author's verdict

*Pending*: https://github.com/heroes-lang/heroes/issues/2262, R7 (A or B), R12 (a map's order) and R8
(the rule reaching the runtime's C names) the three choices inside it.

**RATIFIED, 2026-10-29**, R1 to R12, the author answering that evening in the conversation the
three choices decision 2262 put inside the ratification, meant as: *I ratify everything*. **R7**:
rule (A), as adopted. **R8**: yes, as adopted; task 2237 had landed that evening. **R12**:
**insertion order**, against the provisional resolution and the coordinator's recommendation: `1.0.0`
promises a map's keys in the order they were first written (Python 3.7's route), so R6's paragraph
no longer lists a map's order among what the specification leaves open, and the runtime's
representation and § 10's sentence are owed before `1.0.0` (feature 2274, M-publication-gate).
Recorded as a reading (CLAUDE.md § 4).

**Moved the same night** by the author's instruction, meant as: *it should be done before the
publication gate, since developing the packages' core without it would be a problem; let us do it
now*: feature 2274 is M-compatibility-promise's, and § 10's sentence lands with the others.

## The predictions scored at the milestone's close, 2026-10-30

The rows of § Predictions to score still reading a moment rather than a result, each scored
against the command that settles it:

- **The coordinator's 10,497 real for the synthesis's set** (from the spec-warden's parts): **hit**.
  Step 9 (641b43017) priced the landing on `--refresh` at 10,343 to 10,514 real: the sitting's ten
  at +154, the spec-warden's sum exact to the token, so 10,497 before § 10's map order, which the
  author's turn of R12 added after the prediction and which cost +17 (the ledger's row 8019).
- **The spec-warden's blind re-run of W under E1 with M10a, and of `xs.any(f)` under T and B4r**:
  **lapsed**, not run by the author's instruction of 2026-10-29 to aggregate and finish in hours
  (§ What the seats could not run). It bought no clause: row 8019 pays B4r and the order of
  evaluation with the promise (decision 2217's *A*) and E1's neighbours with sentences false
  today, so nothing returns to a decision.
- **The spec-warden's `k2` exits 139 with nothing on stderr on Darwin arm64 until the window is
  repaired**: **hit**, on its after half. `k2_two_mb_frame.hero`, taken from the sitting's evidence
  archive and built with the batch's stage 2 on this Mac on 2026-10-30, exits 134 saying
  `panic: stack exhausted in k2_two_mb_frame.make`; its before half is the seat's own run.
- **The compiler-engineer's `bigframe18b`, 139 and an empty stderr until `stack.c` changes, then
  134 `stack exhausted in bigframe18b.go`**: **hit** on Darwin arm64, the same build that day
  exiting 134 with exactly that line. **Its Linux half, *the CI's Linux legs stay red with the
  Darwin witness alone***, met no run: the Linux route (`-fstack-clash-protection`) landed in the
  same commit as the Darwin witness (0c2a2f96a), whose body records the Linux arm64 image at 139
  in 8 of 10 thread stacks before that route and 10 of 10 panics after; the CI's legs read
  defect 2253's cases after the push.
- **The ffi-pragmatist's route (b) changes no emission and no golden**: **hit** for route (b) as
  the seat stated it, the clause narrowed. The landing was (b) **with C4's compile-time half**,
  which the synthesis added (§ Disagreements, stated plainly, item 5), and that half, defect 2255's repair
  (ea7eacada), moves the emission of every program whose `extern` call hands it a handle without
  a mark, as its body says: a cost the prediction did not cover, not a miss of it.

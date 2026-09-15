# Panel 153 — report of the completeness critic

I give no verdict on Q1 or Q2. Everything below is a command I ran in my own copy (`scratchpad/critic-tree`, seed built in **3.15 s real**) or in `scratchpad/critic-work`, or a sentence I mark unrun. The coordinator's prototype was run from a copy of the binary against an identical runtime (`diff -rq coord/runtime critic-tree/runtime` — no differences).

## A. A route nobody listed

### A.1 Route C is not a third route. It is Route B's first half, and both A and B need it

I ran the prototype on the coordinator's own program. `heroes build real-gai.hero`: shipped compiler **exit 1**, `error[duplicate_tag]`; `heroes-narrowed` **builds, runs, prints `0`, exit 0**. **The central claim reproduces.**

But the narrowing is textually Route B's own first clause. The shared brief's Route B reads *"`one_tag_one_type` widens to one tag, at most one handle and at most one fielded record"*; the prototype's comment reads *"one tag may carry one handle and one fielded record"*. Same sentence. The coordinator's § 8 concedes it: *"Route C is necessary and not sufficient."*

What the sitting missed is the other half of that: **Route A needs the narrowing too.** Under A, `AI` with fields is still a handle, so `@hints: AI` is `struct addrinfo **`, and A's own text says the fields *"are read and never written"*, so no hints value can be built through it. Real hints therefore need a by-value record on `tag addrinfo` beside the handle — the pair I watched the shipped compiler refuse twice today. So the narrowing is not a route; it is a **precondition common to both**, and the sitting's Q1 was never the A-or-B question the brief posed.

### A.2 The prototype ships the defect its own source file names

`selfhost/handles.hero:126-132` gives this as a reason for the rule: *"the diagnostic machinery cannot even name the second — `record_by_tag` answers with the FIRST record carrying the tag, so a message about the second would carry the first's caret."* The prototype removes the rule for the handle-plus-value pair and does not touch `record_by_tag`. I built the shape that reaches it:

```
extern "sqlite3.h" link "sqlite3"
    record Db tag sqlite3
    record DbValue tag sqlite3 partial
        nBtree: i32
```

- shipped: `error[duplicate_tag]`, caret on **line 3**, `DbValue`. Correct.
- narrowed: `error[ffi_unknown_tag]`, caret on **line 2**, `record Db tag sqlite3` — the handle, which is correct — and the note's repair reads *"drop the field block and the `record` becomes C's pointer to it, `record Db tag sqlite3`"*. `Db` has no field block, and the repair printed is the line it is already pointing at. The offending record, `DbValue`, is never named.

Exit 1 both ways, so no suite sees it. The compiler-engineer made this its adoption condition 2; the coordinator's measurements do not mention it.

And the only golden guarding the rule is `tests/golden/check/ffi-handle-refusals.hero:20`, `record Db2 tag sqlite3` beside `record Db tag sqlite3` — **two handles**. `check` passing 120/120 under the prototype proves the two-handle case still fires and proves nothing about the pair the narrowing admits, which has no annotation and no snapshot anywhere in the tree.

### A.3 The route nobody listed: the dereference already exists, in C, and it compiles today

The shared brief says *"Nothing dereferences and nothing takes an address. No operation turns a handle or a `ptr` into the struct it points at."* The coordinator's § 8 says *"nothing turns a handle into the value"* and its table says *"turning an `AI` into an `AddrInfoValue` — nothing in the language does this"*. The compiler-engineer reports *"Third route: none found"* after searching the IR op list, the Tier-2 library mechanism, generics and the `@` copy-in path — all four inside the compiler.

Nobody searched the FFI, which is where §1.11 says everything in this language comes from. Three lines of header, and the **shipped** compiler reads a struct C hands back by pointer and walks the list:

```c
/* airead2.h */
#include <netdb.h>
typedef struct addrinfo AddrInfoValue;
static inline AddrInfoValue ai_read(const struct addrinfo *p) { return *p; }
```
```
extern "airead2.h"
    record AI tag addrinfo
    record AddrInfoValue partial
        ai_family: i32
        ai_next: AI
    function getaddrinfo(node: cstr, service: ptr, hints: ptr, @res: AI acquires freeaddrinfo) -> i32
    function freeaddrinfo(ai: AI consumes)
    function ai_read(p: AI) -> AddrInfoValue borrows
...
    while cur != nullptr
        node = ai_read(p: cur)
        print(node.ai_family)
        cur @ node.ai_next
```

`HEROES_RUNTIME=… ./heroes build routed4.hero` — **build exit 0**, run prints `0 / 2 / 2 / 30 / 30 / 4`, **exit 0**. `diff` against the equivalent C program's output: **identical**. The variant with real `hints` (`@hints: AddrInfoValue borrows`, `AF_UNSPEC`/`SOCK_STREAM` against `"localhost"`) prints `0 / 2 / 30 / 2`, also byte-identical to its C twin. `ai.read()`'s ergonomics, at `ai_read(p: ai)`, with **no compiler change, no new word, no new built-in, no spec token, and no `duplicate_tag` to narrow** — the value record reaches the struct by typedef name, so the two records never collide.

Every field is still verified by clang against the header. The emitted unit carries `_Static_assert(_Generic(&((AddrInfoValue *)0)->ai_next, addrinfo * *: 1, default: 0) …)` — `ai_next` proved to be a pointer to the struct, at compile time, by the header.

This is not exotic. `find examples tests/golden -name '*.h'` returns **20** headers of the project's own, **16** of which contain a `static inline` body; `tests/golden/run/fixedbugs-a-handle-in-a-record-field.hero` already declares `function slot_make(n: i64) -> Slot` returning a fielded record **by value** from one. The route was in the golden tree before the sitting opened. It also builds from any working directory (measured: `.hero` and `.h` in a subdirectory, `cwd=/tmp`, exit 0), so it is not a cwd accident.

**What it costs, said plainly.** Two things, both measured:
- **the null read segfaults.** `ai_read(p: nullptr, …)` builds clean and runs to **exit 139**, SIGSEGV. design.md §1.12 forbids exactly this, and it is the one thing an in-compiler route buys that this route does not.
- the author maintains one file beside a header they do not own. design.md:2289 already blesses that as standard practice; design.md:2290's *undecided* is about **compiling** a shim, and a header of `static inline`s is never compiled separately.

### A.4 Q2's payment: a removal nobody listed, measured

The spec-warden's `removal` field says: *"For Route B, nothing — and that is a problem: after R1 and R2, the only removable text left in § 13 is R3."* That enumeration is incomplete. § 13's prose reads *"`acquires sqlite3_finalize` after a result …"* — and `sqlite3_finalize` is declared nowhere in the section; the llm-ergonomist independently flagged it as *"prose and fence were not written against each other"*. Shortening it to `acquires` (call it **R4**) measures, in my copy on the landed text:

| draft | vendored | real |
|---|---|---|
| spec as landed | 5989 | 7974 |
| + R4 | **5985** | **7965** |

**R4 is −4 vendored / −9 real, and it deletes no rule** — where R1, the payment that actually landed, deletes one (*Unmarked pointers are never freed.*). Q2 alone on the landed text prices at 5996 vendored (+7), so R4 does not pay it alone; R1+R4 together leave 4 vendored tokens of headroom under the base. Whether R4 is safe for a reader is a question for the ergonomist and is **unrun**.

## B. Claims asserted and not measured

**B.1** — compiler-engineer: *"**Third route: none found.** Searched: the IR's op list (`ir.hero:60-130`, no dereference), the Tier-2 library mechanism, generics, and the `@` copy-in path … every route needs the dereference born somewhere, and A's backend spelling and B's primitive are the two places."*
Command that settles it: `./heroes build routed4.hero` with the three-line header above. I ran it: **exit 0, output byte-identical to C**. The dereference is born in a third place — a `static inline` in a header — and CL-057's rule bites: the enumeration was of the compiler's internals, and it was handed on as a claim about the option set.

**B.2** — shared brief: *"Nothing dereferences and nothing takes an address. No operation turns a handle or a `ptr` into the struct it points at."*
True of the language's own operations; false of what a Heroes program can do. Same command, same result.

**B.3** — spec-warden, prediction: *"if Q2+R1 lands alone as drafted, … `./heroes measure spec/heroes-spec.md` prints a vendored maximum of **5990** tokens … any other number means the landed text is not the text priced."*
I ran it on the text that landed in the working tree at 17:07: **5989 vendored**, digest **`21a9dc541cfa2fa8`**, not the predicted `2ec6dad90753bed2`. By the seat's own falsifier, **the landed text is not the text priced** — the paragraph was reflowed differently. The real figure did hold: I ran `./heroes measure spec/heroes-spec.md --refresh` myself and it printed **7974 tokens on `claude-opus-5`**, so the +0 real in the done record is a real measurement.

**B.4** — spec-warden: *"Q1 Route A — object. **+79 real** … Dominated on §1.2 and on Principle 0."*
The seat's own table reads A **8053 (+79)** and B2 **8051 (+77)**. A **two-token** gap is not domination on a cost formula; the argument that actually separates them is the third-leg one (A needs the narrowing anyway), which is stated in the same field as *"an inference from the measured refusal and the rule's text"*. The §1.2 framing is not carried by the numbers under it.

**B.5** — shared brief: *"`netdb.h` declares **18** functions returning a struct pointer."* The compiler-engineer re-ran it and got **19**, naming the regex difference. I re-ran the engineer's command against `$(xcrun --show-sdk-path)/usr/include/netdb.h`: **19**. The brief's 18 is the number the historian, the warden and the ergonomist were handed; the warden repeats it in its Principle 0 argument.

**B.6** — ffi-pragmatist: *"`function read(` is defined **9** times across `examples`, `selfhost`, `tests/golden`."* Re-run: **9**. Confirmed.

## C. Contradictions between seats

**C.1 — the historian approves Route A; three seats object. The two sides are not answering the same question.**
The historian's brief instructs it to judge *"A versus B by precedent alone"* and gives it no shell; the seat says so in line 5 (*"every repository number below … is taken from the shared brief and unrun by this seat"*). The other three judged A against **this compiler**. So there is no disagreement about any proposition: one seat ruled on *"is implicit field dereference a sound tradition"* (it is — 5 of 8 designs, sourced), and three ruled on *"what does A cost to build here"*.

**What is checkable, and I checked the load-bearing half.** The proposition that decides A is not a precedent question: *can Route A alone bind `getaddrinfo` with real `hints`?* It cannot, because A's record is still a handle (`@hints: AI` is `struct addrinfo **`) and A's own text forbids writing its fields, so hints needs a by-value record on the same tag — which the shipped compiler refuses, measured twice today (`real-gai.hero` and `opaque-pair.hero`, both `error[duplicate_tag]`, exit 1). The historian was never told this and could not have run it.

**What would settle it:** a compiled Route A, the way the coordinator compiled the narrowing in 8 lines. The compiler-engineer's own condition asks for exactly that — *"a compiled demonstration that the three refusals fit in one module of at most 60 code lines"* — and nobody built it. The sitting compared a **measured** half of B against an **argued** A, and then read the asymmetry as evidence.

**C.2 — a second contradiction, about the same three refusals.** The engineer says A's danger is that field writes and `@ai.f` copy-out become writes into C's memory *"unless the checker refuses each"*, and marks the whole analysis **unrun** (*"neither route exists to compile"*). The historian's precedent answers a language where writes through the pointer are normal (Oberon, Ada, Go, Nim all permit them). Route A as the brief words it — *"read and never written"* — has no precedent in the historian's own table, because every system it cites allows the write. Checkable by re-reading the eight URLs the historian lists for **writes** rather than reads; **unrun by me**.

## D. Framing facts a seat was handed and did not check

**D.1 — "The working tree is frozen from this brief to the synthesis." It was not, and the base every seat measured against is dead.**
`git status` in `/Users/joseph/Temp/heroes-lang` now shows **nine modified files** — `spec/heroes-spec.md`, `docs/design/design.md`, the budget ledger, case-law, `DEFECTS.md`, `seed/heroes.c`, `selfhost/measure/pinned.hero`, `suite_spec.hero`, `CLAUDE.md` — plus a done record and a log entry both stamped `2026-09-15-1700`. The spec's mtime is **17:07**; four of the five reports are stamped **16:51**. **Q2 was adopted, landed and recorded ("panel 153 R1, ratification pending") before this sitting had a synthesis**, on the tree the brief declared frozen. `SPEC_DIGEST` moved from `2e77c4e72ce69512` to `21a9dc541cfa2fa8`, so the spec-warden's entire twenty-row table is priced against a base that no longer exists, and no seat could have known.

**D.2 — the ergonomist's blind A/B was not blind, and its numbers are now in an append-only record as the thing that paid.**
The brief labels the fences *"only X and Y"* and asks *"say which version is the section's current fence if you can tell, and how"* — then prints § 13 *"verbatim from `spec/heroes-spec.md`"* at the bottom, fence Y included. I diffed the brief's copy against `git show HEAD:spec/heroes-spec.md`: the two agree on the fence and on *Unmarked pointers are never freed.* exactly. The seat answered, correctly, *"The section quoted verbatim in the brief carries Y's two function lines byte for byte."* The blinding was broken by the brief itself. Further, the "ten independent attempts per version" are ten readings narrated in one pass by one seat, not ten model runs; the seat's own `prediction` section proposes the real instrument (*"Harness sqlite ':memory:' task … Instrument: count the two words in each sample's `extern` group"*), and it was **not run**. The resulting **8 of 10 / 2 of 10** is now the stated payment in `docs/records/done/2026-09-15-1700-…md`, which is append-only.

**D.3 — "It was narrowed at panel 145 for TWO HANDLES over one C type."** Handed to all five seats. `selfhost/handles.hero:126` actually reads *"panel 145's narrowing, **author decision 2026-09-13**"*. Only the spec-warden checked, and drew the consequence the rest of the sitting proceeded without: *"ratification is the author's and not the panel's"*. The coordinator's prototype narrows that rule and its measurements do not mention it.

**D.4 — the defect's own falsified sentence.** The coordinator's § 1 is right that defect 042's *"`struct stat` under `lstat` … are the same shape"* is false of its first half. Both the ffi-pragmatist (`h-lstat.hero`, exit 0) and the ergonomist (Task 3) reached the same answer independently, from briefs that asked them to. That one worked.

**D.5 — nobody was asked about the third input class.** `.claude/rules/cli-surface.md:38` names three input classes — *"a `.hero` file, argv, and the machine's environment"* — and refuses a fourth. A `.h` of the author's own beside the `.hero` is read by the compiler today, from any cwd, and **20 of them ship under test**. No brief mentions this, no seat rules on it, and the coordinator's § 4 treats it as a cost rather than as a shipped, untested-in-the-spec input class.

## E. The question the sitting should have asked and did not

> **What must the LANGUAGE do that a header of the author's own already does?**

Principle 0 (CLAUDE.md § 2) admits a form if the compiler needs it **or** it provably serves the thesis by a measured effect. The compiler does not need it — `selfhost/` declares **0** tagged group records, measured by the engineer. So the whole weight falls on the second clause, and the sitting never established a baseline for it, because the baseline — Route D, which reads and walks today with zero compiler change — was not on the ballot.

With that baseline in hand the question has a shape the panel can answer: *the built-in buys a guard (`exit 139` becomes a named abort) and a line of spec; the header buys the same read at zero tokens and no new word. Is the guard worth +77 real and a new primitive, or is the guard the thing to build — one refusal on `ai_read`-shaped bindings — with no new construct at all?* Nobody asked it, so nobody priced the answer.

Three sub-questions it drags with it, all unasked: is a `.h` beside a `.hero` a supported input class (§ 10's fourth-class refusal), what suite judges one, and does it need a spec sentence?

## F. Is the converged resolution the most robust and complete one?

**Where it is genuinely robust, and I say so first.** The engineer's choice of B over A is a §1.12 argument, not a cost one: *"where a missed refusal corrupts under A and copies under B, B wins at comparable cost"*, and the seats' own line counts (70-85 vs 65-80) back "comparable". The warden's choice of B2 over B is CLAUDE.md § 4 applied correctly — it took the dearer wording for the soundness clause. Neither is a compromise.

**Where it is not complete — and this is measured, not argued.** The one corruption class the ergonomist named, *"the one place a plausible program could compile and corrupt"* (`freeaddrinfo` on a handle read out of a node), is open **today, on the shipped compiler**, and nobody ran it. I did:

```
    node = ai_read(p: r)          # borrows
    freeaddrinfo(node.ai_next)
    freeaddrinfo(r)
```
**build exit 0, no diagnostic**; run **exit 133, stderr empty**; under `--sanitize`, `AddressSanitizer: attempting double-free … in h_routeddfree_main routed-dfree.hero:19`. The acquisition counter says nothing, because `borrows` is the honest mark and `borrows` switches the accounting off. B2 buys a **sentence** about this (*"and is borrowed, so no call consumes it"*); the sentence does not close the class, and the class does not need Route B to exist. Under CLAUDE.md § Precedence rank 3 this outranks every token in the warden's table, and it was never on the sitting's agenda.

**Where it is cheap rather than robust.** Three places:

1. **Q2's payment deleted a rule to buy 7 vendored tokens** while an unlisted removal that deletes no rule (R4, −4 vendored / −9 real, § A.4) sat unmeasured, and the warden's enumeration of what was left is falsified by it.
2. **The narrowing is being adopted with a known-broken diagnostic path.** The compiler's own comment names `record_by_tag` first-wins as a *reason for the rule*; the prototype removes the rule and leaves the lookup, and I measured the predicted wrong caret (§ A.2). It is the engineer's condition 2 and the coordinator's measurements are silent on it.
3. **Every instrument that would have settled a number was named and not run**: the ergonomist's own harness task, a compiled Route A, the `determinism`/`records`/`layout` suites beside the prototype's seven, and a golden for the pair the narrowing admits.

**My answer.** The resolution is the most robust of the **two** options the brief allowed, arrived at by a sound argument. It is not the most robust or complete resolution available, because the option set was incomplete in three ways I could run: a third read route exists and ships today; the narrowing both routes need was never severed and priced on its own; and the corruption class the whole sitting is nominally about is open in the shipped compiler and stays open under everything adopted.

**Unrun, in my own words.** I did not run the full net, nor `determinism`, `layout`, `order` or `records` beside the prototype; I did not rebuild from `selfhost/`; I did not run `real-gai.hero` under `--sanitize`; I did not re-run the ergonomist's task against real models; I did not check the historian's URLs; every number here is this Mac's, and no Linux or Windows figure is mine.
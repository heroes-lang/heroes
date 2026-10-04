# `lent`, and the pointer C hands back

2026-09-20. M-declared-extents step 22, panel 171, the full panel with the
completeness critic run **before** the synthesis.

## The decision

| | |
|---|---|
| date | 2026-09-20 |
| decision | **the word is `lent`**, in its own `CParam` slot beside `counted_by`; **a lend reaches only a `lent` parameter of an `extern` function**, `lend_kept` at an unmarked one and `lend_needs_a_header` into a Heroes function; the spec sentence is the warden's M2 at **+13 real**; the landing is **two commits** with **27 marks on 19 functions across three trees** |
| reason | four seats reached `lent` from four inputs: the llm-ergonomist 3/3 cold where `reads` scored 1/3 and `borrows` 0/3, both marking the function that keeps; the spec-warden, `reads` contradicts the document's own line 369 and `lent` needs no gloss because § 13 says *lend* seven times; the historian, D's druntime writes `scope` on `getenv(scope const char* name)` in exactly this shape; the compiler-engineer, whose veto clause the sentence carries by construction |
| design.md § | §1.7, §1.12, §4.19, §4.17, §1.6, §1.0 |
| panel | 171, provisional on the word and rule; the direction is the author's |

## The direction was not on the ballot, and the sitting says so

The author ruled the default flipped that evening, after making the performance
measurement a condition and having it met. On that half the sitting is a
**retro-record**. The seats' real objections are recorded and none is dissent
staged for the file.

## What the engineer did that no brief asked for

**It built the rule and tested the landing.** Commit A, the word alone, builds
under today's seed in 73.79 s with the fixpoint byte-identical; commit B, the
rule and the marks, builds under commit A's seed in 61.11 s, byte-identical;
**one commit breaks at `selfhost/cli/process.hero:42:40` with no binary**,
because the rung before cannot build the rung after. Cost: **+113 code lines,
eleven files, frontend only**, the emitted C byte-identical with and without the
rule, `check selfhost/main.hero` not a millisecond slower.

**And it found the third tree.** The brief counted nine functions; the ffi seat
added the embedded library's two, without which **every `heroes check` of every
program exits 2**; the engineer added `tests/harness/shell.hero`'s nine, without
which **no suite of the net can run**. The critic's census settles it: **27
marks on 19 functions**.

## The soundness clause, and the sentence carries it for free

A Heroes function `wrap(s: cstr)` forwarding its parameter to an unmarked C
parameter, called with `"12".cstr()`, passes a rule that reads only the extern's
parameter. So the rule reads the callee too: **a lend goes to an `extern`
parameter marked `lent`, or nowhere.** Panel 122 had admitted a lend into a
Heroes `cstr` parameter *because legality depended on what another file
declares*; under the flip that is true of every lend, so the ground is spent.
And the warden's sentence — *a lend reaches only one so declared* — already says
it: a Heroes parameter cannot carry the word, so it is never *so declared*. The
critic confirmed: by construction.

## The pointer C hands back, and why the sentence promises less than the drafts did

The critic attacked the rule where the engineer had stopped and found that **it
reads the lend expression while the hazard is the pointer**. `strchr(s: cstr
lent) -> cstr` returns an address inside the lent bytes; the rule admits a
C-returned value at an unmarked parameter; C keeps a pointer into Heroes memory.
Five programs, three doors, two types, `check` 0 and use-after-free under ASan.

**The flip did not open it — today's compiler is identical there — it left it.**
The corpus already knows the shape as defect 024, closed by a caller lease. And
every draft sentence but the warden's over-promised about it: *a pointer C owns*
is false of a value that points into Heroes bytes. **The sitting lands the
sentence that is silent where the rule is silent**, which is CL-005's standard
for a refusal applied to the document.

## The cost Swift never published, now a number

The historian found no published count of what Swift's foreign default cost its
binding authors. The coordinator ran the seat's unrun command: **77 of 9,386
headers** on this Mac's SDK carry a `noescape` mark ten years later, under one
percent, and at libc level they mark `qsort`'s and `scandir`'s function
pointers, not `const char *`. The flip produced no mass annotation anywhere it
was tried.

## Three gaps the landing owes, and one item filed

`lent` on an uncounted `ptr` is accepted silently and should be `lent_shape`;
`lend_kept`'s notes for a `.ptr()` lend say `cstr` and offer a field lease that
does not exist; and the first `guess` note lacks the *if C frees, neither route
works* sentence, so following it on defect 070's fourth shape yields `check` 0
and run 134. All three close in commit B. **Filed rather than fixed**: two
modules may carry contradictory marks on one C function, `check` 0 today, which
is the shape that broke upstream Clang's `noescape` on its first day.

## What closes and what does not

**Defects 066 and 068 close with commit B.** **Defect 070 stays exactly where it
was**, measured. **The pointer C hands back is written down as what the flip
leaves**, not promised away.

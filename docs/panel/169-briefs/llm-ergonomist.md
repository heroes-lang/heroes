# Panel 169 — brief for the llm-ergonomist

**Read `spec/heroes-spec.md` and nothing else.** Not design.md, not the
repository, not the other briefs, not the other seats' reports. Your verdict
carries information precisely because your input is the document a model is
given to write Heroes, and nothing more.

Your report is an **experiment**, not an opinion.

## The situation, stated without the repository

A Heroes program hands C a pointer into its own bytes, and C **keeps** the
pointer and reads it later. The document's § 13 offers two spellings for handing
bytes to C:

- `s.cstr()` and `f.ptr()`, which **lend** for the duration of a call;
- `x: cstr @ s.lease()`, which the document says *"is a COPY of the bytes that C
  may read for as long as the program says, and `end_lease(@x)` frees it"*.

**Two programs that the document admits, and that give a wrong answer at run
time**, both accepted by the compiler with no message:

```
# A — the bytes die with the frame
function lend_and_keep()
    s: Sl @ mk()
    c_keeps(p: s.name.ptr(), n: 8)

function main()
    lend_and_keep()
    print(to_str(c_reads_later()))     # reads a dead frame
```

```
# B — the bytes die at end_lease
function main()
    label: cstr @ "row-0-payload".lease()
    c_keeps_text(s: label)
    end_lease(@label)
    print(to_str(c_text_length()))     # reads freed bytes: prints 0, honest is 13
```

```
# C — the record is rewritten while C holds a field's address
function main()
    s: Sl @ Sl(name: [72, 2, 3, 4, 5, 6, 7, 8], id: 1)
    c_keeps(p: s.name.ptr(), n: 8)
    print(to_str(c_reads_later()))     # 72, honest
    s @ Sl(name: [1, 2, 3, 4, 5, 6, 7, 8], id: 1)
    print(to_str(c_reads_later()))     # 1 — the program never passed this to C
```

## Your three tasks

**Task 1 — the blind pair.** Two candidate sentences for § 13. Judge them
label-stripped; they are given in no meaningful order.

> **V1.** *A C function that keeps what it is handed takes a handle, never a lent
> pointer: `s.cstr()` and `f.ptr()` live for the call and no longer. A handle is
> a group `record` with a `tag` and no fields; `acquires` names the call that
> begins its life and `consumes` the one that ends it, and a handle nobody gives
> back aborts when `main` returns, saying how many.*

> **V2.** *A binding whose field has been lent is not written again while the
> lend can still be read: the lend borrows the binding, and a write to it is a
> compile error naming both lines.*

For each: write program C above (and, for V1, program B) **the way the sentence
tells you to write it**, using only what `spec/heroes-spec.md` contains. Report
whether you could, on the first try, and what you had to guess. **A sentence you
cannot write a program from is a sentence that does not work**, whatever it
costs.

**Task 2 — the silent-error rate.** For each of the three programs above, say
what a competent model reading today's § 13 would believe about when the bytes
die. Be specific about which sentence produces the belief. The document's phrase
*"for as long as the program says"* is the one under suspicion: what does a
reader take *the program says* to mean, and who do they think decides?

**Task 3 — the one that decides your verdict.** Programs A, B and C are each
**two lines away from a correct program**. Write the correct version of each,
from the document alone. If you cannot, say which sentence is missing. That
answer is worth more to this sitting than any preference between V1 and V2.

## What to report

- a verdict on each variant: approve · object · VETO, with the ground;
- your first-try results, as counts out of the tasks attempted;
- **what you had to invent** because the document does not say it;
- a falsifiable prediction about a reader's behaviour, with the number;
- and, if a third sentence would work better than both, write it. You are the
  only seat whose input is what a reader actually gets.

Write your report to `docs/panel/169-reports/llm-ergonomist.md`. You hold a veto
on non-local constructs: a rule a reader must hold two files in mind to obey is
one.

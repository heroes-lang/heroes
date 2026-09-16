# Panel 156 — llm-ergonomist report

Input: `spec/heroes-spec.md` and `docs/panel/156-briefs/llm-ergonomist.md`, and
nothing else. I did not open design.md, the repository, the runtime, any test or
any other brief. I do not know which machine is A, B or C.

**verdict: object** (I object to the silence, and to adopting B's package whole.
I do not cast my veto.)

---

## Task 1. Find the sentence

The document says **nothing** about a `nullptr` handle reaching C. That silence
is the finding. Four sentences are the closest it comes, and I quote all four
because the reader's answer is assembled from them rather than found in one.

§ 6, which is where the document defines its one word for stopping:

> An abort ends the program at once, saying why; no `T?` carries one.

§ 11, which is the only promise about what a running program writes:

> `print` writes its values with no separator and exactly one trailing newline

§ 13, which hands the reader the null and the type and then stops:

> One with a `tag` and no fields is a **handle**, C's pointer to that type:
> `record Db tag sqlite3` is `sqlite3 *`. `nullptr` is its null and `==`
> compares the address; a map key is an error.

§ 3, the general warning at the boundary:

> A `ptr` is a copied ADDRESS, wherever it sits: two copies reach one foreign
> thing, so a function taking one without `@` may still change, or free, what C
> holds.

Note what § 13 does: it tells me a handle has a null, and that I may compare it,
and it never tells me what happens if I do not. The language everywhere else
names its stopping points beside the operation (overflow, division by zero,
shift count, ordering a `nan`, an out-of-bounds index, a slice splitting a
character, recursion too deep, `.must()`, a lease nobody ends). This one site is
unnamed.

Is the promise kept?

| | kept? | why |
|---|---|---|
| **A** | **kept** | It ends at once and says why, which is all § 6 asks. The `7` its `print` wrote is what § 11 promised. A is the only machine that breaks nothing. |
| **B** | **broken, § 11** | The program *executed* `print(node_value(p: real))` before reaching line 4. § 11 says `print` writes its values and exactly one trailing newline. B wrote nothing. § 6's *at once* can be read as overriding, and the document does not resolve the tension, so a reader cannot predict B from the text. |
| **C** | **broken, § 6, or unnamed** | If this is an abort, C flatly breaks *saying why*. If it is not an abort, the document has **no word at all** for the state the program is in, which is the worse of the two, because a reader of the whole language cannot name what happened to them. |

One more finding from reading the whole document: the word **`panic` does not
appear in it**. § 6 fixes one vocabulary, *abort*. Both A and B print `panic:`.
A model that reads that line and greps the spec for `panic` finds nothing and
concludes the event is outside the language.

---

## Task 2. The blind repair

I wrote the next action twice, honestly, before scaling the program.

### Handed A's line only

```
panic: a null pointer was read through — a handle or ptr holding nullptr reached C
where C dereferences it, at offset 0x0, called from node_value
```

My next action, written as I would issue it:

1. `grep -n 'node_value' main.hero` → two hits, lines 8 and 10.
2. For each hit, ask where the `Node` argument came from. Line 8's `real` came
   from `node_open`; line 10's `empty` is literally `nullptr` on line 9.
3. Repair line 9, or guard line 10.

But step 2 is not what actually decided it. What decided it was **`7` on
stdout**: print #1 completed, so the failure is call #2. Without that byte I had
two candidates and no way to choose, because C functions return null too and
`real = node_open(v: 7)` is just as plausible a null as `empty` is, for anything
I can read in the line.

### Handed B's line only

```
panic: ... called from main.main
```

1. Open module `main`, function `main`. That is the whole program here, so the
   name narrows nothing.
2. B does **not say which C function faulted**. In this program every candidate
   is `node_value`, so it costs nothing; in a `main` that also called
   `node_close` and `node_free` I would have to try each.
3. No stdout prefix, so I cannot tell whether I died on the first C call or the
   last.

### Which got me there faster, and the finding I did not expect

**A did**, in this program, and not because its blame name is better. A carried
two pieces of information where B carried one that happened to be vacuous.
`main.main` is a perfect anchor only because `main` is the entire program.

So I scaled it. Second program, written under the current spec, one wrapper,
which is the shape any program above forty lines has:

```
extern "node.h"
    record Node tag node
    function node_open(v: i64) -> Node
    function node_value(p: Node) -> i64
    function node_child(p: Node, i: i32) -> Node

function value_of(n: Node) -> i64
    return node_value(p: n)

function main()
    root = node_open(v: 7)
    print(value_of(n: root))
    kid = node_child(p: root, i: 0)
    print(value_of(n: kid))
```

`node_child` returns null for a missing child. Nothing in Heroes says so;
nothing in Heroes could.

- **A** now says `node_value`. One call site, inside `value_of`. `value_of` has
  two callers. A does not say which. I am stuck at the wrapper, and the only
  thing that unsticks me is the surviving `7`.
- **B** now says `main.value_of`. One function, two callers, no output prefix.
  **I am equally stuck, and I have lost the `7`.**

**Neither blame name locates the defect**, because both are function-granular
and the defect is the caller's *argument*. They are not competitors: A names the
callee, B names the caller, and the informative one is whichever is rarer in the
program. In a 500-line program with forty `node_value` calls spread over six
modules, A's name is a constant and B's varies, so B wins decisively; here, with
one wrapper, both lose and the output prefix wins.

Where I guessed: I read `called from node_value` twice. First reading, *the
fault is somewhere node_value called*, i.e. deeper in C. Second reading, *the
fault is at a call to node_value*. The wording admits both, and I only chose the
second because the brief told me `node_value` is the C function. Without the
brief I would have gone looking in my own source for a Heroes function named
`node_value`, found none, and concluded my build was stale.

---

## Task 3. The lost line

**It matters, and it matters more than the blame name.** Here is the case where
it changes what I conclude about the program.

```
function main()
    root = node_open(v: 0)          # the 0 is the bug, and it is upstream
    print(f"root value {value_of(n: root)}")
    kid = node_child(p: root, i: 0)
    print(value_of(n: kid))
```

Under A I read `root value 0` and conclude: the tree was built empty, the defect
is at `node_open(v: 0)`, three lines above where the program died. Under B I
read nothing and conclude: the defect is at the line that died. **B sends me to
the symptom, A sends me to the cause**, and B does it silently: my repair
compiles, runs, and dies again one line later.

The second cost is larger and is specific to how a model debugs. A model has no
debugger and no core dump. Its loop is *add a print, rebuild, run, read the
prefix*. **Under B that loop stops working**: every print I add before the crash
is invisible, so I read the silence as *the program never reached here*, add
prints further up, see nothing again, and converge on a wrong region. B does not
merely withhold information, it **manufactures false negatives**.

Does the document license either? § 11 promises what `print` writes and says
nothing about buffering or about surviving a stop. § 6 says an abort *ends the
program at once*, which can be read as licensing the drop. So the document
licenses **both and predicts neither**, and a reader cannot tell from the whole
language which of the two debugging worlds they are in. That is the cleanest
example I found of an unwritten rule that changes a program's observable
behaviour.

---

## Task 4. Name the missing one

**C is a defect of the language, not of the program.** A and B prove the check
exists and is cheap enough to ship; C is the machine where a thing the language
does at every other stopping site is missing. A program that dies with a
non-zero status and not one word is the only outcome here that leaves the reader
with no next action at all. It is not excluded by the document, which is exactly
the complaint.

A and B are defects of *degree*, not of kind. B's dropped output is the more
expensive of the two, for the reason in task 3.

Ranking, best to worst for someone debugging:

1. **A.** Says why, names something greppable, and keeps the progress trace.
2. **B.** Says why, names something I can open, and destroys my primary
   instrument. The name is the better *kind* of name and the package is worse.
3. **C.** Unactionable. The gap from B to C is far larger than the gap from A to
   B.

**The missing one is the fourth row**, and it is the one I would ship:

| | output | error stream |
|---|---|---|
| **D** | `7` | `abort: a null handle was read through in C, at main.hero:10, in main.main, calling node_value` |

Four things, in the order I use them: the document's own word (`abort`, not
`panic`); **a source location in a file I can open**; the Heroes function; and
the C function, last, as the discriminator when one Heroes function makes
several foreign calls. The surviving `7` is not decoration, it is the line that
tells me *how far I got*.

Neither A nor B is D, and D is not a compromise between them: it is A's output
behaviour with B's blame name plus the one field both are missing.

---

## What the verdict answers

**Which blame name serves a reader better?** The **Heroes function**, if exactly
one may be printed, for two reasons my experiment produced rather than my
intuition: it is the only one of the two the reader **can edit**, and it
**varies per call site** where the C symbol is a constant across every call of
that function. But my measured answer is that the question is slightly wrong:
both names together cost about four words and neither alone located the defect
in a program with one wrapper. Ask for both, and add the line number, which
outperforms either.

**Is the answer different for a human than for a model?** Yes, and in the
direction that matters here. A human has a debugger, a backtrace and a core
dump; given the C symbol they recover the Heroes frame from the stack in
seconds, so for a human A's name is nearly sufficient. A model has **grep and
the text of the line**. A name that does not appear anywhere in its own source
is close to useless as an anchor, and worse than useless when it is
grammatically ambiguous, as in task 2, where I nearly went hunting for a Heroes
function called `node_value`. The model's need for the Heroes name is strictly
stronger than the human's.

**Is a program that says nothing when it dies acceptable under the language as
specified?** Under the document as written, it is **not excluded**, because
§ 6's *saying why* binds only what the document calls an abort, and the document
never says this is one. Legal by silence. It should not be acceptable, and the
repair is one sentence, not a mechanism.

**Should the specification say more?** Yes. Two merges, no new sentence
anywhere, both joining a clause that already exists, which is the cheapest shape
this document takes.

Into § 11, onto the `print` sentence, where it covers **every** abort in the
language and not just this one:

> `print` writes its values with no separator and exactly one trailing newline**;
> an abort never loses it**.

Six words. It settles B, and it settles the same question for overflow, index,
`.must()` and every other stop, which is why it belongs there rather than in
§ 13.

Into § 13, onto the handle clause that already names `nullptr`:

> `nullptr` is its null and `==` compares the address; a map key is an error.
> **A `nullptr` handle or `ptr` that C reads through aborts, naming the Heroes
> function that made the call.**

Eighteen words, and the word `aborts` is the merge that makes it cheap: it pulls
in § 6's *ends the program at once, saying why* for free, which is what kills C.

One rider, and it costs nothing: **the runtime must print the document's word.**
If the spec says *abort* and the program says *panic*, a reader who greps the one
document they were told to trust finds nothing, and some will conclude the
language has two failure concepts and try to write a program that distinguishes
them. There is no construct to do that, so they will invent one.

---

## experiment

Program 1 is the brief's, unchanged. I confirmed it is derivable from the
document as written before using it: `node_open` needs no `acquires` because no
`extern` consumes `Node`; `empty: Node @ nullptr` is legal by § 13 and § 5;
`empty` is read, so it is used.

Program 2 (the wrapper shape, written to test whether the blame name scales):

```
function value_of(n: Node) -> i64
    return node_value(p: n)

function main()
    root = node_open(v: 7)
    print(value_of(n: root))
    kid = node_child(p: root, i: 0)
    print(value_of(n: kid))
```

Program 3 (the upstream-defect shape, written to test whether the lost line
changes a conclusion):

```
    root = node_open(v: 0)
    print(f"root value {value_of(n: root)}")
```

Program 4, the one the proposed sentence would make me write, and which I did
**not** write under the current spec because nothing told me to:

```
    kid = node_child(p: root, i: 0)
    if kid == nullptr
        print("no child")
    else
        print(value_of(n: kid))
```

That guard is the whole delta. Under the current document I had no reason to
write it: § 13 gives me `==` on a handle and never says the unguarded read is a
failure site. Under the proposed sentence, `aborts` marks it as one, and the
language's habit is that a marked failure site gets guarded.

## hesitation_points

| # | where I guessed | cost of a wrong guess |
|---|---|---|
| 1 | **What happens when a null handle reaches C.** Unwritten. I guessed *abort, like an out-of-bounds index*, from the language's habit. On machine C that guess is wrong. | **Silent.** The program is accepted, compiles, runs, and dies with no evidence. The worst shape there is. |
| 2 | **Whether `node_child(...) -> Node` needs `acquires`.** § 13: *where any `extern` consumes a handle type every call handing one back says which it is*. I had to read every member of the group to know none consumes `Node`. | Compile error. **Loud, so I do not veto it**, but see below. |
| 3 | **`panic` versus `abort`.** The word in the error is not the word in the document. | Silent wrong model: a reader concludes the event is outside the language, or that two failure concepts exist. |
| 4 | **`called from node_value`.** Read it twice; the wording admits *the fault is inside node_value* and *the fault is deeper than node_value*. | Silent misdirection: I search my own source for a function I did not write. |
| 5 | **Whether output survives a stop.** § 11 promises what `print` writes, § 6 says *at once*. Unresolved. I guessed *survives*, which is right on A and wrong on B. | Silent: my print-debugging loop reports false negatives and I converge on the wrong region. |
| 6 | Small ones that all error loudly: `i: i32` taking the literal `0` (§ 2 context rule); named arguments optional here because no two parameters share a type (§ 9); `print` of an `i64` (§ 11). | Compile error, each. These are the language working. |

Five of six guesses in this exercise fail **silently**. That ratio is the
argument.

## argument

I wrote four programs. The blame name mattered less than I expected: in any
program with a wrapper, both A's and B's names are function-granular, and
neither locates the bad argument. What located it was A's surviving `7`. So the
ranking is A, then B, then C, and the gap from B to C dwarfs the gap from A to
B. C is a defect of the language: § 6 promises an abort says why, and C says
nothing; if this is not an abort, the document has no word for it at all, which
is worse. B breaks § 11's print promise. The cheap repair is two merges, and it
converts my largest guess into written text.

## prediction

Falsifiable, and checkable the next time the harness puts the spec in front of a
model.

1. **The guard rate.** Task: *"open a node, read its first child's value, and
   handle the case where there is no first child"*, given the spec and the
   `extern` block. Under the current document, **at most 20%** of first-try
   programs contain an explicit `== nullptr` comparison on a C-returned handle.
   With the proposed § 13 clause, **at least 50%**. Falsified if the delta is
   under 15 points.
2. **The wasted iterations.** Hand a model the source and the machine's output,
   ask for the repair, count actions before it names the correct line. Predict
   **A ≤ 1**, **B ≥ 2**, **C ≥ 3** on the wrapper program (Program 2), because B
   and C both push the model into the print-debugging loop that they themselves
   disable. Falsified if A shows no advantage over B.
3. **The wrong line.** On Program 3 (the upstream defect), predict that a
   majority of B and C repairs name the *dying* line rather than
   `node_open(v: 0)`, and that a majority of A repairs name `node_open`.
   Falsified if A and B name the same line at the same rate.
4. **The word.** While the runtime says `panic` and the document says `abort`,
   predict that **≥ 10%** of models asked *"what does the specification say
   happens here"* answer that it is unspecified. Checkable by grep on the
   answer.

## condition

- **I withdraw the objection to C**, and instead ask that the document *say* C
  out loud, if someone shows that catching the null requires a construct whose
  meaning is **not** determinable from the line plus the enclosing signature. I
  would take an honest `a nullptr reaching C is the program's own risk, and the
  program may die without a word` over a non-local guard. **Silence is the only
  outcome I will not accept**, because silence is what made five of my six
  guesses silent.
- **I withdraw the output clause** if a measurement shows that writing the
  buffered output before stopping can **duplicate or interleave** it.
  Duplicated output is worse than lost output: lost output is a known unknown,
  duplicated output is wrong data that reads as right.
- **I move from *name the Heroes caller* to *name both*** with no argument at
  all; I only prefer the Heroes caller under the constraint that exactly one may
  be printed. If a source location is on offer, take it first: it beat both
  names in every program I wrote.
- I would raise my objection to a veto if a proposed repair made the abort
  **catchable**, that is, if it introduced any construct letting a program
  observe or recover from this stop. § 6 says *no `T?` carries one*, and a
  catchable abort would make the meaning of a call depend on a handler written
  elsewhere. That is the non-local shape my veto exists for.

## veto

**Not cast.** The proposal introduces no construct. It makes the meaning of
`node_value(p: empty)` *more* local, not less: today that line's meaning is
determined by **which machine runs it**, which is the most non-local dependency
available, since it is not even in the file.

One pre-existing non-locality I found and am **not** vetoing, recorded so it is
not lost: whether a result handle needs `acquires` or `borrows` depends on
whether **any** `extern` anywhere in the program consumes that handle type
(§ 13). That is not determinable from the line plus the enclosing signature, and
I had to read the whole group to write hesitation point 2. It is out of scope
for this sitting, and it **errors loudly**, which is why it earns a note rather
than a veto. If a future panel widens it, I will want it in front of me.

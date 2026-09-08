# 020 — Four sites in fifty-five thousand lines

**The count M-closures-verdict was owed before it sat** (`docs/work/SCHEDULED.md`,
author instruction 2026-09-03). design.md Part 8 wart 1 says the language's
one-line helper functions *"exist mostly to be passed around"*, and Part 7 item 1
says closures *"will delete the handful of named one-line helper functions that
currently exist only to be passed around"*. Neither sentence had a number under
it. This is the number, measured 2026-09-08 over `selfhost/` (**190** modules,
**55,361** lines) and `examples/` (**118** files, **18,622** lines).

## The instrument, and the first one that was wrong

**`.func_ref` is the IR op for a function used as a value**, printed as
`funcref <name>` by `selfhost/ir/print.hero`. So the compiler itself says where a
function is a value, and no text search has to guess. `heroes build <module>
--dump-ir | grep funcref` is the ground truth — the dump comes after
monomorphisation and the ownership pass, so it is what the backend sees.

It cannot be the whole instrument, because **`--dump-ir` prints the module you
name and separate compilation means nothing else**, and a nested module does not
build standalone: `use` starts at the directory of the file you compile
(`spec/heroes-spec.md` § Files and layout), so `heroes build
examples/interpreter/lex/scan.hero` fails with `unknown_module` on its own
`use lex/token`. Of the 308 files, **150** dump and **158** cannot.

So a second instrument reads the text, and **the first version of it was wrong in
a way worth writing down**: it pooled function names across all 308 files and
then counted every identifier matching one. `at`, `after`, `env`, `done` and
`cell` are top-level functions somewhere in the corpus and local bindings
everywhere in it, so the census reported 217 functions passed as arguments, of
which the great majority were locals. **The repair is a rule of the language
rather than a better regex**: a local may not shadow a top-level function in the
same file. Run on this Mac, 2026-09-08:

```
$ heroes check shadow_test.hero        # `double = 3` under `function double`
error[shadowed_binding]: `double` is the name of the declaration at line 1,
which is in scope everywhere — pick another name
```

So **inside one file, an identifier equal to a top-level function declared in
that file IS that function**, and a cross-module value is written `mod.name`
where `mod` is that file's own `use` binding. Scoped that way, the text census
and the compiler were compared over the 150 modules where both can speak:
**78 sites against 78, zero missed and zero extra.** It disagreed by three before
one repair — `selfhost/library_source.hero` embeds the library's own source as
string literals, so `return ok(text)` appears inside a `"…"` and `text` is also
a function in that module. Blanking literals closed it. The validated instrument
then ran over all 308.

## What is there

| | selfhost/ | examples/ | both |
|---|---|---|---|
| top-level functions | 1478 | 860 | **2338** |
| of exactly one statement | 154 | 186 | **340** |
| sites where a function is a value | 17 | 83 | **100** |
| distinct functions passed by name | 6 | 27 | **33** |
| of those, one statement | 2 | 17 | **19** |
| files holding any function value | **3** | 18 | **21** of 308 |

**Wart 1's claim is false in the direction it is written and true in the other
one.** Of the **340** one-statement top-level functions, **19** are ever passed
by name — **5.6%**. The other 321 are ordinary small functions that are called.
Turned around, of the **33** functions that are ever values, **19** are one
statement — **58%** — so the one-liner is the majority *among function values*
and a rounding error *among one-liners*. The wart describes the population it
met and names the wrong one.

## The four sites

**66 of the 100 sites are inside `test` blocks**, where a function value is a
test double rather than a program's shape: `selfhost/cli/argv.hero` alone passes
`yes_dir` and `no_dir` — `return true` and `return false` — at 13 sites, all of
them asserts, injecting the directory probe that `parse` takes so the parser can
be tested without a filesystem.

In `selfhost/`, 13 of 17 sites are tests. **The self-hosted compiler passes a
function as a value at FOUR sites in production code, in 55,361 lines:**

```
selfhost/main.hero:92           argv.parse(given, dir_probe: process.is_directory)
selfhost/measure/pieces.hero:55 run_of(cs: cs, at: i, kind: is_letter)
selfhost/measure/pieces.hero:58 run_of(cs: cs, at: i, kind: is_digit)
selfhost/measure/pieces.hero:61 run_of(cs: cs, at: i, kind: is_other)
```

**Not one of the four would capture anything.** Three are character classes
handed to one scanner; the fourth is the real `is_directory` standing where the
tests put a stub. A closure deletes no declaration here: `is_letter` is named
because `run_of(kind: is_letter)` is what the line means, and an unnamed body
at that call site would say less.

On the other side of the ledger, **one teaching program holds a third of the
corpus's function values**: `examples/readings/main.hero`, 33 of 100.
`examples/interpreter/util/each.hero` holds 20 and
`examples/gallery/06-generics.hero` 5 — the files whose subject IS the function
value. Take the three out and the corpus that is not about higher-order functions
passes one at **42** sites.

## Appended the same day: wart 1's OWN definition, and a peer session's count

**A peer session reached this question first, in the turn that closed
M-discard-refusal, and its number and the table above disagree.** Its closing
words to the author, translated (CLAUDE.md §11): *"the defect closures should
cure — one-line helpers written only to be passed around — shows up in one file
of the whole tree. The compiler, fifty-five thousand lines, does not have a
single one."* That claim is in no record, so there was nothing to read forward
from; it is written here because it turned out to be measuring **a different
thing**, and the difference is the one wart 1's own sentence names.

The table above counts a one-statement function **passed by name at least
once** — 19 of them. Wart 1 says the helpers *"exist mostly to be passed
around"* and Part 7 item 1 says closures *"will delete the handful of named
one-line helper functions that currently exist ONLY to be passed around"*. That
is a narrower set: a one-statement function with **value sites and zero call
sites**. Measured 2026-09-08, same instrument, call sites counted as the name
followed by `(` either plain or after a `.`:

| | selfhost/ | examples/ | both |
|---|---|---|---|
| one-statement, only ever passed, never called | **0** | 21 | **21** in 7 files |
| one-statement, both passed and called | 1 | 2 | 3 |

**The peer session was right about the compiler and wrong about the count.**
`selfhost/` holds **zero** functions matching wart 1's sentence. Its one
one-statement function ever passed by name is `is_other`, and
`selfhost/measure/pieces.hero:147` calls it as an ordinary function too, so it
does not exist only to be passed around. The tree's 21 are in **seven** files,
not one — `examples/readings/main.hero` holds 5 of them, which is presumably the
file that was seen: `too_warm`, `blazing`, `plausible`, `tenths_of`, `add`. The
other six are `examples/gallery/06-generics.hero` (4),
`examples/interpreter/util/each.hero` (5),
`examples/interpreter/syn/parse.hero` (2), `examples/calculator/main.hero` (2),
`examples/calculator/whole.hero` (2, the same program written as one file) and
`examples/threads/main.hero` (1).

**So the subtraction test §1.7 asks for has an exact answer: 21 declarations,
every one of them in an example program, none in the compiler** — and four of
the seven files are the programs whose SUBJECT is the higher-order function.
**21 of 340** one-statement functions, **6.2%**.

**Corrected the same day by the spec-warden's seat, panel 119, and the
correction is written under the sentence rather than into it** (CLAUDE.md §14).
The paragraph above justifies `selfhost/` = 0 by saying
`selfhost/measure/pieces.hero:147` calls `is_other` as an ordinary function.
**Line 147 is `is_other`'s declaration, not a call of it.** Run: `grep -n
is_other selfhost/measure/pieces.hero` gives exactly three lines — `:61` the
value site, `:147` the declaration, `:177` `assert is_other("—")`. The only
ordinary call in the tree is **inside a `test` block**, so under the very split
this file draws forty lines earlier — 66 of 100 sites are test doubles rather
than a program's shape — the compiler's count is **1 and not 0**:

| counting | selfhost/ |
|---|---|
| every use, a `test` block's included | 0 |
| production uses only | **1** — `is_other` |

**The direction is unchanged and the peer session's claim was wrong by one in
the other direction too.** Its words were *"the compiler … does not have a
single one"*; the honest answer is one, and which one depends on whether an
`assert` counts as using a function. `is_letter` and `is_digit` are genuinely
both passed and called, and are **not** one-statement functions
(`pieces.hero:126-130` and `:132-135`), so they do not enter this set on either
reading. The 21 in `examples/` were spot-checked by that seat independently and
stand.

## What the clause costs, measured rather than estimated

design.md says closures are *"~150 lines and ~60 spec tokens"*, written
2026-08-04. The token half is now measurable: four candidate clauses, appended to
the spec and put through `heroes measure`, whose binding number is the `max` over
both vendored instruments. The spec stands at **3965**; the ceiling is 4096 and
the FFI floor mortgages 60 of it, so **71** tokens are free.

| clause | measured | delta | against the 71 |
|---|---|---|---|
| capture by copy, stated in full, with the FFI refusal | 4083 | **+118** | breaches by 47 |
| inline blocks (Part 7 item 12), stated in full | 4058 | **+93** | breaches by 22 |
| capture by copy, terse | 4033 | **+68** | fits, **3** to spare |
| capture-free unnamed function, terse | 4022 | **+57** | fits, 14 to spare |

**And the same 71 tokens have a rival claim, measured the same way.** The
llm-ergonomist's seat at panel 119, reading the spec and nothing else, reported
that what gated every one of its three tasks in every arm was not the absence
of closures: **the spec states `map`'s signature (`spec:121`) and no other**,
while naming `filter`, `fold`, `find`, `any` and `all` as functions written in
Heroes (`spec:188`). It had to invent `filter`'s type to write the task at all.
Checked against the real declarations in `selfhost/library_source.hero`, its
guess for `filter` was exactly right and `find`'s `-> A?` is the one no reader
can derive. Three drafts of the missing signatures, measured:

| clause | measured | delta | against the 71 |
|---|---|---|---|
| the five, each spelled out | 4051 | +86 | breaches by 15 |
| the five, compressed | 4034 | +69 | fits, 2 to spare |
| **the four missing, compressed** | 4028 | **+63** | **fits, 8 to spare** |

**So the spec's last 71 tokens buy the four missing signatures, or a stripped
closure clause, and not both**: +63 and +68 together are +131. Two seats reached
that conclusion from inputs that do not overlap — the warden priced six
candidate removals and found none that pays for +118, and the ergonomist never
saw the repository.

**The estimate was right for the sentence that does not say the rules and wrong
by a factor of two for the one that does.** The +118 draft is the one that states
capture-by-copy, what happens to a captured `@` cell, and that a capturing value
is refused at every `extern` position — which is exactly the distinction panel
013's ffi-pragmatist made a condition (`docs/panel/013-function-type-marker.md`:
*"a capturing closure is a record plus a pointer, so the type system will have to
distinguish capture-free at the boundary"*). A clause that fits leaves that
distinction to be found by the reader or written somewhere that is not the spec.

## The demand side: 18 signatures, and the one that would be silently wrong

Everything above counts what closures would **delete**. This counts what exists
**because** there is no capture — the workaround, which is the other half of
§1.7's test and which no previous note had looked for.

**The whole corpus holds 18 higher-order signatures in 7 files**, measured
2026-09-08 with `grep -rn --include='*.hero' "^function .*(function(" selfhost
examples`. Two are the compiler's: `selfhost/cli/argv.hero:73` takes
`dir_probe`, and `selfhost/measure/pieces.hero:92` takes `kind`. The other 16
are examples.

**Five of the 18 are the same function in one file, and they thread the same
context by hand.** `examples/interpreter/run/expr.hero` — `evaluated`,
`negated`, `inverted`, `joined`, `applied` — each carries
`nodes: tree.Tree`, `f: value.Frame` and
`call: (function(value.Request) -> value.Frame?)`, and passes all three on to
the next. That is the shape a closure exists to remove, and it is the largest
instance of it in the repository.

**And it is the case where capture by copy would be silently wrong**, which is
why it is written here rather than offered as an argument for the feature. Of
the three threaded values, two are constant for the whole evaluation — `nodes`
and `call` — and would capture correctly. **`f` is not.** `expr.hero:94-107`
reads:

```
first = evaluated(nodes, left, f, call)?
…
second = evaluated(nodes, right, first.frame, call)?
```

The frame **threads forward**: the right operand is evaluated against the frame
the left one left behind, because a call inside the left operand can print and
the frame comes back even from an expression (`expr.hero:50-54`, the record's
own comment). A closure capturing `f` by copy would freeze the frame as it was
before the left operand ran, and the second operand would evaluate against a
stale one — **a program that compiles and gives a wrong answer at exit 0**,
which is the failure class design.md §1.1 exists to prevent. So the honest
reading of the corpus's biggest context-threading site is: closures remove two
of its three threaded parameters and make the third a trap.

**CORRECTED THE SAME DAY BY THE COMPILER-ENGINEER'S SEAT, panel 119, and the
paragraph above is wrong.** `f` is a **plain immutable parameter** —
`function evaluated(nodes: tree.Tree, id: i64, f: value.Frame, call: …)`, with
no `@`, verified by `grep -c '@f:' examples/interpreter/run/expr.hero` = **0**.
So a captured copy of `f` can never diverge from the original, there is nothing
for a checker to compare, and **capture by copy does not create the class
described above**. The mistake the paragraph imagines — using `f` where
`first.frame` was meant — is a different binding being named, and it is
**writable today with no closures at all**. The claim was an inference dressed
as a measurement, which is CL-018's exact shape, and it went out to three seats
in a mid-sitting message before one of them ran it down.

**What the seat found in its place is worse, and it is the `@` cell.** A
`total: i64 @ 0` captured by copy freezes at the moment of capture; a later read
yields the stale value. **Every language a model has seen captures by
reference** — Python, JavaScript, Rust's `FnMut`, Swift, C# — which makes the
accumulating closure the textbook idiom, and Heroes would compile it and print
the wrong number. A *write* to a captured cell is refusable by a checker; a
*read* is not, because whether the frozen value was the one wanted is
undecidable. **And the only draft of the clause that states what happens to a
captured `@` cell is the +118 one**, which breaches the budget by 47: the rule
that would warn the reader is the rule that does not fit.

Two seats reached this class from inputs that do not overlap. The
llm-ergonomist, holding only the spec, wrote it as its silent case for Task 2 —
a captured `i: i64 @ 0` incremented inside the body to supply the index `map`
does not give — and its own words are that the closure compiles because the
counter is read inside it, so the unused-binding rule never fires.

**The clean case is real and it is small.** `examples/interpreter/util/each.hero:48`
is `joined(xs, shown: (function(A) -> str), sep: str)` — and `sep` is exactly
the separator panel 054 found a reader wanting `map` to capture
(`docs/panel/054-a-cost-with-no-exit.md`). `first_of(xs, fits, fallback: A)` on
line 39 is the same shape. Those two parameters, and `nodes` and `call` in the
five above, are the whole measured demand: **six parameters across seven
signatures.**

## Two things the record already knows and one it got wrong

**The demand for capture is on the record, measured, once.** Panel 054 priced
`repeat` and found the reader's actual want was `map`'s **capture**, refused
because CLAUDE.md §13 forbids Part 7 before the closure list compiles itself
(`docs/panel/054-a-cost-with-no-exit.md`); panel 037's own falsification test
found that *"with no lambda, `map` cannot capture the separator"* left two of
three tasks with no non-loop route at all
(`docs/panel/037-array-growth.md`). That is a real reader, in a real session,
wanting exactly the form this milestone rules on — and it is one witness, not a
class.

**The C boundary has moved since the condition was written, and the citation
`SCHEDULED.md` carries is stale.** That item says
`selfhost/check/ffi.hero:45` *"refuses `.function_ty` in every extern
position"*. It does not, since M-c-callbacks (2026-09-05): that file now says a
function type *"answers YES here and is still refused at two of its three
positions"*, and a Heroes function reaches a C callback **parameter** today.
`selfhost/emit/ctype.hero` still emits a function value as a bare C function
pointer *"because v1 has no closures"*, and that is what makes `qsort` and every
raylib callback expressible — so the condition is live and its wording is not.

*******************************************************************************

**Every command in this file was run on this Mac on 2026-09-08**, against
`./heroes` rebuilt from `seed/heroes.c` the same session (2.85 s, `/usr/bin/time
-p`). The census script is not in the tree: it is two instruments agreeing on a
number, and what is worth keeping is the number and the disagreement they had.

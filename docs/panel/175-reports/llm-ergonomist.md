# Panel 175 — llm-ergonomist report

**Input.** `docs/panel/175-briefs/llm-ergonomist.md` and `spec/heroes-spec.md`
(variant X), with Y applied as the brief's two-sentence diff to § 13. I read
both from the shared checkout, `/Users/joseph/Temp/heroes/heroes-lang/`, as the
convener instructed. I wrote this file in worktree `heroes-lane-076` because the
harness refused a write to the shared path. The harness also injected
`CLAUDE.md` and three `.claude/rules/` files into my context when I opened
files. I did not ask for them, I did not use them, and this verdict rests on
none of them. One of them quoted spec token counts, which my seat disregards on
purpose.

**Nothing was run.** This seat has no shell, and its discipline forbids the
compiler. Every "what happens" below comes from the document. Where I mention
C-library behaviour, it is marked *unrun* and is my recollection, so treat it as
a question.

## verdict

**object.** I agree with the direction of both changes. I object to two
wordings, and each costs first tries: *"another"* (first change) and *"is a
handle"* (second change). No veto.

## experiment

### Task 1: two ways to open a stream

The extern group is the same under X and Y. **Two records may not name one
tag** (§ 13: *"two records may not name one tag"*), so the type cannot carry the
difference between a pipe and a file. Both streams are one handle type:

```
extern "stdio.h"
    record Stream tag __sFILE
    function popen(command: cstr lent, mode: cstr lent) -> Stream acquires pclose
    function pclose(stream: Stream consumes) -> i64
    function fopen(path: cstr lent, mode: cstr lent) -> Stream acquires fclose
    function fclose(stream: Stream consumes) -> i64
```

Six sentences decided this group:
- No `link`, because libc needs none: *"`link` a library when the symbols need one"*.
- `lent`, because *"a lend reaches only one so declared"*.
- Result `i64`, because *"a result may be wider than C's"*.
- Named arguments at the call site, because two parameters share `cstr` (§ 9).
- `@s` in `finish`, because *"passing one the function borrowed is an error:
  mark the parameter `@`"*.
- `p: Stream @`, because *"only a declared @ name can be mutated"*.

**1F, the program I wrote first, under both X and Y.** The task names the two
closers. X's *"names the one that ends it, which the program owes it"* tells me
the popen stream is owed `pclose`. Nothing lets Heroes ask a handle which
closer it is owed, so `finish` is told:

```
function finish(@s: Stream, piped: bool) -> i64
    if piped
        return pclose(s)
    return fclose(s)

function main()
    p: Stream @ popen(command: "echo hi".cstr(), mode: "r".cstr())
    f: Stream @ fopen(path: "/etc/hosts".cstr(), mode: "r".cstr())
    if p == nullptr || f == nullptr
        print("a stream did not open")
        exit(code: 1)
    print(finish(@p, true))
    print(finish(@f, false))
```

1F is correct by both variants. When it runs, it prints the two closers' results
and exits normally. The document does not give C's values (I expect `0` twice,
*unrun*). Both handles leave the live set, and none is live when `main` returns.

**1L, the program the task's wording invites: one closer for "whatever it is
given".** I wrote it second, to answer "what happens":

```
function finish(@s: Stream) -> i64
    return fclose(s)
```

`main` is 1F's without the flag arguments.

- **Under X:** it compiles and runs. It prints two numbers and exits normally.
  No sentence says what happens when a handle owed `pclose` is ended by
  `fclose`. The live set loses `p` at the `fclose` call, so nothing is live when
  `main` returns. The deciding fact is an absence: *"which the program owes it"*
  states an obligation and no consequence. Under C, the child is not waited for
  (*unrun*). This is a **silently different program**.
- **Under Y:** it compiles, and it **aborts at `finish(@p)`**, saying why, before
  anything is printed. The deciding sentence is Y's *"…names the one that ends
  it, which the program owes it, and ending it with another aborts."* If
  `finish` called `pclose` instead, the abort moves to `finish(@f)`.

### Task 2: a pointer to give back

The header name is not given, so `"thing.h"` / `"thing"` are placeholders.

**Under X I wrote:**

```
extern "thing.h" link "thing"
    function make() -> ptr
    function release(p: ptr)

function main()
    a = make()
    b = make()
    release(a)
    release(b)
```

- **Spelling:** `ptr`. The table offers *"`ptr`: an opaque pointer"*. A handle is
  *"C's pointer to that type"*, where `tag` is *"what the header writes after the
  word struct"*, and `void *` names no struct. There was no hesitation.
- **Correct?** Yes. `release(a)` stands alone as a `()` line (§ 5).
- **What X says about releasing `a` twice:** nothing, directly. The only
  give-back-twice sentence is about handles: *"The live handles are a set, so
  giving one back twice aborts on its own."* § 3 (*"a function taking one without
  `@` may still change, or free, what C holds"*) and § 7 (*"free one and the next
  allocation may reuse that address"*) imply a C double free that the document
  does not promise to catch. I get there only by noticing the silence. Reading
  the nearest sentence by analogy gives the wrong answer: *it aborts*.

**Under Y I first drafted, then rejected:**

```
    record Thing tag void                        # Y says this "is a handle"
    function make() -> Thing acquires release
    function release(p: Thing consumes)
```

- **Why I rejected it:** Y's *"A pointer C hands out for the program to give
  back is a handle"* describes `make` exactly and pulled me toward this draft.
  But the document gives a handle no spelling except a struct tag, and
  `tag void` is a guess. It probably emits `struct void *`, which is a loud
  error, but the document does not say.
- **What I kept:** the X program unchanged, with `ptr`, because Y's own second
  clause speaks to that spelling.
- **Correct?** Yes.
- **What Y says about releasing `a` twice:** *"a `ptr` given back twice is a
  double free nothing catches."* The answer is direct and needs no inference.

### Task 3: one question

Asked "who ends the life of a `popen` stream in Heroes?", I point to the same
sentence under both variants, so the rule keeps one home.
- **X:** *"`acquires sqlite3_finalize` after a result or `@` out-parameter
  reaching a handle says the call begins that handle's life and names the one
  that ends it, which the program owes it."* Answer: the program, by calling
  `pclose`. X leaves open whether `pclose` is the *only* way.
- **Y:** the same sentence with *"and ending it with another aborts"* added.
  Answer: the program, with `pclose` and nothing else. The follow-up question
  ("and if it calls `fclose`?") is answered in the same sentence.

### S: the shape beside Task 1 (mine, not the brief's)

Task 1 is a consume that *closes*. The shape beside it is a consume that
*transfers*, and JSON is named in § 13's first line:

```
extern "cJSON.h" link "cjson"
    record Json tag cJSON
    function cJSON_CreateObject() -> Json acquires cJSON_Delete
    function cJSON_AddItemToObject(object: Json, string: cstr lent, item: Json consumes) -> i64
    function cJSON_Delete(item: Json consumes)

function main()
    root = cJSON_CreateObject()
    child = cJSON_CreateObject()
    _ = cJSON_AddItemToObject(object: root, string: "child".cstr(), item: child)
    cJSON_Delete(root)
```

- **Under X:** it runs correctly. `child` leaves the set at the transfer
  (*"`consumes` after a parameter says the call ends that value's life"*), and
  `root` leaves it at `Delete`. cJSON's own contract is that the object owns the
  item after this call (my recollection, *unrun*).
- **Under Y, read literally:** it aborts at the transfer, because
  `cJSON_AddItemToObject` is "another". A model then tries three repairs, and
  none gives a correct program that also fails loudly:
  1. Drop `consumes` on `item`. The program then aborts when `main` returns,
     because `child` is still live (*"like a handle nobody consumes"*).
  2. So add `cJSON_Delete(child)`. The set is now satisfied, and C frees `child`
     twice. Nothing in the document catches that.
  3. Write `acquires cJSON_AddItemToObject` on the creator instead. Now `root`
     aborts at `Delete`.

  One creator's handles legitimately end in two ways, so this program cannot be
  written correctly under Y.

## hesitation_points

| # | where | what a wrong guess produces | X | Y |
|---|---|---|---|---|
| H1 | distinct `Pipe`/`File` records on one tag | compile error | loud | loud |
| H2 | what a non-matching closer does (1L) | runs to completion in a C-wrong state; or the model believes a check exists | **silent** | loud (abort) |
| H3 | whether Y's abort fires before or after C's `fclose` | still an abort | — | loud, order unsaid |
| H4 | `p = popen(…)` then `finish(@p)` | compile error | loud | loud |
| H5 | a null handle from `popen`: is it in the live set? | not said | unknown | unknown |
| H6 | passing the closer as a value, `finish(@p, pclose)`: a function type has no `consumes` | not said | unknown | unknown |
| H7 | how to make `void *` a handle, as Y prescribes | an invented `tag` (probably loud), or back to `ptr` | none | **costs a first try** |
| H8 | double release of a `ptr`, guessed from the handle-set sentence | the model believes it aborts, and a later edit ships a double free | **silent** | answered |
| H9 | an ownership transfer via `consumes` (S) | abort, then repairs that end in a C double free | correct | **silent at the end of the repairs** |
| H10 | *"sqlite3_finalize"* is not in the fenced example (which pairs open with close) | the reader generalizes from an unpaired name | minor | minor |

## argument

On the brief's tasks, Y improves what a reader predicts, twice. Task 1's
one-closer `finish` runs silently under X, whose sentence names the owed closer
but gives no consequence. Under Y it aborts on its first run. Under X, Task 2's
double release is unsaid, next to a sentence saying handles abort on exactly
that. Y says nothing catches it. Two wordings cost first tries. Read literally,
*"ending it with another aborts"* also aborts an ownership transfer (a
`consumes` into a container). The nested-JSON program then has no correct
spelling, and its repairs end in a C double free. *"Is a handle"* prescribes a
spelling that `void *` cannot take. No veto: the abort is a runtime check, in
the same class as the handle set's.

## prediction

This becomes checkable at the first measured-model harness run on these tasks
after the milestone that carries panel 175's resolution. I cannot name that
milestone, because the roadmap is outside my input.

On Task 1, with at least 20 trials per variant:
- **Silent-error rate** (a program that runs to completion while ending a stream
  with a closer its `acquires` does not name): at least 25% under X, **0%**
  under Y.
- **First-try correct rate** (a `finish` that picks the owed closer): differs by
  **at most 10 points**. Y turns the mistake from silent to loud; it barely
  prevents it.

Either number outside its bound falsifies the prediction. If the gain is above
10 points, Y's sentence steers what models write, and I under-credited it.

## condition

- **Object becomes approve** when both of these hold:
  1. The first change is reworded so that a `consumes` of a function that no
     `acquires` names is not "another". For example: *"…and ending it with
     another that an `acquires` names aborts."* Alternatively, S, built by Y's
     implementation, runs to completion, and the prose says why.
  2. The second change either reads *"a pointer to a struct C hands out…"* or
     says how a `void *` becomes a handle. Alternatively, a harness run shows at
     most 5% of Task 2 trials under Y attempting a handle spelling that fails.
- **Object hardens:** X's Task 1 silent rate measures near 0. Then the first
  change buys nothing on first tries, and only its cost remains.
- **Object becomes veto:** the abort turns out to depend on anything besides the
  handle's acquiring declaration and the ending call, such as the flow between
  them. Then the line's meaning is not visible from the line.

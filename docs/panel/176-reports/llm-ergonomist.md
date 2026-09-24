# Panel 176, llm-ergonomist report

Written out verbatim by the coordinator on 2026-09-23 from the seat's final
message: the seat's write to its scratchpad was refused by the harness, so it
returned the report inline. Its inputs were the two copies outside the
repository named in its prompt.

**Summary (10 lines)**
1. **Verdicts:** V1 approve with conditions, V2 object (but keep its last clause and its list of alternative closers), V3 object, R1 approve, A2 approve if "they share" gets defined. No veto: none of them makes a line's meaning depend on anything outside the line and its signature.
2. **Task 1 (child into parent):** under X it's unsettled. "names the one that ends it" argues against `consumes` on `item`, and nothing says what happens if another call ends the handle.
3. The plausible X binding leaves `item` unmarked. That aborts loudly at the end of `main`, and the obvious repair (`cJSON_Delete(child)`) is a double free the ledger accepts. That's the worst finding.
4. V1 (`transfers`) and V3 (`consumes into object`) I wrote right first try. V2 needs two edits: a list entry on every creator and `consumes` on `item`.
5. **Task 2 (wrong closer):** under X and V3, nothing aborts and nothing says so. V1 implies the program is wrong but says nothing about what happens. Only V2 states the abort.
6. **Task 3 (two references):** under X, the second `obj_unref` aborts by "Giving one back twice", though the C program is correct. Dropping one `obj_unref` to fix it leaks with no signal. R1 (`retains`) is correct first try.
7. **Task 4 (two modules):** X says nothing, so a wrong `lent` in one module leaves C holding a dead string. A2 turns that into a compile error. But "the parameters they share" is ambiguous for two `printf` declarations, which A2 could refuse.
8. **Task 5 (`sqlite3_bind_text`):** both modes work under X and A2 with a lease. A2 only removes the two-module split, which was unsafe anyway. A `lent` declaration passed with `SQLITE_STATIC` is silent under both.
9. **X defect none of the variants fixes:** in the `consumes` sentence, "mark the parameter `@`" reads as the extern's own parameter. On a `void *` handle that becomes `void **`, which C converts silently, so `eat` would free the program's own cell. Also, "No word says C frees what it is handed" nearly answers task 6(c) verbatim, though it's about leases.
10. **Prediction:** task 1 first-try correct rate at most 60% under X and at least 90% under V1. It's wrong if X reaches 80% or V1 drops below 80%. The X half needs no compiler change, so the next harness run can check it; the V1 half needs the milestone that implements `transfers`, which my inputs don't name.

**Context disclosure:** besides the two files, my context also got `CLAUDE.md`, `MEMORY.md`, a git status snapshot (five commit subjects, two of them about panel 175, and one milestone name) and some unrelated tool instructions. I used none of them. Whether an abort comes before or after the C call is marked undetermined throughout, not answered from a commit subject.

---

## Inputs

I opened `brief.md` and `heroes-spec.md` and nothing else. What else reached my context, all unused, is listed in the summary.

The spec I was given has no sentence stating a locality rule by that name; the nearest is section 5, "Signatures are always explicit; inference is local only." I applied my seat's definition.

The section 13 sentences I cite, with short labels:
- **S-acq:** "`acquires` ... says the call begins that handle's life and names the one that ends it, which the program owes it."
- **S-twice:** "Giving one back twice aborts, unless C has since reused its address."
- **S-cons:** "`consumes` ... ends that value's life, so passing one the function borrowed is an error: mark the parameter `@` and the value does not survive the call."
- **S-left:** "a lease nobody ends, like a handle nobody consumes, aborts when `main` returns, saying how many."
- **S-lent:** "a parameter is taken to keep what it is handed unless declared `lent`, and a lend reaches only one so declared."
- **S-void:** "`tag void` is `void *`: a `void *` C hands out for the program to give back is declared as one."
- **S-tag:** "A parameter declared with it takes no other handle, and two records may not name one tag but `void`."
- **S-nofree:** "No word says C frees what it is handed; one that frees a lease kills the process, naming the leases live."
- **S-which:** "where any `extern` consumes a handle type every call handing one back says which it is."

## verdict

| Variant | Verdict against X |
|---|---|
| V1 | approve, on the conditions below |
| V2 | object to it as the way to express a hand-off; approve its "giving it to any other aborts" clause and its list of alternative closers, added to V1 |
| V3 | object |
| R1 | approve, on the conditions below |
| A2 | approve, provided "they share" is defined |

No veto:
- V2's abort depends on where a value came from at runtime, which X's S-twice already does.
- V3's `into object` names a sibling parameter, like `counted_by n`.
- A2 is a whole-program refusal and changes no line's meaning.

## experiment

### Task 1, under X (what I wrote first)
```
extern "cJSON.h" link "cjson"
    record Json tag cJSON
    function cJSON_CreateObject() -> Json acquires cJSON_Delete
    function cJSON_Delete(item: Json consumes)
    function cJSON_AddItemToObject(object: Json, name: cstr lent, item: Json consumes) -> i32

function main()
    parent = cJSON_CreateObject()
    child = cJSON_CreateObject()
    added = cJSON_AddItemToObject(object: parent, name: "child".cstr(), item: child)
    assert added == 1
    cJSON_Delete(parent)
```
**Correct by the document?** Undetermined. S-cons allows `consumes` on `item`, but S-acq says the creator names "the one" that ends it, which is `cJSON_Delete`. Nothing says whether another call may end it. S-left decided it for me: an unmarked child is a handle nobody consumes when `main` returns.

**The other binding a model plausibly writes** leaves `item` unmarked:
- It compiles, then aborts at the end of `main` (S-left). That part is loud.
- The obvious repair is `cJSON_Delete(child)`, which the ledger accepts.
- Placed before the parent's delete, that is a use-after-free; placed after, a double free. Either way the corruption is silent.

**If a later edit also deletes the child** (with my binding): S-twice makes it abort. But every cJSON node is the same size, so a later `cJSON_Create*` very likely reuses the child's address. Then "unless C has since reused its address" lets the delete through, and it frees the new node silently.

### Task 1, under V1, V2 and V3
- **V1:** `item: Json transfers`. Correct, decided by the V1 sentence word for word. Whether a later delete of the child aborts is not stated, because S-twice only covers "giving back".
- **V2:** the creator becomes `acquires cJSON_Delete | cJSON_AddItemToObject`, and `item: Json consumes` stays. Correct if both edits are made. A later delete aborts by S-twice. Leaving out `consumes` brings back the repair trap above.
- **V3:** `item: Json consumes into object`. Correct. A later delete aborts by S-twice.

**Cost on a bigger binding** (3 creators, 2 hand-off calls):
- V1 has 2 marks to get right.
- V2 has 6 list entries plus 2 `consumes`, and the same list is copied onto every creator.
- Each missing list entry is an abort only on the path that uses it.

### Task 2, the same code under every variant
```
extern "stdio.h"
    record File tag FILE
    function popen(command: cstr lent, mode: cstr lent) -> File acquires pclose
    function pclose(stream: File consumes) -> i32
    function fopen(path: cstr lent, mode: cstr lent) -> File acquires fclose
    function fclose(stream: File consumes) -> i32

function finish(@f: File)
    _ = fclose(f)

function main()
    p: File @ popen(command: "ls".cstr(), mode: "r".cstr())
    g: File @ fopen(path: "out.txt".cstr(), mode: "w".cstr())
    finish(@p)
    finish(@g)
```
What happens when it runs:
- **X:** nothing aborts, because only a double give-back is checked. The pipe gets `fclose`d and its child is never waited for. Silent.
- **V1:** the program breaks "with one of the calls its acquires named", but what happens is not stated.
- **V2:** it aborts, stated. Whether before or after C runs is not.
- **V3:** same as X.

Under X there may be a way out: two `tag void` records would make the types differ, but the document doesn't say whether `void *` is accepted against a header that says `FILE *`.

### Task 3
- **X:** S-which forces a mark on `obj_ref`, so I wrote `obj_ref(o: Obj) -> Obj acquires obj_unref`. The second `obj_unref` then aborts by S-twice, though the C program is correct. `borrows` doesn't fit either ("hands back one it keeps"), and the second unref still aborts. The repair that silences it, dropping one `obj_unref`, leaks with no signal.
- **R1:** `-> Obj retains obj_unref`. Correct first try.

### Task 4
- **X:** `keep(s: cstr lent)` in module `a` and `keep(s: cstr)` in module `b` both compile. If C really keeps the pointer, every call from `a` leaves it holding a dead string. Silent.
- **A2:** refused. Loud, though "refused" is my inference from the phrasing, not a stated word.
- **`printf` under X:** nothing is said about declaring a variadic C function, so the default is that both compile.
- **`printf` under A2:** ambiguous. `value` is unmarked in one declaration and `lent` in the other. If position is what makes a parameter "shared", a legitimate program is refused.

### Task 5
- **X, option (i):** one declaration where `text` keeps what it's handed:
  `sqlite3_bind_text(stmt: Stmt, index: i32, text: cstr, n: i32, destructor: ptr) -> i32`
  - STATIC: `destructor: nullptr`, with a lease ended after the statement is finalized.
  - TRANSIENT: `SQLITE_TRANSIENT`, with a lease that can be ended right after the bind.
  - Both modes work.
- **X, option (ii):** two modules, one with `lent` and one without. X allows it, but nothing ties `lent` to TRANSIENT, so a lend passed with STATIC compiles and is silent. S-tag also stops both modules from declaring the `Stmt` record.
- **A2:** refuses (ii). Option (i) keeps both modes usable.
- Under both, a `lent` declaration used with STATIC stays silent, and the mode where SQLite frees the text can't be written (S-nofree).
- Undetermined under both: whether `ptr` is accepted where the header uses function-pointer types for the destructor and `SQLITE_TRANSIENT`.

### Task 6
I wasn't given the budget ledger, so these are my answers without its predictions.
- **(a)** `record Thing tag void` and `make() -> Thing acquires release` (S-void). Writing `-> ptr acquires release` instead is a compile error, so that mistake is loud.
- **(b)** Two `tag void` records, `Block` for malloc/free and `SqlBlock` for sqlite3_malloc/sqlite3_free, each with its own `acquires` (S-tag). A `Block` reaching `sqlite3_free` is a compile error.
- **(c)** `eat(p: Thing consumes)` (S-cons). This is the one I trust least, because S-nofree seems to say no such mark exists.

## hesitation_points
1. **Task 1, X, marking `item`:** wrong guess fails loudly, then the repair it invites (`cJSON_Delete(child)`) is silent corruption. This is the worst point. V1 and V3 close it; V2 half-closes it.
2. **Task 1, V2, still needing `consumes`:** omitting it brings back point 1. V2 should say that a listed call must have a `consumes` parameter; the compiler can check that.
3. **Task 1, V1:** whether giving back a transferred handle aborts is not stated.
4. **V1's wording:** "one of the calls its acquires named" assumes a list that V1's grammar doesn't have.
5. **Task 2, X and V3:** nothing aborts and nothing says so. Silent.
6. **S-cons, "mark the parameter `@`":** this reads as the extern's own parameter, turning `void *` into `void **`. C converts that silently, so C would free the program's own cell. Silent corruption, and none of the variants fixes it.
7. **Task 6(c), S-nofree:** it pushes toward `eat(p: ptr)` with no mark, which is silent.
8. **R1, `retains` put on a getter C keeps:** the program then pays an unref it doesn't owe, which is silent corruption. `retains` put on a creator does something unstated.
9. **V3:** it doesn't say whether "ends both" carries down a tree of three, or what a borrowed parent's own release is.
10. **A2, "they share":** a wrong guess here is a loud refusal, never silent.

## argument
X settles no spelling as correct for three correct C programs: a hand-off (task 1, where `consumes` fights "names the one that ends it"), a second reference (task 3, where the second `obj_unref` aborts by "giving one back twice"), and a wrong closer it never sees (task 2). Each loud failure invites a silent repair: `cJSON_Delete(child)`, or a dropped `obj_unref`. V1 and R1 put the fact on the line where C acts, one word each; I wrote both first try. V2 alone settles task 2, but writes every hand-off into every creator. V3 equals V1 on task 1 and leaves trees undefined. A2 makes task 4 loud; its "share" is undefined for `printf`.

## prediction
With the spec as a fresh model's only context and at least 20 attempts per variant, the task 1 first-try correct rate is at most 60% under X and at least 90% under V1.
- **Counts as correct:** the program's debt for `item` ends at the hand-off, `main` returns with no ledger abort, and the program never also calls `cJSON_Delete(child)`.
- **Falsified if:** X reaches 80%, or V1 falls below 80%.
- **Checkable:** the X half at the next harness run carrying this task; the V1 half at the first run after the milestone that implements `transfers`, which my inputs don't name.

## condition
- **V1 goes to object** if 5% or more of attempts put `transfers` on a closer (`fclose`, `sqlite3_close`, `cJSON_Delete`) or on a hand-off that doesn't take ownership (`cJSON_AddItemReferenceToObject`).
- **V1's approval also needs** V2's abort clause, plus either V2's list of alternative closers or the plural dropped from V1's wording.
- **V2 goes to approve** if its first-try rate on the 3-creator, 2-hand-off cJSON task is within 5 points of V1's.
- **V3 goes to approve** if it says whether a release reaches the whole tree and what a borrowed parent's release is, and matches V1 on a nested-tree task.
- **R1 goes to object** if 10% or more of attempts put `retains` on a creator or on a getter.
- **A2 goes to object** if "share" stays undefined and the harness shows the `printf` case refused.

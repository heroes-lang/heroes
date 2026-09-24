<!-- Written out by the coordinator from the seat's final message, 2026-09-24:
the seat's own Write was refused by the tool ("Subagents should return findings
as text"). The one mechanical change: the transport escaped `->` and `<` as HTML
entities, and they are unescaped here. Nothing else is altered. -->

REPORT.md was not written. My Write to `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e7e3d0df-8db3-44f2-b628-af9bb1a6080e/scratchpad/177-llm-ergonomist/REPORT.md` was refused by the tool ("Subagents should return findings as text, not write report files"), so nothing is at that path and the full report is below. I read only `BRIEF.md` and `heroes-spec.md`. Nothing here was compiled or run: every "check passes" or "aborts" is what a reader of the text expects, and the allocator claims in Part B are unrun inferences.

# Panel 177, llm-ergonomist

## verdict

| Form | Verdict |
|---|---|
| Common text (`acquires a \| b`, `transfers`, `retains`) | **object** to the text as written. The direction is right, but it needs the amendment in *condition* |
| α (`transfers X on 0`) | **approve**, preferred |
| β (`-> i32 when 0`) | **approve**, second choice |
| I (the sentence as it stands) | **object** |
| II (a use after `consumes` is a compile error) | **approve**, once its wording settles three things (H14) |
| III (a call after the life ended aborts) | **approve** as the runtime half next to II. On its own it most likely misses the brief's own program |

**No veto.** You can read each form from its own line and signature (the common text, α, β) or from the function body (II). III is a runtime check that fails loudly. None of them takes its meaning from somewhere else.

## experiment

### A1 under α (my first attempt)
```
extern "json-c/json.h" link "json-c"
    record Json tag json_object
    function json_object_new_object() -> Json acquires json_object_put
    function json_object_new_int(i: i32) -> Json acquires json_object_put
    function json_object_object_add(obj: Json, key: cstr lent, val: Json transfers json_object_put on 0) -> i32
    function json_object_put(obj: Json consumes) -> i32

function main()
    parent = json_object_new_object()
    child = json_object_new_int(i: 42)
    rc = json_object_object_add(obj: parent, key: "answer".cstr(), val: child)
    if rc < 0
        _ = json_object_put(obj: child)
    _ = json_object_put(obj: parent)
```
I then changed `rc < 0` to `rc != 0`. My first choice came from the header's "a negative value"; the revision matches the binding's "any other result" (H2).

### A1 under β
Only the binding line changes, and `main` is the same (with `rc != 0`):
```
    function json_object_object_add(obj: Json, key: cstr lent, val: Json transfers json_object_put) -> i32 when 0
```
I hesitated no more under one variant than the other. The reason is that both spec examples *are* this task's binding, so A1 cannot tell α from β.

### A1: what I think the header means for `val` when the add fails
I concluded that **`val` is still the program's**, but the header never says so.
- **For that reading:** "thus `val` will be freed when `obj` is" says the transfer happens by `obj` holding `val`, and a failed add means `obj` holds nothing. "On error, a negative value is returned" reports an error, not a partial effect.
- **Against it:** "Since ownership transfers to `obj`" has no condition, so a reader could take it to mean ownership moves every time.

What I remember of json-c's source agrees with my reading. A reader who has only the header's words is guessing.

### A2
```
extern "json-c/json.h" link "json-c"
    record Json tag json_object
    function json_object_new_object() -> Json acquires json_object_put
    function json_object_get(obj: Json) -> Json retains json_object_put
    function json_object_put(obj: Json consumes) -> i32

function main()
    first = json_object_new_object()
    second = json_object_get(obj: first)
    _ = json_object_put(obj: second)
    _ = json_object_put(obj: first)
```
- **Under II:** this passes, because the two references have two names.
- **Under III:** this passes; the count goes 2, 1, 0.

### A3 (first attempt, then rewritten for II)
```
extern "openssl/x509.h" link "crypto"
    record X509 tag x509_st
    function X509_new() -> X509 acquires X509_free
    function X509_up_ref(a: X509 retains X509_free) -> i32
    function X509_free(a: X509 consumes)

function main()
    cert = X509_new()
    assert X509_up_ref(a: cert) == 1
    X509_free(a: cert)
    X509_free(a: cert)
```
**First revision:** I moved the call out of the `assert`, writing `rc = X509_up_ref(a: cert)` and then `assert rc == 1`. A side effect inside an assert is risky, and the spec does not say whether an ordinary build runs `assert`.

**Under II**, the last line is refused. That refuses correct code. The version II accepts:
```
    cert = X509_new()
    extra = cert
    rc = X509_up_ref(a: extra)
    assert rc == 1
    X509_free(a: extra)
    X509_free(a: cert)
```

### A4
```
extern "stdio.h"
    record File tag FILE
    function popen(command: cstr lent, mode: cstr lent) -> File acquires pclose
    function pclose(stream: File consumes) -> i32
    function fclose(stream: File consumes) -> i32

function main()
    f = popen(command: "echo hi".cstr(), mode: "r".cstr())
    _ = fclose(stream: f)
```
**Under the common text.** I expect `heroes check` to pass, because a `File` from popen and one from fopen are the same type. I expect the run to **abort at the `fclose` call, before C runs**, saying the handle owes `pclose`. That expectation rests on two things:
- § 13's "names the one that ends it";
- "any one of them ends the life", which implies that a function not in the list does not.

The common text never says this for `consumes`. It states the before-C abort only for `transfers`, and the shared-releaser rule only for `retains`. So three behaviours fit the text and it picks none:
1. It aborts before C runs.
2. It aborts when `main` returns, after `fclose` has already run.
3. Nothing checks it. The program runs silently and the child process is never reaped.

**The colleague's fix,** `function fclose(stream: File transfers pclose) -> i32`:
- `heroes check` passes.
- The before-C test passes too, because the handle was acquired with `pclose` among its releasers.
- The program then owes nothing, so there is no abort at `fclose` and none when `main` returns.
- C closes the stream, and **nothing ever calls `pclose`**, so the child process is never waited for.

**The colleague is wrong.** `fclose` has one parameter and hands the life into no other value. The binding says something false and every instrument accepts it. The lie becomes loud only in a program that also passes an fopen'd `File` to `fclose`: that call would now abort, because the handle was not acquired with `pclose`. The real fix is in the program: call `pclose`.

A second edit to the binding silences the check just as well: `acquires pclose | fclose`. No rule can catch that one.

### B1

| Sentence | `heroes check` | Running it |
|---|---|---|
| I | Passes. I expect this but can't be sure, since § 13 never mentions a use after `consumes` | No Heroes abort, because `b` is never given back twice. C writes into a freed node, or, if `c` got `b`'s address, cJSON is asked to add an object to itself. The program prints, exits 0, and the ledger is empty. **Silent** |
| II | **Error at line 7:** `b` is used after being handed to `cJSON_Delete` at line 5. No binary is produced | Never runs |
| III | Passes | Aborts at line 7, before C runs, **only if `c` did not get `b`'s old address**. A `cJSON_CreateObject` straight after a `cJSON_Delete` of a node the same size is the textbook case for an allocator handing back the block it just freed (inference, unrun). So it most likely behaves like I |

**Would I have written it?** Not as straight-line code. What I would plausibly write is an error path that falls through because I forgot a `return`:
```
    b = cJSON_CreateObject()
    first = cJSON_CreateObject()
    if cJSON_AddItemToObject(object: b, string: "a".cstr(), item: first) == 0
        print("add failed")
        cJSON_Delete(item: b)
    c = cJSON_CreateObject()
    _ = cJSON_AddItemToObject(object: b, string: "c".cstr(), item: c)
    cJSON_Delete(item: b)
```
- **Under I and III:** the only warning is an abort on the *failure* path, at the final delete (under III possibly at the add), and only when the first add actually fails. No test reaches that path.
- **Under II:** `heroes check` reports it, but only if II counts a consume that happens on *any* path. The text doesn't say (H14).

This is II's strongest case: it finds, at compile time, a mistake on a path that tests never take.

### B2
```
function main()
    node: Json @ nullptr
    turn: i64 @ 0
    while turn < 3
        node @ cJSON_CreateObject()
        leaf = cJSON_CreateObject()
        _ = cJSON_AddItemToObject(object: node, string: "leaf".cstr(), item: leaf)
        cJSON_Delete(item: node)
        turn @ turn + 1
```
- **Is it an error under II?** It depends on wording II doesn't give. Each turn reads `node` only after writing it again:
  - If a write revives the name, it is accepted. I expect this.
  - If "used again in the function" means any later execution, it is refused. The error is loud and the fix is easy: bind `node` inside the loop.
- **When does it abort under III?** Never. Each create begins a new life, even at an address used before, and each delete ends it.

**A buggy version** creates `node` once before the loop and deletes it inside:
- **Under II:** a flow-based reading reports an error at the add. A purely textual reading accepts it, because the add comes before the delete on the page.
- **Under III and I:** on turn 2, `leaf` most likely gets `node`'s freed block, so the add passes the check. The second `cJSON_Delete(item: node)` is then caught as a handle given back twice. That is loud, but only when the program runs.

## hesitation_points

For each point: what I guessed, and what a wrong guess produces. **Silent** means the program compiles and runs differently with nothing to say so.

- **H1 (A1). The header is silent on what a failed add does to `val`.** I guessed it stays the program's.
  - Leaving out both the condition and the release on failure: **silent** C leak on the failure path.
  - Writing the release but not the condition: an abort before C runs. Loud, but only on a path tests rarely reach.
- **H2 (A1). `rc < 0` or `rc != 0`.** Harmless: the wrong one fails loudly at `main`'s return, and only if C ever returns a positive value.
- **H3 (β). Where `when` sits relative to `owned`, `acquires` and `retains`.** Not needed here. A wrong order is a parse error, so loud.
- **H4 (α and β). Polarity: which result means success.** OpenSSL-style docs state failure first ("0 for failure").
  - α reads as one phrase, `transfers ... on 0`, with the event and its result side by side. β's `when 0` stands apart from the transfer and does not say which way it points.
  - A misreading applied consistently is **silent**: the runtime thinks the program owns `val` after a success, the program releases it, and C then uses it after it is freed.
  - A misreading applied inconsistently is loud when the program exits.
  - β also looks as if it applies to the whole call. A reader may assume it also governs `acquires` and `retains`, but its text says `transfers` only.
- **H5 (common text). `transfers` does not name which value receives the life.** I assumed `obj`, which the header supports. The cost: after `json_object_put(obj: parent)`, a use of `child` cannot abort under III, because the runtime cannot know `child`'s life ended with `parent`. **Silent** use after free.
- **H6 (A2). Should `retains` go on the result, the parameter, or both?** The text never says one per call. If it goes on both, the program is told it owes one release too many. The exit abort then invites a third `put`, which the ledger accepts and C performs as a double free. **Silent** corruption, brought on by a loud abort.
- **H7 (A2). `acquires` or `retains`?** The header says "taking ownership", which points at `acquires`. What `acquires` does at an address that is already live is not specified.
- **H8 (A3). A `retains` cannot be conditional**, and neither α nor β covers it. If the result is thrown away with `_ = X509_up_ref(...)`, a failed up-ref leaves C holding one reference while the program releases two. The ledger is satisfied and the double free is **silent**. I used `assert` instead.
- **H9 (A3 under II). Freeing one name twice is refused.** That is a false positive, but loud. The fix is an alias (`extra = cert`), which is also exactly how II is evaded.
- **H10 (A3, A4). `tag x509_st` or `tag X509`, and `FILE` has no portable struct tag.** Loud, because clang checks it.
- **H11 (A4). Does a `consumes` by a function outside the releasers abort, and when?** The spec says nothing. If nothing checks it, the program is **silent**.
- **H12 (A4). Two edits to the binding silence the mismatch:** `transfers pclose` and `acquires pclose | fclose`. Both are **silent**. Naming the receiver turns the first into a compile error.
- **H13 (B, sentence I). Does `heroes check` report a use after `consumes`?** The spec says nothing. I assumed it doesn't.
- **H14 (II). The wording has to settle three things:**
  - whether a consume on *any* path counts, or only one on every path;
  - whether writing the name again revives it;
  - whether it covers `transfers`. If it did, A1's correct release on the failure branch would be refused.
- **H15 (III). The caveat "unless C has since reused its address" describes the brief's own program**, in the likely case. **Silent.**
- **H16 (sentence I with `retains`).** "Giving one back twice aborts" contradicts the correct second release in A2 and A3. A reader has to translate it into "releasing more often than it owes".
- **H17 (II alone).** II replaces sentence I, so the spec would no longer say that a double release through an alias aborts.

## argument
The common text makes reference-counted C expressible. Without it A1 to A3 cannot be bound correctly. Its hole is that `transfers` names a releaser but not the value that receives the life. On a closer with one parameter (A4) it silences the releaser check, so the program runs without the release it owes: compiled, and quiet. Naming the receiver makes that lie a compile error and lets III end a transferred life. α keeps the condition beside the transfer it governs and reads with its polarity; β detaches it and reads as covering the whole call. Only II surely catches the brief's own program; III's reuse caveat is that program's likely case. Adopt both.

## prediction
- **P1 (α and β):**
  - Task: a transfer whose header states failure first ("returns 0 on failure, 1 on success"), not the spec's own example, with 20 fresh attempts per variant.
  - Prediction: β writes the wrong success value at least as often as α, with a gap of at most 3 in 20.
  - On A1 itself: no difference. Both variants write the condition in 18 of 20 attempts or more, because the spec example is the task.
- **P2 (III on the brief's program):**
  - Task: run the Part B program under III's implementation on macOS arm64, Linux glibc x86-64 and Windows.
  - Prediction: it **does not abort on at least one of the three**.
- **P3 (II's false positive):**
  - Task: A3 under II.
  - Prediction: at least 15 of 20 first attempts free one name twice and are refused at check.
  - Prediction: at least 90% of second attempts pass by using an alias.
- **P4 (the A4 silencers):**
  - Task: an A4-shaped task whose run aborts on a releaser mismatch, under the unamended common text.
  - Prediction: at least 1 attempt in 20 edits the binding (`transfers` or `| fclose`) instead of calling `pclose`.
  - Prediction: with the receiver named, 0 of 20 take the `transfers` route, because it becomes a compile error.

## condition
- **Common text.** I approve it as written only if two things hold: P4 counts 0 binding edits in 20, **and** the runtime can end a transferred life when its receiver is released. Otherwise I approve it once it says:
  - a transfer names the parameter that receives the life, and a call with no other handle parameter cannot transfer;
  - a `consumes` by a function outside the releasers aborts before C runs;
  - whether `retains` can be conditional.
- **α against β.** I switch to β if P1 shows β no worse on polarity **and** the harness finds a call with several transfers where α's attempts leave out a condition.
- **Part B.**
  - If P2 shows III aborting on all three platforms, III alone is enough, and I would drop II because of A3's false positive.
  - If II as built refuses the correct B2 loop, I object to II until its wording says a write revives the name.

## what one attempt can and cannot show
There is one reader, with one attempt per variant, and the attempt is contaminated in three ways:
- the brief shows which questions are being asked;
- the α and β examples are A1's own binding;
- I remember json-c's source.

My attempt **can** show:
- which sentences I had to guess at (H1, H11, H14);
- which plausible wrong programs compile and then run silently (H4, H5, H6, H8, H12, H15);
- that each form can be written from the text alone.

It **cannot** show a first-try rate, a silent-error rate, or any α/β difference. Those need P1 to P4 run on fresh readers.

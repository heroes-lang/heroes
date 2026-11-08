# Panel 237, the llm-ergonomist's report: the blind seat

How it was run, written by the coordinator: one fresh `claude -p` session on 2026-11-08, `claude-opus-5-5`, run by `blind237.py seat` (kept in the sitting's evidence) from `build/b237/seat/` in lane `p237`, with `--bare --restricted --safe-mode --strict-mcp-config`, `--tools Read,Write` and the same in `--allowedTools`, `--max-budget-usd 2.00`, git blind (`GIT_CEILING_DIRECTORIES` set to the folder's parent) and the key the runner loaded. The CLI reported 0.3635 USD, 10 turns, success. The folder held `brief.md`, the specification at `163912031` with the fence's two `sqlite3` lines replaced by one marker line, `variant-1.md` to `variant-4.md` (D, A, B, C, `blind/variants.tsv`) and `task-1.md` to `task-3.md`. **The session answered that it had no tool to write files**, as panel 235's did, and gave its report whole as its final text; what follows is that text, copied byte for byte from `run.json`'s `result`. Its `context` section names its directory's files and nothing of the project.

**Task 2, judged by the registered criterion** (`blind/README.md`): the repair (delete `stdint.h`, add `money_types.h`) applied to a copy of the task's folder builds and prints `125000` at exit 0 with no `stdint.h` left: **right**.

The folder's files, `shasum -a 256`:

```
9915df2b8c154750af7011e22f1c1c74dad9a4a02ac2ab6cf6f39ca49cc2801e  brief.md
d60bdae9b809a91e06834c5e0a45c73699bdafd2ce2a90fd20953313decd968a  heroes-spec.md
c24851b8ddeea20ecc75c91884da84a082364bc1cc2da0595da33f43432a3504  variant-1.md
953293fb2e7723f85ff5871a876375c2a6b727ba58b02608ca74ee3efe1a53a4  variant-2.md
03497ae5e1872c69ae68b15bee59a2e8bed8dd9c6975a71a355dbf3f2f7089bf  variant-3.md
0cf139b731b1cb8f65e8db3e27cfb785ba3e373a21d3d45078c5b9295279450a  variant-4.md
cbd1b992660948bfbe4e0abd0e536d782468d0306515763f5f497d944fa66d00  task-1.md
bcdbcbc1211f26de7066d98542b23c552cb9674817c532b5878d2a0533c15c19  task-2.md
46bd4fa76d2bf90cec8f7c4a3783ad7dc6260366fe52a1ddfa14ce562f81b865  task-3.md
```

---

I couldn't write `report.md`: the only tool I have is Read, which can't create files. As the brief allows, the whole report follows.

# report.md

## verdict

| variant | verdict |
|---|---|
| variant-1 | **approve** |
| variant-2 | **object** |
| variant-3 | **approve** |
| variant-4 | **object** |

None is vetoed. Each variant's meaning can be worked out from its own lines plus the enclosing `extern` group. The `acquires … | …` and `consumes … when 0` marks only name functions declared in that same group.

## experiment

### Task 1 (SQLite)

**Variant 1**
```
extern "sqlite3.h" link "sqlite3"
    constant SQLITE_OK: i64
    record Db tag sqlite3
    record Stmt tag sqlite3_stmt
    function sqlite3_open(path: cstr lent, @out: Db acquires sqlite3_close | sqlite3_close_v2) -> i64
    function sqlite3_close(db: Db consumes) -> i64 when 0
    function sqlite3_close_v2(db: Db consumes) -> i64
    function sqlite3_prepare_v2(db: Db, sql: cstr lent, n: i32, @stmt: Stmt acquires sqlite3_finalize, tail: ptr) -> i64
    function sqlite3_finalize(stmt: Stmt consumes) -> i64

function main()
    db: Db @= nullptr
    rc_open = sqlite3_open("notes.db".cstr(), @db)
    assert rc_open == SQLITE_OK
    stmt: Stmt @= nullptr
    rc_prep = sqlite3_prepare_v2(db, "SELECT 1".cstr(), -1, @stmt, nullptr)
    assert rc_prep == SQLITE_OK
    rc_close = sqlite3_close(db)
    print(rc_close)
    print(sqlite3_finalize(stmt))
    if rc_close != SQLITE_OK
        print(sqlite3_close(db))
```
- **Output:** `5`, `0`, `0`; exits 0.
- **Why it works:** `when 0` keeps `db` alive after the BUSY result, so the retry is legal.
- **If the retry is forgotten:** the unconsumed handle aborts when `main` returns, saying how many. It fails loudly.

**Variant 3:** the same program without the `sqlite3_close_v2` line, and `acquires sqlite3_close` alone. Same output, same behaviour.

**Variant 2:** the same `extern` lines as variant 3, but `sqlite3_close(db: Db consumes) -> i64` has no `when 0`.
- **Program as written (with the retry):** the compiler refuses it. The line `print(sqlite3_close(db))` reads `db` where every path has already ended it.
- **The only form that compiles:** the same program without the `if` block. It prints `5`, `0` and exits 0. The connection is still open, and the language believes it was given back. **This is a silent leak.** Under this variant no correct program for the task exists.
- **Copy workaround:** making `db2 = db` before the close and retrying on the copy aborts, because it hands C a copy made before the handle ended. That fails loudly.

**Variant 4**
```
extern "sqlite3.h" link "sqlite3"
    constant SQLITE_OK: i64
    record Db tag sqlite3
    record Stmt tag sqlite3_stmt
    function sqlite3_open(path: cstr lent, @out: Db acquires sqlite3_close_v2) -> i64
    function sqlite3_close_v2(db: Db consumes) -> i64
    function sqlite3_prepare_v2(db: Db, sql: cstr lent, n: i32, @stmt: Stmt acquires sqlite3_finalize, tail: ptr) -> i64
    function sqlite3_finalize(stmt: Stmt consumes) -> i64

function main()
    db: Db @= nullptr
    rc_open = sqlite3_open("notes.db".cstr(), @db)
    assert rc_open == SQLITE_OK
    stmt: Stmt @= nullptr
    rc_prep = sqlite3_prepare_v2(db, "SELECT 1".cstr(), -1, @stmt, nullptr)
    assert rc_prep == SQLITE_OK
    rc_close = sqlite3_close_v2(db)
    print(rc_close)
    print(sqlite3_finalize(stmt))
```
- **Output:** `0`, `0`. The connection really closes when the statement is finalized, so step 5 needs no second close. This is correct.
- **If the writer adds their own `sqlite3_close`:** that call aborts before C runs, because the `acquires` mark doesn't name it. Loud.

### Task 3 (store)

I carried each variant's shape over to `store_close`, which behaves like `sqlite3_close`: it can return BUSY and leave the store open.

**Variants 1 and 3** (variant 1's second close has no counterpart in this library, so the two give the same program)
```
extern "store.h"
    constant STORE_OK: i64
    record Store tag store
    record Cursor tag cursor
    function store_open(name: cstr lent, @out: Store acquires store_close) -> i64
    function cursor_open(s: Store, @out: Cursor acquires cursor_close) -> i64
    function cursor_close(c: Cursor consumes) -> i64
    function store_close(s: Store consumes) -> i64 when 0

function main()
    s: Store @= nullptr
    rc_open = store_open("notes".cstr(), @s)
    assert rc_open == STORE_OK
    c: Cursor @= nullptr
    rc_cur = cursor_open(s, @c)
    assert rc_cur == STORE_OK
    rc_close = store_close(s)
    print(rc_close)
    print(cursor_close(c))
    if rc_close != STORE_OK
        print(store_close(s))
```
Output: `5`, `0`, `0`; exits 0.

**Variants 2 and 4:** copying the example gives `function store_close(s: Store consumes) -> i64` with no `when 0`.
- **With the retry:** refused at compile time.
- **Without the retry:** prints `5`, `0`, exits 0, and the store stays open. **Silent leak.**
- **The difference:** variant 4 is correct for SQLite (`close_v2` really always closes), but as a model to copy it never shows `when`. That leads straight to this mistake on a library whose close can return BUSY.

### Task 2 (repair)

Change: delete `stdint.h` and add `money_types.h`. `main.hero` and `money.h` stay as they are.

`money_types.h`:
```
#include <stdint.h>

typedef int64_t money_t;
```

Why:
1. The local `stdint.h` sits in the program's own folder. The build searches that folder before the system folders, so it replaces the C library's `<stdint.h>` for every include in the build. That includes the include in the C the compiler writes, which the error refuses.
2. Renaming it removes the shadowing, and `money.h` then reads the real `<stdint.h>`.
3. `#include_next` only makes sense in a file that shadows a system header, so the renamed file uses a plain `#include <stdint.h>`.
4. Nothing includes the file, and `money_t` is unused, so deleting it outright would also work.
5. `money_cents(amount: 1250)` then builds and prints `125000`.

## choice_points

1. **Variant 1: which close to use first.**
   - My choice: `sqlite3_close`, then a retry when the result isn't 0.
   - Other choice: `sqlite3_close_v2` first is also correct, and prints `0 0`.
   - An unconditional second close after a successful first one stops loudly at run time (it hands C an ended handle).
2. **Retry condition.**
   - My choice: `rc != OK`.
   - Other choice: `== BUSY` behaves the same here. If another error code came back, the handle stays alive and the program aborts when `main` returns: stops loudly.
3. **Forgetting the retry.**
   - Variants 1 and 3: stops loudly (unconsumed handle when `main` returns).
   - Variants 2 and 4 (task 3, and task 1 under variant 2): runs silently wrong.
4. **Retrying under unconditional `consumes`.**
   - Through the same name: refused.
   - Through an earlier copy: stops loudly.
5. **`when 0` on `sqlite3_finalize` or `cursor_close`.**
   - My choice: left off. `finalize` destroys the statement even when it returns an error.
   - Other choice: adding it would, on an error result, report a false leak when `main` returns: stops loudly.
6. **`pzTail`.**
   - My choice: `tail: ptr`, passed as `nullptr`.
   - Other choice: `@tail: cstr` would hold an address into a lend that has already expired. Reading it would be silently wrong or crash.
7. **`db: Db` on `prepare`.**
   - My choice: plain, with no mark.
   - Other choice: `consumes` would make the later close refused or stop loudly. `lent` would make it a pointer to const, which clang would refuse.
8. **`nByte`.**
   - My choice: `i32` with `-1`.
   - Other choice: `i64` is refused, since a parameter must match C's width and C says `int`.
9. **Open failure.**
   - My choice: `assert`, which stops loudly.
   - Other choice: ignoring the result means an unused binding, which is refused.
10. **Variant 4: adding `sqlite3_close`.**
    - If the writer declares their own `sqlite3_close`, calling it stops loudly, because the `acquires` mark doesn't name it.
11. **Task 2: rename or delete.**
    - Either builds.
    - Renaming but keeping `#include_next` builds today because nothing includes the file. It could go wrong later if something includes it.

## argument

The danger is a handle whose close can fail while the language counts it as given back. Variants 1 and 3 put `when 0` on the close that can return BUSY. A forgotten retry then aborts when `main` returns, a retry compiles, and the correct program exists. Variant 2 marks `sqlite3_close` as always ending the handle, which SQLite contradicts. The correct program is refused, and the only program that compiles leaks silently with exit 0. Variant 4 is a true reading of `close_v2` and is safe for task 1. But it is the only example of a close a reader sees, and it has no `when`. Copied onto `store_close`, it reproduces variant 2's silent leak. Variant 1 is the best teacher: it shows both kinds of close side by side.

## prediction

Share of first-try programs that run, exit 0 and silently leave the connection or store open (task 1 / task 3):

| variant | task 1 | task 3 |
|---|---|---|
| variant-1 | ≤5% | ≤10% |
| variant-2 | ~40% | ~40% |
| variant-3 | ≤5% | ≤10% |
| variant-4 | ~15% | ~45% |

For variant 4, task 1 failures come from writers who use `sqlite3_close` with an unconditional `consumes` copied from the example.

## condition

- If many first tries show variant 2 or variant 4 at ≤10% on task 3, within 5 points of variants 1 and 3, I would change those verdicts to approve.
- If variant 1 or variant 3 goes above 15% on either task, I would object to it.
- If variant 4's task-3 rate matches variant 3's, I would approve variant 4.

## context

No instruction, rule, contract, memory or document other than this directory's files reached my context. The files are `brief.md`, `heroes-spec.md`, `variant-1.md` to `variant-4.md`, and `task-1.md` to `task-3.md`. The only other input was the agent setup: the working-directory path and the date.
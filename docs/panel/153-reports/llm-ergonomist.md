# Panel 153 — report of the llm-ergonomist

*Written out by the coordinator from the seat's final message, verbatim, 2026-09-15. The seat read the brief's § 13 and nothing else; it opened no file and wrote none.*

**verdict**: approve. X as the fence, Q as the pointer-read addition; object to P; no veto (P and Q are both local by the brief's definition).

## experiment

### Conventions I had to guess, identical under every variant, so they move no count

§ 13 shows no `main`, no declaration, no `print`, no `if`. I wrote `function main() -> ()` from the callback type `function() -> ()`, `name: Type = expr` for a binding, `print(x)`, indentation blocks from the grammar's INDENT/DEDENT, `".cstr()"` on a literal from `s.cstr()`, and a call-site `@x` for a cell a call writes or empties, from `end_lease(@x)`. A wrong guess here is a parse error, never a different program.

### Task 1, the omission

Shared lines, every attempt, both versions:
```
extern "sqlite3.h" link "sqlite3"
    constant SQLITE_OK: i64
    record Db tag sqlite3
```
Shared `main` unless noted:
```
function main() -> ()
    db: Db = nullptr
    rc: i64 = sqlite3_open(":memory:".cstr(), @db)
    if rc != SQLITE_OK
        print("open failed")
        return
    sqlite3_close(@db)        # X readers; Y readers write sqlite3_close(db)
```

**Version X, ten readings**

X1, copies the fence, prose confirms it:
```
    function sqlite3_open(path: cstr, @out: Db acquires sqlite3_close) -> i64
    function sqlite3_close(db: Db consumes) -> i64
```
Both marks.

X2, grammar first, then fence; `acquires ident` fits the CParam slot. Same two lines; `main` drops the rc check. Both marks.

X3, reads "mark the parameter `@`" as the declaration:
```
    function sqlite3_open(path: cstr, @out: Db acquires sqlite3_close) -> i64
    function sqlite3_close(@db: Db consumes) -> i64
```
Both marks; `@db: Db` is `sqlite3 **` against the header's `sqlite3 *`, refused. Loud.

X4, "after a result" read first:
```
    function sqlite3_open(path: cstr, @out: Db) -> i64 acquires sqlite3_close
    function sqlite3_close(db: Db consumes) -> i64
```
Both marks present, `acquires` on an `i64` that reaches no handle, refused (I predict). Loud. Not counted as an omission.

X5, minimalist; "Unmarked pointers are never freed" read as "marks are about freeing, and sqlite3_close frees":
```
    function sqlite3_open(path: cstr, @out: Db) -> i64
    function sqlite3_close(db: Db) -> i64
```
OMITS both. Compiles. Nothing is tracked. **Silent.**

X6, keeps `consumes`, drops `acquires` ("close already says it ends the life"):
```
    function sqlite3_open(path: cstr, @out: Db) -> i64
    function sqlite3_close(db: Db consumes) -> i64
```
OMITS `acquires`. "Where any `extern` consumes a handle type every call handing one back says which it is": refused. Loud.

X7, copies the fence, checks the ownership paragraph sentence by sentence, keeps both. Both marks.

X8, copies the fence; reads the `@` sentence as call-site and writes `sqlite3_close(@db)`. Both marks.

X9, copies the fence with `-> i32` on both results ("may be wider", so either). Both marks.

X10, copies the fence, both marks; `main` forgets `sqlite3_close`. Aborts at `main` return, "saying how many". Loud.

**X: 2 of 10 omit** (X5 both marks, silent; X6 `acquires`, loud). X3 and X4 carry both marks in the wrong place and are refused.

**Version Y, ten readings**

Y1, copies the fence:
```
    function sqlite3_open(path: cstr, @out: Db) -> i64
    function sqlite3_close(db: Db) -> i64
```
OMITS both. Compiles; untracked. Silent.

Y2, copies; reads `acquires sqlite3_finalize` as a statement-handle feature not needed here. OMITS both. Silent.

Y3, "a handle nobody consumes aborts" read, so close is marked:
```
    function sqlite3_open(path: cstr, @out: Db) -> i64
    function sqlite3_close(db: Db consumes) -> i64
```
OMITS `acquires`. Refused by the "every call handing one back says which" rule. Loud.

Y4, reads the whole ownership paragraph, adds both:
```
    function sqlite3_open(path: cstr, @out: Db acquires sqlite3_close) -> i64
    function sqlite3_close(db: Db consumes) -> i64
```
Both marks.

Y5, copies; "Unmarked pointers are never freed" taken as permission. OMITS both. Silent.

Y6, copies; `main` forgets `sqlite3_close`. OMITS both. Compiles, exits 0, leaks. **Silent: the class X exists to close.**

Y7, adds `acquires sqlite3_close` from "names the one that ends it", leaves close bare:
```
    function sqlite3_open(path: cstr, @out: Db acquires sqlite3_close) -> i64
    function sqlite3_close(db: Db) -> i64
```
OMITS `consumes`. Owed and never consumed, aborts at `main` return (or refused, if the named ender must itself consume). Loud.

Y8, copies with `-> i32`. OMITS both. Silent.

Y9, adds both, declaration-site `@`: `sqlite3_close(@db: Db consumes)`. Both marks; refused (`sqlite3 **`). Loud.

Y10, copies; checks SQLITE_OK. OMITS both. Silent.

**Y: 8 of 10 omit** (six omit both and compile silently; Y3 and Y7 omit one and are loud).

Which is current: **Y**. The section quoted verbatim in the brief carries Y's two function lines byte for byte. Second tell: the prose's own example is `acquires sqlite3_finalize`, a function the fence never declares, so prose and fence were not written against each other.

Note for the panel: the section's rules do not require either mark unconditionally. The one hard requirement is conditional: once any `extern` `consumes` a handle type, every call handing one back must say `acquires` or `borrows`. Y's six silent programs are legal; the fence teaches the legal weaker binding. X's payoff is that once the fence teaches `consumes`, a missing `acquires` is refused and a forgotten close aborts.

### Task 2, reading through a pointer

Under P (marker proposed: `handle`, after the tag; additive, so `record Db tag sqlite3` is untouched; none-needed is not an option, because `record FileStat tag stat partial` with fields is already a struct by value and the two shapes would be indistinguishable):
```
extern "netdb.h"
    record AI tag addrinfo handle partial
        ai_family: i32
        ai_next: AI
    function getaddrinfo(node: cstr, service: cstr, hints: AI, @res: AI acquires freeaddrinfo) -> i32
    function freeaddrinfo(res: AI consumes)

function main() -> ()
    res: AI = nullptr
    rc: i32 = getaddrinfo("127.0.0.1".cstr(), "0".cstr(), nullptr, @res)
    if rc != 0
        print("getaddrinfo failed")
        return
    ai: AI = res
    while ai != nullptr
        print(ai.ai_family)
        ai = ai.ai_next
    freeaddrinfo(@res)
```

Under Q:
```
extern "netdb.h"
    record AI tag addrinfo
    record AddrInfo tag addrinfo partial
        ai_family: i32
        ai_next: AI
    function getaddrinfo(node: cstr, service: cstr, hints: AI, @res: AI acquires freeaddrinfo) -> i32
    function freeaddrinfo(res: AI consumes)

function main() -> ()
    res: AI = nullptr
    rc: i32 = getaddrinfo("127.0.0.1".cstr(), "0".cstr(), nullptr, @res)
    if rc != 0
        print("getaddrinfo failed")
        return
    ai: AI = res
    while ai != nullptr
        info: AddrInfo = ai.read()
        print(info.ai_family)
        ai = info.ai_next
    freeaddrinfo(@res)
```

Words to hold. P: one keyword (`handle`), three rules (fields read through the pointer; never written; a null read aborts), and a three-way classification: tagged with no fields is a handle, tagged with fields is a struct, tagged with fields plus `handle` is a handle again; `partial` stacks on top. Q: one method (`read()`), two rules (a tag names at most one handle and one struct; a null read aborts); the two existing kinds of record stand untouched.

Where a reader goes wrong.
P: line `record AI tag addrinfo handle partial`. Drop `handle` and AI is a struct by value, so line `@res: AI` is `struct addrinfo *` against `**`, refused, and `ai_next: AI` is a struct holding itself, refused. Loud. Drop `partial` (P does not say whether the struct rule carries over) and clang either finds six unnamed fields and refuses, or accepts; I cannot tell. Line `print(ai.ai_next.ai_family)` on the last node: run-time abort, loud, but only on the input that reaches it, and nothing on the line says a pointer is crossed.
Q: line `print(ai.ai_family)` (forgot `read()`), AI has no fields, refused. Loud. Fields written on `AI` with `AddrInfo` left bare: `@res: AI` refused. Loud. Line `record AddrInfo tag addrinfo`: a reader holding "two records may not name one tag" who has not met Q's carve-out gives up and reaches for `ptr`, a dead end, loud; so Q must amend that sentence in place, not append. Line `info: AddrInfo = ai.read()` then `info.ai_family = 0` expecting C to see it: nothing happens, **silent**, but only on a task that writes through, which neither P nor Q supports.
Both: line `freeaddrinfo(@ai)` inside the loop is a C double free. Neither text says whether a handle read from a field is "one the function borrowed". If it is, the line is refused; if not, the count sees one acquire and N consumes, and the section only promises that a double consume can hide a missing one. This is the one place a plausible program could compile and corrupt.

Locality: both local by the brief's definition. `ai.ai_family` and `ai.read()` are each decided by the line plus the declaration of `AI`, which the line names through `ai`'s type. What differs is what sits on the line: under Q the pointer crossing and its abort site are spelt `read()`; under P `ai.ai_family` is spelt exactly like `buf.st_size`, which cannot abort. No veto.

Adopt **Q**. In the section's own terms: "a group's `record` is the header's struct" and "one with a `tag` and no fields is a handle" both stand unchanged; P adds a third kind, needs a marker for it, and stacks the marker with `partial`. Q puts the pointer crossing on the line. And a `getaddrinfo` that passes real `hints` needs the by-value struct and the handle of the same tag at once; Q already permits that pair, P alone collides with "two records may not name one tag" and would need Q's carve-out anyway.

Failure predicted. P: 4 in 10 models will drop `handle` or `partial` on the record line and be refused; 0 in 10 silent on this task. Q: 3 in 10 models will write `ai.ai_family` on the handle and be refused; 0 in 10 silent on this task; on a write-through task 3 in 10 would assign to the `read()` copy and be silently ignored, against 0 under P's "never written".

### Task 3, `lstat`

```
extern "sys/stat.h"
    record FileStat tag stat partial
        st_size: i64
    function lstat(path: cstr, @buf: FileStat) -> i32

function main() -> ()
    buf: FileStat
    rc: i32 = lstat("/etc/hosts".cstr(), @buf)
    if rc != 0
        print("lstat failed")
        return
    print(buf.st_size)
```
§ 13 as written lets me: `record FileStat tag stat partial` is the section's own example, `@buf: FileStat` is "a C out-parameter is an `@` parameter" against `struct stat *`, and `st_size` at `i64` is `off_t`'s width and sign on this platform or clang refuses it. P and Q add nothing here: no pointer is read through, the caller owns the struct. P adds one wrong turn, a reader who has just learnt `handle` writes `record FileStat tag stat handle partial` and `@buf` becomes `struct stat **`, refused. Loud.

## hesitation_points

1. "`consumes` ... mark the parameter `@` and the value does not survive the call." Declaration-site (`@db: Db consumes`) or call-site (`sqlite3_close(@db)`)? Fence X leaves the declaration bare, so call-site; but the word is "parameter". Wrong guess: refused (`sqlite3 **`) or refused (bare argument to a consumer). Loud both ways; the adopted fence should still show the call.
2. Whether `acquires` must name a function that itself carries `consumes` (Y7): compile error or abort at `main` return. Loud either way; the section does not say which.
3. Whether the marks are ever required (X5, Y1 to Y10). They are not, except conditionally. Silent.
4. `nullptr` as a `cstr` argument (`service`): § 13 gives `nullptr` to handles only. I passed `"0".cstr()` to dodge it. Wrong guess: type error, loud.
5. `partial` on P's handle-with-fields: not stated. Loud if required, unknown if forbidden.
6. A handle read from a field or from a `read()` copy passed to a `consumes` parameter: not stated under either. The double-free line.
7. `const struct addrinfo *hints` against a bare `AI`: constness is outside the width-and-sign rule. Loud if refused.
8. Uninitialised `buf: FileStat` before `@buf`: no declaration form in § 13. Parse error if wrong.

## argument

Y is the current fence and it teaches the weaker binding: eight of ten readers copy it, six of those compile a program that tracks nothing, and a forgotten `sqlite3_close` exits 0. X flips the ratio: two of ten deviate, and the one silent deviant is the reader who strips marks the fence shows. Between P and Q, both are local and both fail loudly on the read-only task; Q keeps the section's two kinds of record intact, puts the pointer crossing on the line as `read()`, and already permits the handle-plus-struct pair that `hints` will demand. Adopt X and Q, and both texts must say a field-read handle is borrowed.

## prediction

(1) Harness sqlite ":memory:" task, spec prompt with fence Y versus fence X: bindings carrying both `acquires` and `consumes` at most 3 of 10 under Y, at least 8 of 10 under X. Instrument: count the two words in each sample's `extern` group. (2) Same samples with the `sqlite3_close` call deleted: under Y every unmarked-binding sample compiles and exits 0; under X every sample aborts at `main` return. Instrument: exit status. (3) getaddrinfo task: refused on first try at least 3 of 10 under Q (bare `ai.ai_family`) and at least 4 of 10 under P (the record line); programs that compile and print anything other than `2` once: 0 of 10 under either. Instrument: compile, run, diff stdout against `2`.

## condition

Under Y, if 7 or more of 10 harness samples carry both marks, the prose suffices and I withdraw the objection to Y. If X yields 3 or more of 10 refusals from the `@db` versus `db` call, the fence must also show the call; that adds a requirement and does not flip the verdict. If on a write-through task Q produces 2 or more of 10 programs that assign to a `read()` copy and compile, and the panel will not add P's "never written" sentence or make the copy unassignable, I move to P.

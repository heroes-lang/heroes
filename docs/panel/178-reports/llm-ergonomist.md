<!-- Written out by the coordinator from the seat's final message, 2026-09-24: the
harness refused the seat's write to REPORT.md. One mechanical change: this
comment. The text below is the seat's, unchanged. -->

# Panel 178, llm-ergonomist

The harness refused my write to REPORT.md ("Subagents should return findings as text"), so that file does not exist. This message is the full report, for the coordinator to copy into the record.

## Verdict (one row)

| Taught | I (nothing) | II (`rest: zero` + claim + `to_fixed`) | III (`rest: zero` + `to_fixed`) | IV (`[x; n]`) | Veto |
|---|---|---|---|---|---|
| **III**, with IV a close second. III removes every hand-counted literal and the byte loop, and every guess it leaves fails loudly except the one it states itself (an unnamed field is zero). II's claim asks the wrong question and is the only variant where a plausible false line compiles. | T1 ~45%, T2 ~45%, T3 ~45%. Guessed: `partial` to cut 1280 zeros to 256, every element count, the element write through a field | T1 ~40%, T2 ~30%, T3 ~45%. Guessed: where `zero` goes, whether `to_fixed` is fallible, whether to claim `zero` on the mutex | T1 ~60%, T2 ~50%, T3 ~45%. Guessed: `rest: zero` with no named field, `to_fixed().must()` | T1 ~70%, T2 ~60%, T3 ~45%. Guessed: nothing new; IV's second sentence confirms the element write | none: every form is local |

These percentages are my belief that the program compiles as written, on first attempt, **unrun**: no harness and no compiler were used. About 45% is the shared ceiling set by four guesses every variant leaves open (C1 to C4 below). The char-sign guess dominates them.

**Prediction.** Give Variant II and the three tasks in one prompt to 20 fresh readers. At least 7 write `zero` on `pthread_mutex_t`, a claim that is false for this Darwin-layout header, and each of those programs compiles. At most 2 never call `pthread_mutex_init`, and that count differs from Variant III's by at most 1.

**Condition.** I switch to II if the harness shows II's count of mutexes built from zeros without `pthread_mutex_init` at least 3 in 20 below III's, while at most 3 in 20 write the false claim. I switch to IV if, on an in-parameter struct task with at least 5 fields and one required non-zero field the header does not comment, III's silent forgotten-field rate beats IV's wrong-value rate by at least 2 in 20.

---

## verdict

**approve Variant III.** I object to Variant II. Variant I is the worst on first try. Variant IV is acceptable and is complementary to III. No veto.

## experiment

I wrote all twelve programs as a first-time reader would, from the spec and the brief alone. `# GUESS` marks each line I guessed. The tail of `main` in task 3, from `rc_init` down, is the same in every variant and is shown once.

### Variant I (no text added)

**Task 1.** Declaring all five fields would mean writing 1280 zeros. I used `partial` so that only `sysname` has to be built.
```
extern "sys/utsname.h"
    record Utsname tag utsname partial          # GUESS: partial, to write 256 zeros instead of 1280
        sysname: u8[256]                        # GUESS C1: char is u8
    function uname(@name: Utsname) -> i64

function main()
    u: Utsname @ Utsname(sysname: [             # GUESS I-2: a partial record may be built
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        # 16 such rows, 256 elements (elided in this report, written out in the attempt)
    ])
    rc = uname(@u)
    if rc != 0
        print(f"uname failed: {rc}")
        exit(code: 1)
    print(u.sysname.validated_bytes().must())
```
It prints `Darwin` on this Mac.

**Task 2.**
```
extern "app.h"
    record SockaddrUn tag sockaddr_un
        sun_len: u8
        sun_family: u8
        sun_path: u8[104]                       # GUESS C1
    function app_connect(@addr: SockaddrUn) -> i64   # GUESS C2: a const pointer as @

function main()
    path = "/tmp/app.sock"
    assert path.len() < 104
    addr: SockaddrUn @ SockaddrUn(sun_len: 0, sun_family: 1, sun_path: [
        0, 0, 0, 0, 0, 0, 0, 0
        # 13 such rows, 104 elements
    ])
    for i in range(from: 0, to: path.len())
        addr.sun_path[i] @ path[i]              # GUESS I-3: an element of a field inside a cell
    fd = app_connect(@addr)
    if fd == -1
        print("connect failed")
        exit(code: 1)
    print(f"connected: {fd}")
```
It prints `connected: N` if something is listening on the socket, and `connect failed` otherwise.

**Task 3.**
```
extern "pthread.h"
    record pthread_mutex_t                      # GUESS C3: the typedef's own name, no tag
        __sig: i64                              # GUESS C4: a leading __ lexes as an ident
        __opaque: u8[56]                        # GUESS C1
    function pthread_mutex_init(@m: pthread_mutex_t, attr: ptr) -> i64   # GUESS C5: ptr for a const T * that may be NULL
    function pthread_mutex_lock(@m: pthread_mutex_t) -> i64
    function pthread_mutex_unlock(@m: pthread_mutex_t) -> i64

function main()
    m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: [
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0
    ])
    rc_init = pthread_mutex_init(@m, nullptr)
    if rc_init != 0
        print(f"init failed: {rc_init}")
        exit(code: 1)
    rc_lock = pthread_mutex_lock(@m)
    if rc_lock != 0
        print(f"lock failed: {rc_lock}")
        exit(code: 1)
    print("locked")
    rc_unlock = pthread_mutex_unlock(@m)
    if rc_unlock != 0
        print(f"unlock failed: {rc_unlock}")
        exit(code: 1)
    print("unlocked")
```
It prints `locked` and then `unlocked`.

### Variant II

**Task 1.**
```
extern "sys/utsname.h"
    record Utsname zero tag utsname             # GUESS II-1: "after its name" read literally
        sysname: u8[256]
        nodename: u8[256]
        release: u8[256]
        version: u8[256]
        machine: u8[256]
    function uname(@name: Utsname) -> i64

function main()
    u: Utsname @ Utsname(rest: zero)            # GUESS II-4: naming no field at all is allowed
    # the rest as in Variant I
```
The claim is true here: an all-zero `utsname` is five empty strings.

**Task 2.**
```
    record SockaddrUn zero tag sockaddr_un      # GUESS II-1
    ...
    addr: SockaddrUn @ SockaddrUn(sun_family: 1, sun_path: "/tmp/app.sock".to_fixed().must(), rest: zero)   # GUESS II-3: "fails" means T?
```
The claim is true here too. `sun_len` is left at zero, which is a domain choice I did not have to make.

**Task 3.** No claim. My first draft was `m: pthread_mutex_t @ pthread_mutex_t(rest: zero)`. Then the claim rule stopped me, and I asked whether all zeros is a valid mutex "on every platform". On Darwin it is not: `__sig` must carry the initializer's signature. So I did not write `zero` on the record, and I fell back to Variant I's 56 hand-written zeros. The tempting program I rejected is this one, and it compiles:
```
    record pthread_mutex_t zero                 # a FALSE claim that compiles
    ...
    m: pthread_mutex_t @ pthread_mutex_t(rest: zero)
```

### Variant III

**Task 1.** `record Utsname tag utsname` with all five fields, then `u: Utsname @ Utsname(rest: zero)`. Guessed: II-4.

**Task 2.** `record SockaddrUn tag sockaddr_un`, then `addr: SockaddrUn @ SockaddrUn(sun_family: 1, sun_path: "/tmp/app.sock".to_fixed().must(), rest: zero)`. Guessed: II-3. There is no loop and no length assert, because `to_fixed` fails loudly when the path and its terminating zero do not fit.

**Task 3.** `m: pthread_mutex_t @ pthread_mutex_t(rest: zero)`, then the same `pthread_mutex_init` tail. I wrote it without hesitating: the zeros are storage for `init` to fill.

### Variant IV

**Task 1.** All five fields, then `u: Utsname @ Utsname(sysname: [0; 256], nodename: [0; 256], release: [0; 256], version: [0; 256], machine: [0; 256])`.

**Task 2.** `addr: SockaddrUn @ SockaddrUn(sun_len: 0, sun_family: 1, sun_path: [0; 104])`, followed by `assert path.len() < 104` and the Variant I loop. IV's own example confirms `addr.sun_path[i] @ path[i]`.

**Task 3.** `m: pthread_mutex_t @ pthread_mutex_t(__sig: 0, __opaque: [0; 56])`, then the same tail.

### Task 3, answered explicitly

**No variant's program ever builds the mutex from zeros rather than through `pthread_mutex_init`.** In all four the cell has to hold a value before `@m` can be lent, so it starts zero-filled: by literal (I and II), by `rest: zero` (III) or by `[0; 56]` (IV). `pthread_mutex_init` then runs before any lock. Under II I refused to write the `zero` claim on `pthread_mutex_t`, and that refusal is what cost me the 56 hand-written zeros.

## hesitation_points

LOUD means a wrong guess is a compile error or an abort. SILENT means a wrong guess produces a different program that still compiles.

**Shared by every variant**

| id | where | wrong guess gives |
|---|---|---|
| C1 | `char` as `u8` or `i8`. "The header's own width and sign" meets a `char` whose sign belongs to the platform. `u8` is the only choice that makes `s[i]` and "bytes" fit. | LOUD (the field check) |
| C2 | `const struct sockaddr_un *` as `@addr`. The spec names `@` only for out-parameters, and nothing else points at a whole record. | LOUD |
| C3 | `record pthread_mutex_t`: lowercase, the typedef's name, no `tag` | LOUD (clang) |
| C4 | `__sig` and `__opaque` as identifiers, when `_` is special | LOUD |
| C5 | `attr: ptr` given `nullptr` for `const pthread_mutexattr_t *`, with no `lent` | LOUD |
| C6 | `@m` is copy-in/copy-out. If a temporary is what reaches C, the mutex moves between lock and unlock. | SILENT under contention, invisible single-threaded. No variant touches it. |
| C7 | NEWLINE right after `[` and inside argument lists | LOUD |

**Variant-specific**

| id | variant | where | wrong guess gives |
|---|---|---|---|
| I-1 | I | 256, 104 and 56 elements by hand. This is the dominant first-try failure. | LOUD |
| I-2 | I | building a `partial` record: the spec never says what the undeclared bytes hold | SILENT class for an in-parameter; harmless here because C overwrites the whole struct |
| I-3 | I | `addr.sun_path[i] @ ...`: the grammar allows it, the prose does not say so | LOUD |
| I-4 | I | the Rust reflex `[0; 256]` | LOUD (parse error) |
| II-1 | II | where `zero` goes: straight after the name, or after `tag`/`partial` | LOUD |
| **II-2** | II | **whether to claim `zero` on the mutex**. The compiler cannot check the claim. It is primed by two true claims in tasks 1 and 2, and a missing claim produces an error whose obvious fix is to add the word. | **SILENT: a false claim compiles.** The program stays correct only because `init` is still called. |
| II-3 | II, III | whether `to_fixed()` is fallible, so `.must()` | LOUD either way |
| II-4 | II, III | `rest: zero` with no field named | LOUD |
| III-1 | II, III | a field the reader forgets becomes zero instead of a missing-field error. For example, dropping `sun_family: 1` gives AF_UNSPEC. | SILENT (then -1 at run time) |
| III-2 | II, III | a C struct with a real field called `rest` | unknown: the spec does not say |
| IV-1 | IV | a path of exactly 104 bytes leaves no terminator in the loop form. I guarded it with an assert. | SILENT edge case, not reached here |

## argument

Variant I fails first try on counting, not on meaning: 256, 104 and 56 hand-written elements, plus the Rust reflex `[0; n]`. III removes all three with two forms a reader understands from one sentence. The only silent guess it adds is one it states: an unnamed field is zero. II's claim asks the wrong question. Task 3 needs zeroed storage for `pthread_mutex_init`, not a zero mutex. The honest reader is sent back to 56 hand zeros; the hasty one writes a false `zero` that compiles. Readers who believe zeros make a mutex will claim it anyway, so the gate stops nobody. IV is a close, complementary second. Nothing here is non-local.

## prediction

1. **Primary (Variant II, task 3, 20 fresh readers, all three tasks in one prompt):** at least 7 of 20 write `zero` on `pthread_mutex_t`, and all of those compile. At most 2 of 20 never call `pthread_mutex_init`, and that count differs from Variant III's by at most 1.
2. **Secondary:** the first-try compile rate on task 1 under Variant I is at most half the rate under III or IV. The failures are element-count errors and `[0; 256]` parse errors.

## condition

- **To II:** II's count of zero-built mutexes with no `init` is at least 3 in 20 below III's, with at most 3 in 20 false claims.
- **To IV:** on an in-parameter struct task with at least 5 fields and one required non-zero field the header does not comment, III's silent forgotten-field rate exceeds IV's wrong-value rate by at least 2 in 20.
- **Toward a veto:** only if `to_fixed()` took its length from anywhere other than the position's declared type, or if `rest` could silently bind a real C field of that name.

I read only `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/07a257f7-6606-4944-b558-0be3c97d5c11/scratchpad/178-llm-ergonomist/BRIEF.md` and `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/07a257f7-6606-4944-b558-0be3c97d5c11/scratchpad/178-llm-ergonomist/heroes-spec.md`. The CLAUDE.md in my context played no part in the verdict.

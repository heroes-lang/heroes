# Panel 153 — report of the ffi-pragmatist

**Verdict.** Route A: **object**. Route B: **adopt**, on one condition: the copy-out built-in is not named `read`. **No veto on either**: neither route moves one byte of how a Heroes value crosses the C boundary, and both keep clang's per-field verification. Q2: **adopt** the corrected fence; the defect's "nothing" is measured true.

**Section I stand on.** design.md §1.11 (*FFI ergonomics rank alongside comprehension*), §1.12 (*must not segfault; any C library must be bindable*), §4.8 (`@` is copy-in/copy-out and *you can see on the line that this argument will change*), §4.19 line 2162 (*the handle — a fieldless record in a group*) and line 2289 (*for awkward struct-passing write a thin C shim*). **design.md does not cover dereference**: `grep -c -i dereferenc docs/design/design.md` is **0**; the document's only answer to a struct C hands back by pointer is the shim, which §1.12 itself calls *§1.11 failing at the one thing it exists to do*.

**Method.** Copy at `/private/tmp/claude-501/-Users-joseph-Temp-heroes-lang/8d7432e4-9b44-486f-80db-fda4fdc56efe/scratchpad/seat-ffi`; seed built in 3.32 s real. Every C below compiled through `work/cc14.sh`, the fourteen flags of `selfhost/cli/flags.hero::flags()` word for word. `heroes build` defaults to `-O0`, `heroes run` to `-O2` (`selfhost/cli/verbs.hero:31,56`).

## Program 1 — `getaddrinfo` and the walk

**Route A shape** (`work/p1a-route-a.c`), clang **exit 0**, run prints `2`, **exit 0**:

```c
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

int main(void) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *res = NULL;
    int rc = getaddrinfo("127.0.0.1", NULL, &hints, &res);
    if (rc != 0) {
        printf("rc=%d\n", rc);
        return 1;
    }
    for (struct addrinfo *ai = res; ai != NULL; ai = ai->ai_next)
        printf("%d\n", ai->ai_family);
    freeaddrinfo(res);
    return 0;
}
```

**Route B shape** (`work/p1b-route-b.c`), clang **exit 0**, run prints `2`, **exit 0**:

```c
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

int main(void) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *res = NULL;
    int rc = getaddrinfo("127.0.0.1", NULL, &hints, &res);
    if (rc != 0) {
        printf("rc=%d\n", rc);
        return 1;
    }
    struct addrinfo *ai = res;
    while (ai != NULL) {
        struct addrinfo a = *ai;   /* ai.read() — 48 bytes copied per node */
        printf("%d\n", a.ai_family);
        ai = a.ai_next;            /* the pointer field of the COPY, a handle */
    }
    freeaddrinfo(res);
    return 0;
}
```

**The Heroes each route would read**, pushed through today's compiler. Route A (`work/ra-getaddrinfo.hero`, with `through` standing in for the word): **exit 1**, `error[expected_extern_signature]: expected a function, a constant or a record, found a name (through)` — a new surface form. Route B (`work/rb-getaddrinfo.hero`): **exit 1**, `error[unknown_function]: no function named read`; with `read` removed the same file is `error[duplicate_tag]: AddrInfo and AI both name the C type addrinfo` (`work/h-duptag.hero`, exit 1). **Route A writes the field list twice**: in `ra-getaddrinfo.hero`, `ai_family: i32` appears once under `AddrInfo` (for `hints`, by value) and once under `AI` (for the walk), because `hints` still needs a by-value record. Route B writes it once.

**Route B's precondition already holds**: a fielded record with a handle-typed field builds and runs today — `work/h-ptrfield.hero` (`ai_addr: SockAddr` against `struct sockaddr *`) **exit 0**, prints `2` and `true`; the emitted check is `_Static_assert(_Generic(&((struct addrinfo *)0)->ai_addr, sockaddr * *: 1, default: 0) …)`.

**The swap Route B's widened rule must not let through** (`work/p5-swap.c`): clang **exit 1, 2 errors**, `passing 'struct addrinfo' to parameter of incompatible type 'struct addrinfo *'` and the reverse. A handle beside a fielded record is not panel 145's two-handles mutant; C refuses it unaided.

## Program 2 — `hints` by pointer

`work/p2-hints.c`, clang **exit 0**; run: `rc a=0 b=0 c=0`, `C wrote through const: no`, `sizeof(struct addrinfo) = 48 bytes copied back`, **exit 0**:

```c
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

int main(void) {
    struct addrinfo hints_cell;                  /* the caller's `hints: AddrInfo @ ...` cell */
    memset(&hints_cell, 0, sizeof hints_cell);
    hints_cell.ai_family = AF_UNSPEC;
    hints_cell.ai_socktype = SOCK_STREAM;

    struct addrinfo hints_local = hints_cell;    /* copy in */
    struct addrinfo before = hints_local;

    struct addrinfo *res = NULL;
    struct addrinfo *p = &hints_local;           /* (a) */
    int rc_a = getaddrinfo("127.0.0.1", NULL, p, &res);
    if (rc_a == 0) freeaddrinfo(res);
    res = NULL;
    int rc_b = getaddrinfo("127.0.0.1", NULL, (void *)&hints_local, &res);   /* (b) */
    if (rc_b == 0) freeaddrinfo(res);
    res = NULL;
    int rc_c = getaddrinfo("127.0.0.1", NULL, &hints_local, &res);           /* (c) */
    if (rc_c == 0) freeaddrinfo(res);

    hints_cell = hints_local;                    /* copy out, always (§4.8) */

    printf("rc a=%d b=%d c=%d\n", rc_a, rc_b, rc_c);
    printf("C wrote through const: %s\n", memcmp(&before, &hints_local, sizeof before) == 0 ? "no" : "yes");
    printf("sizeof(struct addrinfo) = %zu bytes copied back\n", sizeof(struct addrinfo));
    return 0;
}
```

**Finding: `@hints: AddrInfo` against `const struct addrinfo *` builds and runs TODAY**, under neither route: `work/h-hints.hero` (a `partial` record of four `i32`, `@hints`, `res: @res`) **exit 0**, prints `0` then `1`, run exit 0. The emitted call is `getaddrinfo(hero_cstr_nonnull(t8), t9, &h0_hints, (void *)&h1_res)` and the probe types the parameter `struct addrinfo * a2`; adding `const` is implicit, so flag 14 is silent. Three costs: the cell must be declared with `@` and the call site says `@hints`, which §4.8 defines as *this argument will change* when the header promises it will not; the copy-back is a **48-byte** memcpy of unchanged bytes (144 for `struct stat`); and when C **keeps** the pointer the copy-in local is dead after the call. `work/p2-retain.c` (a callee storing `const struct addrinfo *`, then `main` reading it after a scribbled frame): clang **exit 0** under the fourteen flags, prints **`2139062143`** (0x7f7f7f7f), exit 0, no diagnostic; under `-fsanitize=address,undefined` it is `AddressSanitizer: stack-use-after-return`, exit 141 through the pipe. The header spells the harmless and the hazardous case identically; `const struct` occurs **22** times in the preprocessed `<netdb.h>` unit, **27** in `<curl/curl.h>`, **1** in `<sqlite3.h>`, **2** in `<sys/stat.h>` (`clang -E | grep -c`). Neither route names retention; it is panel 124's lease shape for structs and belongs on the queue, not in this ruling.

## Program 3 — `lstat` and `struct stat`

`work/p3-lstat.c`, clang **exit 0**; run: `size=544`, `mode=33188`, `nlink=1`, `sizeof(struct stat) = 144`, **exit 0**:

```c
#include <stdio.h>
#include <sys/stat.h>

int main(void) {
    struct stat st;                          /* the `@` cell, a fielded record */
    int rc = lstat("/etc/hosts", &st);       /* @st: FileStat -> &st */
    if (rc != 0) {
        printf("rc=%d\n", rc);
        return 1;
    }
    printf("size=%lld\n", (long long)st.st_size);
    printf("mode=%u\n", (unsigned)st.st_mode);
    printf("nlink=%u\n", (unsigned)st.st_nlink);
    printf("sizeof(struct stat) = %zu\n", sizeof(struct stat));
    return 0;
}
```

**Neither route is needed**: `work/h-lstat.hero` (`record FileStat tag stat partial` with `st_mode: u16`, `st_nlink: u16`, `st_size: i64`; `@buf: FileStat`) builds **exit 0** and prints `0`, `true`, `1`. Verification holds: `work/h-lstat-wrong.hero` with `st_size: i32` is **exit 1**, `error[ffi_field_type]: FileStat.st_size is not i32 in sys/stat.h`. The only ergonomic cost is that the author must construct a dummy `FileStat(...)` to have a cell for an OUT parameter.

## Program 4 — the null read

`work/p4-null.c`, clang exit 0 at `-O0` and `-O2`; run **exit 139** (SIGSEGV) at both levels, and the same with the pointer `volatile`:

```c
#include <stdio.h>
#include <netdb.h>

int main(void) {
    struct addrinfo *ai = NULL;
    printf("%d\n", ai->ai_family);
    return 0;
}
```

So under either route the emitter must write, before each read through a handle, the shape `runtime/parts/str.c:328` already uses for a `cstr`: `if (h == NULL) hero_panic("read through nullptr")` then the access. Bench `work/p4-bench.c`, 1,000,000 reads, best of 50 in-process (the process-level `real` of the first run per level was 0.46 s against 0.06 s user and was discarded as a waiting run):

| level | unguarded | guarded | guarded + 48-byte copy-out (Route B) |
|---|---|---|---|
| `-O0` (build default) | 0.882 ns/read | 1.074 ns/read (+0.192) | 4.429 ns/read |
| `-O2` (run default) | 0.735 ns/read | 0.760 ns/read (+0.025) | 0.763 ns/read |

## The five shipped bindings, before and after

Before, built in the copy with `HEROES_RUNTIME=<copy>/runtime`: `examples/sqlite/main.hero` exit 0 (emitted C 515 lines, sha256 `4bbe4090…`), `examples/ledger/main.hero` exit 0 (19,134 lines, `ab9f3eec…`), `examples/curl/main.hero` exit 0 (1,016 lines, `624d6df7…`). The handles emit as `sqlite3 *`, `sqlite3_stmt *`, `CURL *`. **After is unrun**: neither route exists to build against. What I measured instead: each tag appears **once** per file (`grep -oE "record … tag …" | uniq -c`), so Route B's widened `one_tag_one_type` never fires on them; the five `record` lines are each immediately followed by another declaration, so no field block exists for Route A's `is_handle_parts` to reclassify; and `read` occurs **0** times as an identifier in the three files (16 hits: 12 in comments, 4 inside string literals). That the emitted C is byte-identical after is therefore an inference from the two rule texts, not a diff.

**But Route B's name breaks shipped programs.** `work/h-builtin-shadow.hero` defining `function len` is **exit 1**, `error[builtin_name_taken]: len is a built-in of the language, so the name is taken everywhere`. `function read(` is defined **9** times across `examples`, `selfhost`, `tests/golden`, five of them `read(text: str)` in `examples/assembler`, `spreadsheet`, `markdown`, `maze`, `todo`. Names with **0** definitions in the same search: `load`, `deref`, `pointee`, `copy`, `fetch`, `struct_of`.

## Q2 — the fence, compiled and run against the SDK's `sqlite3.h`

| program | build | run exit | stderr |
|---|---|---|---|
| corrected fence + harness `main` with close (`work/q2-corrected-close.hero`) | 0 | 0, prints `0` | empty |
| corrected fence, open and no close | 0 | **134**, prints `0` | `panic: 1 C handle(s) never given back — every call marked acquires owes one marked consumes …` |
| **current fence copied**, open and no close (`work/q2-current-noclose.hero`) | 0 | **0**, prints `0` | **empty** |
| half copied, `consumes` only | **1** | — | `error[unmarked_handle_producer]: sqlite3_open hands back a Db and does not say whether the program owes it` |

The defect's claim is measured: the un-marked copy compiles, runs, leaks and says nothing. The corrected fence costs the reader nothing at the C boundary (same `sqlite3_open(…, &h0_db)` call) and a half copy is refused at compile. Its token price is the brief's +7 vendored, which I did not re-measure on a frozen tree.

## Argument

Both routes emit C that clang accepts under the fourteen flags and both keep `heroes-ffi-field` verification, so neither touches the ABI and I hold no veto. Route A is the longer binding: a struct both read by pointer and passed by value carries its field list twice, and it spends a new keyword on what Route B does with the record the tree already writes (`Broken tag tm partial`). Route B's cost is one copy of C's own struct and a widened tag rule whose mutant clang already refuses; its one real defect is the name `read`, which `builtin_name_taken` would turn into five refused shipped programs. The `hints` half needs no route: `@` already passes the address, at the price of a mark that lies about mutation.

## Prediction

Under Route B, with a built-in named from the zero-collision set, `work/rb-getaddrinfo.hero` (with `a = ai.<name>()`) builds with **no shim**, prints `0` then `2`, exits 0, and its emitted unit contains `_Static_assert(… ((struct addrinfo *)0)->ai_family …)` and one `hero_panic` before the copy. Score it with `HEROES_RUNTIME=<tree>/runtime ./heroes run work/rb-getaddrinfo.hero; echo $?` and `./heroes build work/rb-getaddrinfo.hero --emit-c -o rb.c && grep -c "struct addrinfo \*)0)->" rb.c`. Falsified if the run needs a `.c` file or exits non-zero on this Mac.

## Condition

Route A becomes acceptable to this seat if a measured binding exists that Route B cannot write without a shim and Route A can: I searched `netdb.h`, `sys/stat.h`, `time.h` shapes (`getaddrinfo`, `lstat`, `localtime`) and found none. Route B loses my adoption if the built-in keeps the name `read`, or if the null guard is left out of `<name>()` — program 4's exit 139 is what §1.12 forbids. Unrun: the after-state builds of the five bindings; any Linux or Windows number; the spec's token count after Q2.
# Panel 178 — shared brief

**HEAD `57679005`, the base of lane `lane-panel-178` in
`~/Temp/heroes-lane-panel-178`, `git status` clean there, read by the
coordinator on 2026-09-24 before the briefs went out.** The trunk is NOT frozen:
another session is landing M-agreed-retention on it tonight, so this sitting
runs in its own worktree and every seat works in its own copy of that HEAD.
Every number below was produced by a command run while this brief was being
written, and the command is named beside it. The probes named here are in this
directory.

**The author's words that bind this sitting**, given in conversation on
2026-09-24 and recorded in English as meant: *bring the most robust solution to
the panel, even if it is less economical*. They do not lift design.md §1.6's
payment rule.

---

## The question

**How does a program build a C struct whose fields include long arrays, without
writing every element, and without giving up what the language guarantees?**
It is M-buildable-structs, row 63 of `docs/ROADMAP.md`, `scheduled`: *a real
five-field `utsname` needs 1280 literal zeros* (panel 164). The row stays
`scheduled` during this sitting, because one `**OPEN**` row at a time is what
`site/src/lib/chain.ts` accepts.

What the language says today:

- spec § 13 (`spec/heroes-spec.md:347-352`): *a group's `record` is the
  header's struct … a fixed array of one: `i32[4]` … build one with
  `[a, b, c, d]`, as many elements as the type says.*
- spec § 5: *All bindings are initialised* (line 124), and the production
  `ident ":" Type ( "@" | "=" ) Expression NEWLINE` (line 137) has no form
  without a value. So a value of a record comes from a construction, a function
  that returns one, or a copy; there is no declared-now-filled-later.
- design.md §4.9 (line 1378): *No methods …, no inheritance, no private fields,
  no default values.* design.md Part 6 has no row on default values (grepped
  `default` over Part 6, line 2629 on).
- Panel 163 (`docs/panel/163-the-wall-was-not-there-and-the-brief-said-it-was.md`)
  refused three routes and adopted the status quo: **a call typed by context**
  (`repeat(0, 256)`; the compiler-engineer's veto: C cannot return an array and
  there is no constant folding), **a zero default for omitted fields** (+32 real,
  120-180 lines; the spec-warden's veto, withdrawn *when three `examples/`
  programs declare a fixed array longer than 8, and a named removal is measured
  in the same commit*), and **an out-parameter needing no initial value** (vetoed
  by the ffi-pragmatist on uninitialised bytes reaching `validated_bytes()` at
  exit 0 under ASan and UBSan; Rust's `mem::uninitialized` withdrawn). It left
  **`[0; 256]` unpriced**, and its critic measured that the veto on a call
  typed by context does not transfer to it.

## Measured today

**1. The repository was the wrong sample; real headers are the right one.**
`git ls-files '*.hero' | grep -v '^archive/' | xargs grep -hoE ': [iuf][0-9]+\[[0-9]+\]'`:
the longest fixed array in this repository is **8** (`u8[8]`, 11 of them), and
`examples/` declares one fixed-array field (`examples/raylib/main.hero`). The
headers M-core-packages will bind are a different world. `census.sh` here
includes 35 headers (POSIX, sockets, `pthread.h`, `termios.h`, `sqlite3.h`,
`curl/curl.h`, `openssl/sha.h`, `openssl/hmac.h`), asks clang for its AST, and
prints every array field of a struct or union after macro expansion:

| leg | array fields | longer than 8 | 64 or more | records with one longer than 8 | of them public (no leading `_`) |
|---|---|---|---|---|---|
| Darwin arm64 (`census-darwin-arm64.tsv`) | 108 | 81 | 27 | 61 | **42** |
| Linux arm64 (`heroes-linux-arm64`) | 77 | 50 | 14 | 34 | **29** |
| Linux x86-64 (`heroes-linux`) | 86 | 44 | 11 | 29 | **23** |

Windows: **unrun**, the box was not started. A record the awk names empty is
an anonymous one (one field on Darwin).

**2. The same struct has different arrays on each platform**, read from the three
`.tsv` files:

| struct | Darwin | Linux, both legs |
|---|---|---|
| `utsname` | `sysname` … `version`, five of `[256]` | six of `[65]`, `__domainname` included |
| `sockaddr_un.sun_path` | `[104]` | `[108]` |
| `sockaddr_storage` padding | `__ss_pad1[6]`, `__ss_pad2[112]` | `__ss_padding[118]` |
| `dirent.d_name` | `[1024]` | `[256]` |
| `termios.c_cc` | `[20]` | `[32]` |

A binding written on Darwin and compiled on Linux is **refused loudly**:
`uname_darwin.hero` under `heroes-linux-arm64` is `check` 0 and `run` **1**, `error[ffi_field_type]: `Utsname.sysname` is not
`i8[256]` in `sys/utsname.h``. So today a record over such a struct is
written once per platform, and nothing corrupts.

**3. The arrays come in three kinds** (the coordinator's reading of the public
rows; a classification, not a count):

- **bytes the program never touches**: padding and opaque state,
  `sockaddr_storage.__ss_pad2[112]`, `sqlite3_snapshot.hidden[48]`, the
  `__opaque` of every pthread type;
- **buffers C fills**: `utsname`, `dirent.d_name`, `statfs` (Darwin, two of
  `[1024]`), `ifreq.ifr_name[16]`;
- **bytes the program writes**: `sockaddr_un.sun_path` (a path, then zeros),
  `in6_addr` (16 bytes), `termios.c_cc`.

**4. `partial` already zeroes what it does not declare.**
`./heroes build tests/golden/run/ffi-a-filled-record-is-asked-for.hero --emit-c`
constructs the record as `t10 = (struct slot){.name = {t1, …, t8}};`, a
designated initialiser, and the header's undeclared `int32_t id` is therefore
zero by C11 §6.7.9 (the members not named are initialised as objects of static
storage duration). The price of `partial` is panel 163's disagreement 3: `==`,
`hash` and a map key are compile errors for it and for any value holding it.

**5. Today's routes, run** (`uname_*.hero`, `sunpath_*.hero`; Darwin with the
lane's compiler, Linux in both containers with a compiler built from the seed):

| program | `check` | run | note |
|---|---|---|---|
| `uname_darwin.hero`, `partial` + a Heroes producer of 256 zeros | 0 | 0, prints `Darwin` | `heroes measure`: 917 on `cl100k_base`, **vendored, a lower bound** |
| `uname_linux.hero`, the same at `[65]` | 0 | 0, prints `Linux`, both legs | 334 vendored |
| `sunpath_*_ascii.hero`, zero `sun_path` then write the path byte by byte | 0 | **134**, all three legs | defect 091 below |

**6. Defect 091, found while measuring: writing one element of a fixed-array
field is `check` 0 and dies at run time.** `elem_min.hero` + `elem_min.h`:
`s: Slot @ Slot(name: [0, 0, 0, 0])`, `s.name[1] @ 72`. Run **134**,
`panic: entered unreachable code — this is a compiler bug, please report it`:
three of three on Darwin and Linux x86-64, and on Linux arm64 by `heroes run`
and by the built binary. The emitted C carries
`hero_unreachable(); /* not an element write */`, written at
`selfhost/emit/inst.hero:133`: a store whose place has an index goes to
`container.write_element` (`selfhost/emit/container.hero:234` returns
`fail(... "not an element write")`), which serves the copy-on-write `[T]` and
not `T[N]`. It is defect 052's sibling: that was `s[0] @ 65` on a `str`, closed
2026-09-16 by a REFUSAL because spec § 3 calls `str` immutable. Here spec § 5
line 121 admits the write — *`@` declares a mutable cell and re-binds it, **or
a field or element inside one*** — so the spec has ruled and the repair is a
LOWERING. It is filed with this sitting, number agreed with the other session.
**So the third kind of array above has no working route today at all**, except a
literal that spells every byte as a number.

**7. All-zero is not a valid value of every C type, and it depends on the
platform.** `zmutex.c`, `memset` a `pthread_mutex_t` to zero and lock it:

| leg | zero equals `PTHREAD_MUTEX_INITIALIZER` | `pthread_mutex_lock` |
|---|---|---|
| Darwin arm64, `-O0` and `-O2` | no | **22, `Invalid argument`** |
| Linux arm64 and x86-64, both levels | yes | 0, and unlock 0 |

`initializers.sh` expands every `*_INITIALIZER` macro the census headers
define: on Darwin **all five are non-zero** (`PTHREAD_MUTEX_INITIALIZER`
`{0x32AAABA7, {0}}`, `…COND…` `{0x3CB0B1BB, {0}}`, `…RWLOCK…`, and the
recursive and error-checking mutexes). On Linux the expansions contain enum
names (`PTHREAD_MUTEX_TIMED_NP`), which the script cannot evaluate, so
`cond` and `rwlock` there are **unrun**; the mutex is measured equal above.

**8. Padding.** `padding.c` dirties the stack with `0xAA`, then builds
`struct addrinfo` (48 bytes, 44 of fields, 4 of padding on every leg) and
`struct termios` four ways and counts the `0xAA` left:

| shape | `-O0`, all three legs |
|---|---|
| CONTROL: every field assigned, no initialiser | **4** left |
| today's emitter shape, `T t; t = (T){.f = v};` | 0 |
| `memset` then assign | 0 |
| `T t = {0};` then assign | 0 |

At `-O2` the control also reads 0 on every leg, so **the method measures
nothing at `-O2`** and no claim is made there. The zeros at `-O0` are clang's
behaviour, not a promise: whether C11 guarantees padding after a compound
literal or after a member store is **a question for the seats**, not a premise
(the coordinator recalls C11 §6.2.6.1p6 saying a member store leaves padding
unspecified, and has not verified it).

## The proposal — the robust composition, as a spec diff to § 13

After *"build one with `[a, b, c, d]`, as many elements as the type says."*:

> Or name only the fields you set and end with `rest: zero`: every field you do
> not name is zero, and so is every byte between fields. A record outside a
> group has no `rest`. `rest: zero` is refused unless the record says `zero`
> after its name, which claims that all zeros is a valid value of that struct
> on every platform. `s.to_fixed()` gives the bytes of a `str` as the fixed
> byte array its position expects, the rest zero, and fails when they and a
> terminating zero do not fit.

Three clauses, each separable, and the spellings are for the reader test to
settle, not fixed:

1. **R1, the rest is zero only where it is written**, `rest: zero`, fields and
   padding both, for group records only. §4.9 is untouched for Heroes records:
   a forgotten field there stays `missing_fields`.
2. **Z, who says zero is valid.** **Z1**: the binding says it once on the
   record, and silence refuses (the direction the `lent` default took: silence
   is the safe reading). **Z2**: every group record admits it. The Darwin mutex
   is the case that separates them, and measurement 7 says a Z1 claim written on
   Linux is false on Darwin.
3. **T, a `str` into a fixed byte field**, failing as a `T?` rather than
   truncating, for the third kind of array.

## The routes, and where the list came from

Enumerated from panel 163's four routes and its unpriced fifth, the author's
conversation of 2026-09-24 (which proposed a zero default for every type and
then weighed the forms above), and the census. **The list is a measurement
too**: say what would have to be true for a route that is not here.

| route | what | origin |
|---|---|---|
| **R1** | `rest: zero` at the construction, group records only | this sitting |
| **R0** | a zero value of a whole group record, `T.zero()`, then `@` stores | this sitting; needs 091 repaired to store into an array |
| **R2** | panel 163's route 2: omitted fields of a group record are zero, no mark | panel 163, conservative there |
| **A** | `[x; N]`, a repetition literal | panel 163, unpriced |
| **Z1 / Z2** | zero-validity declared on the record, or assumed | this sitting |
| **T** | `str` into a fixed byte field, fallible | this sitting |
| **L** | repair 091 and write the bytes in a loop, no new form | the conservative route for the third kind |
| **S** | the status quo: `partial` plus a producer | panel 163's adoption |
| **D** | a zero default for every type, Heroes records included | the author's first question, 2026-09-24; the coordinator argued against it in conversation on §4.9 and the thesis (a forgotten field becomes a silent zero at exit 0), and the sitting may overrule that |

**A route the census suggests and nobody has priced**: a record field whose
length is the header's, so one binding serves every platform (the `utsname`
row above). It is outside the question as asked, and a seat that finds it
inside is heard.

## What every seat is asked for

A verdict per route with its design.md section, its cost in the unit your
instrument counts, one falsifiable prediction, and the condition that would
change your verdict. **Build in your own directory**,
`<scratchpad>/178-<seat>/`, a `git archive` of HEAD `57679005`; build the
compiler there from the seed (`clang -I runtime seed/heroes.c runtime/runtime.c
-o heroes`, 3.50 s measured in the lane); a rebuild of an edited `selfhost/`
from a seed built with `-O2` is about 32 s (`.claude/skills/panel/SKILL.md`
line 123). Never use the trunk's `./heroes`, never another seat's directory,
never write into `~/Temp/heroes-lane-panel-178` or the trunk. Linux legs:
`docker run --rm -v <your dir>:/src:ro heroes-linux-arm64 bash -c '…'` and the
same with `heroes-linux` (x86-64 under emulation); build the compiler inside
the container from `/src/seed/heroes.c`.

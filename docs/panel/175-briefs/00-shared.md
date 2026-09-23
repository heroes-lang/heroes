# Panel 175 — shared brief: the call that ends a handle's life, and the pointer that could have been one

2026-09-23, M-agreed-retention, after step 2 (`64c92654`). Full panel, because
both questions have a sentence of `spec § 13` behind them. The tree is frozen at
`64c92654` from the moment these briefs exist until the synthesis is written.

**Every seat works in its own directory, `<scratchpad>/175-<seat>/`**, where
`<scratchpad>` is the path your prompt gives you. Copy the trunk there, build
your own compiler inside it from the seed (`clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, about three seconds), and never use the trunk's
`./heroes`, build in the trunk, or read another seat's directory
(`.claude/skills/panel/SKILL.md` step 3, amended 2026-09-23). Set
`HEROES_RUNTIME=<your copy>/runtime` for `run` and `build`. Your report goes to
`docs/panel/175-reports/<seat>.md` in the trunk, and nothing else in the trunk
is written.

The reproducers are in this directory, `docs/panel/175-briefs/`, and every
number below was produced by running them on 2026-09-23 while this brief was
written.

## Question 1 — defect 075: `acquires` names a releaser, and nothing holds the program to it

`spec § 13` today:

> `acquires sqlite3_finalize` after a result or `@` out-parameter reaching a
> handle says the call begins that handle's life and names the one that ends
> it, which the program owes it. The live handles are a set, so giving one back
> twice aborts on its own.

**Measured.** `./heroes check` then `./heroes build -O0` and five runs of the
binary, for each file:

| reproducer | what it does | `check` | run |
|---|---|---|---|
| `popen_darwin.hero` | real `stdio.h`: `popen … acquires pclose`, closed with `fclose` | 0 | 0, 0 bytes, 5 of 5 |
| `oneacq.hero` + `i2.h` | `h_open() -> H acquires h_close`, closed with `h_close2`, one module | 0 | 0, 0 bytes, 5 of 5 |
| `outacq.hero` | the same through `@out: H acquires h_close` | 0 | 0, 0 bytes, 5 of 5 |
| `xacquires/` | module `alt` says `acquires h_close`, module `main` says `acquires h_close2` over `alt.H` | 0 | 0, 0 bytes, 3 of 3 |

The popen shape was also run at step 1 on both Linux containers
(`popen_linux.hero`, `tag _IO_FILE`) and on the Windows box
(`popen_windows.hero`, `_popen`/`_pclose`, `tag _iobuf`): `check` 0 and `run` 0,
five of five, on all four platforms. Under `--sanitize` on Darwin it builds,
runs at 0 and writes zero bytes. **What `fclose` on a `popen` stream does inside
each C library is unrun**; what is measured is that nothing in Heroes objected.

**Where it lives, read rather than inferred.**
`selfhost/check/acquiring.hero:194` `releaser_reads` checks that the name after
`acquires` is an `extern` of the module taking the handle `consumes`
(`unread_releaser` at `:269`), and nothing reads the name again.
`runtime/parts/alloc.c:402` `hero_handle_acquired(const void *h)` and `:422`
`hero_handle_consumed(const void *h)` take an address and nothing else, and the
set is `const void **` (`hero_handle_slot`, `:380`). The emitter calls them from
`selfhost/emit/handle_traffic.hero`: `for_call` (`:36`) after the call, where
`fd.value.acquires_result` (`:39`) is the releaser's span, and `before_call`
(`:101`) BEFORE a consuming call, since defect 071.

**Why it is owed.** Panel 148 adopted the named mark over the bare word, and its
resolution says the bare word *leaves both the mismatched-deallocator class and
the forgotten-mark class open*
(`docs/panel/148-the-mark-is-written-and-never-inferred-and-it-names-what-ends-the-life.md`,
§ What conservative would have been). Measured today, the named mark leaves the
first class open too.

**How much of the tree it touches.** Enumerated with a regex over single-line
`extern` signatures (`function <name>(… <p>: <Type> consumes`), so a multi-line
signature is not counted: in `examples/`, every handle type has exactly ONE
consuming function (`Curl`: `curl_easy_cleanup`; `CDb`/`Db`: `sqlite3_close`;
`CStmt`/`Stmt`: `sqlite3_finalize`). In `tests/golden/`, 29 files use
`consumes` and in none does one handle type have two. So **no test in the tree
exercises the shape**, and no shipped program has it. `selfhost/` declares no
`extern` with a handle mark at all (0 non-comment lines), so the compiler's own
speed cannot move with the runtime set.

**The routes, and where the list comes from.** Enumerated from the three places
a releaser's identity could live — the checker's program-wide walk, the
runtime set, and the type system — by reading `check/acquiring.hero`,
`check/decls.hero`'s `one_tag_one_type` (`:334`) and `alloc.c`. **A route nobody
listed is the most valuable thing a seat can bring**, so ask what would have to
be true for one to exist.

- **A — the set remembers the releaser.** The acquiring call tells the runtime
  which function its mark names; a consuming call by another function aborts
  BEFORE C runs, naming both. Loud, at run time, on every path.
- **B — the checker refuses the crossing where it can see it.** A binding made
  by an acquiring call and handed to a different releaser in the same function.
  The checker has no flow analysis (`check/acquiring.hero`'s own header says so,
  citing `check/leasing.hero:29` and `check/consuming.hero:22`), so this is
  partial by construction.
- **C — two Heroes types over one tag when their releasers differ.** Narrow
  `one_tag_one_type` again (panel 170 narrowed it at `tag void`, defect 072), so
  `popen` gives a `Pipe` and `fopen` a `File` and the crossing is
  `type_mismatch` at check. Every C function taking `FILE *` then needs one
  declaration per Heroes type, which `declared_twice` refuses inside a module.
- **D — A and B together**, or A with C.

Whether a sentence is owed in § 13 — *and ending it with another aborts*, or
none because the sentence already promises it — is part of the question.

## Question 2 — the milestone's first item: a pointer C made, given back twice

**Measured**, the C side `noinline` (`r1.h`), five runs each:

| reproducer | `check` | Darwin arm64, this brief | Linux arm64 and x86-64, step 1 | Windows x86-64, step 1 |
|---|---|---|---|---|
| `p10.hero`: a `ptr` from `make()`, `release(p: p)` twice | 0 | 133, **0 bytes** | 134, glibc's `free(): double free detected in tcache 2` | `0xC0000374`, **0 bytes** |
| `b_out.hero`, panel 172's: a `cstr` C filled, freed twice | 0 | 133, **0 bytes** | the same glibc line | `0xC0000374`, **0 bytes** |
| `handle.hero`: the same program over `record Box tag box`, `acquires`, `consumes` | 0 | 134, **396 bytes**, the runtime names the double release | 134, 399 bytes | the runtime's line, 401 bytes |

`consumes` on a `ptr` is already refused (`unread_mark`), so `p10` is the only
spelling. The handle is caught because the emitter tells the set before the
release: in the emitted C of `handle.hero`, `hero_handle_consumed(t3);` is line
126 and `(void)box_close(t3);` line 128.

**Why the runtime says nothing.** Panel 173's crash handler speaks only while a
lease is live: `runtime/parts/os.c:249` (`if (!hero_runtime_spoke && held > 0)`)
and `:207` on Windows. With no lease, the handler chains and re-raises in
silence.

**Routes, enumerated from the same three places:**

- **E — the handler speaks with no lease live**, saying only what it saw: the
  process is dying by a signal (or on Windows the exception) that is not the
  runtime's own, raised under the Heroes function the frame walk names. Panel
  173's R1 binds it: no path prints a sentence measured false, and the fact
  comes first with the likely reason named as likely.
- **F — a sentence in § 13** that a pointer C hands out for the program to give
  back is a handle, which is caught, and that a `ptr` is not.
- **G — nothing**: the double free is C's, and `ptr` is the escape hatch.
- **E and F together.**

## Known, and not part of this sitting

**Defect 076** is repaired without a sitting, on panel 173's R1: `bout.hero`
declares `owned free` on a cell C has already freed, and the runtime's magic
check prints `a str was fabricated from a foreign pointer` (5 of 5 in this
brief's run, 134, 106 bytes), which is false. It is listed here so no seat
reports it as new; a seat that finds a reason it should NOT be repaired that way
should say so.

## Unrun, and named

Windows beyond the table above: the box may be off during the sitting, and a
route that changes the runtime is measured there at its landing. What each C
library does after `fclose` on a `popen` stream. Whether any real library has
two releasers BOTH valid for one acquisition (the ffi-pragmatist's task).

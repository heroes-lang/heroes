# Panel 156 — shared brief

**The question.** A Heroes program that dies because C read through a null
pointer says three different things on the three platforms. Which one is
correct, and what is repaired? Convened by the author 2026-09-16 at
M-check-completeness, after the coordinator diagnosed CI and found the repair's
direction contested.

Full panel: this is a diagnostic's content, it is reader-facing, and the
specification already has a sentence about it.

## Why this is not only a bug report

`spec § 6` says, of the language's one definition of abort:

> An abort ends the program at once, **saying why**; no `T?` carries one.

On one of the three platforms the program says nothing at all. So the question
is not merely which frame to name — it is whether the language keeps a promise
it has written down, and where.

## What ships today, measured

`tests/golden/surface-fixtures/nullread/main.hero`, run at `-O0`:

```
extern "node.h"
    record Node tag node
    function node_open(v: i64) -> Node
    function node_value(p: Node) -> i64

function main()
    real = node_open(v: 7)
    print(node_value(p: real))
    empty: Node @ nullptr
    print(node_value(p: empty))
```

`.expected` demands stdout `7`, and stderr carrying both `panic: a null pointer
was read through` and `called from node_value`.

| | macOS arm64 | Linux x86-64 | Windows x86-64 |
|---|---|---|---|
| exit | 134 | 134 | **139** |
| stdout | `7` | **empty** | **empty** |
| stderr | `panic: … at offset 0x0, called from node_value` | `panic: … at offset 0x0, called from main.main` | **empty** — `Segmentation fault` |
| CI | green | red | red |

**ALL THREE WERE MEASURED BY THE COORDINATOR ON 2026-09-16**, and the Windows
row is a measurement rather than a reading: the author powered the box during
this sitting's preparation. This Mac directly; Linux in the container built from
`docs/ref/environment/linux/Dockerfile`, `uname -m` `x86_64`, tree copied
read-only and the seed built inside; Windows over `ssh win`, tree sent by
`COPYFILE_DISABLE=1 tar --no-xattrs`, built with `seed/README.md`'s own line
(`clang -I runtime seed/heroes.c runtime/runtime.c -Wl,/STACK:67108864 -o
heroes.exe`, exit 0, `MINGW64_NT-10.0-26100`, clang 22.1.8).

**And the Windows number is the one that changes this sitting.** It is not a
missing blame line: it is **exit 139 with both streams empty**, which is word for
word what this fixture's own comment describes as the state BEFORE defect 045's
repair — *"It died at exit 139 with both streams empty, which design.md §1.12
forbids by name."* So defect 045's repair does not reach Windows at all, and the
condition §1.12 names by name is live on one of the three platforms today.

`main` has been red since 2026-09-15. Every other suite is 0 failed on all three
legs; this is one test.

## Three findings, and the second is why the author called this sitting

**1. The lost `7`, Linux.** It is printed before the fault.
`hero_stack_say` writes with a raw `write`; `print` goes through buffered stdio;
the handler ends at `abort()`. macOS's libc flushes stdio on abort and glibc
does not. Nothing in the tree states which behaviour is intended.

**2. THE GREEN LEG LOOKS LIKE THE BROKEN ONE.** `hero_stack_blame`
(`runtime/parts/stack.c:297`):

```c
static const char *hero_stack_blame(uintptr_t pc, uintptr_t fp) {
    Dl_info info;
    const char *first = NULL;
    if (dladdr((void *)pc, &info) != 0) {
        if (hero_stack_is_heroes(info.dli_sname)) return info.dli_sname;
        first = info.dli_sname;
    }
    for (int i = 0; i < 64; i++) {
        if (fp < hero_stack_lo - HERO_STACK_WINDOW || fp + 16 > hero_stack_hi || (fp & 7) != 0) break;
        uintptr_t next_fp = *(uintptr_t *)fp;
        uintptr_t ret = *(uintptr_t *)(fp + 8);
        if (ret == 0) break;
        if (dladdr((void *)(ret - 1), &info) != 0 && hero_stack_is_heroes(info.dli_sname)) return info.dli_sname;
        if (next_fp <= fp) break;
        fp = next_fp;
    }
    return first;
}
```

and `hero_stack_is_heroes` tests for an `h_` prefix after an optional leading
underscore. So `node_value`, a C function from the fixture's own header, is
**not** a Heroes symbol: it is `first`, the fallback returned when the frame
walk finds nothing. `main.main` is `h_main_main` demangled — the frame walk
**succeeding**.

The handler's own comment says which of those it wants:

> The caller is named where the frame walk can find a Heroes function, because
> unlike the `pc == 0` case the stack here is intact: the fault is inside the C
> function the program called, **and its caller is the author's own line.**

So on Linux the walk finds the author's own function and on macOS it finds
nothing and falls back to the C function that faulted. **`.expected` has been
pinning the macOS fallback as the answer since defect 045's repair landed.**

**3. Windows has no arm for this fault at all, and it dies at 139 in silence.**
The 139 is measured; the cause below is read from the source and is the
hypothesis the ffi seat is asked to confirm by building the repair on the box.
`runtime/parts/stack.c:567-590`'s vectored handler has exactly two arms:
`EXCEPTION_STACK_OVERFLOW`, and `EXCEPTION_ACCESS_VIOLATION` **with
`ExceptionAddress == 0`** — which is the PC, so that arm is a null FUNCTION
POINTER being CALLED (defect 013). A null pointer READ THROUGH has a valid PC
inside the C function and the touched address in `ExceptionInformation[1]`,
which no arm reads; the exception falls to `EXCEPTION_CONTINUE_SEARCH` and the
process dies with both streams empty. **The file's own comment names
`ExceptionInformation[1]` two paragraphs above the gap**, while explaining why
the POSIX arm reads the PC instead of `si_addr`.

## What the repository already said was owed

`runtime/parts/stack.c:440-442`, in its own words, on what would close this
class:

> …and dies at 139 in silence, and what would close it is each platform's own
> floor asked of the platform, **which needs a measurement on the Linux and
> Windows machines rather than on this one.**

It was written and nobody ran it. Panel 154's ratification separately records
route A's guard as *"adopted and unlanded"*, with a trigger naming
`selfhost/emit/ops.hero` — and a test demanding the blame line shipped anyway,
green here, so nothing local could catch it.

## What the sitting is asked to decide

- **R1.** Which blame line is correct — the Heroes caller (`main.main`), the C
  function that faulted (`node_value`), or both, or something else? The answer
  binds `.expected` and possibly the arm64 frame walk.
- **R2.** Is the arm64 walk's failure a defect to repair, or is returning the
  faulting C function a legitimate answer this platform gives? If it is a
  defect, what is the repair and what does it cost?
- **R3.** Must a program that aborts keep what it already printed? `spec § 6`
  says an abort *says why*; it says nothing about what came before. If yes, the
  repair is a flush on the abort path, and the panel should say where it lives
  and whether it is safe in a signal handler.
- **R4.** Windows: is the missing arm a defect, and is the repair the third arm
  reading `ExceptionInformation[1]` against `HERO_NULL_WINDOW`, mirroring the
  POSIX side? What may be decided without the box, and what must wait for it?
- **R5.** `main` is red now. Is there a defensible way to make CI green before
  the full repair lands, or does the red stand until it does? A recommendation
  either way, with what it costs.

## Process rules binding every seat

**THREE OF FIVE SEATS WERE KILLED BY THE WATCHDOG AT 600 SECONDS AT PANEL 155**,
and panel 087 lost four of five the same way. So:

- **Run no command that takes more than ~60 seconds.** No full net, no rebuild
  from `selfhost/`. If a measurement needs one, write it down as **UNRUN** and
  name the command that would settle it — CLAUDE.md § RUN IT permits exactly
  that and forbids inventing the number.
- **Write your report to disk FIRST**, with whatever you have, then continue.
  Partial and honest beats complete and never delivered.
- The seed builds in a few seconds: `clang -I runtime seed/heroes.c
  runtime/runtime.c -o heroes`. **Rebuilding the compiler from `selfhost/` takes
  59 s, measured 2026-09-16** — the older figure of ~20 minutes in the panel
  skill is stale by a factor of twenty and is what kept seats from measuring.
- **Build in a copy.** `cp -r` the tree to your scratchpad, `rm -rf target
  build`. The repository's working tree is frozen for this sitting.
- **Never read `archive/bootstrap-rs/` or `crates/`.**
- Capture exit codes directly, never through a pipe.
- Every number you report must come from a command you ran in this session.

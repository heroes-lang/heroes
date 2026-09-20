# 036 — the length does not survive to the IR, and the header it would find is already freed

2026-09-20, M-declared-extents step 10, on the question panel 167 left unpriced
and step 9's record named as the next thing to measure: **whether the field's
length survives to the IR**, so the emitter could pass it to a release that has
to find a TRAILING header.

It does not. And the measurement that answers it turned out to answer a larger
question, which is why this is a measurement record and not a line in a sitting.

## What was asked

Panel 167 clause 1 adopted a trailing header for route A's lease allocation.
Step 9 read `runtime/parts/str.c:461` and found that `hero_held_release` finds
its header by **subtracting** `sizeof(HeroHeldHeader)` from the pointer, which a
trailing header cannot be found by, and that `end_lease(@p)` carries only the
cell. Two candidates were recorded: a pointer-keyed side table, or the emitter
passing the length.

## Answer 1 — the length does not survive to the IR. Three commands

**`./heroes build <lease program> --emit-c`** emits, in one function:

    t3 = hero_str_held(t2);
    ...
    hero_held_release(&h1_c);

**`--dump-ir` on the same program**: `$t3: cstr = call builtin lease($t2)`,
`store c <- $t3`, and later `call builtin end_lease(@c)`. **The release
instruction carries the slot and nothing else** — no length, no operand naming
the initialiser.

**`--dump-ir` with a `while` loop between the lease and the release**: the lease
lands in `bb0` and the release in `bb3`. Recovering the initialiser from the
release site is a cross-block reaching-definition walk, and neither pass has one.
`selfhost/check/leasing.hero:25-30` names the **absence** of flow analysis as the
reason its own rule is sound.

And for the `str` lease the length is not a compile-time constant at all: it is
`$t2`'s run-time `.len`, and `$t2` is dead by the release.

## Answer 2 — the question was the wrong one

A C model of both layouts, committed as `docs/panel/168-briefs/layouts.c`, whose
release panics exactly as `runtime/parts/str.c:462-466` does. The trailing
release was handed the **correct** length. `clang -std=gnu11 -Wall -Wextra
-Wpedantic -Werror`, five runs of each on Darwin arm64:

| sequence | plain | `-fsanitize=address,undefined` |
|---|---|---|
| leading, C frees, then `end_lease` | **exit 133** ×5, **no message at all**, and `MallocErrorAbort=1` changes nothing | `bad-free`, named at C's own line |
| trailing, C frees, then `end_lease`, correct length | **exit 134** ×5, carrying the runtime's panic *"this is a compiler bug, please report it"* | `heap-use-after-free`, named inside the release |
| leading, ordinary case | exit 0, correct bytes | clean |
| trailing, ordinary case | exit 0, correct bytes | clean |

**With the correct length in hand the trailing header still reads freed memory**,
because C has already freed the block. Neither of the two candidates addresses
the failure the trailing header was adopted to fix; both address only the
ordinary case, which the leading header already gets right, identically.

## Answer 3 — and panel 168's engineer found where it becomes unsound

The model above varied the layout and nothing else. Varying the **size** is what
CLAUDE.md § RUN IT asks for, and it is where the trailing header stops being
merely useless. Three runs, identical, independently reproduced by the sitting's
completeness critic three of three:

    n=4096     magic survives the free: no   -> release would panic
    n=65536    magic survives the free: YES  -> release would DOUBLE FREE
    n=1048576  magic survives the free: YES  -> release would DOUBLE FREE

At 64 KiB for real, five runs: **no `panic:` line**, the magic check passes, and
the process dies inside the **second** `free`, exit 134, stderr empty.
`runtime/parts/str.c:447-450` already states the rule this breaks, *"freed memory
owes nobody its contents"*, written when an earlier draft of this same feature
made the same mistake.

**Unrun and named**: Darwin arm64 only. glibc's `mmap` threshold is 128 KiB by
default, so the 1 MiB case is likely the same on Linux and the 64 KiB one may
not be.

## The correction this record owes itself

The first framing of measurement 2, written before the model was aligned with the
runtime, said the leading header gives a **loud abort** and the trailing one a
**silent exit 0**. Both halves are false, and the model that produced them was
flattering one side by returning quietly where the runtime panics.

- The leading header is **silent** on Darwin: exit 133, no message, not even from
  malloc.
- The trailing header is **loud and wrong**: it names the compiler for a fault
  that is the program's.

Neither is a good failure, which is filed separately as **defect 070**.

## What it does not settle, and what settled it instead

Whether any shipping library expects the *library* to free a buffer the program
must then not release. Panel 168's ffi seat answered it and the answer is
larger than this record's question: **the property the give-away case needs is
not the allocation base, it is the allocation heap.** `sqlite3_free` wraps `free`
only in the default configuration; with `sqlite3_config(SQLITE_CONFIG_MALLOC)`
replaced, the trailing header gives `134 134 134 133 133`. No header layout
inside the Heroes runtime can supply a block from a library's own allocator.

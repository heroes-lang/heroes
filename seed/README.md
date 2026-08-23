# The seed — how to build Heroes with nothing but a C compiler

`heroes.c` is the Heroes compiler, in C. It is what the compiler emits when it
compiles itself, and it is here so that a clean checkout can build a working
compiler with no Heroes compiler and no Rust.

```sh
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes
./heroes --version
```

That is the whole of it: one command, no flags beyond the include path, no
configure, no make. Measured on Apple clang 21.0.0, arm64-darwin: **3.4 s at no
optimisation level** (2026-08-19, on the 22,025,792-byte seed this milestone
regenerated; it was 3.7 s on the 21 MB one), 27 s at `-O2`. Any C11 compiler should do — the file was
compiled clean under `-std=c11`, `gnu11`, `c17`, `gnu17`, `c23`, `gnu23`, under
`-pedantic-errors` and under `-Wall -Werror`, all on one clang, which is what
"any C compiler" is currently worth here.

The compiler it produces is the real one. It compiles programs, it compiles
`selfhost/`, and what it emits for `selfhost/main.hero` is this file again, byte
for byte.

## What it needs beside itself

Eighteen files, and they are all in the checkout:

- `seed/heroes.c`
- `runtime/heroes_runtime.h` and `runtime/hero_os.h` — `heroes.c` includes both,
  which is why `-I runtime` is not optional
- `runtime/runtime.c` and the **fourteen** files under `runtime/parts/`, which
  `runtime.c` includes. Shipping `runtime.c` alone does not link.

And at **run** time the compiler needs `runtime/` again — it compiles it — which
it finds under the working directory, or wherever `$HEROES_RUNTIME` says.

Nothing else. The standard library is inside the binary
(`selfhost/library_source.hero`), because panel 028 R3 ruled it embedded: a
library file on disk would be a file clang cannot type-check and therefore a
decoy the `_Static_assert` cannot guard.

## When this file must be regenerated

**In the same commit as the change that breaks it**, by whoever has a working
compiler in hand. That is not a preference. Generated C cannot be hand-patched:
if the seed stops building today's `selfhost/`, the only thing that can make a
new one is a Heroes compiler, and the only way to get one is a seed that works.
Go's own "frozen" 1.4 bootstrap was respun at least twice because a frozen seed
rots against the host; Go could patch theirs, because it was hand-written C.
This is not (panel 085 R2).

```sh
heroes build selfhost/main.hero --emit-c -o seed/heroes.c
```

Three things force it, and each has an instrument — the third was added
2026-08-23 (author decision, `/decide`) because it is the one a green build
cannot see:

1. **`HERO_RUNTIME_ABI` moves.** The seed carries
   `_Static_assert(HERO_RUNTIME_ABI == N, ...)` on its eighth line, so a runtime
   from another compiler stops the build with that message rather than linking
   quietly. `the_seed_builds_from_a_clean_checkout` asserts the two numbers agree.
2. **`selfhost/` uses a form the committed seed cannot parse.** The cheap check
   is the build above — it is 3.4 s and it runs in the test. The expensive one is
   the full fixpoint (`heroes build selfhost/main.hero --emit-c` from the
   seed-built compiler, and the bytes must match), which belongs to a milestone
   close: **15m41s measured 2026-08-19**, when `measure` and `mutate` joined the
   port and the seed grew by a megabyte.
3. **Anything else `selfhost/` says changes what the compiler DOES** — a
   different diagnostic, a different byte in the emitted C, a new library
   function. This is the condition the first two miss and the one that bites:
   the library's own text (`selfhost/library_source.hero`) moves neither the ABI
   nor the grammar, so a stale seed passes condition 2's 3.4 s build **and the
   compiler it makes carries yesterday's library**. Every job after it then tests
   something nobody wrote. Its instrument is CI's *The seed is what today's source
   emits* — the fixpoint of condition 2, run as a `cmp` on Linux at tags — and
   until 2026-08-23 that instrument existed only as a sentence in
   `tests/harness/suite_determinism.hero` claiming it did (panel 087). The rule of
   thumb needs no instrument to apply: **if the diff touches `selfhost/`, the seed
   is regenerated in the same commit.** The two conditions above are what make it
   *loud* when you forget; this one is why you should not rely on them.

## If the seed is already broken — how to get a compiler back

This is the case the rest of this file exists to prevent, written down because
*prevented* is not the same as *impossible*. Suppose someone pushed a change to
`selfhost/` without regenerating the seed, the test was not run, and now the
committed seed no longer builds today's source. Nobody has a working compiler.

**The tags are the chain.** Every milestone tag from `m-selfhost-fixpoint` on
carries its own `seed/heroes.c`, and each one was green at
`the_seed_builds_from_a_clean_checkout` when it was made. So there is always a
last-known-good rung to stand on:

```sh
git tag --list --sort=creatordate            # newest last
git checkout <the newest tag whose seed builds>
clang -I runtime seed/heroes.c runtime/runtime.c -o /tmp/heroes-old
git checkout main
/tmp/heroes-old build selfhost/main.hero --emit-c -o seed/heroes.c
```

The last line is the whole recovery: an **older compiler emitting the current
source**. It works as long as the language the current source uses is one the
older compiler can still read — which is the same condition the refresh rule is
about, one rung further back. If that fails too, go one tag older and repeat; the
chain only ends at `m-selfhost-fixpoint`, which is where the seed begins.

Then re-verify, in this order, because the cheap check catches almost everything:

```sh
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes    # 3.4 s
./heroes build selfhost/main.hero --emit-c -o /tmp/again.c    # 15m41s
cmp seed/heroes.c /tmp/again.c                                # must be silent
```

**Why there is no uploaded executable, and why that is the safer answer.** A
binary would have to be one per platform — arm64 Darwin, x86-64 Linux, Windows —
and none of them can be read. Nobody can diff a Mach-O against the source it
claims to come from, which is the objection Zig's checked-in `zig1.wasm` drew and
answered by making it *reproducible* rather than by trusting it. `seed/heroes.c`
is C: a reader can open it, a compiler can check it, and the same file works on
every platform whose C compiler works. A binary would also be an outward-facing
artifact, which CLAUDE.md §14 puts a hard stop in front of.

The one thing a binary would buy is the 12-minute rung, and the chain above buys
it for the price of a checkout.

## Why raw C, and why in git

Both halves were measured rather than argued (panel 085 R1).

**In git**, because the checkout is the artifact: a seed attached to a release
and not to the repository is the shape of CVE-2024-3094, where the backdoor lived
in the release tarball and not in the source anyone could read.

**Raw**, because git delta-compresses plain text and cannot delta a compressed
stream. Compressing first saves 0.20 MiB on the first copy and costs 1 to 2.4 MiB
on every refresh after it: measured over three stored seeds, **2.74 MiB raw
against 5.78 MiB gzipped**. And a `.gz` would need a decompressor on the one path
that must not need one.

# Panel 157 — shared brief

**The question.** `heroes build --emit-c` writes C that clang refuses, for any
program binding a C type through `record <Name> tag <name>`. The same program
built to a binary works. What does `--emit-c` owe, and what is the repair?

Convened 2026-09-16 at M-check-completeness, on **defect 048**, which panel
156's ffi-pragmatist found at its own boundary while measuring something else —
nobody had put `--emit-c` to that sitting.

## The measurement

`tests/golden/surface-fixtures/structtag/main.hero` is in the repository and the
`surface` suite runs it green at `run`. The same program:

| command | result |
|---|---|
| `heroes build … -o <binary>` | works; the program prints `7` |
| `heroes build … --emit-c` | writes C; **clang gives 15 errors on it** |

The written C says `node *` where the header declares `struct node` with no
typedef. The ffi seat counted `struct node` **12 times** in the translation units
that produced objects and **0 times** in the ones that did not.

## Why the compiler is right and only the artifact is wrong

Defect 037's repair (panels 150, 151, 152) made the build a **ROUND**. The
emitter writes a handle's tag as the author wrote it; clang refuses that word
with *must use 'struct' tag to refer to type 'node'*, which is clang's own
statement that the word names a struct tag of exactly that spelling;
`cli/produce.hero` reads that off the round's refusals, adds the tag to
`struct_tags`, and asks for the round again. `cli/assemble.hero` is that round.

`--emit-c` returns **before any round runs**. `selfhost/cli/compile.hero:240`
says so in its own comment:

> The author's word for every handle tag: this is the first emission, and what
> clang says about a tag is learned in `cli/produce.hero`, after it.

and at `:257` the artifact is written and the function returns.

## What makes this a sitting and not a patch

**Running the round inside `--emit-c` means `--emit-c` calls clang, and one
suite's premise is that it never does.** `tests/harness/suite_emission.hero:70`:

> those cases are wrong FFI bindings, and the thing that refuses them is clang,
> at build time. `--emit-c` never calls clang, so all fourteen emit — measured

That premise is load-bearing for 14 cases. A repair that breaks it must say what
replaces them.

**And the contract is already written down, in two places that agree.**
`.claude/rules/cli-surface.md`: *"`--emit-c` is an **output**, not a dump."*
design.md:717: *"stop at the C and read it (stdout, or -o)"*. An output that
cannot be compiled is a defect against both sentences; a dump would not be.

**Neither spelling is universally right, which is why a round exists at all.**
Panel 151 measured that a spelling read FROM the header can no longer disagree
with it. Panel 152 measured that always writing `struct X *` answers yes for a
misspelled tag too, because `struct nosuchtype *p;` is legal C — and a tag that
is a typedef of an ANONYMOUS struct (`typedef struct { … } node;`) has no
`struct node` at all, so the qualifier is wrong there.

## Why it matters more than an artifact usually would

`--emit-c` is how the **seed** is made — `heroes build selfhost/main.hero
--emit-c -o seed/heroes.c`, `seed/README.md` — and CI asserts on every push that
`seed/heroes.c` is exactly what today's source emits. Measured 2026-09-16:
`selfhost/` declares **no** `record … tag` of its own (the 28 grep hits are
comments, message strings and parser fixtures), so the seed is unaffected today
and the fixpoint held twice this milestone. **The day the compiler binds a C
library through a tagged handle, it would not.**

## What the sitting is asked to decide

- **R1.** What does `--emit-c` owe: C that compiles, or the first emission? The
  two documents above say output; say whether they are right, and if the answer
  is *the first emission*, say which sentence changes and where.
- **R2.** If it owes compilable C, what is the repair? Running the round inside
  `--emit-c`, a refusal when a tagged binding is present, learning the qualifier
  without clang, or something nobody has listed.
- **R3.** What happens to `suite_emission.hero`'s 14 wrong-FFI cases under the
  repair? Their whole value is that they emit where a build would refuse.
- **R4.** Is there an instrument gap to close regardless of R1? Nothing in the
  tree compiles what `--emit-c` wrote, which is why 15 clang errors were
  invisible to every suite.

## Process rules binding every seat

- **Write your report file FIRST**, then improve it. Three of five seats were
  killed by the watchdog at panel 155 for producing nothing in 600 s; none at
  panel 156, where this rule was in the brief.
- **No command over ~60 seconds.** The seed builds in a few seconds
  (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`); rebuilding the
  compiler from `selfhost/` is **59 s measured**, not the ~20 minutes the panel
  skill used to claim. Never run the full net.
- Anything you cannot run is written down as **UNRUN**, with the command that
  would settle it.
- **Build in a copy**: `cp -r` the tree to your scratchpad, `rm -rf target
  build`. The repository working tree is frozen for this sitting.
- Never read `archive/bootstrap-rs/` or `crates/`.
- Capture exit codes directly, never through a pipe.

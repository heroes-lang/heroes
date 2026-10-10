# Panel 204, the shared brief: one module's `extern` groups, whose order is C's include order (563)

Written by the coordinator on 2026-10-10 from 01:16 (`date`), on the tree
frozen at `635e8f67` (worktree `lane-panel-204`: batch 17 closed, panels 202
and 203 ratified, defects 558 to 566 filed). Convened on the author's choice
through the question widget, *zero defects*, which named this sitting, and
their answer to its blind seat's budget, *10 dollars*, both between 01:06
and 01:14 by the clocks read before and after. Every fact names its command or file; a number marked **carried**
is a report's, a question for the seat.

**Lane: full**: what the spec says of declaration order (§ 4) and which
programs `build` accepts and what they mean. The compiler-engineer, the
ffi-pragmatist, the spec-warden, the historian, the blind seat run by the
coordinator as fresh `claude -p` sessions outside the repository (10 USD),
the critic before the seats and after them.

## The question

Spec § 4 (`spec/heroes-spec.md:113`) reads *Declaration order never matters;
mutual recursion needs no forward declarations.* An `extern` group is a
`Declaration` of § 4's grammar. Run by the coordinator from 01:14 to 01:16 with
the trunk's compiler built from the seed at `86189b2b` (the same compiler as
`635e8f67`'s), the cases under `.claude/worktrees/scratch-b15/p204/`, read
only:

- **`cfgone`** (panel 202's spec-warden): one module binds `a.h` (`#define
  LIMIT 100`, `static inline long half`) and `b.h` (`#ifndef LIMIT`, `#define
  LIMIT 10`, `#endif`, `static inline int cap(int v)` returning at most
  `LIMIT`). `main.hero`, `a.h`'s group first: `build` 0, prints `3` and `50`.
  `main2.hero`, `b.h`'s group first: `build` 0, prints `3` and `10`.
- **`jpeg`** (new): `ab.hero` names `stdio.h`, then `jpeglib.h` with
  `package "libjpeg"`: `build` 0, prints 1. `ba.hero`, the two groups
  swapped: `build` 1, `ffi_header_refused`, *`jpeglib.h`, the header this
  group names, does not compile: clang refuses it at line 984: unknown type
  name 'FILE'*, its last note *repair the header, or name the one that
  declares this group's C*, where the header needs no repair and the program
  needs the `stdio.h` group first.
- **`one`** (defect 538's own shape, the critic's): one module binds `a.h`
  (`static inline long twice`) and `b.h` (`static inline int twice`,
  `thrice`): `build` 1, `ffi_header_refused`, *cannot be compiled in one
  unit*, its note *whichever comes first* (defect 561 removes that clause in
  lane b18-ffi). The spec says nothing that refuses it.

**Panel 091** (ratified 2026-08-25,
`docs/panel/091-the-saving-the-architecture-already-makes.md:224-227`) found
the order load-bearing (`<jpeglib.h>` alone, `unknown type name 'size_t'`)
and wrote *§4.19 owes one sentence about include order ... §4.19 does not say
that group order is the author's include order and is never reordered or
thinned*, among the items *none of them this sitting's to spend. They go to
the queue*. `grep -n -i -E 'include order|group order|never reordered'
docs/design.md spec/heroes-spec.md` matches nothing today; `grep -rln -i -E
'include order|header order|order of the headers|order of two groups' issues
docs/panel` matches panel 091, panels 186 and 189's reports, panel 202's
files and defect 563 alone, so the sentence was never filed as an issue.

Panel 202 (R4, ratified 00:24) named four routes for this sitting: a
canonical header order (by bytes), a comparison of a module's meaning over
two orders refusing an order-dependent module, per-group units (its (f)), or
a sentence (its spec-warden's E1, `Declaration order never matters but to C,
which reads a file's headers in order;`, +12 on the vendored maximum,
objected there). **`jpeg` is measured against the first**: sorted by bytes,
`jpeglib.h` comes before `stdio.h`, the order that is refused. **What should
§ 4 say, what should `build` accept or refuse, and what should the messages
say, for a module whose groups' order changes what its headers mean or
whether they compile?** Widen the list: a route nobody named is the sitting's
to find.

Counted by the coordinator at 01:15 (`git ls-files '*.hero'`, each file's
distinct `extern "…"` names): **107 of 3,195 tracked `.hero` files** name two
or more distinct headers, 74 under `tests/golden`, 29 under `docs/panel`, 2
under `selfhost/cli`, 1 under `selfhost/module`, 1 under `examples/gallery`
(the list `.claude/worktrees/scratch-b15/p204/two-headers.txt`). Panel 202's
ffi-pragmatist's census of installed headers (**carried**,
`.claude/worktrees/scratch-b15/202-ffi-pragmatist/census/mac/unordered.tsv`,
read only): of 12,090 unordered pairs on this Mac, 16 fail in one order only
and 8 in both; on Linux 4 in one order only.

## The rules every seat works under

As panel 200's shared brief states them (`docs/panel/200-briefs/00-shared.md`,
§ The rules every seat works under), with your folder
`.claude/worktrees/scratch-b15/204-<seat>/`, your copy rsynced from
`.claude/worktrees/lane-panel-204/`, and **these**: the seed is ABI 30 and so
is the runtime, so your compiler is `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes` in your copy, then `./heroes build
selfhost/main.hero -o heroes` after an edit; three lanes work beside you
(`lane-b18-close`, `lane-b18-ffi`, `lane-b18-infer`): never touch them, and
lane b18-ffi changes `selfhost/cli/` and the probes (panel 202 R2), so a
prototype here is judged on this tree, not theirs. No paid run (the blind
seat is the coordinator's). **Time box**: report within 60 minutes of
starting, the unmeasured said plainly; write your report file as you go.

## Corrections and additions from the critic's first pass, binding

Read at 01:29 (`date`): `docs/panel/204-reports/completeness-critic-pass1.md`,
every one from a command it ran, the coordinator checking its repairs 1 and 3
against the tree; applied before any seat starts and binding over the text
above where they disagree. Read it whole; its routes and questions are part of
your brief. In short:
- **The include order is not the written order today** (repair 1):
  `selfhost/emit/externs.hero` `headers()` (`:101-133`) emits the headers of
  groups holding a function or a constant first, in written order, and a
  record-only group's after them; its `shapes/recfirst/rec.hero` (a
  record-only `stdio.h` group written above `jpeglib.h`) is refused *unknown
  type name 'FILE'* with the same advice, and the same program with a
  function in the stdio group builds. The sitting rules on which order: the
  written, the emitted, or another.
- **Every unit opens with the compiler's own prefix** (repair 2):
  `heroes_runtime.h` (which brings `stdbool.h`, `stddef.h`, `stdint.h`),
  `<math.h>`, `<hero_os.h>`, the guard; so `jpeglib.h` fails at `FILE` and
  not at `size_t`, a header that fails alone builds in Heroes
  (`shapes/zeroth/needsz.h`), a documented switch (`_POSIX_C_SOURCE`) can
  never come first (`shapes/sw/`), and a group with no member is refused.
- **Panel 091's owed sentence was filed** (repair 3, the coordinator's grep
  wrong): `issues/2026-09/07/2026-09-07-0000-four-repairs-to-design-md-that-ride-its-opening-sitting-4-19.md`,
  an open `task` of `M-core-packages`. Say whether this sitting closes it.
- **Panel 202 R4's condition is unmet** (repair 4): R3's instrument is
  unbuilt; this sitting convenes before it on the author's *zero defects*,
  and a prototype of a comparison route is that instrument's first build.
- `cfgone/main2` prints clang's `-Wmacro-redefined` on a correct program
  (repair 5, a `blocking` shape); the Linux census is 2,683 / 4 / **88**
  (repair 6), measured on two headers alone without the prefix, and it counts
  failure only, never two orders that compile and mean different things; the
  emitter drops a repeated header (repair 7); `one`'s message orders the two
  headers by bytes, not as written; `jpeglib.h` alone has no group to move;
  **`mpq`**, three headers, depends on order where the written order and its
  reverse agree, so no comparison of two fixed orders sees it.

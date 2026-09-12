# M-package-layout — `use` paths, the qualifier, and where a program's files live *(closed 2026-09-02)*


**The record is [journal 027](journal/027-package-layout.md), and the rulings are
panels 099, 100 and 101.** Everything below this line is the section **as it read
the day the milestone opened**, kept rather than deleted for two reasons: a later
milestone on modules inherits the same constraints, and a brief that outlives its
own milestone is the only way to see what a sitting was not allowed to re-derive.
**So read the tenses as 2026-08-25's.** The paragraph that begins *"Today a module
name is one word"* describes the state this milestone **ended**: `use syntax/decl`
compiles today. Its diagnostic did not die with it and is the one thing in that
paragraph still live — measured 2026-09-03, `module_path_has_no_parts` now refuses
the **dotted** spelling, `use geom.shapes` (`tests/golden/check/use-has-a-path.hero`),
which is the form the ruling did not take.

**Scheduled by author decision 2026-08-25** (`/decide`), taken mid-M-separate-compilation
and recorded because the expectation came first: the author expected this
milestone's work to arrive with separate compilation, and separate compilation
delivers the build architecture instead — one `.c` per module, the per-module
cache. Organising `.hero` files into directories is a **different deliverable**,
so under CLAUDE.md §14 it is a different milestone rather than an area annexed by
one that already exists.

**What it delivers.** Today a module name is one word and nesting is refused by
name: `selfhost/parse/use_line.hero:47`, `error[module_path_has_no_parts]` — *"there
is no nesting: every `.hero` a program reads sits beside the file that names
it"*. This milestone decides what replaces that refusal — the spelling of a
`use` that names a path, **the qualifier it binds** (from `use shapes/geom`, is
the module `geom`, `shapes.geom`, or `shapes/geom`?), and how a program's files
are laid out on disk.

**The flat layout is a RULING, not an oversight — `docs/panel/032`, ratified
2026-08-12**, and this milestone reopens it rather than filling a silence. That
sitting took the same question with five seats: `C` (a directory is a module) was
**vetoed three times on three independent grounds**; `A` (Nim's quoted form) was
struck as dominated; `S` (stay flat) was adopted. So the sitting this milestone
convenes is bound by what 032 already fixed, and inherits four things rather than
re-deriving them:

- **the landing form is decided in advance.** 032 R4: if the author overrules,
  what lands is the sentence the warden and the ergonomist beat into shape
  together — **+26 spec tokens measured**, never the +38 that was tabled:

  ```
  - A `use` may be a path: `use syntax/decl` binds `decl`. Last parts are unique.
  ```

  So the qualifier's headline answer is already on the record: the module is
  `decl`, not `syntax.decl` and not `syntax/decl`.
- **what R4 leaves genuinely open** is root-relative against file-relative, with
  the ergonomist's prediction attached (≥50% of `use` lines from a subdirectory
  to a peer take the root-relative spelling; a file-relative compiler
  first-try-compiles a 6-file 3-directory program ~0% of the time). That, and
  not the separator, is the sitting's real subject.
- **R5 and R6 hold whether or not paths ever land**: the C name component is the
  **whole path**, not the last part (measured — last parts collided 22-way on
  `mod` over 169 paths, whole paths zero times), and `use` is scoped to *the
  program*, which must not inherit a global scope by silence when packages
  arrive.
- **panel 031's ergonomist declined to use a subdirectory at all**, from the
  spec alone, because it could not predict the qualifier — the measured cost of
  the silence, and the reason the question is worth a milestone rather than a
  footnote.

**Two of 032's own numbers have moved, measured 2026-08-25, and the sitting
should open with them rather than with the 2026-08-12 ones.** The spec-warden's
killer argument was renames — *"D renames ≥40 of 119 files to satisfy last-part
uniqueness"* — and on today's tree **zero of `selfhost/`'s 158 module names
collide**, so last-part uniqueness costs nothing there (repo-wide, over 451
`.hero` files, 9 stems collide). And the compiler-engineer predicted
`find selfhost -name '*.hero'` **≤ 45** at M-selfhost-port's close: measured at
the `m-selfhost-port` tag it was **143**, and it is **158** today — the
prediction is falsified by more than 3×, and the *"a flat tree stays small"*
premise under `S` went with it. Neither fact decides the question; both change
which side owes the argument.

**And what ordering it before M-package-manager costs is written down rather than
discovered later.** The deferral this replaces (ratified 2026-08-12) rested on
packages creating the pressure that decides the qualifier — naming a module of
another library. Sitting first means the panel decides from precedent, from the
compiler's own source, and from the corpus, **without** a distributed package in
hand. That is the trade the author took; if the sitting finds it cannot rule
without that case, the honest outcome is a conservative default and a return
condition, not an invented one.

*******************************************************************************
**OPEN: 0**

*******************************************************************************

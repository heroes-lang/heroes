# Panel 160 — shared brief

**The question.** A value can be fallible twice — `m[k]` on a `{str: i64?}` is an
`i64??`, and `find` over a `[i64?]` hands back the same — and the four readers of
a fallible value each peel exactly ONE level. Three of them hand back a typed
`i64?` the program must still deal with. **`.is_err()` hands back a bare `bool`,
and the second level vanishes from the line.** A program written to ask *did the
stored value fail* compiles, runs, and answers *was the key there*. That is
defect 050, and it is CLAUDE.md's opening sentence falsified: a plausible mistake
that is not a compile error.

Convened 2026-09-17 at M-check-completeness, on defect 050, which panel 158's
completeness critic filed while auditing that sitting's own framing. Full panel:
a refusal is a diagnostic CLASS and the specification owes a sentence either way.

## The ground, measured 2026-09-16 on `./heroes` in this tree

On a `{str: i64?}` holding `m["a"] @ fail(code: "parse", msg: "not a number")`:

| reader, applied to `m[k]` (an `i64??`) | `check` | run | what it answers |
|---|---|---|---|
| `.is_err()` | 0 | 0 | **the outer level** — *was the key there*; the stored failure is invisible |
| `.must()` | 0 | 0 | peels one; hands back an `i64?` the program must still read |
| `?` inside a fallible function | 0 | 0 | peels one; hands back an `i64?` |
| `.default(0)` | **1** | — | `error[type_mismatch]: expected `i64?`, found `i64`` |
| `.default(ok(0))` | 0 | 0 | peels one, silently |
| `match` with `.ok inner` / `.err e` | 0 | 0 | **names both levels**; prints `absent: missing_key` |

**Read the `.default` rows together.** It is loud with `0` and silent with
`ok(0)`, so its loudness is an accident of the argument's type and not a rule
about levels. Defect 050's entry calls `.default(0)`'s noise *the place to
start*; the measurement says it is not a signal at all.

**And the asymmetry is exactly one reader wide.** `.must()`, `?` and `.default()`
return an `i64?`, which the checker will not let the program print or compare
as a number — the second level survives in the TYPE and surfaces one line later.
`.is_err()` returns a `bool`. That is the coordinator's own candidate below
(option E), offered to be tested and not adopted.

## What panel 158 adopted and did NOT land

Both of its resolutions this sitting would lean on are **unlanded**, measured:

- **R1, the parenthesis route.** `v: (i64?)? @ ok(ok(1))` is
  `error[expected_function_type]` today. A `T??` still has no written spelling,
  so a diagnostic here can only print `i64??`, a type the parser refuses.
- **R3, the first spec sentence about nesting.** `grep -n "peel\|nested\|itself
  fallible\|outer" spec/heroes-spec.md` returns **zero** relevant lines. The
  llm-ergonomist's draft from that sitting, ~26 tokens: *"`T` may itself be
  fallible — `m[k]` on a `{str: i64?}` is an `i64??` — and every operation below
  peels one."* The warden's cheapest honest merge: **+9** vendored.

## The corpus, counted

- `grep -rn '{[a-z_]*: [a-zA-Z0-9_]*?}'` over `examples/ tests/golden/ selfhost/`:
  **2 files** — `selfhost/check/table.hero` (a comment written last night) and
  one golden. **Zero** in `examples/`. Panel 158 counted four, all in
  `tests/golden/check/`, with a wider regex.
- `find(` sites: **6** in `examples/`, **3** in `selfhost/` outside the library
  source. How many are over a `[T?]` is **UNRUN** — a grep cannot see types.
- `].is_err(` / `].must(` / `].default(` on a map or array read: **100** sites,
  **72** in `examples/`. With zero `{K: V?}` in `examples/`, those 72 are on a
  single-level `V?` and any rule about NESTED values would touch none of them.
  **That is an inference about the 72 and a seat should run it.**
- `find<A>(xs: [A], f: (function(A) -> bool)) -> A?` is
  `selfhost/library_source.hero:106`; it is the only generic in the library that
  WRAPS what it bound (panel 158).

## Two facts that narrow the option set

- **Heroes has no warning.** `grep -n "warning\|severity" selfhost/diag.hero`
  returns nothing; the `warnings` suite judges clang's warnings on emitted C.
  Every diagnostic here is an error. *"Warn about it"* is not on this ballot.
- **The outer failure of a map read has a name.** `runtime/parts/failure.c:74-76`
  returns a STATIC failure with code `"missing_key"`; `match` above printed it.
  A rule or a message can lean on that code existing.

## The readers, where they live

`selfhost/check/builtins.hero`, `fallible_builtin`: three arms, `must`,
`default`, `is_err`, each matching `.fallible o` and taking `o.payload` once.
`?` is `try_type` in `selfhost/check/access.hero`, the same one-level peel.
None of the four asks what the payload IS.

## What the sitting is asked to decide

- **R1.** Which repair, or one nobody listed:
  - **A.** Refuse all four readers on a value whose payload is itself fallible;
    `match` is how a program names both levels. Closes both doors.
  - **B.** Refuse `{K: V?}` at the declaration — panel 158's option 1, which that
    sitting measured as protecting zero corpus programs; the critic answered that
    it protects the one program measured silent. Closes the map door, leaves
    `find`.
  - **C.** A distinct spelling for the outer question on a map read. A surface
    change; leaves `find`.
  - **D.** Land 158's R1 and R3 and change no behaviour: the document says the
    readers peel one. Not a refusal, so not the thesis's answer.
  - **E.** Refuse **`.is_err()` alone** on a nested fallible, because it is the
    one reader that discards the payload's type. The other three surface the
    second level in what they return.
- **R2.** Does the repair depend on 158's R1 landing first — can a diagnostic
  honestly print `i64??`, a spelling the parser refuses, or must it wait for
  `(i64?)?`?
- **R3.** Principle 0: does the compiler itself, `selfhost/`, ever apply a
  reader to a nested fallible? If it does, A or E breaks the bootstrap and the
  count is the price. **No seat has measured this.**
- **R4.** What does the specification owe — 158's +9 sentence, more, or a
  sentence about the refusal — and what pays for it? Budget 2026-09-16:
  **6004** vendored, **7998** real (`claude-opus-5`), ceiling 10240, **2242 free**
  and 2182 net of the FFI floor.

## Process rules binding every seat

- **Write your report file FIRST**, to `docs/panel/160-reports/<seat>.md`, then
  improve it in place. No seat has been lost to the watchdog in four sittings
  under this rule.
- **No command over ~60 seconds.** Never run the full net. The seed builds in
  a few seconds: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`.
- **Every number, path and count you report is produced by a command you run**
  (CL-077). **Nine claims in this coordinator's briefs have been corrected by
  seats across five sittings; assume this one carries a tenth.**
- Anything you cannot run is written as **UNRUN**, naming the command.
- **Build in a copy**: `cp -r` the tree to your scratchpad, `rm -rf target build`.
  The repository working tree is frozen for this sitting.
- Never read `archive/bootstrap-rs/`. Capture exit codes with `$?` directly,
  never through a pipe — a pipe returns the LAST command's code.
- **A probe whose subject is not read is `unused_binding` and measures nothing.**
  Two coordinators' probe rounds this milestone read that error and mistook it
  for a class. End every program with a read of what it declares.

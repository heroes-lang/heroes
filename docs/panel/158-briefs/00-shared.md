# Panel 158 — shared brief

**The question.** `heroes check` accepts programs whose types have no spelling.
A doubly-fallible value — `i64??` — is tracked by the checker, printed by the
compiler, and **refused by the parser**. What should the language do?

Convened 2026-09-16 at M-check-completeness. Full panel: this is a surface
question with a diagnostic and a spec sentence behind it.

## The measured ground, and none of it is a guess any more

The item docketed four measurements at panel 110 (2026-09-04) and said none had
been made. **All four were made on 2026-09-16 and they narrow this sitting
sharply.**

| what was asked | measured |
|---|---|
| is the representation stable under monomorphisation? | **yes** — `--dump-ir` names it in full: `found: i64??`, `$t8: i64?? = call heroes find(…)` |
| does `?` peel the outer level in the emitted C too? | **yes** — emits at exit 0, the C carries the fallible machinery |
| is `hero_runtime_check_leaks()` clean on a nested fallible carrying a `str`? | **yes** — `{str: str?}` with an allocating payload, exit 0 |
| which library functions produce one? | **ONE of six.** `find<A>(xs: [A], f) -> A?` is the only generic that WRAPS what it bound; `map`/`filter` return `[B]`/`[A]`, `fold` returns `B` bare, `any`/`all` return `bool` |

**So this is CONSISTENCY and not safety.** Nothing leaks, nothing corrupts, the
representation is stable, and the surface through the library is one function.
That is the same footing as `float_map_key` through a generic, which panel 155
R3 left waiting under Principle 0.

## What ships today, re-measured 2026-09-16

```
function main()
    m: {str: i64?} @ {}
    m["a"] @ ok(1)
    v = m["a"]
    print(v.must().must())
```

`check` 0, `build` 0, run 0, prints `1`. The `find`-over-`[i64?]` route is the
same: 0, 0, 0, prints `1`.

And the four facts the item recorded on 2026-09-04, unchanged on the code:

- `function take(x: i64??)` is `error[nested_fallible]` — the **written** form
  is refused, at `selfhost/parse/type.hero:51-58`;
- `function take(x: i64?)` handed that value is `error[bad_operand]`, so the
  checker knows the real type;
- the compiler **prints a type it refuses to parse** — `error[type_mismatch]:
  expected i64?, found i64??` — which is §4.17's own failure, a message that
  cannot be acted on;
- a user generic nests without bound: `wrap<T>(x: T) -> T?` twice gives
  `i64???`.

`spec § 10` promises `m[k]` returns `V?` unqualified.

## The four repairs priced at panel 110, one vetoed

1. **Refuse `{K: V?}` at the declaration** — ~30 lines, breaks 0 programs,
   closes one of three doors.
2. **Flatten `m[k]` — VETOED on soundness**, because it collapses *key absent*
   and *key present, value failed*: a stored `fail("parse", …)` becomes
   indistinguishable from a missing key and the program takes the wrong branch
   at exit 0.
3. **Make written `T??` legal and delete the parse refusal** — the only option
   that closes all three doors, and the only one §1.7 favours because it
   **removes** a special case, at the cost of a catch the thesis may want.
4. **Leave it and qualify the spec.**

## What the sitting is asked to decide

- **R1.** Which of the four, or a fifth nobody has listed? CLAUDE.md § RUN IT
  says a recommendation is a claim about the option **set**.
- **R2.** `nested_fallible` is **absent** from `is_thesis_rule`
  (`selfhost/diag.hero:88-104`), so `--permissive` does not drop a refusal the
  checker computes past happily. Is that a defect, and what is Part 11's control
  arm supposed to contain?
- **R3.** The compiler prints `i64??` in a diagnostic and refuses to parse it.
  Whatever R1 decides, is printing an unspellable type acceptable, and if not
  what does the message say instead?
- **R4.** Does the measured *consistency, not safety* footing change the answer?
  Panel 155 R3 left the twin waiting on exactly that ground.

## Process rules binding every seat

- **Write your report file FIRST**, then improve it. No seat died at panels 156
  or 157 under this rule; three of five died at 155 without it.
- **No command over ~60 seconds.** The seed builds in a few seconds; rebuilding
  the compiler from `selfhost/` is **59 s measured**. Never run the full net.
- **Every number, path and count you report is produced by a command you run**
  (CL-077). Anything you cannot run is written down as **UNRUN**, naming the
  command that would settle it.
- **Build in a copy**: `cp -r` the tree to your scratchpad, `rm -rf target
  build`. The repository working tree is frozen for this sitting.
- Never read `archive/bootstrap-rs/`. Capture exit codes directly, never through
  a pipe.

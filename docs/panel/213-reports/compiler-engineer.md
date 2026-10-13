# Panel 213, the compiler-engineer's report

Written 2026-10-13 from 00:09, in my own copy
`.claude/worktrees/scratch-panel-213/compiler-engineer` at `0b8307daf`
(`git log --oneline -1`), its compiler built from the seed (`clang -I runtime
seed/heroes.c runtime/runtime.c -o heroes`, real 8.05 user 7.71). In progress;
sections fill as they are measured.

## Running log (as measured)

- 00:13. **Hand pipeline proving a DWARF-only rename** (`build/p/`):
  `heroes build locals.hero -O0`, then the unit's emitted C with
  `__attribute__((nodebug))` added by `sed` to every `t<N>` and to the two
  `$own` slots, `clang @words -S -emit-llvm`, a `sed` on the `.ll` turning
  `DILocalVariable(name: "h<k>_` into `name: "` and a member's `"f_` into `"`,
  `clang @words -c` of the `.ll`, relinked with the build's own
  `link-words.rsp`. The binary prints `ada6` and `7` as before; lldb shows
  `(h_locals_Point) p = (x = 3, y = 4)`, `label`, `width = 6`, and `p p.x +
  width` answers `(int64_t) 9` (today: `error: use of undeclared identifier
  'p'`). Today's frame of `describe` lists 15 locals, `h0_p` to `h4_own4` and
  `t1` to `t10`, two of them reading garbage (`t5 = (ptr = "", len =
  6806875720)`).
- 00:15 to 00:18. **Route F built in the emitter**: `selfhost/emit/prologue.hero`
  +11 -2 lines, `__attribute__((nodebug)) ` before every temporary and before a
  `synthetic_slot` or `return_slot` (the `match slot.kind` reads the IR's own
  distinction, so the `$` need not survive the mangler). Compiler rebuilt from
  `selfhost/` (`build/heroes-next`). Cold build of `selfhost/main.hero`,
  each in its own empty directory, `/usr/bin/time -p`: **before real 69.91
  user 54.91 sys 11.08; after real 70.04 user 54.52 sys 11.16** (load 2.7 to
  3.2 both times; real ~ user+sys, kept). Over the 619 units: emitted C
  **117,979,663 -> 126,671,996 bytes (+7.4%)**, 347,551 marks of 25 bytes;
  objects **42,860,312 -> 36,818,512 bytes (-14.1%)**, the variables' DWARF
  gone (`wc -c`, `grep -o | wc -l`). `seed/heroes.c` is 52,126,543 bytes today.
- 00:19. **042's `shapes.hero` built by that compiler**: `frame variable` at
  :30 lists **11 locals, 111 before** (042 § 5 (c)): the author's ten and the
  match binding `n`, not yet bound, reading `f_v = 6809745884`, as it does
  today. With the hand DWARF rename on top: `count = 7`, `name`, `p = (x = 3,
  y = 4)`, `t = { tag = num  as = { num = (v = 42) } }`, `room = { name = ...
  corner = (x = 9, y = 10) }`, `p p.x + count` = 10, `p room.corner` =
  `(x = 9, y = 10)`; `names`, `points`, `ages` still `HeroArrayHeader *` /
  `HeroMapHeader *` addresses and `good`/`bad` still both arms
  (`ok = 4295075736` on the error).
- 00:20. **Two slots of one spelling, after the rename** (`build/dup/dup.hero`:
  a `match` whose two arms both bind `f`, `.circle f` and `.square f`). The
  prologue declares `h2_f` and `h3_f` in one C scope; renamed in the debug
  information to two `f`, lldb stopped in the **square** arm (:10) answers
  `p f` with `(h_dup_Shape_c_circle) (r = 6809873972)`, the circle arm's slot:
  **a wrong value under the author's name, at exit 0.** So a rename keeps the
  index on every spelling two slots of one function share.
- 00:37. **The rename's compile cost**, every one of the 619 units of the
  compiler, serially, `/usr/bin/time -p` around a loop: `clang @words -c`
  **real 28.52 user 22.18 sys 4.52**; `-S -emit-llvm`, the `sed`, `-c` of the
  `.ll`: **real 66.85 user 44.30 sys 14.90**, the load rising to 9.93 by its end
  (another session's build), so its `real` is discarded and its CPU, 59.2 s
  against 26.7 s, is the number: **about 2.2 times the clang stage of a cold
  `-O0` build**. Objects 36,818,512 against 36,677,640 bytes.
- 00:39. **A bare C name is refused on Linux**: `-std=gnu11` is the compile's
  first word (`selfhost/cli/flags.hero:132`); `heroes check` accepts
  `linux = 1` and `unix = 2` (exit 0); in `heroes-linux-arm64:latest`, `clang
  -std=gnu11` refuses `long linux = 1;` with *expected identifier or '('*,
  *expanded from macro 'linux'*, `#define linux 1` (`docker run`). On this Mac
  the runtime header's lowercase macros are `bool`, `false`, `true` and one
  function-like (`clang -dM -E` over `#include "heroes_runtime.h"`).
- 00:39. **`f_` is a C convention, not only the mangler's**: the SDK's
  `sys/mount.h` declares `f_bsize` and `f_flags` (lines 106, 116, 143), so a
  rename that strips `f_` from every member renames a C header's field too.

## Verdict

- `verdict`: **object**, with a soundness **veto** on two shapes of the routes:
  route E carried by **C tokens** (the author's name written into the emitted
  C), and any route that turns `h<k>_x` into `x` **without the one-spelling
  rule**, a per-build name map included. Both show a wrong name or value, or
  refuse a correct program, at exit 0, measured above.
- `section`: design.md §1.1 and §1.7 (the ceiling: core plus elaboration), and
  §1.12 (robustness) for the veto; CLAUDE.md § 10 for B's file, § Precedence
  rank 5 (*never slow the compiler down*) for E's cost. **Part 5's core/sugar
  table does not cover a tool surface**: nothing here touches the lexer, the
  checker or the IR, so no route is core (`grep -n -i -E "debug|lldb"
  spec/heroes-spec.md`, nothing, per the critic's § 6).
- `needed_for_self_hosting`: **no** (Principle 0; the compiler compiled itself
  without any of it). It enters on the thesis side or not at all.

## The route I adopt, and the route I refuse

**Adopt F in the emitter, now, and E through the debug information alone.**

1. **F, `__attribute__((nodebug))` on every temporary and every
   `synthetic_slot` and `return_slot`**, built and run above: the frame goes
   from 111 locals to the author's 11 (042's `shapes.hero` at :30), the
   build's time does not move (69.91 against 70.04 real), the objects shrink
   14.1%, the emitted C grows 7.4%. It cannot show a wrong value: a variable
   absent from the debug information has no value to show. Clang-only, which
   is the C compiler the project drives (CLAUDE.md's opening: *emits C11
   compiled by clang*); gcc would warn on an unknown attribute (unrun).
2. **The `$` distinction does not have to survive the mangler.** The prologue
   holds `slot.kind` where it writes the declaration
   (`selfhost/emit/prologue.hero`, the slot loop), so the decision is made
   there and nothing is kept past it: **no slot table, no new prefix, nothing
   beside the binary.** Measured: what remains is exactly the author's slots.
3. **E, names in the debug information and never in the C**: the hand
   pipeline above shows `p = (x = 3, y = 4)` and `p p.x + width` answering 9.
   It is sound by construction on three conditions, each measured as
   necessary above: (a) a spelling two slots of one function share keeps its
   index (`dup.hero`); (b) members are renamed only in the types the emitter
   made (`h_` names), never by the `f_` prefix alone (`sys/mount.h`); (c) a
   `ph<k>_` parameter, which is a pointer to the caller's place, is either
   left as it is or shown dereferenced, never renamed to a pointer under the
   value's name. **What it costs is the compile**: 2.2 times the clang stage of
   a cold `-O0` build, and a premise about clang's textual IR
   (`DILocalVariable(name: "`), which fails safe (the names stay mangled when
   the pattern is absent) but is unrun on the Linux and Windows toolchains.
   **I adopt E only if that cost is paid by the builds that want it** (a
   debug build the editor and a `run --debug` ask for, D), and not by every
   `heroes build`, whose default is `-O0` (`selfhost/cli/table.hero`).

**Refuse E through C tokens** (veto, soundness): `linux = 1` is a correct
Heroes program that clang refuses on Linux under the project's own `-std=gnu11`
(above); and a func-ref local called by name would meet any function-like macro
an FFI header defines (an inference, unrun). Making it impossible would need
the unit's whole preprocessor namespace, which the emitter cannot know once a
package names a header. **Refuse B as a carrier of names or slots** (veto,
soundness, unrun as a program): a table written per build and loaded into a
debugger session can meet another build of the same path, and the only guard
is a build identity check nobody has priced. **Object to C on the ceiling**: an
assembler for lldb's formatter section in the emitter, its lldb versions
unknown (the critic's `strings` hint is not a measurement).

**The shapes ([T], {K: V}, T?, a variant's live arm) are not reached by E or
F**; they need a formatter or emitted types. Of those, I accept only one that
reads everything from the stopped process and the binary's own debug
information (G), and that shows an address, never a guess, where the
descriptor's symbol is gone (`strip`, the critic's measure). Its lines are
outside `selfhost/` (a second language the compiler's own tests cannot
check), which is the § 10 question for the sitting, not mine to rule.

**Every route keeps one wrong value**: a binding not yet reached reads
garbage (`n = (v = 6809745884)` above, 042's `made`). Only a live range per
slot threaded through the lowering, or C block scopes the goto-shaped bodies do
not have, removes it; that is the dearest item on the table and I price it as
not this milestone's.

## `implementation_cost`, by file

| route | where it lands (today's lines, `wc -l`) | delta |
|---|---|---|
| F | `selfhost/emit/prologue.hero` (215) | **+11 -2, built and run**; production: a `HERO_NODEBUG` macro in `runtime/heroes_runtime.h` (825), ~4 lines, halving the 8.7 MB of marks (estimate); seed churn on every prologue line; `tests/golden/lldb/locals.expected` does **not** move (its `!shows:` lines all still print, run at 00:41 with the prototype: `h0_p`, `h1_label`, `h2_width`, then `h0_count`, `h1_name`, `h2_p`); E moves it |
| `$` distinction | none past `prologue.hero` | 0 (the `match slot.kind` above) |
| E (debug info) | `selfhost/cli/units.hero` (574), the compile step at 150 to 215 becomes two clang calls at `-O0` plus a rewrite module, est. 120 to 160 lines (**estimate, unbuilt in Heroes**; the hand `sed` is 3 expressions) | +2.2x clang CPU per cold `-O0` unit, measured |
| E (C tokens) | `selfhost/emit/mangle.hero` (265) `slot` and its callers (`places.hero` 260, `signature.hero` 165, `lend_local.hero`) | vetoed |
| D | `selfhost/cli/table.hero` (266), `selfhost/cli/flags.hero` (493) | a `run` flag implying `-O0`, est. 20 to 40 lines (unrun) |
| B / C / G | outside `selfhost/` for G; a generator in `selfhost/emit` or `cli` for B; an assembler for C | unpriced in lines; B and C refused above |

For scale (§1.1's ceiling): `selfhost/` is **618 files, 161,393 lines** (`find
selfhost -name '*.hero' | xargs cat | wc -l`); Pascal-P4's 4,000 is long past,
so the ceiling binds on the delta, and F's is the smallest change on the table
that moves the frame the most.

## `prediction`

At M-typed-inspection's close, if F lands as priced: the `lldb` suite's frame
of `describe` in `tests/golden/lldb/locals.hero` lists exactly **3** locals
(15 today, measured above) and 042's `shapes.hero` at :30 exactly **11** (111),
and `seed/heroes.c` grows by **at most 8%** from 52,126,543 bytes (`wc -c`),
at most 4% with the macro. If E lands through a second clang invocation on
every `-O0` build, a cold `heroes build selfhost/main.hero` measured on a still
machine costs **at least 25% more user+sys** than the 66.0 s of today's
(54.91 + 11.08 above).

## `condition`

I would withdraw the objection to E's cost on a measured single-invocation
route to the author's names in the debug information (a clang flag or
attribute I did not find), or on lldb and lldb-dap renaming a frame's
top-level variable without the debug information: I searched `help frame
recognizer add`, which adds recognised arguments to named functions, and
nothing else; that negative is a question, not a finding. I would lift the veto
on C tokens on a proof that the emitter knows every macro a unit can see,
which an FFI header rules out today.

## Not run

- E built into `selfhost/` (only the hand pipeline ran); its cost on Linux and
  Windows clang, and whether their `-S -emit-llvm` spells `DILocalVariable`
  the same; CodeView on Windows (the `.ll` rewrite is format-independent by
  reasoning, unrun).
- G, B, C and D as programs; the Linux image's lldb; lldb-dap's Variables view
  with F and E (the editor's premise stays a question).
- The full net, the `lldb` suite and the emission suites against the F
  prototype; the fixpoint. The `locals` case's two frames were run by hand
  with it (3 locals each, the six names its `.expected` pins).
- The function-like macro capture of a func-ref local (inference).

## The prototype, kept (the scratch copy is removed)

`git diff` of the copy at `0b8307daf`, formatted by `heroes fmt --in-place`:

```diff
diff --git a/selfhost/emit/prologue.hero b/selfhost/emit/prologue.hero
index e98462dc1..673f76ec6 100644
--- a/selfhost/emit/prologue.hero
+++ b/selfhost/emit/prologue.hero
@@ -141,7 +141,14 @@ function written(c: state.Checked, names: ctype.Names, f: containers.Function, l
 
             if !slots_read[index]
                 deliberate @ "__attribute__((unused)) "
-            lines @ lines.push(deliberate + spelled.must() + " " + name + initialiser + ";")
+
+            # Panel 213 prototype: a slot the lowering invented is kept out
+            # of the debug information.
+            hidden = match slot.kind
+                .synthetic_slot | .return_slot => "__attribute__((nodebug)) "
+                .param_slot | .local_slot      => ""
+
+            lines @ lines.push(hidden + deliberate + spelled.must() + " " + name + initialiser + ";")
 
             if by_bytes
                 lines @ lines.push("__builtin_memset(&" + name + ", 0, sizeof " + name + ");")
@@ -209,7 +216,9 @@ function written(c: state.Checked, names: ctype.Names, f: containers.Function, l
 
                 if !lends[value_at].is_err()
                     spelling @ "const " + spelled.must()
-                lines @ lines.push(deliberate + spelling + " " + mangle.value(value_at) + ";")
+                lines @ lines.push(
+                    "__attribute__((nodebug)) " + deliberate + spelling + " " + mangle.value(value_at) + ";"
+                )
         value_at @ value_at + 1
 
     return Prologue(lines: lines, defines: defines)
```

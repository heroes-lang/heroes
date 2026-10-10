# Panel 203, spec-warden

Copied by the coordinator at 23:27 on 2026-10-09 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

I'm the spec-warden, panel 203. Started at 23:06:29 and finished at 23:26:50 by `date`. I worked only in `.claude/worktrees/scratch-b15/203-spec-warden/`. Nothing in the frozen tree was written, no `--refresh` was run, and nothing was paid for.

**How it was built and measured.** I built the compiler in the three stages the brief asks for (`build.log`). Stage 3 and stage 2 gave the same verdict on every probe. `heroes measure` on the base spec reads 7,348 on the legacy table and 7,479 on cl100k, which are lower bounds. The pinned real count is **9,847** (claude-opus-5, 2026-10-09). That leaves 393 below 10,240, and 333 once the 60-token FFI floor is taken out.

I also tested against the compiler-engineer's prototype of the general route. I copied its two binaries (read only) into my folder as `heroes-q1-v1-copy` and `heroes-q1-v2-copy`, and ran my 45 probes through both.

## Q1: what spec § 9 says if the general route lands

- `verdict`: **approve, provisional.** The new sentence is G3a (below), replacing N1f. I **object** to G0, which just removes N1f, as the text to land.
- `section`: design.md §1.6, §1.2, §1.0 (Principle 0) and §4.12. CLAUDE.md § 12 too: when spec and compiler disagree, the spec wins.
- `spec_token_delta`, against 9,847 real (7,348 legacy, 7,479 cl100k):

| draft | legacy | cl100k | real |
|---|---|---|---|
| G0 (N1f removed) | −11 | −12 | **9,831, so −16.** G0 is byte-identical to the text before panel 201 (`cmp` against `a9e557db^`), and the seed's pin prints 9,831 for those exact bytes. |
| **G3a** | **+4** | **+5** | unmeasured |
| G3c | −3 | −4 | unmeasured |
| J1 | +5 | +5 | unmeasured |
| G3b | −6 | −7 | unmeasured |
| G2 | −3 | −4 | unmeasured |
| U1 (receiver clause) | +12 | +13 | unmeasured |

  The spec suite with G3c applied to my copy gave 19 passed and 4 failed. All four are the pin checks (budget, spendable, real, ledger), which fail because the document moved and its pins did not.
- `removal`: N1f itself. **But N1f's tokens do not come back.** A true sentence for the route has to say what order a literal takes its type in, and that costs about what N1f did. The only draft that returns them is G0, and G0 leaves m1 and f3 (below) to the reader's guess.
- `needed_for_self_hosting`: **no.** The compiler built itself at stages 2 and 3 under today's "flat" rule.
- `argument`: On route v2, 23 of my 45 probes go from refused to accepted. None goes from accepted to refused. 10 of 10 programs accepted today print the same output as before. Among the newly accepted is § 9's own example shape, `xs.fold(0, add)`, over a `[u8]` (f1). Today that program is refused, and the same message is printed twice at one place. What a literal argument does is the deciding point. The route accepts `y: u8 = first(a: 1, b: 2)` (m1), because it consults the context before falling back to the literal's `i64`. G3a says exactly that. G0 does not.
- `prediction`:
  - P1: at the landing's `--refresh`, G3a reads 9,849 to 9,860. G0 reads exactly 9,831.
  - P2: on the route that lands, G3a holds for `check` on all 45 probes (r1 depends on condition b below).
  - P3: in the census, no file goes from accepted to refused, and no `run` golden changes its output.
  - P4 (blind): in the G3a arm, 2 of 2 readers predict m1 accepted as `u8`. In the G0 arm, at most 1 of 2.
- `condition`:
  - **(a) f3 decides the sentence.** f3 is `xs.fold(0, keep)` over a `[u8]`, where `keep<T>(acc: T, item: T)`. Route v1 lets a generic function passed as an argument tie the type parameters together, and accepts f3. With v1, G3a is false on f3 and J1 is the sentence instead. Route v2 refuses f3, and G3a is true.
  - **(b) The value before the dot gets no type.** r1, `[].count()` where `count` is not generic, is refused today and on both routes, while `count([])` (r2) is accepted. So § 9's line "`x.f(y)` is sugar for `f(x, y)`" is already false today, whatever happens to N1f. Defect 551's repair defers exactly this to a sitting on that line. Either the route covers it, or U1 is owed.
  - **(c) The compiler's messages quote N1f.** `generic_argument.hero:172` repeats "a generic function's parameter asks for none". `function_value.hero:224` and the goldens 402, 415 and *a-message-never-shows-a-table-index* say "this position asks for none". These texts change in the same commit as the sentence.
  - **(d) design.md §4.12** is amended with the draft below, recording that panel 105 is partly reversed (repair 16).
  - **(e)** If P3 is broken, my verdict becomes an objection.

**The drafts, whole.** Each replaces lines 277 to 280 of spec § 9. Every file is in `drafts/`.

G3a (recommended for v2):
```
  A type parameter takes its type from the arguments that carry one, else from
  the type the context asks for, else from a literal argument (section 2); a
  call that says none of these is an error. `xs.map(double)` takes both from
  `double`'s signature.
```
J1 (if v1 lands):
```
  A type parameter takes its type from the arguments, a generic function's
  signature included, else from the type the context asks for, else from a
  literal argument; a call that says none of these is an error.
  `xs.map(double)` takes both from `double`'s signature.
```
G3c (the cheaper option; less clear on f3):
```
  A type parameter takes its type from the arguments, else from the type the
  context asks for, else from a literal argument; a call that says none of
  these is an error. `xs.map(double)` takes both from `double`'s signature.
```
G0 (the conservative option, the text from before panel 201):
```
  A type parameter takes its type from the arguments, else from the type the
  context asks for; a call that says neither is an error. `xs.map(double)` takes
  both from `double`'s signature.
```
U1 (only if the route leaves the receiver out; it replaces the opening of the UFCS bullet):
```
- `x.f(y)` is sugar for `f(x, y)` (UFCS), but `x` takes no type from `f`.
  There are no methods, no
```
design.md §4.12: the replacement for lines 1801 to 1808 is in `drafts/d412_new.txt`. It applies exactly once (checked with `drafts/design_d412.md`).

**Correction to the critic's repair 17.** m2 is `y: u8 = first(a: 200, b: 2) + 100`. On both v1 and v2 it stays **refused**; it does not become an overflow abort. The reason is that today the type an annotation asks for does not reach the operands of `+`.

**Found beside the question, and unfiled as far as my grep of issues/, docs/panel/ and design.md goes:**
- `y: u8 = 2 + 3` (w9) and `show(v: 2 + 3)` (w11) are refused with "expected `u8`, found `i64`". Spec § 2 and design.md:968-969 both say a literal takes the width its context asks for. This is the same kind of problem as Q1, one level out: an operator's operands get no context.
- The f1 message is printed twice at one place. f4, a genuine mistake, is told three times today; both routes tell it once.

## Q2: does any sentence change

- `verdict`: **approve: no sentence changes, 0 tokens.** I **object** to Q2a, `- Recursion too deep aborts; one that can never end is an error.` (+9 on both tables), and to Q2b (+13 on both).
- `section`: design.md §1.6. Panel 199 R2: unbounded recursion is an abort, at every level.
- `spec_token_delta`: 0.
- `removal`: nothing needed.
- `needed_for_self_hosting`: no.
- `argument`: "Recursion too deep aborts" is true of all three shapes on stage 3. param_value, viafield and viamap each pass `check`, build at 0, and run to 134 with "stack exhausted". wayout runs to 0. Q2a would be false on viafield and viamap even after a parameter-following rule: the engineer's notes say their route does not follow those two. Panel 201 R3 already landed this kind of refusal with no sentence.
- `prediction`: on the engineer's route, `check` passes viafield and viamap and they still abort with 134.
- `condition`: I would price a sentence only if a rule refused every shape that can never end. None does.

## What I could not run

- The real count of any draft except G0. That would take `--refresh`, which the brief rules out.
- The blind arms and the census. Those belong to the other seats.
- The `fixes`, `grammar`, `unseen` and `records` suites on any draft, and the `spec` suite on G3a and J1.
- Exit codes from the overflow runs of f1o and f2o. I captured them wrongly inside a substitution; both print "panic: integer overflow".
- Both prototypes are the engineer's work in progress. Every result taken from them gets scored again on the route that actually lands.

Files are in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/203-spec-warden/`:
- `notes.txt`
- `drafts/`
- `probes/`
- `check_today_stage3.txt`, `check_q1v1.txt`, `check_q1v2.txt`
- `suite_G3c_spec.txt`

# Panel 201, spec-warden

Copied by the coordinator at 20:20 on 2026-10-09 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 201, spec-warden's report

**Recommendations:** for Q1, approve the narrowing N1f (+12 tokens on the vendored maximum) and veto route I on Principle 0. For Q2, object to every rewording, so line 22 stays. For Q3, approve "no sentence", conditionally. I ran from 19:55 to 20:20 (`date`) on my copy of `36be56d0`, with the compiler built from the seed.

**The count.** I grepped design.md §1.6 (`docs/design.md:255-256`): the ceiling is 10240, `claude-opus-5` through `count_tokens`, and the payment rule is unconditional (`:311-314`). `./heroes measure spec/heroes-spec.md` read claude-legacy 7337, cl100k 7467, maximum 7467 (a lower bound, not the reader's tokeniser), and the pinned real count 9831. 9831 plus the 60-token FFI floor is 9891, so **349 tokens are free**.

Every delta below is on the vendored row and is **a lower bound, in those words**. No real count was taken, and there was no `--refresh`. Where I scale a delta by the document's ratio of 1.317, that is an inference.

## Q1 (488)

- `verdict`: **approve N1f, provisional**. **Veto route I** (the compiler moves, 0 tokens) on Principle 0.
- `section`: §4.12 (`docs/design.md:1802-1807`), §1.6, §1.2, Principle 0.
- `spec_token_delta`: N1f is +11 legacy, +12 maximum (7467 to 7479). Real unrun, about +16 by ratio (inference). Budget after it is about 333 of 349 (inference). Route I is 0 tokens only if all 14 refused probes become accepted.
- `removal`: nothing, and that is a problem. The one candidate is *never written at the call site*, and it is load-bearing: `ident<i64>(3)` gives `error[unknown_name]: nothing named 'i64' is in scope`, a message that never names the rule.
- `needed_for_self_hosting`: no. `./heroes check selfhost/main.hero` exits 0 under the current flat rule.
- `argument`: "Read plainly" is not the language. §4.12 rules "Flat only: a generic callee's arguments are synthesised", panel 105 ratified it, and the spec sentence is that sitting's own transcription with the clause left out (ledger row 3750). The same rule refuses nine shapes the card never names: `ok(…)`, a result-only call, a literal, and even a concrete parameter of a generic function. So route I re-rules §4.12, which is exactly the case panel 105's historian said he would object to. It also has no compiler need and no measured rate. The blind seat measured the prompt misleading: 2 of 2 readers predicted *accepted*. N1f states the ratified rule inside the existing sentence, in the diagnostic's own words.
- `prediction`:
  - P1: real delta +13 to +20 at the landing's `--refresh`; above 24 the wording is re-argued.
  - P2: N1f predicts `check` on every probe (14 refused, 5 accepted), scored at the landing.
  - P3 (an observation, it needs a paid run): a fresh reader given N1f predicts *refused* on `app(f: ident, x: 20)` and `ns.map(ident)` in 2 of 2.
- `condition`: I would lift the veto for a measured Part 11 effect or a closure-list program that needs nesting. N5 becomes the wording if the compiler starts giving context through a generic function's concrete parameters.

What I measured with `check` (`probes/q1_*.hero`):
- **Refused:** `app(f: ident, x: 20)`, `ns.map(ident)`, `ns.fold(19, keep)`, `ns.filter(keep)`, `ns.map(helper.ident)`, `first(a: y, b: ok(2))`, `x: i64? = first(a: ok(1), b: ok(2))`, `first(a: x, b: wanted("i"))`, `first(a: b, b: 255)` with `b: u8`, `g(x: 1, n: 255)` with `n: u8` (a concrete parameter), `g(x: 1, n: ok(2))`, `g(x: 1, f: ident)` with a concrete function type, `7.g(255)`, and `first(a: xs, b: [])`.
- **Accepted:** a record field, a `return`, a non-generic parameter, an annotation, the built-in `push` (`fs.push(ident)`, `xs.push(255)`), and `first(a: ident(1), b: 2)`.

On the drafted copy (N1 plus B4 together, +12):
- `spec`: 19 passed, 4 failed. The four are `budget`, `spendable`, `real` and `ledger`, which move by design.
- `fixes`: 916 passed, 0 failed.
- `grammar`: 9 passed, 0 failed.

Route I would move `tests/golden/check/fixedbugs-402-…hero:53` (`ys = xs.map(ident)  #~ cannot_infer`) and the value fix text that `function_value.hero`'s own test pins.

**N1f, recommended, the whole bullet:**
```
- Generics: on functions only, no constraints, always inferred, never written
  at the call site: `function map<A, B>(xs: [A], f: (function(A) -> B)) -> [B]`.
  A type parameter takes its type from the arguments, else from the type the
  context asks for, and a generic function's parameter asks for none; a call
  that says neither is an error. `xs.map(double)` takes both from `double`'s
  signature.
```

The other drafts, each replacing the third and fourth lines of the bullet (legacy / maximum):

| draft | text | legacy / maximum | note |
|---|---|---|---|
| N1 | `, and a generic function's parameter asks none;` | +10 / +11 | |
| N3 | `, which a generic function's parameter never does;` | +10 / +11 | |
| N4f | N1f plus `a call or a function value that says neither` | +15 / +16 | |
| N5 | `, and a parameter whose type holds the callee's own type parameter asks for none;` | +18 / +18 | only after a compiler repair |
| V | `A generic function used as a value takes no type from a generic function's parameter.` | +18 / +19 | leaves the spec false on 9 shapes |
| C (critic's route) | `, and a generic function's parameter asks one only of a function value;` | +15 / +16 | |
| N2 | N1 plus `` and `xs.map(ident)`, `ident` generic, is an error. `` | +27 / +29 | |
| blind seat's sentence | as its own bullet | +40 / +42 | **false**: an annotation and `push` also type the value |

## Q2 (467)

- `verdict`: **object** to every rewording. The sentence stays, 0 tokens.
- `section`: §1.2, §1.6, Principle 0; `docs/design.md:3744` ("this language has no warning level").
- `spec_token_delta`: 0 if it stays. B4 is +1 / +1, A is +8 / +8.
- `removal`: none is needed at 0.
- `needed_for_self_hosting`: no.
- `argument`: The 2 of 12 are not a misreading. Measurement 040's task said *compile*, the module was the file being compiled, and `build` itself says so: `error[no_entry_point]` … "add `function main()`". Both additions checked clean and changed no meaning (040 `:208-209`). A used module's `main` is ignored at build (my probe: exit 0, prints 25), and the tree holds one on purpose (`tests/golden/surface-fixtures/externroute/bind.hero`). The blind seat's 0 of 4 gives no rewording an advantage, and the sentence it read is false on `bind.hero`. No draft that keeps the word *compile* reaches 040's confound. One that says *build* gives the root file a second name beside spec `:30`.
- `prediction` (an observation, it needs a paid run): with 040's design repeated (module given alone, *compile*), B4 still draws at least 1 added `main` in 12.
- `condition`: I would admit B4 if a reading under that design shows B4 at 0 of 12 against the current sentence at 2 or more.

The critic's warning route is unavailable: design.md `:3744` rules out a warning level. Making it a refusal would refuse `bind.hero`'s two roots, which are correct programs.

The drafts, each replacing spec `:22-23`:

| draft | text | legacy / maximum | note |
|---|---|---|---|
| B4 (if one must land) | `- One file is one module; only the file you compile needs `function main()`,` / `  which takes nothing and produces nothing.` | +1 / +1 | |
| B | `- One file is one module. Only the file you build needs `function main()`, which` / `  takes nothing and produces nothing.` | +1 / +1 | second name for the root |
| A | the current text plus `, and a module it uses needs none.` | +8 / +8 | |
| C | the current text plus `; a file only used needs none.` | +7 / +7 | |
| D | `the program's first file holds` | +1 / +1 | vague |
| blind seat's rewording | as read by the blind seat | +8 / +8 | **false** on `bind.hero` |

## Q3 (520)

- `verdict`: **approve "no sentence", conditional.** Neither `:280` nor `:113` changes.
- `section`: §1.6; panel 199's R4; panel 029's R4 precedent (refused by the pass, zero spec tokens).
- `spec_token_delta`: 0. If the condition fails, the sentence owed is q3_I at +24 / +25.
- `removal`: none at 0.
- `needed_for_self_hosting`: no.
- `argument`: `:280` says what an abort is, not what `check` refuses. Under R2 it is true at every level, and a refused program never runs, so widening the refusal leaves it true. `:113` is the only sentence that names mutual recursion, and it stays true. Panel 199 bought no sentence because its rule refuses nothing that ends. The widening inherits that only if every function in the cycle keeps R4's way out, and a call through an unknown function value counts as one. The witnesses survive: `pingpong_helper` and `fnvalue_helper` check 0 and abort 134 at `-O0` and `-O2` today. These are shapes such a rule cannot see, and they can replace the two 508 goldens it would refuse.
- `prediction`: the widened rule moves exactly those two `run` goldens and nothing else in the census. The two helper shapes keep check 0 and exit 134 at both levels.
- `condition`: if the rule refuses any program that ends, I object to the rule rather than buy q3_I.

q3_I, should it ever be owed:
```
- Recursion too deep aborts, and a function whose every path reaches a call of
  itself, directly or through other functions, is a compile error.
```

## Found beside these questions, none filed by me

1. **A concrete parameter of a generic function gives no context.** `g(x: 1, n: 255)` with `n: u8` is refused "expected `u8`, found `i64`". §4.12's letter covers this, but panel 105's stated reason (binding through a type parameter) does not reach it. This is a question for the compiler-engineer.
2. **A misleading message.** On `g(x: 1, n: ok(2))` with `n: i64?`, the `cannot_infer` message says "pass it where a `T?` is expected", which is exactly what the program did. My search of `issues/` for that text found nothing.
3. **Explicit type arguments are told as a scope error.** `ident<i64>(3)` is told as `unknown_name`. My search for "turbofish" and "type argument" found nothing about it.

## What I could not run

- **The real count:** `--refresh` was not run.
- **`unseen`:** its walk asks git, and my copy has no `.git`. I checked instead that the drafts hold only printable ASCII.
- **`special`:** not run; the drafts do not touch its text.
- **The compiler-side routes:** route I and the widened Q3 rule were not built.
- **Reader effect:** no blind reading of N1f or B4.

Files are in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/201-spec-warden/`:
- `notes.txt`
- `drafts/`
- `probes/`
- `scripts/draft.py`
- `suite_draft_*.txt`

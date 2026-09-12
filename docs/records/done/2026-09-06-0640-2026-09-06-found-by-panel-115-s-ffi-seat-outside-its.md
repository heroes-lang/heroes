- [x] **015 — a `certain` fix machine-applies into a program that does not compile** | 2026-09-06, found by panel 115's ffi seat outside its remit, verified by the coordinator the same hour | **repaired 2026-09-06, same day** | panel 115, § Two side findings | **a rule the whole fix machinery rests on, broken by one diagnostic**

  **The reproducer**, four lines, run at commit `13165dc7`:

      function main()
          a = 3.7
          n: i64 @ 0
          n @ int(a)
          print(n)

  `heroes check` says, correctly:

      error[reserved_word]: `int` is not a word in this language — an integer
      says its width: `i64`, and also `i8` `i16` `i32` `u8` `u16` `u32` `u64`
        fix (certain): replace `int` with `i64`

  `heroes check main.hero --apply` writes `n @ i64(a)`, and **that program does
  not compile**:

      error[unknown_name]: nothing named `i64` is in scope

  **The cause, and it is one word: the fix is CONTEXT-BLIND.** `int` is wrong in
  two different positions and the repairs are different. Where it stands as a
  TYPE — `n: int @ 0` — the repair is `i64` and the fix is right. Where it stands
  as a CALL — `int(a)`, which is what a reader coming from C or Python writes to
  truncate a float — the repair is `to_i64`, which the spec names at :66 as the
  conversion that takes a float. The diagnostic offers the type-position repair
  in both places.

  **Why it is a defect and not a rough edge.** CLAUDE.md §8 says only `certain`
  is machine-applicable, and §9 says CI asserts the applied fix compiles. The
  first is the promise this breaks; the second is the instrument that should have
  caught it and did not, because the assertion runs over `x.fixed` goldens and no
  golden covers `int` in a call position. So the guarantee is asserted for the
  cases somebody wrote down, and this one was not among them — CLAUDE.md §1's
  enumeration rule, in the goldens.

  **What is owed.** The fix becomes `to_i64` where the name is the callee of a
  call and `i64` where it is a type, or it drops to `guess` in the call position
  and stays `certain` in the other — the narrowing asks the VALUE (which position
  is this name in?) and never the world (CLAUDE.md §11). A `tests/golden/` case
  named after this defect, carrying symptom, cause and date, with its `.fixed`
  file so the CI assertion covers it (CLAUDE.md §9, Go's `test/fixedbugs`). And
  the same question asked of every other `certain` fix that replaces a NAME
  rather than inserting a label, since this one was found by accident: the list
  comes from the emitter, not from memory.

  **Not this milestone's**, and named rather than smuggled: M-thread-stacks is
  the stack guard and the floor. Filed open so it is not rediscovered.

  **The repair, and it is wider than the defect that provoked it.** `int` was
  the witness; the class is *a word-for-word swap that is right where the word
  DECLARES something and wrong where it is CALLED*. Measured with `--apply`
  before the repair, three of the ten swaps produced programs that do not
  compile: `int(3)` -> `unknown_name`, `switch(3)` -> `missing_match_arms`,
  `fn(3)` -> `expected_expression`.

  `selfhost/scan.hero` now reads the byte after the word, which is a fact about
  the text in hand rather than a premise about the program around it (CLAUDE.md
  §11).

  **AND THE FIRST DRAFT OF THE RULE WAS WRONG, WHICH IS THE PART WORTH KEEPING.**
  The obvious rule is *a `(` after the word means it is called, so drop the fix*,
  and the net refused it: an adversarial case the author **ratified on
  2026-08-04** asserts that `function apply(f: (fn(i64) -> i64), x: i64)` repairs
  correctly, in its own words *"`fn` is one word with one meaning — an error — in
  BOTH positions, and the same certain fix repairs both"*. **A `(` is not a
  call**, and a repair aimed at the class had broken a ruling about one of its
  members.

  **The rule that survived asks the REPLACEMENT rather than the position**, which
  is the only half the lexer can know: may this replacement legally precede a
  `(`? `function` may, because a function type is spelled `function(A) -> B`.
  `i64`, `record`, `variant`, `constant`, `match` and `use` may not, so exactly
  those lose their machine-applicable fix in front of a `(` while `fn`, `func`
  and `def` keep theirs.

  **`int(` gets a better answer rather than only a quieter one**, and it is a
  new `Repair.hint` — shown, never applied — because the honest repair does not
  fit in one word. Measured: `n @ to_i64(a)` is `error[type_mismatch]: expected
  i64, found i64?`, since `to_i64` is fallible, so the reader still has to choose
  `.must()` or `.default(v)`. That is the message's job and not a fix's, and the
  message now says it.

  **Measured after the repair**: `--apply` leaves all three call-position
  programs byte-identical, while `n: int @ 0` still becomes `n: i64 @ 0` with a
  `certain` fix. Nothing that is machine-applicable produces a program that does
  not compile.

  **The instruments**: `tests/golden/surface-fixtures/calledforeign/` and two
  rows in `tests/harness/suite_surface.hero` (45 -> 47), which are the claim in
  two halves — one asserts what is SAID, the other asserts what is NOT DONE, and
  §8's promise is only tested by the second. Plus a unit test in
  `selfhost/keywords.hero` over six words, because the provoking case is a
  witness and not the class.

  **Two things this repair found and did not fix**, filed rather than smuggled:
  `spec/reserved-words.md` calls itself the single source of these messages and
  has **no `int` row at all**, and five of its own rows spell examples with `int`
  as a type — the very word the language refuses. Both are the registry's, not
  the lexer's.

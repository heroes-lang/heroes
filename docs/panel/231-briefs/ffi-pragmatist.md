# Panel 231, the ffi-pragmatist's brief

Read `00-shared.md` beside this file first. You judge the founding constraint (design.md § 1.11,
§ 4.19): the C boundary arm of the generator, and the two tasks about what reaches C. Compile, don't
opine. You hold a veto on anything that breaks the C ABI or makes bindings harder.

**Where you work**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/231-ffi-pragmatist`,
detached at `bd0c4477b`, its `heroes-next` built (`sources 5e0ca4c634086a7c`). Your report:
`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/231-reports/ffi-pragmatist.md`, written as you go.

**What you are asked, each answered with the command that settles it:**

1. **A header generated with the program.** Write by hand, as the generator would, three `extern`
   groups and their headers, each in a shape spec § 13 admits and measurement 056 § 2.3 found
   defects in: a struct passed and returned by value with a nested struct and a fixed array; an `@`
   out-parameter and a `counted_by` array; a callback; a `cstr` lent in and an `owned` result; a
   handle with `acquires` and `consumes`. The C bodies `static inline` in the header, as
   `tests/golden/run/ffi-*.h` are (19 of them), each computing something whose answer the generator
   knows. Build and run each with your `heroes-next` in the five configurations of `run` (00-shared §
   Facts), and report what each printed, and every refusal or difference, which is a finding.
2. **What the generator must know about C to know the answer**: a C `int` overflowing is undefined
   behaviour, not an abort; a `size_t` wraps; the header's arithmetic must stay inside what C defines,
   or the clean arm is not clean. Say which C operations the header generator may use, and how it
   keeps them defined, and whether clang's `-fsanitize=undefined` (the `--sanitize` configurations)
   is the check that it did.
3. **The shapes that are refused on purpose** (spec § 13: a parameter at another width or sign than
   the header's, a `partial` record compared, a handle as a map key, a lend escaping): the generator
   writes valid programs only, so these are the mutation instrument's. Say whether a declared arm of
   the C boundary has a use (a program whose expected outcome is the abort § 13 names: a lease never
   ended, a handle given back twice, C reaching past a `counted_by` extent), and build one.
4. **Task 972** (`--sanitize` at `-O2` deletes the leaks LeakSanitizer exists to find). The run form
   gained `--sanitize -O0` on 2026-10-07 (defect 390), and the corpus suite still runs three
   configurations without it. Measure what 972 measured, a leak through a `static inline` header
   function at `-O0` and at `-O2`, on this Mac (whose ASan has no LeakSanitizer, panel 021, the run
   suite's header) and say what the Linux leg must run; then say whether 972 is settled by defect 390
   for the run form, owed for the corpus, or re-homed on the generator's oracle 5.
5. **Task 1234** (nothing compiles or links what `--emit-c` wrote). Re-measure its premise today:
   compile with `clang -fsyntax-only` (and then compile and link) the blessed emissions under
   `tests/emission/` and `tests/golden/emit/`, report how many fail and why, an indication of the
   cost, and whether the leg belongs in `emission`, in the generator's oracle 4, or behind the push.
6. **Oracle 5** across platforms: what each of the four legs can see of a leak (`hero_runtime_check_leaks`
   in every generated `main`, LeakSanitizer on Linux, none on Darwin's ASan), so the oracle's
   sentence says what it promises where.

**Your output**, per question: `verdict` (approve | object | veto) with the design.md section;
`experiment` (the C you wrote and whether clang accepted it); `argument`; `prediction` (falsifiable,
about a named binding or case); `condition`; every number with its command.

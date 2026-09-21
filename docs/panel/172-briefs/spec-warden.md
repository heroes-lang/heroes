# Panel 172 — brief for the spec-warden

Read `docs/panel/172-briefs/00-shared.md` first. You judge the indicator
(design.md §1.2, §1.6) and Principle 0's burden of proof, and you hold a veto
on budget breach.

## The numbers, measured today

`spec/heroes-spec.md` at `abf9a17e`: **8216** real on `claude-opus-5`, **6172**
vendored, digest `2b1556634e73455a` (`heroes measure spec/heroes-spec.md
--refresh`, 2026-09-21, pinned in that commit). `DELTA_GATE` 50 vendored. The
document's own rule (`.claude/rules/spec-shape.md`): **a vendored delta is not
a price**; apply the draft to the real path in your copy, `--refresh`, revert.

## Build, in a copy

    cp -r /Users/joseph/Temp/heroes/heroes-lang <scratch>/spec-warden-172
    cd <scratch>/spec-warden-172 && rm -rf build
    clang -I runtime seed/heroes.c runtime/runtime.c -o heroes
    . /Users/joseph/Temp/heroes/heroes-lang/.env       # never print the key

`./heroes measure spec/heroes-spec.md --refresh` after each edit; `git checkout
-- spec/heroes-spec.md` to revert in the copy.

## Drafts to price, on the real instrument

**A1 (route A, merged into the `consumes` sentence)**: after *`consumes` after a
parameter says the call ends that value's life, so passing one the function
borrowed is an error: mark the parameter `@` and the value does not survive the
call.* add *On a `cstr` or `ptr` parameter it says C frees what it is handed,
so no lend and no lease reaches it.*

**A2 (route A, appended as its own sentence, for the merge-versus-append
comparison the last two sittings ran)**: *`consumes` on a `cstr` or `ptr`
parameter says C frees what it is handed: nothing lent or leased reaches it.*

**B1 (route B, rewriting the lend sentence)**: replace *a parameter is taken to
keep what it is handed unless declared `lent`, and a lend reaches only one so
declared* by a sentence that says: a pointer parameter says `lent` (read and let
go), `borrows` (kept, never freed) or `consumes` (freed); a lend reaches only
`lent`, a lease only `lent` or `borrows`, and a parameter that says nothing
takes neither. Write the cheapest TRUE sentence and price it; then price the
version that keeps *taken to keep* as the reading of silence.

Report each as vendored and real, before and after, with the digest.

## Principle 0 and Part 6

- **Does the compiler need it?** `grep -rn 'consumes' selfhost/cli
  selfhost/emit selfhost/library_source.hero tests/harness/shell.hero` — does
  any binding the compiler itself makes hand a pointer to a function that frees
  it? If none, the form enters on the thesis (design.md §1.12 and Part 11), and
  the argument must be measured: 070's reproducer in the shared brief is the
  measurement; say whether it suffices and what else would.
- **Part 6**: is a rule that a freeing parameter must be declared, or the
  reading of an unmarked one under route B, already refused anywhere in design.md
  Part 6 or `docs/panel/`? `grep -n 'consumes\|frees\|give-away' docs/design/design.md`
  and the three sittings the shared brief names.
- **Fails-unsafe**: your own finding at panel 170 was that a forgotten
  `consumes` fails unsafe where the other marks fail safe. Under route A that
  stands; under route B a forgotten word refuses. Does that change your
  verdict on which route the document should carry, and at what price?
- **The third reserved case**, design.md:2333-2340: *a buffer that C takes
  ownership of*, about a pointer C made. Is a lease into a freer that case, or
  a fourth? If a fourth, design.md's line needs a correction added underneath
  (never rewritten), and you price nothing for it.

## Report

`docs/panel/172-reports/spec-warden.md`: verdict · section · the table of
prices, each with its command · one falsifiable prediction naming an instrument
that exists · veto condition if any · which draft you would land and why, with
the merge-versus-append rule the last sittings recorded.

# Panel 160 — brief for the historian

Read `00-shared.md` for the question. Your seat is precedent, advisory, no veto.
**Every claim you make is verified by web search or marked UNVERIFIED** — at
panel 159 you refused to launder a coordinator's Java claim into a citation, and
that refusal is the reason your report was usable. Six retrievals, one at a time.

## The precedent class

A language or a lint that **refuses, warns on, or gives a distinct spelling to**
a predicate applied to a nested optional — `Option<Option<T>>`, `T??`,
`Optional<Optional<T>>` — where the predicate asks about the outer level and a
reader would take it as asking about the inner.

## Where to look, in order

1. **Clippy's `option_option` lint** (rust-clippy). What does it fire on — the
   TYPE in a signature, or a use? What is its stated rationale, verbatim? Is it
   warn-by-default, pedantic, or restriction? If it warns on the type alone, the
   precedent is for option B (refuse the declaration) and not for A or E.
2. **Swift, `if let` on a double optional.** `if let x = dict["k"]` where the
   dictionary's value type is `Int?` — does `x` come out as `Int?` or `Int`, and
   is there a documented warning? The 2016 Swift Forums thread you fetched at
   panel 158 is the write-side story; this is the read side.
3. **Kotlin's `?.` and `!!` on `T??`.** Kotlin has no nested nullable — `T??` is
   `T?` by construction. That is panel 158's vetoed flatten shipped as a type
   rule. Verify that it is so, and whether the Kotlin design notes give a reason.
4. **Rust `Option::is_some` on `Option<Option<T>>`.** Is there ANY lint,
   clippy or rustc, that fires on `.is_some()` applied to a nested option? If
   none exists after ten years of `Option<Option<T>>` being legal and common,
   that is a precedent AGAINST option E, and the sitting should hear it.
5. **Haskell `Maybe (Maybe a)`** and `isJust`. Same question; GHC warnings.
6. One retrieval you choose, following what the first five turned up.

## What to say plainly

If no shipped language or lint refuses the predicate — only the type — then
options A and E have no precedent and option B has clippy's. Say which class the
evidence supports, and say when it supports none.

## Report

You have no write tool. Return the full text of your report as your final
message; the coordinator writes it to `docs/panel/160-reports/historian.md`
verbatim, marked as such. Verdict · sources with dates fetched · what is
VERIFIED and what is not · prediction · condition.

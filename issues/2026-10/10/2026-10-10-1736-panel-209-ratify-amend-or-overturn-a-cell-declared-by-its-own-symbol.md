---
kind: decision
area: compiler
milestone: none
filed: 2026-10-10
commit: self
github: none
---

- [x] **panel 209** | ratify, amend or overturn the provisional resolution (R1b with every condition the seats built: `@=` declares a mutable cell and `@` re-binds it, the type optional on both `=` and `@=` and demanded by `cannot_infer` where the value cannot say it, a bare `nullptr` included; a cell nothing re-binds refused with a certain fix to `=`, an `@` argument counting as a re-binding; the mutation's `unknown_name` gaining a fix that writes the declaration; the width class told with a note and a fix; `(@=x` and `v @= v + 1` one message each with a certain fix; the spec's text W1b-min at 9850 real; landed as R9, then R10, then R1, in the commits the synthesis names; R8 vetoed; R0, R2 to R7, R11 refused; what conservative would have been: R0 with R9 and the never-re-bound rule) | `docs/panel/209-a-cell-is-declared-by-its-own-symbol-and-typed-by-its-value.md`

    **Origin:** panel 209's synthesis, 2026-10-10 from 17:33, on the trunk frozen at `87794631`; convened by the author's request of that afternoon, after their question why a mutable declaration carries a type and an immutable one does not, and their proposal of `@=`.

    **Default while open:** nothing lands; the language stays as it is, and no lane touches `selfhost/token.hero`, `punctuation.hero`, `grammar_expr.hero`'s binding arms or spec § 5 for this question.

    **Recommendation:** ratify, and schedule the landing as one milestone (its name the author's, two words naming the deliverable, the inferred cell; the sitting names no id, since a name enters `docs/roadmap/names.md` when the row is scheduled) in three commits in the order measured (R9 and the never-re-bound rule with its 233 and more rewrites first, the token and the 1671-file rewrite second, the optional type third), because every seat approved that order, no seat vetoed the route, the blind seat read it right 4 of 4 in every cell, and the one route that moves a ceiling for a mechanism (R8) is the one vetoed. If the author wants the asymmetry kept, the conservative route is in the synthesis and loses only tokens and uniformity.

    **Verdict, 2026-10-10 17:57:** **ratified**, the author answering in conversation between 17:52 and 17:57 by the clocks read before and after (meant as: *all right, then I ratify the panel; and then on to the push*), after weighing, on the human side, that the small asymmetry at the declaration is the price for the mutation standing out, and asking the coordinator's own reading as a model, which agreed (the common act keeps the common symbol, the rare act the rare one, and the line a reader must find is the rare one). A variant the author raised in the same conversation, `:=` for the once-bound name and bare `=` for the mutation, was weighed and set aside unmeasured: it would move the loud glyph from the mutation to the birth, against design.md §4.4's reason, and rewrite every binding line of the tree. Recorded as a reading (CLAUDE.md § 4). The landing is a milestone the author schedules; the critic's `@`-parameter question stays its own open decision.

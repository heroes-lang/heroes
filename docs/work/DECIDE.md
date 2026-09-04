# DECIDE — the decisions the compiler is waiting on

Read by **`/decide`**. Every item here asks **what should be true**, and until it
is answered the compiler goes on behaving some way by default. That default is
the cost of leaving an item open, so each item names it.

**Only open items live here.** The moment one is answered it is ticked with the
verdict written into it and moved to `docs/work/DONE.md`. A file that keeps its
own closed items stops being a list of what is owed — this one held **138 ticked
items and zero open ones** on 2026-08-26, under eighteen headings still titled
`## Open`, which is why the rule now lives in `/decide` itself instead of in
prose about `/decide`.

**One notation: `- [ ]`.** No section headings, no prose sections, no
strikethrough. A finding written as a bare bullet is invisible to every count in
this project, and nine of them sat in exactly that shape under two panel
headings here until 2026-08-26 — **five already closed** and two of those
measurably stale, while the file reported itself empty.

Two rules bind this file, both learned the hard way:

- **Verify before asking.** An entry is a claim from the day it was written, and
  entries outlive their causes. Asking the author about a settled question is the
  one cost this list cannot pay. Of the nine findings recovered on 2026-08-26,
  five were closed and were ticked with what closed them rather than asked.
- **Rank by what it blocks**, never by age, and say the blocker in the question.

Format: `- [ ] <origin> | <what> | <where to look> | <why it matters>`

- [ ] **Should the compiler refuse a local that is passed as `@` to an `extern` function and never read afterwards?** | asked by the author 2026-09-04 (*"shall we put a rule in the compiler to avoid these leaks?"*) after the Linux leg of CI found `examples/ledger/` leaking 40 bytes per refused statement (`docs/work/DONE.md`, that entry) | **The rule that does NOT work, and it is the one that comes to mind first**: track what C owns. C's type cannot express it — `char **errmsg` (the caller frees it) and `const char **pzTail` (it points into the caller's own string) are the same type — and **five of the six pointer-returning bindings in this corpus are library-owned**, so a rule that inferred ownership from a header would be wrong most of the time. That is a premise about the world, which CLAUDE.md §11 forbids resting a narrowing on. **The rule that does work asks the value**: a local passed as `@` and never read after the call is meaningless whatever C owns — either the value is wanted, and it is read, or it is not, and `nullptr` is passed, which is what both repairs did. **Six things measured for it, in the session that proposes it.** (1) The compiler already computes exactly that fact: `unread.slot_is_read` at `selfhost/emit/body.hero:148`, used today only to decide whether to write an attribute into the C. (2) **No C warning can catch this class**: the two forms in one file under `clang -Wall -Wextra -Wunused-but-set-variable` give **zero warnings on both**, because the slot is address-taken and clang counts that as a use — so Heroes is the only layer where it is catchable, and that is the argument rather than a convenience. (3) It fires at **four** sites in `examples/`: the two `@error` of `sqlite3_exec` (both leaks, one of which turned CI red) and the two `@tail` of `sqlite3_prepare_v2` (harmless, same shape, and `nullptr` is what SQLite documents there). (4) The escape already exists and costs one character: `_ = error` after the call, for a C function that demands a non-NULL out-pointer whose value the program does not want. (5) It is the thesis' own shape — copy the C signature, pass a local for the out-parameter, never read it — and the assistant made that mistake **twice in one session, in two programs**. (6) Principle 0: the compiler does not need it, so it enters on the thesis argument, and the argument carries a count rather than a conviction. **The limit, stated so the sitting is not asked the wrong question**: this covers out-parameters and NOT memory C returns as a return value, so `curl_easy_init` without `curl_easy_cleanup` stays invisible to it. That is a second and harder question. **Also owed and not a language change**: `selfhost/emit/body.hero:141-143` says `_ = v` is *the only* way to reach a written-never-read slot, which the `@error` site falsifies — an expired premise (§11) to correct with whichever change to that file lands first. **Recommended: convene the full panel** (not the soundness lane: the spec may owe a sentence about `@` on an `extern`, which is the ergonomist's and the warden's work), and offer it a fifteenth `heroes mutate` operator that plants exactly this mistake, so the rule arrives with a kill rate the way panel 102's did | selfhost/emit/body.hero:141-152 · selfhost/emit/unread.hero · examples/ledger/db/sqlite.hero · examples/sqlite/main.hero · docs/work/DONE.md (the defect entry) · CLAUDE.md §4, §11, §12 | an FFI binding is the one place a Heroes program can leak, and the machine this project is written on has no leak detector, so the compiler is the only instrument that can be in front of the mistake rather than behind it

# Panel 164 — spec-warden

Read `00-shared.md` first, then `spec/heroes-spec.md` § 13 and
`.claude/rules/spec-shape.md`. Veto on budget breach; you carry Principle 0's
burden.

## The budget, measured this session

`./heroes measure spec/heroes-spec.md`: **6041** vendored, **8040** real,
ceiling **10240**. **This milestone has already landed two amendments**, +46
and +10 real, both in § 13's `cstr` paragraph. You are pricing a third in the
same paragraph.

`--refresh` refuses every path but `spec/heroes-spec.md` and `CLAUDE.md`;
`. ./.env` first; price in a **scratchpad copy**. A vendored delta is not a
price.

## What you are asked

1. **Where does the rule live, and is it one rule or two?** § 13's paragraph
   already says `s.cstr()` lends a `str`. Widening it to a field is one clause;
   a `ptr` lend is a second sentence; refusing is a Part 6 row. Price each
   alone, real.

2. **The termination clause is the cost that matters.** Whichever route lands,
   § 13 has to say what a lent field's END is — a `str` has a runtime zero, a
   field has whatever C left. Draft the shortest true sentence and price it.
   Panel 162's spec-warden found *to its first zero or whole* for the read; the
   lend needs its own, because a lend does not scan.

3. **Three amendments to one paragraph in one milestone.** Say whether §1.6's
   payment rule reads differently when a paragraph is amended three times in a
   day, and whether a named removal exists in that paragraph after the two
   already landed. Panel 162 declined the removal it found; say if it is still
   there.

4. **Refusing is live.** Zero corpus programs pass a fixed field to C today (the
   defect makes it impossible). If Principle 0 says the form has not earned its
   way in, say so as your verdict, and hold it to Part 6's standard: `strlen` on
   a `struct utsname` field is the program fact that would make the refusal
   wrong, and the sitting owes an answer for it either way.

## Deliver

Verdict · the section it rests on · the real count of each draft or *lower
bound* in those words · a falsifiable prediction · any budget veto.

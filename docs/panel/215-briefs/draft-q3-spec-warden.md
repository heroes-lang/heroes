# Panel 215, after the critic's pass 2: the spec-warden drafts Q3's two sentences, measured

Written 2026-10-14 at about 11:40 by the coordinator, after the five reports and the critic's pass
2 (`../215-reports/critic-reports.md`, read it whole first, § 1 above all), and the paid run's
correction (`../215-reports/llm-ergonomist.md`, its last section). **The reading the coordinator
would adopt**: A2's 1 of 10 is a miss of panel 194's registered first-try threshold on its *one task
holding a union*, under R1 with K, and the readers' notes put its cause in the specification's text:
§ 13's union sentence (*named or anonymous*, three readers) and R1's sentence, silent about the bytes
a `partial` record does not declare (four readers). So R1 stands (the compiler keeps its promise:
`zeroed.hero`'s whole-object zero) and **its two sentences are repaired**, then the reader test is
re-run on the repaired text (the coordinator's paid run, after your drafts).

**Your work, in your own copy** (`git worktree add --detach .claude/worktrees/scratch-panel-215/sw-build
7e5c02048`, the compiler from the seed):

1. **Measure what the language accepts for every union shape**, each a program built and run on
   this Mac, against the header it binds: an anonymous C11 union's members as the record's fields; a
   named member of an untagged union (`struct tagged { int kind; union {...} via; }`, the A2 header,
   `.claude/worktrees/readers/kit/union.h`) bound `partial` and ended `rest: zero`, and through the
   header's macro (`#define fd via.fd`); a named member of a TAGGED union, bound as a record of the
   group (`record V tag v`) and held as a field; a typedef'd union; an anonymous struct inside a
   union. Say for each what builds, what it prints, and what message the wrong spelling meets.
2. **Draft the two sentences** from what you measured: R1's (*"every field sharing no byte with one
   it names is zero"*) so it says the whole object, a `partial` record's undeclared bytes included;
   and the union sentence so a reader can bind each shape above without guessing. Merge rather than
   append (`.claude/rules/spec-shape.md`), and the shortest text that answers the four readers'
   notes (`../215-reports/llm-ergonomist-work/r0*/notes.md`, `grep -i -E "partial|rest: zero|named
   or anonymous"`).
3. **Price each draft offline** (`heroes measure` on a copy of the spec with the draft applied, both
   vendored tables) and give the real band as you did; the coordinator takes the real count with
   `--refresh`. Say which removal pays, if any, and whether the § 9 `@` rewrite you found still
   applies.
4. **Write the reader task for the re-run**: the A2 union task as it was (the same header and words,
   so the score compares), panel 178's Darwin `utsname` task as panel 194 registered it (find it in
   `docs/panel/178-*` and `docs/panel/194-*`, its header and words), and a third task where copying a
   lock is the natural code (a constructor helper returning a counter, a read-only helper taking it,
   a counter kept in a Heroes record), the critic's § 6 question; each with the answer key the
   coordinator scores against.

Write `../215-reports/spec-warden-drafts.md` as you go. Limits as your first brief: no paid run, no
`--refresh`, no credential, no GitHub write, no push, no commit, nothing outside the repository's root,
your copy removed at the end.

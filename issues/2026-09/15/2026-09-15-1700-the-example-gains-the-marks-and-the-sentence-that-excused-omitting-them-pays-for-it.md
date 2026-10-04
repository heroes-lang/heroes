# The example gains the marks, and the sentence that excused omitting them pays for it

2026-09-15. Panel 153 R1, on `docs/work/DEFECTS.md` 043. The sitting is
`docs/panel/153-*`; this entry is the decision and what it cost.

## The fence teaches, and it was teaching the wrong thing

`spec § 13`'s one worked example binds `sqlite3_open` and `sqlite3_close` — an
acquire and a release — and carried neither `acquires` nor `consumes`. The
llm-ergonomist read the section ten times with that fence and ten times with the
corrected one, and counted: **8 of 10 readings omit a mark the section requires
under the old fence, six of them compiling in silence; 2 of 10 under the new
one.** The rule only arms where some `extern` declares `consumes`, so a reader
who copies the example gets a binding that tracks nothing, and the runtime
counter never sees it.

Measured on the program rather than argued. The same forgetful reader, opening
`:memory:` and never closing: **exit 0 with an empty stderr** under the old
fence, **exit 134 naming the address** under the new one.

## What paid is the sentence the defect is about

*Unmarked pointers are never freed.* leaves § 13. **Two of the twenty blind
readings took that sentence as permission to omit the marks**, so the removal
pays the bill and lowers the rate it is paying for. Its content is not lost:
`owned` says what the compiler frees, the two marks say what the program owes,
and the abort at `main` return catches the handle nobody gave back.

The package is **+0 real and +0 vendored**: 7974 stays 7974 on `claude-opus-5`,
5989 stays 5989, digest `21a9dc541cfa2fa8`. The spec-warden priced it at 7974
real and 5990 vendored with a different digest, so its prediction **split** — the
real figure exact, the vendored one token cheaper — and the ledger row says so
rather than rounding.

`design.md` §4.19 carried the identical bare pair and is corrected in the same
commit, free, on that seat's own note.

## And an author instruction of the same hour, which the contract already held

The author's words were *twenty-five minutes is an eternity; it may never be that
high*, about this session's own scheduled wakeups of 1200 and then 1500 seconds.
**The rule was already in § 3** — *in a `/loop` a wakeup is at most three
minutes* — and what made it breakable is that the harness's scheduling tool
advises 1200 to 1800 seconds and argues it in terms of token cost. The two
answer different questions: the tool's advice is about what a wakeup COSTS, the
contract's rule is about how long the author is left in silence, and
§ Precedence puts the contract above a skill. The rule now binds every wakeup
and not only a loop's. The sentence in § 3 is four words longer; the story is in
CL-010, which is where what a rule cost to learn lives, because the contract is
measured and its repair is to move a rule rather than to cut one.

The contract's pinned record was refreshed with it: **5570 → 5592 vendored**,
**7449 real**. Of the 36 tokens the pin was behind, **14 predate this session**
and were never named by the commits that spent them; 22 are this instruction's.

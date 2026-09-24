# Panel 177 — completeness critic

You are not a sixth judge and you give no verdict. Read the six briefs in this
directory and the five reports in `docs/panel/177-reports/`, and name what is
MISSING: a route nobody listed, a claim asserted and not measured with the
command that settles it, a contradiction between seats and which side is
checkable, and the question the sitting should have asked.
`.claude/skills/panel/SKILL.md` step 3b is your definition.

Work in `<scratchpad>/177-completeness-critic/`, a `git archive` of HEAD
`521c5e02` made for you; build the compiler there from the seed (a seed built
with `-O2` rebuilds an edited `selfhost/` in 67-72 s, the compiler-engineer
measured). You may READ and copy the seats' prototypes and probes out of
`<scratchpad>/177-compiler-engineer/work/` (trees `P/`, `R/`, `M/`, `Mm/`, `B/`
and their diffs) and `<scratchpad>/177-ffi-pragmatist/` (`exp/`, `work/`,
`linux-out/`) and rerun them in your own directory; never build or run inside
theirs. The trunk is read-only. Return your report as your final message; the
coordinator writes it to `docs/panel/177-reports/completeness-critic.md`
unchanged.

## Known to the coordinator, so report them only if you find them wrong

- **The shared brief said R catches defect 088's shape. It does not**, and two
  seats measured it independently on three legs: the reproducer's next
  `calloc` returns the freed address, so the dead `b` is live again when R
  asks. That sentence was the coordinator's inference, written unrun.
- **The label-stripped sentences II and III** handed to the llm-ergonomist are
  false of the routes they stood for (spec-warden): II misses copies and loops,
  III refuses every borrowed handle.
- **Four seats' reports were written out by the coordinator** from their final
  messages, the harness having refused their writes; each file's header says
  so and names the one mechanical change (HTML entities unescaped).
- **Found in what ships, to be reproduced and filed by the coordinator**: the
  ffi-pragmatist's false *`nullptr` reached C* on a stale handle
  (`runtime/parts/stack.c:502-504`) and its `--sanitize` blind inside an
  uninstrumented library, which makes design.md Part 8 wart 20's *"The remedy
  today is `--sanitize`, which names the line"* false for real libraries.

## Where the seats meet, which is where to look hardest

Settle each by running, not by reading:

1. **Nobody built P + T + M-must together.** The compiler-engineer built P and
   M-must and never saw T; the ffi-pragmatist built P and T as emulations and
   never saw M-must. What does the composite catch and refuse over the five
   reproducers, the ffi seat's eight correct real-library programs, its
   `sqlite_copies` and `pair_escape`, and the 510 files?
2. **T's false abort.** T remembers released addresses, and the ffi seat's own
   `cb_reuse` (a header model) is a correct program T aborts: an address the
   program gave back, which C then reuses for a handle it hands over without an
   `acquires`. Write the real-library shape — json-c freeing one object and
   handing back a borrowed child at the same address through
   `json_object_object_get` is the obvious candidate — and run it. Then the
   repair nobody priced: a handle C hands over through a `borrows` result or a
   callback parameter clears its dead mark. Does that close it, and what does
   it cost?
3. **The relabel the llm-ergonomist found, A4, unrun by it.** Under panel 176's
   V1c, `fclose(stream: File transfers pclose)` on a `popen` stream passes the
   releaser check and nothing ever calls `pclose`. Emulate it on the
   compiler-engineer's or panel 176's prototype and run it. Then the
   ergonomist's repair: a transfer names the PARAMETER that receives the life,
   and a call with no other handle parameter cannot transfer. Does any real
   transfer function in the ffi seat's census break under that rule?
4. **The success clause.** Four seats put it on the result and the ergonomist
   on the parameter, on polarity. The compiler-engineer built the result form
   and says it must cover `consumes` (`sqlite3_close` and `SQLITE_BUSY`); the
   spec-warden and the historian say it must cover `retains` (`X509_up_ref`).
   Is there one spelling that covers all three, and does the
   compiler-engineer's `B/` build do it?
5. **M-must against the ergonomist's H14.** Does a write revive the name? Does
   M-must refuse the correct B2 loop, and does it catch the buggy one? What does
   the compiler-engineer's claim that design.md §4.4 (lines 1077-1081) counts
   move checking as deleted require of the resolution?
6. **P's conditions.** The compiler-engineer lists four: `==`, `@` parameters,
   the poison page, the IR. The ffi seat approves P *"on four conditions"* and
   does not list them; its evidence implies at least poisoning a PLACE (the
   ledger releases through `db.handle`, a field of an `@` cell), checking at
   every READ rather than at handle arguments (`pair_escape`), and composing
   with the success clause (`jsonc_success`). That reading is the
   coordinator's. Reconcile the two lists, and say which conditions are needed
   for P to be sound rather than merely better.

Say, for every route a seat recommends, which shapes it was RUN against and
which it was only argued against.

# Panel 176 — completeness critic

You are not a sixth judge and you give no verdict. Read the six briefs in this
directory and the five reports in `docs/panel/176-reports/`, and name what is
MISSING: a route nobody listed, a claim asserted and not measured with the
command that settles it, a contradiction between seats and which side is
checkable, and the question the sitting should have asked.
`.claude/skills/panel/SKILL.md` step 3b is your definition.

Work in `<scratchpad>/176-completeness-critic/`, copied from the trunk at
`a747e5a2` and built from the seed there. You may READ and copy the seats'
prototypes and probes out of `<scratchpad>/176-<seat>/` and rerun them in your
own directory — in particular the compiler-engineer's
`work/prototype-176-final.diff` (build it as a compiler in your copy) and the
ffi-pragmatist's `exp/hv.c` route runtime. Write your report to
`docs/panel/176-reports/completeness-critic.md`.

## Known to the coordinator, so report them only if you find them wrong

- **Filed after this sitting, each reproduced by the coordinator**: 083, an `@`
  cell holding a pointer accepted against a `void *` parameter (the
  llm-ergonomist's finding from the spec alone; `@p: Mem borrows` over `tag
  void` hands C a stack address); 084, `SSL_set_bio(s, b, b)` refused at 134
  (ffi-pragmatist); 085, `unread_releaser` answering 1 alone and 0 inside a
  program (ffi-pragmatist). The compiler-engineer's three — a double release
  before any acquisition dying at 133 with zero bytes, `heroes grammar`'s *six
  contextual words*, and **`emission` red on the trunk by the two goldens lane
  076 added** — are the coordinator's to reproduce and file; the last is the
  coordinator's own fault, from trusting `.claude/rules/verification.md`'s map
  instead of rerunning its command.
- **Two errors in the shared brief, found by seats**: the `+14` and `+29` it
  attributed to panel 175 were the coordinator's own measurements that day and
  are not in panel 175's file (spec-warden); and panel 175's *json-c freed the
  root* is false, json-c 0.19 leaks every object that had a child (ffi-pragmatist).
- **The blind seat's input** was copied outside the tree; the seat reports that
  `CLAUDE.md`, `MEMORY.md` and a git status snapshot still reached its context,
  injected by the session rather than loaded by a read.

## Where the seats meet, which is where to look hardest

Settle each by running, not by reading:

1. **An unchecked transfer is a new way to silence defect 075.** The
   compiler-engineer's `transfers` is emitted as `hero_handle_consumed(h, NULL,
   NULL)` and asks no releaser set. The historian's V1c says Clang's
   `ownership_holds` still counts toward mismatched deallocation. Build the
   prototype and write `popen` → `fclose` with `fclose` marked `transfers`:
   does anything stop it, at check or at run time?
2. **The conditional transfer.** The ffi-pragmatist measured every route ending
   the obligation before the call, so a failed json-c add refuses the correct
   program and passes the leaking one. Does the engineer's prototype do the
   same, and what spelling would make the obligation end only on success?
3. **The call-site rule against the ergonomist's veto.** `unadmitted_release`
   refuses a call when no mark IN THE PROGRAM names the function. The
   ergonomist's veto condition is an abort that depends on anything besides the
   acquiring declaration and the ending call. Is the call-site rule's verdict a
   function of the line and its signature, or of a third declaration?
4. **R1's two readings.** The ffi seat's *first if not held* against the
   warden's *live or borrowed* against the engineer's `retains` resetting
   nothing and adding one. Run json-c's documented borrow-then-`json_object_get`
   (`jsonc_borrow_get` in the ffi seat's work) under the engineer's prototype.
5. **Question 2's false refusal.** The engineer measured `realpath` in two modes
   across two modules refused by `contract_differs`. How common is a
   value-dependent contract bound twice, and is there a narrowing that keeps
   `xmod-lent` refused and `realpath` legal?
6. **`SSL_set_bio(s, b, b)`**, which every route leaves at 134: what would it
   take, and is it this sitting's or a defect's?

Say, for every route a seat recommends, which shapes it was RUN against and
which it was only argued against.

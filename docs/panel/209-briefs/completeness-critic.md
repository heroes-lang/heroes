# Panel 209, the completeness critic's brief

Not a sixth judge and no verdict. Two passes. Your folder is
`.claude/worktrees/scratch-b15/209-critic/`, a detached worktree of the trunk
at `87794631`; run no git in it; build your compiler there from the seed. The
rules of `00-shared.md` § The rules every seat works under bind you. Your
reports are `docs/panel/209-reports/completeness-critic-pass1.md` and
`-pass2.md`, written as you go.

## Pass 1, before any seat is launched: the briefs against the world

Read the six briefs of `docs/panel/209-briefs/` (`00-shared.md`,
`compiler-engineer.md`, `ffi-pragmatist.md`, `spec-warden.md`, `historian.md`,
`llm-ergonomist.md`) and, for every number, path, line number, count and
negative sentence in them, run the command that settles it in your copy and
say *holds*, *false, reads X*, or *could not run, because*. In particular:

- the shapes table and the initialiser classes (the patterns are in the
  compiler-engineer's brief and the shared one; re-run them and also attack
  the pattern: what a grep on the line's shape misses, a declaration split
  over lines, a `_ =`, a `@` declaration inside a `test` block);
- the seven probes (`209-coordinator/probes/*.hero`, re-run with your copy's
  compiler) and the two t3 programs;
- the negatives: *`@=` appears nowhere in the spec, the compiler, the goldens,
  design.md or the sittings*; *`@` followed by `=` is legal nowhere today*;
  *no sitting has had the mandatory type on the table* (re-run the search
  with a vocabulary of your own and name it); *the round touches none of the
  lexer, parser, printer or AST files*; *`Binding   =` exists only in the
  spec*;
- the spec rows: the vendored counts you can re-run (`heroes measure` on the
  trunk's spec and on `spec-B1.md`, `spec-B2.md` from your copy's root); the
  real rows you cannot (no paid run), so say they stand unverified;
- the design.md and spec line numbers, each opened;
- **the option set**: a route the eight listed do not cover, and the question
  the sitting should ask and has not. Two the coordinator sees and did not
  put in the list: a cell's type inferred from ALL its writes in the block
  rather than its first value (what it would cost against design.md §4.5's
  *inference is local only*), and keeping R0 while repairing only the `v @ 0`
  message so it says the declaration's shape (design.md §4.17's promise,
  which today's message does not keep: the probe).

Return the list; the coordinator repairs the briefs and only then launches
the seats.

## Pass 2, after the five reports: what is missing

Read the five reports under `docs/panel/209-reports/` and the six blind
reports, and name: a claim asserted and not measured with the command that
settles it; a contradiction between seats with which half is checkable and
the command; a route nobody listed; a prediction that names no instrument; a
seat handed a framing fact it did not check; and the question the sitting
should have asked. For the blind reports, say whether any `context` answer is
a yes, and whether a difference between A and B rests on the specification
or on the task's wording.

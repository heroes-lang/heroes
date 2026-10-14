# Panel 215, after the critic's pass 2: the compiler-engineer builds Q1's route G, recursive

Written 2026-10-14 at about 11:40 by the coordinator, after the five reports and the critic's pass
2 (`../215-reports/critic-reports.md`, read it whole first, § 2 and § 5 above all). The sitting's
procedure: **a route that does not build is not adopted**, and the route the coordinator would adopt
for Q1 is the one the critic names in § 7: **G with the header's membership, every span written as
clang-checked `offsetof`/`sizeof` of a C member the dump names, recursing into member records
(`offsetof(struct ocd, inner.d)`) and elements of array members (`offsetof(struct arr, a[1].c)`),
and the elements of an array lend zeroed in the region `hero_lend_take` fills.** Your own prototype
(`../215-reports/compiler-engineer-work/prototype.diff`, `padding.hero`) is the base.

**Build it** in your own copy: `git worktree add --detach .claude/worktrees/scratch-panel-215/ce-build
7e5c02048` from the repository's root, the prototype applied, the compiler built from the seed and
then from `selfhost/`. Then run, each named shape with its command and what it printed:

- the critic's probes in `../215-reports/critic-work/pass2/padc/`: `pc.hero` (tail padding, field
  lend, **a record nested in a record**, **an array of records lent with `counted_by`**), `uni.hero`,
  `unimid.hero`, `dv.hero` (the dirtied stack, `heroes build -O0` and `-O2`); your own `cret`,
  `fieldlend`, `rzpad`; MemorySanitizer in `heroes-linux` (x86-64 emulated) at `-O0`, `-O1`, `-O2`,
  `-O3`, `-O2 -fno-inline`, and the value probes (`dv`, the ffi-pragmatist's `memcmp`/`sha` main) with
  no sanitizer, three runs each;
- **the half you left unbuilt**: a `partial` record and a record with an anonymous member, through
  clang's layout dump (or say, measured, why it cannot be done and what it leaves);
- `run`, `emit`, `check` whole; `emission` (count what moved and read every moved line by script);
  the compiler's own tests; a cold `--emit-c` of the compiler, user plus sys, two runs, against
  today's, the load said.

Write `../215-reports/compiler-engineer-build.md` as you go: per shape, today against the route; the
lines the route costs by directory; what stays open, named. **No verdict needed**: the numbers.
Limits as your first brief: no paid run, no credential, no GitHub write, no push, no commit, nothing
outside the repository's root (never `/tmp`: set `TMPDIR` to your copy's `build/` if a tool needs
one), your copy removed at the end with `git worktree remove --force` on YOUR copy only.

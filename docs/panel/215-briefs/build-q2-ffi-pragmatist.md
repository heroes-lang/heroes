# Panel 215, after the critic's pass 2: the ffi-pragmatist builds Q2's route C, by type identity

Written 2026-10-14 at about 11:40 by the coordinator, after the five reports and the critic's pass
2 (`../215-reports/critic-reports.md`, read it whole first, § 6 above all). The route the
coordinator would adopt for Q2 while the word B waits on its measurement and on the author: **C,
decided from the header by type identity, no new word and no spec token**: at `build`, where every
other header fact is told, a group record whose C struct holds (at any depth, through members,
arrays of them and anonymous members) a member whose type clang calls compatible
(`__builtin_types_compatible_p(__typeof__(((T *)0)->M), L)`) with one of the standard's lock types
`L` is a record C forbids to copy, and the program's copies of it are refused: a binding of it from
a place (`m2 = m`), a by-value parameter, a return, an element of a `[T]` (whose growth moves it,
spec § 13), and any Heroes record or array holding one by value, as copied. **The list of `L`**:
POSIX XSH § 2.9.9's set (`pthread_mutex_t`, `pthread_cond_t`, `pthread_rwlock_t`,
`pthread_spinlock_t`, `pthread_barrier_t`, `sem_t`), C11's `mtx_t` and `cnd_t`, Windows'
`CRITICAL_SECTION`, `SRWLOCK` and `CONDITION_VARIABLE`, each asked only where its header exists on
the platform. The critic's `../215-reports/critic-work/pass2/q2c/ident.c` and `identw.c` are the C
question, measured on three platforms.

**Build it** in your own copy: `git worktree add --detach .claude/worktrees/scratch-panel-215/ffi-build
7e5c02048` from the repository's root, the compiler built from the seed and then from `selfhost/`.
The dump route is the layout screen's (`selfhost/emit/layout_screen.hero`), today dumped only for a
record built from two or more fields; the critic counted 37 `partial` record lines in 35 files. Then
run, each with its command and what it printed:

- refused: issue 1335's copy, the critic's pass-1 `copy.hero` (10 sites), a counter returned by a
  constructor helper, a counter in a `[T]`, a Heroes record holding one copied, on this Mac, Linux
  arm64 and the Windows box (`CRITICAL_SECTION`, `SRWLOCK`);
- kept: the critic's `b.hero` and `race.hero`, the ten readers' `b.hero`
  (`../215-reports/llm-ergonomist-work/r*/b.hero`), both defect 413 goldens (`z_stream`, libuv: no
  lock, held in arrays on purpose), `examples/ctime` (a `partial` `struct tm`, no lock), every
  `@` lend and field lend;
- the census: `heroes check` and `heroes build --emit-c` over the 905 files with an `extern` (the
  brief's command), today against the route, how many move and why each;
- the cost: how many records the route dumps that today's screen does not, and a cold `--emit-c` of
  the compiler and of the corpus, user plus sys, the load said;
- the diagnostic's text: one error naming the record, the lock member by its C path, and the copy,
  with the spelling that works (`@`), per design.md §4.17.

Write `../215-reports/ffi-pragmatist-build.md` as you go: per shape, today against the route; the
lines by directory; what C cannot see (named: `z_stream`'s self-address, `TAILQ_HEAD`), which is
what B would add. Limits as your first brief: no paid run, no credential, no GitHub write, no push,
no commit, nothing outside the repository's root (never `/tmp`: set `TMPDIR` to your copy's `build/`),
your copy and your Windows folder removed at the end.

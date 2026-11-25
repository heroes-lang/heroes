# Panel 259, the historian's brief

Read `00-shared.md` first, whole: § 1 (the proposal), § 4.2 and § 4.3 (what the runtime sees and does not), § 4.7
(what is at hand), § 6 (the routes). The briefs were repaired from the critic's first reading
(`docs/panel/259-reports/critic-briefs.md`) and re-frozen at the base `3ec775b66` (`00-shared.md` § 2). **Your agent
file is the base's** (`.claude/agents/historian.md`): every factual claim carries a source, verified by a web search or
a page you read, and **anything you could not confirm is written `unverified`**. You have no veto. Your report is
`docs/panel/259-reports/historian.md`, written as you go (you have `Write` since 2026-09-29,
`.claude/skills/panel/SKILL.md` § 3c), in the tree the coordinator's prompt names. You read pages; you start no paid
run and save no file but your report.

**What only you can answer, and others will read from you** (the critic's first reading, § H, and its item 5): the
ffi-pragmatist has no page reader (`.claude/agents/ffi-pragmatist.md`: Read, Write, Bash), the sitting forbids a
download, this Mac holds Boost's headers and libraries but **no Boost assembly source** (`find /opt/homebrew -iname
'jump_*'` prints nothing), and **neither Linux image holds Boost** (`00-shared.md` § 4.7). So **Boost.Context's Windows
switch** (question 2's last part) and **every library nobody can build here** (libco, corosensei, `stacker`, greenlet,
Windows fibers, the Boehm collector, OCaml 5: questions 3, 4 and 6) are **yours alone**: the ffi-pragmatist cites your
report for them or writes *unread*. Write each such answer so it can be cited: the page, its URL, the day read, the
words quoted.

## The question you answer

**Who has met a stack switch the runtime is not told of, and what did they assume?** Every precedent below is what the
coordinator or the clerk recalls or was told, **none verified by this sitting**: each is a question for you, and a
precedent you cannot source is written `unverified` or dropped. For each, say which of five answers the system gave:
**(a)** it refuses or aborts; **(b)** it detects heuristically and warns; **(c)** it must be told of each switch; **(d)**
it proves which stack it is on by a fact no switch can fake; **(e)** it does not care, because nothing it keeps is
ordered by stack address. And what happened to it: the bugs, the false positives, the rule it wrote down.

1. **Go's runtime and cgo.** What happens when C calls an exported Go function (a cgo callback) on a stack Go did
   not make, or after C switched stacks (`makecontext` and `swapcontext`, a coroutine library)? Recalled, unverified:
   that the runtime reads or refreshes the stack bounds of the thread's system goroutine at each callback (a
   function named like `x_cgo_getstackbound` in `runtime/cgo`), and that cgo's documentation forbids C from jumping
   over Go frames. Find the issues and commits, their dates and Go versions, and quote the documentation's rule.
2. **Boost.Context.** `make_fcontext` and `jump_fcontext` are written in assembly (recalled): where may a context's
   stack come from (its stack allocators; whether any memory the caller supplies, the caller's own stack included,
   is allowed by its documentation)? Does it tell AddressSanitizer of each switch (a build option recalled as
   `BOOST_USE_ASAN`) or Valgrind (recalled as `BOOST_USE_VALGRIND`)? This Mac's headers answer part of it as files you
   can read: `/opt/homebrew/opt/boost/include/boost/context/fiber_ucontext.hpp` calls `__sanitizer_start_switch_fiber`
   under `#if defined(BOOST_USE_ASAN)` around a `::swapcontext` (lines 154 to 167), and `fiber_fcontext.hpp` and
   `continuation_fcontext.hpp` hold no `sanitizer` word (`grep -c -i sanitizer` prints 0 for each; Boost 1.92.0_1, `brew
   list --versions boost`): say what the documentation adds. **On Windows, does its x86-64 switch save and restore the
   thread block's stack fields** (base, limit, deallocation stack, fiber data)? The runtime reads that block to name a
   stack (`runtime/parts/stack_lane.c:89-100`), so the answer decides whether Windows has this defect for Boost, and
   **no other seat can read it** (above): quote the source you read (its file for `x86_64_ms_pe`, the line), or write
   `unverified`.
3. **libco**, the two libraries of that name (recalled: byuu's, for higan, `co_create` and `co_switch`; Tencent's,
   `coctx_swap`, with a *shared stack* mode). Which **copies stacks** in and out of one region, so that two
   coroutines' frames hold the same addresses in turn (§ 6 G of the shared brief)? Does either tell a sanitizer? Where
   does each take a coroutine's stack from, and does either document a stack the caller carves from its own?
4. **Rust: `stacker` and `corosensei`.** `stacker` grows a stack by running a closure on a new segment (recalled:
   through `psm`): what does it do about the stack bounds other code reads? **`corosensei`: does it link a
   coroutine's first frame record to the resumer's on purpose, so that a backtrace or an unwinder crosses the
   switch?** That one fact decides whether route C's frame-record walk (§ 6 C) is fooled by a real library. Does it
   support unwinding across a switch, and does it tell AddressSanitizer?
5. **AddressSanitizer's fiber interface.** `__sanitizer_start_switch_fiber(void **fake_stack_save, const void
   *bottom, size_t size)` and `__sanitizer_finish_switch_fiber(void *fake_stack_save, const void **bottom_old, size_t
   *size_old)` (declared in `tests/golden/run/fixedbugs-coroutines-switched-by-hand.h:22-23` of this tree, read): when
   was it added and why; **what does ASan do when it is NOT told** (recalled, unverified: a warning that it ignores a
   `__asan_handle_no_return` because the stack looks switched, *"False positive error reports may follow"*, with an
   issue of `google/sanitizers` cited in the message)? That is route A's precedent (detect and warn) and route D's
   (be told) in one tool. ThreadSanitizer's fiber calls (recalled: `__tsan_create_fiber`, `__tsan_switch_to_fiber`)
   and Valgrind's warning on a large stack-pointer change (recalled: *"client switching stacks?"*, the
   `--max-stackframe` option, `VALGRIND_STACK_REGISTER`) belong beside it if you can source them.
6. **Beside them, if time allows, each sourced or dropped**: Python's greenlet (stack copying on the thread's own
   stack), Windows fibers (`SwitchToFiber` and the thread block), the Boehm collector's calls for a coroutine's stack
   bottom (a runtime told of a switch), OCaml 5's fibers and C callbacks (a runtime that owns the switch).

## What the sitting does with it

The synthesis names, per resolution, the precedent it rests on, `verified` and re-read, or none
(`.claude/skills/panel/SKILL.md` § 4, `.claude/rules/records.md` § And the record says whose idea it was): a line of
prior art says **what the cited page itself states, and nothing of what Heroes did**. So quote the page's own words for
every precedent you mark `verified`, with its URL and the day you read it.

Output the structure of your agent file (`verdict`, `precedents` each marked verified or unverified, `argument` of at
most 120 words, `condition`, `search` with every query made and every source swept), then one line per numbered
question above: the answer (a) to (e), its source, and what would change it.

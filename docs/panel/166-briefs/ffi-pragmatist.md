# Panel 166 — brief for the ffi-pragmatist

**Read `docs/panel/166-briefs/00-shared.md` first.** Every figure in it was run.
Contradict it where you can run something that refutes it.

## Your question

`f.ptr()` shipped two days ago with two holes: an unchecked extent that corrupts
memory, and a write into an immutable binding. **Write the C, compile it, and say
which of the seven routes a real binding can live with.**

At panel 165 you found that `pipe(int [2])` does not cross, that route 3's extent
is unchecked, and that three call forms are byte-identical at `-O0`. This sitting
is where those findings decide something.

## The five things only you can settle

1. **What does a real C function that fills a buffer look like, and how many are
   there?** The shared brief counts 66 `ptr`-taking `extern` functions in this
   repository and 18 with a length-looking sibling, from a regex. **Read the
   eighteen** and say how many genuinely carry a count for that pointer, how many
   carry it for a different one, and how many carry none.
2. **`__counted_by` from the other side.** Panel 165's historian found that Swift
   READS clang's bounds-safety attributes and that `_LIBC_COUNT` expands to
   nothing on this Mac (verified: `clang -dM -E` has no
   `__LIBC_STAGED_BOUNDS_SAFETY_ATTRIBUTES`, and `clang -E -P` over `stdio.h`
   yields `char * tmpnam(char *);`). **Could Heroes WRITE the attribute instead —
   emit `__counted_by(n)` into the probe and let clang check the binding?** Build
   it with `-fbounds-safety` if that flag exists in your clang, and say what
   happens if it does not.
3. **Route F, the conservative one: refuse the `ptr` lend entirely.** You argued
   at panel 164 that the write direction had no door at all and that 50 of 141
   pointer parameters are non-`const`. If the lend goes, what breaks? Bind
   `getcwd` without it and show the result.
4. **The out-parameter that already works.** `spec § 13` says *"A C
   out-parameter is an `@` parameter"*, and that machinery ships for scalars and
   handles. **Run one** — `@n: u64` where C says `size_t *` — and say precisely
   what it does that the field lend does not, because route B is the claim that
   the field should join it.
5. **Attack the shapes beside the defect.** An extent that is a variable rather
   than a literal; a lend of a field of a field; a lend inside a loop; the same
   field lent twice in one call; a `u8[N]` against an `i8[N]`; and a field of a
   `partial` record, whose size is C's and not the field list's.

## What your verdict must carry

A verdict per route, the C you compiled with its diagnostics, the design.md
sections it rests on, and **one falsifiable prediction**. You hold a veto on ABI
breakage — and note that defect 063 is a memory-safety hole in shipped code, so a
route that leaves it open is not the status quo, it is a choice.

## Build guidance

`cp -r` the tree to your scratchpad, `rm -rf target build`. The seed builds in
~3 s. **Rebuild `heroes` before trusting it** — it is gitignored and can be older
than the tree. Never `archive/bootstrap-rs/`.

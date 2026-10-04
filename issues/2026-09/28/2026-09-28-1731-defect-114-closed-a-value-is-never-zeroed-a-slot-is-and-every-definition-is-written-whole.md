# Defect 114 closed: a value is never zeroed, a slot is, and every definition is written whole

2026-09-28, M-agreed-retention step 29, in lane 182 (`81532acc`, the trunk
merged into it at `6233d4a5`, design.md and the verification map at
`44a1196b`), merged `c8ed80bc`, on panel 182's provisional resolution
(`docs/panel/182-a-value-is-never-zeroed-a-slot-is-and-a-definition-is-whole.md`).

- [x] **114 — the emitted C zeroes every temporary of a function at its entry, so a lookup that returns early pays for every arm** | the emitter declares each temporary at the top of the C function with `= {0}`, and a `match` over a string of twenty arms declares them all: the emitted `h_keywords_keyword` zeroes 139 temporaries on every call to make 21 string comparisons, and on the default build line (plain `clang`, no optimisation, CLAUDE.md § Commands) every one is executed; `memset` was about a fifth of `fmt`'s samples after defect 105's repair | `selfhost/emit/` (the temporaries' declarations) · `seed/heroes.c` (`h_keywords_keyword`) · **closed 2026-09-28**

    **Origin:** lane 105's agent, 2026-09-27 (`memset` 240 of 1,113 samples
    of `fmt` on eight copies of `walk.hero`, under the lexer's keyword and
    punctuation lookups); the emitted function read by the coordinator in
    the trunk's seed at `2b1a1f24`: 941 lines, 139 `= {0}`, 21 string
    comparisons.

    **Why it is a defect.** Every Heroes program pays it, the compiler first:
    work the program never reads, done at each call. The repair keeps the
    guarantee the zeroing buys, every temporary initialised before any read,
    and moves the initialisation to where the temporary's life begins.

    **What it actually is, read in the emitted C, 2026-09-28** (the
    coordinator, on the trunk's seed at `c90fd658`). The zeroing is not
    waste: it is the release's precondition. Each arm of the `match` writes
    its value into an owned variable of its own (`h3_own3` to `h25_own25` in
    `h_keywords_keyword`), each store releases the variable's old value
    first, and the function's exit releases all 23, the arms not taken
    included, so an unzeroed one would be released uninitialised. `= {0}` is
    emitted only on a refcounted type (`emit/body.hero`, `is_refcounted`),
    and `TokenKind?` is one because its failure half carries strings. The
    cost is in the lowering's shape, one owned variable per arm where the
    arms could share one destination, and changing how a `match` lowers is
    the IR's architecture: a panel path (CLAUDE.md § 4), convened when the
    seats can sit again. Removing the zeroing without it would trade a
    robustness guarantee for speed, which § Precedence refuses.

## The repair

`emit/body.hero` never prints `= {0}` on a value or on an `@` parameter's slot,
and prints it on every other refcounted slot, and its header says why: every
store into a slot loads the old value first, so a slot is read before its first
write, while a value's one definition precedes every read on every path. The
verifier proves the second within a block too (`ir/values.hero`'s `in_order`,
with a test that makes it fire); before, its dominance compared blocks only.
The one place the emitter wrote a value in parts, `map_get`'s found path, writes
the option whole first, and the new suite `wholes` pins zero partial writes
over the seed and the blessed emission (on the trunk before, 30 files and 430
writes, 264 in the seed). `unread.slots_read` counts the exit sweep as a read.
design.md Part 5 states the ownership pass's rules as `ir/own.hero` does, which
slots are zeroed and why, and every exit edge as the emitted C writes it, a
dated note quoting the three sentences that were false. The note of 2026-09-28
in this item read the zeroing as the release's precondition: true of the slots,
false of the four fifths that were values.

## The gate

In the lane, one suite at a time, the compiler built from the regenerated seed,
the fixpoint by `cmp`: the compiler's 803 tests, the net's own 178; run 210,
determinism 240, ir 24, emit 8 (four goldens edited by hand, 86 lines, each a
dropped value zero), canonical 2, warnings 271, lines 211, corpus 55, check
153, annotations 199, surface 325, layout 4, order 3, records 24, probe 24, and
wholes, descriptors, cache and units, each 0 failed; `emission` blessed by its
own procedure, 259 files and 21,946 lines each way, every line a dropped value
zero (21,770), a dropped `@` parameter's slot zero (93, each with its copy-in)
or a tag write made whole (83), then 636 and 0. After the merge of the trunk at
`5fdd0edd`: the whole net, 3,120 passed and 0 failed; the compiler's 820 tests
and the net's own 178.

**Robustness**, the sitting's instruments on the landed seed: 21,566 `= {0}`
(106,734 before; 21,652 after the merge); clang's `-Wuninitialized
-Wsometimes-uninitialized -Wconditional-uninitialized` on the seed 0, and with
every remaining zero stripped 21,566 of 21,566 variables flagged, all slots, no
value; the critic's `defassign2.py` 0 values reached unwritten over the seed,
the blessed corpus and the 119 FFI emissions; a compiler built with
`-ftrivial-auto-var-init=pattern` compiles itself to identical bytes, and `run`
under the pattern on an emptied `build/` is 210 and 0; the ffi-pragmatist's
`boundary.py` 0 zeroed locals among 420 extern arguments, 25 addresses and 8
thunks.

**The time**, user seconds, three interleaved rounds on a still machine (load
1.60 to 2.31, real within 0.14 s of user plus sys): `fmt` on 36,688 lines 3.05
against 2.39 with the compiler built plainly (0.78) and 0.71 against 0.66 at
`-O2` (0.93); `check selfhost/main.hero` 4.29 against 3.34 (0.78) and 0.96
against 0.87 (0.91); `build --emit-c` 35.05 against 32.18 (0.92) and at `-O2`
within noise.

Linux arm64 on the lane's tree (`44a1196b`, the `heroes-linux-arm64` image):
the compiler's 820 tests and surface 332, canonical 2, annotations 210, check
161, fixes 30, layout 4, order 3, lines 207, run 206, emission 624,
determinism 236, wholes 302, descriptors 302, cache 6, grammar 9, spec 20 and
probe 24, each 0 failed. The Windows box on the same tree, one archive whose
sha256 matched on both sides (`c458cac54c35feba`), seed `c75c72eca1e93e88`:
the compiler's 820 tests and surface 325, canonical 2, annotations 210, check
161, fixes 30, layout 4, order 3, lines 205, run 204, emission 602,
determinism 234, wholes 302, descriptors 302, cache 6, grammar 9, spec 20 and
probe 24, each 0 failed; the cache key read from clang's `-###` answer holds
under Windows' clang. On the trunk after the merge (`c8ed80bc`): the fixpoint
by `cmp`, the compiler's 820 tests, the net's own 178, records 24, surface
332, spec 20, probe 24, wholes 302 and cache 6.

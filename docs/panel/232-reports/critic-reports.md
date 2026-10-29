# Panel 232, the completeness critic's second pass: the reports

Written as it went, 2026-10-29, after the five reports, against the frozen
tree `afe983265` in the critic's own detached worktree
`.claude/worktrees/232-critic/`, whose `heroes-next --version` read `heroes
0.5.0` / `sources 3e3c95dc246b73b3` before anything ran. Probes are in
`.claude/worktrees/232-critic/critic-scratch/` (`second/` for this pass),
each built by `heroes-next build` and its binary run under `timeout 20`, its
output through `head -c 4000`. No verdict on any proposal. No paid run, no
clock, no credential printed (`gh auth status` only). The seats' scratch
folders were read and never run in.

What was already filed and is not re-found here: defects 2253 to 2259
(titles read with `gh issue view`, all open).

Kinds of finding: **ROUTE** (a route nobody listed), **UNMEASURED** (a
claim asserted and not measured with the command that settles it),
**CONTRADICTION** (two seats, which is checkable, checked), **QUESTION**
(what the sitting should have asked), **RULE** (the author's rule held to a
route).

## Findings

### R1. CONTRADICTION, checked: the spec-warden's E1 against the compiler-engineer's § 1.3, and § 9's own UFCS sentence beside them

The spec-warden recommends E1, *An expression's parts are evaluated left to
right as written* (+18 real), as *true on all twelve shapes run*; the
compiler-engineer's § 1.3 found `.default(v)` evaluating `v` only when the
receiver is an error. Which is checkable: both, by running, and by reading
what a reader is given.

Command: `critic-scratch/second/run2.sh` over `d1_default_abort.hero`,
`d2_default_order.hero`, `d3_default_value_is_call.hero`; `sed -n
'3150,3172p' docs/design.md`; `grep -n -i -E 'default|must\(\)|is_err'
spec/heroes-spec.md`.

Printed: `d1`, `m["a"].default(xs[len(args()) + 4])` on a two-element
array: `1`, `then`, **exit 0**, the fallback's out-of-bounds read never made.
`d2`, `noisy("left") + m["a"].default(noisy("fallback")) + noisy("right")`:
`left`, `right`, `4`, no `fallback`. `d3`: `lookup a`, `1`, then `lookup z`,
`fallback`, `9`. design.md Part 5's sugar table rules it: *`.must()`,
`.default()`, `.is_err()` | a branch on the `ok`/`err` tag*, two rows above
*UFCS `x.f(y)` | `f(x, y)`*. The specification says *`.default(v)` | extract
or fall back* (§ 6) and *`x.f(y)` is sugar for `f(x, y)` (UFCS)* (§ 9).

Finding: the compiler and design.md agree; the specification, read with E1
and § 9's UFCS sentence, predicts the opposite: a reader of `m["a"].default(
xs[5])` derives `default(m["a"], xs[5])`, a call, its arguments evaluated
left to right, and predicts an abort the program never meets. So E1 as
recommended is false on a shape in the language's own table of failure
operations, and the recommended set (10,426 real) prices no clause for it.
The other branches are already said (`&&` and `||` short-circuit, § 7; `?`
propagates, § 6); `.default(v)` is the one branch the specification calls a
call. Routes nobody listed, none priced: § 6's row says `v` is read only on an
error (the way § 7 says short-circuit); E1 names the branches it does not
reach; or the compiler made eager, which design.md Part 5 refuses and which
changes `examples/ledger/db/sqlite.hero:280`'s `problem.default(why(db))` into
a call of `why(db)` on every success. No blind task measured it; the wording
for a paid task is a fourth program beside W, `m["a"].default(noisy(...))`,
under T and under E1, at the launcher's 0.14 USD a session. Defect 2254 (the
direct spelling refused at build) is the same row seen from the other side:
§ 9 promises a spelling that does not build, of an operation that is not a
call.

### R2. QUESTION the sitting did not ask: what *a defect of the compiler* is, when the thesis files its new refusals under defect numbers

The paragraph excludes *a program that depends on a defect of the compiler,
which a 1.x release may repair*, and defines no defect. Go's exclusion,
which the historian read and marked verified, is scoped: *a bug that
violates the specification*.

Command: in the critic's worktree, the thesis rules counted at each tag
(`git show <tag>:selfhost/diag.hero`, the `is_thesis_rule` list, the codes
`check --permissive` drops); `git log -L '/^function
is_thesis_rule/,/^    \]/:selfhost/diag.hero' v0.1.0..afe983265`, the
codes each commit added; `git show --stat` of the five `Defect` commits;
`grep -n -i -E 'only call itself|endless' spec/heroes-spec.md`; and
`critic-scratch/second/t1_endless.hero`, `function spin(n: i64) -> i64` /
`return spin(n: n + 1)`, checked plain and `--permissive`.

Printed: thesis rules **14** at `v0.1.0` (2026-09-07), 26 at `v0.3.0`, 27 at
`v0.5.0` and at `afe983265`: **13 joined in 52 days**, one of them
(`non_exhaustive`, decision 2078) a refusal already made and only classed
there, the others' commits saying something *is refused* or naming a new
code. Six of the 13 came in
commits titled `Defect NNN:` and saying *is refused*: `unreachable_statement`
(defect 174), `hole_without_f` (176), `undefined_header_name` and
`unshowable_name` (216), `package_comparison` (223), `endless_recursion`
(457). None of those five commits touched `spec/heroes-spec.md` (`--stat`);
the specification gained a sentence for two of the six in the same days, by
other commits (§ 8's *A statement after a jump, in its block, is a compile
error*, through the round's merge `d2d358a1a`; § 2's *A literal without the
`f` is an error where its braces ... would be a hole*, `0e715728d`,
2026-10-03), and holds none for the other four: no sentence refusing a
function that can only call itself (`grep -n -i -E 'only call
itself|endless'`, nothing), nor a header's or a package's name of the shapes
216 and 223 refuse (`grep` for *pkg-config*, *header's name*, nothing). `t1_endless.hero`:
`error[endless_recursion]`, exit 1; `--permissive`, exit 0: the control arm
gives it a meaning, and § 9 already says what it does (*recursion too deep
aborts*).

Finding: the project's own practice files a new refusal of a program the
specification allows as a defect repair, so the paragraph's first exclusion
as worded admits every new thesis rule, and *a program that compiles under
1.0.0 compiles under every later 1.x* binds nothing the thesis loop wants to
change; read Go's way instead, a 1.x could add no refusal of a program its
release's specification allowed, and the loop that put 13 codes on the list
in 52 days stops at the tag, its sentences landing beside the refusal
(two of the six) changing nothing, since they change the specification
after the program's release. Neither reading is written,
and no seat asked which. Routes nobody listed: the exclusion defined against
the specification as the program's release stated it (Go's words), so a new
refusal waits for `2.0.0`; or the paragraph names the thesis rules as their
own exclusion, said once, so a program's author knows a refusal may arrive
in a `1.x` (what C# put behind warning waves and Rust behind editions, both
modes, which *It runs one way* excludes after `1.0.0` and the author's rule
before it). The control arm already draws the line a definition needs:
`is_thesis_rule`, the codes `--permissive` drops.

### R3. ROUTE nobody listed, and a measured rate: every name a `1.x` adds refuses an older program, since a built-in's name is taken everywhere

The ffi-pragmatist asked it of C (finding 13, a runtime name added refusing
a program's own C) and left the Heroes side unrun: *whether a built-in a
`1.x` library adds can refuse a program that defined a function of that
name*. Run here.

Command: `heroes-next check` (and `--permissive`) on `b1_fn_named_keys.hero`
(`function keys(n: i64)`), `b2_binding_named_range.hero` (`range = 3`),
`b3_fn_named_clamp.hero` (`function clamp`, no built-in), `b4_binding_named_
join.hero`, and `n_<name>.hero` for `lease`, `end_lease`, `validated_bytes`,
`validated`, `ptr`; `kw_<word>.hero` (`<word> = 1`) for nineteen words other
languages use as keywords; the reserved inventory (`selfhost/inventory.hero`,
`table()`) at `v0.1.0` and at `afe983265`, by `git show` and `comm`.

Printed: `b1`, `b2`, `b4` each `error[builtin_name_taken]: ... the name is
taken everywhere`, exit 1 (`b1` refused under `--permissive` too); `b3`
exit 0. The inventory held **37** names at `v0.1.0` and **41** at
`afe983265`, the four added `end_lease`, `lease`, `ptr`, `validated_bytes`,
each refusing `<name> = 1` today, and `validated` likewise (reserved by
defect 501 on 2026-10-09, its comment says). `finally`, `except`, `lambda`,
`yield`, `await`, `async`, `then`, `elsif`, `loop`, `defer`, `self`, `do`,
`unless`, `nothing`, `undefined`, `interface`, `trait`, `impl`, `type`: each
`<word> = 1` checks clean, exit 0; `try = 1` is `error[reserved_word]`.

Finding: Go's built-ins are predeclared names a program may redeclare (its
specification, read 2026-10-29 into `critic-scratch/gospec.html`: *The
scope of a predeclared identifier is the universe block*; *An identifier
declared in a block may be redeclared in an inner block*), and Go 1.21
added three of them (its release notes, read the same day into
`critic-scratch/second/go121.html`: *Go 1.21 adds three new built-ins to
the language*, `min`, `max`, `clear`), so a Go program's own `min` stood
(an inference from the two pages, not a run); Heroes refuses the shadow (`builtin_name_taken` is a thesis rule) and
refuses a foreign keyword as a name, so a built-in, a reserved word or a
foreign word a `1.x` adds refuses every `1.0` program that used the name:
four names joined the reserved inventory in the 52 days measured and a fifth
was reserved by defect 501 (whether each was a free name the day before was
not run on an older compiler; `validated`'s comment says a program's own
was accepted until then), and any of the nineteen words above would be the
next. The paragraph promises *the functions it
names* and says nothing of the names a `1.x` may take. Routes nobody listed:
reserve now, before `1.0.0`, the names and words a `1.x` might take (a
refusal, which the author's rule allows, told); new built-ins and words only
in `2.0.0`; or a `1.x` addition reached through a name a program cannot
already hold (a module named by `use`). Not a route: a program's own name
winning over a later built-in, which is the shadowing the thesis refuses.

### R4. UNMEASURED by both wordings of item 3: § 13's two *aborts* are compile errors where the end is seen

Both narrowings keep *handing it to C again aborts, `==` on it aborts* (the
spec-warden's H1, 0 real; the ffi-pragmatist's route (b), +4 vendored); the
ffi-pragmatist's s1a was refused at build, and no seat asked what the
sentence says of it.

Command: `run2.sh` over `h1_eq_on_it_direct.hero` (`a = make_at(i: 0)`,
`release(o: a)`, then `a == nullptr`) and `h2_handing_it_again_direct.hero`
(the same, then `value(o: a)`), over the ffi-pragmatist's `slots.h`
(copied from the coordinator's `verify-ffi/`).

Printed: both **build exit 1**, `error[handle_used_after_end]: `a` was given
to `release` at line 10, which ended its life, and this reads it again`, its
note: *decided one function at a time ... an end on one branch, in an earlier
turn of a loop, under `when`, through a copy or inside a helper is the
runtime's to stop*. The specification names no such refusal (§ 13 read
whole).

Finding: the shape item 5 is about, a sentence saying *aborts* where the
compiler refuses, stands inside item 3 too, and both repaired wordings keep
it: a reader of H1 or of (b) has a sentence saying `value(o: a)` after
`release(o: a)` runs and aborts, and task X's ten T readers predicted the
abort their sentence said (`llm-ergonomist.md`); that a reader of § 13 does
the same is an inference, no task having asked it. The words the compiler keeps:
reading it after its end is refused where every path to the read has ended
it, and aborts where the end is a copy's, a branch's or a helper's. Unpriced.

### R5. QUESTION the sitting did not ask: whether the promise covers how much stack a program has, when the two default commands already disagree

Every seat treated item 4 as a sentence and a crash (defect 2253). None asked
what *does what the specification says it does* means for a program near
the stack's end, which the specification leaves to the machine (§ 9: *a
source nested up to 2,000 deep compiles ...; recursion too deep aborts*,
how deep unsaid).

Command: `heroes-next build --help` (its `build` and `run` entries);
`ulimit -s`; `run2.sh f1_two_mb_frame.hero` and `run2.sh
f2_two_mb_frame_o2.hero -O2`, both the spec-warden's `k2_two_mb_frame.hero`
copied byte for byte from its scratch.

Printed: the help: `build` *-O0 compile at -O0 (the default here)*, `run`
*-O2 compile at -O2 (the default here)*. `ulimit -s` 8176. At `-O0`:
`start`, exit **139**, stderr empty (defect 2253's shape). At `-O2`:
`start`, `1`, exit **0**. The spec-warden measured the same source at exit 0
under `ulimit -s 65520`, and the compiler-engineer `bigframe18` at `-O2`.

Finding: one source completes under `heroes run` and dies under `heroes
build`, on one machine and one compiler; repairing 2253 turns the death
into an abort and leaves the two outcomes. So whether a program runs out of
stack is the optimisation level's, the shell's `ulimit` and the compiler's
frame layout, and a `1.x` lowering that adds a temporary at `-O0` can make a
`1.0` program that completed abort, with every sentence of the
specification kept. The paragraph excludes three things and not this; Go's
page excludes *performance* in its own words, which the historian quoted.
Route nobody listed: the paragraph says how deep a program may call and how
much it may hold is the machine's and the compiler's, never the promise's;
or the promise names a floor (a depth every `1.x` keeps), which needs an
instrument no seat priced.

### R6. UNMEASURED: the promise says *compiles*, its instrument asks `check`, and `check` reads no C header

`tests/harness/suite_compat.hero`'s header: *Each is asked `check` by the
compiler under test*. The paragraph: *compiles under every later 1.x
compiler*. No seat asked whether the two verbs agree.

Command: `heroes-next check` over every case of `tests/golden/unsupported/`
(`printf '%s\0' *.hero | xargs -0 -n 1 -P 4`), counting exits; `sed -n
'160,212p' tests/harness/suite_golden.hero` (the form's `verb: "build"`,
`want_code: 1`); `check` and `build --emit-c` of
`ffi-a-char-field-declared-u8-is-told-i8.hero`; `git grep -l -E '^extern "'
v0.5.0 -- examples`; then `check` and `build --emit-c` of the 21 programs it
reaches, in the first pass's unpacked `critic-scratch/compat/compat-v0.5.0/`.

Printed: **371 of 371** `unsupported` cases exit 0 under `check`, every one
a case the form holds to `build` exit 1 (its definition; one run here, the
`char` case below). The `char`
case: `check` exit 0, `build --emit-c` exit 1 `error[ffi_field_type]`, a
rule panel 194 added on 2026-10-06 (its header). At `v0.5.0`, 20 programs
bind a C header in their own file and `examples/ledger` through
`db/sqlite.hero`: **21 of the 69**; today each of the 21 exits 0 under both
`check` and `build --emit-c` on this Mac.

Finding: the suite sees nothing of the C boundary, nor any refusal only the
build makes (defect 2254 is the non-C instance): a `1.x` that refuses a
binding at build, a stricter FFI rule like panel 194's or a header that
moved as the ffi-pragmatist's finding 8 measured, reads green on 21 of the
corpus's 69 programs while none of them builds. Today's corpus builds, so
the gap is the instrument's, not yet a program's. Route nobody listed: the
suite asks `build --emit-c` (clang's rounds, no link), skipping a program
whose header the machine lacks as the `unsupported` form does
(`library_skip: true`); or the paragraph says *checks*, and says what the
build adds is outside it.

### R7. QUESTION: which reading of the blind seat pays a sentence, when the registered effects on O, S and W are answers whose lines did not change

The blind report gives both readings and says the registered one stands.
What no report does is say which verdicts turn on the difference.

Command: `critic-scratch/second/blind_join.py` (`python3 -I`), read-only:
the sealed key joined with `scores.tsv` and each `answers/<n>.txt`, per task,
specification and model, the registered right count, the answers whose
`answer` section opens with the truth's lines, and of those the ones prose
follows.

Printed (registered / lines first / of which prose follows, of 5): **W** T
Haiku 5/5/0, T Sonnet **0**/5/**5**, P Haiku 5/5/0, P Sonnet 3/5/2; **S** T
Haiku 3/4/1, T Sonnet 0/5/5, P Haiku **2**/5/3, P Sonnet 3/5/2; **O** T Haiku
3/3/0 (42 printed the values for the labels, 47 hedged), T Sonnet 3/5/2, P
10 of 10; **X** T 0/0/0 (all ten `before`, `ends: abort`), P Haiku 3/5/2, P
Sonnet 2/5/3. Prose after correct lines: Sonnet 19 of its 40 answers, Haiku
6 of 40. The coordinator's totals reproduce exactly.

Finding: **every one of the 20 W answers predicted the program's lines**;
W's registered +3, the one effect the spec-warden scored a hit and the one
it names as paying M10a (*W, P − T ≥ 3 of 10 (pays M10a)*), is three Sonnet
answers that stopped writing *the spec doesn't say ... I assumed*, and its
own condition (*W's P − T ≤ 1 → I object to the write clause as well*) reads
+3 on one reading and 0 on the other. S's registered +2 holds a Haiku cell
that went down under P (3 to 2), its answers quoting the new sentence after
the right lines. What the registered rule measured on O, S and W is mostly
whether a model writes a caveat, which is a model's habit (19 against 6),
not a reader's prediction; on X both readings agree, ten of ten moved. The
sitting must say which reading pays a sentence before item 1's write clause
and item 2's sentence are called paid, and whether a caveat removed is an
effect the thesis counts. The recommended wordings themselves (E1 with M10a,
S3, X4, K1, H1) were not what the blind seat read (P is E0, S0, X0), and
items 3 and 4 had no reader at all. And O's two wrong T answers are not
both about order: `answers/42.txt` printed `left`, `right`, `3`, `4` (the
record's values for its labels) after *I took source order*, and `47.txt`
hedged (*the order of the four labels is not determined*); so the order
sentence moved one prediction in the 40 sessions of O and W, which is what
item 1's clauses rest on besides the promise (decision 2217's *A*).

### R8. ROUTE beside defect 2258: every stop raised inside a bound C function names no place, `hero_os.h`'s included

The paragraph promises *saying why and where* and covers `hero_os.h`;
the ffi-pragmatist found one place-less stop (`==` on a dead handle, now
2258) and asked no further.

Command: `run2.sh` over `p1_join_twice.hero` (`hero_thread_spawn` then
`hero_thread_join` twice, bound from `hero_os.h` as `examples/threads`
binds them), `p2_sort_nan.hero` (`sort` of an array holding a computed
`nan`), `p3_index_read.hero`; `grep -rn -E '\bhero_panic\s*\(' runtime/parts/*.c
| wc -l` and the same for `hero_panic_at(`; `sed -n '1,45p'
selfhost/emit/stop_place.hero`.

Printed: `p1`: `first join: 2`, then `panic: a thread was joined twice, or
joined before it was started ...`, **no `at` line**, exit 134. `p2`: `panic:
sort of an f64 array containing nan` / `at p2_sort_nan.hero:6:10, in
p2_sort_nan.main`. `p3`: `at p3_index_read.hero:3:11`. The runtime's parts
hold **99** calls of `hero_panic(` and 6 of `hero_panic_at(`; a place
reaches the runtime only as the four arguments an `_at` entry point takes
(`stop_place.hero`'s header), and a function bound by `extern` is called
with the program's arguments alone.

Finding: the place panel 230's R1 gave reaches the language's operations
and the library's, never a function the program binds, so a stop inside one
of `hero_os.h`'s 57 (threads, processes, directories: `spawn.c`, `run.c`,
`dir.c`, `os.c` among the 99 sites, a count of sites and not of reachable
stops, unrun beyond `p1`) says why and not where, and 2258 is one of a
class. The paragraph promises the place for the header it promises. Routes
nobody listed: the generated call to a `hero_os.h` function hands its site
the way a library call does (the header's functions gaining the `_at` form
panel 230 gave the runtime's own entry points, whose place-less twins task
2237 removes); or the paragraph says *where* of the language's own
operations only.

### R9. RULE, two places the author's rule bites that no seat named: task 2237's old names, and `--permissive` as the answer to R2

**(a) Task 2237 keeps an old spelling beside the new.** The ffi-pragmatist's
brief and report name 2237's removal (*kept beside their `_at` twins while a
seed called them, removed after the release whose seed knows the new ones*,
the issue's body, read in the first pass into `critic-scratch/issue2237.md`)
and price it as a freeze, never against CLAUDE.md § 2's *no old spelling
kept beside the new, no deprecated alias*.

Command: for each of the eight place-less entry points `heroes_runtime.h`
declares beside an `_at` twin (`hero_panic_overflow`, `_must`, `_assert`,
`_assert_sides`, `_assert_str_sides`, `_unmatched_case`, `_unmatched_value`,
`_unmatched_str`), `grep -c` of its call in `seed/heroes.c` and of its
`_at` twin's; the same of `hero_panic_must` in `v0.5.0`'s seed; then the
`v0.5.0` seed's first 64 lines (`git show v0.5.0:seed/heroes.c | head -64`)
and today's, each `clang -fsyntax-only -I runtime`.

Printed: today's seed calls **none** of the eight (0 each) and their twins
7,101, 2,381, 30, 0, 0, 1, 1, 1 times; the header still declares all eight.
`v0.5.0`'s seed calls `hero_panic_must(` 2,382 times and its twin 0 times,
and **cannot compile against today's runtime at all**: `error: static
assertion failed due to requirement '31 == 30': heroes_runtime.h is from
another compiler`, exit 1; today's head exits 0.

Widened the same way (every `hero_*_at` the header declares whose
place-less name it also declares, `grep -c` of the old name in today's
seed): **29** stand beside a twin (the header's own words: *Each `_at`
below is an old entry point's twin ... the old one unchanged beside it*);
today's seed holds 27 of them 0 times, `hero_handle_alive` once (a string
`emit/callback_thunk.hero:115` still writes, so that one is in use) and
`hero_panic` 3 times (the runtime's general stop). The eight above appear in
`selfhost/` only in `emit/guarded_names.hero`, whose list is *every word
`heroes_runtime.h` and `hero_os.h` write*, a second cost of keeping them,
and in the seed once each, as that list's strings (`grep -o` of each name
not followed by `_`: one hit each, `seed/heroes.c:4579-4580` among them);
whether the emitter writes any of the other 19 for a program's C
(`emit/builtins.hero` names some as bases) was not read one by one.

Finding: the eight serve no seed this runtime can compile, the release's
included, and the compiler neither calls nor writes them; what still names
them is a program's own `extern "heroes_runtime.h"` (the door of defect
2256) and the guarded-names list that mirrors the header. They are a
deprecated alias kept until a release for an older program's sake, which
the author's rule refuses in so many words; whether the rule
reaches the runtime's C names or only the language's forms is unruled, and
the sitting is where it is ruled: if it reaches them, 2237 is owed now, not
after a release; if not, the sitting says so beside its ruling on
`heroes_runtime.h` (the ffi-pragmatist's condition (2)).

**(b) `--permissive` is the one mechanism in the tree that would keep an
older program compiling across a new thesis rule.** `build --permissive`
*drop[s] the diagnostics that exist for the thesis and build[s] what is left*
(`heroes-next build --help`), and `check --permissive` passed R2's
`t1_endless.hero` (exit 0) where `check` refused it. No seat proposed it;
it is the route R2's question invites next, and offered as that answer it
is a mode kept for compatibility: refused before `1.0.0` by the author's
rule, and after it by *It runs one way*. Named here so it is refused before
it is weighed.

### R10. UNMEASURED, its compile half measured here: on Linux a frame past the guard may be a write, not a death, and defect 2253 says a death

The compiler-engineer left it *a question rather than a premise*: with no
`-fstack-clash-protection`, *whether that is a silent 139 or a write into
another mapping was not run on Linux*. Defect 2253's title says *dies at
139*.

Command: `critic-scratch/second/bigframe.c`, one function holding a 4 MiB
array (`bigframe18`'s size), compiled with `heroes`' own flags that bear on
it (`-std=gnu11 -fsigned-char -fno-optimize-sibling-calls -O0`, from
`selfhost/cli/flags.hero:130-150`) by this Mac's clang (`Apple clang version
21.0.0`) for four targets, `-S`, and the assembly grepped; `grep -rn
guardsize runtime/`; `sed -n '660,692p' runtime/parts/stack.c`.

Printed: `arm64-apple-macos14` calls `___chkstk_darwin` (2 lines) and
`x86_64-pc-windows-msvc` `__chkstk` (1); **`aarch64-unknown-linux-gnu`**
moves `sp` in one instruction, `sub sp, sp, #1024, lsl #12 // =4194304`,
and **`x86_64-unknown-linux-gnu`** `subq $4194320, %rsp`, neither with a
probe or a probe loop. No `guardsize` anywhere under `runtime/`, so a
thread's guard is the C library's default (unread here). The handler
claims a fault only within `HERO_STACK_WINDOW`, 1 MiB.

Finding: on both Linux triples nothing touches the pages between the old
`sp` and the new, so the frame's first store, or the call's return address,
lands up to 4 MiB below the old `sp` without a page between being touched;
whether that address is unmapped (a fault, the handler's 1 MiB window
missing it as on Darwin) or another mapping (a thread's neighbour, written
in silence) is what lies below each stack, the main thread's and a spawned
one's, and is unread here. Which the CI's Linux legs show is **unrun** (the
CI's clang is not this one, and no Linux machine was used), so 2253's *dies
at 139* is a Darwin measurement, and on Linux possibly design.md § 1.12's
worse half, a write. The
compiler-engineer's 12-line witness repairs Darwin alone (its own words);
the route nobody priced is `-fstack-clash-protection` in `flags()` for the
Linux targets, which turns the jump into the probe the witness already
reads, at a cost no seat measured.

### R11. ROUTE nobody listed: § 6 promises codes are stable, and the one code every narrowing conversion fails with is named nowhere

The spec-warden's caution for item 7: § 6's *Codes are stable snake_case
strings* is about a program's own `fail` codes, *which the specification
already promises by naming them*. Not every one is named.

Command: `grep -o -E 'fail\(code: \\?"[a-z_]+' selfhost/library_source.hero`
and `grep -rn` of the runtime's `HERO_STR_STATIC(code, ...)`
(`runtime/parts/failure.c`); `grep -c <code> spec/heroes-spec.md` for each;
`run2.sh c1_code_of_a_conversion.hero` (`match to_u8(len(args()) + 300)`,
printing `e.code`).

Printed: the codes a program can receive from the library and the runtime
are `does_not_fit`, `file_not_found`, `not_found`, `not_text`, `null_cstr`,
`read_failed`, `write_failed`, `missing_key`; the specification names seven
of the eight (1 or 2 lines each) and `does_not_fit` **0** times. The program
printed `does_not_fit`, exit 0.

Finding: `to_i8` to `to_u64`, eight built-ins, fail with a code § 11 never
names (*give a `T?`, because the number may not fit*), and `exit`'s abort
reports it too (F7); a program written from the specification cannot match
it except by reading the compiler, and the promise § 6 makes covers a string
§ 6 never states. Routes nobody listed: § 11's conversion sentence names the
code, for a few tokens unpriced; or the paragraph says a code is promised
where the specification names it. The same question one level down is item
7's (diagnostic codes); this one is a program's.

### R12. QUESTION the sitting did not ask: does `1.0.0` promise the `heroes` command, its verbs, flags, defaults and exit codes

Item 7 asked about diagnostic codes; nobody asked about the command that
reads the programs, though the historian quoted Go's *Tools* exclusion
(*scripts that depend on the location and properties of the tools may be
broken by a point release*).

Command: `grep -n -E '`heroes|heroes (build|run|check|test)'
spec/heroes-spec.md`; the `Command(` rows of `selfhost/cli/table.hero` at
each tag; `sed -n '38,46p' .claude/rules/cli-surface.md`; the
`heroes-next build --help` read for R5.

Printed: the specification names no verb (nothing). `Command(` rows: 11 at
`v0.1.0`, 13 at `v0.3.0`, `v0.5.0` and `afe983265`. cli-surface.md: *exit 0
clean, exit 1 the input has diagnostics, exit 2 the tool could not run*,
in a rules file. `build` defaults to `-O0` and `run` to `-O2` (R5).

Finding: the paragraph covers *the language the specification describes*,
and the specification describes no command, so as drafted the verbs, the
flags, their defaults, the exit-code contract and `check --json`'s schema
are all outside the promise without a word saying so, while a program's
build, its CI and an editor reach the language only through them; a `1.x`
that moved `build`'s default to `-O2` would change R5's program from a
death to a completion with no sentence of the specification moved. Routes
nobody listed: the paragraph names the command's contract (the three exit
codes, `--json`'s schema, item 7's route) as promised and the rest outside,
Go's way; or all of it outside, said.

### R13. CONTRADICTION, checked: 66 of the 69 have an expected output, not 67, and the error was this critic's (F23)

The compiler-engineer's item 8 table says *66 expected outputs*; the shared
brief and `paragraph-draft.md` say *67 of the 69*, citing F23.

Command: for every file of `v0.5.0`'s `examples/` holding `^function
main(`, whether `<dir>/<name>.expected` exists at the tag (`git cat-file
-e`); `git ls-tree --name-only v0.5.0 examples/calculator/`.

Printed: **66 with, 3 without**: `examples/calculator/whole.hero`,
`examples/curl/main.hero`, `examples/gallery/09-holes.hero`. The calculator
holds two programs, `main.hero` and `whole.hero`, and one `main.expected`.

Finding: F23 counted `main.expected` as covering both of the calculator's
programs; the compiler-engineer is right. Corrected here, under the first
pass's record rather than in it; the paragraph draft's *exist for 67 of the
69 examples* carries the error.

### R14. ROUTE nobody listed: the paragraph has no exclusion for a repair robustness demands of a program the specification allows

The historian read and marked verified Go's first exclusion, *Security* (*a
security issue in the specification or implementation may come to light
whose resolution requires breaking compatibility*), Nim's (*if a security
vulnerability is discovered in the standard library, we reserve the right
to break code which uses it*) and RFC 1122's soundness clause (*minor
releases may only contain breaking changes that fix compiler bugs or other
type-system issues ... soundness issues*). The paragraph's three
exclusions hold none of them, and no seat proposed one.

Command: `grep -n -i -E 'security|soundness|robust'` over
`paragraph-draft.md` and the four seats' reports; the labels of defects 2253
to 2259 (`gh issue view`); § 13 read whole.

Printed: no hit in the draft; in the reports only the ffi-pragmatist's and
the spec-warden's *robust* as the name of a route. Six of the sitting's
seven defects are `class:blocking`, and the ffi-pragmatist's robust route
for 2256 refuses a program the specification allows today (§ 13 lets a
group name any header, `heroes_runtime.h` included).

Finding: robustness outranks every rule here (CLAUDE.md § Precedence, rank
3; design.md § 1.12), and the sitting found six blocking holes in one day;
the next one found after `1.0.0` whose repair refuses a program the
specification permits has no clause to land under (*a defect of the
compiler*, read Go's way, is a compiler that disobeys the specification, and
read the project's way it swallows everything, R2; *an error in the
specification* is a sentence that is wrong, not one that permits too
much). Route nobody listed: an exclusion in the language's own terms, a
repair that keeps a program from crashing or corrupting memory, which may
refuse or stop a program that did, said once; Go reserved exactly that
right and the historian's page says so.

### R15. ROUTE, the shapes beside the recommended item 1 wordings: an index read inside a written place aborts before the value, and L1 sits inside a bullet scoped to same-typed pairs

**(a)** M10a says *An out-of-bounds index or slice aborts, in a place
written with `@` after its value or the call's other arguments*. Run beside
it: `m2_index_inside_place_index.hero`, `ys[zs[noisy("inner", 5)]] @
noisy("value", 1)` on two-element arrays: `inner`, then `panic: array index
out of range` at 8:8, `value` **never printed**, exit 134. The read `zs[5]`
stands inside a place written with `@` and checks at once, as every read
does (the compiler-engineer's 1.2); a reader of M10a has a sentence telling
it that indices in such a place wait for the value. E0's *evaluates its
place's indices* holds it (evaluating `zs[5]` is a read); M10a's *in a place
written with `@`* does not say which index. None of the spec-warden's twelve
shapes put a read inside a written place's index. (`m1`, a missing key under
a field, `m[k].a @ v`, is refused, `error[no_such_field]: `Pair?` has no
fields`, which § 10's `V?` already says.)

**(b)** L1 changes *a call through a value names each in its own position*
to *a call names each in its own position*, inside the bullet that opens
*When two parameters in a signature share a type* (§ 9, read whole). The
rule it states binds every call: `l1_labels_no_pair.hero`, `add2(b: "xy",
a: 1)` with `a: i64, b: str`, is refused by `check --brief` with **four**
diagnostics, two `wrong_label` and two `type_mismatch` (the spec-warden's
row, reproduced). So a reader of L1 still meets the order only where two
parameters share a type, and the case the compiler refuses most loudly is
the one outside the bullet. Neither is priced.

### R16. CONTRADICTION, partly checkable: *It runs one way* is an absence the document already rules (spec-warden) or a choice every precedent later reversed (historian)

The spec-warden: *its second states an absence the document's silence
already rules*. The historian: *Two departures should be chosen, not
inherited. It runs one way as a 1.x rule: Go began undeclared, then made the
go line "a mandatory requirement" (1.21) and gated a meaning change on it;
Rust added editions after 1.0; Nim 1.0's guide lists `--useVersion`*.

Command: `grep -n -i version spec/heroes-spec.md`.

Printed: one line, 75, inside *conversions*: the specification holds no word
for a version.

Finding: the checkable half agrees with the spec-warden: as a fact of
`1.0.0` the absence is already the document's. The other half is what F14
asked and nobody ruled: whether the paragraph promises the absence for all
of `1.x`, the one promise every precedent the historian read did not keep,
and the one R2 and R3 lean on (a refusal or a name a `1.x` adds has no
version to hide behind). The sitting should rule it as a rule or as a fact,
in those words.

## Beside the items: unfiled, each searched

- **The `exit` abort names a `.must()` the program never wrote** (F7, the
  compiler-engineer's item 5 table, *panic: .must() on an error:
  does_not_fit: ...* for `exit(code: 255 + 1)`): searched `gh issue list
  --state all --search` for *exit abort names must*, *library must exit
  report*, *does_not_fit exit*, *exit(code:) takes 0 to 255*, *names a
  .must() the program never wrote*: no issue on the shape (hits: 1066, 2083,
  812 and others on other shapes). A report that sends its reader to a
  `.must()` absent from their file is design.md § 4.17's *without opening
  another file* broken; its class is the coordinator's.
- **One swapped pair of labels is four diagnostics** (R15 (b), the
  spec-warden's row): searched *wrong_label type_mismatch one mistake*,
  *labels out of order four diagnostics*, *wrong_label twice*: no issue on
  the shape (hits 1415, 2183, 2184, 2175, 1126 on other label defects).
  verification.md's `adjacent`, *a second message for one mistake*.

## Run, and found nothing

- **A Heroes function bound as a `SIGALRM` handler that allocates**, beside
  defect 2257 (`a2_handler_allocates_short.hero`: `signal` from
  `signal.h`, `ualarm(usecs: 200, interval: 200)` from `unistd.h`, a loop of
  2,000,000 `repeat`s in `main`, the handler a `repeat` of its own): printed
  `102000000`, exit 0, run once. The 200,000,000-turn version
  (`a1_handler_allocates.hero`) reached `timeout 20` (exit 124, no process
  left by `pgrep`), unread as slowness or a lock. No crash shown; a Heroes
  function running at an arbitrary instruction of a runtime that is not
  reentrant stays a question, not a finding.
- **The v0.5.0 corpus's 21 C-binding programs** all `build --emit-c` clean
  today (R6).

### R17. CONTRADICTION: the stack sentence, K0 (the compiler-engineer) against K1 (the spec-warden), and the platform neither can promise yet

The compiler-engineer: *adopt "a call chain deeper than the stack aborts"*
(K0, +5 real). The spec-warden: *approve K1, "running out of stack
aborts"* (−1 real), *object to landing any wording before the large-frame
defect ... is filed*, K0 naming *a chain* where the failing shape is one
call. Both say the runtime has the bug (CLAUDE.md § 12).

Checkable half: the shapes each wording must cover were run by both seats
and here (R5: `f1_two_mb_frame.hero`, `main` calling `make` once, 139 at
`-O0`); a chain of two calls is *deeper than the stack* only in bytes, which
K0's words do not say, and on a spawned thread (the compiler-engineer's
`thread_recursion`, `bigframe18_thread`) *the stack* is that thread's, which
K0 leaves to the reader and K1's *running out of stack* does not need to
say. Unchecked half:
which wording a reader predicts from, a blind task neither seat priced (the
wording for one: `bigframe18b` as task, once 2253 is repaired, under K0 and
under K1, 2 specifications × 2 models × 5 = 20 sessions, at most 2.80 USD at
the launcher's bound, the measured mean being 1.5590 / 80 a session).
And the spec-warden's own condition (*a shape that runs out of stack
unreported which no repair can reach on some platform → the sentence names
only what can be promised*) is open on Linux by R10, for both wordings.

### R18. CORRECTION, this critic's F10: `thread_deep` is no thread, and no thread's stack is smaller

The compiler-engineer's 4.1: *F10 and the brief name `thread_deep` as a
thread's smaller stack; it is not one.* Checked: `head -12
tests/golden/surface-fixtures/recursion521/thread_deep.hero` is `down(n)`
recursing on the main thread, its header *`direct`'s shape in a module whose
C component is `threaddeep`*, and `grep -c hero_thread_spawn` of it is 0;
`stack.c:311-316` names it for its spelling. F10's *a thread's smaller
stack* was a premise; the compiler-engineer read `spawn.c:88-117`'s floor
(a spawned thread's stack never smaller than `main`'s) and ran the thread
shape on `hero_thread_spawn`. Corrected here, under the first pass's record.

### R19. QUESTION: what moves `1.Y` and what moves `1.Y.Z`, which the promise's readers will read off the number

`.claude/rules/records.md` § Release tags (line 171): *While `X` is 0, `Y`
moves when `git diff vA vB -- spec/heroes-spec.md` is not empty and `Z` when
it is*. The rule says nothing of `X` at least 1, and the paragraph says
nothing of the number either; `README.md:131-133` holds `0.x`'s one sentence.
Under the paragraph a `1.x` may change the specification (an error in it
corrected, the third exclusion), so by today's rule read forward every such
correction is a minor release and every compiler repair a patch, which is
semver's reading only by accident. Unasked by any seat; the record's rule is
CL-067's and the decision issue of 2026-09-07 (records.md), the place the
sitting would amend.

## The historian's quotations, spot-checked on the raw pages

The historian read every page through a fetch tool that *returns a model's
extraction* (its method note), and `.claude/rules/records.md` measured 33 of
90 such lines saying more than their page. Five load-bearing ones re-read
here by `curl` into `critic-scratch/second/pages/`, tags stripped, read
2026-10-29: csharpstandard #169, *essentially a bug*, *most elegant and
orthogonal*, *do nothing and live with the discrepancy*, *We know Roslyn
deviates from the spec*, *known-deviation*, each found (2, 2, 2, 3, 3 lines);
the Rust Reference's *The assigned value operand is evaluated first,
followed by the assignee expression* and *right operand is evaluated before
the left one*, 1 each; Swift's `Integers.swift`, *performs a \*smart
shift\*, which defines a result for a / shift of any value* (lines 1150 and
1190, the historian's words with the markup and the line break dropped);
go1compat's *Compatibility is at the source level* (the first pass's copy),
1. Every one holds.

## Method notes

- **A slip, said**: while running R6's `build --emit-c` loop this critic
  wrote a temporary file to `/tmp` (`/tmp/x_emitc_err_<pid>`), one per
  program, each removed in the same turn of the loop; `ls
  /tmp/x_emitc_err_*` afterwards found none. It broke the binding *no file
  outside the repository's root*; nothing else did.
- Every count above came from the command beside it; every *unrun* is said
  where it stands.

### R20. QUESTION: § 10 leaves a map's order open, the runtime fixes it on purpose, and the compiler's own output depends on it

The draft excludes *what the specification leaves open, such as the order
of a map's keys*; F22 named Go's route (an order that varies so no program
comes to depend on it) and the historian verified Go 1's words for it (*the
order ... is defined to be unpredictable ... very likely to break early*).
No seat priced it.

Command: `sed -n '14,30p;74,82p' runtime/parts/map.c`; `grep -rn -E
'keys\(' selfhost | grep -v 'sort('` counted; `git grep -n -E 'keys\('
v0.5.0 -- 'examples/*.hero'`, and those without `sort(` read in their
context (`git show v0.5.0:<file> | sed -n ...`). (A first count with `\b`
in `git grep -E` matched nothing at all, and is discarded.)

Printed: map.c: *The seed is FIXED, and it is not a style choice: the
M-selfhost-fixpoint fixpoint compares generated C byte for byte, and a seed
that varied per run would make the compiler produce two different correct
outputs*; `HERO_MAP_SEED` *makes iteration order, and therefore generated
C, reproducible across runs (panel 006)*. `selfhost/`: 64 lines naming
`keys(` with no `sort(` on the line (some only take `.len()`). `examples/`
at `v0.5.0`: 33 lines, 8 without `sort(`, none order-dependent as read:
two comments, two `.len()`, `json/value.hero:120` a sum,
`query/run/engine.hero:198` sorted two lines on (*a report that walked the
map as it came would print a different table on a different day*), `:299`
a test's assert.

Finding: Go's route is refused in the tree already, by panel 006 and the
fixpoint (CLAUDE.md § 7, byte-identical emission), and the sitting should
say so rather than leave the route listed. What stays open: the largest
Heroes program depends, by its runtime's own comment, on the order § 10
calls *no order*, so a `1.x` that changed the hash or the seed would keep
every promise the paragraph makes and still change every unsorted program's
output, the compiler's emission included; and a reader told *in no order*
writes `sort(keys(m))`, as every example does. The question nobody asked:
does § 10 say what the runtime keeps (*the same order on every run, in no
order the program may rely on*), or does the paragraph name the hash as free
to move in a `1.x`.

### R21. QUESTION on the compiler-engineer's item 7 route (this critic's own F24): moving `"schema"` on a renamed code versions the codes, it does not promise them

The compiler-engineer's verdict: *promise the code, not the words, through
the schema's rule*, the rule being *a code renamed, split or withdrawn moves
`"schema"`*, once a list of the 285 exists.

Command: `sed -n '36,42p' selfhost/cli/check_json.hero`; `git log -G` of the
constant; the historian's item 4 read again.

Printed: `constant SCHEMA: i64` / `2`, moved last by `a42553adb`
(2026-10-06, *Defect 271, its JSON half*). The historian's precedents: Swift
SE-0443, *a stable identifier for an error or warning ... can never become
narrower*; SARIF's `deprecatedIds`, *identifiers by which this report was
known in some previous version*; Go's `codes.go`, *Error code values should
not be changed: add new codes at the end*.

Finding: under the route a `1.x` may rename any code so long as the number
moves, and every tool matching the old code breaks at that release, told;
that is a versioned format, the weaker of two promises, and the verdict's
word *promise* covers both. The precedents read are the stronger one (a code
never changes or narrows; Go and Swift), or the stronger one with an alias
(SARIF's `deprecatedIds`, an old spelling kept beside the new, which before
`1.0.0` meets the author's rule if the rule reaches what the compiler hands
a tool, R9's open question again). The sitting should say which: codes fixed
for all of `1.x` (the list as the instrument), or codes free to move with
`"schema"` (the number as the instrument).

## Also run, and holding

- `e1_assert_sides.hero`, `assert noisy("left") + noisy("mid") ==
  noisy("right")` failing: `left`, `mid`, `right` once each, then `assert
  failed: ...`, `left:  2`, `right: 3`, exit 134. The sides are evaluated
  once, left to right, and the report reuses them: E1 holds there.

## Ranked, most consequential first

1. **R2** the paragraph's *defect* is undefined, and the project files its new refusals as defects: 13 thesis codes in 52 days, six under `Defect NNN:`, `endless_recursion` with no sentence of the specification (run: refused, `--permissive` 0).
2. **R1** the spec-warden's E1 is false on `.default(v)`: `m["a"].default(xs[5])` exits 0 (run); design.md Part 5 makes it a branch, § 9's UFCS makes it a call.
3. **R3** every name a `1.x` adds refuses an older program (`builtin_name_taken`, run); four names joined the inventory and a fifth was reserved in 52 days; nineteen likely keywords are free names today.
4. **R10** on Linux this Mac's clang gives a 4 MiB frame no probe (one `sub`), so 2253 may be a silent write there, not a death; unrun on Linux.
5. **R6** the promise says *compiles*, its suite asks `check`, and `check` passes 371 of 371 `unsupported` cases; 21 of `v0.5.0`'s 69 programs bind C the suite never builds.
6. **R7** of the blind seat's 38 registered misses 25 are right lines plus prose; every W answer was right, so W's +3 (M10a's payment) is Sonnet's caveats; the order sentence moved one prediction in 40.
7. **R14** no exclusion for a repair robustness demands of a program the specification allows (Go's *Security*, Rust's soundness), with six blocking defects found in one sitting.
8. **R4** both item 3 wordings keep *aborts* where the compiler refuses (`handle_used_after_end`, run): item 5's shape inside item 3.
9. **R9** the author's rule: 2237 keeps place-less names beside their `_at` twins (29 declared, 27 absent from today's seed, the eight `hero_panic_*` written by nothing but the guarded-names list) for a `v0.5.0` seed that cannot compile against this runtime (run); `--permissive` named and refused as R2's would-be answer.
10. **R5** one source completes under `heroes run` (`-O2`) and dies under `heroes build` (`-O0`) (run); the paragraph is silent on stack and memory.
11. **R8** a stop inside a `hero_os.h` function names no place (`hero_thread_join` twice, run); 2258 is one of a class.
12. **R12** the command (verbs 11 to 13, defaults, exit codes, `--json`) is outside the promise without a word; Go excluded *Tools*.
13. **R11** `does_not_fit`, every narrowing conversion's code, is named nowhere though § 6 promises codes stable (run).
14. **R20** Go's random map order is refused by panel 006's fixed seed, and the compiler's own output depends on the order § 10 calls none.
15. **R21** the schema route for item 7 versions codes rather than promising them; Go and Swift promise the stronger thing.
16. **R15** M10a misreads a read inside a written place's index (run: aborts before the value); L1 lands inside the same-typed-pair bullet.
17. **R16, R17** *It runs one way* as rule or fact, unruled; K0 against K1, with Linux open for both.
18. **R19** no rule says what moves `1.Y` or `1.Y.Z`.
19. **R13, R18** this critic's F23 (66, not 67) and F10 (`thread_deep` is no thread) corrected under the first pass.

Beside: two unfiled shapes (the `exit` report naming a `.must()`; four diagnostics for one swapped pair of labels), five of the historian's quotations re-read raw and holding, and one slip of this critic's own (a temporary file in `/tmp`, removed).

Added to the method notes: R6's `build --emit-c` of the `char` case, run
from inside `tests/golden/unsupported/` of the critic's own worktree, left
that compiler's cache there (`build/`, 25 files, every one newer than this
pass's runner); removed, and `git status --short` of the worktree reads
`critic-scratch/` and `heroes-stage1` alone, as before the pass.

# Panel 230, the compiler-engineer's report

Seat: compiler-engineer (design.md §1.1, §1.7, Part 5; the veto on a core construct or the ceiling).
Worktree `.claude/worktrees/230-compiler-engineer`, detached at `595d92d88`; scratch
`.claude/worktrees/230-compiler-engineer-scratch/`. Written as it goes; the verdict table is at the
end once the numbers are in. **No clock was started by this seat**: every duration below is either
quoted from the shared brief or a command written for the coordinator.

## Log, in the order found

(appended as the work proceeds)

### 0. The compiler this seat judges with

`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` then `./heroes build selfhost/main.hero
-o heroes-next`, both exit 0 (no clock); `./heroes-next --version` prints `heroes 0.5.0` and `sources
413da33e7969760a`, the shared brief's key. Kept as the "before" compiler,
`230-compiler-engineer-scratch/heroes-before`. The "before" programs, copied (`rsync -a --exclude
build/ --exclude '*.bin'`, `key.txt` excluded and never read), rebuilt and run: the brief's eight rows
reproduced byte for byte (`assertint` 134 with `left:  3`/`right: 4`; `assertrecord` 134 with the
text alone; `deep` 134 `panic: stack exhausted in deep.down`; `handled` 0; `index`, `must`,
`overflow`, `slice` 134 with their one `panic:` line).

### 1. The seed's `#line` lines are 3.5% of it, so M-bounded-seed's 10 MB is not where L1's bytes are judged

`wc -c < seed/heroes.c` prints **27,000,911**; `grep '^#line' seed/heroes.c | wc -lc` prints **57,860
lines, 947,574 bytes**. A profile that drops every `#line` leaves a seed of about **26.05 MB**
(arithmetic on those two counts), so the 10 MB target of `docs/roadmap/milestones/M-bounded-seed.md:26-34`
is not reached by that profile: the page's premise, *its `#line` directives buy nothing and cost half
of it*, predates panel 219's line-join (52,044,067 to 25,897,631 bytes of the compiler's own C,
`selfhost/emit/writer.hero:28-33`). So the shared brief's frame (*L1 writes a place at each site,
against M-bounded-seed's 10 MB*) is not the right ruler: L1's bytes are judged as a share of the seed
as it is, below (§ 6). This is an inference about M-bounded-seed, not this sitting's to rule; it is
said because the brief makes the 10 MB the yardstick.

### 2. The census of the runtime's aborts, in kinds (item 1)

**Command**: `awk -v PAT='hero_panic[a-z_]*\\(|hero_abort\\(\\)' -f encl.awk runtime/parts/*.c runtime/*.c`
(`encl.awk`, in this seat's scratch `tools/`, prints each matching line with the C function that
encloses it), then each of the 145 lines tagged by hand in `tools/kinds.txt` after reading its
function, and counted by joining the two (every line tagged, none left over). **Printed**: 145 lines,
the brief's count.

| kind | lines | what they are |
|---|---|---|
| **1**, the check is in the program's C and the runtime only words the message | 2 | `array.c:176` (`hero_array_at`) and `str.c:301` (`hero_str_byte`), called by the emitted read on its failing branch alone |
| **2**, a program's own operation reaching a check the runtime makes on every call | 38 | below, by entry point |
| **3**, raised where no call of the program stands | 15 | `stack.c` POSIX handler 6 (`:439` is the Windows ASan arm, `:624-762`), Windows VEH 5 (`:885-964`), `hero_runtime_check_leaks` at exit 4 (`alloc.c:275-408`; the brief's *six* are its six `panic:` lines, four `hero_abort()`s) |
| resource limits (out of memory, too large, the OS refusing a thread or a locale) | 28 | `alloc.c:418`, `map.c:125`, `sort.c:186`, `array.c:69/289`, `spawn.c:265/311/386`, `codepage.c` 3, ... |
| invariants a correct compiler never reaches (*compiler bug* in their own words, or a descriptor or slot the emitter always fills) | 46 | `array.c:54-108`, `cow.c` 5, `map.c`/`map-write.c` 15, `lend.c:439-573` 4, `panic.c:94-104` (the test index), `os.c:535/926` (`args()` indexes in range by construction), `f64.c` render buffers 4, ... |
| entry points' own definitions | 14 | `panic.c` 8, `failure.c` 6 |
| comments | 2 | `os.c:157`, `panic.c:23` |

**Kind 2, by entry point, with its call sites in the seed** (`grep -oF '<name>(' seed/heroes.c | wc -l`):
`hero_array_set` (`xs[i] @ v`, emitted by `emit/container.hero:227`; *index out of range*, `cow.c:117`)
**254**; `hero_array_at_mut` in its impure form (`array_read.hero:62`, every access; `cow.c:63`) within
**170**; `hero_str_slice` (*out of range*, *splits a character*, `str.c:325/327`) **582**;
`hero_array_slice` (`array.c:346`) **112**; `hero_array_sort` (*nan*, `sort.c:61/67`) **71**; the map's
*key not equal to itself* (`map.c:97`, through `hero_map_find` 562 and `hero_map_set` 436);
`hero_str_repeat` (*string length overflow* on a large count, `str.c:256`) **25**; the C boundary:
`hero_cstr_nonnull` **51** (`str.c:550`), `hero_str_refuse_nul` (`str.c:589`), a `str` from C that is
null or not UTF-8 (`str.c:408-410/555`), the lend checks (`lend.c:236/254/323/415`), the eight
handle reports (`alloc.c:216-983`), a lost mark (`str.c:66/665`), `end_lease` twice (`str.c:659`),
`hero_dir_at` (`dir.c:174/189`, an `extern` of `hero_os.h`); threads (`spawn.c:403-419`); `run()`
(`run.c:92/96`); and the callback guard (`thread.c:98`), which already names the Heroes function C
called back. `hero_str_concat` (**10,336** sites), `hero_str_join` (424) and the pushes abort only on
a length past `INT64_MAX`, which memory refuses first (`alloc.c:418`), so they are counted as resource
limits, not as a place L1a must pay an argument for.

**The ten `panic:` lines outside the census** (`grep -nE '"panic: '` without a `hero_panic*` call, then
the first of `hero_abort()`, `abort()`, `raise(`, `return` within 30 lines): `lend.c:651-699`
(`hero_lend_tell`, 4, C past a buffer or a local, said from the fault handler) and `os.c:211-426`
(the lease handlers, 6). **All ten are kind 3.** And they already name what they can: `hero_lend_tell`
names the owner function, the callee and the parameter; the POSIX lease report says `under
<module>.<function>` through `hero_stack_blame` (`os.c:404-429`); the stack guard says `in deep.down`
and the recursion (`stack.c:617-623`); the null read and the dead handle say `called from
<module>.<function>` (`stack.c:744-762`). **So L4 for kind 3 is not new work: it is what those
handlers do today**, on POSIX, through `dladdr`; Windows has no walk (`stack.c:850-857`), and the exit
check (`alloc.c:272-404`) runs after `main` has returned, where there is no frame to name at all.

**The emitter's side, by `code_of`** (the brief's 19 lines in 7 modules confirmed), and **three index
sites the shared brief does not list**: `emit/str_read.hero:24`, the `str` byte read in place, whose
failing branch calls `hero_str_byte` (**1,293** seed sites, absent from finding 35's tally, so the
seed's abort sites needing a place are 17,297 + 1,293 = **18,590**); `emit/places.hero:158`, a fixed
array's index, inline; and the store `emit/container.hero:227`, `hero_array_set`, checked inside the
runtime on every store (kind 2 above). `hero_array_len(` (**4,771** seed sites) aborts only on a
NULL slot, a compiler bug, so it needs no place.

**The 59 `hero_unreachable` lines** (`code_of` over every `selfhost/` module naming it, 58 writing a
call and `emit/guarded_names.hero:75`, a word list): **none is reachable by a correct program through
a correct compiler.** Five follow a runtime call that has already aborted (`array_read.hero:37/42/65`,
`str_read.hero:24`), there so clang sees the branch end; two guard the element size against its C
type (`array_read.hero` `sized`, a type confusion); the rest are the type system's claims (`term.hero`
`default:` and `.unreachable`, the gate's refusals in `inst.hero:272`, `ops.hero:107`,
`counted.hero:96`, `release.hero:48`, the generated `eq`/`hash` of `structural.hero`). Reaching one
is a compiler bug, and `panic.c:46` says so. **This seat recommends no place on them**: 23,996 seed
sites would cost more bytes than every real abort together, for a message whose reader is the
compiler's maintainer, who has the program.

### 3. The place record: where the emitter has it, and the shape this seat built (items 2 and 3)

**Where the emitter has a site's place.** Every instruction reaches `selfhost/emit/inst.hero`'s `emit`
with `inst.span` and the `Source` (`inst.hero:52`), and the writer already turns `inst.span.start`
into the `#line` (`writer.at_span`). From that offset `source.locate` gives the file entry, the line
in that file and the column in characters with **two binary searches and no walk of the line**
(`selfhost/char_column.hero:8`, defect 372), so a column costs what a caret costs and defect 228's
quadratic walk does not come back. The function is `f.name` (`ir/containers.hero:103`): the
unmangled name, a generic instance's base name (its types are in `f.instance`, rendered for the
`/* name<T> */` comment by `signature.render_instance`), and **a `test` block's title with its
quotes** (`ir/lower.hero:144-146`), so a failing test's place can say `in test "title"`, which is
what ties the failure line to its test (question 12). A function value called through a parameter
aborts inside the callee's body, so the place is the callee's own, by construction.

**The file is the entry's name with the root's own directory taken off.** A `use`d module's file is
named `paths.directory_text(root) + module + ".hero"` (`selfhost/modules.hero:107-116`), so the root's
directory is a prefix of every user entry, and taking it off gives `check/state.hero` for a module and
the root's own file name for the root, whatever path the build was invoked with (finding 6's two
spellings give one text). Not `stem_of` plus `.hero`: `stem_of` cuts at the first dot
(`module/names.hero:33-52`), so `my.prog.hero` would print `my.hero`. The library is `<heroes library>`
(`source.LIBRARY_FILE`), the name its `#line`s already carry.

**The shape: four trailing literal arguments, not a static record.** Each failing branch calls a new
entry point, `hero_abort_overflow_at("report.hero", "report.windows", 13, 44)`. Why not the brief's
static record beside the function: (a) a site in expression context (the index reads, `a ? (abort) :
(void)0`) cannot declare one in C11, and a record declared beside the function must be declared before
the body is written, which the emitter learns only while writing it; (b) a record holding two pointers
is two load-time fixups per site in a position-independent binary (18,590 sites, about 37,000 fixups
the loader applies on every start of the compiler, an inference from the format, unrun), where an
argument is PC-relative code on the cold path and costs no fixup; (c) arguments go in registers, so
no frame grows at `-O0` (a compound literal per site would, against panel 076's frame cliff). The
numbers are plain decimals, not `INT64_C(n)`: they are arguments to an `int64_t` parameter, not
arithmetic, which is what `INT64_C` is for (`.claude/rules/generated-c.md` § Arithmetic).

**New names, never new parameters of an old name.** design.md §4.20 measured that *adding a function
to this ABI is self-guarding, an undefined symbol at link, while adding a struct field is not*; a C
function's parameter list is not part of its symbol either, so `hero_panic_overflow(void)` gaining a
parameter would let a stale object call it with nothing in the register the runtime then reads as a
place, on the one path where it prints. So the prototype adds `hero_abort_at`, `hero_abort_overflow_at`,
`hero_abort_must_at`, `hero_abort_assert_at`, `hero_abort_assert_sides_at`, `hero_array_at_abort`,
`hero_str_byte_abort` and `hero_str_slice_at` (`runtime/parts/place.c`, new), and keeps every old name,
because the seed that calls them builds the first stage (§§ 5 and 12).

**What the guard costs.** Every word the runtime header writes is owed to `emit/guarded_names.hero`'s
`NAMES` and to a `push_macro` and a `pop_macro` line of `runtime/heroes_guard_open.h` and
`heroes_guard_close.h` (546 each today, `grep -c`). The prototype leaves the four place parameters
**unnamed in the prototypes, a comment naming each**, so `file`, `function`, `line` and `column`, none
of them guarded today, never meet a program's macro, and the guard grows by the eight function names
alone: 24 lines. (Not yet written into the guard in this prototype; the compiler's own tests will say
whether `macro_guard`'s test notices, below.)

**What it cost, in the instrument's unit** (`tools/code_lines.sh`, the awk twin of
`suite_layout.hero:1005`'s `code_lines`; before → after): `emit/inst.hero` 329 → 335 (decided 350);
`emit/operator.hero` 276 → 280; `emit/abort.hero` 133 → 138; `emit/access.hero` 145 → 146;
`emit/ops.hero` 243 → 253 (the slice); `emit/container.hero` 299 → **300, its ceiling exactly**, which
took one binding to keep a wrapped line from costing two; `emit/division.hero`, `array_read.hero`,
`str_read.hero` unchanged; `emit/place.hero` new, 51. **+78 code lines in `selfhost/`**, comments
included (first written *+75*, a slip of addition; § 14 has the final count, 83 with the assert's span). `runtime/parts/place.c` new, 127 lines (`wc -l`), and `heroes_runtime.h` +27. A first
version that computed the place in `inst.emit` and passed it down wrapped six calls and took
`inst.hero` to **368 against its decided 350**: the place is therefore computed by each writer only
where it writes an abort, from `s` and the offset, which is also what keeps the compiler from building
a place string for every instruction it emits.

### 4. M7, the obligation: it refuses a correct program of the tracked examples (item 6, part)

**Command**: an awk over every `function main()` of `examples/` (55 directories), each `.err <name> =>`
arm inside it, read as *ending* where it or the eight lines below it at a deeper indent hold `exit(`,
`return` or an abort, then the three open arms read by hand. **Printed**: 47 `.err` arms in
`examples/`, 3 of them in a `main` and not ending the program: `examples/calculator/main.hero:82` and
its twin `whole.hero` (`.err e => print(c, " -> ", e.code, ": ", e.msg)`, inside a loop that shows
each expression's error **on purpose**, a program that is right to exit 0), and
`examples/query/main.hero:58` (*cannot write*, followed four lines later by `exit(1)` on the read that
then fails). So a rule *a `main` whose `.err` arm leaves normally is told so* refuses the calculator,
a correct program, and would need an escape the language does not have (a new form, or a pragma, both
design.md Part 6 territory). And it cannot see the other ways a failure is swallowed: `.default(v)`,
`.is_err()` and a print, a `match` in a function `main` calls. A crude reading, an awk and not the
checker, so the number is a floor on the false alarms, not a census.

### 5. The prototype runs (item 3): what each abort prints under L1

**Built** by the seed's route for an ABI step: `clang` on the seed gives stage 1 (it writes
`_Static_assert(HERO_RUNTIME_ABI == 30)`), which builds the modified `selfhost/` against **a copy of
the new runtime with its number put back to 30** (`HEROES_RUNTIME=<copy>`, `abi-put-back`); that stage 2
links because the new runtime keeps every old entry point; stage 2 (the new emitter, writing 31)
builds stage 3 against the real runtime. Both exit 0. `heroes-l1` is stage 3.

**The "before" programs** (`heroes-l1 build <f>.hero`, each binary run, stderr):

| program | stderr under L1 | exit |
|---|---|---|
| `index` (`return xs[i]`) | `panic: array index out of range` / `  at index.hero:2:12, in index.pick` | 134 |
| `overflow` (`return n + 1`) | `panic: integer overflow` / `  at overflow.hero:2:12, in overflow.bump` | 134 |
| `must` (`print(half(3).must())`) | `panic: .must() on an error: odd: not divisible by two` / `  at must.hero:7:11, in must.main` | 134 |
| `slice` (`s.slice(from: 0, to: 5)`, kind 2 by L1a) | `panic: string slice splits a character` / `  at slice.hero:3:11, in slice.main` | 134 |
| `assertint` | `assert failed: a == b` / `  at assertint.hero:4:5, in assertint.main` / `  left:  3` / `  right: 4` | 134 |
| `assertrecord` | `assert failed: p == q` / `  at assertrecord.hero:8:5, in assertrecord.main` | 134 |
| `deep` (kind 3, unchanged) | `panic: stack exhausted in deep.down` | 134 |

**The columns**: four of six follow the shared brief's convention as the IR already holds it (the
start of `xs[i]`, `n + 1`, `half(3).must()`, `s.slice(...)`). **The `assert` gave its keyword's column
(5)**, because the lowering gives the abort the statement's span (`ir/flatten.hero:933`); the asserted
expression's id is in hand there (`asserted`), so the abort now takes `a.exprs[asserted].span`. One
token, but `heroes fmt` wraps the longer call one argument a line: **`ir/flatten.hero` 1245 → 1250
code lines, exactly its decided 1250** (this sentence first said *no line added*, before the format was
measured; binding the span on a line of its own would cost one line, 1246). No golden moves by it:
the IR dump prints no instruction's span (`grep -n span selfhost/ir/print.hero`), and `assert_stop`
writes under the generated file's `#line` (`emit/abort.hero:8-10`), so only the place moves. Rebuilt
below (§ 7).

**The probes** (`230-compiler-engineer-scratch/cases/`, each checked by `heroes-before` first, exit 0):

| probe | before | under L1 |
|---|---|---|
| a started thread reads `xs[7]` (`hero_thread_spawn`, `runtime/parts/spawn.c`) | `panic: array index out of range`, 134 | `+ at thread_index.hero:9:12, in thread_index.worker`, 134 |
| a generic instance, `second<T>` over `[str]` | same | `+ at generic.hero:4:12, in generic.second` (the base name; the instance's types are not printed) |
| a function value through a parameter, `apply(grow, ...)` | `panic: integer overflow` | `+ at fnvalue.hero:4:12, in fnvalue.grow`, the callee |
| `map(xs, grow)`, the library calling the program's function | same | `+ at mapped.hero:4:12, in mapped.grow` |
| `use sub/tools`, `tools.pick(5)` | index | `+ at sub/tools.hero:3:12, in tools.pick` |
| `xs[i] @ 9`, an indexed **store** past the end | index | **no place**: `hero_array_set` checks inside the runtime (kind 2) and this prototype did not locate it |

**A thread costs L1 nothing**: the place is a literal at the site, so a started thread prints it as
`main` does, where L4's walk depends on the thread's own stack bounds (`stack.c:276-277`).

**Finding 6 is answered for the abort's text**: `heroes-l1 build cases/nested.hero` and the same by its
absolute path give **identical stderr** (`cmp`), and the place arguments of the two emissions are
identical (`diff` of every `hero_array_at_abort(...)`); the `#line`s still differ between the two, as
the critic measured, which L1 neither causes nor repairs.

**What the store costs to locate**: the place walk is `emit/container.hero` (now **300 of 300**) and
`emit/places.hero`; locating `hero_array_set` and `array_read.hero:62/:65` needs a `where` threaded
through the walk and either `hero_array_set_at`/`hero_array_at_mut_at` (L1a, one argument on every
store and every impure step) or an inline check before the call (L1b). Either way it needs **a seam
cut in `container.hero` first**, since it has no line left. This is the one class of the "before"
table's kind that the prototype leaves as today, and the § 6 sentence would be false for it until it
is done.

### 6. The cost in bytes (item 4): the seed and the compiler's own binary

**The seed.** `heroes-before build selfhost/main.hero --emit-c` over a clean copy of `selfhost/` and
`runtime/` at `595d92d88` (`git archive`) writes a file **byte-identical to `seed/heroes.c`** (`cmp`
silent), so "before" is the seed itself: **27,000,911 bytes, 271,387 lines**. The L1 (and M1) compiler
over the modified tree writes **27,889,403 bytes, 271,588 lines: +888,492 bytes, +3.29%** (`wc -c`).
In it, **18,988 places** (`grep -oE` of the four-argument run, every entry point counted:
`hero_abort_overflow_at` 7,570, `hero_array_at_abort` 6,830, `hero_abort_must_at` 2,383,
`hero_str_byte_abort` 1,293, `hero_str_slice_at` 584, `hero_abort_at` 273, the two asserts 67), in
**3,648 distinct functions of 650 files**, two of them in the library; their text with its separators
is 1,044,101 bytes, which replaced shorter calls (`, hero_unreachable()` left every index read), for
the net 888,492. **No old entry point is left in the after seed** (`grep -oE` of the seven old names
prints none), so the refresh after it can drop them from the runtime (§ 12).

**What a smaller seed would cost instead** (arithmetic on those counts, not built): a record per
function, `{file, function}`, written before the function, and `&record, line, column` at each site,
is about 3,648 records of some 70 bytes and 18,988 sites of some 15, **about 0.54 MB against 1.04**;
it needs the emitter to know before writing a body whether the body holds a site (a pre-scan of the
IR, the two lists that drift CLAUDE.md § 11 warns of) or `__attribute__((unused))` on a record per C
function (about 9,000), and it costs two load-time fixups per function where the literals cost none.
**M-bounded-seed's profile** may leave the places out of the seed, since stage 1 is used only to build
stage 2 and every compiler a person runs is built by an emitter that writes them; the runtime must then
accept a NULL file and function and print no `at` line (one `if`), never a second set of entry points.

**The compiler's own binary** (`size -m`, Darwin arm64, both built at `-O0` by `heroes build`,
`heroes-before` from the clean tree by the seed's stage 1, `heroes-after` from the modified tree by its
own stage 2, so each is its tree's fixpoint): `__text` **13,967,772 → 14,407,772 (+440,000, +3.15%)**,
about 23 bytes, six instructions, per site, all on failing branches; `__cstring` 14,914 → 101,665
(the 650 file names and 3,648 function names, once each); `__DATA_CONST,__const` **42,696 → 42,696**,
no relocated record, as designed; the file 18,736,664 → 19,268,392 (+2.84%).

### 7. The rebuilt prototype (L1 with the assert's span, and M1): columns, `-O2`, `heroes test`, threads

`heroes-l1m1`, built as above. **The asserts now give the asserted expression's column**:
`assertint.hero:4:12`, `assertrecord.hero:8:12`. **The four blind items print exactly the P variant's
place lines** for `report` (`at report.hero:13:44, in report.windows`), `basket` (`18:12, in
basket.line_total`) and `checksum` (`11:13, in checksum.checksum`), byte for byte (`diff`); for `rects`
the compiler prints `at rects.hero:35:16`, the expression, where **the copy of
`blind230/items/rects.err.P` this seat took still reads `35:9`**, the keyword: the shared brief's
convention was repaired after finding 20, the blind file perhaps not. *A question for the
coordinator*: if the sessions read `35:9`, family P reads a column no compiler of this route prints.

**`-O2`** (`heroes build -O2`, `heroes run`'s level): the four items' stderr is **identical to `-O0`**
(`diff`), and so are `fnvalue` and `generic`; `deep` still says `stack exhausted in deep.down`. But
`otool -tV` of `rects` shows **`main` calling neither `normalized` nor `Rect_eq` at `-O2`** (both called
at `-O0`): they were inlined, so a name read from the program counter at an abort inside `normalized`
(L4's `dladdr`) would say `rects.main`, where L1's literal still says `rects.normalized`. L2 reads
inline frames only if it reads DWARF's inline information, which `-gline-tables-only` keeps and a
reader in the runtime would have to parse. **Inlining moves L4's and L2's names, never L1's.**

**`heroes test`** (`before/tested.hero`, two failing tests): before, `assert failed: ...` on stderr and
`FAIL "<title>"` on stdout, tied by their order alone; under L1 each failure carries `at
tested.hero:12:12, in test "shifting moves x"` and `at tested.hero:15:12, in test "shifting keeps y"`.
**The place ties a failure to its test by name** (question 12), because a `test` block's IR name is its
title.

**A thread** (`cases/thread_deep.hero`, a recursion with no base case inside `hero_thread_spawn`):
`panic: stack exhausted in threaddeep.down`, 134. The walk works on a started thread, and it **spells
the module by its C component**, `threaddeep`, since it reads the mangled symbol (`stack.c`'s
`hero_stack_say_heroes_name`, the component sanitised to `[A-Za-z0-9]`), where L1 spells it
`thread_deep`, as the program does; `use check/state` would be `checkstate` there and `state` here.
**One program, two spellings of one function, chosen by the abort's kind.** If the sitting keeps the
walk for kind 3 (it is today's mechanism), the walk should print the reader's name, which needs a table
from the C symbol to it, the one thing the place literals already carry.

### 8. The clock, prepared for the coordinator (item 4); this seat started none

**The two compilers**, frozen in `230-compiler-engineer-scratch/timing/` (`shasum -a 256`):
`heroes-before` (`b249206229422ec3451eeebf406eb894775a4a5701668be1a119332c1e19b0fd`, `sources
413da33e7969760a`, the lane's `heroes-next`) and `heroes-after` (`53cbdd142175dd9c0a76d729dcb8fa2fae3c43536680464f4200b229e9e09fd9`,
`sources 28b2face6fc3ee0f`, L1 for kind 1 and `str` slices, and M1). **One input for both**: the clean
copy of `selfhost/` at `595d92d88`, `230-compiler-engineer-scratch/clean/`, each compiler with its own
runtime (`clean/runtime`, ABI 30; `timing/runtime-after`, ABI 31). Every command below was run once by
this seat **without a clock**, exit 0, so the coordinator times commands known to work.

```sh
S=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/230-compiler-engineer-scratch
cd $S/clean
B="env HEROES_RUNTIME=$S/clean/runtime $S/timing/heroes-before"
A="env HEROES_RUNTIME=$S/timing/runtime-after $S/timing/heroes-after"
# 0. the noise floor: the same compiler, the same command, six times (first discarded)
for i in 1 2 3 4 5 6; do /usr/bin/time -lp $B build selfhost/main.hero --emit-c -o /dev/null; done
# 1. the compiler's path that does not abort: emission, warm cache, alternated, first of each discarded
for i in 1 2 3 4 5 6; do /usr/bin/time -lp $B build selfhost/main.hero --emit-c -o /dev/null
                         /usr/bin/time -lp $A build selfhost/main.hero --emit-c -o /dev/null; done
# 2. the whole build, cold: the cache emptied before each run (it is this seat's scratch)
for i in 1 2 3; do rm -rf $S/clean/build; /usr/bin/time -lp $B build selfhost/main.hero -o $S/timing/out-before
                   rm -rf $S/clean/build; /usr/bin/time -lp $A build selfhost/main.hero -o $S/timing/out-after; done
```

`-l` adds **instructions retired**, steadier than `real` on a shared machine; read `real` against
`user + sys` as `CLAUDE.md` § Verification says. What the comparison holds: the after compiler runs its
own code with the places on cold branches only, pays the four literal arguments of L1a as built on
each of its own `hero_str_slice_at` calls (§ 9), and does the emitter's extra work, a place written at 18,988 sites when it
emits itself (each a locate, two binary searches, and five concatenations). The whole build adds clang
over C that is 3.3% longer.

### 9. What L1a's argument costs on the path that does not abort: counted, not timed (item 4)

**Command**: a copy of the after runtime (`230-compiler-engineer-scratch/runtime-count/`, never the
tree's) whose `hero_array_at_mut`, `hero_array_set`, `hero_str_slice` and `hero_array_slice` each bump a
counter, printed at exit; a compiler of the **clean** tree linked with it (`heroes-counting`); then its
`build selfhost/main.hero --emit-c -o /dev/null` and its `check selfhost/main.hero`. **Printed**:

| entry point | calls in one self-emission | calls in one `check` |
|---|---|---|
| `hero_array_at_mut`, the impure place (`array_read.hero:62`) | **0** | 0 |
| `hero_array_set`, `xs[i] @ v` (`container.hero:227`) | **17,163,158** | 1,341,237 |
| `hero_str_slice` | **18,861,430** | 8,510,902 |
| `hero_array_slice` | 378,591 | 51,767 |

So **L1a's argument on `:62` costs the compiler nothing**: the form is never executed when the compiler
compiles itself (its 170 seed sites are failing branches of `:65` and impure places that do not run).
The cost of L1a lands on the store and the slice. As built, the slice carries the place as four
literal arguments, six instructions per call on arm64 (two `adrp`+`add` pairs and two `mov`s, read
from the after binary's `otool -tV` at a call of `hero_str_slice_at`), so about **113 million
instructions per self-emission**, and a store located the same way would add about 103 million more;
**one pointer to a per-site static record would be two instructions per call** (about 38 and 34
million). Those are counts multiplied by an instruction count, an inference; the coordinator's `-l`
runs give the real share (§ 8). For a sense of scale only: each such call allocates or unshares, so
its own work is hundreds of instructions. **Where the place rides on every call, a record pointer
beats literals; where it rides on a failing branch only, literals cost nothing and need no record.**
This seat's recommendation therefore splits L1 by kind (the verdicts, at the end).

### 10. M1 built and run, and its neighbours (item 6)

**What it took** (code lines, `tools/code_lines.sh`): `check/decls.hero` 294 → **299 of 300**, the
`main_returns` test widened by one call and a four-line `main_fails` (is the result the interned
`()?`); `value_errors.hero` unchanged in lines, its message narrowed to *`main` produces nothing or
`()?`*; `emit/c_main.hero` 87 → 100, the shim testing the returned tag (`__typeof__` of the call, so
the emitter names no `T?` struct) and a `fails` that asks the type, never the type id; one argument
more at its three callers (`emit.hero`, `emit/unit.hero`, `emit/value_probe.hero`), no line; the
runtime's `hero_main_failed`, 20 lines. **No construct is added**: `main`'s result is a type rule of
the checker and a shape of the emitted `main`, `?` already lowers to an early return (design.md Part 5's
sugar table), so it is erased before the IR's verifier sees anything new. **Not core.**

**What it prints** (`230-compiler-engineer-scratch/m1/`, `heroes-l1m1`, each binary run):

| program | stdout | stderr | exit |
|---|---|---|---|
| `main -> ()?` returning `fail(code: "no_key", msg: "port")` | `looking for the port` | `error: no_key: port` | **1** |
| the same returning `ok()` | `fine` | | 0 |
| `p = port("")?` in `main` | | `error: empty: no port given` | **1** |
| a `fail` returned with a lease live (`x = s.lease()`, never ended) | | `error: late: a lease is still live` / `panic: 1 lease(s) never ended — ...` | **134** |
| a unit `main` with the same lease (today's § 13) | | `panic: 1 lease(s) never ended — ...` | 134 |
| `main -> i64` | refused, `error[main_returns]`, the narrowed message and note | | 1 |

**§ 13 and § 1, both orders measured.** The prototype writes the failure's line, gives the failure
back, then runs the exit check as after any return of `main`: a live lease aborts there, **its line
under the failure's, at 134**. The other order, a copy of the runtime with the check first
(`runtime-checkfirst/`), printed **only** `panic: 1 lease(s) never ended`, 134: the failure's own
reason lost. So: *the exit check runs on every return of `main`, the failure's line first, and its
abort wins over the failure's status*; a leak is never hidden behind an expected failure, and the
reason is never lost behind the leak. `exit(code:)` keeps bypassing the check (`runtime/hero_os.h:138-150`).

**M5** (a built-in writing a line on stderr): `hero_write_err` exists (`runtime/hero_os.h:108`) and the
compiler writes its own errors through a binding of it (88 code lines in 21 modules, the critic's count).
As a built-in it is an inventory row and a printer like `print`'s (`emit/printing.hero`), tens of lines,
no construct; it would let the compiler drop its binding of the runtime's own header, which is the one
self-hosting argument any M route has (Principle 0). **M1 has none**: the compiler's `main` ends by
`exit(code:)` with 0, 2 and the CLI table's codes (`selfhost/main.hero:99-225`), statuses M1 cannot
give, so the closure-list program would not use it; M1's ticket is design.md Part 11 alone.

**M7** refuses a correct program (§ 4). **M6** (`main -> i64?`, the status carried) is M1 plus the
range below on the status, the same shim; it serves the 13 of 40 visible failing cases that ask 2 or 3
(the critic's count), which M1 cannot.

### 11. S1 built far enough to run (item 5)

**Where it lives**: a new module, `selfhost/emit/sides.hero` (**123 code lines**; its first build warned
`-Wunused-variable` 96 times, a record whose every field writes `...` never reading `v`, and one line,
`(void)v;`, took the compiler's own build to 0 warnings), not
`emit/structural.hero`, which stands at 324 against its decided 335 (`suite_layout.hero`'s `DECIDED`);
`emit/perfn.hero` 223 → 226 (two calls, beside `_eq`'s prototypes and bodies); `emit/abort.hero`
→ 147 (`assert_stop` asks `sides.side` where it asked `builtins.to_str_entry`, and takes `names` and
`decls`); the runtime's `parts/show.c`, **189 lines**, and 20 lines of declarations. A renderer per
record, `<prefix>_show(HeroShow *o, const void *p)`, never a `HeroDesc` member (design.md §4.20, panel
027), declared beside the other per-type prototypes so every body may call it; numbers through the
entry points `print` uses; a `str` quoted with the spec's escapes and `\u{..}` for what a line cannot
show, the set asked of `failure.c`'s `hero_failure_unshowable`, once; an array through one bounded
runtime walk; `ptr`, `cstr`, function values as an address, never read through; the formatter's four
bounds (`SHOWN` 16, `TEXT_BYTES` 1024, `BUDGET` 256, `DEPTH` 6). **Not rendered by this prototype**,
written `...` where they stand: variants, maps, `T?`, fixed arrays, arrays of arrays, integers narrower
than `i64` inside an array; partial, foreign and handle records write `Name(...)`.

**What it prints** (`230-compiler-engineer-scratch/s1/`, built by the S1 emitter, each binary run):

| program | the sides, before | the sides under S1 |
|---|---|---|
| `rects` (the blind item) | none | `left:  Rect(x0: 2, y0: 1, x1: 5, y1: 1)` / `right: Rect(x0: 2, y0: 1, x1: 5, y1: 4)`, **the shared brief's text exactly** |
| `drawn == expected`, two `[Rect]` | none | `left:  [Rect(x0: 5, y0: 4, x1: 2, y1: 1), Rect(x0: 0, y0: 0, x1: 3, y1: 3)]` / `right: [...]` |
| `s == ""` with `s = "a\"b"` | `left:  a"b` / `right: ` (empty) | `left:  "a\"b"` / `right: ""` |
| defect 2225, `"a\n  right: a"` | three lines, two of them `right:` | `right: "a\n  right: a"`, one line |
| `["ada", "grace"]` against an ESC `[2J` | none | `right: ["ada", "grace\u{1b}[2J"]` |
| a `Node` of `str`, nested records, `[i64]`, `f64`, `bool` | none | `Node(label: "root", left: Leaf(name: "l", at: [1, 2]), right: Leaf(name: "r\n", at: [3]), weight: 1.5, open: true)` |
| 20 elements against 21 | none | `[0, 1, ..., 15, ... 4 more]` / `[..., ... 5 more]` |
| `x == x`, `x` a `nan` | `nan` / `nan` | **`nan` / `nan`, unchanged** |
| the tripwire's own `[1, 2] == [1, 3]` | text alone | `left:  [1, 2]` / `right: [1, 3]` |

**What S1 does not answer.** Two sides rendering alike stay alike: `nan`, and two arrays that differ
only past element 16 at one length, two texts past byte 1,024 (finding 10). What answers them is
cheap and the prototype does not have it: compare the two rendered texts on the failing path and, where
they are equal, write one line more (`the two sides render alike: a nan is equal to nothing, or they
differ past what is shown`), about 10 runtime lines. **The tripwire** (`suite_special.hero:308-317`)
**goes red under S1**, as panel 117 meant: its needle `left:  [1, 2]` is printed; it is unpinned and
replaced by `fixedbugs` cases per shape. Under S2 (an array by its length) and S3 (the first differing
element) the needle is absent, so the row **stays green while a side is rendered**, and its own message
(*an aggregate side is rendered now*) would be false if it fired: under either it is rewritten, never
left.

**Defect 2225 is two shapes, and S1 repairs one.** The `.must()` line is the failure printer's
(`failure.c:186-198`), not a side: `forge_must` still prints `panic: .must() on an error: bad: x` and a
forged `panic: integer overflow` beneath it, and **under L1 the place line now follows the forged line**,
`at forge_must.hero:7:11, in forge_must.main`, so the forgery reads as a located overflow. **L1 must not
land before the `.must()` line's repair**: that order would make a false message more convincing
(design.md §4.17; §1.12's *what would make this wrong*). The repair is the failure printer's own: a line
feed in a program's text written as the printer writes every other control character, or the message
quoted as S1 quotes a side.

**One notation with the formatter, except a map.** The `str` escapes, `\u{1b}`, a record's construction
and `[e0, e1, ...]` with `... N more` are `heroes_lldb.py`'s (its docstring, `:20-40`). **A map is
not**: the formatter writes entries *in bucket order, the order `keys(m)` gives*, and S1 must order
them by their rendered text to answer panel 117's map clause. So *one notation* holds only if the
formatter sorts too (it reads at most `SCAN` buckets today), or the sitting records the one difference.

**Panel 117's map clause.** Ordering entries by their rendered text makes the text a function of the
value, with no `cmp` on records and no sixth descriptor member. Keys cannot hold a float
(`error[float_map_key]`, measured on `{f64: i64}` and on a record key holding an `f64`; through a type
parameter the runtime aborts on `nan`, `map.c:97`), so a key's rendering is injective on every key type
**except where a bound cuts it** (`TEXT_BYTES`, `BUDGET`, `DEPTH`); there, two distinct keys can tie. The
tie-break that keeps the text a function of the value is **the keys' full renderings, compared only
when the bounded ones tie**; a hash tie-break would leave a residue to bucket order. The cost is every
key rendered once on the failing path (a 16-entry selection, memory bounded by 16 × 1,024), estimated at
80 runtime lines and 40 emitter lines with the per-key and per-value renderers; unbuilt.

**Panel 076's frame cliff** (about 52,200 frames at `-O0`) is closed for rendering by construction: the
renderer recurses at most `DEPTH` 6 levels and writes at most `BUDGET` 256 values, whatever the value's
depth, where the generated `eq` and `hash` recurse as deep as the value.

### 12. The landing (item 7), and the questions of the shared brief this seat can answer

**The seed's route is `abi-put-back`, measured, not inferred** (§ 5): the runtime gains the new entry
points and keeps every old one, `HERO_RUNTIME_ABI` 30 → 31, the emitter writes the new ones, and the
refresh is emitted by the stage that the old seed's stage 1 built against a copy of the new runtime with
the number put back. **The old names must stay in the runtime until that refresh**: stage 1 is the old
seed, which calls them in the C it writes for stage 2. The refreshed seed calls none of them (§ 6), so
**a second step removes them** (a change of the header's shape, so another number and another
`abi-put-back` refresh) or they stay as one-line wrappers. Under the rule of 2026-10-18 (*anything the
compiler's own code uses enters `selfhost/` only after the refresh whose seed knows it*) nothing here
waits a refresh: the compiler's own Heroes calls no new library function and declares no `main -> ()?`;
the new symbols are written by the emitter, which stage 1 does not run.

**The order of the commits**, each gated by its own cases:
1. **defect 2225's `.must()` line** in the failure printer, *before* L1 (§ 11: L1 would put a real
   place under a forged line);
2. **one ABI step** carrying L1's runtime half, M1's `hero_main_failed` and S1's `show.c` (three
   features, one number, one `abi-put-back` refresh: each new declaration moves the number, and a
   refresh is a release-sized act since 2026-10-17); the emitter's L1 for kinds 1 and 2, the assert's
   span (`ir/flatten.hero:933`); `Seed-link: abi-put-back`;
3. **a seam cut in `emit/container.hero`** (300 of 300 after the read's place), then the store and the
   place walks located (§ 5);
4. M1's checker and shim; S1's emitter; the control arm's two `unmatched` entry points with a place
   (question 13), so the arms are compared on one form of feedback;
5. `exit(code:)`'s range (§ 13);
6. **after the refresh**, the old names dropped (another ABI step) or kept as wrappers.

**The goldens owed.** **Blessed again**: 394 of `tests/emission/`'s 639 files and 8 of
`tests/golden/emit/`'s 24 hold an abort call L1 rewrites (`grep -rlE` of the seven old names); none of
`tests/golden/ir/` (58) or `tests/golden/lldb/` (26). **Not moved**: the 132 `!panic:` goldens match
their first line by `strings.contains` (`tests/harness/expectation.hero:159-166`), and L1 keeps every
first line. **New**: a `check` case annotated `#~ main_returns` for `-> i64` (none exists today,
finding 40); `run` cases for `main -> ()?` failing (`!exit: 1`), `ok()`, `?` leaving `main`, and the
lease under a failure (`!exit: 134`, both lines); a `fixedbugs` `run` case per abort class pinning its
`at` line (index read, `str` byte, each overflowing operator, `/` and `%` by zero and by -1, a shift,
an ordering of a `nan`, `.must()`, both `assert` forms, a slice, the store once located, a `test`, a
generic instance, a used module, a thread); defect 2224's case; defect 2225's two shapes; and the
`special` tripwire unpinned under S1.

**The questions of the shared brief, as far as this seat measured them**:
- *2, the function's name*: a generic instance prints its base name (`generic.second`); its types are
  in `f.instance` and would cost `signature.render_instance` at each site; a `test` block prints
  `test "<title>"`; a function value through a parameter prints the callee (`fnvalue.grow`, also
  under `map`); the library prints `<heroes library>` with **the library's own line**
  (`library.range` at 31:13, `library.args_checked` at 221:13 in the after seed), a line its reader
  cannot open, and not the program's call that reached it, which only a walk could add; `main` prints
  `<root>.main`.
- *4, Windows*: unrun. L1's text is the same literals and one `fprintf`; kind 3 there has no walk
  (`stack.c:850-857`), so it says `stack exhausted` and no name.
- *5, the signal path*: L1 prints from the program's failing branch, never from a handler; kind 3 keeps
  `write(2)` alone. **But the place line goes through `hero_failure_write`, which allocates**
  (`failure.c:177-183`), and so do S1's renderers: on a heap that is gone, `hero_malloc_raw` aborts with
  `panic: out of memory` (`alloc.c:418`) and the place or the side is lost. The robust shape writes the
  place from a fixed buffer, and **lets the entry point print the `assert`'s first line and its place
  before calling the renderers** (the renderers passed in, not their results), so a renderer's failure
  loses the sides and never the head line; the prototype renders first, as the call's arguments.
- *7, determinism*: the place is literals, identical across invocation paths (§ 5) and runs.
- *8, the statuses*: an abort 134 on Darwin (panel 174 measured `0xC0000409` on Windows, unrun since);
  `main`'s `fail` 1; a lease under it 134; `exit(code: 256)` 0 today (§ 13).
- *10, a thread*: L1 is the same there (§ 5); the walk works on a started thread (§ 7).
- *13, the control arm*: `hero_panic_unmatched_case` and `_value` carry no place in this prototype; two
  entry points and two lines of `abort.hero` give them one.

### 13. S1's bytes, and `exit(code:)`'s range (items 5 and 6)

**S1's bytes, as built** (a renderer for every record, `HERO_TU_LOCAL`, whose `used` attribute keeps
each in every binary whether called or not, `emit/tu_local.hero:58-60`): **449 renderers** in the
compiler's own C (`grep -c '_show(HeroShow \*o, const void \*p) {'`, beside 1,646 generated `_eq`s); the
seed **28,212,560 bytes, +323,157 over L1's 27,889,403 (+1.16%)**; the compiler's `__text`
14,407,772 → 14,429,244 (+21,472). **What a reachable-only S1 costs instead**: a worklist from the
types that stand as an `assert`'s sides (`abort_op` of `.assert_abort`, their operands' types, closed
over fields and elements, as `synth`'s worklist already closes `eq`'s), so a program asserting no
aggregate carries no renderer: the compiler's own C would carry about none, since its `assert`s outside
`test` blocks compare scalars (its 37 `hero_panic_assert_sides(` and 30 `hero_panic_assert(` seed sites
are what `to_str_entry` already renders or the text alone). About 40 more emitter lines; unbuilt.

**`exit(code:)`'s range.** A copy of the runtime (`runtime-exit/`) whose `hero_exit` aborts outside
0..255: `exit(code: 256)` → **134**, `panic: exit(code: 256) is outside 0..255, and a shell would read it
as 0`; `512` the same; `-1` → 134, *read as 255*; `255` → 255; `1` → 1. No tracked program calls `exit`
outside 0..5 (`grep -rnoE` over every `.hero`, worktrees excluded: `1` 519 times, `2` 154, `3` 25,
`0` 18, `4` 3, `5` 1). **Two costs the ruling must carry**:
- **The compiler forwards a child's status through `exit(code:)`**: `heroes run`'s `.program_exit(status:
  ran.code)` (`selfhost/cli/verbs.hero:112`), and on Windows that status is the child's 32-bit `DWORD`
  (`runtime/parts/run.c:1404-1421`), `0xC0000409` for an abort (panel 174's reading). **A run-time abort
  outside 0..255 would make `heroes run` abort when its child aborts, on Windows** (read, unrun: no
  box). So the rule needs the compiler's forwarding to leave by an entry point of its own (it already
  binds `hero_os.h`), or a mapping it states; the language rule itself can stay 0..255 everywhere.
- **The abort names no place a reader can use**: `exit` is a library function written in Heroes
  (`selfhost/library_source.hero:268`, `hero_exit(code)`), so its call into the runtime stands in the
  library; a place at the program's call needs `exit` emitted at the call site (it is one line of the
  library today). **The literal case is the checker's**, `exit(code: 256)` refused at compile time: a
  new diagnostic class and a sitting's matter (CLAUDE.md § 4), about 20 checker lines, a builder in
  `value_errors.hero` and an annotated `check` case.
- **Clamping** (256 to a failure) writes a status the program did not ask, silently; **the low eight
  bits stated** documents the silent success instead of ending it. Both fail design.md §1.12's test.

### 14. The compiler's own tests on the whole prototype, and the totals

`heroes-s1 test selfhost/main.hero` (the compiler built with L1, M1 and S1): **1,752 tests, 2 failed**.
Both are edits the landing owes, and the test runner shows the feature on itself:

```
assert failed: missing.join(" ") == ""
  at emit/macro_guard.hero:267:12, in test "the list holds every word the runtime's headers write"
  left:  "HeroShow hero_abort_assert_at hero_abort_assert_sides_at hero_abort_at ... o show"
  right: ""
```

**27 words** the header gained and the guard does not keep: the 8 new entry points, `hero_main_failed`,
`HeroShow`, 13 `hero_show_*`, `hero_str_quoted`, and two parameter names (`o`, `show`) that unnamed
parameters would spare; each is a `NAMES` word and a `push_macro` and a `pop_macro` line. The other,
`emit/body.hero`'s *a whole function prints*, pins `hero_panic_overflow()` inside a whole emitted
function. **No other test of the 1,752 moved.**

**The totals** (`230-compiler-engineer-scratch/code-lines-final.txt`, every touched module before and
after in the instrument's unit): **+238 code lines in `selfhost/`, 0.19% of the clean compiler's
122,243** (684 modules): L1 83 (`emit/place.hero` 51, `ops.hero` 10, `inst.hero` 6, `ir/flatten.hero`
5, `abort.hero` 5, `operator.hero` 4, `access.hero` 1, `container.hero` 1), M1 20 (`emit/c_main.hero`
13, `check/decls.hero` 5, `value_errors.hero` 2), S1 135 (`emit/sides.hero` 123, `abort.hero` 9,
`perfn.hero` 3). **Runtime**: `parts/place.c` 141 lines (L1's entry points and M1's
`hero_main_failed`), `parts/show.c` 189 (S1), 51 header lines, 2 includes: about 380 lines, 2.7% of
its 14,436. Owed and unbuilt: the store and the remaining kind-2 entry points with a record pointer
(about 25 emitter and 40 runtime lines, after a seam in `container.hero`), S1's variants, maps, `T?`
and the reachable-only worklist (about 150 emitter and 150 runtime lines), the `exit` range (5 runtime
lines, about 30 for the literal's refusal), the guard (27 words, 3 places each).

**Optional third binary for the clock**: `timing/heroes-s1` (`sha256
1a5824e1fd7eb5647f4f0400fa5097793d71daf18f9bce219a2e027f3b42c1d1`, `sources 20325b0f12fd39d3`, runtime
`timing/runtime-s1`), run once without a clock over the same input, exit 0:
`env HEROES_RUNTIME=$S/timing/runtime-s1 $S/timing/heroes-s1 build selfhost/main.hero --emit-c -o /dev/null`.
S1 costs nothing on the path that does not abort; what it costs the build is 449 renderers written and
compiled.

### 15. How S1 and the lldb formatter are held to one notation (item 5)

**Not one generated from the other**: the formatter is Python reading a stopped process's memory through
the binary's debug information (`heroes_lldb.py`'s docstring), the renderer is C reading a live value
the program owns; they share the notation table and nothing else, and no single source can be compiled
into both without a generator neither tree has. **One corpus of values both render, compared by a
test**: one program declaring each value of the corpus (every scalar width, a `str` with every escape
and an ESC, a record, a nested record, `[T]` under and past `SHOWN`, a `str` past `TEXT_BYTES`, a
variant, a `T?` both ways, a map), run twice, once failing an `assert` per value so its `left:` lines are
S1's text, and once stopped under lldb (`tests/golden/lldb/`, `tests/harness/suite_lldb.hero`) so the
formatter's summaries are printed; a harness check holds each pair equal. About 40 harness lines and two
goldens; unbuilt. It fails today on the map (§ 11) until the formatter sorts too, which is the point of
having it.

### 16. Where the prototype stands

Uncommitted in this seat's worktree, `.claude/worktrees/230-compiler-engineer` (detached at
`595d92d88`; `git diff --stat -- selfhost runtime`: 20 files changed, 208 insertions, 85 deletions,
and four new files, `runtime/parts/place.c`, `runtime/parts/show.c`, `selfhost/emit/place.hero`,
`selfhost/emit/sides.hero`). The scratch, `.claude/worktrees/230-compiler-engineer-scratch/`, holds the
frozen compilers and runtimes of § 8, the clean input, the counting runtime of § 9, the runtime copies
of §§ 10 and 13 (`runtime-checkfirst/`, `runtime-exit/`), the seeds of § 6 and § 13
(`seed-before.c`, `seed-after.c`, `seed-s1.c`), and every probe program with its output (`l1run/`,
`cases/`, `items/`, `m1/`, `s1/`). No commit, no ref and no index was touched.

## Verdicts, per route

**No veto.** Nothing the sitting judges adds a core construct: design.md Part 5's seven are unchanged,
`assert` stays sugar (*a branch plus `panic`, carrying the source text and both sides*, which is what
S1 renders), `main -> ()?` is a type rule and a shape of the emitted `main`, `?` already lowers to a
return. And nothing measured breaches §1.1's ceiling: +238 code lines on 122,243, about 380 runtime
lines on 14,436. The seat's objections are to what the text promises and to the order, not to the size.

| route | verdict | design.md | the measured reason |
|---|---|---|---|
| **L0**, macros over `__FILE__`, `__LINE__`, `__func__` | object | §4.17 | `.must()` and `assert` stand under the generated file's `#line` on purpose (`emit/abort.hero:8-10`); no column; `__func__` is the C name; the file is the invocation path |
| **L1 for kind 1**, the place as literals on the failing branch | **approve** | §1.12, §1.1 | built: exactly the P variant's places on three items, the convention's columns, a thread, `-O2` identical; zero instructions where nothing aborts; +83 lines; seed +3.29%, `__text` +3.15%, `__DATA_CONST` +0 |
| **L1a for kind 2**, the entry point takes the place on every call | **approve, with a pointer to a per-site static record** | §1.1, §4.20 | built for the slice with literals (six instructions a call); 18.9M slices and 17.2M stores per self-emission make two instructions (a pointer) the cheaper form; `:62` runs 0 times; new names, never new parameters |
| L1b, the check moved into the program's C | object | §1.1 | the runtime keeps its check, so the path that does not abort pays both; a slice's character boundary is byte loads inline at 584 sites |
| L1c, a status the program's C tests | object | §1.1 | a store, a load, a branch and a frame slot per call for what L1a gives |
| L2, the line table read at the stop | object | §1.1, §1.12 | the runtime has no debug-format reader (`grep -i 'dwarf\|debug_line' runtime/`: none); a Darwin binary carries no DWARF (finding 5); Windows has PDB; it would run inside a signal handler |
| L3, a thread-local current site | object | the milestone page's rule | unbuilt; a store before every call that can abort, which the page refuses |
| **L4 for kind 3** | **approve (it is today's mechanism)** | §1.12 | the handlers already say `in deep.down`, `called from`, `under`, on a started thread too; owed: the reader's names, `thread_deep` and not `threaddeep` |
| L4 for kinds 1 and 2 | object | §1.1 | at `-O2` `rects.main` holds `normalized`'s code (`otool -tV`), so a PC names the caller; Windows has no walk |
| L5, the address resolved off-line | object | §4.17; CLAUDE.md § 10 | the message stops carrying the place; a verb; an address a golden cannot pin |
| **§ 6 as drafted** (*saying why and where: the file, line and function*) | **object** | §1.12 *what would make this wrong* | false for kind 3 by construction (25 `panic:` lines with no call site) and for each kind-2 class not yet located (the store printed no place, § 5) |
| **S1** | **approve, with four conditions** | §4.18, §4.20, §1.12 | built: `rects`' sides exactly, `[Rect]`, quoted `str`, 2225's `assert` shape on one line, bounded; conditions below |
| S2, records of scalars, arrays by length | object | §4.18 | two `[Rect]` that differ render alike, by length, which is the commonest aggregate `assert` measured here |
| S3, the first difference with its path | object | §1.1 | a per-type differ walks two values at once and is larger than a renderer, and still needs the map's order |
| S4, today | object | §1.12 | defect 2225's forged `right:` line and the invisible `""` side are measured false messages |
| M1, `main -> ()?` | object (no veto) | §1.0, Principle 0 | not needed for self-hosting (the compiler's `main` gives statuses M1 cannot); `handled.hero`, the program issue 1001 measured, still exits 0 under the M1 compiler (run); its ticket is Part 11 alone, unmeasured; 20 lines and 25 runtime lines, not core |
| M2, `main -> i64` | object | Principle 0 | M1's case, and a status range to rule |
| M4, M1 with the `fail`'s place | object | §4.20 | a place in a `fail` is a field of `HeroFailure`, the struct-field change §4.20 calls not self-guarding, and every `T?` grows |
| **M5**, a built-in that writes a line on stderr | **approve** | Principle 0 | the compiler writes its errors through a binding of the runtime's own header today; a built-in it would use, portable, tens of lines, no construct; serves tasks that forbid `extern` |
| M6, a status carried by the failure | object | Principle 0 | M1's case; serves 13 of 40 failing cases M1 cannot |
| M7, the obligation | object | §1.12 | refuses `examples/calculator`, a correct program |
| **`exit(code:)` outside 0..255** | **approve: abort a computed code, refuse a literal** | §1.12 | 256 → status 0 in silence (defect 2224), measured; the abort built (134 and a message); owed: the compiler's Windows forwarding leaves by its own entry point; clamping and *the low eight bits stated* both keep a falsehood |

**The four conditions on S1**: (1) **renderers generated for the types that stand as sides**, by a
worklist, not for every record under `HERO_TU_LOCAL`'s `used` (449 renderers and +323,157 seed bytes as
built); (2) **the entry point prints the `assert`'s head line and place before it calls the renderers**,
passed in, so a renderer's allocation failure loses the sides and never the head; (3) **a map's entries
in the order of their rendered text**, ties broken by the full renderings, **and the lldb formatter
writing the same order** (or the sitting recording the one difference); (4) **a line when the two sides
render alike** (`nan`, a difference past a bound).

**The recommended route, per half**:
- **Where**: L1, literals on the failing branch for kind 1; L1a with a pointer to a per-site static
  record for kind 2, every entry point a program's own operation reaches (the store first, after a seam
  in `container.hero`); kind 3 as the runtime says it today, with the reader's names; **§ 6 narrowed**
  to what that delivers, for instance *an abort the program's own operation raises names the file, the
  line, the column and the function; a fault the runtime catches names the function it stopped in,
  where the platform can tell*. Landed **after** defect 2225's `.must()` line repair.
- **The sides**: S1 under the four conditions.
- **What `main` tells the shell**: M3 (today) plus M5, the stderr line `exit(code:)` lacks; M1 waits
  for a measured Part 11 effect (Principle 0), and if the sitting takes it, the order measured here for
  the specification's § 13 exit check: the failure's line, then the check, whose abort wins at 134.
- **`exit`'s range**: 0..255 everywhere; a literal outside refused at compile time, a computed one an
  abort with its message; the compiler's own forwarding of a Windows child's status by an entry point
  of its own.

## The seat's output

- **verdict**: object (no veto)
- **section**: design.md §1.12 (*what would make this wrong*), §1.1 (the ceiling), Part 5 (core
  versus sugar), §4.20 (a function is self-guarding, a struct field is not)
- **implementation_cost**: measured: `selfhost/` +238 code lines of 122,243 (L1 83: `emit/place.hero`
  51, `emit/ops.hero` 10, `emit/inst.hero` 6 of its decided 350, `ir/flatten.hero` 5 to exactly its
  decided 1250, `emit/container.hero` 1 to exactly 300; M1 20; S1 135, `emit/sides.hero` 123); runtime
  `parts/place.c` 141, `parts/show.c` 189, header 51; `HERO_RUNTIME_ABI` 30 → 31 by `abi-put-back`, the
  old names kept until the refresh; 27 guard words; seed 27,000,911 → 27,889,403 (+3.29%, L1 and M1),
  28,212,560 with S1 as built; the compiler's `__text` +3.15%; 402 kept emissions blessed again; 1,750 of
  the compiler's 1,752 tests unchanged
- **needed_for_self_hosting**: no (L1, S1, M1); M5 alone is used by the compiler today, through a
  binding
- **argument**: Where it was built, the mechanism is sound and cheap: kind 1 and the slice print exactly
  the brief's places, on a thread and at `-O2`, at no cost where nothing aborts. But the drafted § 6
  promises file, line and function for every abort, and L1 reaches neither kind 3 (25 `panic:` lines
  with no call site) nor, until each entry point is widened, kind 2: the store, 17.2 million calls a
  self-emission, printed no place, and locating it needs a seam in `container.hero`, at 300 of 300.
  L1 landed before defect 2225's `.must()` repair puts a real place under a forged line. Narrow the
  sentence, order the commits, condition S1.
- **prediction**: the coordinator's `/usr/bin/time -lp` of § 8 reads the after compiler's instructions
  retired at **between 1.00 and 1.04 times** the before compiler's, for `--emit-c` warm and for the cold
  whole build alike (the C 3.3% longer; the compiler's own extra work estimated, unrun, at about 0.1%),
checkable in this
  sitting before its synthesis; and at M-panic-location's close, with L1 complete (the store included)
  and S1 reachable-only, `seed/heroes.c` measures between 27.80 and 28.05 MB.
- **condition**: § 6 narrowed (or a kind-3 place measured on all three platforms); every kind-2 entry
  point a program reaches located, the store first; the `.must()` line's 2225 repair landed first; S1's
  four conditions. Each met moves its row to approve. A veto would follow an L2 or L5 route adopted as
  § 6's mechanism (a debug-information reader in the runtime or the compiler), or S1 placed in
  `HeroDesc`.

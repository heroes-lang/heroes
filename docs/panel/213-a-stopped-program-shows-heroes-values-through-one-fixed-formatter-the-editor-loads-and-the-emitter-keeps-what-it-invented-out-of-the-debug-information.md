# Panel 213: a stopped program shows Heroes values through one fixed formatter the editor loads, and the emitter keeps what it invented out of the debug information

**Convened and written 2026-10-12 to 13, on the tree frozen at `0b8307daf`.** M-typed-inspection's
opening sitting, on GitHub issue 939, convened without asking: the author's answers of 2026-10-12 asked
for it once for the milestone (https://github.com/heroes-lang/heroes/issues/2029). **Full panel**: the
tool's surface is a trigger (CLAUDE.md § 4). Seats: compiler-engineer, ffi-pragmatist, historian,
spec-warden, llm-ergonomist, and a completeness critic twice. The llm-ergonomist ran as a subagent **not
blind**, as at panel 212, by the author's choice of 2026-10-13 (*like 212, free*), no paid run.
**Resolution: provisional, author ratification pending.**
Briefs: `docs/panel/213-briefs/` (shared brief, five seats, the critic, three frames and a symptom under
`samples/`, and how each frame was made under `samples-provenance/`). Reports: `docs/panel/213-reports/`
(the five seats, `critic-briefs.md` and `critic-reports.md`, the ffi-pragmatist's sources and transcripts
in `ffi-pragmatist-work/`, the critic's in `critic-reports-work/`).

## The proposal, verbatim

From the shared brief, `docs/panel/213-briefs/00-shared.md` § The sitting:

```
A program built at -O0 and stopped in a debugger, lldb directly or the editor extension's
lldb-dap, should show what the author wrote: p and not h4_p, a record's fields x and y and
not f_x and f_y, a [T] and a {K: V} with their contents and not an opaque header pointer, a
T? with its one live arm, a variant as its case and payload, and a frame without the emitted
C's temporaries. Routes: A nothing; B a formatter module the build emits beside the binary;
C formatters embedded in the binary, in a section the debugger loads by itself; D a heroes
surface that starts the debugger with the formatters; E names a debugger reads with no
formatter; F the frame's temporaries hidden by the emitter or filtered by the debugger side;
and, named by the critic at pass 1, G one fixed formatter per toolchain, H emitted C types
that carry the shape, I a type table in the binary, J the editor's side alone, B without
Python, and gdb.
```

What the sitting had to rule (issue 939 and the milestone's page): whether CLAUDE.md § 10's *never a
script* forbids a formatter nobody types; the surface; whether the frame filter needs the `$`-slot
distinction kept past the mangler; Windows; and how the editor extension consumes it.

## What the sitting found before it judged

In order of what each changed:

1. **The briefs steered the ergonomist four ways** (critic, pass 1): the first frame showed a variable out
   of scope at the stop, rendered only the maximal route, came with a program that gave the bug away, and
   leaked its provenance. The briefs were repaired before any seat ran: three frames stripped of their
   labels (names only, today's, all routes) and a symptom in place of the program.
2. **The CI's Linux legs install lldb 18, and LLVM 20 is the first release that loads a formatter embedded
   in a binary** (critic, pass 2: `apt-cache policy lldb` on `ubuntu:24.04` printed `1:18.0-59~exp2`;
   `FormatterSection.cpp` is 404 on `release/18.x` and `19.x`, 200 on `20.x`). The seats' Linux evidence
   was the local image's lldb 22.1.8. **Route C is dark on two of the CI's legs.**
3. **On every released lldb, `[T]` and `{K: V}` need Python** (critic, pass 2, reading LLVM's source):
   LLVM 22.x's loader takes only a summary from `__lldbformatters` and drops every synthetic-children
   signature; no bytecode can name a type, and `HeroArrayHeader *` leads only to `const HeroDesc *`.
   **Corrected by the coordinator, 2026-10-13, before the synthesis was committed**: the critic read
   `release/22.x` and `main` and not `release/23.x`. LLVM 23.1.0 was released on 2026-08-25 and 23.1.3 on
   2026-10-06 (GitHub's releases of `llvm/llvm-project`), and `release/23.x`'s `FormatterSection.cpp`
   loads synthetic children from the binary (`CreateSyntheticImpl`, *"Loaded embedded type synthetic"*);
   what 23.x's bytecode lacks is `create_child_at_offset`, `get_pointee_type` and `get_byte_size`, which
   only `main` holds (`grep -c` over each branch's `FormatterBytecode.cpp`: 0, 0 and 1 for the first).
   So an element in-line after the header has no bytecode route on a released lldb; whether an emitted C
   type that carries the element (route H) gives 23.x one is unrun. Homebrew's stable `llvm` is 23.1.3;
   this Mac's Xcode lldb is `lldb-2103.0.34.103`.
4. **The author's name written into the emitted C breaks correct programs** (ffi-pragmatist and
   compiler-engineer, both run): under the build's own `-std=gnu11`, `linux = 1` is refused by clang on
   Linux and built on this Mac; `errno = 3` is refused on both; `stdin = 1` builds and the debug
   information names the variable `__stdinp`; `true` collides through raylib's `<stdbool.h>`.
5. **Two slots of one spelling, renamed, show a wrong value under the author's name** (compiler-engineer,
   run): a `match` whose two arms both bind `f`, stopped in the second arm, answered `p f` with the first
   arm's slot, at exit 0.
6. **Hiding what the emitter invented is cheap and cannot lie** (compiler-engineer, built and run):
   `__attribute__((nodebug))` on every temporary and every slot the lowering made took measurement 042's
   `shapes.hero` frame at line 30 from 111 locals to 11 and `locals.hero`'s `describe` frame from 15 to 3;
   a cold build of the compiler read 69.91 s real before and 70.04 s after; the objects shrank 14.1%, the
   emitted C grew 7.4% with the attribute spelled out on 347,551 declarations.
7. **A summary embedded in the binary can pick a `T?`'s live arm** (critic, pass 2, run on this Mac's lldb
   and lldb-dap and the image's lldb 22.1.8 and lldb-dap-22 with LLVM's own bytecode assembler); and every
   integer of an embedded record is ULEB128, so the seats' one-byte sizes are right only below 128.
8. **A binding not yet reached shows a plausible garbage value on every route** (compiler-engineer:
   `n = (v = 6809745884)`); `-ftrivial-auto-var-init=pattern` makes it read `0xAAAAAAAAAAAAAAAA` (critic,
   run; its cost on a build unmeasured).

## Verdicts

| seat | verdict | section | cost or delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **object**; **veto** on E carried by C tokens and on any rename without the one-spelling rule | design.md §1.1, §1.7, §1.12; CLAUDE.md § 10; § Precedence rank 5 | F +11 −2 lines in `selfhost/emit/prologue.hero`, built; E through the debug information 2.2 times the clang stage of a cold `-O0` build (59.2 s against 26.7 s of CPU over 619 units) plus an estimated 120 to 160 lines, unbuilt; D an estimated 20 to 40 lines | with F, `describe` lists exactly 3 locals and `shapes.hero` at :30 exactly 11; `seed/heroes.c` grows at most 8% (at most 4% with a macro); E on every `-O0` build costs at least 25% more CPU on a cold compiler build | withdraws E's cost objection on a single-invocation route to the names in the debug information; lifts the C-token veto on a proof that the emitter knows every macro a unit sees |
| ffi-pragmatist | **approve** B + C + H-by-typedef with J or D as B's loader; **veto** E renaming the emitted C identifiers | design.md §1.11, §4.19, Part 8 item 12, §1.12 | `HERO_RUNTIME_ABI` moves for no route; no descriptor field; a Python formatter of about 280 lines printed every shape on this Mac's lldb, Linux's lldb 22.1.8 and both lldb-daps through `initCommands` | no step of the milestone moves `HERO_RUNTIME_ABI` off 30 or adds a field to `HeroDesc`; a compiler-written `__lldbsummaries` record shows in lldb-dap with the extension unchanged | moves to C alone if bytecode builds `[Point]`'s typed children; vetoes `--gc-sections` while a section lacks `retain` |
| spec-warden | **object**, no veto | design.md §1.6, §1.0, CLAUDE.md § 2 and § 10, panel 016's stopping rule | 0 spec tokens for every route (7520 legacy, 7655 cl100k, real 10050 pinned) | no milestone commit touches `spec/`; `Command(` stays 13; build's and run's flags unchanged; the `locals` case shows `x` and `y` | approves D only on the author's admission; takes G before B if bytecode cannot carry containers where the milestone delivers |
| historian (advisory) | **approve** C + E; **object** to B as the primary surface | the precedents | none | a `__lldbsummaries` record is applied by this Mac's lldb with nothing typed | changes if the CI's lldb predates the loader (met: finding 2) or bytecode cannot reach `[T]`'s element type (met: finding 3) |
| llm-ergonomist | **approve** the rendering with source names and values in the spec's own syntax; **object** to the two C renderings | the thesis, as a reader | the bug in 3 lines read with frame 3, about 11 with frames 1 and 2 | on a stop-diagnosis task set, frame 3 at least 15 points above frame 1 and 20 above frame 2; 0 answers citing a temporary under frame 3 | object to frame 3 if it drops the address of a `ptr` or a handle |

The ergonomist **was not blind**: `CLAUDE.md`, the memory index, `.claude/rules/verification.md` and
`.claude/rules/records.md` reached it as context, and it names one sentence of `verification.md` it cannot
unknow. By its own definition the reading is void until re-run from a copy; the author chose the non-blind
run.

## Disagreements, stated plainly

- **Names in the emitted C**: the historian approves them; two seats veto them, each with a program that
  fails (finding 4). **The measured side holds**: the historian ran nothing, and its precedent (Nim keeping
  most locals unmangled, an IRC line of 2017) says nothing of the headers a Heroes unit includes, whose
  macros are what the two programs met.
- **Route C**: the spec-warden would adopt it and ran it for a record; the compiler-engineer objects that
  it needs an assembler. **Both are true, of two sections** (critic): `__lldbsummaries` is plain bytes,
  `__lldbformatters` is bytecode. What decides it is finding 2 and finding 3, which no seat had: C is dark
  on the CI's Linux legs and cannot carry containers on any released lldb.
- **The names through the debug information (E)**: the compiler-engineer adopts it only for builds that ask
  for it (D); the spec-warden refuses D by the stopping rule. **The panel cannot admit a flag**: the
  spec-warden names the author's admission as the one route, and so does `.claude/rules/cli-surface.md`
  (panel 179's reading). The question goes to the author (R4).
- **A formatter file**: the ffi-pragmatist approves B with a loader; the spec-warden objects that a file
  written per build can meet another build's binary and show a value that is not there. **G answers the
  objection**: one file per toolchain, keyed on the runtime's C type names and reading every offset from
  the binary's own debug information, so no build writes it and a stale copy steps aside rather than
  misreads. The spec-warden's own condition for G (bytecode cannot carry the containers) is met by
  finding 3.
- **The ergonomist scored frames no route produces** (critic, pass 2): the realistic frame after this
  resolution, C names with Heroes values and no temporaries, was not scored. Its prediction stands for a
  task set this milestone does not build.

## Resolution (provisional: author ratification pending)

**R1, the emitter keeps what it invented out of the debug information (route F).** Every slot whose kind
is `synthetic_slot` or `return_slot` and every temporary is declared with `nodebug`, through a short macro
defined in each emitted unit's own preamble, so `runtime/heroes_runtime.h` and `HERO_RUNTIME_ABI` do not
move. A parameter is never hidden. **It cannot show a wrong value**: a hidden slot has no debug entry. The
`#pragma clang attribute push` form (two lines per function, the critic's) is not taken: it needs the
prologue to group the hidden slots, which is untested.

**R2, one fixed formatter for lldb (route G)**, in Python, at `editors/lldb/heroes_lldb.py`. It shows a
record as `Point(x: 3, y: 4)` with children `x` and `y`, a `str` by its length with the spec's escapes, a
`[T]` and a `{K: V}` with their elements, a `T?` as its one live arm, `ok(...)` or `fail(...)`, and a
variant as `.case(...)`. **Its soundness rules**, each a case of the `lldb` suite: every offset and size
from the binary's own debug information; an element's type from its descriptor's symbol or the debug
information's static of that name, else the address and never a guess; `len` and `cap` bounded; `f_`
stripped only in the types the emitter made; any failure falls back to lldb's own display, with no
traceback; Python that Xcode's lldb runs.

**R3, the editor loads it with nothing typed (route J)**: the extension prepends the import to lldb-dap's
`initCommands`, and the packaging copies `editors/lldb/` into the extension. At lldb's own prompt it is
one `command script import` line, which `editors/README.md` gives. **On § 10**: the letter (a second binary,
a script, a Makefile reaching a capability outside the one command) does not reach a component the
debugger loads, any more than it reaches the extension's own JavaScript; the substance, *nobody types*,
holds in the editor and costs one line at lldb's prompt, which the sitting records rather than hides.

**R4, the author's names (route E)**: **refused in the emitted C** (the two vetoes, finding 4).
**Through the debug information it is sound** on the compiler-engineer's three conditions (a spelling two
slots share keeps its index; members renamed only in the emitter's types; an `@` parameter never renamed
into a pointer under the value's name), **and not this milestone's**: on every `-O0` build it costs at least
the measured 2.2 times the clang stage, which § Precedence rank 5 refuses for a display; behind a flag it
needs the author's admission. **The question to the author**: admit a flag for it (a debug build the editor
asks for), or leave the names as they are. Recommendation: leave them, for now, because the ergonomist's
frame with names alone was the one it objected to, and what moved its reading was the values and the empty
frame, which R1 and R2 deliver; and revisit when a single-invocation route exists.

**R5, a `heroes` surface (route D)**: not admitted by the panel. Nothing in the fixpoint, the golden harness
or Part 11 must type it, and `build -O0` then `lldb` compose (spec-warden). The author's admission is the
only route.

**R6, formatters embedded in the binary (route C)**: **not this milestone's.** Return condition: an lldb
whose embedded bytecode can make an element at an address (`create_child_at_offset`, on LLVM's `main`
today and in no release; LLVM 23 already loads synthetic children from the binary, finding 3), released
and installed on the CI's legs. Then a sitting moves R2 into the binary, where it needs no loader and no
Python, which is what the author asked for first (§ Author's verdict).

**R7, the `$` distinction does not have to survive the mangler**: the prologue reads the slot's kind where
it writes the declaration (compiler-engineer). No slot table, nothing beside the binary.

**R8, Windows**: no route reaches it while LLVM's `lldb.exe` exits `0xC0000135` before any command
(`.github/workflows/ci.yml`); the `lldb` suite's Windows skip stays, and `heroes doctor` gains no row,
so the question of a third outcome does not arise this milestone.

**R9, a binding not yet reached**: not this milestone's. Its routes (a live range per slot through the
lowering; `-ftrivial-auto-var-init=pattern`, which makes the garbage recognisable) are unpriced.

**R10, design.md Part 2's debugger non-goal is amended, not retired** (spec-warden's reading), in its own
commit once R1 to R3 land and are measured: the line half stays; *"`p x` shows a mangled C temporary"*
becomes what lands (at `-O0` the frame lists the author's bindings under their mangled names, and the
formatter shows their values in Heroes' syntax); what is not delivered is named (the names, `-O2`'s line
tables alone, Windows, rendering inside the stopped process); *"printing rich values is still `print`'s
job"* stays. Part 8 item 12 (the mangled names are what lldb shows) stays true as written.

### What the conservative resolution would have been, so the author can choose it

R1 alone, with `__lldbsummaries` records for the record types: no Python in the tree and no § 10 question,
records readable at lldb's prompt on lldb 20 and later with nothing typed, and `[T]`, `{K: V}`, `T?` and
variants left as today until an lldb release carries synthetic children from the binary. The sitting takes
R2 and R3 because they reach every shape on every lldb the milestone's instrument runs, the CI's lldb 18
included, and because one formatter is one place where the display can be wrong rather than two.

### What a veto would compel

The compiler-engineer's veto binds R2: a child renamed keeps its index wherever two share a spelling, and
`f_` is stripped only in the types the emitter made. The ffi-pragmatist's binds any later C: a section kept
through `--gc-sections` carries `retain`.

## Prior art

Panel 213 (`docs/panel/213-a-stopped-program-shows-heroes-values-through-one-fixed-formatter-the-editor-loads-and-the-emitter-keeps-what-it-invented-out-of-the-debug-information.md`,
`docs/panel/213-reports/historian.md`), every link re-read 2026-10-13. The sitting's decision issue
carries the same lines.

- **Reached first by Nim**: [nimlldb.py](https://raw.githubusercontent.com/nim-lang/Nim/devel/tools/debug/nimlldb.py): one lldb Python module, initialised by `__lldb_init_module`, registers type summaries and synthetic children for the C type names the compiler emits, matched by regular expressions such as `tySequence_+[[:alnum:]]+$` and `NimStringV2$`.
- **Reached first by Zig's users**: [Display slices in the debugger](https://ziggit.dev/t/display-slices-in-the-debugger/5079): the editor's plugin will not read `.lldbinit` from the home directory, so the commands that load the pretty printer go into `initCommands` in `launch.json`.

## Predictions to score

At M-typed-inspection's close:

- **compiler-engineer**: with R1, `describe` lists exactly 3 locals and `shapes.hero` at :30 exactly 11;
  `seed/heroes.c` grows at most 8% from 52,126,543 bytes, at most 4% with the macro. Its E prediction is
  not scorable: E does not land.
- **ffi-pragmatist**: `HERO_RUNTIME_ABI` stays 30 and `HeroDesc` gains no field (`git diff` of
  `runtime/heroes_runtime.h` over the milestone). Its lldb-dap prediction is about C and is not scorable.
- **spec-warden**: no milestone commit touches `spec/heroes-spec.md`; `grep -c "Command(" selfhost/cli/table.hero`
  stays 13; `build`'s and `run`'s flags unchanged; the `lldb` suite shows the record with `x` and `y`.
- **historian**: about C, not scorable.
- **llm-ergonomist**: needs a task set this milestone does not build; not scored.
- **the sitting's own**: the `lldb` suite's new cases pass on this Mac and on both Linux legs of the CI,
  lldb 18 included, after the push.

## Process notes

- **The critic's first pass changed the experiment before it ran**, and its second pass changed the
  resolution: the CI's lldb version and the source of LLVM's loader were in no seat's report.
- **`strings` over the LLDB framework misled** (critic): it listed `read_memory*` selectors that nothing
  implements and missed `cast`, which 22.x does. A source read is the instrument.
- **Three timings ran beside other sessions' work**; the compiler-engineer discarded the `real` of its E
  pass and kept its CPU time.
- Defect 2034 was repaired in its own lane while the sitting ran, beside the frozen tree.

## Author's verdict

Pending: https://github.com/heroes-lang/heroes/issues/2037 asks for it, naming the sitting as
`panel 213`. A yes settles R1 to R10 as written and R4's names the way the author answers them; it does
not settle a flag for the names, formatters embedded in the binary or a route for a binding not yet
reached, each of which needs its own sitting or the author's admission.

**2026-10-13, about 01:15, the author on R2 and R3, read before the synthesis was committed.** The author
asked what *Python* meant here, and said they did not want to depend on it and would rather raise the LLVM
required. Put to them with what was measured (an lldb from LLVM 20 reads formatters from the binary with
no Python; records, strings and a `T?`'s live arm shown that way tonight; LLVM 23 loads synthetic children
from the binary but cannot make an element at an address, which only the unreleased LLVM 24 can; Windows'
`lldb.exe` dies without its Python DLL), with three answers: LLVM 23 with the arrays' contents after, LLVM
24's development line, or Python kept. **The author chose to keep Python**: R2 and R3 stand as written.
The rest of the ratification stays pending on the issue above.

**Ratified, 2026-10-14**, a reading: the author's word at about 00:30, meant as *I ratify the panel*,
given after M-typed-inspection's close. R1 to R10 stand as written. R4's question takes the synthesis's
default, the names left as they are and no flag admitted for them; R3's reading of CLAUDE.md § 10 stands,
a component the debugger loads not being a script. R2 and R3 had been the author's own choice at 01:15 on
2026-10-13, above. What the yes does not settle is what the stub names: a flag for the names, formatters
in the binary (R6's return condition), and a route for a binding not yet reached.

## Scored at M-typed-inspection's close, 2026-10-13

Each by the command named, on the milestone's closing tree.

- **compiler-engineer**: with R1, `describe` lists exactly 3 locals and 042's `shapes.hero` at :30 exactly 11: **held**, 3 and 11 (step 4's agent, `c6f4d0157`; measurement 043 read 11, 10 and 7 over the three census frames). The seed grows at most 8%, at most 4% with the macro: **falsified**, 52,126,543 to 56,650,897 bytes at step 4 alone (+8.7%, `wc -c`), the macro written 347,570 times; the seed is one fused unit, where declarations weigh more than in the per-unit build, whose C grew 3.8%. Its E prediction is not scorable: E did not land.
- **ffi-pragmatist**: `HERO_RUNTIME_ABI` stays 30 and `HeroDesc` gains no field: **held** (`grep -n "define HERO_RUNTIME_ABI" runtime/heroes_runtime.h` reads 30; `git diff a9be36dc5..` over the header is empty; `HeroDesc` holds its five members). Its lldb-dap prediction was about C: not scorable.
- **spec-warden**: no milestone commit touches `spec/heroes-spec.md`: **held for the milestone's own steps**; the spec's opening line did move the same night, by the author's instruction to change the claim everywhere, not by this milestone (`7bb804f4b`, ledger row 111). `grep -c "Command(" selfhost/cli/table.hero` stays 13: **held**. `build`'s and `run`'s flags unchanged: **held**. The `lldb` suite shows the record with `x` and `y`: **held in the five `formatter-` cases**, which import the formatter; the `locals` case imports nothing and still shows `f_x`, as R3 leaves lldb's own prompt.
- **historian**: about C: not scorable.
- **llm-ergonomist**: needs a task set this milestone did not build: not scored.
- **the sitting's own**: the `lldb` suite's new cases pass on this Mac and on both Linux legs of the CI: **held** on this Mac, on the Linux arm64 image's lldb 22.1.8 and on Ubuntu 24.04's lldb 18.1.3 in a container (step 5's agent), and on the CI's four legs at `0c7eae194` (run 38350572891, all four green), the Windows leg skipping the suite by name as before.

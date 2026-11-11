---
paths:
  - "runtime/**"
  - "examples/**"
  - "selfhost/emit/ffi*"
  - "selfhost/parse/group.hero"
  - "selfhost/cli/**"
---

# The C boundary

Home of CLAUDE.md § 12's FFI half and § 7's clang-failure classes, since
2026-09-07. What each rule cost to learn is in `docs/records/contract/case-law.md`,
cited as `CL-NNN`.

## Why this boundary outranks the rest

Everything comes from C (design.md §1.11), so everything a program touches
arrives through §4.19, and this is where the language's own guarantees stop.
Robustness is a **goal of the language** and it wins here first: a Heroes program
must not segfault and must not corrupt memory (CL-012).

The author's instruction is that the FFI be **complete and bug-proof** (CL-028).
Complete, because a library Heroes cannot bind is a library the author must leave
C code around for. Bug-proof, because a binding is the one place where a Heroes
program can reach an address nobody checked.

## What the compiler already shuts

A null `cstr` reaching a C function is `panic: a null cstr was passed to a C
function`, exit 134, because `guard_arguments` wraps every `cstr` argument
on its way out (measured twice on 2026-08-24). `to_str` on a null `cstr` is a
clean abort in the runtime.

**The function's name was wrong here until 2026-09-15**, and it is corrected
rather than annotated because a rule file is read for its citations: this line
said `guard_cstr_arguments` and the function is `guard_arguments`,
`selfhost/emit/ops.hero:244`, called at `:121`. Panel 154's compiler-engineer
found it while pricing a symmetric guard for handles, and the stale name had
already been copied out of here into that sitting's own briefs and into defect
045's entry — which is what a citation nobody greps costs.

**Say this rather than the danger it replaced.** A contract that states a danger
the compiler has already closed funds the wrong decision next time, and it
nearly did once: a token payment read as *deleting a warning* when the compiler
had become loud in both directions (CL-028).

## A clang failure that the author's own extern caused

A clang failure is normally exit 2 and says the **compiler** is wrong. One class
is exit 1 with a diagnostic on the `.hero` line, and it has fifteen members
(panel 036, widened by panel 048, CL-008; the fifth by panel 166; the sixth by
defect 360, 2026-10-06, with no sitting, on defect 060's precedent: who is
blamed moves, the line between accepted and refused does not; the seventh,
eighth and ninth by panel 202, ratified 2026-10-10, its R2; the tenth by its
R3, the same day; the eleventh by panel 208, ratified 2026-10-10, its R2,
which counted it the seventh on the trunk's list of that hour; the twelfth
by defects 591 and 592, 2026-10-11, with no sitting, on defect 360's
precedent; the thirteenth by panel 239's R1, 2026-11-09, and the fourteenth by
its R4, the same day, both ratified 2026-11-09; the fifteenth by panel 245's R3,
2026-11-11, provisional, author ratification pending):

- a result type the header refutes, and a record's constant whose header value
  C will not build as that record, judged by the accessor's own declaration at
  the constant's line, told `ffi_constant_type` with no new code (defect 094,
  lane b12-ffi13, repaired at `e86a26e5`, 2026-10-06); and a number's constant
  whose header value C cannot read or leaves undefined, judged at its
  accessor's `return` at the constant's line, where clang's warnings that the
  value is undefined are errors, told the same way (defects 2071 and 2072,
  lane b17-emit, 2026-10-14). The first moved who is blamed, from `internal
  error` at exit 2; the second moved the line between accepted and refused,
  on robustness's rank: a shift past its width built at exit 0 and read what
  the machine held (`selfhost/emit/value_check.hero`, which also names the
  two undefined values it does not reach);
- a `constant` that is not one;
- a name the header does not have;
- a symbol the **linker** cannot find because the group named no `link`;
  and, widened 2026-11-08 by defect 2365 (lane b26-link, with no sitting, on
  defect 360's precedent: who is blamed moves, the line between accepted and
  refused does not), a name **a library the program links needs** and
  nothing on the link line defines, told `ffi_missing_link` at the string of
  the group whose `link` names that library, or of the `pkgconfig` group
  whose answer carried it, the library and every such name named
  (`cli/library_needs.hero`). The narrowing is the program's own words: every
  undefined name the linker reports must be referenced from a library a
  `link` or a `pkgconfig` answer's `-l` names (`cli/link_report.hero` reads who refers
  to each, in GNU ld's, ld64's and the lld family's words), so a reference
  from the program's own object stays exit 2; and, widened 2026-11-09 by
  defect 2372 (lane b26-cli, on the same precedent), a name **the text of a
  group's header puts in the program's own object**, a `static inline`
  body that calls it or a bound name the header makes a macro for it, told
  `ffi_missing_link` at the binding whose header's text reaches it, the
  header, the function and the name named (`cli/header_needs.hero`). The
  narrowing is clang's answer, never the linker's words, which name the
  header's function at `-O0` and the compiler's own caller at `-O2`: every
  reference in a module's own object or a library the program names, the
  name bound by no group and of none of the compiler's forms, declared by
  none of the unit's opening read alone, and named by a body or a macro of
  the module's header list (`cli/header_bodies.hero`); a reference no
  header's text explains stays exit 2. **Each name is judged by the reader
  whose question it answers** (defect 2381, lane b26-cli, 2026-11-09, the
  same precedent): a link missing a name a linked library needs beside a
  name a header's inline body needs was told by neither while each reader
  judged the whole link; it is told where every name is one reader's, each
  told as that reader tells it, in the program's order, and stays exit 2
  where any name is neither's (`cli/link_needs.hero`);
- **a `ptr` lend C would write through, rooted at a name the program may not
  write** — added 2026-09-19 (panel 166, defect 065). The lend crosses as
  `const void *` from a `=` binding or a by-value parameter, and clang's own
  qualifier error against the header's parameter is the verdict;
  `emit/ffi_lend.hero` reads it into `field_lend_written` on the lend. This is
  the one member that points at a **call** rather than at a declaration.
- **a header a group names, or one it includes, that clang refuses on its
  own**: added 2026-10-06 (defect 360, batch 12, repaired at `6464dd6c`). Told
  `ffi_header_refused` at exit 1 on the group's string with clang's located
  words, narrowed by the include stack as `ffi_missing_header` is; until then
  it was *internal error: compiling the generated C failed* at exit 2, the
  author's header blamed on the compiler. **Widened 2026-11-11 by panel 245's
  R1** (provisional, author ratification pending; defects 2464 and 2469),
  moving the line between accepted and refused on robustness's rank: in a
  header's own code every warning clang gives is an error
  (`header_region.LINE_WORDS`) but `-Wall`'s, sign conversions and
  deprecations, passed over, and a `#warning` or a `#pragma message`, told and
  never refused, so a door's `1 << 40`, a `strncat` sized wrong or a call a
  header marks `warning` is refused where it built at exit 0 with clang's raw
  words; an order trial tells no order the unit refuses
  (`cli/order_search.hero`'s `sees`, a generating trial where the syntax pass
  compiles the written order); and a unit above `-O0` holding the region is
  compiled once more at `-O0`, its refusal this member's and its note saying
  that compile made it (`cli/region_level.hero`, `cli/unit_verdict.hero`). And
  by its R2, a door handing on its format alone, `printf(f)`, is this member,
  `-Wformat-security` kept with `-Wformat` in the region.
- **two headers that each compile alone, refused together** in one unit:
  added 2026-10-10 (panel 202; defects 538, 550, 557 and 561, repaired in
  batch 18). Told `ffi_header_refused` on the group whose header sorts first,
  both headers and both lines named, a file one of them includes named with
  the header it arrives through (`cli/headers_together.hero`); a header that
  compiles alone and fails after an earlier one, with no note in it, is
  found by compiling the two alone (`cli/header_alone.hero`). Each module's
  unit reads its own groups' headers, so the two are one module's, or
  `heroes test`'s one unit's.
- **one C name two modules' headers declare two ways**, external in both:
  added 2026-10-10 (panel 202's C1, defect 555). Told
  `ffi_declared_two_ways` before the link, both files and lines and clang's
  canonical types named, a `static` definition passed over
  (`cli/two_ways.hero`); a name no group binds is held to it too, the
  sitting's widening, which refused no tracked program the bound names do
  not.
- **one C name the units of two modules would each define**: added
  2026-10-10 (panel 202's C2, defect 556). Told `ffi_defined_twice` before the
  link on every platform, a tentative definition included, the files, lines
  and modules named (`cli/defined_twice.hero`); until then the link said
  *duplicate symbol* at exit 2, and a tentative one linked on this Mac and
  not on Linux.
- **the one file `--emit-c` writes, where it cannot mean the program the
  units build**: added 2026-10-10 (panel 202's R3, defect 453). Told
  `ffi_one_file` at exit 1, nothing written, where a module's unit reads
  other headers than the file and clang refuses the file, naming the two
  modules and their headers, or where a module's binding has another C type,
  another macro or another dump in it (`cli/one_file.hero`); until then the
  file was written at exit 0, clang never reading it.
- **a name its header marks `unavailable`**, a function, a constant or a
  record, called or only bound: added 2026-10-10 (panel 208's R2, defect
  588). Clang refuses every use of it, the compiler's own probe of the binding
  included, and `emit/ffi_unavailable.hero` reads that error, wherever in the
  unit clang names it, into `ffi_unavailable` on the binding, once, the
  header's own words in a note; until then it was *internal error: compiling
  the generated C failed* at exit 2. A name its header marks deprecated is
  not a member: it binds and builds silent (design.md §4.19, panel 208's R1).
- **a linker name a header gives a C name, by `#pragma redefine_extname` or
  an `__asm__` label, that the linker cannot find**: added 2026-10-11
  (defects 591 and 592, lane b20-pragma). Read on the link's failure path
  from clang's dump of the module's header list, followed by the unit's own
  `heroes_standard.h` and `main` (`cli/renamed.hero`, `AsmLabelAttr`): the
  label of a function a group binds is told `ffi_missing_link` on the
  binding, the file and line clang gave it named; the label of a name the C
  this compiler writes uses, a runtime function read before the groups, a
  function of `<math.h>` read after them, or `main` renamed away, is told
  `ffi_header_refused` on the group's header. C has no way to take a label
  back (measured on this Mac: a second pragma does not replace it, a label
  written after it is *conflicting asm label*), so the program is told rather
  than protected; until then *internal error: linking failed* at exit 2.
  A label is the symbol as written, with no platform prefix, so
  `redefine_extname labs llabs` fails on Darwin and links in the Linux arm64
  image, where `llabs` is the symbol's own spelling.
- **a C name a group binds, or the runtime calls or defines, that a library
  the program names other than the one meant would supply**: added 2026-11-09
  (panel 239's R1 and R2, ratified 2026-11-09, decision
  2374; defects 2363 and 2378). Told `ffi_defined_elsewhere` on the binding,
  or on the group whose library defines a name of the runtime's, both files
  named, the same words in every order of the modules and under every name
  (`cli/supplied.hero`, `cli/supplied_words.hero`). It moves both lines the
  preamble's history names: **the line between accepted and refused**, on
  robustness's rank (`sig_sorted` built and died by a memory fault, Bus error
  on this Mac and SIGSEGV on Linux arm64; `dup_sorted` built and printed
  another library's `2`; `interpose`'s `puts` from a library of the program
  died the same way; `runtime_name`'s `printf` printed nothing), and **who is
  blamed** (`hero_name`, a library defining `hero_print_end`, and GNU ld's
  `multiple definition` of a bound name in one order, `internal error:
  linking failed` at exit 2 until then). The link is asked who supplied each
  name (GNU ld's and lld's trace, Apple's map), and where the program names
  two or more libraries a link with them in the other order and one with
  every library file of objects loaded whole, their answers kept by the key
  of every library file read (design.md §4.19). A library the program does
  not name is never judged; a group of the defining library that binds a
  runtime name says the replacement is meant. Not refused, said: a name no
  group binds and the runtime does not call, defined twice among the
  program's libraries, a known cost kept by the author's ruling of 2026-11-09
  on decision 2374's second question (defect 2375, pinned by the net's own
  case `intern`). On Windows the link is asked by its map, `/MAP`, lld-link's
  and link.exe's alike, and a library defining again a name a library the
  program does not name defines is told on its group (defect 2376, the
  Windows box, 2026-11-09).
- **a group record another module's unit reads otherwise than its own
  module's unit does, where no order of the headers makes the two alike**:
  added 2026-11-09 (panel 239's R4 and R5, ratified 2026-11-09; defect 2364). Each unit's header list is read by clang's
  record-layout dump and every record a unit spells of another module is
  compared with its owner's reading, the size, the alignment and each
  field's byte and type (`cli/agreed_layout.hero`); where an order of the
  other modules' headers in that unit, or their owners' earlier headers
  before them, makes every record agree, that order is the unit's and
  nothing is told (`cli/agreed_order.hero`); where none does, told
  `ffi_record_two_layouts` before any unit compiles, at the line where the
  reading module first spells the record, the two sizes and the first two
  fields that differ named (`cli/agreed_told.hero`). `--emit-c`'s one file
  is one more list, `ffi_one_file` where no order of the modules' own
  headers agrees. It moved both lines: until then two other modules' records
  in one unit followed the module walk, so a correct program was refused
  with a false `ffi_field_type` (`two_others`), and a record packed by
  another module's macro or by a pragma its header left set, or widened by a
  hidden field or a union, built at exit 0 and printed a wrong value
  (`pack`, `pragma`, `partial`, `union2`, each measured by the sitting on
  this Mac and Linux arm64). The member never sees a record whose reading
  unit's own list does not compile; that unit's compile tells it. Nor one
  that a file standing in for a header lays out otherwise: what a package's
  search may read is judged first and alone (`cli/package_stand_in.hero`,
  defects 2351 and 2357), since the round's gate of 2026-11-09 found the
  program's own `pt.h` standing in for the package `pt`'s told as two
  layouts in its place, and a program's own header that changes a package's
  record is still this member (both in `unsupported/`).
- **a name a library the program names supplies whose header and library come
  one from a folder the build names and one from none**: added 2026-11-11
  (panel 245's R3, provisional, author ratification pending; defect 2448).
  Told `ffi_unpaired_release` on the binding the name is reached from, one
  refusal per library, the header clang read and the library file the link
  took both named (`cli/supplied_paired.hero`, `cli/supplied_paired_words.hero`),
  judged from the link's own answer of the thirteenth member, the names a
  door's body reaches included; a header's named folders are panel 237's,
  a library's `--library`, a package's `-L` and `LIBRARY_PATH` from clang's
  link job, a folder clang's job adds itself the platform's where no other of
  its folders holds a second release (`supplied_names.tied`). It moves the
  line between accepted and refused on robustness's rank: curl 8.23.0's
  header beside the SDK's 8.7.1 library printed both at exit 0 under `CPATH`
  (batch 31's base), and readline's record read back `0` for `42` and
  overflowed the heap under `--sanitize` (the sitting). A name only the
  program's own folder declares is not judged; two halves each named by
  unrelated words build, a residue the sitting measured and left open.

**A variadic is bound only through a door since panel 245's R2** (provisional,
author ratification pending; defects 2465 and 2470), beside panel 243's R1, which
refuses every parameter past a header's `...`: a binding that names a variadic
with nothing past its `...` is refused `ffi_call_shape` at the binding, asked in
the check unit by clang's own `"variadic": true` (`emit/ffi_bare_variadic.hero`),
since the callee reads past it by a value only the run gives (`open` with
`O_CREAT`, `sqlite3_mprintf`, `ioctl`, each built at exit 0 and reading what the
machine held until then). Every refusal past `...` cites spec § 13 only for what
§ 13 holds, and C11 6.5.2.2p7 and 7.16.1.1p2 for what C does there.

**Since panel 214 (2026-10-14, provisional, author ratification pending) whose
a refused header is, where no reader names it, is decided by a clang run and
by a name's form, never by clang's words** (its R1: the readers stay, as how a
refusal is told). Where the readers would print `internal error`, the unit's
opening is compiled with the groups' headers and without them; refused with
them and compiled without them, the headers are what clang refused, told on
the group whose header the opening refuses first as its include lines are
added one by one in their order (R2, `cli/opening_told.hero`). A first error
in the program's C at a name of the compiler's own forms no group binds, `h_`,
`hero_`, `Hero`, `HERO_`, `HEROES_`, `main`, and the emitter's `f_<field>` and
`t<N>` where a header's macro meets them, its note in a group's header at that
name, is the header's (R3, `cli/own_names.hero`): a blame rule acting only
after clang refused, so the specification reserves no prefix. The runtime's
own names are held to those forms by the compiler's tests (R4), and
`<heroes library>` and the unit's own `.c`, as the `#line`s name them, are the
compiler's files (R5, `group_file.compilers`).

**Widened 2026-11-09 by defect 2389** (lane b27-names). The emitter's forms a
header's macro meets are now every name a function's body declares: `t<N>`,
`h<N>_<name>` (a parameter or a local), `ph<N>_<name>` (an `@` parameter's
pointer), and the names it writes there itself, `hero_here` before every C
call, `found` in a map lookup and the rest of `emit/shadowed.hero`'s `LOCALS`
and `GROWN`. Its test reads every word the emitter's code writes in a string
literal where C declares or assigns a name, and fails on one the set does not
know, past the words it lists as standing where no bound function is named
(`ELSEWHERE`: C's own words, struct members, the locals of `main`, a thunk,
the per-type functions and renderers, and the C of a probe).
And the macro is read from the first note under the error that expands one,
wherever that note stands: `#define t1 t2` puts clang's *redefinition of 't2'*
at the real `t2`, its first note *previous definition is here*, and was exit 2
until then, as was `#define f_left f_right`. The other half is no blame rule:
a group that BINDS a C function of one of those names is a correct program, so
the emitter steps around it rather than refusing it, an accessor written before
any body (`static inline __typeof__(t1) *hero_bound_t1(void) { return &t1; }`)
and the body calling `(*hero_bound_t1())`, and the extern table's reference
the same; a bound name of no such form is written as before, so no program
that binds none moved a byte of its C (233 `run` cases' `--emit-c` and the
three `examples/` that bind `hero_thread_*`, against the compiler of the
round's base, measured that day). **A group's record of one of those names**,
`record t1` beside the temporary `t1`, is a type a body spells after hiding it,
so it is renamed rather than reached: right after the group's include the unit
writes `typedef t1 hero_bound_type_t1;` and the unit's own names spell the
record by the alias (`emit/bound_type.hero`); a record the header tags lives in
C's tag namespace and keeps its spelling. The alias is the unit's alone: a
probe compiled apart builds names of its own and spells the header's `t1`,
which nothing there hides, and given the alias the layout screen read *use of
undeclared identifier* on each of its rows, measured that day before the
alias was kept out of it. Built, received from C, lent with `@`, held as
another group record's field and passed between two modules, `record t1` and
`record h0_p` now build and run, with every C file of their builds that names
the alias declaring it, and the same 233 cases and three examples
byte-identical.

**The narrowing is `declaration()`, not whose text it is.** Every class recovers
a name and asks whether *this program* declared it `extern`, or linked the
library that defines it, so a symbol nobody declared and no library of the
program defines stays exit 2 and the compiler's fault. The second half is the
thirteenth member's (panel 239, 2026-11-09): a name the runtime calls or defines
is the author's where a library a `link` or a `pkgconfig` answer of the program
names supplies it, recovered through that library, never through the name. The fifth
member recovers the lend from the IR at the line clang names, which is the same
question asked of the call: a qualifier error at a line where no immutable lend
is passed to C stays exit 2.

## The instruments

`--sanitize` adds `-fsanitize=address,undefined`, which catches use-after-free
and double-free. **Leaks are caught by `hero_runtime_check_leaks()`**, because
ASan's leak detector does not exist on Darwin arm64.

**Since 2026-10-07 the `run` suite also sanitises at `-O0`, gives both
sanitised runs ASan's free fill, and runs the `-O0` binary under Guard Malloc
on Darwin** (defects 390 and 321, lane b14-cli): ASan at `-O2` missed a stack
overwrite `-O0` catches, and a C library reading the bytes of a string already
freed was seen by no leg. LeakSanitizer runs in the Linux arm64 container too
(measured that day, a 77-byte leak; `.claude/rules/platforms.md`).

**A program that declares an `extern` runs its Linux leg under `--sanitize`**
(CL-055). LeakSanitizer exists on that leg and on no other, so a leak in a C
binding is invisible on this Mac in all three configurations. The rule is narrow
on purpose: a program without an `extern` cannot leak from the C side, because
its own allocations are counted on every platform.

**Corrected 2026-10-28, by panel 231** (the ffi-pragmatist's Q4, reproduced by the sitting's
critic, finding 4): the paragraph above is true of the net's legs and false of this Mac. Darwin's
ASan has no leak detector (`ASAN_OPTIONS=detect_leaks=1` is refused, *not supported on this
platform*), but **`leaks --atExit -- <binary>` on the plain `-O0` binary sees a leak in a C
binding here**: 200 leaks and 6,400 bytes of a `static inline` header's `malloc` at `-O0`, one
21-byte leak, a `-> cstr` bound without `owned`; and 0 leaks over 61 committed `run` cases and
seven generated programs, so it has not yet named a leak that is not one. At `-O2` clang deletes
that allocation, so a leak through C the optimiser can see is judged at `-O0` on every leg that
can judge it: on Linux by LeakSanitizer under `--sanitize -O0`, which the corpus runs since the
same day for a program that declares an `extern` (task 972), and on this Mac by `leaks`, by hand
until the generator's oracle 5 runs it. Unrun: `leaks` on the CI's `macos-26` runner.

**And since 2026-10-28, M-generated-programs step 11, the generator's oracle 5 runs it**: every
generated program with an `extern` that ends at exit 0 has its `-O0` binary read by `leaks
--atExit` on Darwin (`tests/harness/gen_cjudge.hero`, `leak_free`), 0.51 s a program on this Mac,
and LeakSanitizer reads the same program under `--sanitize -O0` on Linux. Each leg proves its
instrument live on every run of the `generated` suite: a header that drops 200 allocations must be
judged `LEAKED`, and a control that passes fails the suite as *the leak oracle is blind on this
leg*, which this Mac showed with `leaks` taken away. Windows has neither instrument and the control
says so as a skip. Unrun until the push: the control on the CI's Linux and `macos-26` legs.

**And the leak oracle on this Mac needs the developer tools' permission**, measured 2026-10-28 at
M-generated-programs' gate: `leaks` attaches to the program it starts, and with `DevToolsSecurity
-status` reading *disabled* an authorization dialog stood open and every attach waited on it, `leaks`,
`lldb`'s `debugserver` and ASan's `atos -p` alike, so `generated`, `lldb` and `run` could not end.
`sudo DevToolsSecurity -enable` grants it once; a reading cut by its 60 seconds says so
(`tests/harness/gen_cjudge.hero`, `leak_free`). GitHub's macOS images run that command in their
`configure-machine.sh` (read that day); the CI's Darwin leg is unrun until the push.

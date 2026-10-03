# DEFECTS — the compiler defects that are still open

Every item is a **measured** failure of the compiler on a program — a crash, a
wrong answer at exit 0, a silence where a message is owed — carrying its
reproducer, its cause where known, and what is owed. **Only open defects live
here**: a repaired one is ticked, gains a *The repair* section with the
measurements that prove it, and moves to `docs/records/done/`. A repair is owed
at the class and not at the witness, with a `tests/golden/fixedbugs/` case per
shape.

**The shape** is `.claude/rules/records.md` § The lists, and § A live list is a
preamble, a count and its items is why this preamble is fifteen lines. **The
next number is READ, never remembered** — `records/numbering` takes one above
the highest issued across this file and `docs/records/done/`. Who issued which
number since 2026-09-08, and why 014 exists twice, is
`docs/records/log/2026-09-16-2200-the-defect-register-leaves-the-list.md`.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 51**

- [ ] **143 — a function-like macro cannot be bound** | `extern "sys/wait.h"` with `function WEXITSTATUS(status: i32) -> i64`: `build` exit 1, *`sys/wait.h` declares no `WEXITSTATUS` — clang read the header and could not find it*, though the header defines it as a macro; design.md §1.11 says *Macros, `inline` functions and `#define` constants are now reachable directly* | the `extern` probe's parenthesized call (panel 092's `(fn)(...)`), which no function-like macro expands · `selfhost/emit/` (the probe) · **class: blocking**

    **Origin:** panel 184's ffi-pragmatist, 2026-09-30 (a header of its own
    and `sys/wait.h:144-146`); reproduced by the coordinator at 20:44 on the
    trunk's compiler at `a294a6ff` (`scratchpad/p184/ffi-side/macro.hero`);
    again at 00:17 on 2026-10-01 on `3cc3b553`, exit 1 and `ffi_unknown_name`.

    **Why it is a defect.** A program calling `waitpid` needs `WEXITSTATUS`,
    and the design says it is reachable; the FFI is to be complete (CL-028).

    **Widened 2026-10-02** by panel 185's ffi-pragmatist: glibc's `FD_ZERO`,
    a statement macro (`do { ... } while (0)`), takes the same false *declares
    no* (`scratchpad/185-ffi-pragmatist/p185/`); on macOS `WEXITSTATUS`,
    `htonl`, `FD_ISSET` and `WIFEXITED` are macro-only names alike. Panel 185
    sat on the route (its Q1); the seats measured that a call-form probe holds
    none of a macro's parameters, a `u8` declaration reading out of bounds.

    **2026-10-02, lane ffi-macro, panel 185 R1, a macro-only name is
    `ffi_macro_name`, its note drafting a function of the program's own with
    a placeholder for every C type**: repaired at `357589d6`, gated by its
    cases and the compiler's own tests; the net is owed at the batch's close,
    and so are Linux x86-64 (the gate's container), Linux arm64 and the
    Windows box, R1 being at the C boundary.

    **2026-10-03, its cases read one by one, held for the Windows box**: on this Mac (00:30 to 00:42 by `date`) and in the Linux arm64 container (one run, 00:36 to 00:54, Debian clang 22.1.8), with a compiler built from the trunk's seed at `2620bed1`, all twelve passed, `run/fixedbugs-143-*` 2 of 2 and `unsupported/fixedbugs-143-*` 10 of 10. Four of them bind `sys/wait.h` or `sys/select.h` (`run/fixedbugs-143-system-macros-through-functions-of-the-programs-own`, `unsupported/fixedbugs-143-a-macro-declared-at-a-wrong-width-drafts-no-width`, `-a-system-macro-is-named-as-a-macro` and `-the-fd-set-macros`), which the Windows box lacks (this list's item 158, whose origin is the first of them red there at `2bb45a96`): there they are told `ffi_missing_header` and skipped, and the leg of `6bec7c8c` read `run` 234 and `unsupported` 113 at 0 failed, short of the Mac's 242 and 119 by exactly the cases that can skip there, these four among them (deduced from the totals; the box did not answer on 2026-10-03). A case skipped on a platform does not close the item: it waits for those four to run on Windows, which needs headers the platform does not have, or for a ruling that a case on a POSIX header is judged where the header exists.

    **2026-10-03, lane ffimsg**: cases at `391c04d4` on headers of the tree's own for the three shapes only the four system cases witnessed, a statement macro told as a macro and reached through a function of the program's own behind a handle, and a macro declared at a wrong width drafted with no width (`tests/golden/run/fixedbugs-143-statement-macros-through-functions-of-the-programs-own.hero`, `tests/golden/unsupported/fixedbugs-143-statement-macros-of-the-programs-own-header.hero`, `tests/golden/unsupported/fixedbugs-143-a-macro-of-the-programs-own-header-at-a-wrong-width.hero`), gated by their own cases on this Mac; the rest is owed at the round's gate. The item closes when they run on the Windows box.

    **2026-10-03, read one by one at `02e507bc` with the Windows box answering, and the author's answer *A***: all twelve passed on this Mac and in the Linux arm64 container, under clang 22.1.8 and again under 18.1.8; on the Windows box (clang 23.1.1) eight passed and the four named above were skipped, each built alone there reading `ffi_missing_header` on `sys/wait.h` or `sys/select.h`, which the paragraph held for the Windows box had deduced from the totals. The author's answer *A* (`docs/records/log/2026-10-03-1123-the-author-answers-a-a-case-a-platform-cannot-run-is-judged-where-its-header-is.md`): those four are judged where their headers are, and the item closes once its twin, lane ffimsg's in-tree macro case, has run on all three.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): a false message
    (*declares no*) on a header that defines the name; repaired and gated,
    closes after the push's platform legs.

- [ ] **156 — a C struct with a bit-field member stops `build` with an internal error and clang's text** | `extern "bf.h"` over `typedef struct { int32_t kind; uint32_t flag : 1; uint32_t rest : 31; } BF;` with `record BF` naming `kind: i32`, `flag: u32`, `rest: u32`, reading `make_bf().kind`: `check` exit 0, `build` exit 2, *internal error: compiling the generated C failed: ... invalid application of 'sizeof' to bit-field* at the field assertions and *address of bit-field requested* in the generated hash | `selfhost/emit/` (the field assertion, `heroes-ffi-field`, and the record's descriptor) · panel 073's item 3 (bit-fields filtered before the assertion) · **class: blocking**

    **Origin:** panel 186's completeness critic, 2026-10-02, in its first
    pass over the briefs (`docs/panel/186-briefs/probes/critic/bf.h`, `bf.hero`);
    reproduced by the coordinator at 11:02 on `ae08ed93`.

    **Why it is a defect.** Exit 2 is the compiler blaming itself for a
    binding the author can be told about, and clang's text reaches the
    author (`.claude/rules/c-boundary.md`). Panel 073 resolved that a
    bit-field is filtered before its assertion; what such a field binds to,
    if anything, is panel 186's question beside defect 151.

    **2026-10-02, lane land186, a bit-field refused on its own line at
    build, and one left out sent to `partial`** (panel 186 R5): repaired at
    `afd07f00`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close, and the platform legs before the push.

    **2026-10-02, lane land186, spec § 13's *A bit-field is none of these:
    leave it to `partial`.*** (panel 186 R6): at `b7c510c5`, gated by its
    own cases; the rest is owed at the round's gate.

    **2026-10-03, its cases read one by one, held for the Windows box**: all eight, `run/fixedbugs-156-records-beside-bit-fields-build` and `unsupported/fixedbugs-156-*` 7 of 7, passed on this Mac (00:30 to 00:42 by `date`) and in the Linux arm64 container (one run, 00:36 to 00:54, `libcurl` 8.14.1 answering there), with `docs/panel/186-briefs/probes/critic/bf.hero` told `ffi_field_type` on each bit-field at exit 1 on both. On the Windows box `unsupported/fixedbugs-156-curl-s-hsts-entry` binds `curl/curl.h`: whether it ran there is unread, the box not answering on 2026-10-03, and the leg of `6bec7c8c` (`unsupported` 113 at 0 failed against 119) leaves exactly one skip between it and defect 158's fourteen cases, if the box has neither `pkg-config` nor raylib as its install list says. The other seven name only their own header over `stdint.h` and passed there, by the same totals. The item waits for the box to say whether the curl case ran.

    **2026-10-03, read one by one at `02e507bc` with the Windows box answering, and the author's answer *A***: all eight passed on this Mac and in the Linux arm64 container, under clang 22.1.8 and again under 18.1.8; on the Windows box seven passed and `unsupported/fixedbugs-156-curl-s-hsts-entry` was skipped, built alone there reading `ffi_missing_header` on `curl/curl.h`; `bf.hero` was told `ffi_field_type` on each bit-field at exit 1 on all three. The author's answer *A* (`docs/records/log/2026-10-03-1123-the-author-answers-a-a-case-a-platform-cannot-run-is-judged-where-its-header-is.md`): the curl case is judged where curl is, and the item gains a twin, a case on a header beside the program carrying curl's bit-fields, closing once that twin has run on all three.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): exit 2 and clang's
    text for a binding the author can be told about.

- [ ] **158 — a group's header that includes a header this machine lacks stops `build` with an internal error and clang's text** | `extern "outer.h"` over a header holding `#include <no_such_header_here.h>`: `check` exit 0, `build` exit 2, *internal error: compiling the generated C failed: In file included from ...: ./outer.h:2:10: fatal error: 'no_such_header_here.h' file not found*, where a missing header the group names itself is told `ffi_missing_header` at exit 1 | `selfhost/emit/ffi_build.hero` (where `ffi_missing_header` is told) · `tests/golden/run/fixedbugs-143-system-macros-through-functions-of-the-programs-own.hero`, red on the Windows box · **class: blocking**

    **Origin:** the coordinator, 2026-10-02, reading the Windows
    box's pre-push leg on `2bb45a96` (16 of 19 suites green; `run`,
    `emission` and `determinism` each 1 failed, all on lane ffi-macro's new
    run case, whose header includes `sys/wait.h` and `sys/select.h`, absent
    on Windows; the leg's log reads its exit at 15:52), then reproduced on
    this Mac before 15:55, the filing commit's time, on the trunk at
    `4d0f27a1` (`docs/panel/186-briefs/probes/coordinator/nested.hero` and
    `outer.h`).

    **Why it is a defect.** Exit 2 is the compiler blaming itself for the
    machine's fact (`.claude/rules/c-boundary.md`), and clang's text reaches
    the author; the run suite skips a case only on `ffi_missing_header`
    (`tests/harness/shell.hero`'s `machine_lacks_the_library`), so the same
    fact also turns a platform's correct skip into a red that would reach
    the CI's Windows leg at the next push.

    **2026-10-02, lane h158, a header the group's own header includes is
    told on the group from clang's line alone, and one reached through other
    headers from the include stack above it**: repaired at `744cdc08` (the
    line) and `7790f2f6` (the stack, the call in `selfhost/emit/ffi.hero`
    handing clang's whole stderr), gated by their cases, the first also by the
    compiler's own tests; the net is owed at the batch's close.

    **2026-10-03, its cases read one by one, held for the Windows box**: all fourteen `unsupported/fixedbugs-158-*` passed on this Mac (00:30 to 00:42 by `date`) and in the Linux arm64 container (one run, 00:36 to 00:54), and `docs/panel/186-briefs/probes/coordinator/nested.hero` is told `ffi_missing_header` on `outer.h`'s line 2 at exit 1 on both, with the ten tests of `744cdc08` and `7790f2f6` `ok` there and in the four jobs of the CI's run 37065944766. Every one of the fourteen EXPECTS `ffi_missing_header`, so on a machine where one's text differs from its `.expected` the harness skips it rather than failing it (`tests/harness/suite_golden.hero:210`), and the Windows box is the machine this defect was found on: its leg of `6bec7c8c` (`unsupported` 113 at 0 failed against 119) leaves exactly one skip between these fourteen and defect 156's curl case, and the box did not answer on 2026-10-03 to say which. The item waits for the box to read the fourteen one by one.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): exit 2 and clang's
    text for the machine's own fact; it would turn the CI's Windows leg red.

- [ ] **167 — the pointee check reads a header without the build's own `-O2` or `--sanitize`, so a parameter the header types otherwise under them is accepted and the program writes past its `i32`** | `opt.h` declares `fill(int32_t *p)`, and `fill(int64_t *p)` under `#ifdef __OPTIMIZE__`, bound `function fill(@p: i32)`, `a: i32 @ 0`, `fill(@a)`, `print(a)`: `build -O0` prints `7`; `build -O2` exit 0, prints `0`; `build -O2 --sanitize` stops in *AddressSanitizer: stack-buffer-overflow ... WRITE of size 8* on the 4-byte local, exit 134; a header keyed on `__has_feature(address_sanitizer)` does the same under `--sanitize` alone | `selfhost/cli/pointee.hero` (its dump and its check, about `:153-174` and `:197-222` at `6bec7c8c`, `probe_flags()` at `:279-286`), run without the level and the sanitizer `selfhost/cli/units.hero` and `selfhost/cli/flags.hero:201` give the program's own compile · defect 163's class · **class: blocking**

    **Origin:** lane h158 at defect 163, 2026-10-02, a question left unmeasured (*no probe compiles under them*); measured by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/optimize-macro/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`), defect 163's repair `0235b942` included. A result type is held by the program's own compile (a header that changes a return type under `-O2` is refused, `ffi_return_type`), so only the pointee check diverges, as in 163. Unmeasured: whether a real header changes a pointee's type under these macros, and whether the layout check's probe, beside it, is judged without them too.

    **2026-10-03, lane cb4, every probe of the program's headers compiled
    under the words its units compile with, the level and the sanitizers
    included** (the shapes beside it with its cause, S7 to S10 of the lane's
    first pass, 2026-10-02, `scratchpad/lane-cb4/first-pass.md`: the layout
    check under `-O2` and under `--sanitize`, which answers the origin's
    question, a refused round's header asks under `-O2`, `--emit-c -O2` and
    `heroes test --sanitize`): repaired at `e28f4fc7`, gated by its own
    cases; the rest is owed at the round's gate, and the platform legs before
    the push.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a wrong value and a memory fault.

- [ ] **168 — the layout check's cache key does not name the header it read, so a program beside a same-named header replays another directory's verdict: a binding its own header refutes builds and prints a wrong value** | `a/x.h` holds `typedef struct { int32_t x; } E;` and `b/x.h` `typedef struct { int32_t x; int32_t y; } E;`, both bound `record E` naming `x` alone, built from one working directory: alone, `b/p.hero` is refused, `ffi_incomplete_record`; after `a/p.hero` in the same cache it builds at exit 0, and with `gety(e: E) -> i32` reading C's `e.y` its run prints `true`, `2`, `3`, `p == q` true for two values whose `y` C reads as 2 and 3; in the other order the correct `a/p.hero` stops at exit 2, *internal error: checking the header's layout of the group records failed ... no member named 'y' in 'E'* | `selfhost/cli/layout.hero:97-99` (the key: the screen unit's text, `#include <x.h>` and one function per record, no source path) · `selfhost/emit/layout_screen.hero:35`, `:51-59` · the pointee check's key names the declaring file (`selfhost/cli/pointee.hero:111-113`) and does not replay · **class: blocking**

    **Origin:** lane round1002c's gate, 2026-10-02, a question left unrun (*neither the pointee key nor the layout key names `source_dir`*); measured by the coordinator's agent on `6bec7c8c` (2026-10-02, `scratchpad/file-queue/source-dir-key/`), every experiment from one working directory, each with a fresh-cache control. The width shape (`int32_t v` against `int64_t v`) replays too, and the program's own compile still refuses it (`ffi_field_type`): a member left out has the layout check as its only judge. Unmeasured beside it: the standard library's own pointee asks key alike in both directories, and a header found through `CPATH` or another environment variable is named by no key.

    **2026-10-02, lane cb4, a probe's kept verdict keyed by every word its
    lines compile under and by what its headers resolve to, a header that
    appears earlier in the search and the library's own asks included** (the
    two shapes beside it with its cause, S4 and S6 of the lane's first pass,
    2026-10-02, `scratchpad/lane-cb4/first-pass.md`): repaired at `cf949d33`,
    gated by its own cases; the rest is owed at the round's gate, and the
    platform legs before the push.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a wrong value at exit 0, and an exit 2 on a correct program.

- [ ] **169 — `check` aborts on a correct program of 300 additions** | `total = 1 + 2 + ... + 300` over `print(total)`: `check` exit 134, *panic: stack exhausted in checkwalk.synth* | `selfhost/check/walk.hero:101` (`synth`) · panel 184's R5 (*the compiler runs its passes on a thread whose stack it chooses*) and R6 (*a floor, not a ceiling*), ratified 2026-10-01, not landed (`docs/panel/184-a-brace-is-written-both-ways-a-statement-after-a-jump-is-refused-and-depth-is-the-compilers-to-hold.md:197`, `:204-206`) · **class: blocking**

    **Origin:** panel 184's blind task 3 (`docs/panel/184-briefs/blind/task3a.hero`), aborting in every `check` census since (lane round1002b's; lane round1002c's, *both arms abort on the same file*); reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/check-abort-task3a/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). The sitting's prediction is that *under R5 every shape of the shared brief's table checks at 2,000 on this Mac* (`:238-239`), and R6's N is to be measured before its sentence is written (`:270-271`). No list held the landing until this item.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a crash, and a correct program refused.

- [ ] **170 — a correct program whose types nest deeper than the C compiler survives stops `build` at exit 2 with clang's crash text** | 3,000 variants nested by value, `variant R<i>` whose case `a` holds `R<i-1>`, `xs: [R2999] = []`, `print(xs == xs)`: `check` exit 0, `build` exit 2, *internal error: compiling the generated C failed: clang: error: unable to execute command: Illegal instruction: 4* (Apple clang 21.0.0); the CI's Apple clang died at 1,000 (defect 155) | the build's clang call (`selfhost/cli/units.hero:96`, `tu_object`) · defect 140's closed record (*facts about the C compiler, not refusals of this one*) · panel 184's R6 · **class: blocking**

    **Origin:** lane ci140's report, 2026-10-02 (*a C compiler's own depth limit reaching the author as an internal error*); defect 155 holds only the CI's red; reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/clang-depth/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). Each crash leaves a 7.7 MB preprocessed file and a script in the system's temporary folder and a report under `~/Library/Logs/DiagnosticReports/`. Defect 140's record says a refusal at a depth would be a new diagnostic class, and panel 184's R6 says no source is refused for its depth, so the route may be a sitting's.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told.

- [ ] **171 — a C function-pointer object declared as an `extern` `function` builds, and called while null it panics naming a callback the program never passed** | `own.h`'s `static int32_t (*hook)(int32_t) = 0;` bound `function hook(x: i32) -> i32`, `print(hook(x: 1))`: `build` exit 0; the run prints *panic: a null function pointer was called — a `ptr` holding `nullptr` reached C where C calls it back*, exit 134 | `runtime/parts/stack.c:515` (and `:738`) · the `extern` member's kind check (defect 152's `2d5240a0`, which lets a function-pointer object through) · **class: blocking**

    **Origin:** lane ffi-macro's first pass for defect 152, 2026-10-02 (`scratchpad/lane-ffi-macro/p1/obj/e23-fnptr-object-called.hero`, 2026-10-02), queued as *a message question and a declaration question, likely a sitting's (panel 038's refusal for `constant`)*; reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/null-fnptr-object/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). No memory is corrupted: the guard holds.

    **2026-10-03, lane ffimsg**: repaired at `428fc2a6`, gated by its own cases; the rest is owed at the round's gate, and Linux and the Windows box before the push, the repair being under `runtime/`. The message is made true and the declaration is not refused: measured, a refusal is possible only in a unit of its own (in the program's own, `__typeof__` is a hard error on an `overloadable` function), it cannot see a macro over `(*p)`, and it would refuse a loader's shape, volk's and GLAD's function pointers called as functions, a correct program that builds and runs (`tests/golden/run/fixedbugs-171-a-loaders-pointers-set-then-called.hero`).

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a false message. Refusing the declaration or making the message true is the lane's question.

- [ ] **172 — in an `extern` group, `int` is refused with the `certain` fix `i32`, which `check` cannot know: where the header's type is 64 bits, the applied binding is refused anew by `build`** | `docs/panel/176-briefs/xfer_cj.hero`'s `-> i64`, over `cj.h`'s `int64_t cJSON_AddItemToObject(...)`, written `-> int`: `check` exit 1, `reserved_word`, *in an `extern` group a type is the header's own width and sign, and C's `int` is `i32`*, fix (certain) *replace `int` with `i32`*; `check --apply` writes `-> i32`, `check` exit 0, `build` exit 1, `ffi_return_type` | the `reserved_word` fix for `int` in an `extern` group (defect 135's L6, `c61a1d04`) · `.claude/rules/diagnostics-and-goldens.md` § Errors are a deliverable (*a fix that leaves the defect standing is a `guess`*) · **class: blocking**

    **Origin:** panel 187's completeness critic, 2026-10-02, on the recovery instrument's APPLY-OTHER 20 and APPLY-NEW 19 on lane recovery-b8's runs, 0 and 0 on lane recovery-b6's gate, every one operator `int`, and the same 20 and 19 on the trunk's compiler at `6bec7c8c`, the instrument's run of 2026-10-02 from 21:55 to 22:08 by `date` (`scratchpad/inst-187/round3/`, 2026-10-02); built by the coordinator on `62d65e48` (2026-10-02, `scratchpad/apply-int/case/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). In each of the instrument's cases the original said `i64`, so the header's type is 64 bits there. `check` does not read the header, so it cannot tell C's `int` copied from it (`i32` right) from `int` meaning an integer (`i64` right); `build` reads it, and the binding never runs with the wrong width.

    **Why it is a defect.** `check --apply` applies a `certain` fix without asking, and this one writes a binding that says something else than the header in every case the instrument planted.

    **2026-10-03, lane ffimsg**: repaired at `c53ce231`, gated by its own cases; the rest is owed at the round's gate, and so is the recovery instrument's `int` row re-read there (`scratchpad/inst-187/run.sh` over lane recovery-b6's `rGate` plan, APPLY-OTHER 20 and APPLY-NEW 19 before). In a group `int` is a guess now, `i64` where a result or a constant may be wider and `i32` elsewhere; outside one it keeps its certain `i64`.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a `certain` fix that writes a program meaning something else; `build` refuses that program, so no wrong value runs.

- [ ] **177 — a `match` whose arms fall inside a bracket left open has each arm told again after the bracket's own message** | `return match scores[name` over `.ok v  => v.to_str()` and `.err e => e.code`: `unclosed_bracket` at the `[`, then `line_end_before_continuation` at each arm, three messages for one missing `]`; `x = match (n` over `.ok v => 1` the same; `y = match n` below `x = [n, 1` gets `expected_end_of_line` at each arm's `=>` | the reach of a bracket left open (panel 183's R1 and R2) over a `match`'s arms · `selfhost/parse/line_end.hero:242` · **class: adjacent**

    **Origin:** lane 135c's report and lane recovery-b4's (*one extra message per arm*), queued under recovery-b5, 2026-10-02 (`scratchpad/lane-135c/shapes/P6/`, 2026-10-02); reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/open-bracket-arms/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). The certain fix lane recovery-b4 saw inside a `[` left open is a guess today, and `check --apply` leaves the text as it is.

    **2026-10-03, lane rec187, panel 187's R5: measured as filed on the lane's compiler at `97008231` (V1, V5 and R6 landed), its reproducers reading as on the head's; pinned by `tests/golden/check/panel-187-arms-inside-a-bracket-left-open-are-told-again.hero`, a known cost under the sitting's R1**, not repaired inline: where the reach of a bracket left open ends over a match's arms is panel 183's R1 and R2, ratified, which the lexer and `parse/unclosed` apply. The item stays open.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake; no wrong value, no false message, no certain fix. The recovery cluster's, beside 130 and 131.

- [ ] **178 — a separator habit on every line of a block is told once per line: a `,` after each statement or each arm, a `;` after each field** | `x = 1,`, `y = 2,`, `print(x + y),` in a function: three `expected_end_of_line`; arms `0 => 1,`, `1 => 2,`, `_ => 3,`: three; `record P` over `x: i64;`, `y: i64;`, `z: i64;`: three `unexpected_character`; a `,` after every member of a declaration is one message since `68e46a13` | `selfhost/parse/member_lines.hero` (the members' rule, `68e46a13`) · the line end of a statement and of an arm · the lexer's `;` · **class: adjacent**

    **Origin:** lane recovery-b4's report (*the per-line `,` and `;` habits, one message per run with a certain deletion*), queued under recovery-b5, 2026-10-02; reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/habit-every-line/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`).

    **2026-10-03, lane rec187, panel 187's R5: measured as filed on the lane's compiler at `97008231` (V1, V5 and R6 landed), its reproducers reading as on the head's; pinned by `tests/golden/check/panel-187-a-separator-habit-on-every-line-is-told-once-a-line.hero`, a known cost under the sitting's R1**, not repaired inline: one message for a run of lines is a reading of its own, as ruling 5's is for an indentation habit, and the `;` is the lexer's (`selfhost/scan.hero:299`). The item stays open.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake. The recovery cluster's.

- [ ] **179 — `@return 1` costs two `expected_expression`, one at the `@` and one at `return`** | `function f() -> i64` over `@return 1`: `check` exit 1, *expected an expression, found `@`* at 2:5 and *expected an expression, found `return`* at 2:6, neither naming the sigil | `selfhost/parse/at_prefix.hero` (a sigil before a name is one message since `4441148b`; before a keyword it is not) · **class: adjacent**

    **Origin:** lane recovery-b4's report (*`@return 1`: two messages*), queued under recovery-b5, 2026-10-02; reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/at-return/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`).

    **2026-10-03, lane rec187, panel 187's R5: measured as filed on the lane's compiler at `97008231` (V1, V5 and R6 landed), its reproducers reading as on the head's; pinned by `tests/golden/check/panel-187-a-sigil-before-return-is-told-twice.hero`, a known cost under the sitting's R1**, not repaired inline: the statement's start is `selfhost/grammar_expr.hero`'s, at its `DECIDED` ceiling of 1085 with no line of room, so a repair first moves code out of that knot. The item stays open.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake. The recovery cluster's.

- [ ] **180 — a range written `1..2` is told twice as a field access** | `x = 1..2`: `expected_field_name` at 3:11, *found `.`*, and again at 3:12, *found a number (`2`)*; `x = 0x1..5` the same at 3:13 and 3:14 | `selfhost/grammar_expr.hero:343` · **class: adjacent**

    **Origin:** lane arm's first pass for defect 154, 2026-10-02 (`scratchpad/lane-arm/pass1/n154/e20.hero`, 2026-10-02), queued as *a range habit, recovery's file*; reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/range-dots-twice/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). The recovery instrument counts its `range-dots` operator at 12 EXTRA.

    **2026-10-03, lane rec187, panel 187's R5: measured as filed on the lane's compiler at `97008231` (V1, V5 and R6 landed), its reproducers reading as on the head's; pinned by `tests/golden/check/panel-187-a-range-written-with-two-dots-is-told-twice.hero`, a known cost under the sitting's R1**, not repaired inline: its site is `selfhost/grammar_expr.hero`'s `after_dot`, at its `DECIDED` ceiling of 1085 with no line of room, and the range's right operand can be read only inside that knot, so no module of `parse/` can carry the repair; a `for i in 0..10` head costs the same two. The item stays open.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake. The recovery cluster's.

- [ ] **181 — a decimal number with a second point, `1.5.2`, is told as a field access** | `x = 1.5.2`: `expected_field_name` at 3:13, *expected a field or function name after `.`, found a number (`2`)* | `selfhost/grammar_expr.hero:343` · `selfhost/number.hero` · defect 154's closed record (*Left, queued*) · **class: adjacent**

    **Origin:** lane arm's first pass for defect 154, 2026-10-02 (`scratchpad/lane-arm/pass1/n154/e08.hero`, 2026-10-02), named *Left, queued* in `docs/records/done/2026-10-02-1415-defect-154-closed-a-based-literal-with-a-fraction-is-told-as-the-number-it-is.md` and on no open list; reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/float-second-point/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). Lane arm: no existing code covers it, so a new one is a sitting's.

    **2026-10-03, lane rec187, panel 187's R5: measured as filed on the lane's compiler at `97008231` (V1, V5 and R6 landed), its reproducers reading as on the head's; pinned by `tests/golden/check/panel-187-a-number-with-a-second-point-is-told-as-a-field.hero`, a known cost under the sitting's R1**, not repaired inline: its site is `selfhost/grammar_expr.hero`'s `after_dot`, beside 180's, at its `DECIDED` ceiling with no line of room. The item stays open.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be. The recovery cluster's.

- [ ] **182 — an operator alone on a line deeper than a `match`'s arms costs `continuation_outside_brackets` and `unexpected_block`** | `k = match n` over `1 => "one"`, then a line holding only `+` (or `-`) one level deeper, then `_ => "many"`: two messages on line 5; the `certain` deletion of the operator, applied, checks clean | `selfhost/open_line.hero` · `selfhost/sign_above.hero` (the deletion, defect 165's `8cb4ba6c`) · the orphan block's `unexpected_block` · **class: adjacent**

    **Origin:** lane h158's pass for defects 165 and 166, 2026-10-02 (`scratchpad/lane-h158/d166/q1_plus_deeper.hero`, `q2_minus_deeper_wild.hero`, 2026-10-02); reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/operator-deeper-line/`), where the one fix was a guess, and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`), where it is the `certain` deletion 165's repair brought; the second message stands.

    **2026-10-03, lane rec187, panel 187's R5: measured as filed on the lane's compiler at `97008231` (V1, V5 and R6 landed), its reproducers reading as on the head's; pinned by `tests/golden/check/panel-187-an-operator-alone-deeper-than-the-arms-costs-two.hero`, a known cost under the sitting's R1**, not repaired inline: the deeper margin is laid out by the lexer before `selfhost/sign_above.hero` leaves the operator out of the stream, in `selfhost/open_line.hero`, two lines under its ceiling, and only the lexer's indent stack can take the block back. The item stays open.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake. The recovery cluster's.

- [ ] **183 — a `.pc` whose `prefix` holds an unescaped space is refused naming `space/include` as the flag, and not the package file's line that splits it** | `prefix=/opt/with space`, `Cflags: -I${prefix}/include`: `pkg-config --cflags s7` prints `-I/opt/with space/include`; `build` exit 1, `ffi_package`, *the package `s7` answered with `space/include`, which this compiler does not pass on* | `selfhost/cli/shell_split.hero` (the word splitter, defect 162's `b37bfce1`) before `filter_words` (`selfhost/cli/libraries.hero:83`) · **class: adjacent**

    **Origin:** lane h158 beside defect 162, 2026-10-02 (`scratchpad/lane-h158/d162/pc/s7.pc`, 2026-10-02, *true, could say more*); reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/pc-prefix-space/`, run with `PKG_CONFIG_PATH` naming that folder) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`), where only the note's list of accepted flags is worded otherwise.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

- [ ] **184 — `check` takes time quadratic in the `for` heads it refuses: 3,000 `for n > 0` lines cost 16 s** | a function of N loops `for n > 0` over `n @ n - 1`, `check --brief`: 0.44 s of user time at 500, 1.74 at 1,000, 3.87 at 1,500, 15.92 at 3,000, `real` within 0.16 s of `user`; 3,000 `while n > 0,` cost 0.15 s and 3,000 `n @ 0X1` 0.06 s | `selfhost/grammar_expr.hero:899` (`for_stmt`) · `selfhost/parse/loop_habit.hero` (`refuse`), the cause unrun · **class: adjacent**

    **Origin:** the coordinator's file-queue agent, 2026-10-02, measuring lane arm's item on field-place pushes, on `62d65e48` (2026-10-02, `scratchpad/file-queue/for-habit-quadratic/`), the machine at load 2 to 5; re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`), the times above being that run's, on a busy machine and in the same order as the first. Two candidates were measured out over the same 3,000: `loop_habit.hero:62`'s append made in place, and `for_stmt`'s trial parse removed.

    **Why it is a defect.** Defect 146's class: a file of N mistakes costs N² work.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): real, found beside the work, no wrong value and no crash.

- [ ] **185 — the emitted C's line restores after a fixed-array field's assertion name a line one too low per such field** | `heroes build tests/golden/run/ffi-a-char-field-becomes-text.hero --emit-c`: line 20 is `#line 19 "ffiacharfieldbecomestext.c"`, so line 21 is reported as 19, and every later restore is 2 short; over `tests/emission`, 261 of 36,781 restores in 19 files are 1 to 10 short, each file's shortfall equal to its number of two-assertion lines | `selfhost/emit/extern_field.hero:158`, `:161` (a `"\n             _Static_assert(` the printer does not count) · `.claude/rules/generated-c.md:27-29` · **class: adjacent**

    **Origin:** lane round1002b at its gate, 2026-10-02 (*not chased*); measured by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/line-restore/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`).

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a place less exact than it could be in every clang note, sanitizer frame or debugger line past such a field; no value moves.

- [ ] **186 — `ffi_return_type` says a function does not return the program's type and never names the type the header gives** | defect 172's applied program: *`cJSON_AddItemToObject` does not return `i32` — that is what `cj.h` says, and clang read it*, its note *correct the result type, or name the header that declares this*; neither `int64_t` nor `i64` appears | `selfhost/emit/ffi_declared.hero:60` · **class: adjacent**

    **Origin:** the coordinator, 2026-10-02, building defect 172's applied program on `62d65e48` (2026-10-02, `scratchpad/apply-int/case/`), and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). The pointee check's message beside it names the header's type and offers it (`ffi_parameter_type`, *the header's `int64_t *` points at a different width*, fix *declare `p` as `@p: i64`*).

    **2026-10-03, lane ffimsg**: repaired at `edfeb7d2` and `64b0d525`, gated by its own cases; the rest is owed at the round's gate. A refused round asks clang the type of each result and each constant (`selfhost/emit/ffi_asked.hero`), and `ffi_return_type` and, beside it, `ffi_constant_type` name it and offer the word that declares it, a guess; `fixedbugs-144-*` and `fixedbugs-145-a-result-spells-the-typedef` read anew.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be: it carries the header's name and not the type that would fix the program (design.md §4.17).

- [ ] **187 — four parser diagnostics are still appended through a field place, against defect 146's rule and `cursor.hero`'s own comment** | `git grep -n 'c.diagnostics @ c.diagnostics.push' -- selfhost/parse/` prints `loop_habit.hero:62`, `line_end.hero:251`, `type.hero:257`, `type.hero:320`, while `selfhost/cursor.hero:272` says *Every parser module appends through this* | the four lines · `cursor.push_diagnostic` (`selfhost/cursor.hero:273`) · **class: improvement**

    **Origin:** lane arm's report, 2026-10-02; read by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/field-place-push/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`), which finds three more modules appending the same way, their cost unmeasured. No cost is measured for any: at `loop_habit.hero:62` the append is not what makes defect 184 slow.

    **Class: improvement**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a cleaner form nobody needs to be right.

- [ ] **188 — two texts still state the arm rule panel 185's R3 replaced** | `selfhost/check/lending.hero:108-109` quotes the spec as *An arm that does nothing is a block holding `_ = 0`*, where the spec now reads *An arm that does nothing holds `_ = 0`*; `tests/golden/check/fixedbugs-135-a-discard-that-is-the-line-s-one-reading.hero:8` reasons *`_ = ` on the arm's own line is `declaration_in_arm`*, false since R3 | the two lines · **class: improvement**

    **Origin:** lane arm's report, 2026-10-02; read by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/stale-arm-texts/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). design.md §4.7 owes nothing: R3 brought the spec to it.

    **Class: improvement**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): no program moves.

- [ ] **189 — `typeorder.visit`'s walk without recursion has no witness** | the record chains the run cases emit are 1,000 deep (`run/fixedbugs-140-records-` and `-extern-records-a-thousand-deep-build`), and at 1,000 and 5,000 lane ci140's mutant, `visit` restored to its recursion, emits the trunk's bytes (666,776 and 3,358,806, `cmp` equal) | `selfhost/emit/typeorder.hero:130` and its one test at `:206` · **class: improvement**

    **Origin:** lane ci140's report, 2026-10-02 (`scratchpad/lane-ci140/mut/emit-typeorder/heroes-mut`, 2026-10-02, *the mutant that no case catches*); measured by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/typeorder-witness/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). At 20,000 deep the mutant reached clang, which died (defect 170), so a unit test is the witness the lane names.

    **Class: improvement**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): coverage.

- [ ] **190 — `records/lists` reads an item's class token and not the rest of its first line, so an item that lost its title and its *where* field reads as whole** | item 130's first line as `8349d264` left it, with no `**` closing its title and no ` | ` at all, before its class token: `records` 24 passed, 0 failed, after that commit and after every one until the item was restored at `50644159` | `tests/harness/suite_records.hero` (`list_offences`; the class rule, about `:4448-4610`) · **class: improvement**

    **Origin:** the coordinator, 2026-10-02, on the damage panel 187's completeness critic found (`docs/records/log/2026-10-02-2141-item-130-cut-in-8349d264-and-restored-what-cut-it-is-unknown-the-commit-did-not-read-its-diff.md`). It would have caught the cut in the item's line, not the one in its body.

    **Class: improvement**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): hardening of an instrument; nothing a program does moves.

- [ ] **191 — a record named after a typedef of a struct the header keeps opaque, with fields, stops `build` at exit 2 under clang 18, whose refusal of a member read through it does not name the typedef** | `tests/golden/unsupported/fixedbugs-145-a-typedef-of-an-opaque-struct-bound-with-fields`, `ffi_unknown_tag` expected: on the public CI's Linux x86-64 leg (Ubuntu clang 18.1.3), `internal error: compiling the generated C failed`, clang's *incomplete definition of type 'struct opaque_s'* three times at the field assertions, exit 2; Apple clang 21 on this Mac words it *'opaque_t' (aka 'struct opaque_s')* and the case passes | `selfhost/emit/ffi_incomplete.hero` (`incomplete_typedef`, which finds the record by the typedef's name in clang's words) · `f2a08f13` (panel 186's layout route, which removed the unit's positional completeness probe) · **class: blocking**

    **Origin:** the public CI on `07ccb72a`, run 37065944766, read by the coordinator (2026-10-02, `scratchpad/ci-x86-07ccb72a.log`, lines 1726 to 1763); reproduced by the coordinator under clang 18 in the arm64 container (Debian clang 18.1.8): `unsupported fixedbugs-145` 8 passed and 1 failed on `07ccb72a` and on `b48d02b8`, 9 and 0 on `8b98bcc7` (the last green CI) and on `415c0a14`, so the round merged at `b48d02b8` brought it; this Mac's clang, Debian clang 22.1.8 and Windows' clang 23.1.1 pass it. Reproduced by lane cb4 on `c2b3f3a1` under Debian clang 18.1.8, the same 8 and 1, and measured on both clangs beside it (2026-10-03, `scratchpad/lane-cb4/d191/words/`): the member read is the one refusal worded apart, a variable of the type, `sizeof` of it and a typedef of `void` are worded alike on both, and every position is the same. The positional probe `f2a08f13` removed was a variable of the record's type, which is why the case passed under clang 18 before it.

    **2026-10-03, lane cb4, the layout check asks a record named after a
    typedef, by where clang errs and never by what it says, whether its name
    is a type and whether that type has a layout, and refuses it before any
    unit compiles, the type behind its name read from clang's JSON**:
    repaired at `7c05e64d`, gated by its own cases under Debian clang 18.1.8
    and this Mac's clang; the rest is owed at the round's gate, and the
    platform legs before the push.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told, and a red CI.

- [ ] **192 — the compiler built from its seed on Windows, by the seed's own documented command, gets three clang warnings: `getenv` deprecated by the C library's headers** | `clang -I runtime seed/heroes.c runtime/runtime.c -Wl,/STACK:67108864 -o heroes` (`seed/README.md`'s Windows line) on the Windows box: *'getenv' is deprecated: This function or variable may be unsafe. Consider using _dupenv_s instead*, at `seed/heroes.c:50` and, through `#line`, `selfhost/cli/process.hero:64` and `:209`, then *3 warnings generated*; `heroes build` passes `-D_CRT_SECURE_NO_WARNINGS` to every unit it compiles (`selfhost/cli/flags.hero:96`) and `runtime/runtime.c:75` defines it for its own unit, so only a unit compiled outside `heroes build` gets the warnings | `runtime/heroes_runtime.h`, the first include of every emitted unit (`seed/heroes.c:2`) · `seed/README.md` · **class: blocking**

    **Origin:** the coordinator, 2026-10-02, reading the Windows leg's log on `6bec7c8c` (`scratchpad/platforms/win-6bec7c8c.log`, 2026-10-02); the same three warnings stand in every Windows leg's log read that day (`b48d02b8`, `2bb45a96`, `8b98bcc7`, `e252fda4`), and `docs/ref/environment/windows/WINDOWS-MACHINE.md:493-497` records them since 2026-09-21 as warnings *whether they are new is unrun*, never filed.

    **Why it is a defect.** The emitted C is C11 that clang type-checks clean (CLAUDE.md § 7), and this unit is the compiler itself built by its own first command; any `--emit-c` output compiled by hand on Windows gets the same advice. The runtime already says why the switch is the documented one and not a workaround (`runtime/runtime.c:65-74`).

    **2026-10-03, lane warn, the two macros `heroes build` passes for
    Windows are `runtime/heroes_runtime.h`'s own, defined on `_WIN32` before
    its first system include, each under a guard; `_USE_MATH_DEFINES` beside
    `_CRT_SECURE_NO_WARNINGS`, the same shape (`M_PI`, which
    `tests/golden/run/ffi-constant.hero` binds)**: repaired at `dc23c4a8`,
    gated by its own case, a compiler test preprocessing for
    `x86_64-pc-windows-msvc`, on this Mac and on Linux arm64 under Debian
    clang 22.1.8 and 18.1.8; the rest is owed at the round's gate, and the
    Windows box before the push, where the seed's documented line is the
    proof, unrun (the box offline on 2026-10-03). On this Mac a stub
    `stdlib.h` deprecating `getenv` as the box's UCRT does warned under
    `02e507bc`'s header and not under this one.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a clang warning on a correct program.

- [ ] **193 — an arm whose pattern failed hides the mistakes in its body: on one line in both arms, and, joined below the failed line, under `--permissive` since panel 187's V1** | `k = match n` over `x | 2 => f(1 +)`: `expected_pattern` at the `x` and the `1 +` untold, in both arms, on the head's compiler and on `29425af6`; `x |` over `2 => f(1 +)`: the normal arm tells the `1 +` from the lines apart, and `check --permissive` told it, `expected_expression`, until `29425af6` and not since; the same over `1 | +`, `x ==`, `1 -> 2 |` and an arm one level deeper | `selfhost/grammar_expr.hero` (`arms_of`'s `.err` branch: the failed arm's line goes with `cursor.drop_rest_of_line`, its body with it) · panel 187's R4 · **class: adjacent**

    **Origin:** panel 187's compiler engineer, 2026-10-02, on its V1 probes `h01`, `h05`, `h14`, `h16`, `h17` and their one-line twins `h02`, `h06` (`scratchpad/187-compiler-engineer-work/probes2/`, 2026-10-02); filed by lane rec187 at the sitting's R4, reproduced on the head's compiler and on `29425af6` (2026-10-03, `scratchpad/lane-rec187/pass1/k-166-control.txt` and `v1-vs-k.txt`). No golden form runs `--permissive` (the sitting's R3), so the control arm's half cannot be pinned; the one-line half can.

    **Why it is a defect.** A mistake told only once another is fixed: design.md §4.17's measure counts an exchange more.

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a mistake told only once another is fixed; no false message, no wrong certain fix.

- [ ] **194 — C's three-clause loop header costs three messages: the lexer tells each `;` and the loop habit the `for (`** | `for (i = 0; i < 3; i++)` over its body: `unexpected_character` at each `;` and `for_missing_in` at the `(`, three messages for one habit; `for (;;)` the same; a `{` after the header adds `missing_body`, ruling 4's | `selfhost/scan.hero:299` (the lexer's `;`) · `selfhost/parse/loop_habit.hero:58` (`for_missing_in`) · pinned by `tests/golden/check/panel-187-a-c-style-for-header-is-told-by-the-lexer-and-by-the-loop.hero` · **class: adjacent**

    **Origin:** panel 187's compiler engineer, its § 1's cause A1 (`docs/panel/187-reports/compiler-engineer.md`, 2026-10-02), on the audit's rows 131-33a, 131-54a and 131-54b and the recovery instrument's `c-for`, 52 of its 439 EXTRA; filed by lane rec187 under the sitting's R1 with its pin, which reads byte for byte the same on the head's compiler and on `29425af6` (2026-10-03).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery, panel 187's R1): a second message for one mistake, class (a).

- [ ] **195 — a line a recovery hands on to the statement reader is told a second time, by the expression's `primary`** | an arm's `0` over a `=> 5` two dedents out: `expected_arm_arrow` at the `0` and `expected_expression` at the `=>`; `use geom` over an indented `function g()`: `expected_declaration` at the block and `expected_expression` at the `function` | `selfhost/grammar_expr.hero:429` (`primary`'s message), after `selfhost/parse/broken_arm.hero:55` or `selfhost/parse/top_level.hero:82` · pinned by `tests/golden/check/panel-187-a-line-a-recovery-hands-on-is-told-again-by-the-expression.hero` · **class: adjacent**

    **Origin:** panel 187's compiler engineer, its § 1's cause A2 (`docs/panel/187-reports/compiler-engineer.md`, 2026-10-02), on the audit's rows 131-55a and 131-55b and lane recovery-b8's shape `g1/a11`; filed by lane rec187 under the sitting's R1 with its pin, which reads byte for byte the same on the head's compiler and on `29425af6` (2026-10-03).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery, panel 187's R1): a second message for one mistake, class (a).

- [ ] **196 — the colon habit is told once a line, and the next head's `:` on the same line is named a missing body** | `if n > 0: if n > 1: print(1)`: `trailing_colon` at the first `:`, whose `certain` fix rewrites the whole line and checks clean, then `missing_body` at the second, *found `:`*; three heads on one line cost the same two | `selfhost/parse/colon_habit.hero:98` · `selfhost/parse/opening.hero:274` (`absent`) · pinned by `tests/golden/check/panel-187-the-colon-habit-is-told-once-a-line.hero` and its `.fixed` · **class: adjacent**

    **Origin:** panel 187's compiler engineer, its § 1's cause A3 (`docs/panel/187-reports/compiler-engineer.md`, 2026-10-02), on the audit's rows 131-56a and 131-56b; filed by lane rec187 under the sitting's R1 with its pin, which reads byte for byte the same on the head's compiler and on `29425af6` (2026-10-03).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery, panel 187's R1): a second message for one mistake, class (a).

- [ ] **197 — one `)` left out of a function type costs four messages: the lexer names two openers, and the type reader asks for the `)` and the `->` it was owed** | `function f(g: (function(i64 -> i64)` over its body: `unclosed_bracket` at each `(`, *never closed* at the file's end and *still open at line N* where a declaration below ends the reach, then `expected_function_type_params_close` at the `->` and `expected_function_type_arrow` at the line's end | `selfhost/closers.hero` (`never_closed` at `:151`, `still_open` at `:169`) · `selfhost/parse/type.hero:307` and `:229` · pinned by `tests/golden/check/panel-187-a-closer-the-lexer-pairs-with-another-opener.hero` · **class: adjacent**

    **Origin:** panel 187's compiler engineer, its § 1's cause A4 (`docs/panel/187-reports/compiler-engineer.md`, 2026-10-02), on the audit's row 131-41a; filed by lane rec187 under the sitting's R1 with its pin, which reads byte for byte the same on the head's compiler and on `29425af6` (2026-10-03). The row's own file ends below the head, so it reads *never closed* (`scratchpad/audit-130-133/cases/131-41a/`, 2026-09-30, re-run by the lane on `29425af6`); the pin has a declaration below, so *still open*.

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery, panel 187's R1): a second message for one mistake, class (a).

- [ ] **198 — a head whose line goes on past it is told by one message and the rest of its line dropped, so a stray closer on it is told only once the first is repaired** | `variant T )` and `record Point )` with nothing below: `empty_variant` and `empty_record`, the `)` untold; `function f(): i64 )` over its body: `expected_end_of_line` at the `:`, the `)` untold until `->` replaces the `:`; with no body, `missing_body` (found `:`) and the `)` untold | `selfhost/parse/members_below.hero:87` (`skip_line` after a record's or a variant's head) · `selfhost/parse/opening.hero:192` (`drop_rest_of_line` after a function's) · pinned by `tests/golden/check/panel-187-a-heads-line-that-goes-on-is-told-once.hero` · **class: adjacent**

    **Origin:** panel 187's compiler engineer, its § 1's cause B1 (`docs/panel/187-reports/compiler-engineer.md`, 2026-10-02), on the audit's rows 131-16a, 131-16b, 131-53a and 131-53b; filed by lane rec187 under the sitting's R1 with its pin, which reads byte for byte the same on the head's compiler and on `29425af6` (2026-10-03).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery, panel 187's R1): a mistake told only once another is fixed, class (b).

- [ ] **199 — a head that refused something drops the rest of its line as debris, a stray closer in it with it** | `if f(1 +) )` over its body: `expected_expression` at the call's `)`, and the stray `)` after it told only once the operand is written | `selfhost/parse/opening.hero:147` (`drop_rest_of_line` after a failed head) · pinned by `tests/golden/check/panel-187-a-failed-heads-rest-is-dropped-as-debris.hero` · **class: adjacent**

    **Origin:** panel 187's compiler engineer, its § 1's cause B2 (`docs/panel/187-reports/compiler-engineer.md`, 2026-10-02), on lane recovery-b8's shape `g4/ti`, one of the six beside item 130; filed by lane rec187 under the sitting's R1 with its pin, which reads byte for byte the same on the head's compiler and on `29425af6` (2026-10-03).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery, panel 187's R1): a mistake told only once another is fixed, class (b).

- [ ] **200 — a `function` among a variant's cases is told once and dropped with its block, so a mistake inside it waits until it moves out** | `variant V` over `red` and `function f()` over `print(1 +)`: `expected_case` at the `function`, and the `1 +` untold; among a record's fields the same function is read as the function it is since `26358f9c` | `selfhost/parse/member_lines.hero:205` (`skip_line`) and `:202` (`balanced_block`) · pinned by `tests/golden/check/panel-187-a-function-among-a-variants-cases-is-dropped.hero` · **class: adjacent**

    **Origin:** panel 187's compiler engineer, its § 1's cause B3 (`docs/panel/187-reports/compiler-engineer.md`, 2026-10-02), on lane recovery-b8's shape `g2/r12`, one of the six beside item 130; filed by lane rec187 under the sitting's R1 with its pin, which reads byte for byte the same on the head's compiler and on `29425af6` (2026-10-03).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery, panel 187's R1): a mistake told only once another is fixed, class (b).

- [ ] **201 — in a braced body, the rest of the line past an inner closing brace is not read** | `function main() {` over `do {`, a body and `} while (1 == 1)`: `missing_body` at the `{`, `expected_end_of_line` at the `do {` and in the body, and `while (1 == 1)` untold until the braces are gone | `selfhost/parse/braced_lines.hero:154` (`brace_habit.pass`, which passes the function's braces after its lines are read, the inner `}`'s line with them) · pinned by `tests/golden/check/panel-187-a-line-past-an-inner-closing-brace-is-dropped.hero` · **class: adjacent**

    **Origin:** panel 187's compiler engineer, its § 1's cause B4 (`docs/panel/187-reports/compiler-engineer.md`, 2026-10-02), on the audit's row 131-32, where `heroes lex --dump-tokens` shows the lexer hands the parser every token of that line; filed by lane rec187 under the sitting's R1 with its pin, which reads byte for byte the same on the head's compiler and on `29425af6` (2026-10-03).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery, panel 187's R1): a mistake told only once another is fixed, class (b).

- [ ] **202 — a body written at its head's margin is told twice at its first line, and once more for each line after** | `function main()` over `print(1)` at column 0: `missing_body` and `expected_declaration`, both at 2:1, for the one indentation left out; `function f(x: i64) -> i64` over `y = x + 1` and `return y` at column 0: a third, `expected_declaration` at the second line; the same under a `test` and a `constant` | `selfhost/parse/top_level.hero:82` (`expected_declaration`, at the line the missing body was just told at) · `selfhost/parse/opening.hero:274` (`absent`) · **class: adjacent**

    **Origin:** lane rec187's first pass beside panel 187's R5, 2026-10-03, on the head's compiler and on `eddb0a7c` (`scratchpad/lane-rec187/pass1/v5/v24*.hero` and `k-v24.txt`, 2026-10-03). No golden pins the two at one place (every `check` case's `.expected` read for a `missing_body` and an `expected_declaration` at one line and column), and the recovery instrument plants no body dedented to its head (its operators in `scratchpad/instrument/tool/ops.py`, read 2026-10-03), so no count has seen it.

    **Why it is a defect.** One mistake, the body's indentation, told twice at one place (design.md §4.17).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake, class (a); both messages true.

- [ ] **204 — inside a list whose `[` the lexer paired with a closer of another kind further down, a binding is read as an element and told without the `[`, and the stray closer waits for the `]`** | `x = [1, 2` over `print(x)` over `y = 3 )`: one message, `expected_separator` at the `=` two lines below the `[`, *found `=`*, naming no `[`; with the `]` written, the `)` is told, `expected_end_of_line`, on a second run | `selfhost/parse/unclosed.hero` (panel 183's R1, a binding below a `[` ends its reach only where the lexer named the `[` never closed) · `selfhost/closers.hero` (the closer of another kind paired with the `[`) · **class: adjacent**

    **Origin:** lane rec187's first pass beside defect 203, 2026-10-03, on the head's compiler and on the lane's (`scratchpad/lane-rec187/pass1/r6/r16_closer_two_below.hero` and `r16b_bracket_written.hero`, 2026-10-03). Another cause than 203's: the found token is no closer, and the reach rule is not asked.

    **Why it is a defect.** The `]` left out is told nowhere and the stray `)` only on a second run (design.md §4.17's measure).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a mistake told only once another is fixed, class (b).

- [ ] **205 — a closer of another kind where a map entry's `:` goes is told without the `{` or its closer** | `m = {1: 2` over `print(x) )`: `expected_map_entry_colon` at the `)`, *expected `:` between a map's key and its value, found `)`*, naming no `{` and no `}`; `{1: 2` over `y]` the same | `selfhost/grammar_expr.hero:567` (`map_literal`'s `line_end.expect_after` for the `:`) · defect 203's message, the separator's, which names them (`selfhost/parse/list_line.hero`, `another_kind`) · **class: adjacent**

    **Origin:** lane rec187's first pass beside defect 203, 2026-10-03, on the head's compiler and on the lane's (`scratchpad/lane-rec187/pass1/r6/r04_map_paren.hero` and `r12_map_bracket.hero`, 2026-10-03). The same reading as 203's at another site: the line is read as the map's next key, and the closer stands where its `:` goes.

    **Why it is a defect.** As 203: one reading served, the other's repair a run more (design.md §4.17's measure).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

- [ ] **206 — on Linux, a package whose `.pc` gives `-F <dir>` builds with clang's *argument unused during compilation* warning, the link very likely handed the compile's `-F`** | `docs/panel/186-briefs/probes/coordinator/fw/` built with `PKG_CONFIG_PATH` at its `pc/` in the Linux arm64 container (Debian clang 22.1.8): `clang: warning: argument unused during compilation: '-F.../fw/pc/../frameworks' [-Wunused-command-line-argument]`, then the program prints `7` at exit 0; this Mac prints no such line; on this Mac `clang --target=aarch64-linux-gnu -F/tmp/fwdir x.o -o x.bin` prints the same warning and the same flag with `-c` prints nothing | `selfhost/cli/libraries.hero` (the words a package gives the compile and the link) · `selfhost/cli/units.hero` (the link line) · defect 160's closed record · **class: blocking**

    **Origin:** the coordinator's closings agent, 2026-10-03, reading defect 160's cases one by one on Linux arm64 between 00:36 and 00:54 by `date` (`scratchpad/closings/table.txt`, 2026-10-03), and its target probe on this Mac after 01:03 (`scratchpad/closings/fwprobe/`, 2026-10-03). The queue's question *a link step handed compile words, unmeasured on Linux* (lane h158, 2026-10-02) is this, measured. Which of the build's clang calls prints it on Linux is not read: that the link carries `-F` is an inference from the target probe.

    **2026-10-03, lane warn, a link is handed only the words a link reads
    (`libraries.link_words`, `link.link_line`): a package's `-L`, `-l`,
    `-Wl,-rpath` and `-framework` pair, its `-F` only where the answer links a
    framework, and no include directory. Which call printed it, read first
    under a wrapper logging every clang call (Debian clang 22.1.8): the
    program's final link and no other; of every compile word at a Linux link,
    22.1.8 and 18.1.8 warn about `-F` alone**: repaired at `9b31cd64`, gated
    by its own cases on this Mac and on Linux arm64, where the fixture prints
    `7` with an empty stderr and its link carries no `-F`; the rest is owed
    at the round's gate.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a clang warning on a correct program.

- [ ] **207 — a correct program whose string holds `??` before `)`, `(`, `<`, `>`, `=`, `/`, `'`, `!` or `-` gets clang's *trigraph ignored* warning on the emitted C** | `print("what???)")`: `check` exit 0, `run` exit 0 and prints `what???)`, and clang prints `build/tu-<key>/tri.c:10:42: warning: trigraph ignored [-Wtrigraphs]`; the net's own tests' build prints it twice for the harness's own `print(???)` | the emitter's string literals (a `?` that would begin a trigraph is written `?\?` in C) · **class: blocking**

    **Origin:** lane round1003b's gate, 2026-10-03, seeing it in the net's own tests' build and in the last gate's (`scratchpad/lane-round1003b/progress.md`, 2026-10-03); measured by the coordinator on the trunk at `e5893696` before 08:00 by `date` (`scratchpad/file-r5/tri.hero`, 2026-10-03).

    **2026-10-03, lane warn, every text the emitter writes into C is spelled
    for where C reads it (`selfhost/emit/c_text.hero`): a literal or a
    `#line` name writes a `?` that would end a trigraph as `\?`, a header
    name is split by a line splice, and the comment opening a unit neither
    closes nor opens one; the shapes beside it with its cause included, the
    four `#line` writers that escaped nothing (a directory `a\q` or `a"b`), a
    line end in a path and a directory `a*` (both exit 2), a header named
    `h??).h`**: repaired at `ed776fb2`, gated by its own cases on this Mac;
    the rest is owed at the round's gate, and the platform legs before the
    push. On Linux arm64 since (Debian clang 22.1.8, a copy of the tree with
    206's repair beside it), the case prints its `.expected` with an empty
    stderr and the compiler tests of the modules it touched read 31 and 0;
    the two spellings were measured there under 18.1.8 too; unrun on Windows.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a clang warning on a correct program.

- [ ] **208 — a dead `break` after a `return` inside `while true` is told twice, `unreachable_statement` and `missing_return`** | `while true` over `if m > 3`, `return m`, `break`, in a `function f(n: i64) -> i64`: `unreachable_statement` at the `break` and `missing_return` on `f`, both gone once the `break` is deleted | `selfhost/check/flow.hero` (panel 184's R4: a `while true` with a `break` of its own does not end a path, read by syntax) · **class: adjacent**

    **Origin:** lane flow4's report, 2026-10-03 (`scratchpad/lane-flow4/w/w16-dead-break-under-return.hero`, 2026-10-03); measured by the coordinator on the trunk at `e5893696` (`scratchpad/file-r5/`, 2026-10-03).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake.

- [ ] **209 — a `layout` run filtered to a name that matches no module reads 1 passed** | `./heroes run tests/harness/main.hero -- ./heroes layout <a name no module has>`: `1 passed, 0 failed`, because the harness's guard against an empty selection counts cases and the suite's file-wide checks are always one case | `tests/harness/suite_layout.hero` · the harness's selection guard · **class: improvement**

    **Origin:** the coordinator's agent finishing defect 167 in lane cb4, 2026-10-03, which then checked by hand that each of its six filters matched one module (`scratchpad/lane-cb4/progress.md`, 2026-10-03).

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): an instrument that can read green over nothing; no program moves.

- [ ] **210 — the recovery instrument lives in the scratchpad as a Python tool, where a reboot can lose it; its home is `heroes mutate`'s recovery arm, with its plan pinned** | `<scratchpad>/instrument/tool/` (3,618 code lines of Python, the compiler-engineer's count); panel 187's R2 reads it at every recovery round's gate, and the fourth round's reading had to subtract a corpus program the language changed under it by hand | `selfhost/cli/mutate.hero` and `selfhost/mutate/` · panel 187's R2 and R3 · **class: improvement**

    **Origin:** panel 187's R10 (filed beside the sitting), 2026-10-03, and the fourth round's differential reading (`scratchpad/inst-187/r4-differential.txt`, 2026-10-03): the port owes a frozen plan in the tree and the subtraction of an unmutated program's own messages.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): an instrument's home; no program moves.

- [ ] **211 — no golden form runs the control arm (`--permissive`), so a control-arm row cannot be pinned** | `tests/harness/suite_golden.hero` has no such word (the compiler-engineer's search); 131-22's open half and V1's five control-arm hides (defect 193) have no pin | `tests/harness/suite_golden.hero` · panel 187's R3 · **class: improvement**

    **Origin:** panel 187's R3 and R10, 2026-10-03.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): coverage.

- [ ] **212 — class (b), a mistake told only once another is fixed, has no measurement at the project's sizes** | the blind readings' two class (b) programs are 3 and 5 lines; panel 183's 20 of 20 were 31 to 105; the spec-warden's named run (rows 131-32 and `g2/r12`, ten sessions, about 4 to 5 USD) and the 300-line form panel 183's critic left owed are unrun | panel 187's R7 and R10 · `docs/panel/187-reports/spec-warden.md` § 8 · **class: improvement**

    **Origin:** panel 187's R10, 2026-10-03; deferred by the author's answer *7b* (*later, after the ratification*): a paid run the author funds.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a measurement nobody needs to be right today; it decides whether D1's stronger promise enters.

- [ ] **213 — the Linux image carries no SDL3, so defect 151's SDL3 event runs on this Mac alone** | `run/ffi-a-construction-polls-an-sdl3-event` in the arm64 container: *the package sdl3 is not installed on this machine*, skipped; the CI's Linux jobs and the Windows leg skip it too, by their totals | the `heroes-linux-arm64` image and the CI's install list · defect 151's closed record · **class: improvement**

    **Origin:** the coordinator's closings agent, 2026-10-03, and the author's answer *4a*.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): coverage of one platform.

- [ ] **214 — clang on Windows writes every path of a dependency listing with backslashes, and two readers compare it with forward slashes: defect 168's own test fails there, and no `skip` prefix matches** | `heroes test selfhost/main.hero` on the Windows box at `02e507bc`: *1083 tests, 1 failed*, `what a probe's headers resolve to is asked of clang, and a header that appears earlier in the search is another answer (defect 168)` at `process.holds(text: beside, needle: root + "/src/x.h")`; the box's clang, given `-I build/selftest-compiling-reads/src`, writes `build\selftest-compiling-reads\src\x.h`; and a translation unit's record there lists its own `build\tu-0b52ebf10bd34d86\library.c`, the file `rows_of`'s `skip: dir + "/"` exists to leave out | `selfhost/cli/compiling.hero` (the test), `selfhost/cli/deps.hero` (`under`, `rows_of`) and its five callers · **class: blocking**

    **Origin:** the coordinator's Windows leg at `02e507bc`, 2026-10-03: the compiler's tests red there, where the thirteen legs before it read them all passed (the last, `6bec7c8c`, 1056 of 1056); the test named by a second run in a folder of its own (10:08 to 10:13 by `date`), the dependency listing and the record read on the box by hand.

    **2026-10-03, the first half repaired at `b8ad7f9e` and `fcf09839`, lane win214, merged at `773db596`**: defect 168's test reads clang's rows with every separator a `/`, the turn written in the test as a list of pieces joined once, and holds each path to the digest of what it holds, in one row. Gated once on this Mac on the merged tree: the seed regenerated, the same bytes as `02e507bc`'s once its `#line` numbers are masked, the compiler built from it the same binary as `02e507bc`'s by `cmp`, the fixpoint by `cmp`; the compiler's own tests 1083 and the net's own 200, all passed; the full net, 26 suites, 4,901 passed and 0 failed. In the Linux arm64 container, under clang 22.1.8 and again under 18.1.8, the compilers built from the two seeds are the same binary and the compiler's own tests read 1083 of 1083; on the Windows box the assembly clang writes for the two seeds is the same and the compiler's own tests read 1083, all passed, defect 168's three among them. The second half, a `skip` prefix no Windows listing matches (`under` in `selfhost/cli/deps.hero`), is open.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a red on a platform, and the CI's Windows leg would be red on the push. The second half, a `skip` that never matches on Windows, has the same cause and stays in the item; it writes no wrong value, the file it fails to leave out being keyed and hashing the same, and costs each warm build there the digest of every unit's C (unmeasured). The test's other assertion, that the answer does not hold `root + "/inc/x.h"`, passes on Windows whatever `reads` answers.

- [ ] **215 — a text grown by `+` in a selfhost module is first seen by the whole `layout` at a gate: the write-time hook does not ask it, and `layout` narrowed to the file cannot** | lane win214's `b8ad7f9e` wrote `beside @ beside + ch` in a loop of `selfhost/cli/compiling.hero`'s test; `.claude/hooks/fmt_check.py` passed it (it asks parse, canonical form, the compiler's check and the line ceiling), and the lane's gate read `layout/concat` red at 11:06, four sites; `tests/harness/suite_layout.hero` asks `appends`, `concat` and `budget` only when `only == ""`, so a narrowed `layout` never asks them | `.claude/hooks/fmt_check.py`, `tests/harness/suite_layout.hero` (`GROWTH_ALLOWED`) · defect 209 · **class: improvement**

    **Origin:** the coordinator, 2026-10-03, at lane win214's gate: one run of 25 suites stopped at 20 to repair it, then run again whole.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): an instrument's coverage. § A suite is the last judge asks that what a hook can see on the touched file never wait for a suite, and the growth sites of one file against `GROWTH_ALLOWED`'s entries for that file are such a thing.

- [ ] **216 — a header whose name holds a `>` passes `check` and stops `build` at exit 2 with an internal error and clang's text** | `extern "a>b.h"` over `function seven() -> i32`, the header beside the program: `check` exit 0; `build` exit 2, *internal error: compiling the generated C failed*, clang's `#include <a>b.h>` and *'a' file not found*, then *error: clang refused the generated C*; the same program over `ab.h` builds and prints `7` (the trunk's compiler, `02e507bc`'s seed, 2026-10-03) | `selfhost/parse/group_head.hero` (what a group head refuses of its header's text, `machine_locked_path` its one refusal today) · the `#include` line the emitter writes · **class: blocking**

    **Origin:** lane warn's first pass beside defect 207, 2026-10-03, reported to the coordinator with its reproducer; reproduced by the coordinator the same day.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 and an internal error where the author can be told. No angled `#include` can spell a `>`, and panel 036's R2 keeps the angled form and never the quoted one, since a quoted include takes a decoy planted beside the unit (`docs/panel/036-the-ffi-ladder.md`); so the repair is a refusal at `check`, a diagnostic class and so a sitting's (CLAUDE.md § 4) before a lane's.

- [ ] **217 — under `ulimit -s unlimited` on Linux, a program that starts a thread panics at its first spawn** | `examples/threads` as it stood at `02e507bc`, run in the Linux arm64 container with the stack limit unlimited: a panic at the first spawn, glibc reporting the main thread's stack as 93,823,035,207,680 bytes, from which `runtime/parts/spawn.c` asked a floor | `runtime/parts/spawn.c` · lane depth's commit `ffaf9f4d` · **class: blocking**

    **Origin:** lane depth's first pass beside defect 169, 2026-10-03, reported to the coordinator (`<scratchpad>/lane-depth/linux2/`).

    **2026-10-03, repaired at `ffaf9f4d` in lane depth, beside defect 169**: under an unlimited stack limit no floor is asked. Owed at the round's gate, and on the platforms before the push, `runtime/` being the C boundary.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a crash of a correct program in a setting Linux allows.

- [ ] **218 — the IR verifier's cost grows about as the cube of a function's size, so 7 of 25 nesting shapes at 2,000 deep do not build in 150 s on this Mac, 8 in the Linux container** | lane depth's 25 shapes (panel 184's thirteen and twelve beside them) at N = 2,000: `check` and `fmt` hold every one, `build` of seven does not finish in 150 s; the cost the lane traced to `released_on_return` in `selfhost/ir/phases.hero` and `dominators` in `selfhost/ir/values.hero` (`<scratchpad>/lane-depth/pass1-findings.md`) | those two functions · panel 184's R6 · **class: blocking**

    **Origin:** lane depth, 2026-10-03, measuring panel 184's R6 on this Mac and in the Linux arm64 container, reported to the coordinator; not yet run by the coordinator.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): panel 184's R6, ratified, owes spec § 9 a sentence the lane drafted, *a source nested up to 2,000 deep compiles on every platform*, and it cannot be written true while these builds do not finish; a cost that stops a needed program from building at all is compiler need (CL-006).

- [ ] **219 — clang's debug information dies on a type chain between 3,000 and 5,000 nested variants on this Mac, by 10,000 in the Linux container** | past panel 184's R6 floor of 2,000: since `6c95f44a` (defect 170) the build says so in the compiler's words at exit 2 and leaves no crash files; clang's own stack raised to 64 MB compiled 10,000 in the lane's measurement, about eight times further, not built | `selfhost/cli/clang_died.hero` (lane depth's, at `6c95f44a`, 2026-10-03), `selfhost/emit/typeorder.hero` · **class: improvement**

    **Origin:** lane depth beside defect 170, 2026-10-03, reported to the coordinator.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): beyond the floor panel 184's R6 sets, and told truly at exit 2 since defect 170's repair; a reach nobody needs to be right today.

*******************************************************************************

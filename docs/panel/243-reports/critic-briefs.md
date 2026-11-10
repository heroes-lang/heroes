# Panel 243, the completeness critic's first pass, over the briefs

The critic's final message, saved by the coordinator (a subagent may not write a report file), 2026-11-10.


I found 22 items. In short:
- **Wrong in the briefs:** five line numbers, paths or cases are wrong or imprecise. The briefs also say the compiler already reads a format's conversions; it has no such reader.
- **Missing:** six routes nobody listed and seven questions the sitting does not ask.
- **To file:** three things found beside the work. The worst is a `surface` fixture the net keeps green that prints a wrong value at exit 0.

Everything was run with `b29-round/heroes-next` (`sources 14e9e5ce168a2336`). Probes are under `.claude/worktrees/p243-scratch/critic/` in `vararg/`, `curlwrap/`, `curldirect/`, `sqlitewrap/`, `q3danger/{a,d,live,live2}/`, `fixedarity/`, `twoarity/` and `c/`. No process of mine is still running, and I wrote nothing in the tree and made no GitHub write.

## Confirmed as stated

1. **The base, the reproducers and the counts hold.**
   - **Ran:** `git log -1`, `git log -1 origin/main`, `git worktree list`, `heroes-next --version`, the 2421 and 2428 builds, `grep -n` on spec and design.md, `heroes-next measure spec/heroes-spec.md` from b29-round's root, a `sizeof(mode_t)` C probe, `brew info --json=v2 curl`.
   - **Printed:**
     - HEAD and `origin/main` are both `89db843472cf…`.
     - The three seat worktrees are detached at `89db84347`.
     - 2421 builds at exit 0 and prints `4294967295`.
     - 2428 is exit 1 `shadowed_header` with the keg's `PKG_CONFIG_PATH`, and exit 0 without it.
     - The spec and design.md lines cited are right: spec `373`, `379-382`; design.md `3903`, `3162-3163`; `ffi_call.hero:144`.
     - `shadowed.hero`, `shadowed_tree.hero`, `shadowed_words.hero` and `examples/curl` exist.
     - `measure` prints 8167, 8314 and `real 10898 claude-opus-5, 2026-11-09`.
     - `mode_t` is 2 bytes here.
     - curl is keg-only (`provided_by_macos`): 8.23.0 in the keg, 8.7.1 in the SDK.
   - **Repair:** none.

## Framing facts to correct

2. **"panels 241 and 242 this milestone" (00-shared).**
   - **Ran:** the first 12 lines of each of panels 234 to 240, grepped for `M-`.
   - **Printed:** panel 234 names M-core-packages (2026-11-05); 235 names both M-core-packages and M-package-manager.
   - **Repair:** "panels 234, 241 and 242 sat in this milestone; the gate was asked at 07:35 on 2026-11-10 (panel 242's header)."

3. **The R10 paragraph is "3901-3906".**
   - **Ran:** `awk 'NR>=3880&&NR<=3915'` and the Part 7 row numbers.
   - **Printed:** the 2026-11-06 correction runs 3900 to 3910, in Part 7 row 10 ("Sized integers", 3869). Issue 2421 cites 3890-3894, the older falsifier.
   - **Repair:**
     - Cite 3900-3910, Part 7 row 10.
     - Add that R10 says "a parameter has a spelling every leg shares". Past `...` no shared spelling of a C `long` exists: `i32` is wrong on Mac and Linux, `i64` is undefined on Windows (panel 242's ffi-pragmatist § 1.4 printed `sizeof(long)` 4 on the box). Route (C)'s wording must say so.

4. **Route (R): "a header of the program's own declaring the variadic at a fixed shape".**
   - **Ran:** `grep -n` in panel 052 (lines 32 and 164) and in `ffi_past_prototype.hero` (205, 233).
   - **Printed:** panel 052 ratified the ffi-pragmatist's standing veto on any variadic emitted through a re-declared prototype (wrong on arm64). 2107's note asks for a `static inline` function of a new name that hands the argument on and is bound instead.
   - **Repair** (00-shared and ffi-pragmatist): "a header of the program's own defining a `static inline` function of a new name with a fixed prototype, which calls the variadic through its real prototype (2107's note); never a re-declaration (panel 052's veto)."

5. **"whose conversions the compiler reads".**
   - **Ran:** `grep -rn` over `selfhost/` for `"%d"`, `byte == 37`, `length modifier`, `va_arg` and `conversion spec`. Also `--emit-c` of `run/ffi-variadic-at-a-fixed-arity`.
   - **Printed:**
     - No conversion reader exists. The one format rule reads clang's `-Wformat-security` words from the probe (`ffi_call.hero:62,111`).
     - The format reaches C as `printf(hero_cstr_nonnull_at(t2, …), t3)`, a run-time pointer, never a literal.
   - **Repair:** say the reader is new code, and ask the compiler-engineer:
     - its grammar (`*` width eats an `int`; `hh`, `h`, `l`, `ll`, `j`, `z`, `t`, `L`; `%c`; `%n`);
     - how a "format function" is known (a name list, or clang's `format` attribute in the AST dump, as `cli/renamed.hero` reads `AsmLabelAttr`; 2034's `say` carries one);
     - what "literal" means through `.cstr()` and `.lease()`, a `str` constant, or a `format: str` parameter.

6. **"`open(path, flags, mode)` with `mode` as the tree binds it today".**
   - **Ran:** `git grep -n -E '^\s+function (open|openat|fcntl|ioctl)\(' -- '*.hero'`.
   - **Printed:** nothing. The only binding is `docs/panel/178-reports/.../flock_S.hero:10`; the lane's measurement is its own `2421/modet.c`.
   - **Repair:** "the tree binds no `open`; write it as a case, `mode: i32`."

7. **"the 2420 and 2343/2344/2356/2359 cases".**
   - **Ran:** `git ls-files 'tests/golden/*' | grep fixedbugs-(…)` and `git grep -l 2359`.
   - **Printed:** 2359 has no golden case. It is cited only in `selfhost/cli/shadowed.hero` and `shadowed_tree.hero`, the compiler's own tests.
   - **Repair:** say where 2359's cases live.

8. **"panel 237's cases (the `shadowed_header` cases under `tests/golden/`)".**
   - **Ran:** `git grep -l shadowed_header -- tests/golden`, then a grep of those cases for `pkgconfig|PKG_CONFIG`. Also read `tests/harness/case_setting.hero:55-61`.
   - **Printed:**
     - 20 `.case` directories hold the refusal, and none reaches through `pkg-config`.
     - A case's `environment` may set only `PATH`, `GIT_CEILING_DIRECTORIES`, `CPATH`, `C_INCLUDE_PATH` and `CCC_OVERRIDE_OPTIONS`.
     - Panel 237's `.pc` probes exist only in its reports (`ffi-pragmatist.md:227`, `critic-reports.md:200`).
   - **Repair:**
     - Point the compiler-engineer at those probes.
     - Ask what form holds Q2's cases: either `SETTABLE` widened, a `tests/harness/` change owing the net's own tests, or another way to reach a `.pc`.

9. **Q3's shapes, "`"sys/timeb.h"` from the folder above".**
   - **Ran:** read all seven shapes and their `.after.txt` files.
   - **Printed:**
     - Shape g is `clock.h` at the root, including `"sys/timeb.h"` from its own subfolder.
     - Shape e (`sys/clock.h` including `"sys/timeb.h"`, found through `-I .` and not from the includer's folder) builds today. The lane's sentence ("found from that header's own folder") does not cover it.
   - **Repair:** describe each shape by its files, and require each route to say how it reads shape e.

10. **Q3 is "defect 2420's two rows left", but 2420 is closed.**
    - **Ran:** `gh issue view 2420`, and `gh issue list --state all --search` with five phrasings (`shadowed_header not a sibling`, `quoted include folder above`, `"../sys/timeb.h"`, `shadowed_header quoted`, `sibling quoted shadowed`).
    - **Printed:** CLOSED. No open issue holds the two rows.
    - **Repair:** say so in the brief. The synthesis files them (a correct program refused is `blocking`), or `records/tagged` cannot see them.

11. **Pointers the brief omits.**
    - **Ran:** read `ffi_past_prototype.hero:1-30`, `gh issue list --search 'Panel 217 in:title'`, and `docker images`.
    - **Printed:**
      - The module rests on panel 217's R3(c), and drops asks in `cli/header_types.hero`'s `answered` and `emit/ffi_read_only.hero`'s `lend_answers`.
      - Its doc says "provisional"; 217 was ratified 2026-10-17 (issue 2105, closed).
      - Docker: "Cannot connect to the Docker daemon".
    - **Repair:**
      - Add panel 217 R3(c) and those two functions to Q1's "where to look".
      - Tell the ffi-pragmatist that panel 242's report § 1.4 already holds the Linux arm64 and box measurements, in case its one Docker try fails.

## Routes nobody listed

12. **Q1 (R0): no format exception at all, symmetric with 2107.**
    - **Ran:** `gh issue view 2107`, and a grep of by-value integer bindings past `...`.
    - **Printed:**
      - 2107 refused `sscanf("%d", @n)` even with a literal format.
      - The tracked by-value integers past `...` are four `i64` bindings: `run/ffi-variadic-at-a-fixed-arity`, `run/fixedbugs-584` (`sprintf`), `surface-fixtures/twoarity/as_int` and `twoarity-lent/as_int`.
      - The tree's own variadics (`va_*`, `tally_bump`) pass only `cstr`, `ptr`, a record or `@`. `packages/` binds no `extern`.
    - **Repair:** list (R0) and ask whether (R)'s exception should reach `@` past `...` as well, or neither should have one.

13. **Q1 (E): put the literal format into the C call, so the `-Wformat` the compiler already keeps judges it.**
    - **Ran:** `grep -n` in `header_region.hero:100` and the 2034 case.
    - **Printed:** `-Wformat` is an error in header code today, and refused 2034's `say("%d", "seven")`.
    - **Cost:** the lane measured that it compares types, not widths (`%ld` with `int64_t` and `%zu` with `uint64_t` refused here; `%lld` with `int64_t` refused on Linux arm64, which is the tree's own case).
    - **Repair:** list it, and tie it to question 19.

14. **Q1's shapes beside 2421: every by-value argument past `...`, not only narrow integers.**
    - **Ran:** `twoarity/main.hero` built with `value: 4294967338`.
    - **Printed:** exit 0, `n = 42`. A wider integer read narrower gives a wrong value; `f64` or a pointer read as a `long` are the same class.
    - **Repair:** widen (R)'s scope question to every by-value argument past `...`.

15. **Q2 is not about `pkg-config`.**
    - **Ran:** 2428's wrapper built with the keg's folder under `CPATH`, under `--include`, and through `PKG_CONFIG_PATH`. A sqlite wrapper over the keg. A group naming `<curl/curl.h>` directly with the keg's `.pc`. The `pkg-config --cflags` answer. A collision count of every installed keg-only formula against the SDK.
    - **Printed:**
      - **Not `pkg-config`'s alone:** `CPATH`, `--include` and `pkg-config` all give exit 1 `shadowed_header`.
      - **Not curl's alone:** the sqlite wrapper is refused too, though `sqlite3.h` is flat.
      - **The wrapper alone is refused:** the direct group builds and prints `libcurl/8.23.0`.
      - **Nine folders:** `pkg-config` names nine `-I` folders, eight of them other libraries'. Only curl's own collides with the SDK (11 headers).
      - **Population:** seven keg-only formulae collide here: curl, ncurses, readline, sqlite, icu4c@78, icu4c@79, llvm@22.
    - **Why it matters:** every wrapper the compiler's own notes draft is refused for such a library:
      - `ffi_macro_name` (`pragma_ask.hero:298`);
      - 2107's `@` past `...` (`ffi_past_prototype.hero:205,233`);
      - and (R) for 2421.
    - **Routes nobody listed:**
      - (a) **Group-scoped:** a file the program's own header includes is that library's when it lies in a folder the same group's answer names.
      - (b) **Consistency:** a file a group of the same build names directly is the program's choice wherever it is included.
      - (c) **Narrower second arm:** narrow R2's second arm ("a platform folder holds a file of its name") for folders named for a library, keeping the first arm.
      - (d) **Per arm:** `pkg-config` ties the header to its own `-L`; `--include` and `CPATH` do not (a header of 8.23.0 with the SDK's 8.7.1 library). Ask whether the answer may differ by arm.
    - **Repair:** reframe Q2 as "a library header in a named folder, included by a header of the program's own" and add these routes and the population.

16. **Q3's danger side is untested, and the landed sibling rule has a hole.**
    - **Ran:** a quoted sibling `sys/types.h` (guarded, `#include_next`), then a second group `<sys/socket.h>`, built in both orders. Then `clang -H`, and `clang -H -fshow-skipped-includes`.
    - **Printed:**
      - **One order:** clock first builds at exit 0. An unguarded copy shows `SDK/sys/socket.h` → `./sys/types.h` in `-H`.
      - **The other order:** socket first is refused `shadowed_header` "which the C library's `<sys/socket.h>` includes".
      - **The instrument:** `-fshow-skipped-includes` lists the skipped include under a guard (Apple clang 21; unrun on Linux).
    - **What it shows:** the rule judges a file by its first open in `-H`. The lane's shapes use `sys/timeb.h`, which the platform never reads, so they cannot test the danger.
    - **Repair:** add this shape and this flag to the compiler-engineer's and ffi-pragmatist's Q3, and judge the lane's widening on them.

## Questions the sitting should ask

17. **Q1 and Q2 are one program.**
    - **Ran:** read `coord-curlwrap/undef/net/opt_long.hero`, and panel 242's ffi-pragmatist §§ 1.4 and 1.6.
    - **Printed:**
      - The reproducer binds `curl_easy_setopt(…, value: i64)` past `...`: correct here, undefined on Windows, refused by (R).
      - Panel 242's `h6/net/net_http.h` (a typed `long` door) is (R)'s wrapper and builds against 8.7.1, but over the keg it is 2428's shape.
      - `curl_easy_getinfo(@out)` already needs a wrapper under 2107.
    - **Repair:** ask, for each Q1 route, what the libcurl package writes and whether it builds over the keg. Q2 is a prerequisite of (R).

18. **Is (C) admissible at all?**
    - **Ran:** read CLAUDE.md § Precedence and `verification.md` § Bounded discovery ("blocking… never deferred"), plus panel 242's R5.
    - **Printed:** R5 refused (c) on 2421 by the ffi-pragmatist's veto; (C) pins a wrong value at exit 0.
    - **Repair:** ask whether (C) can be a resolution, or only the record of what the conservative route would have been.

19. **Which yardstick: the C standard's types, or the widths the machine reads?**
    - **Ran:** the lane's report, line 40.
    - **Printed:** `-Wformat` refuses same-width, different-type pairs.
    - **The question:** C11 §7.21.6.1p9 and §7.16.1.1p2 judge by type (`%zu` with `unsigned long long` is undefined by the letter at the same width).
    - **Repair:** ask (R)'s reader which yardstick it applies, and add those two clauses, and Apple's arm64 variadic ABI page, to the historian's Q1 (its Q3 already asks for the standard).

20. **Windows.**
    - **Ran:** panel 242's ffi-pragmatist § 1.4.
    - **Printed:** `sizeof(long)` 4 on the box.
    - **Repair:** ask that (R)'s reader be per platform and that its note say so; R10 needs its exception written (item 3).

21. **The blind seat.**
    - **Ran:** `grep -n -i -E 'variadic|\.\.\.|va_arg|printf|format' spec/heroes-spec.md`.
    - **Printed:** the only hit is "no user variadics" (289); § 13 says nothing about `...`.
    - **The question:** the brief leaves the llm-ergonomist out for cost, while CLAUDE.md § 4 says to choose seats by whether their input differs.
    - **Repair:** if the spec-warden finds that (R) needs a § 13 sentence, the synthesis queues the blind seat's paid run for the author's yes rather than dropping it.

22. **The compiler's own drafted bindings past `...`.**
    - **Ran:** `sed -n 135,150p ffi_call.hero`.
    - **Printed:** line 148's note offers `function NAME(format: cstr, value: i64) -> i32` whatever the format reads.
    - **Repair:** ask the compiler-engineer to list and rewrite every note that drafts a binding past `...` (`ffi_call.hero:139,148`, `pragma_ask.hero:298`, `ffi_past_prototype.hero:205,233`) under the chosen route.

## Found beside the work, for the coordinator to file

None of these has been filed; I have no GitHub write.
- **The `twoarity` fixture (item 14).** A wrong value at exit 0. The `surface` suite pins it green (`suite_surface.hero:687`). That is `blocking` by its class's definition. Searched with `twoarity printf`, `i64 %d variadic`, `wider than its C read`, `printf %d i64`: no issue holds it.
- **The order-dependent stand-in (item 16).** Panel 237's class is accepted in one order of the groups. I showed no wrong value, so its class is the coordinator's to read.
- **The stale doc (item 11).** `ffi_past_prototype.hero` still calls panel 217 provisional, an `improvement`.
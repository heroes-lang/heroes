# Panel 197, ffi-pragmatist's report

Copied by the coordinator at 14:16 on 2026-10-07 from the seat's final reply,
verbatim: the seat's Write tool refused its report file, so the reply is the
record. Its raw outputs are under
`<scratchpad>/197-ffi-pragmatist/` (`out/`, `prog/`, `box/run.sh`, `e/e_ffi.c`,
`coff/unit.c`, `linux/shape/mem.py`); the scratchpad is session-specific, so
what must survive is this file.

---

**verdict**: object to (B). **Veto (J)**: it loses a sanitizer's report location. Adopt **(C)**, `-g` for unoptimised builds and `-gline-tables-only` at `-O2`, with (E)'s declarations allowed alongside it. Approve (A), (E) and (K) as neutral for bindings, and (I) as neutral on Mac and Linux (the box is unrun). (D) and (L) are not judged here.

**section**: design.md §1.11 (`:467`), §4.19 (`:2331`), §3.1 *Debug info* (`:653-654`). **The document does not cover my objection to (B)**: being able to inspect the C locals of a binding's header code. Part 2 `:630-632` speaks only of Heroes values.

**experiment**: `S/prog/bind.h` defines `struct vec2` and four plain `static inline` functions (no `noinline`): `vec2_dot` (three locals), a planted `int32_t` overflow, a planted heap use-after-free, and a C recursion. Six `.hero` programs bind it. Each was built with `heroes build` by four compilers whose `flags.hero:94` was changed to `-g`, `-gline-tables-only`, `-g0`, or `-g -gno-column-info`, at `-O0` and `-O2`, with and without `--sanitize`. In Docker the same three words were made by editing seed line 896. clang accepted every build: 69 binaries on the Mac and 42 in Docker, all exit 0.

1. **lldb on this Mac.**
   - `-g` and (K) at `-O0`: `step` enters `vec2_dot`, `bt` shows `vec2_dot(a=…, b=…) at bind.h:13:20`, and `p px` reads 15.
   - Line tables at `-O0`: step and `bt` with file and line still work, but `frame variable` says *no variable information*.
   - `-g0`: the breakpoint never resolves (*no locations*).
   - At `-O2`, `-g` reads one value falsely: `a = (x = 4…)` at `:13`, then `a = (x = 0…)` at `:15`.
   - (E), applied by hand to the emitted unit, behaves exactly like `-g`.
2. **Sanitizers through the compiler's own path** (debug map, no `.dSYM`).
   - After addresses are normalised, ASan's report is identical under `-g`, line tables and (K), at both levels, on the Mac and on Linux.
   - UBSan's `bind.h:22:21` is the same under all four words.
   - Under `-g0` the location is lost everywhere. Linux names a **false file**: `vec2_uaf uaf.c` for code that lives in `bind.h`. At `-O2` the header's frame disappears entirely.
3. **The stack guard.** On the Mac it names `deep.down` and `vec2_deep` under every word, including `-g0`. So `flags.hero:174`'s *"because of `-g`"* is false as a cause, and its wording needs repair under any route that moves `-g`. On Linux it names `deep.down` under every word, and names nothing for the header's C recursion. That gap does not depend on the route; it is the `.dynsym` fact panel 156 recorded.
4. **The box**: unrun. It has been offline since about 14:00 (`tailscale status`, ssh timeouts 14:04 to 14:16). As a substitute, I built cross-target COFF from the container (clang 22.1.8 and lld-link, not the box). `-gline-tables-only` keeps the procedures, the inline sites and line tables identical to `-g`'s at `-O0`. It drops every `S_LOCAL`. `-g0` leaves a `.pdb` with neither.
5. **Linux arm64, 400-return shape, `-O2`**. The emitted C is byte-identical on Mac and Linux.
   - `-g`: max RSS 1,942,786,048 and 1,938,161,664.
   - Line tables: 836,980,736 and 837,107,712, which is **−56.86%**.
   - cgroup peak: −59.60%.
   - `-g0` is −0.48% from line tables; (K) is −0.21% from `-g`.

**argument**: Every route that keeps line tables keeps everything a binding needs at the boundary: the step into header code, `bt` with file and line, every ASan and UBSan location, and the guard's name, on both platforms. The one thing at stake is `p` on a header's C locals. Losing it at `-O0`, where authors debug (`heroes build` and `heroes test`), is a real cost of (B). At `-O2` the `-g` view already printed a false value, so (C) gives up little that was trustworthy and saves 57% of clang's memory. (C)'s word depends only on the level, which `build_words` already carries in the cache key. (J) loses report locations on both platforms and invents a wrong file on Linux.

**prediction**: On the box, `llvm-pdbutil dump -symbols` of `stepr.c` linked with the compiler's words will list `S_LOCAL px` under `-g` and none under `-gline-tables-only`. `dump -l` will list identical `stepr.hero` and `bind.h` rows under both. And `-gline-tables-only` on the link line will still produce a `.pdb`.

**condition**:
- I would object to (C) if, on the box, line tables drop the `.pdb` or the header's lines, or if a `--sanitize` report on Windows or the CI's clangs loses its location under line tables.
- I would accept (B) if the sitting decides C-local inspection in header code is outside v1, which design.md is silent on.
- (E) should cover Heroes types only. On extern records it is harmless (no warning), but it forces their full DWARF into the object: 3,168 → 7,120 bytes for seven raylib and sqlite types.

**Could not run**:
- The whole of item 4 on the box. It was offline, so lldb.exe and the box's 23.1.1 `.pdb` are unread.
- `report.md`: the Write tool refuses report files from a subagent, so everything is here instead. Raw outputs are in `S/out/` (for example `linux-400-mem.txt`, `linux-san.txt`, `linux-guard.txt`, `coff-cross.txt`, `san-*.err`, `lldb-*.txt`).
- Calling a raymath.h `inline` at `-O0` goes to libraylib's out-of-line copy under every word, so it does not depend on the route. At `-O2` the breakpoint resolved but never stopped, and I did not chase why.

Files are under `S` = `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/197-ffi-pragmatist`: `prog/bind.h`, `prog/*.hero`, `box/run.sh` (ready for the box), `e/e_ffi.c`, `coff/unit.c`, `linux/shape/mem.py`.

---

## Item 4 on the box, 14:22 to 14:24 (the seat's second reply, verbatim in substance)

The box answered again at 14:22; the seat ran its ready `run.sh` once in
`/c/w/p197-ffi-99642`, compiling one emitted file, `stepr.c` (the same file
the Mac and Linux emit), with the compiler's own compile and link words at
`-O0` under each debug word, clang 23.1.1. Every compile and link exited 0
with no warnings; both programs printed `dot: 48`.

| bytes | `-g` | `-gline-tables-only` | change |
|---|---|---|---|
| program object | 18,363 | 12,863 | −29.95% |
| runtime object | 406,737 | 317,829 | −21.86% |
| `.pdb` | 7,229,440 | 7,163,904 | −0.91% |
| `.exe` | 710,144 | 710,144 | 0 |

`llvm-pdbutil dump -l -symbols`: under `-g` the program's module holds 67
local-variable records, `vec2_dot`'s five (`a`, `b`, `px`, `py`, `sum`) among
them; under line tables none, program or runtime. The same functions under
both words, `vec2_dot` included. Line rows 113 against 100 for the program
(3,273 against 2,948 for the runtime); the extra rows repeat a line already
listed at a later address, and with them removed both lists hold the same
lines at the same addresses, for `stepr.hero` and `bind.h` alike.
`llvm-symbolizer` names `vec2_dot` at `bind.h:13` and `:14` and `h_stepr_main`
at `stepr.hero:11` under both `.pdb`s (the path a Windows sanitizer report
uses); it reads no locals under either.

**lldb.exe dies before doing anything**: *could not load 'python3.dll'*,
exception `0xC06D007E`, for `--version` and both programs; the box has no
Python, `liblldb.dll` imports `python3.dll` directly. Installing Python on the
box was not the seat's to do, so no breakpoint was set there.

**Prediction scored**: (1) a local record for `px` under `-g` and none under
line tables: held; (2) identical line rows: held for the lines, not word for
word (113 against 100, the difference repeats only); (3) line tables on the
link line still produce a `.pdb`: held.

**Verdict unchanged**: adopt (C); object to (B); veto (J). The condition (line
tables must not drop the `.pdb` or the header's lines on Windows) is met.
What a Windows author sees stepping under either word is unsettled with any
debugger on the box. Raw output: `<scratchpad>/197-ffi-pragmatist/out/box-run.txt`,
`out/box-lldb.txt`; the pdbutil dumps stay on the box.

# Panel 204, completeness critic, second pass

Copied by the coordinator at 02:29 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 204, completeness critic, second pass

I worked from 02:15:01 to 02:28 on 2026-10-10 (`date`) in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/critic-204b/`. I give no verdict.

Setup:
- **Compilers:** the compiler-engineer's `heroes-base` and `heroes-p3`, copied into `bin/` and run with `HEROES_RUNTIME` set to a copy of the frozen tree's `runtime/`.
- **Cases:** the seats' cases were copied, and their `build/` folders and binaries deleted before every run. Each case was built fresh with both compilers by `scripts/case.sh`. Results are in `out/base.tsv` and `out/p3.tsv`.
- **No git command** was run in any copy. No paid run.

## Findings, each from a command I ran

**1. `cfgone/main2`: the compiler-engineer refuses it, the ffi-pragmatist accepts it. Checked against C11 itself.**
- **The standard:** N1570 §6.10.3, read by fetch. Paragraph 2 sits under *Constraints*, and *Semantics* starts at paragraph 7: *"An identifier currently defined as an object-like macro shall not be redefined by another #define preprocessing directive unless…"*.
- **So `main2` is not correct C.** Its unit violates a constraint. Two statements are therefore false:
  - the ffi-pragmatist's *accepted in both orders, as C means it*;
  - defect 563's own line, *a clang `-Wmacro-redefined` warning on a correct program*.
- **What the standard does not settle:** it requires a diagnostic and does not forbid going on. So *refuse* (the compiler-engineer) and *accept, with the warning told in Heroes' words* (the ffi-pragmatist) are both conforming. What is settled is only the word *correct*.
- **Runs:**
  - on `heroes-base`: `main` prints `3 50`; `main2` prints `3 10` with the warning;
  - on `heroes-p3`: `main` prints `3 50`; `main2` exits 1 with `ffi_header_refused`.
- **p3's message contradicts itself:**
  - its headline says *cannot be compiled in one unit*;
  - its last note says *these compile when `a.h` is included before `b.h`*.
- **The suggested move changes the program's value.** Following it turns `main2` into `main`, so `cap` returns 50 instead of 10. The note does not say so, and it carries no Fix.

**2. `dual`: two `#ifndef K` defaults, both orders silent, different values.**
- **Runs:** on base and p3 alike, `pq` prints `5 105` and `qp` prints `9 109`. Neither emits a warning. No constraint is violated, because each order defines `K` once.
- **What this falsifies today:**
  - the spec-warden's F7 is false on `dual`, on both compilers;
  - the compiler-engineer's own § 13 draft, *a macro two headers define differently is refused*, read literally, is false on his own prototype (`p.h` says 5 and `q.h` says 9).
- **What stays true:** his § 4 draft, *never matters, but between `extern` groups*.

**3. What would a detector refuse? The spec-warden's F7 checked against the ffi-pragmatist's census.**

Each module was compiled at C level in the real unit, in both orders: `census/decl.sh`, using the ffi-pragmatist's `unit.sh`, flags and `-I` list. Three comparison criteria:

| module | both orders compile? | whole `-E` text | final macro table | bound names only |
|---|---|---|---|---|
| GLFW + OpenGL, binding only `glfwInit` and `glClear` | yes | differs | differs | same |
| `cfgone` | yes | differs | **same** (`LIMIT` ends at 100 either way) | `cap` differs |
| `dual` | yes | differs | differs | `pk`, `qk`, `K` differ |

- **So:**
  - a detector comparing text or macros refuses a correct GLFW program;
  - a detector comparing the final macro table misses `cfgone`;
  - only a comparison of the bound names separates the three.
- **Caveat on that last column:** my bound-name check splits the text at `;` and compares the first line naming each function. It reported `half` as differing although `a.h`'s `half` is identical, which is position noise from my instrument. Treat that column as rough. The text and macro columns are `cmp` of whole outputs.
- **GLFW with `GL_ABGR_EXT` bound:** one order is refused. By the spec-warden's own rule (*a refused order is not a meaning*), it is accepted under any criterion.
- **GMP on this Mac:** refused in both orders before any comparison, by `-Werror=sign-conversion` inside `gmp.h` line 1882 (`fp-p/gmpmac/g.hero`, base and p3). GMP's `FILE *` case is therefore Linux-only, and I did not run it.

**4. How far `-Werror=macro-redefined` reaches.**

Measured with `census/w.sh` and `lin/w.sh`: the real unit (prefix, guard, the trunk's flags), each compiled with and without the flag.

- **Mac, the ffi-pragmatist's 125 headers, each alone:** 112 compile either way and 13 fail either way. 0 are newly refused.
- **Mac, the compiler-engineer's 4 ordered pairs** (found on panel 202's unit): all 4 reproduce in the real unit (0 without the flag, 1 with it). Their reverses compile, except `expect.h`/`jmorecfg.h`, which fails in both orders.
- **No contradiction with the ffi-pragmatist's count of 0.** His sample (`mac-headers-r.txt`) contains `expect.h` and `turbojpeg.h` but not `jmorecfg.h` or `tcldbg.h`. The two seats measured different samples.
- **A real two-library module the flag makes unbuildable** (`ex/ej.hero`, `ex/je.hero`). It binds Expect's `EXP_ABORT` and libjpeg's `JPEG_LIB_VERSION`.
  - On base, both orders build and print `1 80`, with clang's `EXTERN` warning.
  - On p3, both orders are refused, with different messages:
    - `ej`: *cannot be compiled in one unit whichever comes first*, plus the own-header advice;
    - `je`: *`expect.h` … does not compile … repair the header*, which is false, since `expect.h` compiles alone.
  - p3's search has no move to offer. The only route left is a header of the program's own with `#undef EXTERN`.
- **Linux**, `heroes-linux-arm64:latest`, clang 22.1.8, every ordered pair of the ffi-pragmatist's 85 headers (7,140): 6,598 compile with and without the flag, 542 fail either way. 0 are newly refused. The 85 headers alone: 0 newly refused. The compiler-engineer's own condition (more than 1% newly refused on Linux) is not met: it is 0.
- **The tracked tree:** `grep -rli 'macro.redefined'` over `tests`, `examples`, `selfhost`, `runtime` and `issues` matches only defect 563's file. Defect 361's three run cases `#undef` before they redefine. So the compiler-engineer's *0 of 107 moved* means the tree holds no case of this class, not that the flag was tested against one.
- **The flag also reaches a header of the program's own** (`sw-wrap/`):
  - `w_ab` prints `3 50` and `w_jpg` prints `1` on both compilers;
  - `w_ba` prints `3 10` with the warning on base, and exits 1 on p3 (`ba_own.h` refused at `a.h` line 1).
  - The spec-warden's §4.19 draft says *such a header builds in either order and prints that order's values*. That holds today, but only with a clang warning, itself a `blocking` shape. Under the flag it is false.

**5. What p3's `header_order` note says on each shape.**

| shape | base | p3 |
|---|---|---|
| `jpeg/ba` | exit 1 | exit 1, move note naming `stdio.h` at line 4 |
| `rec`, `recfirst/rec` | exit 1 | build, print `1` |
| `recfn`, `con` | `1`, `-1` | `1`, `-1` |
| `alone` | exit 1 | exit 1, no move note |
| `two_below` | `1` | `1` |
| `two_below_rev` | refused | refused |
| raylib/raymath (`rl/b`) | refused | refused, true move note |
| `pcre/raw` | refused | refused, no note |
| GLFW first | `ffi_unknown_name` | `ffi_unknown_name`, no note |
| `gmpmac/g`, `gmpmac/av` | refused | refused, no move |

- **The false advice stays.** On `jpeg/ba` and readline, p3 keeps *repair the header, or name the one that declares this group's C* directly above its own correcting note. `alone` and `pcre/raw` keep it with no correction.
- **The search never reaches `ffi_unknown_name`.** It is called only from `told_header` (`cli/whose.hero:120`), which is the `ffi_header_refused` path. `ffi_unknown_name` is emitted in `emit/ffi_declared.hero`, which the search does not touch.
- **Readline depends on an include path nobody stated.**
  - In this shell (`CPATH` and `C_INCLUDE_PATH` empty), `readline/readline.h` resolves to the SDK's libedit header, and `rd/swap` builds and prints `0` on base and on p3.
  - The ffi-pragmatist's own build used `/opt/homebrew/opt/readline/include` (his `ok.deps.txt`), and his report does not say so.
  - With `CPATH=/opt/homebrew/opt/readline/include`, `swap` is refused on both compilers, and p3 adds the move note, again under *repair the header*.

**6. The ffi-pragmatist's five new findings, sorted by cause.**
1. **The GLFW false `ffi_unknown_name`:** defect 563's cause (an earlier header configures a later one, through `GL_GLEXT_LEGACY`), reached through a different message. p3 does not repair it (measured above). Its message is a defect of its own: *`OpenGL/gl.h` declares no `GL_ABGR_EXT`* is false of `gl.h`, since `glfirst` prints `32768`.
2. **The GMP false `ffi_unknown_name`:** the same cause and code path as (1), by his Linux run. It cannot be reproduced on this Mac, where `gmp.h` is refused earlier. I did not run it on Linux.
3. **`_GNU_SOURCE`:** not defect 563's cause. The compiler's own prefix comes before every group, so neither a group order nor a program's own header can reach before it. No route in this sitting moves the prefix. I did not run it.
4. **`-Werror` inside `gmp.h` and libavutil:** a cause of its own, and an already ruled one.
   - The compiler's own note says *no warning is turned off for a header*.
   - Panel 198 hands `-isystem <dir>` on as `-I<dir>` (its ratification issue, line 10).
   - The compiler-engineer's flag widens the same policy, which is what refuses Expect + libjpeg. So finding (4) and the flag are one policy and want one ruling.
5. ***Repair the header* is false:**
   - for readline, it is 563's cause; p3 adds the right note but keeps the false sentence;
   - for `pcre2.h`, it is a cause of its own: a configuration macro no group can supply. p3 adds nothing there.

**7. G1m (the blind seat) against the spec-warden's objection.** I read the readings' files; I re-ran no session.
- **The writing task was primed.** The brief (`readings-204/k*/brief.md`) names *`fopen` and `fclose` from the system's `stdio.h` and libjpeg's `jpeg_stdio_src` from `jpeglib.h`*, in that order. Every writer also cites libjpeg's known need for `FILE`.
- **What the writing arm did measure is certainty, not order:**
  - k1 (F7) writers hedge: `k1-a` *I am assuming…*, `k1-c` *a precaution*, `k1-d` lists it among what it is least sure of;
  - k2 (F7 + G1m) writers cite the spec's sentence (`k2-a`).
- **All four readers who chose per-group isolation** (`h1-a`, `h1-b`, `j1-a`, `j1-b`) quote § 13's *clang checks every result type, constant and record field against that header* (`spec:348-349`), alongside § 4 or F7. No draft touches that line.
- **The spec-warden's objection rests on the writing arm alone.** The measured effect is on reading:
  - under F7 alone, 2 of 2 readers mispredict the correct `jpeg_ab` and `limit_ab`'s value;
  - under F7 + G1m, 0 of 2 do.
  - Under p3, `limit_ab` still builds and prints `3 50` at exit 0, so a reader who predicted `3 10` (4 of 4 in h1 and j1) is told nothing.

## Routes nobody built
1. **A detector by bound names only:** the preprocessed declaration, body and macro value of each name the module binds, compared across orders. My rough C-level check accepts GLFW + GL and refuses `cfgone` and `dual`. It is not built in the compiler, its cost is unmeasured, and the `mpq` shape is unrun against it.
2. **Reword `spec:349`'s *against that header*,** instead of G1m or together with it. Unpriced, and untried with the blind seat.
3. **The order search on the `ffi_unknown_name` path** (GLFW, GMP).
4. **Replace, not append:** when an order compiles, drop *repair the header*, and correct the headline *cannot be compiled in one unit*.
5. **Under the flag:** a note that drafts the program's own header with `#undef`, and a note that names the value change when the suggested move changes one.
6. **Scope the flag to redefinitions between two groups' headers, or drop it and tell the warning.** The Expect + libjpeg module is the case that decides between them. Neither is measured in the compiler.
7. **Keep package headers as system headers.** That would quiet finding (4) and the flag inside libraries while keeping them for a program's own headers. It goes against panel 198's ruling. Unrun.

## Questions the sitting did not ask
1. Is `main2` correct? C11 §6.10.3p2 says no, while defect 563 and the ffi-pragmatist say yes.
2. What should `dual` get: a refusal (F7), a sentence (the compiler-engineer), or a build-time message? The readers' wrong `limit_ab` at exit 0 is the plausible mistake that was actually measured.
3. Which criterion would a detector compare, and on which unit?
4. Must a move note say when it changes a value, and is it a Fix?
5. Does the flag need a ruling of its own, alongside panel 198's warnings-in-headers policy?
6. Should the writing arm be re-run, with headers named in the wrong order and a pair the model cannot know?
7. Should the readline case state its include path, or use `package`?

## What I did not run
- GMP or `_GNU_SOURCE` through Heroes on Linux; Windows.
- p3 on Linux: the Linux census measured clang's units, not p3 builds.
- The 107 tracked files and the 309 emit files on p3 (the compiler-engineer's runs, not repeated).
- `mpq`, `one`, `one2` and `dup` on p3.
- The Mac pair census with the flag, beyond the 125 headers alone and the compiler-engineer's pairs.
- Token prices.
- Any blind session.
- C11 checked only in the N1570 draft, through the fetch tool's summary.

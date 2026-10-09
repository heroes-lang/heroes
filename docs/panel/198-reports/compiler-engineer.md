# Panel 198, compiler-engineer

Copied by the coordinator at 10:13 on 2026-10-09 (`date`) from the seat's reply,
verbatim apart from this header and the HTML entities of the notification turned
back into their characters: a subagent's Write of a report file is refused, and
the seat's running notes are
`.claude/worktrees/scratch-b15/198-compiler-engineer/notes.txt` (ignored by
git). The seat ran from 08:11 on 2026-10-08 in a copy under the session
scratchpad, was stopped by the account's session limit at about 08:25, resumed
at 09:50, and lost its copy, prototype and census to a reboot at about 11:18;
resumed at 09:48 on 2026-10-09, it rebuilt its prototype from its own
transcript inside the repository's root (the author's rule of 2026-10-08) and
re-measured every number below by 10:12.

---

# Panel 198, compiler-engineer: verdict

The work was redone after the 11:18 reboot, from 09:48 to 10:12 on 2026-10-09. Everything is in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/198-compiler-engineer/`, with running notes in `notes.txt`. I rebuilt my prototype from my transcript and re-measured every number below today.

- **verdict**: **approve route K**, my own, built and run on four machines. **Veto on soundness** for (D), (E), and (H) as Go ships it. **Object** to (A) alone, (B), (B-ELF) and (G).
- **section**: design.md §1.1 (simplicity sets the ceiling), §1.7 with Part 5 (core or sugar), §1.12 (robustness; any C library must be bindable), §4.19 at `docs/design.md:2636-2642` (the list names what is permitted).
- **implementation_cost**: route K touches only the driver: `selfhost/cli/` plus two harness cases.
  - **Lexer, checker, descriptors, ownership pass, emitter, runtime:** 0 lines.
  - **`selfhost/cli/libraries.hero`:** +116 −12, of which +104 are tests. In the layout suite's unit it goes from 294 to 298, against a ceiling of 300.
  - **New `selfhost/cli/package_words.hero`:** 138 lines, 85 in the layout unit.
  - **`tests/harness/absence.hero`:** +99 lines, holding two cases and one stand-in helper.
  - **Seed:** regenerated at the batch's close, as for any `selfhost/` change.
- **needed_for_self_hosting**: no. Of the 10 `selfhost/` files holding an extern group, none names a package.
- **argument**: Nothing here is core. Part 5's seven constructs are untouched, and K is a table of whole words in the driver. It admits by name what the census shows correct C programs need: `-pthread`, and `-isystem <dir>` read as `-I<dir>`. It leaves out the two linker words the compiler's own link line already says, and it drops the Windows rpath, which lld-link reads as an input file. Every route that admits by shape or drops unknown words fails on a measured run, and so does Go's list. (G) breaches two modules' ceilings to allow one word per build.
- **prediction**: At the batch that lands K, with `-DSDL_RPATH=OFF` removed from `ci.yml:347`, three things hold on the CI's Ubuntu 24.04 x86-64 leg. `run sdl3` reads 1 passed and 0 failed. The net's own test *"an admitted -Wl,-rpath leaves a RUNPATH…"* passes. A fresh Debian 13 census refuses exactly 1 package, libpsx. It is falsified if that leg's GNU ld writes RPATH (the test fails), or if any of the 22 newly admitted Debian packages is refused.
- **condition**: Any of these changes my verdict.
  - **RPATH on a project leg:** the premise test fails because a GNU ld built with upstream's default writes RPATH (ffi-pragmatist). Then `--enable-new-dtags` must be passed on ELF only, which is (B-ELF) with its platform question.
  - **A correct library refused:** its `-isystem` header trips the compiler's `-Werror` flags under `-I`. None of Debian's nine does. Then pass `-isystem` as given.
  - **An advisory:** one naming `-pthread` or `-isystem` (the historian's condition).
  - **Windows:** a `package` build there where `-pthread` warns at a compile.

## Route K, word by word
- **`-pthread`**: passed to the compile on every platform, and to the link everywhere but Windows. On the Windows box (clang 23.1.1) a link warns *"argument unused during compilation"*.
- **`-isystem <dir>`**: handed on as `-I<dir>`. Under `-isystem`, clang silences only diagnostics inside the header's own code (`inc/h.h:1` disappears). The program's own lines, including header macros expanded in them, keep every error, on Apple clang 21, Debian clang 22.1.8 and Ubuntu clang 18.1.3. Reading it as `-I` keeps `ffi_header_refused`'s promise that *no warning is turned off for a header*, and keeps panel 188's premise true. All seven headers under Debian's `/usr/include/mit-krb5` compile under `-I`.
- **`-Wl,--enable-new-dtags` and `-Wl,--export-dynamic`**: accepted and left out. Dropping either gives byte-identical binaries on Debian GNU ld 2.44 (arm64 and x86-64), LLD 22.1.8 and Ubuntu GNU ld 2.42. Apple's ld rejects both, and lld-link warns on both.
- **`-Wl,-rpath,<dir>` on Windows**: left out. With a C stand-in `pkg-config.exe` on the box, the frozen compiler admitted `-Wl,-rpath,<dir>/h.o`. lld-link warned, then linked `h.o` as an input, and the program printed `7`. Under K the link refuses the undefined `helper`.
- **Still refused**: `--disable-new-dtags` (it moves the tag, and ffi's Q5 shows that changes what a `dlopen` finds), the position-dependent words, `-W…`, `-Xlinker`, and the comma-tunnelled `-Wl,--enable-new-dtags,--plugin,x.so`.
- **Item 4**: K restores `eac3e5b5`'s rpath argument and the 2026-08-15 answer (c) beside the list, in `package_words.hero`. It changes how `-Wl,-rpath` is judged on Windows only.

**What judged K:**

| Check | Result |
|---|---|
| Compiler's own tests | 1434, all passed, on the Mac and on the Windows box |
| `run sdl3` on the real reproducer (Ubuntu 24.04, SDL 3.2.10 from source, `SDL_RPATH` ON) | K 1 passed 0 failed; frozen 0 passed 1 failed (defect 444) |
| `run sdl3` on the Mac | 1 passed 0 failed |
| `unsupported package` | 3/0 on the Mac and on Ubuntu |
| `examples/sdl` by hand (`corpus` takes no filter) | output equals `main.expected` |
| `layout`, `canonical`, `order` | 5/0, 2/0, 3/0 |
| Net's own tests, Mac | K 1 failed, frozen 2 failed |
| Net's own tests, Ubuntu | K 3 failed, frozen 4 failed |

The one difference in each net run is my build-level case: red with the frozen compiler, green with K. The other failures are the same with either compiler and come from my copies: no `.git`, and the `HEROES_COMPILER` checks. The RUNPATH premise test passes on Ubuntu.

No `surface` row is owed, because K adds no token. Its pins are the four compiler tests and the two `absence.hero` cases.

## The other routes
- **(A)**: built. The last note is computed from the answer: one `link` per `-l`, an `--include` per header directory, a `--library` per `-L`, and a run-time warning where the answer carried an rpath. Cost: `libraries.hero` +10 −4 (300 of 300) and a new `package_way_out.hero` of 101 layout lines. 1431 tests passed. Its way out, followed exactly as written, builds and runs both tasks. On its own it still refuses 22 of Debian's 23 correct programs. Adding its note to K is not built; it would push `libraries.hero` past 300 and need code moved out.
- **(G)**: built as a flag, `--allow-word <word>`. Cost: eight modules (+62 −27) and a new `package_allowed.hero` of 65 layout lines. Three costs show up only by building it:
  - `compile.hero` goes to 303 and `produce.hero` to 304. Both stand at exactly 300 today, so any new `compile.Options` field breaks the ceiling.
  - A repeatable flag must be a search path, by author decision of 2026-08-14 (`table.hero`'s `Flag`). So (G) allows one word per build and cannot reach libpsx's four.
  - argv refuses any value beginning with `-` (`argv.hero:161`), so the flag needs a special case. The person must also say which step takes the word, which is exactly what the list encodes.

  1432 tests passed after a fix: my first note printed the raw word, and defect 237's guard caught it. The allowance its note names builds and runs both tasks. An environment-variable variant is priced at about 15 lines in one module, not built.
- **(B)**: about 4 lines, priced not built. Apple's ld refuses both words with *"unknown options"*, and lld-link warns. **(B-ELF)** needs a second platform question: a runtime function beside `hero_run_exe_suffix` in `runtime/parts/run.c`, `hero_os.h`, `process.hero`, the seed and the platform legs, about 25 lines. That contradicts `flags.hero:162-167`, *"`selfhost/` gains no platform axis"*.
- **(B-asym)** and **(F)**: K's left-out entry is (B-asym) done as a drop, which matches the ffi-pragmatist's recommendation. (F)'s drop of `--disable-new-dtags` is rejected on ffi's Q5 run.
- **(C)**: K is (C) narrowed to the census.
- **(D), veto**: dropping the `-Wl,` words links at exit 0, and an archive's constructor silently never runs (Debian 2.44, Ubuntu 2.42).
- **(E), veto**: `-Wl,-Map=Mapx` and `-Wl,--dependency-file=depx` write files named by values with no slash, and `-Wl,@resp.txt` reads options from a file (GNU ld and LLD).
- **(H) as Go ships it, veto**: a package's `-Wno-X` placed after the compiler's `-Werror=X` turns four of the compiler's errors into exit 0 on three clangs. One is `incompatible-pointer-types`, panel 103's guard against an 8-byte value written into a 4-byte slot. Go's `--as-needed`, left unscoped, drops a later `link` library's constructor.
- **(J)**: 0 lines. It builds and runs on both machines. It needs the loader to know the directory, since `link` writes no rpath.

## Census
Every refused word has equal weight. Cells read words/packages. On this Mac and Debian 13 the classifier mirrors `filter_words`; yesterday it agreed with the compiler on all 501 Mac packages. On Debian 13 the compiler's own verdict, run yesterday, gave 23 refused and then 1.

| Machine | Packages refused | `-Wl,` | `-pthread` | `-isystem` | `-W` | Other | Under K |
|---|---|---|---|---|---|---|---|
| This Mac, pkg-config 3.0.7 (505 lines, 501 answer) | 221 | 0 | 1/1 | 0 | 1251/219 | 1/1 (uvwasi) | 220 |
| Debian 13, pkgconf 1.8.1 (155 listed) | 23 | 6/3 | 19/13 | 9/9 | 0 | 0 | 1 (libpsx) |
| Ubuntu 24.04 with the CI's install list and SDL from source | 1 (sdl3) | 1/1 | 0 | 0 | 0 | 0 | 0 |

- **This Mac:** the 219 `-W` packages are all `absl_*`. Their API headers (`string_view.h`, `status.h`) do not parse as C.
- **Debian 13:** the gap to the critic's 124 is the 31 packages that answer nothing. The `-Wl,` words come from gmodule (2) and libpsx (4).
- **Position-dependent pairs:** only libpsx, on Debian. No package on any machine uses `--start-group` or `--push-state`, and none uses `-Xlinker`.

## Notes for the blind seat
These are the compiler's own output for `heroes build main.hero -o main`. Both programs match the hashes the ergonomist's brief records (t1 `faf2b7ec88ace6a7`, t2 `bb55e8ac2de3488b`). M is the frozen message, identical to the brief's variant A. The headline and excerpt of A and G are the same as M's.

**Route K (variant B1): no message on either task's machine; both build.**
- t1 on Ubuntu prints `true true true 42 true`, and the binary carries RUNPATH `/usr/local/lib`.
- t2 on Debian 13 prints `libcurl/8.14.1 OpenSSL/3.5.7 …`.

So K falls back to (A)'s note below. Where K still refuses (libpsx), its first note reads:
```
  note: only `-D`, `-U`, `-I`, `-L`, `-l` and `-F`, each with its value joined or as the next word, `-isystem <dir>` (handed on as `-I<dir>`), `-pthread`, `-framework <name>`, and `-Wl,-framework,<name>` and `-Wl,-rpath,<dir>`, in one word or split as `-Wl,-rpath -Wl,<dir>`, are accepted, as are `-Wl,--enable-new-dtags` and `-Wl,--export-dynamic`, which are not sent to the linker because this compiler's own link line says both already: everything else is a flag a package file could use to run code during the build (Go's CVE-2018-6574)
  note: name the library directly with `link` if you need it
```
That first note grows from 52 to 80 words.

**Route (A), last note (variant K's fallback):**
```
t1:  note: name the library directly: `link "SDL3"` in place of `package "sdl3"`, built with `--include /usr/local/include --library /usr/local/lib`; `link` writes no rpath, so the loader must find `/usr/local/lib` when the program runs
t2:  note: name the library directly: `link "curl"` in place of `package "libcurl"`, built with `--include /usr/include/aarch64-linux-gnu --include /usr/include/mit-krb5 --include /usr/include/p11-kit-1`
```

**Route (G), last note (variant R):**
```
t1:  note: name the library directly with `link` if you need it, or pass this word on for one build with `--allow-word -Wl,--enable-new-dtags`
t2:  note: name the library directly with `link` if you need it, or pass this word on for one build with `--allow-word -isystem`
```

## Found beside, not filed, for the coordinator
With the frozen compiler on this Mac, two packages end the build at **exit 2**, *"internal error: linking failed"*:
- `package "libiodbc"`: its `.pc` names `-liodbc`, which this Mac does not have.
- `package "libuv-static"`: it answers `-l:libuv.a`, a GNU ld spelling that names a file. The list admits it through its `-l` prefix, and ld64 refuses it.

I found neither in `issues/`. Defect 224, now closed, was a `link` group's library name read wrongly. By `.claude/rules/verification.md` I read both as `blocking`, an exit 2 where the author could be told.

## What I could not run
- **CI's own leg:** Ubuntu 24.04 x86-64. There is no local image, and `--platform` would pull one.
- **Timing:** the compiler before and after K was not timed.
- **Batch gate:** the full net, the census of `check` over tracked files, the seed and its fixpoint. Nor `records` and `unseen`, which need git.
- **Windows:**
  - The real `package` path there is unrun: the box has no pkg-config, so only my C stand-in reached it.
  - Neither (A) nor (G) was built on the box.
  - I left one incomplete folder, `/c/w/b16-198ce-225`. My first transfer's tar pattern excluded the seed, and the box's rule forbids removing anything there. The good run is in `/c/w/b16-198ce-62060`.
- **Linux:** K's own tests ran there only yesterday, on the earlier K.
- **Not built:** (A)'s note added to K, (G) as an environment variable, (B-ELF).

Files are in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/198-compiler-engineer/`:
- `notes.txt`
- Route K: `tree/selfhost/cli/package_words.hero`, `tree/selfhost/cli/libraries.hero` and `tree/tests/harness/absence.hero`
- Route A: `varA/selfhost/cli/package_way_out.hero`
- Route G: `varG/selfhost/cli/package_allowed.hero`
- The patch scripts that re-apply each route: `scripts/patch_*.py`
- The blind-seat outputs: `blind/run-*/build-output.txt`

---

## The seat's second reply: route (V), built on K

Copied by the coordinator at 11:29 on 2026-10-09 (`date`) from the seat's reply,
verbatim apart from the HTML entities of the notification turned back into their
characters. Asked at 11:08 to build route (V), which the completeness critic's
second pass named (`completeness-critic-pass2.md`), on top of K, and to re-run
the critic's probes and its own census.

# Route V, built on route K: a value the list admits is checked as a value

The critic's finding reproduces on both platforms, and K did not close it. V closes it at no cost to the census. It is now part of the route I recommend: **K+V**.

## What V does
Every admitted word's value goes through one check, in every slot, joined or as the next word: `-D -U -I -L -l -F`, `-framework`, `-isystem`, and the `-Wl,-rpath` and `-Wl,-framework` forms, whole or split. A value must not be empty, must not begin with `-` or `@`, and for `-l` must not begin with `:`. An `@` inside a path, as in Homebrew's `openssl@4`, is still a letter of the path, and a test asserts it.

The refusal names the words it read and says why. For example:

> *the package `probe` answered with `-Wl,-rpath -Wl,@…/rsp4.txt`, which this compiler does not pass on: a value beginning with `@` names a file whose words clang and the linker read in its place*

> *answered with `-l:libuv.a`, which this compiler does not pass on: `-l:<file>` names a file, where `-l` names a library*

## Cost (diff against K)
- **`selfhost/cli/package_words.hero`:** +90 lines, going from 85 to 147 in the layout suite's unit (its line count against the 300 ceiling). It gains `is_value`, `told`, `told_word`, `named` and two small helpers, plus `joined`.
- **`selfhost/cli/libraries.hero`:** +39 −16, going from 298 to 296 against its ceiling of 300.
  - The formatter wrapped the longer refusal calls and took the file to 303.
  - So `allowed_prefixed` moved out of it, whole, into `package_words.hero` as `joined`.
- **`tests/harness/absence.hero`:** +48 lines, one build-level case.
- **Lexer, checker, emitter, runtime:** 0 lines. V is not a core construct.

The `layout` suite itself, run on K+V's tree, reads 5 passed and 0 failed.

## Tests
- **Compiler's own tests:** K+V reads 1436, all passed (K's 1434 plus V's two).
- **Red on the base:** K's code with only V's test added reads 1435 and 1 failed. The failure is V's test, at `assert false`, because `-framework @r` was accepted.
- **Net's own tests (311), from K+V's tree:**

| Compiler under test | Failed |
|---|---|
| K+V | 3 |
| K | 4 (the 3, plus V's case) |
| frozen | 5 (the 3, plus K's case and V's case) |

The 3 common failures belong to my copy: two harness checks fall back to a `./heroes` binary the copy lacks, and one asks git about a copy with no `.git`.

## The critic's probes, before and after
I re-made the probes in my own folder (`vprobe/`); the critic's `probe/` was only read. The evidence is the files a door leaves behind, not the exit code.

**This Mac** (Apple clang 21, ld64):

| Door | frozen | K | K+V |
|---|---|---|---|
| `-framework @f` (writes a map) | exit 0, map written | same | exit 1, refused |
| `-Wl,-framework,@f` (`--ld-path=` a fake linker) | exit 0, **fake linker ran** | same | exit 1, refused |
| `-Wl,-rpath,@f` | exit 0, map written | same | exit 1, refused |
| `-Wl,-rpath -Wl,@f` (split; mine) | exit 0, map written | same | exit 1, refused |
| `-I@f` joined (the critic's control) | exit 0, not expanded | same | exit 1, refused |
| `-isystem @f` (mine) | exit 1 (refused word) | exit 0 | exit 1, refused |
| `libuv-static` (`-l:libuv.a`) | exit 2 | exit 2 | exit 1, told |

Under K+V no door left a file.

**Debian 13** (`sdl3-b14`, clang 22.1.8, GNU ld 2.44):

| Door | frozen and K | K+V |
|---|---|---|
| `-Wl,-rpath,@f` | map written, RUNPATH `/x/lib` | exit 1, refused |
| `-framework @f` | **fake linker ran**, then exit 2 | exit 1, refused |
| `-Wl,-rpath -Wl,@f` (split) | map written | exit 1, refused |
| `-Wl,-framework,@f` | exit 2 | exit 1, refused |
| `-l:file` | exit 2 | exit 1, refused |

Under K+V no door left a file.

## Census (each compiler's own verdict, each package built in its own folder)
- **This Mac, 501 packages:**
  - K: 279 built, 220 refused, 2 at exit 2.
  - K+V: 279, 221, 1.
  - **One package moves: `libuv-static`**, from exit 2 *"internal error: linking failed"* to the told `-l:` refusal.
  - All 220 common refusals print the identical first line, and no build becomes a refusal.
  - `libiodbc` stays at exit 2: its library is absent, which V does not touch.
- **Debian 13, 155 packages:** 154 built and 1 refused (libpsx) under both. **0 move**, first lines identical.
- **No census answer has a value beginning with `@`.** V's price lies outside the census: a hand-written `.pc` with `-Wl,-rpath,@loader_path/…` (an ld64 rpath macro) is now refused. I did not run whether ld64 would ever read such a value as a file, so V refuses it by construction.

## On my verdict, and two corrections
- **Recommended route:** K+V. V stands alone and fits beside any route A to J. The vetoes on (D), (E) and (H) stand.
- **design.md is false today:** `docs/design.md:2639-2642` claims *"a `.pc` answering `@…` is `ffi_package` at exit 1"*. That is false on the frozen compiler and true again under K+V. The correction is owed to design.md, which is not mine to edit.
- **My own over-claim, corrected:** K's comment in `package_words.hero` said *every* header under `/usr/include/mit-krb5` compiles under `-I`. I had compiled seven named headers. The comment now says exactly that, and that `gssapi/gssapi_alloc.h` fails under `-I` but none of the seven includes it (the critic's finding). After the edit, check and `layout` (5/0) re-ran clean. The compiler binary predates the edit, which changed only a comment.
- **A slip of mine, checked:** at 11:11 a broken `&&` chain ran `heroes check` and a line counter with the trunk's root as the working directory. No trunk file changed after 11:10:50 (checked with `find`, outside `.git`), and `git status` there is clean. That `git status` itself may refresh the trunk's index.

## Not run
- V on the Windows box.
- The CI's native Ubuntu 24.04 x86-64 leg.
- Any timing.
- The batch gate: full net, census of `check`, seed and fixpoint.

Files are in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/198-compiler-engineer/`:
- `notes.txt`
- K+V's tree: `treeKV/`, with `treeKV/selfhost/cli/package_words.hero`, `treeKV/selfhost/cli/libraries.hero` and `treeKV/tests/harness/absence.hero`
- The compilers: `heroes-kv` (Mac) and `lx-bin/heroes-kv` (Linux)
- The patch scripts: `scripts/patch_v.py` and `patch_v2.py`
- The probes: `vprobe/` (`mac-*.txt`, `linux-*.txt`)
- The census tables: `census/gt/*.tsv`

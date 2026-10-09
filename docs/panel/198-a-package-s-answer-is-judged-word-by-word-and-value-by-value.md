# Panel 198: a package's answer is judged word by word and value by value

Convened 2026-10-07 by the coordinator for defect 444 (`systemic`, it blocks
the milestone's tag): SDL 3.2.10 built from its source with `SDL_RPATH` at its
default answers `-Wl,--enable-new-dtags`, and `package "sdl3"` is refused. The
critic's first pass widened it to the whole class of words a correct package
answers and the list refuses (Q1: `-pthread`, `-isystem`, `-Wno-*`, gmodule's
and libpsx's `-Wl,` words). **A full panel** (the critic's first pass, § 1:
every route changes the `ffi_package` note, now the rule's only statement a
reader meets, and a word the list admits changes which programs `build`
accepts): the compiler-engineer, the ffi-pragmatist, the spec-warden, the
historian, the blind seat as six fresh `claude -p` sessions outside the
repository, and the completeness critic before the seats and after them. The
tree frozen at **`56def9b4`**, worktree `lane-panel-198`. Briefs written from
23:42 on 2026-10-07, read by the critic from 01:44 to 01:57 on 2026-10-08 and
repaired from 08:02 to 08:10; the seats from 08:11, stopped by the account's
session limit at about 08:25 and resumed at 09:50; the spec-warden's report
copied at 08:24, the historian's at 09:57, the ffi-pragmatist's at 09:58; a
reboot at about 11:18 emptied the session scratchpad and took the
compiler-engineer's copy, prototype and census with it; resumed at 09:48 on
2026-10-09 inside the repository's root (the author's rule of 2026-10-08), it
re-measured everything by 10:12; the blind seat's six sessions from 10:47 to
10:50, 1.5732 USD of the 6 the author approved; the critic's second pass from
10:52 to 11:06; route (V), which that pass named, built by the
compiler-engineer from 11:08 to before 11:29; this synthesis from 11:29,
every time read from `date`. Briefs in `198-briefs/`, reports in
`198-reports/`, each marked with how it was copied (a subagent's Write of a
report file is refused).

## The question, from the shared brief

**Which words may a package's `pkg-config` answer carry, and how is one
admitted?** Each word of the class admitted, refused or dropped, by what test,
and what the refusal's note tells the person whose correct program it stops.
The list is `selfhost/cli/libraries.hero`'s `filter_words`, its property *does
a word name a file, load anything, or write anything at build time?*, its
precedent Go's CVE-2018-6574; the spec has said nothing of it since panel
192's r1 (`d0409cc5`). The routes (A) to (J) of `198-briefs/00-shared.md`,
widened by the sitting: K, the compiler-engineer's, and (V), the critic's
second pass.

## The verdict table

| seat | verdict | on what |
|---|---|---|
| compiler-engineer | **approve K**, its own; **veto** (D), (E) and (H) as Go ships it on soundness; **object** to (A) alone, (B), (B-ELF) and (G) | K built and gated on this Mac, the Windows box and an Ubuntu 24.04 source build of SDL; (A) and (G) built; a census of three machines |
| ffi-pragmatist | **object** to (F) as stated; recommends (B-asym) done as a drop; leaves `-pthread` and `-isystem` open | every word linked on ld64, GNU ld 2.42 and 2.44, LLD 22 and lld-link 23; Q5's `dlopen` run; the SDL source build end to end |
| spec-warden | **no spec sentence**, a provisional veto on restoring one; approve (A), (B-ELF), (B-asym), (C), (F), (J) at 0 tokens; object to (B), (D), (E), (G), (H) on what their note becomes; the note's reason clause false | the spec re-measured, eight drafts priced on the vendored row |
| historian (advisory) | **object** to (D), (E) and to the note's appeal to Go; admitting by exact name has precedent | Go's list and its five later advisories, Rust's `pkg-config` crate, Meson, CMake, Zig, SwiftPM, Nix, SDL's own commits |
| blind seat | six readings, **six build and run, none weakens the list**; both readers offered (G)'s allowance refused it | `llm-ergonomist-scoring.md` |
| critic, second pass | the admitted words' **values** are a response-file channel in the frozen compiler and in K alike; route (V) unlisted; K's `-isystem` as `-I` safe for every entry header measured; K unrun on the CI's native leg | its own copy, probes on this Mac and Debian 13 |

## What the sitting measured

- **The fault, reproduced**: on Ubuntu 24.04 arm64 with SDL 3.2.10 built from
  source (`SDL_RPATH` ON), `pkg-config` answers `-I/usr/local/include
  -L/usr/local/lib -Wl,-rpath,/usr/local/lib -Wl,--enable-new-dtags -lSDL3`
  and the frozen compiler refuses both SDL programs; K builds them and `run
  sdl3` reads 1 passed (the compiler-engineer, the ffi-pragmatist).
- **The class (M4, the census)**: on Debian 13, 23 of the packages that answer
  are refused, 13 for `-pthread`, 9 for `-isystem /usr/include/mit-krb5`
  (`libcurl` among them), gmodule's two for `-Wl,--export-dynamic`, libpsx for
  its whole-archive bracket; on this Mac 221 of 501, 219 `absl_*` for
  `-Wno-*`, whose API headers do not parse as C. **Under K, Debian refuses 1
  (libpsx), this Mac 220, the Ubuntu reproducer 0** (the compiler-engineer).
- **The dtags (M3, Q5)**: `--enable-new-dtags` is byte-identical to no word on
  Debian GNU ld 2.44 (arm64 and emulated x86-64), LLD 22.1.8 and Ubuntu GNU ld
  2.42 (arm64, and its x86-64 cross build); ld64 refuses it, lld-link warns.
  Upstream GNU ld defaults to RPATH, and Debian configures it to RUNPATH
  (`debian/rules.defs:52`, bug #835859), so on a GNU ld built with upstream's
  default the word would move the tag. `--disable-new-dtags` moves it
  everywhere measured, and Q5's run shows the move changes what a `dlopen`ing
  library finds (the ffi-pragmatist).
- **`-pthread`** is a driver spelling of `-D_REENTRANT` (`_MT` on Windows) and
  `-lpthread`, both already admitted; it names no other file, loads nothing,
  writes nothing (the ffi-pragmatist). On Windows a link warns *argument unused
  during compilation*, so K passes it to the link everywhere but there (the
  compiler-engineer).
- **`-isystem`**: the compiler's own questions to clang about a header
  (`ffi_parameter_type`, `ffi_return_type`, `ffi_macro_name`) fire identically
  from a system directory (the ffi-pragmatist); what a system directory silences
  is a warning inside the header's own code. K hands the directory on as `-I`,
  so no warning is turned off for a header (`ffi_header_refused`'s promise,
  panel 188's premise). The six entry headers a binding names compile under
  both; `gssapi/gssapi_alloc.h` fails under `-I` and is reached by none of them
  (the critic, correcting the engineer's *all seven headers*).
- **The Windows rpath**: the admitted `-Wl,-rpath,<value>` names a file on
  lld-link, which warns on `-rpath` and links the value as an input (`h.o`
  given only as the rpath's value, the program prints `7`); K leaves the rpath
  out on Windows and the link refuses the undefined symbol (the
  compiler-engineer, the ffi-pragmatist).
- **The response file (the critic's second pass)**: no admitted word's value is
  checked for `@`. A `.pc` answering `-Wl,-framework,@rsp`, `rsp` holding `X
  --ld-path=<program>`, runs that program as the linker during `heroes build`
  at exit 0 on this Mac; `-Wl,-rpath,@rsp` and `-framework @rsp` make clang
  read `rsp` as options on this Mac and Debian 13; `-I@rsp` joined is not
  expanded. The same list admits `-l:<file>` through the `-l` prefix. This
  falsifies `docs/design.md:2639-2642`'s claim that panel 050's allow-list
  holds against a `.pc` answering `@…`, in the frozen compiler and in K alike.
  Filed as defect 527 (`blocking`).
- **(V), built** (the compiler-engineer, 11:08 to before 11:29): every
  admitted word's value goes through one check in every slot, joined or as
  the next word: not empty, not beginning with `-` or `@`, and for `-l` not beginning with
  `:`; an `@` inside a path (Homebrew's `openssl@4`) stays a letter of the path.
  `package_words.hero` +90 lines (147 in `layout`'s unit), `libraries.hero` +39
  and -16 (296 of 300, `allowed_prefixed` moved out whole as `joined`),
  `absence.hero` +48; the compiler's own tests 1,436 passed, V's test red on K
  (`-framework @r` accepted). The probes re-made in its own folder: on this Mac
  every door the frozen compiler and K leave open (a map written, a fake linker
  run) is refused at exit 1 and leaves no file, and `libuv-static` moves from
  exit 2 to the told `-l:` refusal; on Debian 13 the same, five doors. The
  census: on this Mac one package of 501 moves, `libuv-static`, and the 220
  common refusals print an identical first line; on Debian 13 none of 155
  moves. No census answer has a value beginning with `@`; V's price lies
  outside them: a hand-written `.pc` answering `-Wl,-rpath,@loader_path/…`, an
  ld64 rpath macro, is refused, whether ld64 would read it as a file unrun.
- **(D)** links libpsx at exit 0 with an archive's constructor never run; **(E)**
  admits `-Wl,-Map=x` and `-Wl,--dependency-file=x`, which write files named by
  values with no slash, and `-Wl,@resp.txt`; **(H)**, Go's list, lets a
  package's `-Wno-X` after the compiler's `-Werror=X` turn four errors into
  exit 0, one of them panel 103's `incompatible-pointer-types` guard (the
  compiler-engineer).
- **(G)**, built as `--allow-word`: `compile.hero` and `produce.hero` past 300
  lines, one word per build (a repeatable flag must be a search path), and the
  person must name the step that takes the word, which is what the list
  encodes (the compiler-engineer). Both blind readers offered it refused it.
- **(A)**, built: the note computed from the answer (`link` per `-l`,
  `--include` per header directory, `--library` per `-L`, a run-time warning
  for an rpath); followed as written it builds and runs both tasks; alone it
  still refuses 22 of Debian's 23 correct programs.
- **The blind seat**: today's message already leads a reader to `link` on both
  tasks; the readings do not tell K's note from M's, since K builds both
  tasks in silence and nobody reads a K note; they show that (G)'s allowance,
  offered beside `link`, was not taken. Both tasks sit on machines where
  `ldconfig` had run, so the case where `link` loses a needed rpath is
  unmeasured.
- **Precedent**: Go admitted both dtags words, `--as-needed` and `-isystem` on
  2018-02-14, thirteen days after its first list, and `-pthread` from the
  first; its five later advisories were a wildcard (`-lto_library` under `-l`),
  next-word smuggling (CVE-2023-29404), `@` indirection (CVE-2025-22867, a
  widening that was itself the CVE) and a file-reading word (`-sectcreate`,
  open as #78750). Rust's crate, Meson and CMake pass every word; Zig drops
  unknown ones; SwiftPM restricted dependencies and reversed it in 2025 (the
  historian).

## Disagreements, stated plainly

- **(F) or a drop of one word.** The brief's (F) dropped both dtags words; the
  ffi-pragmatist measured that dropping `--disable-new-dtags` changes what a
  `dlopen`ing library loads. K drops `--enable-new-dtags` alone (a no-op on
  every toolchain the project ships on) and keeps `--disable-new-dtags`
  refused, which is the ffi-pragmatist's own recommendation.
- **`-pthread` and `-isystem`.** The ffi-pragmatist left them open, not settled
  by its seat; its own measurements (a `-D` and an `-l`; the compiler's header
  questions unchanged from a system directory) are the ground K admits them
  on, `-isystem` as `-I` so that nothing the compiler promises about a header
  moves. The engineer's sentence that *all seven headers compile under `-I`*
  was false as written and the critic measured what holds: every entry header
  a binding names.
- **(G).** The historian found Go's override in its first commit and routine
  since; the compiler-engineer, the spec-warden and both blind readers stand
  against it, the readers because the note's own CVE clause turned them away.
  Refused: the way out for a word the list refuses stays `link`, which widens
  nothing.
- **The note's reason clause.** The spec-warden read it false for the refused
  words (`--enable-new-dtags` and `-pthread` run no build-time code); the critic
  found that the measured build-time code runs through the admitted words'
  values. Both are corrected: the clause is reworded and (V) closes the
  channel.
- **ld64 and `@`.** The ffi-pragmatist's edge note read ld64's `@file` door as
  closed inside a `-Wl,` value; the critic measured a fake linker run through
  `-Wl,-framework,@rsp`, which the compiler hands on as `-framework @rsp`.
  The
  engineer's re-made probes settle it: the fake linker ran through
  `-Wl,-framework,@f` on this Mac and through `-framework @f` on Debian 13,
  under the frozen compiler and K alike.
- **Principle 0.** The spec-warden finds no measured thesis effect for a
  sentence, and the resolution writes none; a word of the list is not a form of
  the language and costs no spec token, so Principle 0 does not bar K.

## The resolution, ratified by the author (below)

The most robust and complete route at every disagreement (CLAUDE.md § 4,
CL-040); what conservative would have been is below the list.

1. **R1, route K: the list admits, by whole word, what the census shows
   correct C libraries need.** `-pthread` (to the compile everywhere, to the
   link everywhere but Windows); `-isystem <dir>`, joined or as the next word,
   handed on as `-I<dir>`; `-Wl,--enable-new-dtags` and `-Wl,--export-dynamic`
   accepted and not sent to the linker, because the compiler's own link line
   says both already; `-Wl,-rpath,<dir>` not sent to lld-link. Still refused:
   `--disable-new-dtags`, the position-dependent words, `-W…`, `-Xlinker` and a
   comma-tunnelled word. The engineer's modules as built,
   `selfhost/cli/package_words.hero` and the lines in `libraries.hero`, with
   `eac3e5b5`'s rpath argument and the answer (c) of 2026-08-15 restored as
   comments beside the list (Q6: run-time properties stay outside the list's
   test, cited, not reopened).
2. **R2, route (V): every admitted word's value is judged as a value.** A value
   beginning with `@` is refused in every slot, joined and next-word, `-D`,
   `-U`, `-I`, `-L`, `-l`, `-F`, `-framework`, `-isystem` and the `-Wl,` forms;
   `-l:<file>` is refused. A value must not be empty or
   begin with `-`; an `@` inside a path stays a letter of it; the refusal names
   the words it read and why (*a value beginning with `@` names a file whose
   words clang and the linker read in its place*; *`-l:<file>` names a file,
   where `-l` names a library*). The engineer's `is_value`, `told`, `joined` as
   built. Its known price, refused by construction: `-Wl,-rpath,@loader_path/…`
   in a hand-written `.pc`. It closes defect 527 and makes
   `docs/design.md:2639-2642` true again, which the landing corrects
   underneath with its date.
3. **R3, the note**: its list is the code's, word for word, and a compiler test
   compares the two (the spec-warden's finding 3, an executor where there was a
   comment); its reason clause reworded so it is true of every refused word:
   *everything else is refused, because a word the list does not name could be
   one that runs code during the build (Go's CVE-2018-6574)*; its last line
   stays *name the library directly with `link` if you need it*.
4. **R4, the spec: no sentence**, the spec-warden's verdict; 0 tokens.
5. **R5, the cases**: K's four compiler tests and two `absence.hero` cases;
   (V)'s two compiler tests, red on K, and one `absence.hero` build case; the
   critic's probes, each slot's `@` refused, as compiler tests; the RUNPATH
   premise case (*an admitted -Wl,-rpath leaves a RUNPATH*), which is the
   falsifier K's drop rests on.
6. **R6, the CI (Q8)**: `-DSDL_RPATH=OFF` and its comment leave
   `.github/workflows/ci.yml`, so the CI's native Ubuntu 24.04 x86-64 leg, the
   one toolchain K never ran on, builds SDL with the word and runs `run sdl3`
   as the measurement.
7. **R7, the landing**: in batch 16, a lane of its own on the round, its
   gate the batch's (seed, fixpoint, full net, census), the engineer's package
   census re-run with the landed compiler on this Mac and Debian 13, then the
   platforms: Linux arm64, the Windows box, the CI's four legs.
8. **R8, the cards**: 444 closes at the landing after the CI's legs; 527 closes
   with R2; 528's `libuv-static` half is told under R2, and its `libiodbc`
   half, a library the machine lacks, is not this route and keeps 528 open;
   libpsx's whole-archive bracket stays refused, named in 444's closing as the
   one Debian package K still refuses.
9. **R9, refused**: (D), (E) and (H) as Go ships it, vetoed on soundness; (A)
   alone, which refuses 22 of 23 correct Debian programs; (B), a raw ld64
   failure and an lld-link warning on a correct program; (B-ELF), a platform
   axis `flags.hero:162-167` refuses; (F) as stated, which changes what a
   `dlopen`ing library loads; (G), for the reasons above; (C) is K narrowed to
   the census, and (J) stays the note's way out for what the list refuses.

**The conservative alternative, the author's to choose instead**: (A) with (V),
the list as today with its note computed from the answer and the response-file
channel closed; Debian's 22 correct programs stay refused and the CI keeps
`-DSDL_RPATH=OFF`. A narrower alternative between the two: K without
`-isystem`, which keeps panel 188's premise by refusal and `libcurl` refused
on Debian. And one narrower than R2, unbuilt: admit ld64's own rpath macros,
`@loader_path`, `@executable_path` and `@rpath`, as the leading word of an
rpath's value on macOS, which no package of either census answers and which
would need ld64's reading of such a value measured first.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | with `-DSDL_RPATH=OFF` removed, the CI's Ubuntu 24.04 x86-64 leg reads `run sdl3` 1 passed and 0 failed and the RUNPATH premise case passes; a fresh Debian 13 census refuses exactly libpsx | the landing's CI legs |
| spec-warden | P-tok moot, no sentence; P-rw (readers with m2 need as many builds as without) unscored, no m2 arm ran | n/a |
| coordinator | the blind seat's four clauses | **two held, one half falsified, one falsified**: no M reader wrote a needless `--include`, and both R readers refused the allowance (`llm-ergonomist-scoring.md`) |
| historian | an advisory naming `--enable-new-dtags`, `--disable-new-dtags`, `--as-needed`, `--export-dynamic`, `-pthread` or `-isystem` would turn it against admission by name | open |

## Author's verdict

**RATIFIED, 2026-10-09**, R1 to R9 as written above, the author answering
through the question widget between 11:30 and 11:38 by the clocks read before
the question and after the answer, choosing *ratify R1-R9* over the
conservative alternative, over K without `-isystem` and over *I want to read
it first*, on the coordinator's summary of the route and its price. Recorded
as a reading (CLAUDE.md § 4). The author may overturn it. The ratification
issue is
`issues/2026-10/09/2026-10-09-1130-panel-198-ratify-amend-or-overturn-r1-to-r9-a-package-s.md`.

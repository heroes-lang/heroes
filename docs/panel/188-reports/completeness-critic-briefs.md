# Panel 188, the completeness critic's first pass: the briefs

Written 2026-10-03 by the completeness critic, over every file of
`docs/panel/188-briefs/` (`00-shared.md`, `00-facts.md`,
`compiler-engineer.md`, `ffi-pragmatist.md`, `spec-warden.md`,
`historian.md`, `llm-ergonomist.md`, `blind/a-brief.md`, `blind/b-brief.md`,
`blind/p.hero.txt`, and `completeness-critic.md`, my own). No verdict on the
routes. No paid run: no `claude -p`, no API call, no `heroes measure
--refresh`, and no web search or fetch either (both bill per use), so the
standard texts below stay questions for the historian. Written as I go; in
progress until the closing section says otherwise.

`<scratchpad>` below is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`,
and `<copy>` is `<scratchpad>/188-critic/`. Every case I ran is under
`<copy>/work/`, run by `<copy>/run_cases.sh` (check, build, run, each exit
code), with `HEROES_RUNTIME=<copy>/runtime`. Where the compiler's own words
hold an em dash, this report writes a comma in its place (the house rule), so
those quotations are exact but for that.

## The finding that changes the framing, first

**The "false message" face of F1 is not clang failing on `\`, `"` or a line
end. The compiler writes the Heroes string's SOURCE SPELLING, escapes
included, into the `#include` line, not the string's value.** F1's own build
trees show it (`<scratchpad>/188-facts/case-*/build/tu-*/p.c`, line 5, and the
`build/reads/reads-*.c` probe, line 3):

| `p.hero` line 1 (bytes, `od -c`) | the string's value by spec § 2 | the line the compiler emitted |
|---|---|---|
| `extern "a\\b.h"` | `a\b.h` (one backslash) | `#include <a\\b.h>` (two) |
| `extern "a\"b.h"` | `a"b.h` | `#include <a\"b.h>` |
| `extern "a\nb.h"` | `a`, a line feed, `b.h` | `#include <a\nb.h>` (a backslash and an `n`) |

The code says why: `selfhost/emit/externs.hero:61-67`, `unquoted`, *"The
quotes stripped from a span's text"*, feeds `headers` (line 75) and so
`c_text.includes`; and the parser's `machine_locked` reads
`module_text.unquote` (`selfhost/parse/module_text.hero:74-83`), which also
strips the two quotes and nothing else. **No reader of a header string
decodes its escapes**, while `spec/heroes-spec.md` § 2 (line 47) gives a
string exactly six: *"`\n` `\t` `\r` `\\` `\"` in a string"*, and § 13's
production makes the header a `string`.

And this Mac's clang (Apple clang 21.0.0, `clang --version`) on the VALUE,
in `<copy>/work/clang-direct/`, with the two headers copied from F1's case
folders, `clang -std=c11 -I. -Wall -Wextra <f>.c -o <f>.bin`, each unit
`#include <...>` then `int main(void){return seven()-7;}`:

| the `#include` line | clang | the program |
|---|---|---|
| `#include <a\b.h>` (the value) | exit 0, **no warning** | exit 0 |
| `#include <a\\b.h>` (what heroes wrote) | exit 1, *'a\\b.h' file not found* | not built |
| `#include <a"b.h>` (the value) | exit 0, **no warning** | exit 0 |
| `#include <a\"b.h>` (what heroes wrote) | exit 1, *'a\"b.h' file not found* | not built |

So:

- **False as stated**: route (1a)'s premise, *"refuse what this Mac's clang
  fails on: `>`, a line end, `\`, `"` (F1)"* (`00-shared.md` lines 39-40).
  This Mac's clang does not fail on `\` or `"`; it fails on the two-byte
  spellings the compiler wrote in their place. Of F1's four failing names,
  only `>` and a line end are names `#include <...>` cannot carry at all.
- **True as an observation, false as a cause**: `00-facts.md` lines 32-34,
  *"`build` says the header is missing where it exists"*, which the briefs
  then read as a limit of C or clang (route (1a)). Clang was asked for
  `a\\b.h`, which does not exist; the file the string names, `a\b.h`, does.
  The message names the spelling (`a\\b.h`) and is true of the file the
  compiler asked for and false of the one the author named: CL-078's shape,
  the defect as the finder saw it.
- **A question the sitting does not ask** (written out in § Missing): is a
  header string's meaning its value (spec § 2) or its spelling (the
  compiler)? Panel 055 already met the edge of it: *"'a group names its
  header' never says the string is a C include spelling rather than a
  filesystem path"* (`docs/panel/055-where-a-header-is.md:32-33`).

## My copy and my compiler

| fact (where) | command | result |
|---|---|---|
| the copy is the frozen head (`00-shared.md:3`, `completeness-critic.md:13`) | `git -C <trunk> archive 826ddc2f \| tar -x -C <copy>`; `git -C <trunk> rev-parse HEAD` | HEAD `826ddc2f06a5...`; **verified** |
| the seed's sha256 begins `2c809845ed0f7ba8` (`00-shared.md:80`) | `shasum -a 256 seed/heroes.c` | `2c809845ed0f7ba8bc98...`; **verified** |
| the compiler built from it, `afc05be6b2c50184` (`00-shared.md:81`) | `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`; `shasum -a 256 heroes` | `afc05be6b2c5018453...`, and the trunk's `./heroes` (mtime 13:43) hashes the same; **verified** |
| rebuilding from `selfhost/` *"takes about a minute"* (`00-shared.md:82-83`) | not run: my brief and the shared one forbid timing | **unverified by me**; the panel skill's § 2 records `real 60.97 s` for that rebuild from the plain seed, an earlier day's measurement, so the sentence is carried, not run |

## `00-shared.md`

| line | fact | command | result |
|---|---|---|---|
| 3 | the trunk at `826ddc2f` | `git log --oneline -3` on the trunk | HEAD is `826ddc2f`; **verified** |
| 10-13 | defect 216 is `blocking` in `docs/work/DEFECTS.md`; `>` passes `check` at 0 and stops `build` at 2 with an internal error and clang's text | `sed -n 237,242p docs/work/DEFECTS.md`; `<copy>/work/case-gt` re-run | item 216 at line 237, `· **class: blocking**`; check 0, build 2, *internal error: compiling the generated C failed*, clang's `#include <a>b.h>`, *'a' file not found*, *error: clang refused the generated C*; **verified** |
| 12-13 | a refusal at `check` is a diagnostic class, so a sitting's (CLAUDE.md § 4) | read CLAUDE.md § 4 | *"a diagnostic **class**"* is among § 4's triggers; **verified** (that the repair IS a refusal at `check` is the defect item's own reasoning, line 242, and is a route, not a fact; see § Leans) |
| 13-15 | *"The author convened this one in full on 2026-10-03, with the blind seat's one paid session capped at 3 USD (the author's answer 1a)"* | `git grep -n -i '3 USD\|answer \*1a\*\|answer 1a' 826ddc2f -- docs`; `grep -n '188\|216' docs/work/DECIDE.md`; the 2026-10-03 log entries listed | no tracked file records it (the `3 USD` hits are panel 184's); **unverified by me**, the coordinator's report of a conversation. **And it contradicts `llm-ergonomist.md` lines 5-6**, *"two sessions of at most 1.5 USD each"*: *one* paid session and *two* sessions cannot both be the author's words |
| 17-21 | the class has two faces; six other names work | the thirteen cases re-run on my compiler (below, F1) | the exit codes and messages: **verified**. The reading of the second face: **false as a statement about clang** (§ The finding first) |
| 21-25 | `extern "<stdio.h>"` stops `build` at 2 with an internal error; `extern "stdio.h>"` builds at 0 | F6's three cases re-run | **verified**; and *"the plausible mistake"* is an inference: see F6 below |
| 29-33 | C11 6.4.7 as recalled | not run (no web access under the no-paid-run rule) | **unverified, and correctly marked a question**; the only citation of 6.4.7 in the tree is `selfhost/emit/c_text.hero:14`, *"A header name has no escapes (C11 6.4.7)"* (`grep -rnF '6.4.7' docs selfhost .claude spec`) |
| 39-40 | (1a) *"refuse what this Mac's clang fails on: `>`, a line end, `\`, `"` (F1)"* | `<copy>/work/clang-direct/` | **false for `\` and `"`**: this Mac's clang compiles both names silently under `-Wall -Wextra`; it fails on `a\\b.h` and `a\"b.h`, what heroes wrote |
| 41-43 | (1b)'s list, *"(as recalled above)"* | as 29-33 | a question until the historian answers |
| 44-46 | (1c), its cost the real headers it refuses | surveyed by the ffi-pragmatist; see § Missing for two counts I ran | route definition, not a fact |
| 52-53 | `machine_locked_path` *"saying where the machine keeps it"* | `<copy>/work/format/abs.hero`, `heroes check` | the message reads *"... says what the machine has, **not where this machine keeps it**"*: **not verbatim** (*the* for *this*, and in the message the phrase is negated); harmless as a paraphrase, but it is set in italics like a quotation |
| 55-57 | `.claude/rules/diagnostics-and-goldens.md` says a `certain` fix repairs the defect the diagnostic names | `grep -n` that file | line 23, *"A `certain` fix repairs the defect the diagnostic names"*; **verified** |
| 54-55 | a `\` written for a `/` *"reads as a separator there [Windows] and as a byte of a name elsewhere"* | Windows offline; the emission read | **unrun on Windows**; and the premise needs the value reading: today the source `"sys\\types.h"` reaches C as `sys\\types.h`, TWO backslashes, on every platform (`emit/externs.hero:61-67`) |
| 59-61 | `machine_locked` in `selfhost/parse/group_head.hero`; the budget 8,696 with 7 of room | F2 and F3 below | **verified** |
| 62-63 | `.claude/rules/diagnostics-and-goldens.md` § A new surface form | `grep -n '^## '` that file | line 96, *"## A new surface form lands in every tool that reads the language"*; **verified** |
| 65-67 | § 13 says nothing of what a name may hold, nor of absolute paths | F4 below | **verified, and wider than stated**: neither `absolute` nor `machine` occurs anywhere in the spec |
| 69-70 | a `link` and a `package` string reach a linker and `pkg-config`, not an `#include` | `grep -rn '"-l" +\|"--cflags"' selfhost` | `selfhost/cli/link.hero:144`, `"-l" + name`; `selfhost/cli/libraries.hero:294`, `["pkg-config", "--cflags", "--libs", package]`; **verified**. Both names are read by `emit/externs.hero`'s `unquoted` too (lines 134, 155), so they also travel as their spelling |
| 75-81 | the scratchpad path; the copy recipe; the hashes | § My copy | **verified** |
| 84 | F1's cases at `<scratchpad>/188-facts/` | `ls` | 13 case folders, ten F1 and three F6; **verified** |
| 87-88 | the Windows box is offline | `tailscale status`, `ssh -o ConnectTimeout=20 -o BatchMode=yes win true`, 16:12 | *"windows offline, last seen 1h ago"*; ssh timed out, exit 255; **verified** |

## `00-facts.md`

| line | fact | command | result |
|---|---|---|---|
| 3 | measured between 14:50 and 15:50 | `stat -f '%Sm'` of the case files | F1's files 14:54:15-16, F6's 15:48:48-49; inside the window; **verified** |
| 4-6 | F1 ran on the trunk's compiler at `e339ece9`; `git diff --stat e339ece9 826ddc2f -- selfhost seed runtime tests spec` is empty | that command, `\| wc -l` | `0`; `e339ece9` committed 14:48:51; **verified** |
| 10-17 | F1's method: a header holding `static inline int32_t seven(void) { return 7; }` over `<stdint.h>`, a program binding it, check, build, run, escapes `\\`, `\"`, `\n` | `cat`, `cmp` of every header against `case-plain/ab.h`; `od -c` of each `p.hero`'s line 1 | all nine headers byte-identical to the stated text; the three escaped names are spelled as stated; **verified** |
| 19-30 | the ten rows | `<copy>/run_cases.sh` over copies of the ten folders | `ab.h`, space, `'`, `d//b.h`, `e/*b.h`, `a??)b.h`: 0, 0, prints `7`; `a>b.h`: 0, **2**; `a\b.h`, `a"b.h`, line end: 0, **1**, `ffi_missing_header`; every exit code and message as stated; **verified** |
| 26 | `a??)b.h` builds by defect 207's line splice | the emitted `p.c` | `#include <a?\` over `?)b.h>`; **verified** |
| 32-34 | *"build says the header is missing where it exists"* | `<copy>/work/clang-direct/`; the emitted `p.c` | the observation is true; **the cause it implies is false**: clang was asked for the spelling (§ The finding first) |
| 34-35 | both faces `blocking` by § Bounded discovery | `.claude/rules/verification.md:418` | *"an exit 2 where the author can be told, a false message"*; **verified** |
| 37-38 | Windows offline since about 14:47, *last seen 54m ago* at 15:41 | `tailscale status` at 16:12 | a past reading I cannot re-take; mine, *last seen 1h ago* at 16:12, agrees |
| 38-40 | unrun: a NUL (*"whether a Heroes string can carry one is a question"*), a byte above ASCII | spec § 2 line 47, § 1 lines 35-36; `<copy>/work/beside/raw-nul`, `non-ascii` | **now run** (§ Shapes beside): no escape writes a NUL, but § 1 (lines 35-36) lets a string hold *"any [UTF-8] but a raw carriage return or line end"*, U+0000 is UTF-8, and a raw NUL passes `check` at 0 and stops `build` at **2** with an internal error; `é.h` builds and prints 7 |
| 42 | F6 measured at *"15:58"* | `stat` of `case-angle-*` | the files were written at **15:48:48**, and `00-facts.md` itself last at 15:49:04; **false** (15:48, by the files) |
| 44-45 | F6 binds `puts(s: cstr lent) -> i32`, no header beside | `cat`, `ls` | **verified** |
| 47-51 | F6's three rows | re-run | `"<stdio.h>"` 0, **2**; `"stdio.h>"` 0, **0**, prints `hi`; `"<stdio.h"` 0, 1, `ffi_missing_header`; **verified** |
| 53-55 | *"the spelling a model fluent in C reaches for"* | the census below; `grep -rlE 'extern "<[a-z]' docs/` | **an inference, unmeasured**: of 832 `extern "..."` lines (286 distinct names) in the tree's `.hero`, `.md` and `.txt` files, none writes brackets inside the quotes, and the two `extern "<header>"` hits are comments using a placeholder (`selfhost/emit/clang_place.hero:142`, `tests/harness/suite_records.hero:2604`). No record shows a model writing it. Plausible, not measured; the blind A/B measures repair, not occurrence |
| 56-57 | unrun: whether `"stdio.h>"`'s build drew a clang warning | `<copy>/work/case-angle-stdio.hG/out-build.txt` | **now run**: `build` prints clang's *warning: extra tokens at end of #include directive [-Wextra-tokens]* twice, once from `build/pointee-*/check-*.c` and once from the unit, then *wrote p*, exit 0. Other platforms unrun |
| 61 | `group_head.hero` 164 lines by `wc -l` | `wc -l` | 164; **verified** |
| 61-67 | `header` at 30 bumps the string and calls `machine_locked`; comment from 60; function at 72; leading `/`, `\`, `:` or a second byte `:`; `machine_locked_path`; no fix; the quoted sentence | read lines 30-99 | lines 30-33, 60, 72, 81-86, 96, 69-71; **verified** |
| 67 | *"Nothing else judges the header's characters before `build`"* | every reader of a header span outside `emit/` (`grep -rn 'spans\.header\|rec\.header\|\.header\b\|header:' selfhost`) | ast, handles (`in_group`), resolve (presence), ir/verify (presence), parse (`group_head`, `group`, `tails`), print (`dump`, `anchors`, re-printing): none judges a character; **verified on that search**. And a sibling the brief does not name: the parser's own `unquote` judges the spelling, not the value |
| 68-70 | `c_text.hero`'s `includes` at 99; the splice comments at 15 and 110 | read | **verified**; the `#include <` text itself is built at line 113, `header_lines` |
| 71-72 | `ffi_missing_header` built at `ffi_build.hero:205` and `header_reach.hero:96` | `grep -rn 'ffi_missing_header' selfhost`, comments excluded | those two, plus two test asserts (`header_reach.hero:158`, `ffi_build.hero:339`); **verified** |
| 76-77 | `suite_layout.hero` line 490, `"selfhost/parse/ 8696"`, panel 187's R9 | `sed -n 490p`; `grep -n 'R9'` in panel 187's file | **verified** (R9 at line 210 of the sitting's file) |
| 77-79 | 8,689 over 54 files, 7 of room, by the mirror | the mirror copied to `<copy>/work/code_lines.py` (not run in place), compared with `suite_layout.hero:780-799`, then run | the three rules are the suite's; `selfhost/parse/` has no subdirectory, so `*.hero` is what the suite's prefix match sums; **8,689, 54 files, 7 of room; verified** |
| 80-81 | the `budget` check passed at the sixth gate `da3e29af` | `git log -1 da3e29af`; `git diff --stat da3e29af 826ddc2f -- selfhost/parse tests/harness/suite_layout.hero`; `./heroes run tests/harness/main.hero -- ./heroes layout` whole in my copy, to a file | the body reads *"layout 5"* within *"5,033 passed and 0 failed"*, not `budget` by name; nothing it reads moved since (0 lines); my whole run reads *"layout: 5 passed, 0 failed"*, exit 0, and the whole run is the one that asks `budget` (`suite_layout.hero:631`); **verified** |
| 81-83 | `layout/budget`'s message | `grep -n` | line 658, verbatim; **verified** |
| 85-86 | `cl100k_base` 7,117, `claude-legacy` 6,990, real 9,392 (`claude-opus-5`, 2026-10-03) | `env -u ANTHROPIC_API_KEY ./heroes measure spec/heroes-spec.md` in my copy | the same three numbers; **verified**. The tool also prints what no brief carries: *"Headroom: 848 against the 10240 ceiling, but the FFI floor mortgages 60 of it (panel 030 R3)"* |
| 86 | design.md §1.6's payment rule | read lines 253-330 | lines 312-314 and 316-321; **verified** |
| 90-92 | the sentence and the production, verbatim | read § 13 in full (lines 341-441, the file's end) | **verified** |
| 92-94 | no sentence on a header's characters or on absolute paths; the grep | that grep over the whole file | it prints nothing anywhere in the spec, not only in § 13; reading § 13 whole finds no such sentence; **verified** |
| 96-101 | panel 036's R2 at line 147, the decoy from line 48 | `sed -n 147p`, `sed -n 44,62p` | **verified** |

## `compiler-engineer.md`

| line | fact | result |
|---|---|---|
| 9-12 | 164 lines, line 30, comment from 60, function at 72, `module_text.unquote`, `machine_locked_path`, no fix | **verified**; *"the unquoted value"* is the spelling minus its quotes (`selfhost/parse/module_text.hero:74-83` strips the two quotes and nothing else) |
| 13 | *"The same function judges a `link` string (`what` is `"link"` there)"* | **false**: `group_head.hero:115` passes `what: "library"`, over `library_name_span`, which answers for `.link` AND `.package` (lines 137-140). Run: `link "/usr/lib/libm"`, `package "/opt/x.pc"` and `link ":libm.a"` each draw `machine_locked_path` at `check` exit 1, all three saying *"a library in a group head"* (`<copy>/work/beside/link-abs`, `package-abs`, `link-colon`) |
| 14-17 | `c_text.hero`, `ffi_missing_header`'s two sites | as F2; **verified** |
| 18-19 | line 490, 8,689 used | as F3; **verified** |
| 23 | *"all thirteen names"* | ten in F1 and three in F6; **verified** |
| 32-35 | `check` is a golden form; `annotations` and `fixes` take a case filter; `layout` whole asks `budget` and `concat` | `tests/harness/main.hero:482` lists check, ir, emit, unsupported, run, fixes, annotations, layout as taking cases; `suite_layout.hero:631` asks `appends`, `concat` and `budget` only when `only == ""`; **verified** |
| 36-37 | cases as `tests/golden/check/` files annotated `#~ <code>` | 443 check goldens carry `#~`; **verified**. The sibling case `tests/golden/check/ffi-a-group-head-names-not-locates.hero` (panel 055's) is not named, and it pins `link` paths but no `package` path |

## `ffi-pragmatist.md`

| line | fact | command | result |
|---|---|---|---|
| 13 | Apple clang on this Mac | `clang --version` | Apple clang 21.0.0 (clang-2100.3.34.2); **verified** |
| 14-15 | `heroes-linux-arm64`, Debian clang 22.1.8 | `docker image ls`; `docker run --rm --network none heroes-linux-arm64 clang --version` | image `338a1044ac90`; *Debian clang version 22.1.8 (1~deb13u4)*; **verified** |
| 15-16 | *"clang 18 by `apt-get install -y clang-18` inside it"* | in the image: `ls /var/lib/apt/lists \| wc -l`, `apt-cache policy clang-18`; then a throwaway container with `apt-get update` first | **fails as written**: the image holds **0** apt list files, so the package cannot be located; after `apt-get update` the candidate is `1:18.1.8-18+b1`. The recipe owes `apt-get update` (and the network) first. No file at `826ddc2f` names the `clang-18` package (`grep -rn 'clang-18' docs .claude`), though `c_text.hero:17` records *"Debian clang 22.1.8 and 18.1.8"* measured today |
| 17-19 | the Windows box offline | as above | **verified** |
| 22-24 | `$(xcrun --show-sdk-path)/usr/include`, `/opt/homebrew/include`, the image's `/usr/include` and `/usr/local/include` | `ls -d` each | all four exist; **verified** |
| 26-28 | `pkg-config` on this Mac and on Linux | `which pkg-config`; in the image `command -v pkg-config` | `/opt/homebrew/bin/pkg-config` 3.0.7; `/usr/bin/pkg-config`; **verified** |
| 15-16 | one container at a time, `docker ps -q` empty first | `docker ps` at the end of this pass, about 16:25 | **not empty**: `457eee8183a0`, `heroes-linux-arm64`, `bash /inner.sh`, up five minutes, not mine (both my containers were `--rm` and had exited). Another session holds the image; the seat must wait for it or be told whose it is |

## `spec-warden.md`

| line | fact | result |
|---|---|---|
| 5-7 | §1.2 the cost, §1.6 the budget and its payment rule | design.md lines 190 and 253; the rule at 312-314; **verified**. The rule as written also requires (ii) *"the milestone at which it is scored"* (line 319) and lists the instruments that may pay: *"metric 3, `heroes mutate`, `heroes measure`, a line count, a compile, a diagnostic transcript"* (line 320). The brief names neither; panel 162's seat nearly lost its payment for want of that list (`docs/measurements/010-spec-budget-ledger.md`, the 6032 row) |
| 11-12 | the three counts | **verified** (F3); the headroom line is not in the brief |
| 13-16 | spec-shape: a draft's delta is *"a lower bound, in those words"*, the real count the coordinator's with one `--refresh` | `.claude/rules/spec-shape.md:152` (*"counts any file offline"*) and `:174`; **verified** |
| 21-22 | *"merging beats appending"* | `.claude/rules/spec-shape.md:67`; **verified** |
| 23-24 | panel 055's refusal is *"unstated in the spec"* | **true today, and misleading as framed**: the spec STATED it from `4333dab9` (2026-08-14, panel 055, *"Both are names, never paths: an absolute one is refused."*, the ledger's +14 row at `docs/measurements/010-spec-budget-ledger.md:55`), later worded *"Neither may be an absolute path."*, and `aab44f9b` (2026-09-05, M-c-callbacks step 3) **removed it as a priced named removal, -8**, on this ground, from its body: *"the message states the entire rule ... So the sentence is §1.4 redundancy the compiler pays back loudly, which is panel 089's shape"* (`git log -S 'absolute' -- spec/heroes-spec.md`; `git show`; design.md §1.4 at line 223; `docs/panel/089-text-from-c.md:163`). The brief asks the seat whether that refusal belongs in a new sentence without telling it a removal already spent it, nor the rule that removal rests on, which bears on Q4 for the new refusal as much as for the old |

## `historian.md`

| line | fact | result |
|---|---|---|
| 8-11 | N1570 for C11 | the tree cites N1570 for C11 on seven lines in six files under `docs/panel/`, five reports and one sitting (`grep -rn 'N1570' docs`); consistent, not verified by me against the source |
| 11 | *"C17 (N2310)"* | **unverified, and a question**: the tree never names N2310, N2176 or N3096. My recollection, unrun and so not a fact: N2176 is the draft usually cited for C17 and N2310 an early C2x working draft. The historian settles it |
| 11 | C23, *"N3096 or the final draft"* | unverified by me (no web access) |

## `llm-ergonomist.md` and `blind/`

| line | fact | command | result |
|---|---|---|---|
| 3-5 | fresh sessions, never a subagent (`/panel` § 2, 2026-09-30) | `.claude/skills/panel/SKILL.md:145-166` | **verified** |
| 5-6 | two sessions of at most 1.5 USD each | the skill's command carries `--max-budget-usd 3` for one session | consistent with a 3 USD cap split in two; **inconsistent with `00-shared.md:13-15`**, *"one paid session"* |
| 10-15 | `a/` and `b/` outside any git tree, no `CLAUDE.md` in or above, each `spec.md` byte-identical to the trunk's, each `brief.md` the kept copy | `git rev-parse --show-toplevel` in each; a parent walk for `CLAUDE.md` and `.claude`; `cmp` | not a git repository; nothing found in or above; `cmp` silent for all four; **verified**. Also: `/Users/joseph/.claude/CLAUDE.md` does not exist; `~/.claude/skills/synced/` holds five entries (account-synced skills), and `--safe-mode` disables skills by its help text |
| 15-16 | the two briefs differ only in the printed block | `diff blind/a-brief.md blind/b-brief.md` | lines 44 and 47-59 against 44 and 47-52, nothing else; **verified** |
| 17-21 | A is what the compiler prints today; captured at `<scratchpad>/188-blind-src/build.txt` | the A block extracted and `diff`ed against that file; `p.hero.txt` `cmp`ed against the capture's `p.hero`; the program rebuilt on my compiler | identical byte for byte; my rebuild differs only in the build directory's hash (`tu-3bfcdf095444c566` against `tu-4ac7f14704f923ea`); `check` 0, `build` 2; *"did you mean 'math.h'?"* present; **verified** |
| 22-25 | B is *"the coordinator's draft of route (1b)'s refusal"* | `<copy>/work/case-lt` | **false as labelled**: B says `<math.h>` *"holds `<` and `>`, which a header's name cannot"*, and (1b) as `00-shared.md:41-43` defines it refuses `>` but not `<`, which C's grammar admits. Run: a header `a<b.h` compiles directly under `-Wall -Wextra` with no warning, and `extern "a<b.h"` checks 0, builds 0 and prints `7`. **So B's message is false about `<` on this Mac**, and it is closer to (1c) or to a message for the bracketed shape alone |
| 23 | B *"in the compiler's own full form"* | `heroes check` on `extern "/usr/include/math.h"` (the sibling at the same column) and on a `trailing_colon` case | the compiler's gutter puts its bar at column 5 for a one-digit line, B's at column 6; the wording *fix (certain): replace X with Y* is the compiler's own convention (e.g. *"replace `int` with `i32`"* in `selfhost/`). Near, **not exact** |
| 27-29 | the repair `extern "math.h"` builds and prints `1.4142135623730951` | `<copy>/work/blind-c` | check 0, build 0, prints `1.4142135623730951`; **verified** |
| 34-40 | the command | `claude --version`; `claude --help` | 2.1.285; every flag exists. **No `--model`**: `--restricted` ignores the user's settings, so the two sessions run on the CLI's default, unnamed in the brief; the skill (line 185) warns that the CLI must support the sitting's model |

## `completeness-critic.md` (mine)

`/panel` § 3b and § 3c exist as cited (`SKILL.md:242`, `:258`); the copy
recipe and the paths are as stated; **verified**.

## Shapes beside F1, run (none is in any brief)

CLAUDE.md § RUN IT names the shapes to attack: one field, none, padded,
nested, tagged, generic, empty. Written by `<copy>/make_beside.py` into
`<copy>/work/beside/<case>/` (the header file, where there is one, named by
the string's VALUE, i.e. what the author means), run by `<copy>/run_cases.sh`
on my compiler, this Mac, 2026-10-03 about 16:05:

| shape | written in `extern` | file beside | check | build | run | what `build` or `check` said |
|---|---|---|---|---|---|---|
| empty | `""` | none | 0 | 1 | | `ffi_unknown_name`, its message an empty pair of backticks and then *"declares no `seven`, clang read the header and could not find it"*; the unit holds `#include <>` and clang says *"empty filename"*: **a false message** (no header was read) |
| escape `\t` | `"a\tb.h"` | `a`, TAB, `b.h` | 0 | 1 | | `ffi_missing_header` naming `a\tb.h`, the file there: the escape face |
| escape `\r` | `"a\rb.h"` | `a`, CR, `b.h` | 0 | 1 | | the same |
| a raw TAB | a TAB byte | `a`, TAB, `b.h` | 0 | 0 | `7` | |
| a raw NUL | a NUL byte | none | 0 | **2** | | *internal error*, clang's `-Wnull-character`, *'a<U+0000>b.h' file not found*, *clang refused the generated C*: **defect 216's face, a second shape** |
| above ASCII | `"é.h"` | `é.h` | 0 | 0 | `7` | |
| `<` | `"a<b.h"` | `a<b.h` | 0 | 0 | `7` | and clang direct: exit 0, no warning |
| padded, leading | `" ab.h"` | `ab.h` | 0 | 1 | | `ffi_missing_header` naming `` ` ab.h` ``: true, the space hard to see |
| the same, its own file | `" ab.h"` | ` ab.h` | 0 | 0 | `7` | clang does not trim |
| padded, trailing | `"ab.h "` | `ab.h` | 0 | 1 | | the same, `` `ab.h ` `` |
| the same, its own file | `"ab.h "` | `ab.h ` | 0 | 0 | `7` | |
| a directory | `"d/"` | `d/ab.h` | 0 | 1 | | `ffi_missing_header` naming `d/`: true |
| home-relative | `"~/ab.h"` | a directory named `~` | 0 | 0 | `7` | neither `machine_locked` nor C expands `~` |
| parent | `"../beside-sib/ab.h"` | a sibling directory | 0 | 0 | `7` | a relative path out of the program's directory |
| `link`, absolute | `"ab.h" link "/usr/lib/libm"` | | 1 | | | `machine_locked_path`, *"a library in a group head"* |
| `package`, absolute | `"ab.h" package "/opt/x.pc"` | | 1 | | | the same words, and its route says *"set `LIBRARY_PATH`"* and *"`link` takes the library's **name**"* to an author who wrote `package`; `pkg-config` reads `PKG_CONFIG_PATH`, as panel 055's own table runs it (`055-where-a-header-is.md:95`) |
| `link`, colon | `"ab.h" link ":libm.a"` | | 1 | | | `machine_locked_path` |
| C's brackets for the quotes | `extern <math.h>` | | 1 | | | `expected_extern_header`, *"found `<`"*, **no fix** (`<copy>/work/c-habits/`) |
| C's word | `include "math.h"` | | 1 | | | `reserved_word`, `fix (guess)`, then `unexpected_block` |
| C's line | `#include <math.h>` over `print(sqrt(2.0))` | | 1 | | | the line is a comment; `unknown_name`, ``fix (certain): rename to `sort` `` (next section) |

What this does to the routes as written, facts only: (1a) and (1b) list
neither the empty name nor a NUL, so both leave an exit 2 and a false message
standing; (1c)'s set refuses the NUL and says nothing of the empty name; (1c)'s
set as the ffi-pragmatist's brief writes it (letters, digits, `.`, `_`, `-`,
`+`, `/`) refuses a space, `'`, `<`, `~` and `é`, each of which builds and runs
today; and whether `\t` or `\r` is refused depends on whether a route judges
the spelling (a `\`) or the value (a TAB, which every route but (1c) admits).

Two counts for (1c)'s cost, run so the seats need not: of **832** lines
`extern "..."` in the tree's `.hero`, `.md` and `.txt` files (**286** distinct
names; `grep -rhoE '^[[:space:]]*extern[[:space:]]+"[^"]*"'`, my `work/`
excluded), **none** holds a character outside `[A-Za-z0-9._+/-]`; of **91**
`link` and `package` strings (29 distinct), only panel 055's two absolute
golden lines fall outside `[A-Za-z0-9._+-]`. And the ffi-pragmatist's survey
omits the SDK's `System/Library/Frameworks` (332 frameworks), whose headers a
C program includes as `<Framework/Header.h>`: **6,266** such names, **none**
outside the set, so that omission changes nothing on this SDK. What no survey
reaches is the population a positive rule meets first, a program's own headers
beside it, where a space and `é` already work.

## Found beside, outside the sitting's question, for the coordinator to file

**A `certain` fix that does not compile, on the blind seat's own program.**
`function main()` over `print(sqrt(2.0))` (`<copy>/work/c-habits/bare.hero`,
and the same with a `#include <math.h>` line above it, which Heroes reads as
a comment): `check` exit 1, `unknown_name`, *"nothing named `sqrt` is in
scope, did you mean `sort`?"*, ``fix (certain): rename to `sort` ``.
`heroes check --apply` prints `print(sort(2.0))`, and `check` refuses that
at exit 1: *"`bad_operand`: `sort` takes `[T]`, found `f64`"*.
``grep -n -i 'sqrt\|`sort`' docs/work/DEFECTS.md`` finds nothing. The golden convention
says *"CI asserts the applied fix compiles"* (`diagnostics-and-goldens.md`),
so by § Bounded discovery's list it reads as `blocking`; the class is the
coordinator's to set. It sits beside this sitting because it is where the
blind program lands when a repair drops the `extern` line or writes C's
`#include`.

## Missing

### Routes nobody listed

1. **Emit the value.** Decode the five escapes before `#include`, in the one
   function the emitters take a header's text from (`emit/externs.hero:61-67`,
   `unquoted`: called on 15 lines as `emit_externs.unquoted` and on 5 inside
   `externs.hero`; `c_text.includes` on 10, all by `grep -rn`), and refuse
   at `check` only what no emission can carry: `>`, a line end, a NUL, the
   empty name. On this Mac clang compiles `\` and `"` in a header's value
   (measured above, clang alone; I rebuilt no compiler). On Windows a `\`
   would become a separator, which is Q2's concern, unrun.
2. **Make the spelling the rule.** Refuse any `\` in a header string's
   source: C's header name has no escapes (`c_text.hero:14`), so the Heroes
   string carrying one may not use any. One rule catches `\\`, `\"`, `\n`,
   `\t` and `\r`, and the value question stops mattering for headers.
3. **A refusal and (1d) together.** The briefs offer (1d) as the alternative
   to a refusal, never as its complement: a refusal at `check` for the class,
   and `build` made true for whatever still reaches clang (another platform's
   clang, a character this pass did not think of, as the NUL and the empty
   name were until today).
4. **Hand clang the header through argv** (`-include`) instead of an
   `#include` line. Recalled, unrun, all of it: an argument is not lexed as
   a header name, so `>` and `"` would pass; and the flag searches as the
   quoted form does, which panel 036's R2 refused for the decoy. A seat with
   Bash should keep or dismiss it with a run.

### Questions the sitting should ask and does not

1. **Is a header string its value (spec § 2) or its spelling (every reader
   in the compiler, and the sibling golden's *"the string goes straight to
   `#include`"*)?** The same for `link` and `package`, which reach `-l` and
   `pkg-config` through the same `unquoted` (`externs.hero:134`, `:155`).
   The answer decides what (1a) to (1c) judge, whether Q2's Windows premise
   holds, and what § 13 would have to say.
2. **Which bytes does the refusal read?** `machine_locked` reads the
   spelling; a refusal of *"a line end"* over the spelling can never fire,
   since § 1 keeps a raw line end out of every string.
3. **The empty name and the NUL**: every route owes its answer for both.
4. **Does the plausible mistake occur?** F6 asserts it; the A/B measures a
   one-turn repair given a message, not whether a model writes the brackets
   in the first place. A generation arm (the task *bind `sqrt` from `math.h`*,
   the spec as it is) would measure it: one more paid session, the
   coordinator's and the author's to decide, not mine to run.
5. **Q4 against its own precedent**: the absolute-path sentence was in the
   spec and was removed at `aab44f9b` as §1.4 redundancy *"the compiler pays
   back loudly"* (panel 089's shape). Does that reasoning hold for the new
   refusal's sentence, and does a new sentence restore the removed one?
6. **Q3 against panel 187's registered prediction**: *"`selfhost/parse/` at
   most 8,696 lines (its unit) at the next `m-*` tag if R9 is adopted"*,
   scored by `code_lines.py selfhost/parse/*.hero` (panel 187's file, line
   236). R9 lets a `blocking` repair raise the row, *"never refused for it"*
   (line 216), and raising it fails that prediction; the compiler-engineer's
   brief offers raising the row without saying so.
7. **The siblings' fixes**: `extern <math.h>` gets no fix and `include
   "math.h"` a `guess`; a `certain` fix for `"<math.h>"` should say why its
   nearest sibling has none, or give it one.
8. **C's own positive rule** (recalled, unrun, for the historian): C11
   6.10.2's paragraph on which header names an implementation must map
   uniquely, POSIX's portable filename character set, and C++'s
   `[lex.header]` (conditionally supported where C says undefined). Each
   bears on (1b) or (1c), and the historian is asked for 6.4.7 alone;
   nothing in the tree cites 6.10.2 (`grep -rnF '6.10.2'`).
   Also whether a C2y working draft after C23 changed 6.4.7's undefined
   behaviour: unrun, I have no web access here.

### Seats handed a framing fact they cannot check

1. **The blind seat**: B's message says a header's name cannot hold `<`. The
   seat runs nothing and will take it as true; on this Mac it is false. Its
   experiment then scores a message the sitting could not adopt as written.
2. **Every seat**: the author's answer *1a* and the cap are in no tracked
   file, and the shared brief's *one paid session* contradicts the
   llm-ergonomist brief's *two*.
3. **The ffi-pragmatist**: the `clang-18` recipe fails as written (no apt
   lists in the image); checkable, at the cost of its own time.
4. **The compiler-engineer**: *"`what` is `"link"` there"*; checkable, false.
5. **The spec-warden**: the instruments that can pay a prediction, and the
   milestone the rule also requires (design.md:318-321), are not in its
   brief.
6. **Minor**: the blind sessions' working directory is
   `<scratchpad>/188-llm-ergonomist/a`, so the session sees the seat's name
   and the sitting's number in its path, as in earlier sittings; the spec
   names Heroes on line 1, so the language's name is not the leak.

### Facts that lean the briefs toward one route

1. **The class's second face is the emission's, written as clang's.** F1's
   reading and route (1a)'s premise make `\`, `"` and a line end look like
   what C cannot carry, which pulls toward refusing them at `check` and away
   from repairing what the compiler writes.
2. **Every pointer leads to a refusal at `check`**: defect 216's item
   (*"so the repair is a refusal at `check`"*), Q3's *"beside
   `machine_locked`"*, and the compiler-engineer told the coordinator's
   reading, *"(1b) or (1c)"*, before it builds. No seat is asked to build or
   cost (1d), or the value route.
3. **The A/B has no arm for (1d)**: today's output against a refusal at
   `check` with a `certain` fix. It can inform Q2's message; it cannot choose
   among Q1's routes, yet a win for B is easily read as a win for a refusal,
   though no (1d) message was offered. And B is labelled (1b) while refusing
   `<`, which only (1c) refuses.
4. **F6's *"the spelling a model fluent in C reaches for"*** is an
   inference, and it carries *"the plausible mistake is in the class"*.
5. **Q4's *"(F4: unstated in the spec)"*** omits that a sitting removed that
   very sentence as redundancy, which leans toward writing one back.

## Summary for the coordinator

**False** (eight): (1a)'s premise that this Mac's clang fails on `\` and `"`;
F1's second face read as clang's (it is the emitted spelling); F6's time
*15:58* (the files say 15:48); *"`what` is `"link"` there"* (it is
`"library"`, and it judges `package` too); B labelled route (1b) while
refusing `<`, and B's claim that a name cannot hold `<`; B *"in the
compiler's own full form"* (the gutter is one column wider); the `clang-18`
recipe as written; and the one paid session of `00-shared.md:13-15` against
the two of `llm-ergonomist.md:5-6` (one of the two is false). Not verbatim,
harmless: *"where the machine keeps it"*.

**Unverified by me**: the author's answer *1a*, its cap and *"convened in
full"* (no tracked record); *"about a minute"* (timing forbidden); C11 6.4.7,
N2310 as C17 and N3096 (no web access; questions for the historian);
*"the spelling a model fluent in C reaches for"* (an inference); F1 on Linux
and Windows (unrun, as the brief says); the 15:41 Tailscale reading (past).

**Everything else** in the briefs was run and holds, listed per line above.

First pass complete, 2026-10-03.

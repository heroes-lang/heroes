# Panel 193, the ffi-pragmatist: where `check --json` says a fix applies

Started 2026-10-05 23:16:50, stopped by the account's session limit (the
coordinator's message), resumed 2026-10-06 01:30:34 (`date` each time). Every
number comes from a command run in this session, named beside it. A sentence
that is an inference says so.

**My copy**: `<scratchpad>/193-ffi-pragmatist/`, made at 23:15:53 with
`git -C /Users/joseph/Temp/heroes/heroes-lang archive 00217c39 | tar -x -C
<dir>`. The compiler was built there from the seed, `clang -I runtime
seed/heroes.c runtime/runtime.c -o heroes`, `real 4.17 user 4.10`. No paid
run. Nothing in the trunk or a worktree was built or run. Two compilers were
built from `selfhost/` in my directory with that seed compiler: my
instrument (section 1) and route A (section 6), the latter from the
compiler-engineer's `check_json.hero` copied into a second archive copy,
`<copy>/routeA/`. The Windows box was used in `/c/w/193-fp-10445`, a fresh
folder, with 30,889,528 KB free at 23:27 and 30,726,544 at 01:38 (`df -k
/c`); no folder was removed.

## Verdict

- **verdict**: **object** to the proposal as framed, *each fix gains the place
  of the text it replaces, so a tool can apply a fix without re-deriving it*:
  a place makes a fix locatable, not an answer applicable. Not a veto: this is
  a tool's answer, not the C ABI and not a binding.
- **section**: design.md §1.11 (*FFI ergonomics rank alongside
  comprehension*; the consumer of this answer is written in C, the language's
  world) and §4.17 (`:2125-2131`, only `certain` fixes are machine-applicable;
  `:2142-2144`, a `certain` fix *writes nothing else*). **design.md does not
  cover `--json`** (`grep -c -- --json docs/design.md`: 0) **nor how an
  answer's fixes combine**; the combination rule's only home is
  `selfhost/cli/certain.hero`.
- **experiment**: `ffi/apply_fixes.c`, a C11 consumer of `check --json`,
  clang 21.0.0 on this Mac and clang 23.1.1 on the Windows box, zero warnings
  under `-Wall -Wextra -Wpedantic -Wshadow -Wconversion` (one on Windows, the
  UCRT's deprecation of `fopen`, none with `_CRT_SECURE_NO_WARNINGS`), ASan
  and UBSan on the Mac. Run on JSON the compiler wrote, my instrument's and
  route A's, over 541 roots, 8 adversarial roots and the Windows box. And
  `ffi/json_c_probe.c` against the real json-c 0.19 header: accepted, zero
  warnings.
- **argument**: see section 8 (118 words by `wc -w`).
- **prediction**: section 8.
- **condition**: section 8.

## 1. What JSON I ran the consumer on

At 23:17 the compiler-engineer's directory held a seed-built compiler and no
JSON of a route (`ls -la`, its `heroes` built 23:15). So at first the consumer
did not run on JSON written by hand: it ran on JSON the compiler itself wrote
from its own spans. In my copy only, `selfhost/cli/check_json.hero`'s fix row
gained a function `placed` printing, per fix: `file` (`locate`'s), `raw` (the
span as the compiler holds it), `start` and `end` each as `{offset, line,
col}` with `offset = span - files[k].start`, `line` per file and `col`
`locate`'s characters, `text` (`span_text`), `same_file`, `message_line`,
`file_index`. Built with the seed compiler, `./heroes build
selfhost/main.hero -o heroes-inst`, `real 112.69 user 90.58`. **Control**: on
178's case its answer with those fields stripped equals the frozen compiler's
answer (`json.load` both, compared: `True`); stderr 4,615 bytes against 1,799.
The offsets index the file on disk: bytes 788, 824 and 842 are each `,`
(`dd ... | od -c`). This instrument is a superset printed to measure units.

**After the resumption route A's JSON existed** (the compiler-engineer's
report, section 2), and section 6 runs the same consumer on it.

## 2. The consumer

`<scratchpad>/193-ffi-pragmatist/ffi/apply_fixes.c`, 528 lines of C11
(`wc -l`, after it learned route A's keys), with its own RFC 8259 reader so it
builds where json-c is absent. It reads the answer, keeps the `certain` fixes
whose `file` is the root, resolves each place in one of four units, and writes
the result on stdout:
- `offset`: the byte offsets, file bytes, half-open;
- `char`: `line` plus `col` counted in code points;
- `utf16`: the same `col` read as UTF-16 units, LSP's default;
- `byte`: the same `col` read as bytes (`line_start + col - 1`).

It combines fixes as `cli/certain.hero` does: by start then end, runs of
fixes that may touch, shortest first, a twin written once, a fix touching one
written withheld (exit 3 says "withheld, ask again"). `--naive` applies every
certain fix back to front with no rule; `--check-text` refuses a fix whose
`text` is not the file's bytes at its place; `--text-mode` opens with `"r"`
and leaves stdout in text mode, as a C programmer who forgets `"rb"` does;
`--lsp-lines` splits lines at `\n`, `\r\n` and a lone `\r`, as LSP 3.17 does.
A place field that is absent is a refusal (exit 1), never a 0.

## 3. The corpus: 541 roots, the critic's set, on my instrument's JSON

`ffi/census.py` (the critic's root definition from its `rounds.py`, read, not
run): 510 `tests/golden/check/*.hero` and 31 surface fixtures with a `.fixed`
or `.applied`. Per root: the instrument's `check --json`, the frozen
`./heroes check --apply` as reference, the consumer in every unit, eight at a
time (`real 11.00`). From `out/census.txt`:

| measure | total | roots |
|---|---|---|
| fixes | 1,551 | 281 |
| certain fixes | 670 | 170 |
| certain fixes off their message's line | 134 | 43 |
| fixes spanning more than one line | 368 | 89 |
| empty spans (insertions) | 178 | 22 |
| fixes in another file than their diagnostic | 0 | 0 |
| raw span different from the file offset | 0 | 0 |
| non-ASCII before the fix's start on its line | 1 | 1 |
| an astral character (4 bytes) there | 0 | 0 |
| non-ASCII inside the replaced span | 2 | 2 |
| twin pairs among one root's certain fixes | 0 | 0 |
| touching pairs | 7 | 3 |

So **536 of the 670 certain fixes (80%) stand on their message's own line**,
where lane str192's rendered form prints no place: a JSON that said only what
the text says would leave four fixes in five with no place a machine can use.

`--apply` exits 0 on 537 roots; the 4 others are `fixedbugs-227-*`, not UTF-8,
exit 1. **Equal to `--apply`, of 537**:

| consumer | equal | differs on |
|---|---|---|
| offset | 534 | the three `certain137-*`, consumer exit 3 (withheld) |
| char | 534 | the same three |
| utf16 | 534 | the same three |
| byte | 532 | the same three, **plus `fixedbugs-242` and `nonascii110`** |
| offset `--naive` | 534 | the same three, written corrupt |
| offset `--check-text` | 534 | the same three |

**What the byte reading writes** (diff against `--apply`): on `fixedbugs-242`
the fix *delete the byte-order mark* is bytes 0 to 3 and columns 1 to 2, so
reading the column as bytes deletes one byte of three and writes
`bb bf 66 75 ...` where the file opens `ef bb bf 66` (`od -tx1`), a file that
is **no longer UTF-8** (Python's decoder: invalid start byte 0xbb at 0); on
`nonascii110` the span `5.default("é")` ends at column 23 and byte 693, so the
byte reading leaves `x = 5)`, which does not parse.

**What the naive consumer writes** on the multi-round roots: `inside` gives
`print(total(xs.must())ad_operand bad_operand`, `types` gives
`-> function(i64) -> i64)4)`, and on `last` a fix runs past the text's end (my
consumer refuses, exit 1; the fixture's header records the compiler's own
naive pass aborting at 134 on that shape). **These are the shape
`certain.hero:15-18` records, reproduced from places that are each correct.**

**The faithful one-answer consumer** writes a partial program, never a corrupt
one, and knows it: exit 3 on each, with 3, 1 and 2 fixes withheld (`inside`,
`last`, `types`; `--stats`). **Looping** `check --json` then the consumer
until `check` exits 0 reaches `--apply`'s bytes on all three (`cmp`):
`inside` in 3 writing rounds, `last` and `types` in 2.

**A clean program's answer is 0 bytes** at exit 0, not a document
(`check --json out/clean.hero`: stdout 0, stderr 0; `suite_surface.hero:372`
pins it). My consumer, fed that, says `bad JSON: a value was expected`, exit
2, which is how I found it; json-c says `json_tokener_parse_ex failed:
continue` (section 5). The compiler-engineer and the spec-warden found it
too. A C consumer must branch on the exit code before it parses.

## 4. The unit of a column, line ends, and the Windows box

**The cases** (`out/units/`): `u1`, line 2 `    s = "è€😀",`, the comma after
characters of 2, 3 and 4 bytes; `u2`, `x = pair(a: "è€😀", b:
5.default("😀"))`, a certain fix whose span holds a 4-byte character and
follows three; `u4`, `u1`'s comma as the file's last byte, no final line end;
`u5`, a lone `\r` inside a comment above a comma; `u1crlf`, `u2crlf` and
`c178crlf`, the same files with every `\n` made `\r\n`; `b242`, the BOM case.

**What each unit gives on `u1`'s comma** (the instrument's JSON, and Python
over line 2's bytes): 13 characters, 19 bytes and 14 UTF-16 units stand
before it, so character column **14** (the JSON's), byte column **20**, UTF-16
column **15**; its file offset is **35** (line 2 starts at byte 16).

| case | offset | char | utf16 | byte |
|---|---|---|---|---|
| u1 | right | right | **deletes the closing quote**: `s = "è€😀,` | **cuts `€`**: `c3 a8 e2 82 f0 9f 98 80` (`od`), its last byte gone, not UTF-8 |
| u2 | right | right | `b:5"))` | `"è€😀5t("😀"))` |
| u4, no final `\n` | right (end 49, the file's length) | right | as u1 | as u1 |
| u1crlf | right (byte 36, one `\r` before) | right (col 14) | as u1 | as u1 |
| u2crlf | right (117 to 134 against 112 to 129) | right | as u2 | as u2 |

"Right" means equal to the frozen `--apply` byte for byte (`cmp`).

**CRLF**: the compiler reads it, gives the same line and column, and its byte
offsets count every `\r` (u1crlf 36 against 35; `c178crlf`'s first comma 800
against 788, twelve `\r` before line 13); `--apply` writes CRLF back.
**No final line end**: right in both units; an end equal to the file's length
is a legal place. **A lone `\r`** (`u5`): the compiler counts only `\n`, so
the comma is line 3; under LSP's line rule (`--lsp-lines`) the
line-and-column consumer deletes a space inside the comment (`carriage
return` becomes `carriagereturn`), leaves the comma, exits 0. The offset
consumer is unaffected by either rule.

**The Windows box** (clang 23.1.1, Git Bash, `/c/w/193-fp-10445`): the frozen
compiler, my instrument and later route A built there from their C
(`seed/heroes.c` and the emitted C of each, `-Wl,/STACK:67108864`). On all
nine cases (`out/win/run.sh`):
- the frozen compiler's JSON is **byte-identical** to the Mac's, with **0
  `\r`** in it (`tr -cd '\r' | wc -c`): stderr is written in binary;
- the instrument's JSON, offsets included, is byte-identical to the Mac's;
- `--apply`'s output is byte-identical to the Mac's, CRLF kept;
- the consumer opened with `"rb"` equals the Windows `--apply` on all nine, in
  both units.

**What forgetting `"rb"` costs, by unit** (`--text-mode`, the box):

| file | offset | char |
|---|---|---|
| LF (u1, u2, u4, u5, c178, b242) | differs: every `\n` written `\r\n` | differs: the same |
| CRLF (u1crlf, u2crlf, c178crlf) | **differs: wrong bytes deleted** | equal |

On `c178crlf` the offset reading in text mode deletes ten wrong characters
and leaves all ten commas: `#~ expectd_end_of_line`, `print(x+ y),`, and
**`function ars(n: i64)`, a renamed function** (`diff`, `\r` stripped). **With
`--check-text` it refuses all three CRLF cases** (`refused: the file changed
under a fix at bytes 800..801`, exit 1, 0 bytes written), and in binary mode
`--check-text` costs nothing: equal to `--apply` on all three. So no unit
excuses a consumer from binary I/O (the write side breaks every LF file in
both), and the replaced text is what turns the offset reading's read-side
failure into a refusal.

## 5. A stale file, another module, and json-c

**A file edited after the check** (`out/stale/`, one comment line put above
178's case after its answer was taken): the offset consumer writes wrong
deletions at exit 0: its first changed line is `x = 1,  ~ expected_end_ofline`,
the comment's `#` deleted so the comment becomes code, and a later one
`#~ exected_end_of_lie` (`diff`); the character
consumer happened to meet a column that does not resolve and refused (exit
1); with `--check-text` both refuse at the first fix (exit 1). Without the
replaced text, neither unit detects a stale file; with it, both do.

**Another module** (`applyx/main.hero`, its certain fix in `geom.hero`, file
index 1): the instrument gives `raw [123, 128]` and file offsets **66 to
71**; `geom.hero` is 89 bytes (`wc -c`), so the raw span lies past its end,
57 bytes on, `main.hero`'s 56 plus the joiner. Pointed at `geom.hero`, the
consumer by file offsets and by characters writes exactly `geom.fixed`
(`cmp`), where `check --apply main.hero` writes the root alone and says
`1 certain fix(es) are in another module and were not applied`. **The
critic's inference is measured**: a raw span is not a module's offset.

**json-c 0.19, the real header** (`ffi/json_c_probe.c`, `clang -std=c11 -Wall
-Wextra -Wpedantic -I/opt/homebrew/include ... -ljson-c`, exit 0, 0
warnings):
- on today's schema-1 answer of 178's case, the idiomatic read
  `json_object_get_int64(json_object_object_get(..., "offset"))` gives
  **`start 0 end 0` for every fix**, while `json_object_object_get_ex` says
  `ABSENT`. The header itself, `json_object.h:792-793`: *null is equivalent
  to 0 (no error values set)*;
- on the instrument's answer the same read gives 788 and 789;
- on a clean program's answer (0 bytes): `no document (json_tokener_parse_ex
  failed: continue)`, exit 2.

**Both answers say `"schema": 1`.** So under an unchanged schema a json-c
consumer written for places, meeting an answer that has none (an older
`heroes`, or a fix route A leaves unplaced, section 6), reads the place as
bytes 0 to 0: for a non-empty replacement it writes it at the file's first
byte. That last step is arithmetic on the measured 0s, not a run.

## 6. Route A's own JSON

The compiler-engineer's `check_json.hero` (sha256 `d0159e18...`, mtime 23:23)
copied into `<copy>/routeA/` and built there with my seed compiler (`real
92.80`). Each fix gains `file`, `line`, `col`, `end_line`, `end_col`,
`byte_start`, `byte_end`, `text`; **a malformed or file-crossing span gets no
place at all**, and `"schema"` stays 1 (its report, section 2; its writer,
read). The consumer learned the flat keys (`clang` again, 0 warnings).

`ffi/census_route.py` over the same 541 roots, plus `applyx/main.hero` and
seven of my cases, against the frozen `--apply` (`real 13.79`):

| consumer | 541 roots: equal of 537 | 8 extra roots: equal of 8 |
|---|---|---|
| offset | 534 (the three `certain137-*`, exit 3) | 8 |
| char | 534 | 8 |
| utf16 | 534 | 3 (u1, u1crlf, u2, u2crlf, u4 differ) |
| byte | 532 (plus `fixedbugs-242`, `nonascii110`) | 3 (the same five) |
| offset `--naive` | 534, the three written corrupt | 8 |
| offset `--check-text` | 534 | 8 |

670 certain fixes, **0 without a place** in the corpus, schema 1 throughout.
**Route A gives exactly what my instrument gave.** On the Windows box
(`out/win/runA.sh`, route A's emitted C built there): its JSON is
byte-identical to the Mac's on all nine cases, and the consumer by offsets and
by characters, binary mode, equals the Windows `--apply` on all nine.

**One weakened pin, beside the question**: route A names a fix's start with
the diagnostic's own keys, so `suite_surface.hero:286`'s
`err_has("\"line\": 10")` now matches twice in the answer of
`certain-labels.hero`, the diagnostic and its fix (`grep -o | wc -l`: frozen
1, route A 2): the pin would stay green if the diagnostic's line moved and
the fix's did not. `:425` is unaffected (its diagnostic has no fix).

## 7. What each field costs, in bytes and in C

**Bytes**, over the 281 answers that carry a fix (my instrument's answers,
fields kept or dropped, `json.dumps` in the writer's separators):

| fix object | bytes | against today |
|---|---|---|
| today: title, replacement, certainty | 744,722 | |
| + start/end as `{offset, line, col}` | 894,400 | +20.1% |
| + text | 922,635 | +23.9% |
| + a per-fix file | 1,055,335 | +41.7% |
| + start, end as bare byte offsets only | 787,102 | +5.7% |

Route A's real answers against the frozen ones: **+35.7%** over those 281,
**+20.9%** over all 541 (`out/routeA/` against `./heroes check --json`). The
replaced text is 8,867 bytes over the whole corpus, the longest 88. The
per-fix `file` is most of the rest because the golden paths are long; the
spec-warden's token table says the same of 178's case.

**C**, counted in my consumer (non-blank, non-comment lines, `awk` over line
ranges):
- the JSON reader, 196 lines: what a consumer gets from json-c instead;
- line and column to a byte (a line index split as the compiler splits, a
  UTF-8 walk, the UTF-16 variant), **47 lines**: what byte offsets remove;
- the combination rule (`same`, `one_point`, `touches`, shortest first,
  `choose`), **47 lines**: what an outcome per fix would remove; sorting by
  place, 6 lines, stays either way.

So a consumer of route A that writes what `--apply` writes carries 47 lines
re-deriving `certain.chosen`, and the one that leaves them out writes the
corruption section 3 shows. With an outcome per fix the same consumer is a
filter on `written`, a sort, and an assertion that the written places do not
overlap: an inference from my code's structure, since no compiler prints
outcomes yet.

## 8. The verdict's fields

**Which fields a C consumer needs, and the unit**, every fix, every one of
them placed in **its own file's** coordinates:
- `file`, the fix's own, asked of its span (0 of 1,551 differ from their
  diagnostic's today; the type allows it, and the rendered ` (at
  file:line:col)` already prints it);
- **`byte_start`, `byte_end`: bytes of the file as stored, 0-based,
  half-open** (`\r` and a BOM counted). The unit a C consumer applies with
  `memcpy` and no decoding; right on every root and every case above, on two
  platforms;
- `line`, `col`, `end_line`, `end_col`: per-file lines split at `\n` alone,
  columns in **characters** (code points), as the diagnostic's own `col` and
  the rendered text count them. For display and parity; a consumer that
  applies by them needs 47 lines and the compiler's line rule;
- **`text`**, the bytes replaced: turns a stale file and Windows' text-mode
  misread into refusals;
- **an outcome**, what `--apply`'s first round does with the fix: written,
  twin, withheld, elsewhere, malformed (the spec-warden's condition (b),
  computed by `cli/certain.hero` and reported, never restated). A fix with no
  place carries `malformed`, so no consumer reads an absent place.

**What route A leaves out, and what that costs the C consumer**: the
outcome (47 lines of `certain.chosen` re-derived, or the corruption of
`certain137-*`, measured); a moved schema (a json-c consumer cannot refuse an
unplaced answer by its number, and reads its places as 0, measured); and a
written home for the units (none exists: design.md and the spec say nothing
of `--json`, `check_json.hero:1` cites §4.17, which holds no word of it).

**On the schema I differ from the spec-warden**, on the C measurement: an
added key keeps every schema-1 READER right (true; only two pins test the
number, `suite_surface.hero:286` and `check.hero:376`, `grep -rn -F
'"schema'`, and no content reader does), but it leaves every new consumer
unable to refuse an OLD answer, the frozen compiler's today, by the number the
writer says exists for that (`check_json.hero:12-14`). The project's own
record of a stale `heroes` costing 31 minutes (`verification.md`) is that
failure's shape. Robust: move to 2 with the outcome, the two silent carriers
held as the spec-warden proposes. Conservative, for the author to choose: keep
1 and require every consumer to test presence per fix.

**The critic's question, from the C side**: an answer must promise (i) every
place is in the text as checked (one answer's places, applied to the original,
equal `--apply` on 534 of 537); (ii) its certain fixes are written together in
one pass, and **it says per fix what that pass does** rather than leaving each
consumer the rule; (iii) a `withheld` outcome means "ask again", and asking
again reaches `--apply` (3 of 3, at most 3 writing rounds); (iv) a consumer
that writes exactly the `written` ones writes the first round by
construction. **Panel 016's**: the JSON must say at least the text's place,
same file, line and character column, and it must say more: a place for every
fix (80% of certain fixes are on their message's line), the end, the bytes,
the replaced text. `check --apply --json`: an edit list against the original
would be the easiest answer for C (no rule, no loop), but it is a new
capability; refusing it at exit 2 now, as the spec-warden proposes, costs a
C consumer nothing the outcome does not give.

**argument** (118 words, `wc -w`):

> A place makes a fix locatable, not an answer applicable. On route A's JSON my
> consumer matches `--apply` on 534 of 537 roots only by re-deriving
> `certain.chosen` in 47 lines; without them it writes
> `print(total(xs.must())ad_operand` on the multi-round roots. The character
> column is right only for a consumer that decodes UTF-8 and splits lines as
> the compiler does: read as bytes it cuts a BOM into non-UTF-8, as UTF-16 it
> deletes a closing quote, with LSP's lines it edits a comment. Byte offsets
> need none of that, and `text` turns Windows' text-mode misread and a stale
> file into refusals. json-c reads an absent place as 0, and schema 1
> cannot tell a placed answer from an unplaced one.

**prediction**: on the route landed with an outcome per fix and schema 2, a C
consumer bound to json-c 0.19 that refuses a schema below 2, writes only the
`written` fixes by `byte_start`/`byte_end` with `"rb"` and binary stdout,
checks `text`, and asks again while any fix is `withheld`, holds **no
combination code** and equals `check --apply` byte for byte on **all 537**
roots of the critic's set where `--apply` exits 0, on this Mac and on the
Windows box; and on the box the same consumer opened with `"r"` refuses
`c178crlf`, `u1crlf` and `u2crlf` instead of writing them. One root that
differs, or one CRLF case written, falsifies it.

**condition**, what turns this to **approve**: route A's fields (`file`,
`line`, `col`, `end_line`, `end_col`, `byte_start`, `byte_end`, `text`, on
every fix, per file, half-open) **plus** (1) the outcome of `--apply`'s first
round on every fix, `malformed` where no place is given; (2) `"schema": 2`;
(3) the terms (bytes as stored, half-open, characters as code points, lines at
`\n` alone, every place in the text as checked, what each outcome means)
written in one home, the spec-warden's §4.17. Without (2) I would still
approve if the sitting shows no consumer can meet an answer without places,
which a stale binary contradicts. The objection falls entirely if the sitting
shows that no consumer ever writes two fixes of one answer together (the
spec-warden's falsifier, which 178's case already refutes: ten certain fixes,
one answer). Nothing here reaches a veto.

## 9. Beside the question, for the coordinator to file and class

- **A clean program gets no JSON document** (sections 3 and 5): three seats
  found it. From C, a document always (`{"schema": N, "diagnostics": []}`) is
  one parse path instead of two; `suite_surface.hero:372` pins the empty
  answer (defect 109).
- **The `:286` pin matches a fix's line** under route A (section 6).
- **The consumer's own code**: `ffi/apply_fixes.c` and `ffi/json_c_probe.c` in
  my directory are measurements, not tools of the tree.

Finished 2026-10-06 01:41:23 (`date`).

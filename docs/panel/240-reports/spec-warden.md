# Panel 240: spec-warden

Seat: spec-warden. Worktree `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/240-spec-warden/`,
detached at `429464c94012e0691dfa091e0b38070c5c9550f0` (`git rev-parse HEAD`, run 2026-11-09).
Instrument: the vendored count of `./heroes measure`, compiler built from the seed in that worktree
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, real 9.69 s by `/usr/bin/time -p`).
**No paid run**: the `real` row is the pin of 2026-11-08 and is not refreshed; every verdict below is
**provisional on the real row**.

Written as the sitting goes; numbers are named beside the command that produced them.

## 0. The ceiling, read today

`grep -n '### 1.6' docs/design.md` gives line 253. §1.6 today: **16384** real tokens, `claude-opus-5`
through `POST /v1/messages/count_tokens` (author decision 2026-10-14), with the consolidation block
(M-buildable-structs to M-compatibility-promise) spending at most **1,500** real tokens before the
thesis harness has run. The payment rule is unconditional (panel 012 as amended by panel 046): every
amendment owes a named removal or a registered prediction naming an instrument that exists today.

## 1. Baseline (base spec, unmodified)

`./heroes measure spec/heroes-spec.md` (stage 1, from the seed), exit 0:

| row | tokens |
|---|---|
| claude-legacy | 8178 |
| cl100k_base | 8325 |
| maximum (vendored) | 8325 |
| spread | 147 |
| real (claude-opus-5, pinned 2026-11-08, not refreshed) | 10900 |

Matches the brief's 8325 / 10,900.

## 2. The § 13 drafts, each measured (vendored only: a LOWER BOUND, not a price)

The drafts are copies of the base spec with lines 479 to 482 rewritten, built by a script
(`python3 -I`, every replacement asserted to match exactly once) into
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/240-spec-warden/.p240-drafts/` (untracked, never
committed). Each is measured with `./heroes measure <draft>` (stage 1, from the seed) and again with
`./heroes-next measure <draft>` (stage 2, built in 111.93 s real), and the two maxima agree on every
draft. A copy at another path gets the vendored rows and no `real` row (*"No ceiling judges ... this is
a count and not a verdict"*), so **every delta below is the vendored one, written as a lower bound in
those words** (`.claude/rules/spec-shape.md`, *a vendored delta is not a price*).

The **uniform** prose is one sentence for every mark, `Instead of `link`, a group may write `M`:
`extern "raylib.h" M "raylib"` asks the system where its headers and libraries are and what else it
needs.`, so that across marks the delta is the mark's own. The **coord** prose is 00-shared.md's word for
word. The **bold** prose keeps the base sentence's shape with the bold noun replaced.

| draft | legacy | cl100k (max) | delta, vendored max |
|---|---|---|---|
| base (byte copy) | 8178 | 8325 | 0 |
| `pkgconfig`, coord | 8182 | 8329 | **+4** |
| `pkgconfig`, uniform | 8180 | 8327 | **+2** |
| `pkgconfig`, bold | 8182 | 8329 | +4 |
| `pkgconf`, coord prose | 8182 | 8329 | +4 |
| `pkgconf`, uniform | 8180 | 8327 | +2 |
| `system`, uniform | 8177 | 8324 | −1 |
| `system`, bold | 8176 | 8323 | −2 |
| `library`, uniform | 8177 | 8324 | −1 |
| `library`, bold | 8178 | 8325 | 0 |
| `pc`, uniform | 8177 | 8324 | −1 |
| `machine`, uniform | 8177 | 8324 | −1 |
| `installed`, uniform | 8177 | 8324 | −1 |
| `package` kept, prose 1 (*a system's **package**, not a catalogue's,*) | 8185 | 8332 | +7 |
| `package` kept, prose 2 (*the system installed, never one a catalogue fetches,*) | 8189 | 8336 | +11 |
| folded: `link pkgconfig "raylib"`, `[ "link" [ "pkgconfig" ] string ]` | 8176 | 8323 | −2 |
| folded: `link system "raylib"` | 8173 | 8320 | −5 |
| control: uniform prose, `package` kept | 8177 | 8324 | −1 |
| control: base prose, only the three `package` words become `pkgconfig` | 8181 | 8328 | +3 |

What the controls say: the uniform prose saves 1 against the base sentence, and `pkgconfig` (and
`pkgconf`) cost **+1 per occurrence** over `package` on both vendored tables, three occurrences in the
base shape (the bold noun, the example, the production). Every one-token word (`system`, `library`,
`pc`, `machine`, `installed`) prices exactly as `package` does. On the binding instrument none of this
is measured tonight: the ledger's own rows of small deltas (vendored → real: +14 → +20, +5 → +6,
0 → −2, +10 → +13, −2 → −2, 0 → 0, −16 → −21, +16 → +20, +25 → +32, +11 → +15, +8 → +11, −10 → −15)
put the real delta of a +2 to +4 vendored draft at about +2 to +6, which is an **inference** from the
ledger, not a measurement.

### What the string after the mark is, measured (this bears on the name)

The distinct strings the tree writes after the mark (`git grep -hoE '^\s*extern "[^"]*" package "[^"]*"' -- '*.hero'`,
then `sort | uniq -c`): leaving out the deliberately malformed cases of the goldens (`zlib>=1`, `p\`,
`CLOCK$` and the rest), the real names are `raylib`, `libcrypto`, `libjpeg`, `sdl3`, `libcurl`, `libuv`
and `zlib`. Asked of this Mac's `pkg-config` (`/opt/homebrew/bin/pkg-config`, which `readlink -f` shows
is **pkgconf 3.0.10 answering under the `pkg-config` name**), `pkg-config --libs -- <name>` gives
`-lraylib`, `-lcrypto`, `-ljpeg`, `-lSDL3`, `-lcurl`, `-luv -lpthread -lm`, `-lz`. So **6 of the 7 real
names the tree writes after the mark are not the name `link` takes**; only `raylib` is (and `sqlite3`,
§ 13's `link` example, is both). The string is pkg-config's name for the library, which is a different
namespace from `link`'s. A mark that says which namespace its string is in tells the reader that from the
line itself (§1.3's locality test); a mark that does not leaves the reader to guess.

The compiler runs the tool by the name `pkg-config` and never `pkgconf`
(`selfhost/cli/libraries.hero:290-291`, `selfhost/cli/package_answer.hero:136-176`, by `git grep`). The
compiler's own sources write no group with the mark (`git grep -nE 'extern "[^"]*" package "' -- selfhost/`
prints nothing), so the mark is not needed for self-hosting, under any spelling.

The spec has never carried the string `pkg-config`: `git log -S'pkg-config' -- spec/ SPEC.md` prints no
commit. Panel 050's landing (`a48ddc1da`) wrote *asks the system where its headers and libraries are*;
its record says *asks the machine, through `pkg-config`* (`docs/panel/050-the-package-clause.md`, *What
landed*). So the coordinator's prose brings the tool's name into the spec for the first time.

## 3. The concepts a reader meets for the word *package*, before and after

Panel 235's spec-warden (`docs/panel/235-reports/spec-warden.md` § 8, read today) counted 15 concepts for
its winning arm (U-angle, F-toml: 14, with the sixteenth added) and wrote: *"One word collides with a word
the specification already uses ... That is one prompt holding two meanings of one word ... I count it here as
a sixteenth concept every arm carries until it is [ruled]."* Panel 235 landed U-angle and F-toml, so
today's spec carries the 16th.

Today, by `grep -noiw package spec/heroes-spec.md`: **9 occurrences in 2 senses**. Fetched Heroes code
on lines 31, 34, 35 (twice), 36 and 37 (§ 1); the C library the system is asked about on lines 479 (twice)
and 482 (§ 13's prose, example and `Extern` production).

| route | senses of *package* in the spec after | the sixteenth concept | a new collision |
|---|---|---|---|
| `pkgconfig` | 1 (§ 1) | **removed** | none: `pkgconfig` appears nowhere else in the spec |
| `pkgconf` | 1 | removed | none in the spec; names an implementation the compiler never runs |
| `system` | 1 | removed | none in the spec: line 480's *asks the system* is the same sense |
| `library` | 1 | **moved, not removed** | `library` would name what `link` names (lines 375, 479: *`link` a library*, *instead of a library*) **and** a mark whose string is pkg-config's name, which differs from `link`'s in 6 of 7 measured cases |
| `package` kept, prose only | 2, now stated | **kept**, made explicit | none new; +7 to +11 vendored to say it |
| `link pkgconfig` (folded) | 1 | removed | `link` would head two acts: linking a named library, and asking pkg-config for header folders and libraries (*where its headers ... are* is not linking) |
| `pc`, `machine`, `installed` | 1 | removed | none in the spec |

So every rename but `library` takes the count from 16 back to 15. The § 13 mark itself stays a concept
under every route: it was one of § 13's before panel 235 and is not among 235's fifteen.

## 4. Every other place in the spec where a word names two things, among the five

What I searched: `grep -noiw` over `spec/heroes-spec.md` at the base for `package`, `packages`, `module`,
`modules`, `library`, `libraries`, `catalogue`, `index`, `indexed`, `indices`, `dependency`,
`dependencies`, `system`, `pkg-config`, `link`; then for the neighbouring words `entry`, `entries`,
`Entry`, `fingerprint`, `sum`, `JSON`, `json`, `source`, `publish`, `header`, `headers`, `group`. Every
hit was read in its sentence.

**Among the five, the only word with two senses is `package`.**

- `module` (22, 26, 29, 31, 376): one sense, one `.hero` file. Line 31's *the module `parse` of the
  package `json`* and line 376's *a module's headers* are both a file.
- `library` / `libraries` (375 twice, 479, 480): one sense, C code a build links.
- `catalogue` (32, 35, 36, 37): one sense, `heroes.toml`.
- `index` / `indexed` / `indices` (85, 86, 329, 334 twice): one sense, a position in a string or array.
  The package index (the root `packages/`) is **never named** in the spec; line 33 says only *its
  source*.

**Found beside the five, outside the question, not widened into this rename** (each is a question for
the synthesis, not a finding this seat rules on):

1. **`Entry`**: the map literal's production (§ 7, lines 237 and 239, `Entry = Expression ":"
   Expression`) and a catalogue's entry (§ 1, line 34 *writes that entry*, line 36 *entries of the
   package's own catalogue*). One word for two things, but a production's name is not something a
   program writes.
2. **Two names for one thing**, the converse of *one name for one thing*: line 33's *fingerprint* is
   the TOML key `sum` (lines 41, 47); line 36's catalogue *entries* are `[dependencies.<name>]` tables
   (39, 43). Panel 235 counted these words as the format's.
3. **JSON has two homes.** § 13's first sentence (line 374): *Anything beyond this document — sockets,
   maths, JSON, databases — comes from C libraries*. § 1's example (line 31): `use <json>/parse`
   reaches *the module `parse` of the package `json`, code others publish*. Since panel 235's landing,
   something beyond this document can also come from a package of Heroes code, and the example § 13
   picks for a C library is the example § 1 picks for a package. This is where *package* and *library*
   meet in the prose, and the rename leaves it as it is. Whether line 374 is now false is a question,
   not a premise: a package of Heroes code arguably adds nothing *beyond this document*.
4. **`source`**: where a package comes from (33, *its source*) and program text (307, 367). Outside
   the five.

## 5. The critic's d.5, run: what a changed spec does to the net without `--refresh`

The critic (`reports/critic-briefs.md` d.5) left unrun whether `spec/spendable` and the `contract` check
go red on a stale pin. Run in my worktree, `./heroes-next run tests/harness/main.hero -- ./heroes-next spec`
(stage 2), with `pmset -g assertions` showing the machine held awake (Amphetamine and a `caffeinate`):

- **Control, the base spec unchanged**: `spec: 35 passed, 0 failed`, exit 0 (`real 46.30`).
- **The coordinator's draft copied over `spec/heroes-spec.md`, nothing else changed**: `./heroes-next
  measure spec/heroes-spec.md` prints the vendored rows (8182 / 8329), then *STALE: the recorded count is
  for 43a8cf64676f5a99 and this file is 2ae4064a69757622*, and exits **1**. The suite: `spec: 31 passed,
  4 failed`, exit 1:
  - `spec/budget` (*the spec measures 8329 and the record says 8325*) and `spec/ledger` (*the ledger's
    newest row says 8325*): the two pins a landing writes in its own commit, as every row does;
  - **`spec/spendable`** (*`measure` does not say "is 10960 and the check goes red at 16384"*) and
    **`spec/real`** (*does not print "10900   claude-opus-5, 2026-11-08"*): these two are red **because
    the real row is stale and for no other reason**. Writing `SPEC_TOKENS` and the ledger row cannot make
    them green; only a `--refresh` and the pasted pin can.
  - `spec/contract` stayed **green**: it measures `CLAUDE.md`, whose pin the spec's change does not touch.

**So the rename cannot land with a green `spec` suite tonight**: the author excluded the paid call that
turns `spendable` and `real` green. Either it lands with the refresh, which the author is asked for, as
every spec change has been (the ledger's rows 7650 to 8325 each name their `--refresh`), or the spec text
waits while the compiler's side lands. **What the net does NOT see**: no check of `spec` failed on the
spec saying `pkgconfig` while this compiler still reserves `package` (`selfhost/keywords.hero:83-84`,
`WORD_PACKAGE`). The other suites that read the spec are being run below to see whether any of them does.

**The rest of the spec's row, run the same way** (the `spec/heroes-spec.md` row of
`.claude/rules/verification.md`'s map names `spec special grammar fixes unseen`), the coordinator's draft
in place, the compiler unchanged: `grammar: 9 passed, 0 failed`, `special: 14 passed, 0 failed`,
`fixes: 1075 passed, 0 failed`, `unseen: 3 passed, 0 failed`, each exit 0. **So no suite holds the spec's
mark to the compiler's word, in either direction**: a spec saying `pkgconfig` beside a parser reading
`package` turns nothing red but the four token pins. The reason is in the code: the `WORD_` constants are
held to the editor's grammar (`spec/colouring`, `tests/harness/suite_spec.hero:3042-3051`), and the
productions to themselves (`tests/harness/suite_grammar.hero:432-441`: *"the grammar checked against
ITSELF, never against the parser"*). A rename that lands on one side only would pass the net.

The spec was restored from the byte copy after each run (`cp`, then `git diff --stat` empty and
`git status --porcelain` showing only the untracked `.p240-drafts/`).

## 6. A wording that renames AND costs less, measured

The coordinator's prose brings `pkg-config` into the spec beside the mark `pkgconfig`: **two spellings one
hyphen apart in one document**. Written as a mark, the hyphenated one is refused (below, § 7), so the cost
is a round trip (§1.2), not a silent program. The cheapest robust wording carries one spelling only. Five
tighter wordings, measured the same way (`./heroes-next measure`, vendored, a lower bound):

| draft | line 479 becomes | max | delta |
|---|---|---|---|
| **T1** | ``Instead of `link`, `extern "raylib.h" pkgconfig "raylib"` `` | 8319 | **−6** |
| T2 | ``A group may name a library as `pkg-config` does: `extern ...` `` | 8328 | +3 |
| T3 | ``A group may instead name a library as `pkg-config` knows it: ...`` | 8330 | +5 |
| T4 | ``Instead of `link`, a group may name a library as `pkg-config` does: ...`` | 8333 | +8 |
| T5 | ``Or a group names a library as `pkg-config` does: ...`` | 8328 | +3 |

(line 480, *asks the system where its headers and libraries are and what else it needs.*, and the
production's `( "link" | "pkgconfig" )` are the same in all five.)

The same T1 shape with the other marks: `pkgconf` −6, `system` −8, `library` −8, and `package` kept −8.
So **on T1's shape `pkgconfig` costs exactly +2 vendored over the cheapest name**, one per occurrence (the
example and the production).

T1 keeps every fact of the base sentence: that the mark stands where `link` would, the example, and what
the system is asked. It drops only the noun *package*, which is the point. It does not say in prose that the
string is pkg-config's name for the library, because the mark says that on every program line that uses
it, which is where §1.3 wants the meaning: a program line is read without the prose beside it.

**T1 with the spec suite**, copied over `spec/heroes-spec.md`: `spec: 31 passed, 4 failed`, the same four
pins (`budget` reading *a delta of -6*, `ledger`, `spendable`, `real`) and nothing else; then restored.

## 7. What a writer of a wrong mark is told (§4.17), run on today's compiler

A program `extern "zlib.h" <mark> "zlib"` with one function, `./heroes-next check`, exit 1, one error. For
`pkgconfig`, `system` and `pkg-config` alike:

    error[expected_end_of_line]: expected the end of the line after `extern "zlib.h"`, found a name
    (`pkgconfig`) — its signatures go on the lines below it, one level deeper

For `pkg-config` the name found is `pkg`. **The message names neither `link` nor the other mark**, and its
advice (*the signatures go on the lines below*) sends the writer in the wrong direction. Today's compiler
meeting an unknown mark stands in for the renamed compiler meeting `package`. That is an **inference**:
the renamed compiler was not built here. After the rename, `package` reaches that position from every
`0.x` program, the 34 frozen recovery programs, the two sealed thesis contexts (`tests/thesis/contexts/`,
copies of the spec at `a4e09e061` that still write `package "raylib"`) and anyone who read the published
site before it is redeployed. §4.17 (*every error carries all the context needed to fix it*) asks that
this message name the words that may stand there. That is a refusal for every wrong word, not an alias for
the old one, so CLAUDE.md § 2 does not forbid it.

## 8. The ledger: the row this change writes, and what pays for it

`docs/measurements/010-spec-budget-ledger.md`, *How to append*: in the same commit as the spec change,
with the delta and its payment, then `SPEC_TOKENS` and `LEDGER_ROWS` in `tests/harness/suite_spec.hero`.
§ 5 above adds that **the real pin must move in the same commit** (`--refresh`, pasted into
`tests/harness/suite_spec.hero` and `selfhost/cli/measure.hero`'s pin), or `spec/spendable` and
`spec/real` stay red. That pin is a paid call the author has not allowed tonight.

**Under T1 (recommended)**, the row would read, its real number left for the refresh:

| 8319 | panel 240 (retro-record, the author's instruction of 2026-11-09, *one name for one thing*), landed ...: § 13's mark `package` becomes `pkgconfig`; *A group may name a **package** instead of a library: `extern "raylib.h" package "raylib"`* becomes *Instead of `link`, `extern "raylib.h" pkgconfig "raylib"`*, and the `Extern` production's `"package"` becomes `"pkgconfig"`; −6 on the vendored maximum (8325 to 8319) and −6 legacy (8178 to 8172), the spec-warden's draft T1 to the token; priced on `--refresh` at the landing: **N real** on `claude-opus-5`, against 10,900 at row 8325; a removal net, so no payment is owed; the spec-warden's registered prediction W1 scored by that refresh | **§ 13's mark is `pkgconfig`, and *package* names one thing in the specification: code others publish** |

**Is a payment owed?** Under T1, **no**, provided the refresh reads at most 10,900: the ledger's precedent
for a change with no net addition is *"no addition, so no payment is owed"* (rows 7655 and 7854) and *"a
removal, so no payment is owed"* (rows 7838 and 8325). The clause removed, *A group may name a
**package** instead of a library:*, pays the mark's +2 inside the same sentence. If the refresh reads
above 10,900, W1 below is falsified and the row owes a named removal, which this seat has not found in
§ 13 without losing a fact.

**Under the coordinator's prose**, +4 vendored (8329): **a payment is owed**, and the only one available
tonight is a prediction of the price (`heroes measure --refresh` is an instrument §1.6 lists, and row 7655
was paid that way). It pays the letter of the rule and buys the reader nothing, while also bringing in the
second spelling.

**Headroom, for the record**: 16384 − 10,900 − 60 (FFI floor) = 5,424 real, by `heroes measure`'s own
line (*what is measured against the ceiling is 10960*). Every draft here moves the document by single
tokens, so no draft comes near the ceiling and **there is no budget veto**.

## 9. Verdict

- `verdict`: **approve, provisional** on the `real` row, which was not refreshed tonight. The approval
  covers the name `pkgconfig` with the T1 wording. I **object** to `library` and to the coordinator's prose.
- `section`: design.md §1.6 (the payment rule, unconditional), §1.2 (real cost), §1.3 (a line's meaning
  from the line), §4.17 (an error carries what fixes it).
- `spec_token_delta`: vendored, measured with `./heroes measure` and `./heroes-next measure`, a lower
  bound and not a price. T1: 8325 → **8319 (−6)**, legacy 8178 → 8172. The coordinator's prose: 8325 →
  8329 (+4). Real: **unmeasured**, the pin reading 10,900 (2026-11-08).
- `removal`: T1's own lead-in, *A group may name a **package** instead of a library:*, replaced by
  *Instead of `link`,* (−8 on the shape), which pays the mark's +2 in the same sentence.
- `needed_for_self_hosting`: **no**. No group in `selfhost/` writes the mark (`git grep`, empty).
- `argument`: Today *package* carries two senses across nine occurrences: § 1's fetched code and § 13's
  lookup. This is panel 235's sixteenth concept. Every rename except `library` removes it. `library` only
  moves it, because `link` already names a library, and 6 of the 7 real names the tree writes after the
  mark are not the names `link` takes (`pkg-config --libs`, run). `pkgconfig` tells the reader that
  namespace on the line itself (§1.3). It is also the program the compiler spawns, and pkgconf answers
  here under that name. T1 costs −6 vendored, so no payment is owed (§1.6). The coordinator's prose costs
  +4 and puts `pkg-config` beside `pkgconfig`.
- `prediction`: **W1**, scored by the landing's `heroes measure spec/heroes-spec.md --refresh`: T1 reads
  **10,890 to 10,896 real** (−10 to −4 against 10,900). Above 10,900 falsifies it, and then the row owes a
  named removal. **W2**, registered as an observation because it needs a paid run: given the landed spec
  and the task *bind libcurl's `curl_version` so it builds on any machine*, at least 8 of 10 blind Haiku
  5.5 readers write `pkgconfig "libcurl"` on their first try. Its instrument is the reader-test runner
  kept in panel 237's evidence. It is scored at the next paid reader run the author allows, and lapses at
  the next milestone close.
- `condition`, what would change this verdict:
  1. **Instruments first** (`.claude/rules/spec-shape.md`, *How a change to the document is made*):
     before the text moves, a check holds the `Extern` production's quoted marks to
     `keywords.WORD_LINK` and the renamed `WORD_*` in both directions. Measured in § 5: today the whole
     spec row is green on a spec/compiler mismatch. Landing without that check, I **object**.
  2. The landing carries the `--refresh` and its pins in one commit, so `spec/spendable` and `spec/real`
     go green (measured red without it). That is the author's yes for a paid call. Without it, the
     spec's half waits.
  3. The compiler side: a name after the header string is told with the two words that may stand there
     (§4.17, measured silent today); and `head_names.hero`'s `noun()` does not print *this pkgconfig's
     name*.
  4. `library` chosen: **object** (the collision moves). The coordinator's prose chosen: **object**
     (+4 owing a payment, and two spellings one hyphen apart), unless a reader run shows the prose
     needed.
  5. A reader run where `system` (T1 −8) scores as well as `pkgconfig` on W2's task would make me
     recommend `system`, two vendored tokens cheaper.

**The conservative route, for the synthesis to record**: keep `package` and say the difference in prose,
+7 (`package-prose-1`) to +11 (`package-prose-2`) vendored. It costs more than any rename, keeps the
sixteenth concept, and owes a payment.

## 10. What I could not measure, as questions

- What does `claude-opus-5` read for T1 and for the coordinator's draft, and how does it tokenise
  `pkgconfig` against `package` and `system`? (`--refresh` excluded tonight. Every delta here is
  vendored.)
- Does a reader write pkg-config's name (`libcurl`) more often under `pkgconfig` than under `system`?
  (W2, a paid run.)
- What does the **renamed** compiler say to `package` in the mark position? (Inferred from today's
  compiler meeting an unknown mark. No renamed compiler was built here.)
- Does any held-out thesis task (`tests/thesis/held-out/reserve.enc`, which I did not open) bind a C
  library through the mark? If one does, the sealed variant contexts teach a refused spelling after the
  rename, and the variant arms are penalised for something they do not ablate.
- Is § 13's first sentence (line 374, *JSON ... comes from C libraries*) still true beside § 1's
  `use <json>/parse`? This is a question for the synthesis, outside the rename.

# Panel 253, the completeness critic's second reading: the five reports against the world

Written as it goes, 2026-11-19, from 17:39 CET (`date`; `uptime` load 4.02), by the completeness critic of
sitting C of M-rebel-library. Read first, in the brief's order: `.claude/skills/panel/SKILL.md` § 3b and
§ 3c; `docs/panel/253-briefs/` (`00-shared.md` whole, its § Repairs after the critic's first reading
included, and the five seat briefs); my first reading `critic-briefs.md`; then the five reports,
`spec-warden.md`, `compiler-engineer.md`, `ffi-pragmatist.md`, `historian.md` and `llm-ergonomist.md` (its
body, and its appendix where a grade needed a session's words). I give no verdict. Each finding is
numbered as found, carries its kind (**wrong**, **unrun**, **route**, **rule**, **contradiction**,
**question**) and the command that settles it, or the words that it is unrun.

**Where I worked**: my copy of the frozen commit from the first reading,
`.claude/worktrees/rebel-scratch/253-critic/tree/` (`./heroes-next --version`: `heroes 0.6.0`, `sources
23c7d2a626b29fd5`, the frozen key, re-read at 17:39), and `253-critic/r2/` for this reading's probes. The
seats' folders were read, never built in; a seat's binary was only inspected (`nm`, `otool`, `atos`). No
paid run, no credential printed, no GitHub write, no sub-agent, no commit; one build or run at a time; no
wall clock; nothing read under the blind area's `home/` (its transcripts were counted by name alone).

## Findings

### 1. Q5: the compiler-engineer's *"an inlined frame at `-O2` is lost"* is the walk's own skip, not the line tables; the ffi-pragmatist is right (**contradiction**, settled; changes what (W) "built" means)

The compiler-engineer: *"`middle` and `outer` inlined into `main`, the walk tells `deep.main` alone"*
(`compiler-engineer.md:521-524`). The ffi-pragmatist: `main` calls `outer`, `pick` and `middle` are inlined
into `outer`, and (W) skips `outer`'s frame as *"the one whose place the report already names"*
(`ffi-pragmatist.md:12-19`, `:461-468`). **Settled on the compiler-engineer's own binary**,
`253-compiler-engineer/q5/deep-O2`, inspected only: `nm` lists `_h_deep_outer` at `0x100018910`;
`otool -tV -p _h_deep_main` holds `bl _h_deep_outer` at `0x100018a2c` (its return address, minus one, is the
walk's `deep-O2+0x18a2f`); `_h_deep_outer` holds `bl _hero_panic_index_at` at `0x100018a08`. So the frame
that aborts is `outer`'s, and the report's place names `pick`, which is not that frame's function.
`xcrun atos -i -o deep-O2 -l 0x100000000 0x100018a0b 0x100018a2f` (with `HOME` and `TMPDIR` in my folder)
prints `h_deep_pick (deep.hero:3)`, `h_deep_middle (deep.hero:6)`, `h_deep_outer (deep.hero:10)`, then
`h_deep_main (deep.hero:14)`: every call is in the line tables. **What it changes**: (W) as built drops,
at `-O2`, the aborting frame's function and every call inlined into it; the repair (tell the aborting
frame's address too) is **unbuilt** by either seat (`ffi-pragmatist.md:566`). `SKILL.md` § 3c: *"A route
that does not build is not adopted"*; so the synthesis may adopt (W) as built with this loss stated, or the
repaired (W) only once it is built and run on `deep.hero` at `-O2` (finding 16 lists every such gap).

### 2. The `>>` lowering is a branch on the sign at `-O0`: reproduced, the frozen tree's, wider than named, and no written promise of the frozen tree is broken by it (**wrong** in one word; changes how the defect may be filed)

**Reproduced** in my copy (`r2/shr/shr.hero`: one function `shr(x: i64, n: i64) -> i64` returning `x >> n`,
called with a value whose sign the program reads from `args()`): `heroes-next build shr.hero -o shr-O0`
(the default level, `selfhost/cli/verbs.hero:38` and `:136` read `default_level: "-O0"`, `:69` `-O2` for
`run`), exit 0, prints `-1152921504606846976`; the emitted C (`--emit-c`, `r2/shr/shr.c:123`) is `t3 = (t1 <
0) ? ~(int64_t)((~(uint64_t)t1) >> (uint64_t)t2) : (int64_t)((uint64_t)t1 >> (uint64_t)t2);`, the text of
`selfhost/emit/operator.hero:172` (read); `otool -tV -p _h_shr_shr shr-O0` holds **`tbz x8, #0x3f,
0x100020b58`** on `x` (loaded from the slot `h0_x` was stored to), choosing between `mvn; lsr; mvn` and
`lsr`. At `-O2` (`-o shr-O2 -O2`) the function is `cmp x1, #0x40; b.hs ...; asr x0, x0, x1; ret`: the sign
test is gone, the remaining branch tests the public count. **It is the frozen tree's**: my copy is a `git
archive` of `181555e9a` and its stage 2 prints the frozen key.

**How far it reaches, enumerated by `grep -rln -e shift_right -e rotate_right64` over Rebel's files** (31
files): every 64-bit word whose top bit is secret and passes `x >> n`, which is `crypto/internal/word.hero`'s
`shift_right` (`:19-23`) and `rotate_right64`, called by `sha512.hero:82-102` (SHA-384/512), `blake2.hero:114-143`
(BLAKE2b, and so Argon2's compression), `keccak.hero:54` (**every SHA-3 and SHAKE**, which the
ffi-pragmatist did not name), `scrypt.hero:115`; and, outside `crypto`, **`random/internal/chacha.hero:173`,
`wide.shift_right(x: x, n: 32)` on the generator's next key** (`random/internal/wide.hero:28-31`, the same
body), the package that makes keys and salts. 32-bit words held in an `i64` (SHA-256, BLAKE2s, BLAKE3,
ChaCha's own rounds) are non-negative, so their sign test runs one way whatever the secret (an inference
from the masks, not run).

**What it breaks, read**: Rebel's `crypto.hero:15-19` says *"A digest needs no such proof"* and promises
constant time only of `equals_constant_time`, whose body has no `>>` (the ffi-pragmatist's H.1, 0 reports);
Argon2i's promise (`crypto.hero:1050-1051`) is that *"the blocks read are chosen by a counter, so the memory
read tells nothing of the password"*, about memory, not branches; `random` states no timing promise
(`grep -n -i 'constant time'` there: nothing, an inference from that one word). So the ffi-pragmatist's
*"a defect of the frozen tree"* (`ffi-pragmatist.md:258-260`) is true of the machine code and breaks no
sentence the frozen tree wrote: by `.claude/rules/verification.md` § Bounded discovery it reads as an
`improvement` (hardening nobody needs to be right) **today**, and as the first prerequisite of route (H) the
day the sitting adopts (H), when it would become that work's `blocking` item. The synthesis should file it
with that class and that reason, not as a broken promise. The repair's C (`ffi-pragmatist.md:262-265`,
`m = 0 - ((uint64_t)x >> 63)`, then `(((uint64_t)x ^ m) >> n) ^ m`) is the ffi-pragmatist's, checked by it
on 10 M pairs; I did not re-run it.

### 3. Q3: CryptoKit's Ed25519 signs differently each time, measured again; the historian's *"approve (S) now"* rests on a precedent that holds no Ed25519 and answers none of the ffi-pragmatist's three measured objections (**contradiction**, the ffi-pragmatist's side checkable and checked, and a **route**; changes what Q3 may adopt)

**Run, in my folder** (`r2/ed/ed.swift`, built by `xcrun swiftc -sdk "$(xcrun --show-sdk-path)" -O
-module-cache-path r2/tmp/mc ed.swift -o ed`, `HOME` and `TMPDIR` in my folder, exit 0; run under `timeout
-s KILL 20`, exit 0): `Curve25519.Signing.PrivateKey(rawRepresentation:)` over RFC 8032 § 7.1 TEST 1's
secret prints TEST 1's public key `d75a9801...511a`; two signatures of the empty message,
`588e30bf...0d6e04` and `6b489f96...688d0f`, **`first-equals-second false`**, **`first-equals-rfc8032
false`**; RFC 8032's own signature verifies (`true`) and so does CryptoKit's (`true`). So the
ffi-pragmatist's finding (`ffi-pragmatist.md:318-324`) holds on this Mac: under route (S), Rebel's
`crypto/keypairs.hero:96` (*"Ed25519, of the message itself; deterministic."*, read) is false on macOS
and its test at `:485-496` (*"assert bytes.format_hex(signature) == t.signature"* over RFC 8032 TEST 1 to
3, read) fails there.

**The historian's side** (`historian.md:282-287`, `:379-393`): its precedent is .NET's commit `e0294993`,
and by the historian's own quotation that member exports four `@_cdecl` functions,
`AppleCryptoNative_ChaCha20Poly1305Encrypt`/`Decrypt` and `AppleCryptoNative_AesGcmEncrypt`/`Decrypt`:
**the two AEADs, no Ed25519 and no X25519**, so the primitive the seats disagree on has no precedent in
it. `grep -n -i 'determin\|randomi'` of `historian.md` prints two lines, both about a documentation tool's output (`:244`, `:276`): the report, which says it
read the ffi-pragmatist's twice, answers neither the non-deterministic signature, nor the header that clang
checks only when compiled as Objective-C (`ffi-pragmatist.md:304-312`), nor the layer compiling every
member for every build (`selfhost/cli/layer.hero:11-18`, read: *"the linker, not the compiler, then takes
only the members a program references ... So no predicate decides here which program needs which
member"*), which makes the Swift member a `swiftc` compile in **every** Mac build, `print(1)`'s included,
where the milestone page and the brief say *"every Mac that builds a program using it"*
(`M-rebel-library.md:209-210`, read). **A question the historian's condition does not ask**: whether a
.NET program's build compiles that Swift member at all, or receives it compiled once by .NET's own build
(the PR's *"bumping the native macOS toolchain"* reads as .NET's toolchain; unread here, so a question).
If the second, .NET's precedent does not carry (S)'s cost to Heroes, whose layer compiles on the machine
that builds the program (`00-shared.md:343-348`).

**A route nobody listed**: the two AEADs by CryptoKit (their texts and tags equal OpenSSL's byte for byte,
`ffi-pragmatist.md:316-318`; an AEAD's output is fixed by its key, nonce and data) and Ed25519 with X25519
by (V) on all three platforms, which also closes Windows' Ed25519. It keeps every byte Rebel promises
deterministic and still pays (S)'s toolchain cost; (V) alone, if it holds AES-GCM too, makes (S)
unnecessary. **What the synthesis may not do**: adopt (S) on the .NET precedent while Rebel's
*"deterministic"* and its RFC 8032 test stand; either Rebel's promise is ruled per platform (the author's,
since it changes what a Rebel signature is) or Ed25519 does not go through CryptoKit.

### 4. Q3: no route but the conservative is built, and (V)'s admission under decision 24 is the author's, not a seat's reading (**rule**; bounds what Q3 may adopt)

`SKILL.md` § 3c: *"A route that does not build is not adopted."* (S) was built as a library and run against
vectors, **not as a member of the layer** (`ffi-pragmatist.md:558-560`: Rebel's own `crypto/seal` and
`crypto/keypairs` tests under it unrun, at either level and under `--sanitize`); (V) has no candidate at
hand and nothing was downloaded (`:371-374`); (H) is a ChaCha20 of 86 lines, which shows the discipline,
not a route. The historian: Monocypher has **no AES** (`historian.md:405-409`, its page read), its
ChaCha20-Poly1305 in RFC 8439's 96-bit-nonce form is `unverified`, libsodium's AES-GCM *"requires the
Intel AES-NI extensions, or the ARM Crypto extensions"*; BearSSL, which the ffi-pragmatist names for AES
and GHASH, was not searched by the historian (its `search` field for Q3 does not name it). So (V) is
**two** libraries, one of them unsourced. And **decision 24** (*"What reads data from outside is written
in Heroes, never bound from a C library"*, `M-rebel-library.md:129-131`, read) is the author's: the
ffi-pragmatist's reading that it *"reaches [V] only as it already reaches the layer"*
(`ffi-pragmatist.md:383-386`) is a seat's reading of the author's words, and decision 29's three options
(`00-shared.md:300-304`) did not include (V). **So the synthesis can adopt the conservative (the gap,
`unsupported`, 0 lines), the measurements of (H), and (V) as a route put to the author with its two
libraries named, never (V) as built.**

### 5. Q2: whether a parameter line names `@r` or `r` is ruled by no arm, and it decides 18 of the compiler's refusals the spec-warden counts as 0 (**contradiction**, checkable, settled as a missing ruling)

The spec-warden: of the 642, *"**0** name a non-parameter"*, and *"built with the arm's literal refusal, it
refuses 0"* (`spec-warden.md:93-94`, `:190`), both from the brief-writer's census, whose line pattern is
`` `([a-z0-9_]+)` `` (`compiler-engineer.md:268-271`). The compiler-engineer's verb, built and run over the
149 faces, refuses **18 `doc_unknown_parameter`, every one an `@` parameter written with its mark**, and 1
`doc_example_unknown` (`compiler-engineer.md:255-259`). Counted again in my copy: `grep -rn -E '^ *# -
`@'` over Rebel's files prints **18** lines (`config.hero:101` `` - `@layer`: ``, `tokenizer.hero:362`,
`markdown.hero:591`, ...). § A7 says *"one bullet per parameter ... `` - `name`: `` ... an `@` parameter:
what the function changes in the caller's place"* and *"every parameter named by its own name"*
(`examples-rebel/README.md:329-333`, `:369-371`, read), which settles neither spelling; `spec-doc-r.md`
and the spec-warden's `doc-w` say *"one `` - `name`: `` line per parameter"* and are silent too. So the
spec-warden's prediction is false of the verb as built unless `@r` is ruled to name `r`, and the
compiler-engineer's 18 are refusals of a spelling nobody forbade. **The synthesis owes the ruling** (the
line names the parameter without its mark, or with it), in the spec sentence or the verb's help, before
either count is cited. t2's `count_words` has no `@` parameter, so the blind run measured neither.

### 6. Q2: the verb as built prints none of the five `doc-r` readers' examples, because it takes examples under `## Examples` only, which decision 22 asks and neither spec arm says (**contradiction**, run; changes the Q2 sentence or the verb)

The compiler-engineer's verb takes *"the documented `test` blocks under `## Examples` whose title opens with
its name"* (`compiler-engineer.md:233`); the spec-warden recommends the opposite, *"`heroes doc` finds an
example by the arm's rule wherever it stands"* (`spec-warden.md:112`); `spec-doc-r.md` and `doc-w` define
an example as *"a `test` with a comment directly above it and a title opening with a function's name and a
colon"*, no heading; the author's decision 22 says *"one documented example for every public function,
under `## Examples`"* (`M-rebel-library.md:109-112`, read). **Run**: the compiler-engineer's binary
copied to my folder (`r2/doc/ce-q26`, `--version` `sources 3355136a0f6addbc`, the seat's Q2 build) over
`coord/253-grade/t2-m-1/words.hero` copied beside it, `./ce-q26 doc words.hero`, exit 0: the reference
holds the signature, the comment, the call `words.count_words(text: text, sep: sep)`, and **no example**;
`grep -c '^## Examples'` over the five `doc-r` answers prints 0 each. So the five answers the blind row
grades right are references with no example under the verb as built. **Either** the sentence gains
*"under `## Examples`"* (decision 22's words, **unmeasured** by t2, whose readers never wrote the heading),
**or** the verb drops the heading and decision 22's heading becomes Rebel's convention, held by Rebel's own
check (which no file performs, `00-shared.md:266-270`). The same run shows a second defect of the
prototype, smaller: with the comment directly above the module's first function and no other, the verb
prints that comment twice, once as the module's doc under `# words` and once under the function.

### 7. Q2: `doc-w`'s refusal of a missing line is unbuilt, refuses 16 of the 142 printable faces whole, Rebel's most used, and is the one refusal no tool the historian read makes an error (**rule** and **route**; changes what Q2 may adopt)

The spec-warden's `doc-w` refuses *"a comment whose lines do not name each parameter once, in order"*
(`spec-warden.md:106-107`), which includes a function with parameters and no line; the compiler-engineer
built the arm's three refusals and priced that fourth at *"about 15 lines ... unbuilt"*
(`compiler-engineer.md:277-279`). **Counted over the compiler-engineer's 142 references** (its
`q2/run1/*.out`, read by my `r2/doc/faces_refused.py`, `python3 -I`): **16 faces** hold a function with
parameters and no parameter line, **661 functions** in all, the largest `bytes` 83, `strings` 77, `path` 72,
`net` 71, `lists` 65, `unicode` 54, `math/libm` 41, `fs` 40. Since the verb prints no reference for a module
it refuses (`compiler-engineer.md:227-228`), `doc-w` as the spec-warden words it leaves `heroes doc
<rebel>/strings` and fifteen more printing nothing until 661 comments are written, beside the 7 faces the
three refusals already stop; the spec-warden's *"refuses 642 functions"* is the census's count, the
compiler's being 661 over 142 faces (`compiler-engineer.md:262-266`). The historian: Javadoc's DocLint
*errors* on a bad `@param` name and *warns* on a missing one, rustdoc allows a missing doc by default, and a missing line *"was **not**
refused as an error by any tool read today"* (`historian.md:173-176`, `:230-238`, `:252-255`); and this compiler has no
warning class of its own (`grep -rln 'warning\['` over `selfhost/`: nothing; its `warning:` texts are
clang's, `cli/clang_told.hero:128`). **A route nobody priced**: the verb refuses what is wrong (a name no
parameter holds, an order) and prints the reference whatever is missing, the missing line held by Rebel's own
check, the one § A7 already promises (Javadoc's line, measured on Rebel as 0 order refusals and 18 or 0 name
refusals by finding 5). **What the synthesis may not do**: adopt `doc-w` as built, or cite 642.

### 8. The blind seat's grading holds cell by cell, P-t2-c is rightly scored false, and no session is voided; three things it does not say (**question**, checked as asked; changes no decision rule, and one reading of t2)

**Read against the registered rows** (`253-briefs/llm-ergonomist.md:245-251`) with the coordinator's
`coord/253-grade/grade.py` (133 lines, read whole) and `grade-out.txt` (154 lines, read whole), and each
answer's extracted files:

- **t2** (`grep -c '^test '`, a `grep -c` of the comment's fence lines, and the titles, over each `words.hero`): `now` 0 of 5, every
  one with a fence in the comment (2 fence lines each) and no test titled `count_words:`; `doc-r` 5 of 5,
  no fence, one `test "count_words: ..."` with a comment above, `ok` under `heroes-next test`. The script's
  shape check (a `#` alone, then exactly `` # - `text`: `` and `` # - `sep`: ``) reads True in all ten, so
  **P-t2-c is false as scored**: 0 of 5 `now` answers write the parameters in another form. **What the
  report does not say**: 3 of the 5 `now` answers (`t2-g-1`, `-4`, `-5`) already write a test of the
  documented example, titled `"the documented example"`; only `t2-g-2` and `-3` hold none. So `needed`
  (5 against 0) was near-certain from the row's title rule, which no reader of the frozen spec can guess,
  and **the measurement t2 actually bought is the fence**: five of five `now` readers put the example in
  a code fence inside the comment, the doctest form panel 136 refused, and none of `doc-r`'s did.
- **t4**: `now` 5 of 5 by the build and the three runs (15 of 15 `ok`, exit 0); of the predictions,
  `t4-g-1` and `t4-g-3` say *"presumably 0"* and `t4-g-4` names no outcome at all, only *"Ending: normal, with
  exit status 0"* (read in the appendix, `llm-ergonomist.md:1780-1857`): by the row's words (*"the test's
  outcome (`ok` or `FAIL`) and the exit status"*) `t4-g-4` names half, and the registered rule says a hedge
  is *recorded*, not how it counts. **No decision rule reads `now`'s right count** (the hazard is the build's
  count, 5 of 5 silent passes), so this moves nothing. `skip-r` 5 of 5 by the arm's words, as registered; but
  `t4-f-3` (`.ok url` and `.err e` arms with no `=>`) does not build, `error[expected_arm_arrow]`, where the
  `now` row gates on the build: read symmetrically `skip-r` is **4 of 5**, still *suffices*, and the hazard
  rule (0 silent) is unchanged.
- **t6**: the ten answers' commands, catalogues and `use` lines read from the extracted files: `now` five
  `heroes add rebel` (one `rebel@0.4.0`), `[dependencies.rebel]`, `version = "0.4.0"`, `sum = "<sum>"`, no
  `packages`; `umb-r` five `heroes add rebel/yaml` and `heroes add rebel/template`, `packages = ["yaml",
  "template"]`; the question-4 counts of `heroes add rebel/time` per session (an `awk` over the appendix)
  match the report's `1, 1, 1, 2, 2`, and `t6-g-4`'s one mention is its *"choice point"* of another reading,
  its prediction *accepted*. 5 of 5 each, as graded.
- **Voided**: `run.out` ends *"32 transcripts read; prompt-alone=NO: 0, git-status=LEAK: 0,
  claude-md-or-memory=YES: 0"*, one line per folder, the preflight, the 30 and the verdict; `find
  home/.claude/projects -type f` counts **32** files, one per folder (names only; no transcript opened);
  every `context` answer in the appendix denies any other input; `warden-at-start.md`'s sha256 is
  `1f0ba1909c8af73e...`, the hash `run.log` wrote at 15:51:14, and its lines 1 to 17 equal the spec-warden's
  report's (`diff`). **No session is voided, rightly.**

### 9. The answer to the brief's open question about `CLAUDE_CODE_TMPDIR` is in the area, and the report omits the listing it registered (**unrun** as written; settles a question)

The brief asked whether `TMPDIR` and `CLAUDE_CODE_TMPDIR` move the CLI's `cache-break-state` file into
the area (`00-shared.md:808-809`, *"unrun"*), and registered that *"a listing of `tmp/`"* goes into the
report (`253-briefs/llm-ergonomist.md:282-286`). The report says only that none was left in
`/private/tmp/claude-501/` (`llm-ergonomist.md:27`). `find 253-llm-ergonomist/tmp` prints
**`tmp/claude-501/cache-break-state-af7567b8-7bfd-43fa-87b3-db346e6f0aab.json`**, its folder dated 16:08:
**the variable did move it inside the repository's root**, and that is the answer, measured. The
listing belongs in the report and in the evidence.

### 10. Q4: the floor's own reason (*"a run that tested nothing being no green"*) is false of today's `heroes test` on a file with no test, which exits 0; and the floor turns no Rebel file red (**contradiction**, run; and a measurement nobody made)

The compiler-engineer and the spec-warden both make a run where every test skipped fail, on
`verbs.hero`'s *"One selecting none is refused, a run that tested nothing being no green"* and the
harness's floor (`compiler-engineer.md:62-67`, `spec-warden.md:125`); the historian finds Catch2's
precedent, *"returns a non-zero exit code, same as it does if no test cases have run"*
(`historian.md:485-487`). **Run in my copy** (`r2/q4/`): `heroes-next test none.hero`, a file with a `main`
and no test, prints `no tests in none.hero`, **exit 0**; `--only nothing` on it, the same, **exit 0**; on
`one.hero`, one passing test, `--only nothing` prints *"no test of one.hero has `nothing` in its title"*,
exit 2 (`selfhost/cli/verbs.hero:162`, `:171-172`, read). So a file that tests nothing is green today, and
the adopted floor would make a file whose one test skipped red: half of Catch2's rule. The synthesis should
rule both (a file with no test failing too, which moves every `heroes test` of a program with none) or say
why they differ. **And its cost over Rebel, counted**: the 105 skipping tests the brief-writer's lists name
(`bw/q4/rebel-skips-list.txt` and `rebel-live.txt`, `file:line` extracted and deduplicated, 105; `dns.hero`'s
the 106th) stand in 17 files, and **no file is all skips**: `acme.hero` 19 of 33 tests, `mongodb.hero` 56 of
70, every other 1 to 4 of 29 to 144 (`grep -c '^test "'` per file). So with the 106 rewritten as skips, the
floor fails no `heroes test` of a Rebel file on a machine with no service; it fails only a run `--only`
narrows to skipping tests (the compiler-engineer's `acme` and `mongodb` runs, exit 1).

### 11. Q4: the name, checked; two declarations of `skip` stand in frozen files a rename cannot touch (**wrong** count corrected; a cost unnamed)

`grep -rnE '^ *function skip[<(]'` over my copy, `heroes.packages` and `build` aside, prints **11**: the
compiler-engineer's **9** (the brief's grep missed `lists.hero:81` and `lists/internal/ends.hero:38`, both
`function skip<T>(`: the compiler-engineer is right) **and 2 more**, in
`tests/golden/recovery/corpus/examples/json/parse.hero.frozen:94` and
`.../markdown/inline.hero.frozen:26`, the frozen corpus panel 187's R2 reads trunk against round. Those
cannot be renamed, so with `skip` a built-in every program of that corpus that declares it gains a
`builtin_name_taken`, which R2 then reads as moved messages (an inference from R2's rule in
`.claude/rules/verification.md` § Bounded discovery, unrun). `grep -rlw skip_test --include='*.hero'`:
nothing. So the count favours `skip_test` by at least 47 sites, not 45 (the frozen files' calls of their
`skip` uncounted); the spec-warden's sentence (*"`skip(reason:
...)`"*) is what t4 measured, and `skip_test` lands *"with one word changed, carried by argument"*
(`spec-warden.md:139`), unmeasured, as the compiler-engineer's own condition says.

### 12. Q6: what one name in `packages` names, a top-level folder, a face or a module path, is decided by no arm, and the seats assumed two different answers (**question** the sitting did not ask; changes the refusal and the bytes)

**Counted in my copy**: the 161 programs' `use <rebel>/` lines name **163** distinct paths (`grep -h -o
'use <rebel>/[a-z0-9_/]*' examples-rebel/*.hero | sort -u`); **16** are `hero_*` modules (the
compiler-engineer is right, `grep -c hero_`), the other 147 each a file of Rebel's tree; **68 of the 163
are nested** (`grep -c /`), and their first parts are **97** distinct names (`cut -d/ -f1 | sort -u`). The
compiler-engineer's prototype lists paths and refuses a path not listed, so a list of faces refuses the 16
`reach_hero_*` programs (`compiler-engineer.md:185-195`); the spec-warden predicts that the list *"lists
exactly the distinct first parts of its programs' `use <rebel>/` lines, and 0 of the 161 programs is
refused"* (`spec-warden.md:160`), which is a list of 97 top-level names under which `packages = ["sql"]`
must admit `use <rebel>/sql/postgres`: a rule the prototype does not have. The arm (*"places only those
its `packages` names ... `heroes add rebel/csv` adds one"*) says neither. **It moves the numbers too**: the
brief's placement (median 4.5% of the version's bytes) is the face grain (`closure.py`'s face, a `.hero` not
under `internal/` and not `hero_*`); a top-level grain places every face under `sql`, `crypto`, `compress`,
... with the one named, **unmeasured**. The sitting should rule the grain (and so the 16 `hero_*` modules,
which a folder grain covers and a face grain does not) before the sentence, the refusal or the bytes are
adopted.

### 13. Q1: the robust arm is not the text to adopt: beside the ffi-pragmatist's three false sentences, its core paragraph rests on rulings the spec-warden's register invalidates, and it keeps 30 of design.md's 31 `core` words (**wrong**, three checked; a **contradiction** between the arm and the register)

**The three, re-run**: `git log -1 89a33de42` is *"M-rebel-library step 3: the 47 packages finished so far,
the platform layer under them, ..."*; `git diff --shortstat 89a33de42^ 181555e9a -- runtime/` prints *"36
files changed, 39110 insertions(+), 34 deletions(-)"*, of them `runtime/platform/` 18 files and 34,877
lines: so `design-r.md:503-504`'s *"nothing in the compiler or the runtime grows with Rebel"* is false at
the frozen commit. `design-r.md:573-574` keeps *"The rule above says every one of those needs comes from
C"* under a box that no longer says it. `design-r.md:554-555`'s *"written in Heroes where it reads data
from outside ... (M-rebel-library's decision 24)"* is false on its own SQL row: `sql/sqlite/hero_sqlite3.hero:27`
is `extern "sqlite3.h" link "sqlite3"`, and Rebel's README row 5 says *"SQLite over its library"*
(`examples-rebel/README.md:135`). The ffi-pragmatist is right on all three.

**And a fourth the seats did not name**: the arm's paragraph after the table (`design-r.md:572-590`, read)
still opens *"Rebel does not change this, as the core packages before it did not ... (panel 234's R1 and
R2, ratified 2026-11-06 ...)"*, says *"a package of M-core-packages is a module a program carries"*, and
grounds the rule on *"panel 234's R1, whose word was *shipped*"*, while the spec-warden's register marks
**234's R1 and R2 invalidated** (`spec-warden.md:66`). The two cannot both land. `grep -c -w -E
'core|<core>|M-core-packages'` prints **30** lines in `design-r.md` against **31** in `design-now.md`;
decision 8 says *"`core` no longer exists: every trace removed"* and decision 10 *"living texts are
rewritten clean"* (`M-rebel-library.md:75`, `:79-80`, read). How many of the 30 are dated history design.md
may keep with a note, and how many living rules, is uncounted here; the robust arm as handed to the seats is
a beginning, and the synthesis should adopt its direction and a text rewritten against the register, not
`design-r.md`.

### 14. Every *instructions retired* figure counts the process `/usr/bin/time` starts and none of its children (**rule**: the ffi-pragmatist's finding, reproduced; qualifies the brief's decision (b) and several numbers)

Run twice in my folder (`r2/ir/`): `/usr/bin/time -l clang -I runtime -c runtime/runtime.c -o a.o` reads
**75,651,510** and **75,763,728** instructions retired, the driver alone, its `-cc1` being a child; the
same compile under `sh -c "...; true"` reads **24,427,502** and **24,337,878**, the shell alone. So the
brief's method (`00-shared.md:673-679`) measures a parent's own work. **What it qualifies**: every figure of
a verb that spawns clang or another tool is the `heroes` process's alone: a `heroes build` or `heroes
test` (the ffi-pragmatist's `nats` test, which says so itself), the compiler-engineer's `xcrun atos`
207,660,061 (whatever `xcrun` starts). The compiler-engineer's `heroes doc` costs (`mcp` 104,176,278,848)
are likely whole: `heroes doc` runs `check`'s stages, and `check` hands clang no header (`heroes-next check`
of `r2/ir/wrong.hero`, `extern "math.h"` with `sqrt` declared `-> i32`, exits 0, where a build would ask
clang), an inference that no other child runs. The happy-path and abort figures of a program run directly (the compiler-engineer's
(W), +510,193 and +216,074 at the abort) are whole. The synthesis should cite each figure with *"the named
process's own"*, and the next brief's rule should say it.

### 15. Q5: `dladdr` waits on glibc while another thread is inside `dlopen`, reproduced; what it waits for is the other thread's constructors, and the frozen stack guard already makes the same call (**contradiction**, settled; one **question** for the frozen tree)

**Run**: the ffi-pragmatist's `hang.c` and `lock.c` copied to `r2/lx/` (read whole: thread B `dlopen`s a
library whose constructor blocks on a pipe; one second later the main thread calls `dladdr` under a
five-second alarm), built and run in `heroes-linux-arm64:latest` (container `cr-253-2`, only `r2/lx`
mounted, an outer `timeout -s KILL 120`, gone after: `docker ps --filter name=cr-253` empty): `clang -O2
-D_GNU_SOURCE hang.c` (without the macro glibc's `Dl_info` is undeclared, my first run `cr-253-1`), then
**`dladdr: still waiting after 5 s, the loader held by the other thread`, exit 3**, under `ldd (Debian
GLIBC 2.41-12+deb13u3) 2.41`. The same container printed **`-mframe-pointer=non-leaf-no-reserve`** for
`clang -### -O2 -c f.c` (Debian clang 22.1.8) and no `valgrind`: the ffi-pragmatist's premises 3 and 7
hold. **What it means, read narrowly**: the walk waits as long as another thread holds the loader; the
probe's constructor never returns, so its wait never ends; a real `dlopen` (Rebel's crypto, TLS and ODBC
members load `libcrypto`, `libssl`, `libodbc` at first call, `ffi-pragmatist.md:96-99`) holds it for its
constructors' time, and the wait is endless only where those wait on the aborting thread (an inference,
unrun). Either way spec § 6's *"at once"* and the compiler-engineer's open question
(`compiler-engineer.md:568-572`) are answered: (W) on Linux reads its base without `dladdr`, as
`parts/symbols.c` already reads the executable's table. **The question for the frozen tree**, the
ffi-pragmatist's (`ffi-pragmatist.md:500-503`), stays unrun: the stack guard's `hero_stack_symbol`
(`runtime/parts/stack.c:585-593`) falls back to `dladdr` on Linux for an address outside the executable,
inside a signal handler, where `dladdr` is not async-signal-safe; whether a stack overflow beside a
`dlopen` waits there is a probe nobody built.

### 16. What is built and run, and what is only priced, per question (**rule**, `SKILL.md` § 3c; bounds every resolution)

*"A seat with Bash builds the adopted route in its own copy and runs the cases the sitting named, before
the synthesis is written ... A route that does not build is not adopted"* (`SKILL.md:274-277`). Read from
the three building seats' reports and their folders:

| question | built and run | priced or written, **not built** |
|---|---|---|
| Q1 | nothing to build; the ffi-pragmatist's `sql/sqlite` binding held to its header (exit 0, and `ffi_return_type` on a wrong width) | the design.md text (finding 13); the register |
| Q2 | the verb, three refusals, the call by the checker's types, two runs `cmp`-equal over 149 faces (`ce-q26`) | the missing-line refusal (finding 7); a verb taking examples outside `## Examples` (finding 6); the golden forms `doc` and `doc_refused`; Linux and Windows determinism |
| Q3 | the conservative, one call per inheriting package (`fp/q3/conservative/`); a Swift library against vectors; a ChaCha20 in Heroes | (S) as a layer member, Rebel's own tests under it, `--sanitize`, `macos-26`; (V) (no candidate); (C); the `>>` repair in the emitter |
| Q4 | `skip_test`, its channel, the refusal outside a test, the all-skipped failure, five of the 106 rewritten (`ce-q4`) | its `run` and `check` goldens; the `test_shim` line closing forgery (g); the `skip` spelling's renames |
| Q5 | (W) on this Mac and in both Linux images, both levels, three programs and a C callback | (W)'s three repairs (finding 1's frame, finding 15's `dladdr`, the x86-64 flag); (A) in `heroes run` and `heroes test`; (P); (B); Windows |
| Q6 | `packages` read, the refusal over a `path` entry, `heroes add <entry>/<face>`, the four falsifier edits (`ce-q26`, `ce-q6`) | `fetch` placing the list's closure; Rebel in the index; a top-level grain (finding 12); the `hero_*` rule |

So the synthesis can adopt as built: the Q2 verb **with its three refusals and its `## Examples` rule as
built** (or say it is not that verb); `skip_test` with the floor; (W) **with its measured losses** or not
yet; the Q6 refusal and `add` over the face grain. Everything in the right-hand column lands only after its
own build and run, which the synthesis names as owed rather than as adopted.

### 17. Q1: decision 8 reaches 66 catalogues, not 17: the net itself, the hooks and 47 golden cases name `core`, and neither the brief nor the register says what becomes of them (**wrong** as a set, and a **question** for the author; changes the register and the order of work)

The brief counted *"17 programs of `examples/`"* naming `core` (`00-shared.md:164-167`), and the spec-warden's
register rules those 17 (`spec-warden.md:80`). **Counted in my copy**: `grep -rl --include=heroes.toml
'dependencies.core' .` (`build` aside) prints **66** catalogues: the 17 of `examples/`, **`tests/harness/heroes.toml`**
(`index = "file://../../packages"`, `[dependencies.core] version = "0.6.0"`, read), **`.claude/hooks/heroes.toml`**,
and **47 under `tests/golden/`** (29 `check` cases, 18 `full`). `grep -rl '^use <core>/'` over `*.hero`:
**116 files of `tests/harness/`** (142 under `tests/`, `use <core>/strings` 139 times by `uniq -c`), 3 of
`.claude/hooks/`, 0 of `selfhost/`. So the net, the third suite, and the write-time hooks
(`.claude/rules/verification.md` § A suite is the last judge) stand on `core` 0.6.0 through
`file://../../packages`: deleting `packages/core` as decision 8 reads (*"every trace removed"*) breaks both
before any example moves. Decision 7 says *"The ones the compiler is tested on use neither Rebel nor
`core`"* (`M-rebel-library.md:71-72`); whether the harness and the hooks are *"the ones the compiler is
tested on"* (then rewritten to use no library: 116 files) or keep a library (then which, since decision 6
keeps Rebel from the compiler, not from the harness) is **the author's question the register must carry**,
with the 47 golden cases, whose subject by their names is package resolution
(`fixedbugs-2368-a-nested-package-module.case`, `fixedbugs-2379-a-package-its-catalogue-does-not-list-is-named.case`,
...) and whose fixture must then be another package. The spec-warden's open question on `compat` (`spec-warden.md:80`) is the same set's smallest part.

### 18. Every premise a seat calls false in its brief, judged (**wrong** or right, row by row; checked as asked)

| seat, its number | the claim | judged | by |
|---|---|---|---|
| spec-warden 1 | the consolidation block's 1,500 binds nothing | **right** | `docs/design.md:291-293` (rows M-buildable-structs to M-compatibility-promise, *"before the thesis harness has run"*); `ROADMAP.md:207`, `:211`, `:215`: rows 66, 70, 74 done by 2026-10-30; the ledger's last `consolidation` row 7856 |
| spec-warden 2 | *"a spec that named it would make it the language's"* is the writer's inference | **right** | decision 9's words (`M-rebel-library.md:76-78`) refuse nothing of the kind; `spec-umb-r.diff` names `rebel` four times |
| spec-warden 3 | the arm's refusals meet 0 of the 642 | **right only if `@r` names `r`** | finding 5 |
| spec-warden 4, compiler-engineer 2 | the arm's `certain` fix is refused by panel 235's R5 | **right** | `selfhost/module/package_told.hero:10-11` (*"told with its line and never applied, the catalogue being no file of the compilation, which a `Fix` edits"*), `:102` |
| spec-warden 5 | `umb-r`'s `sum` exists nowhere | **right** | `grep -rl 5ecca972...2e41` over my copy: nothing; the frozen example's `json` sum is a fixture pinned at `selfhost/module/catalogue.hero:483` and `tests/harness/suite_spec.hero:1438` |
| spec-warden 6 | the brief omits panel 230's ruling on § 6 at zero tokens | **right** | `docs/panel/230-*.md:68-75` (*"Taken: zero tokens"*), `:137` |
| spec-warden 7 | no M-rebel-library row in the frozen ROADMAP | **right** | `grep -n M-rebel ROADMAP.md`: nothing; `:17` names M-core-packages current |
| compiler-engineer 1 | 9 declarations of `skip`, not 7 | **right, and 11 with two frozen files** | finding 11 |
| compiler-engineer 3 | 163 paths are 147 faces and 16 `hero_*` | **right** | finding 12 |
| compiler-engineer 4 | the card writer compares written types | **right** | `tests/harness/package_card.hero:169-170`, `q.ty == p.ty` |
| compiler-engineer 6 | `builtin_name_taken` also in `resolve/state.hero` | **right** | `selfhost/resolve/state.hero:355` |
| compiler-engineer, Q5 | *"an inlined frame at `-O2` is lost"* | **wrong** | finding 1 |
| ffi-pragmatist 1 | that the inlined frames are lost | **right** | finding 1 |
| ffi-pragmatist 2, 3, 7 | `dladdr` waits on glibc; `non-leaf-no-reserve`; no valgrind | **right** | finding 15 |
| ffi-pragmatist 4 | *"neither is measured"* is now measured | **right as an answer**, not a false premise: the brief's sentence was true when written | its Q3 |
| ffi-pragmatist 5 | the page's (S) measurement checked no signature | **right** | `M-rebel-library.md:205-207`: a public key, a seal opened back, a bit refused |
| ffi-pragmatist 6 | three false sentences in `design-r` | **right, and a fourth** | finding 13 |
| ffi-pragmatist 8 | a fourth home of system calls, `dlopen`ed C typed by hand | **right as read** | `runtime/platform/hero_crypto.c:2323-2330`, function pointers declared with `void *` arguments; the count of 151 not re-run |
| ffi-pragmatist 9 | `time -l` counts no child | **right** | finding 14 |
| historian 1 to 5 | Go's default trace, Go's doc page, Deno's words, `bcp`, its own correction | **not re-read** (no page fetched by me); each quotes its page | |

### 19. Smaller things, each with its command (**unrun** 1 and 3, **rule** 6, 7 and 8, **question** 2, 4, 5 and 9)

1. **Q4's hazard is wider than the 106**: test blocks that `return` in silence right after `if
   !...is_available(` (an `awk` over every `test` block of Rebel's files): **12**, in `crypto/seal.hero` 6,
   `crypto/keypairs.hero` 5, `web.hero` 1 (`:3030-3031`, read), none in the brief-writer's lists, which
   counted services and networks; with them the floor is at least 118.
2. **The spec's catalogue example is pinned twice** (`selfhost/module/catalogue.hero:483`'s test and
   `tests/harness/suite_spec.hero:1438`'s fragment, `grep -n` of its `sum`): any Q6 sentence that changes
   the example moves both, which no seat named.
3. **The compiler-engineer's Q6 union list was checked on 3 of the 161 programs** (`reach_hero_net`,
   `reach_strings`, `jq`, `compiler-engineer.md:199-201`); the spec-warden's *"0 of the 161 programs is
   refused"* and the compiler-engineer's *"0 by construction"* rest on those three and on argument.
4. **The verdict session approved `all-r`**, the three arms' own words (`llm-ergonomist.md:91-94`), which
   no seat adopts as written (`doc-w`, `skip-v2`, `umb-w`); it is a reading of sentences that will not land.
5. **The historian's *"No runner read calls its skip a bare global `skip`"*** (`historian.md:458-459`)
   stands beside its own Catch2 precedent, whose `SKIP` is a global macro of that word
   (`historian.md:482-487`); the name's case for `skip_test` rests on the compiler-engineer's counts, not on it.
6. **The historian's WebFetch left a file outside the repository's root**, as it says itself
   (`historian.md:9-12`, a PDF under `~/.claude/projects/.../tool-results/`): CLAUDE.md § Hard stops' *"no
   file is written outside this repository's root"*; I did not touch it; the coordinator or the author
   removes it.
7. **No seat ruled `heroes doc` against the stopping rule**: it enters by decision 13 as `probe` and `new`
   entered, and `.claude/rules/cli-surface.md:31-43` names those two by name, so the landing owes it a
   sentence for the third (and issue 808 its closing by the decision).
8. **The ffi-pragmatist's veto is reserved, not spent**: on any wording of decision 24 that reaches a
   program's own binding (`ffi-pragmatist.md:127-131`); the register and Part 2's bullet must keep it
   Rebel's rule, and the synthesis should say it did.
9. **The `run` form pins an abort's message, not its trace**: an expectation's ending is *"`!panic:
   <message>` or `!exit: <code>`"* (`tests/harness/suite_run.hero:123`, read), so (W)'s `called from` lines
   should move no `run` golden (read, not run over the form).

## What held, so the synthesis need not re-pay for it

- **The blind seat's grading**, every cell (finding 8), P-t2-c's *false*, no session voided, the predictions'
  copy equal to the report's lines by `diff` and to `run.log`'s hash.
- **Q5's spec half has no defender**: the spec-warden vetoes `spec-stack-r.md`, and neither the
  compiler-engineer's, the ffi-pragmatist's nor the historian's verdict asks for the sentence (each approves
  runtime work only); panel 230's zero-token ruling (`230-*.md:68-75`) stands unopposed.
- **The facts each seat re-ran and I re-ran again**: the `>>` branch at `-O0` and `asr` at `-O2` (finding 2);
  CryptoKit's signatures (finding 3); `dladdr` on glibc 2.41 and the arm64 image's `non-leaf-no-reserve`
  (finding 15); 9 declarations of `skip` outside frozen files (finding 11); 163 paths, 16 `hero_*`
  (finding 12); 39,110 lines of `runtime/` since Rebel's step 3 (finding 13); `time -l`'s parent-only count
  (finding 14); panel 235's R5 in `package_told.hero` (finding 18).
- **The floor costs Rebel's files nothing today** (finding 10): no file's tests are all skips.

## Not run by me, and why

The ffi-pragmatist's MemorySanitizer runs (336 reports per ChaCha20 block, the SHA-512, BLAKE2b and Argon2
counts) and its `>>` repair over 10 M pairs: one container run of mine was spent on finding 15, and the
rules allow one at a time; the compiler-engineer's builds (`ce-q4`, `ce-q6`, (W)), of which I ran one copied
binary once (finding 6); the Swift member inside the layer, which no seat built; the x86-64 image; every
Windows row (the box is not mine); the spec-warden's refreshes (its own, logged); the historian's pages
(none fetched). The bytes a top-level grain places (finding 12) and the frozen guard's `dladdr` in a signal
handler (finding 15) are unrun and posed as questions.

## Tally

19 findings: **contradiction** 7 (1, 3, 5, 6, 10, 15, and 13's arm against the register), **wrong** 5 (2,
11, 13, 17's set, and the rows of 18 that are), **rule** 5 (4, 7, 14, 16, and 19's three), **route** 2 (3's
mixed route, 7's Javadoc line), **question** 4 (8's reading of t2, 12, 15's guard, 17's harness and hooks),
**unrun** 1 (9), and 19's nine small things. Finished 17:52 CET (`date`), load 4.99 (`uptime`); my
containers `cr-253-1` and `cr-253-2` gone (`docker ps --filter name=cr-253`: none); no process of mine left
(`pgrep -fl 253-critic`: none).

## Summary: what changes what the synthesis may adopt, then the rest

**First, the findings that change what may be adopted:**

1. **Q5, (W) as built drops the aborting frame at `-O2`** (finding 1): the frame skipped is `outer`'s, not
   `pick`'s (`otool` and `atos -i` on the compiler-engineer's own `deep-O2`); adopt (W) with that loss
   stated, or the repaired walk once built and run.
2. **Q5, no `dladdr` in the walk on Linux** (finding 15): reproduced, glibc 2.41 waits while another
   thread is in `dlopen`; a (W) that calls it can wait after its report, against § 6's *"at once"*.
3. **Q3, (S) may not be adopted on the historian's .NET precedent** (finding 3): CryptoKit's Ed25519 is
   randomised (two signatures differ, neither RFC 8032's, re-run here), which falsifies Rebel's
   `keypairs.hero:96` and its test at `:485`; .NET's member holds the two AEADs and no Ed25519; a mixed
   route (AEADs by CryptoKit, Curve25519 by a C library) nobody listed.
4. **Q3, only the conservative is built; (V) is two libraries, one unsourced, and its admission under
   decision 24 is the author's** (finding 4).
5. **Q2, the `@` ruling is owed before either refusal count is cited** (finding 5): 18 lines write `@r`;
   no arm says whether that names `r`.
6. **Q2, the verb as built prints none of the five `doc-r` readers' examples** (finding 6): it takes
   examples under `## Examples` alone, which decision 22 asks and neither measured sentence says.
7. **Q2, `doc-w`'s missing-line refusal is unbuilt and stops 16 faces whole, `strings`, `bytes`, `path`,
   `lists`, `fs` among them** (finding 7); the count is 661, not 642; Javadoc's line (refuse a wrong name,
   tell a missing one elsewhere) is the unpriced route.
8. **Q4, the floor's reason is false of a file with no test, which exits 0 today** (finding 10): rule
   both, or say why they differ.
9. **Q6, the grain of a name in `packages` is undecided, and the seats assumed two** (finding 12): 97
   top-level names or 163 paths; it decides the 16 `hero_*` refusals and the bytes placed.
10. **Q1, `design-r` is not the text to land** (finding 13): its three false sentences and a core paragraph
    grounded on panel 234's R1 and R2, which the register invalidates; 30 of 31 `core` lines kept.
11. **Q1, decision 8 reaches 66 catalogues, the net and the hooks among them** (finding 17): 116 harness
    files and 3 hooks use `core` 0.6.0; the register must say what becomes of them before `packages/core`
    goes.
12. **What is built, per question** (finding 16): the right-hand column lands only after its own build.
13. **The `>>` branch is real and the frozen tree's, but breaks no written promise** (finding 2): file it
    as an `improvement` today, the first prerequisite of (H) if (H) is adopted; it reaches SHA-3 and
    `random`'s next key too.

**Then the rest**: the grading checks (8), the `CLAUDE_CODE_TMPDIR` answer found in the area (9), the name's
count with two frozen files (11), `time -l`'s parent-only count (14), the premises judged (18), and nine
smaller things (19).

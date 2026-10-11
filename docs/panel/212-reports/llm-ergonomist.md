# Panel 212, llm-ergonomist report (second run)

Inputs read: the brief and the five files under `212-briefs/samples/`, each with the Read tool. Nothing else was opened by me. See `context` for what reached me without my opening it, and it matters.

## verdict

**object.** As sketched, the rendering lets a stranger recover most answers, but only by digging, and in two samples it mislabels content or gives a title that cannot be searched. The fixes are cheap and are in the ranked list. No veto: the veto is for non-local language constructs, and nothing here is one. The nearest analogue is that samples 02 and 04 point at paths a stranger cannot open, which I count under noise, not under veto.

## experiment

I read each sample as a stranger and answered (a) problem, (b) solution, (c) commit, (d) credit, (e) noise. Marks are *found*, *not found* or *wrong*, with a caveat where the text gives the answer but a stranger would doubt it.

### Sample 01, defect 235 (NTFS header names)

- (a) **found.** `*`, `<`, `?`, `|` in a header's name build on Mac and Linux but on Windows `build` says "missing header", because `CreateFileW` refuses those characters. I had to guess that "header" means a C header named in an FFI declaration. No sentence says so, and no example name is given.
- (b) **found.** Those four characters are refused "in a group head's name on every platform", under *The repair*. "Group head" is undefined.
- (c) **found, with a caveat.** `24cb4fd4` (the repair) and the closing comment's full `24cb4fd404db...`. A second hash, `7ec19cb7`, is "the code of the batch's closing commit", which a stranger can mistake for the fix. Two hashes with two roles, and nothing marks which is the answer.
- (d) **found, correctly none.** "the ffi-pragmatist" and "panel 188" are internal origin, not somebody who solved it before. A stranger could misread "Origin: panel 188's ffi-pragmatist" as a credit. It is not one.
- (e) noise: the *Class* section, the batch 8 / "net is owed at the batch's close" paragraph, the three-platform legs paragraph (clang 22.1.8, 18, 23.1.1), the "Record: .../blob/<the commit the run is pinned to>/..." line, which is a placeholder and not a URL, and `class: adjacent` printed three times. The title is good: "header name", "NTFS", "Windows", "Mac and Linux" are all searchable words.

### Sample 02, decision of panel 105 (inference of a type parameter)

- (a) **not found.** The title is a path and the *Question* repeats it. I reconstructed the problem from the default: a generic type parameter that no argument fixes had to take its type from context. The text never says it.
- (b) **found.** The checker binds the unbound parameter from the annotation, the declared result or a non-generic callee's parameter. It refuses a call with no context under `cannot_infer`. It refuses a type parameter used nowhere in the signature. It keys instantiations by the bound types, and the verifier refuses any unresolved type. All of this sits under the heading *Where to look*.
- (c) **found.** `4d16200f...` in "Settled by". Caveat: the body says "the work lands in the same session", so I cannot tell whether this is the ratification commit or the implementation commit.
- (d) **found, one line wrong for a stranger.** Swift #48363, Go #50285, Java JEP 101, each under "Taken from". The Go line says inference gets hard when nested and "the proposal for it sits on hold". Under the verb "Taken from Go" a stranger reads that Go solved something and Heroes borrowed it, but the line says Go did not. The preamble says "the sitting's own file says what was taken from it", a path a stranger cannot open, and a path in backticks is not a link.
- (e) noise and hiding: the seat vote tally, "warden A/OBJECT on Principle 0 cost (+32 tokens, 52 of 52 signatures)", "Option B in the warden's B3 wording (+15)", two Italian quotations, and "A" and "B" never defined for a reader who did not see the sitting. The verdict and the resolution sit under *Where to look*, which is not a place.

### Sample 03, task (the Windows census)

- (a) **found.** Price a route on four platforms instead of two, by walking the census on the Windows headers. I had to infer "census of byte-typed fixed-extent array parameters in C headers" from the table.
- (b) **found.** The C library spells none and Win32 spells two, so the sets stay disjoint and the milestone's question does not move. It is stated only at the bottom, under *What was done*, after about 60 lines.
- (c) **found, doubtful.** `61fa67e4` in the body with its subject quoted: "M-declared-extents step 3: panel 166 finds the mark was there and the lend was discarding it". That subject is about something else. Either the quotation is the useful check and the card is wrong, or the subject is simply a different commit's. A stranger cannot tell which, and would trust neither.
- (d) **found, correctly none.** Odin's binding "says 15; it is stale" is a criticism, not a credit. "panel 165's historian" is internal.
- (e) noise: *How it was run* (clang flags, "billed by the hour", "no Python beyond the Microsoft Store stub"), the 86.7 MB AST dump, and `scratchpad/extents/census.py` and `docs/ref/environment/windows/WINDOWS-MACHINE.md` under *Where to look*, neither of which a stranger can open. The title "The fourth platform spells none in its C library" has no subject: none of what? It has no "Windows", "array" or "census" in it, so a search for the topic misses it.

### Sample 04, defects 018 to 021

- (a) **found, buried.** Four problems in one paragraph of about 700 words under *Where to look*: 018 `<` named two of its ten accepted types in the message; 019 a `guess` fix offered `fit_<width>`, a family that no longer exists; 020 the library's extern constants were in every program's scope (`print(HERO_OS_OK)` printed `0`); 021 a message showed `[#0]`. *What happens* says only "filed and closed 2026-09-08, M-closures-verdict step 3", so I would have read the card as empty and left.
- (b) **found.** Each has "The repair" in bold inside the paragraph. Fifth item: the repair of 020 introduced a wording wart, said in capitals near the end.
- (c) **found.** `c789bcc5...` only in the closing comment. No hash anywhere in the body. One commit for four defects, and nothing says that "Fixed by" covers all four.
- (d) **found, correctly none.** "panel 119's ergonomist had predicted" is internal.
- (e) noise: "1697 of a DECIDED 1700", "44.89 s, /usr/bin/time -p", "hand-edited, never regenerated", `design.md:923`, `.claude/rules/module-shape.md`. The labels line prints "(no class)".

### Sample 05, old log row (one binary)

- (a) **not found.** No problem is stated: why one binary?
- (b) **found.** The title is the decision. Usable.
- (c) **found, caveat.** `113b1019...` "Settled by". For a row dated 2026-08-03 I cannot tell what the commit is: the commit that recorded the row, or the one that did the work.
- (d) **found, weak.** The raw row says "the zig model". A stranger reads that as credit to Zig, but it is unlinked, undated and says nothing about what Zig does. It is also not under any "Prior art" heading.
- (e) noise: the *Question* section repeats the title; *Notes* prints the raw pipe row `2026-08-03 | ... | corollary of §3.3; the zig model | §3.3 | 004`, where "§3.3" and "004" mean nothing outside the repository. The labels line prints "(area none: no label)" as text.

### Tally

Of 20 cells: 13 found cleanly, 5 found with a caveat (01c, 02d, 03c, 05c, 05d), 2 not found (02a, 05a), 0 wrong in the strict sense. 02d is the nearest to wrong: it is true to the page and it misleads under its verb.

### Could a stranger use the titles of 02 and 05?

- **Sample 02: no.** A path tells a stranger nothing, cannot be opened, and a list view truncates it. It also names the sitting's file, not the problem. The title slug does contain the words "a type parameter takes the type the context asks for", so the title could be that sentence and nothing more. Searching by hyphenated path tokens is luck.
- **Sample 05: yes, as a decision.** "One executable `heroes` with subcommands; never a second binary" is a statement I can search for ("single binary", "subcommands"). What fails is the body, which gives no reason.

## hesitation_points

- **Which section holds the answer.** In 02 and 04 the substance is under *Where to look* and the section called *What happens* in 04 is metadata. A reader who trusts the headings skips the answer. Wrong guess: the card reads as empty and the reader leaves, silently, a wrong conclusion that nothing compiles or catches.
- **Which hash is the fix** (01: `24cb4fd4` against `7ec19cb7`; 02: ratification or implementation; 03: subject unrelated). Wrong guess: the reader checks out or cites the wrong commit, which looks right and is silently wrong.
- **"Taken from Go"** on a line saying Go put the proposal on hold. Wrong guess: Go is credited with something it did not do, silently.
- **`<the commit the run is pinned to>`** inside a URL. Wrong guess is a loud one, a broken link, but it appears on all five samples.
- **Undefined internal words**: "group head", "A/B", "the net", "batch 8", "§3.3", "004". Wrong guess: the reader invents a meaning and trusts it.

## argument

A stranger needs three things in the first ten lines: the symptom in words, the resolution in one sentence, and one commit that is the answer. The sketch has them, but scattered across sections whose headings do not match their contents, behind a path title in 02 and a wall of text in 04, with several hashes in 01 and a doubtful one in 03. The credit lines in 02 pair a verb ("Taken from") with a line that sometimes says the opposite. Tidy the headings, put a summary first, and demote the project-internal text.

## ranked changes to the rendering

1. **A fixed three-line answer block before anything else**: *Problem*, *Resolution*, *Commit* (one hash, linked, with its subject). Needed in 03, where the answer is at line 81 and in 04, where it is a paragraph under the wrong heading. Check: 03 should read the same answer in the first ten lines.
2. **Headings named by what they hold, not by a position in a template.** 04's *What happens* holds "filed and closed...", and *Where to look* holds the whole defect. 02's *Where to look* holds the vote and resolution. 01's *Where to look* holds panel citations. If a field is not a location, do not call it one; if it cannot be mapped, do not render it under a template heading.
3. **Titles by symptom nouns, never a path.** 02: use the sitting's own title ("A type parameter takes the type the context asks for"). 03: add the subject ("Windows headers spell no byte-typed fixed-extent array parameter"). 04: name all four numbers ("Defects 018, 019, 020, 021") so each number is a search hit, and name what broke ("`<` message, `fit_` fix, extern constants, `#0`"). 01 is the model.
4. **One commit line, and say what role each hash plays.** 01: "repaired at `24cb4fd4`; verified on `7ec19cb7`". 03: if the subject does not describe the work, drop the subject. 04: say "this commit repairs all four". 02: say whether it is the ratification or the implementation.
5. **Replace `<the commit the run is pinned to>`** in every Record link with a real permalink or drop the line. It is on all five samples and reads as a bug.
6. **Credit lines: the verb must match the line.** For 02, either say what was adopted in a clause the stranger can read ("Go: has no equivalent; proposal on hold, which is why Heroes refuses a call with no context"), or use a neutral verb such as "Considered". And make the sitting a real link, not a backticked path. One caveat I owe, see `context`: I know that the project's own rule restricts these lines to what the page states and nothing about what Heroes did. If so, my fix conflicts with that rule, and the rule should be the thing under test, because a stranger cannot get the second half from a path they cannot open.
7. **Move the project-internal text into a collapsed `<details>` at the end**: seat votes, the batch gate, DECIDED ceilings, timing, `Class`, the platform-legs paragraph, Italian quotations. A stranger can stop reading at the end of the answer block.
8. **Old log rows** (05): do not print the raw pipe row; either split its fields with labels or drop it; drop the *Question* section when it repeats the title; expand or drop "§3.3" and "004".
9. **Labels line**: drop the literal "(no class)" and "(area none: no label)". A label that is not there should not be written.
10. **Gloss or link the first use of a project term** ("group head", "net", "census"), or move the sentence to the collapsed part.

### What would make me stop reading

A *What happens* that says "filed and closed" (04). A section called *Where to look* holding a vote count (02). A title that is a path (02). A paragraph over about 150 words with no line break (04). A placeholder in a URL (all five).

## prediction

With changes 1 to 5 applied to the same five samples and a fresh reader asked the same (a) to (d) questions, **at least 18 of 20 cells will be read cleanly**, against 13 of 20 on the sketch (my count above). 02a and 05a (the problem) flip to found, because the problem line is a required field. The answer to (a), (b) and (c) is within the first 10 lines of every body. If the re-run reads fewer than 16 of 20 cleanly, my ranking failed. A second prediction: on 02d, a reader asked "did Go solve this?" answers "yes" on the sketch and "no" after change 6; the sketch's rate is unmeasured, and I would expect at least one of three readers to say yes.

## condition

I would withdraw or reweigh change 6 if the author keeps the rule that a line states only what the page does and shows that a stranger can answer "what did Heroes take?" from the sitting by a real link. I would drop changes 7 and 8 if a stranger run reads the samples as clearly with the noise left in. Any result where the reader finds the answer inside ten lines on the sketch as it is would also lower my verdict to approve.

## context

**Yes, my reading is void by the rule of my agent definition, until re-run from a copy outside the repository.** Here is what reached me beyond the five files and the brief, none of it opened by me:

- **`CLAUDE.md`** (the project's operating contract), delivered as project instructions. It says nothing about what a good issue looks like beyond: an issue is one file and one GitHub issue, and the item carries "Prior art".
- **The memory index** (`MEMORY.md`): titles and one-line summaries of local notes. Not relevant to the task.
- **`.claude/rules/records.md` and `.claude/rules/verification.md`**, in full, delivered through the system context a second time (the brief predicted exactly this). **`records.md` § The issues does tell me what the rendering is meant to be**, and I did not ask for it: the title opens "Defect NNN"; the body headings per kind; the `Prior art` form and the rule that its lines state only what the page states; "Fixed by" and "Settled by" comments; the Record permalink; commit hashes at the end of each section. So I **did** know in advance what a good issue is meant to look like, in the project's own terms. How it shaped my reading: I recognised `<the commit the run is pinned to>` as a placeholder rather than a mistake, I knew the headings *Where to look* and *What happens* come from a template, and I flagged the conflict in change 6 only because I knew the prior-art rule. Findings that rest on knowledge of the repository, and not on the five pages alone, are the template remarks and the rule remark in change 6; the marks in the tally are from the pages.

I did not open `00-shared.md`, `design.md`, any issue file or any project document, and I did not read the spec (this task gave none and needed none).

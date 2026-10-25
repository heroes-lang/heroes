# Panel 228, the historian's report

Seat: historian (advisory, no veto). Written 2026-10-25. Every precedent below names the page it was
read on; **every page was read on 2026-10-25**, and the date given beside a precedent is the source's
own (a release, a commit, an issue). Anything not confirmed on a page read that day is written
`unverified` in those words. Where a sentence joins two read facts into a third, it says *inference*.
The searches were for the brief's list plus what they led to (HLS, rustc's truncated candidate list,
GCC, SARIF's `kind`); a precedent outside that vocabulary may exist and was not looked for.

## verdict

**approve (advisory)**, of a `"holes"` member beside `"diagnostics"`, routes A, A' or D in shape.
**object (advisory) to C**: it is GHC's route, and GHC's own language server paid for it in regular
expressions over English (below). The schema number is **split by precedent** and this seat does not
settle it: the nearest precedent, a compiler's JSON diagnostics (GHC), bumps on every added member;
cargo does not bump, and pairs that with two mitigations route A as written lacks.

## precedents

### 1. Is a hole a diagnostic? Who kept it out, and what their consumers do

- **Agda keeps goals out of warnings and errors, in one JSON answer** (verified). Its
  `--interaction-json` (*"Added in version 2.6.1"*, *"For use with other editors such as Atom"*;
  https://agda.readthedocs.io/en/latest/tools/command-line-options.html) encodes
  `Info_AllGoalsWarnings` as `kind "AllGoalsWarnings"` with four members, `"visibleGoals"`,
  `"invisibleGoals"`, `"warnings"`, `"errors"`; the place-list is a separate response, `kind
  "InteractionPoints"` with `"interactionPoints"`; a goal's content another, `kind "GoalSpecific"`;
  and the JSON carries **no protocol version** (the only `"version"` is Agda's build version)
  (https://raw.githubusercontent.com/agda/agda/master/src/full/Agda/Interaction/JSONTop.hs, master).
  The constructor is `Info_AllGoalsWarnings Goals WarningsAndNonFatalErrors`, the second *"Assorted
  warnings and errors to be displayed to the user"*
  (https://hackage.haskell.org/package/Agda-2.6.1.3/docs/Agda-Interaction-Response.html). A hole is
  not an error there: reloading a file whose only gap is `?`, *Agda "no longer throws an error"* but
  shows the goals buffer
  (https://agda.readthedocs.io/en/latest/getting-started/a-taste-of-agda.html). **This is route A's
  shape: goals beside the non-fatal errors in one answer.** Whether Agda still reports goals after a
  fatal type error is `unverified`.
- **Idris 2 lists holes by a command of their own** (verified): `(:metavariables WIDTH)`, *"List the
  currently-active holes"*, beside a distinct `(:warning ...)` reply; and the protocol is versioned,
  `(:protocol-version MAJOR MINOR)`, *"IDE Clients can use this to help support multiple Idris
  versions"*, Idris 1 at version one and Idris 2 (v0.3.0) at two
  (https://idris2.readthedocs.io/en/latest/implementation/ide-protocol.html). A hole's IDE record is
  `name`, `type` and `context`, each premise `name` and `type`, the types rendered by `show`, **with no
  cap on the premises** (`holeIDE`, `premiseIDE`, `extractHoleData`;
  https://raw.githubusercontent.com/idris-lang/Idris2/main/src/Idris/IDEMode/Holes.idr, main).
- **Lean 4 splits it: a marker in the diagnostics, the goal by a separate request** (verified).
  `sorry` logs a **warning**, *declaration uses `sorry`*, under the option `warn.sorry` (default
  true), and **only `if !(← MonadLog.hasErrors)`**, so beside an error the marker is not logged
  (https://raw.githubusercontent.com/leanprover/lean4/master/src/Lean/AddDecl.lean, master). A `_` the
  elaborator cannot fill is an error (*"Otherwise, a hole is an error"*;
  https://lean-lang.org/doc/reference/latest/Terms/Holes/); that error's wording is `unverified`. The
  goal itself travels by custom LSP requests, `$/lean/plainGoal` (`rendered : String` and `goals :
  Array String`) and `$/lean/plainTermGoal`, *"Returns the expected type at the specified position,
  pretty-printed as a string"*
  (https://raw.githubusercontent.com/leanprover/lean4/master/src/Lean/Data/Lsp/Extra.lean); a
  structured form, `InteractiveGoal` with `type : CodeWithInfos`, serves the infoview
  (https://raw.githubusercontent.com/leanprover/lean4/master/src/Lean/Widget/InteractiveGoal.lean).
  **Its consumer**: Emacs `lean4-mode` sends both plain requests and renders the goal in its `*Lean
  Goal*` buffer, with the diagnostics fetched separately and shown in the same buffer under *Messages
  here* (https://raw.githubusercontent.com/leanprover-community/lean4-mode/master/lean4-info.el). One
  view, two sources.
- **GHC makes the hole a diagnostic, an error by default** (verified). *"A "Found hole" error usually
  terminates compilation, like any other type error"*; `-fdefer-typed-holes` *"converts them into
  compile-time warnings"* (https://downloads.haskell.org/ghc/latest/docs/users_guide/exts/typed_holes.html,
  the 9.14.1 guide); code GHC-88464, every example labelled `error:`
  (https://errors.haskell.org/messages/GHC-88464/). In its JSON (schema below) `severity` is
  `"Warning"` or `"Error"` and `message` an array of strings, so a hole's fits reach a machine as
  English lines (*inference* from the schema and the error page). Typed holes entered in **GHC 7.8.1**
  as the default warning `-fwarn-typed-holes`
  (https://downloads.haskell.org/ghc/7.8.1/docs/html/users_guide/release-7-8-1.html; the release's day
  `unverified`); valid hole fits in **8.6.1**
  (https://downloads.haskell.org/ghc/8.6.1/docs/html/users_guide/8.6.1-notes.html; day `unverified`).
- **What GHC's consumer did with it** (verified). The Haskell Language Server reads the fits out of the
  diagnostic's text: `=~ " *Valid (hole fits|substitutions) include"`, both wordings GHC has used, the
  name taken as the text before the first `:`, and the line `Some hole fits suppressed` filtered out
  because *"GHC prints this line where we expect a hole fit to be"* (PR 4288, merged 2024-06-09,
  https://github.com/haskell/haskell-language-server/pull/4288 and its `/files`). Issue 4198
  (2024-04-30): a code action on a `_` gave *"1800 (!) suggestions"*
  (https://github.com/haskell/haskell-language-server/issues/4198); HLS had set `maxValidHoleFits =
  Nothing -- quantity does not impact speed` and now sets 10. Asked whether users lose the
  suppression notice, the author answered there is no good way to convey it in completions (PR 4288).
- **SARIF can hold a non-problem in its results list** (verified): `result.kind` takes `"pass"`,
  `"open"`, `"informational"`, `"notApplicable"`, `"review"`, `"fail"`, default `"fail"`; for any kind
  but `"fail"`, `level` SHALL be `"none"`, *"The concept of "severity" does not apply"* (SARIF 2.1.0
  Errata 01, OASIS, 2023-08-28, §3.27.9-10,
  https://docs.oasis-open.org/sarif/sarif/v2.1.0/errata01/os/sarif-v2.1.0-errata01-os-complete.html).
  The one precedent read under which route C would not need a severity.
- **Scala's `???`**, the same spelling, earlier (whether Heroes took it from Scala is `unverified`):
  *"can be used for marking methods that remain to be implemented"*, throws `NotImplementedError` when
  invoked (https://www.scala-lang.org/api/2.13.x/scala/Predef$.html); the page says nothing of a
  compile-time report, and the version that added it is `unverified`.

### 2. Versioning a checker's machine answer when it gains a member

- **GHC bumps a minor version for every added member** (verified). `-fdiagnostics-as-json`, JSON
  Lines on stderr, is in the 9.10.1 guide with schema `1_0`
  (https://downloads.haskell.org/ghc/9.10.1/docs/users_guide/using.html; tag pushed 2024-05-10,
  https://mail.haskell.org/pipermail/ghc-commits/2024-May/061448.html): seven required properties,
  `additionalProperties: false`. The 9.12.1 guide links schema `1_1`
  (https://downloads.haskell.org/ghc/9.12.1/docs/users_guide/using.html), which adds an **optional**
  `reason` (the two schemas compared by this seat). Commit of 2025-07-24, shipped in 9.14.1
  (2025-12-19, https://www.haskell.org/ghc/): `schemaVersion` `"1.1"` to `"1.2"` for an optional
  `rendered`, with the comment *"If the content is altered in a backwards compatible way, update the
  minor version (e.g. 1.3 ~> 1.4)"*
  (https://mailman.haskell.org/archives/list/ghc-commits@haskell.org/thread/EOLMEM76HJUSX2V5JFKJFNPEJCP2QFPN).
- **cargo keeps 1 and adds fields** (verified): *"Adding new fields — New fields will be added when
  needed. Reserving this helps Cargo evolve without bumping the format version too often"*; `"version":
  1`, *"changed if incompatible changes are ever made"*; and two mitigations: `--format-version` (*"It
  is recommended to include the `--format-version` flag to future-proof your code"*) and at least one
  field annotated with the release that added it, `dep_kinds`, *"Added in Cargo 1.40"*
  (https://doc.rust-lang.org/cargo/commands/cargo-metadata.html). Panel 193's historian read cargo
  rightly; the mitigations are not in that reading as the brief quotes it.
- **rustc has no version at all** (verified): *"New fields may be added. Enumerated fields like
  "level" ... may add new values"*, and a `rendered` field holds the text form
  (https://doc.rust-lang.org/rustc/json.html). cargo's build messages: one JSON object per line on
  stdout, a `reason` per kind, no version, and *"only interpret a line as JSON if it starts with `{`"*
  because other output shares that stream (https://doc.rust-lang.org/cargo/reference/external-tools.html).
- **LSP versions by capabilities, not numbers** (verified): *"Features in the spec are kept compatible
  using so called capability flags"*; unknown properties are ignored
  (https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/).
- **SARIF fixes the string and extends by property bags** (verified): `version` *"SHALL have the value
  "2.1.0""*, placed first so parsers can *"handle multiple versions"*; every object may carry a
  `properties` bag for what the format does not specify (same document, §3.8, §3.13.2).
- **GCC's unversioned JSON was retired for SARIF** (verified as fact, no reason stated): added in GCC 9
  (2019-05-03), *"deprecated"* in GCC 15 (2025-04-25), *"removed"* in GCC 16 (2026-04-30), each time
  *"Users seeking machine-readable diagnostics from GCC should use SARIF"*
  (https://gcc.gnu.org/gcc-9/changes.html, `gcc-15/changes.html`, `gcc-16/changes.html`,
  https://gcc.gnu.org/releases.html). That its lack of a version caused this is `unverified`.

### 3. Capping the suggestions handed to a machine

- **GHC**: `-fmax-valid-hole-fits` and `-fmax-refinement-hole-fits`, default 6, `-fmax-relevant-binds`
  default 6, each with a `-fno-` form for no cap; fits sorted by default, *"the most relevant hole fits
  at the top"*; a cut says it was cut without a count, *"(Some refinement hole fits suppressed; use
  -fmax-refinement-hole-fits=N or -fno-max-refinement-hole-fits)"* (typed_holes.html, and
  https://downloads.haskell.org/ghc/latest/docs/users_guide/using-optimisation.html) (verified).
- **rustc writes the count and the machine got no more** (verified): *"and 17 others"*; *"`--message-format
  json` is not enough"* (forum, 2024-06-04,
  https://users.rust-lang.org/t/how-can-i-get-a-list-of-the-and-17-others-types-in-this-compilation-error/112458);
  issue 125984 asked `--verbose` to print the whole list, closed completed 2024-06-12
  (https://github.com/rust-lang/rust/issues/125984).
- **LSP's machine form of a cut is a boolean**: `isIncomplete`, *"This list is not complete. Further
  typing should result in recomputing this list"* (LSP 3.17 page above) (verified).
- **Idris 2**: no cap on a hole's premises (above) (verified). Agda and Lean: whether they cap a goal's
  context is `unverified`.
- **No precedent read truncates a list in silence**, as `nearby` does: GHC says *suppressed*, rustc
  says how many, LSP sets a flag. A question rather than a finding: it rests on the five tools read.

### 4. Each route: the precedent it follows, the one it breaks

- **A**: follows Agda's `AllGoalsWarnings` (goals a member beside warnings and errors, unversioned) and
  cargo's additive rule; breaks GHC's minor bump, and takes cargo's rule without cargo's two
  mitigations (a consumer-chosen format version, a field's introducing release written down).
- **A'**: one answer in two renderings follows rustc's `rendered`, GHC 1.2's `rendered` and Lean's
  `PlainGoal.rendered`; breaks them in **where** the rendering goes: every one puts it **inside** the
  machine document, none on a second stream.
- **B**: follows cargo's and GHC's JSON Lines (several objects, a kind each, one stream) and Idris's and
  Lean's separate answer for holes; breaks the single RFC 8259 document Heroes' readers take, and meets
  the hazard cargo documents, non-JSON on the same stream.
- **C**: follows GHC (hole an error, a warning when deferred) and Lean's `sorry` warning; SARIF's
  `kind: "informational"` with `level: "none"` is the only form read that would carry it without a
  severity; breaks Agda, Idris and Lean's goal channel, and GHC's history shows the consumer it invites
  (HLS's regexes, two wordings, a filtered suppression line).
- **D**: follows GHC's bump-on-addition and Idris's `MAJOR MINOR`; breaks cargo's rule. GHC bumps a
  **minor**; Heroes' `schema` is one integer, so D is a major bump where GHC used a minor (*inference*).
- **E**: follows Lean (a marker in the diagnostics, the goal elsewhere) and Agda's `InteractionPoints`
  (the places) beside `GoalSpecific` (the content); answers the reader who must know a hole is there,
  as Lean's warning does, not the one who must fill it.
- **Routes nobody listed, each with its precedent**: (i) cargo's `--format-version`: the consumer names
  the version it reads and an older compiler refuses the flag at exit 2 instead of a consumer crashing
  (that the older `heroes` exits 2 on an unknown flag is `.claude/rules/cli-surface.md`'s contract as
  the brief states it, not run by this seat); (ii) A with the report's rendering as a member of each
  hole (rustc, GHC 1.2, Lean), stdout empty; (iii) GHC's minor transposed, a second number that an
  older document lacks.

### 5. The brief's other questions, where a precedent speaks

- **Holes beside errors** (question 1): Agda answers goals and non-fatal errors together. Lean drops
  its `sorry` marker when errors are logged; whether its goal request still answers beside an error is
  `unverified` (the handler was not read).
- **Types** (question 3): every precedent read hands a type as a pretty-printed string in the
  language's own syntax (Idris 2 `show`, Lean `PlainTermGoal.goal : String`, GHC's message lines);
  Lean alone adds a structured form, for its own infoview. Whether any of them qualifies a type by its
  module as the file must write it was not checked.
- **The order of landing, `--apply --json` and the cap's sentence** (questions 4 to 6): no precedent
  was searched for, and none is offered.

## argument

Every proof assistant read (Agda, Idris 2, Lean 4) gives a hole's goal a channel of its own beside the
diagnostics, and Agda's JSON puts goals beside warnings and errors in one answer: route A's shape. The
one compiler that made the hole a diagnostic, GHC, left its language server reading fits by regex over
two wordings and filtering GHC's own suppression line; C repeats that. On the number, cargo keeps 1
while adding fields but annotates a field's release and lets the consumer request a format; GHC, the
nearest precedent, bumps a minor per added member. A without either mitigation is cargo's rule half
taken. And no precedent read truncates a machine's list silently, as `nearby` does.

## condition

My reading changes if: (1) a compiler's JSON diagnostics added a member under an unchanged integer
version **without** a per-field release note or a consumer-chosen format and drew no consumer breakage
(that makes bare A the precedent); (2) a tool kept a hole as a non-error diagnostic **with its fits as
structure**, and its consumers did not scrape text (that rehabilitates C, probably as SARIF's
`kind`); (3) a tool's machine format truncates a suggestion list silently and was not reported for it
(that clears `nearby`'s asymmetry).

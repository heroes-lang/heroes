# Panel 244, the brief every seat reads first

Convened 2026-11-10 by the coordinator of batch 30, in M-core-packages, on the author's
instruction of that day, meant as: *document packages well on the site and in the guides, with
the reasons behind each choice, and put what is missing into the specification*. The guide is a
lane's (`b30-docs`); this sitting rules on **the specification's half**, a change to the language's
text (CLAUDE.md § 4). Seats: spec-warden and compiler-engineer, and the completeness critic before
and after. The llm-ergonomist's blind reading is a paid run and is **not** named here; the
coordinator asks the author for it, and the synthesis records whether it ran.

## The base

`main` at `89db84347`, its stage 2 compiler `sources 14e9e5ce168a2336`. Each seat with Bash works
in its own detached worktree `.claude/worktrees/244-<seat>` at that commit, builds its own compiler
from the seed in two stages, probes in `.claude/worktrees/p244-scratch/<seat>/`. One heavy command
at a time. No paid run (no `measure --refresh`, no `claude -p`), no credential, no GitHub write,
no push, no file outside the repository's root. Reports to `docs/panel/244-reports/<seat>.md` in
`.claude/worktrees/p244`, or, if that write is refused, your final message is the report.

## What the specification says today, measured

Spec § 1 (`spec/heroes-spec.md:21` on, `grep -n '^## 1\.'`) holds the package text panel 235's R13
chose, the short draft with its catalogue example (`docs/panel/235-*.md:491-494`: the critic
refused to drop the example, 236 of 240 readers having copied it). Counted on 2026-11-10 with
`grep -c -i <word> spec/heroes-spec.md`: `internal` 0, `heroes.packages` 0, `path =` 0, `heroes
fetch` 0, `vendor` 0, `private_module` 0, `fingerprint` 1, `heroes add` 1. The spec reads
`real 10898` tokens (`heroes measure spec/heroes-spec.md`, pinned 2026-11-09) against the ceiling
of 16,384 (`tests/harness/suite_spec.hero:162`).

## What the compiler holds that the specification does not say (each to be run by the seats)

- **`internal/`**, panel 235's R16 (`docs/panel/235-*.md:758`): a module under an `internal`
  folder belongs to the module before the last `internal` part, and only it and the modules under
  it name it; `private_module` otherwise (`selfhost/module/package_told.hero:259`,
  `selfhost/placed_use.hero` `internal_owner`); it binds the program's own `internal/` folders too.
  R16 landed after R13's text; no record found by the coordinator says it was kept out of the spec
  on purpose (searched: `grep -n -i 'spec' docs/panel/235-*.md` for `internal`, `R16`), a question
  rather than a premise.
- **Where fetched code stands**: `heroes fetch` places every package the catalogue names under
  `heroes.packages/` beside the catalogue, each checked against its sum; `build`, `check` and `run`
  never fetch (`selfhost/cli/table.hero:184`'s summary); a placed tree is held to its `sum` at build
  (lane b29-envfetch's report, unrun by the coordinator: run it).
- **The sources of an entry**: an index name, a `git` address with a tag and commit, a folder
  (`path`), `heroes add`'s three forms (`table.hero:183`); spec § 1's example shows the first two.
- **`use <entry>` alone** (`package_wants_a_module`), and a `use` naming a module the package has
  not (`unknown_package_module`).

## The question

Which of these the specification states, in what words, at what cost, and which it leaves to the
compiler's messages, by Principle 0 (CLAUDE.md § 2) and design.md §1.6's budget: a sentence enters
where a reader holding only the specification would otherwise write a wrong program or be unable
to predict a refusal, measured, never because it is true. For each candidate: the sentence, its
vendored token count, the plausible mistake it prevents and the error the compiler gives that
mistake today (run), and whether that error alone already tells the reader what the sentence would.

## After the critic's first pass (2026-11-10, `244-reports/critic-briefs.md`, 20 items)

Read the critic's report whole; it overrides what is written above where they differ.

- **The author's instruction** was given in this conversation on 2026-11-10 and is recorded here
  first (item 19).
- **The seats** are the full panel but the blind seat: spec-warden, compiler-engineer,
  ffi-pragmatist (the C boundary of packages, item 5) and historian (how other languages' specs
  state these rules: does Go's language specification state `internal`, or only the go command's
  documentation; item 15). The blind seat is a paid run, asked of the author.
- **The scope widens** (items 3, 5, 6, 12): spec § 1's present sentence is false for a `path`
  entry (no version, no sum; a version beside a `path` is refused; a package's own catalogue may
  hold no `path`), so that candidate is a correction; the C boundary of packages (panel 235's R9
  C1 to C5, panel 237's R2 and R3, defect 2357: a fetched package's header constant refused
  `ffi_constant_type` where the program's own builds; `package_reads_outside`; the package-folder
  stand-in refusals), with § 13 its home; **task 2402** (§ 13:374 still says JSON comes from C
  libraries, while `encoding/json` ships in core 0.2.x), whose rewrite is the natural named
  removal; the catalogue's own rules (an unquoted version, a `[package]` table, a version range,
  the `index` line every example carries and the spec never states, `fingerprint` in the spec
  where the key and every message say `sum`).
- **`use <entry>` alone is already in the grammar** (`"<" ident ">" "/" ident`, a parse-time
  refusal); drop it from the list (item 1).
- **`internal`'s rule as the compiler applies it** is Go's element rule, a path part `internal`
  after the first (shapes `p4` to `p6b`): the drafted sentence states that rule (item 4).
- **The payment rule** (item 7): an addition needs a named removal or a pre-registered falsifiable
  prediction whose instrument exists today (`heroes measure`'s own words; design.md:322-334).
  **The headroom** is 5,426 against the ceiling, the FFI floor mortgaging 60 (item 8). **How a
  draft is priced** (item 9): `measure` from the worktree's root with the draft in place in the
  spec, each vendored delta written as a lower bound in those words, a merged draft against an
  appended one, and a W-bound registered for the landing's `--refresh` (a paid call, the author's).
- **The 1.0.0 promise** (item 16; panel 232, `docs/panel/232-*.md:264-276`): a rule in the spec
  is promised in substance, a rule left to the messages only by its code. Say, for each candidate,
  which promise the author takes on.
- **What moves under the sitting** (item 10): lanes b30-pkg (2431, committed) and b30-fetch (2433)
  change messages this sitting judges; judge on `89db84347`'s messages and name a lane's change
  where it moves a verdict. **Panel 243** sits on the same base and may change § 13's lines
  379-382: this sitting prices against 89db84347 and the landing re-prices against 243's (item 18).
- **Messages that do not carry their rule** (items 1, 11): `package_wants_a_module` offers a
  `parse` the package has not; a sum or version mistyped by one character is told *not placed*
  rather than `sum_mismatch` until `heroes fetch` runs; filed apart as defects.
- **The default index** `https://index.heroes-lang.org` (`fetch/index_source.hero:46-50`) waits
  on ROADMAP row 78: does § 1 promise a verb whose default source does not exist yet (item 13)?
- **The cheap route to a package** (item 17): `add core --index file://<path to packages>`; a
  hand-made index is `versions`, `<v>/list` and `<v>/files/`, as the critic's `idx/cpkg` shows.

## A question added by the author, 2026-11-10 (Q-stub)

The author, in this conversation, meant as: *perhaps a `heroes` command is needed that creates a
project stub with the folders for a library, say `heroes make lib` or `make app`, as cargo does*;
asked whether it enters (the stopping rule, `.claude/rules/cli-surface.md:18-38`, admits a new verb
by the author's decision only, as `heroes probe` entered on 2026-09-26), the author answered
*merge the sittings*: the verb enters, and this sitting rules its shape beside the specification's
package text. Today `heroes` has 15 verbs (`selfhost/cli/table.hero:263`, `assert table.len() == 15`)
and none writes a project; `gh issue list --state all --search 'scaffold OR "heroes new" OR "heroes
init"'` and a grep of design.md and `docs/panel/` for `scaffold|heroes new|heroes init|cargo new`
found no proposal or refusal before this one.

What the sitting rules: the verb's name (`make`, `new`, `init`, or another; the author's `make`
first among them) and its operands, in the project's vocabulary, where *library* is a C library
(spec § 13) and Heroes code others publish is a *package* (spec § 1; panel 240's words), so the
author's `lib` and `app` are weighed against `package` and `program`; whether it writes into a new
folder or the current one; what it writes for each (a program: `main.hero` and no catalogue, a
program that uses no package having none; a package: the layout `examples/packages/README.md`
teaches, a module, its card `.md`, `internal/`, its tests, and how it is reached by a `path` entry
while developed); what it refuses (an existing folder, a name no `use` writes); its exit codes and
output by the contract of every invocation; and what the site's verb count and the spec owe. The
compiler-engineer prices it; the spec-warden rules on whether the spec names it; the historian
reads `cargo new`/`cargo init`, `go mod init`, `dart create`, `zig init`, `swift package init`,
`gleam new` and what each writes.

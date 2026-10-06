# Panel 193, the compiler-engineer's brief

Read `00-shared.md` beside this file first; it holds the question, the
measured facts and your directory, `<scratchpad>/193-compiler-engineer/`.

## Your task: build the route and prove it against the compiler's own applier

1. In your copy, make `check --json` write each fix's place
   (`selfhost/cli/check_json.hero`, 65 lines; `selfhost/diag.hero:45-50` for
   `Fix`, its `span: token.Span`; `selfhost/token.hero` for `Span`; the line and
   column of a place come from `source`, whose `locate` counts a column from
   the line's start, defect 256's note). Choose the fields you would defend
   and say why: byte offsets of the replaced range, half-open; line and column
   of its start and its end; the column's unit, which you measure for today's
   diagnostic `"col"` on a line holding a character above ASCII before the
   error (bytes or characters?); whether `"schema"` moves.
2. **Prove it against the applier as it is** (repaired after the critic's
   pass, see the shared brief): `--apply` runs up to 64 rounds, writes twins
   once, holds a touching fix to the next round, and writes nothing outside the
   root file. A consumer written in your scratch (a script there is a
   measurement, not a tool of the tree) reads only your JSON (from **stderr**)
   and applies every `certain` fix; compare it byte for byte with what
   `check --apply` writes, over every case under `tests/golden/check/` with a
   `.fixed` or an `.applied` file **and over the surface fixtures' answer
   files**, the `certain137-*` cases (the only multi-round ones) and `applyx`
   (the only fix outside its root) among them, and over 178's case. Report the
   counts, agree and differ, and why each difference; a difference that is the
   one-pass consumer missing a round is the critic's question answered by
   measurement, and the route that closes it (for example `--apply --json`
   answering the edits `--apply` writes) is yours to build or to refuse with
   its reason. Offsets are into one concatenated text of every file
   (`source.hero:14-24`): say what your route writes for a file's own
   coordinates.
3. **The shapes beside**: a fix whose span is empty (an insertion); a fix at the
   end of the file; a fix on a line holding characters of two to four bytes; a
   fix in a file with `\r\n` line ends (does the compiler read one?); two fixes
   of one diagnostic on two lines; a fix whose span lies in another file than
   its diagnostic's, if any of the 113 sites makes one (a question to answer by
   reading, naming what you searched).
4. **Cost**: the JSON's size before and after over the check corpus, and
   instructions retired (`/usr/bin/time -l`) for `check --json` over the
   largest case before and after. No duration: other work runs on this Mac.
5. **What the route touches**: every reader of the JSON the shared brief lists
   (the harness's `fix_names.hero`, `suite_surface.hero`'s rows, the site's
   `llms.txt` sentence), and whether each still holds.

Print, for the blind seat, into `<scratchpad>/193-compiler-engineer/blind-json/`:
178's case **with every comment removed** (its header describes the fix, and a
`#~` mark names each diagnostic, so the case as committed gives its answer
away; the critic's finding), as `case.hero`; today's answer for it as
`a.json` and your route's as `b.json`, each the compiler's stderr; and what
`check --apply` writes for it as `applied.hero`. Check that the stripped case
still draws the same ten fixes, and say so.

# Panel 240's landing: the brief of the lane `b27-words`

**The lane**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/b27-words`,
branch `b27-words`, opened at `d723e92d6` (the round `b27-round`'s HEAD,
2026-11-09 22:27). Work there only. Build its compiler there, in two stages
from the seed: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`,
then `./heroes build selfhost/main.hero -o heroes-next`, then
`./heroes-next build selfhost/main.hero -o heroes-s3` and `cmp heroes-next
heroes-s3` for the fixpoint. The seed is not regenerated (the
compiler-engineer measured that `selfhost/` writes no extern group).

**What to land**: panel 240's resolution, R1 to R8, in
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/b27-scratch-round/p240/synthesis.md`,
with every seat's condition: the reports are in the `reports/` folder beside
it (`compiler-engineer.md`, `spec-warden.md`, `historian.md`, and the critic's
`critic-briefs.md`; a second critic pass, `critic.md`, may land while you
work: read it when it appears and follow what it finds that bears on the
landing).

**Start from the compiler-engineer's prototype**:
`git apply --3way /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/b27-scratch-round/p240/reports/compiler-engineer.diff`
(it applied cleanly at 22:28; if the file changed since, apply the newest).
Read its report's §§ 2, 4, 5 and 6 for what it did and did not finish. Then
complete, in this order:

1. **Spec § 13** in the spec-warden's T1 wording (its report, and
   `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/240-spec-warden/.p240-drafts/pc2-t1.md`
   lines 479 and 482): *"Instead of `link`, `extern "raylib.h" pkgconfig
   "raylib"` asks the system where its headers and libraries are and what else
   it needs."* and `( "link" | "pkgconfig" )`. If the prototype wrote another
   wording, replace it with T1. **Do not run `heroes measure --refresh`**: the
   coordinator runs it after you, with the author's yes, and writes the pins
   and the ledger row (`spec/budget`, `spec/spendable`, `spec/real`,
   `spec/ledger` stay red until then, and only those).
2. **The compiler**: the mark `pkgconfig` (`WORD_PKGCONFIG`, the AST case
   `.pkgconfig`), the codes `ffi_pkgconfig` and `pkgconfig_comparison`
   (`no_pkg_config` unchanged), the driver marker `heroes-ffi-pkgconfig`,
   `shadowed_words`' `PKGCONFIG`, `old_package_word` and `pkgconfig_spelled`
   each with a `certain` fix writing `pkgconfig`, and no message that reads
   "this pkgconfig's name" (the spec-warden's condition 3: the wrong-mark
   message names both words that may stand there). Every diagnostic class
   gets its cases, annotated in the source (`#~ <code>`) as well as
   snapshotted (`.claude/rules/diagnostics-and-goldens.md`).
3. **The tree moved by the fix**, the precedent `fcec76d8d`: every program
   holding the old mark (`git grep -lE 'extern "[^"]*" package "'`, 70 files at
   the base) rewritten by `./heroes-next check --apply --in-place` (or `fmt`
   where the precedent did), never by hand-sed of code lines; comments that
   quote past messages and golden case NAMES stay as they are (records); the
   `.expected` files follow the new compiler's output and each is read before
   it is written (`UPDATE_GOLDEN=1` does not exist and is never used).
   `.hero.frozen` recovery-corpus programs are records: do not edit them; say
   in your report whether any suite reads them differently now.
4. **Every reader in the same change** (R6): the TextMate grammar
   (`editors/vscode/syntaxes/heroes.tmLanguage.json`), `editors/README.md`,
   `site/src/lib/highlight.ts`, `site/src/lib/examples-model.ts`'s regex,
   `tests/harness/suite_emission.hero` (its ` package "` reader, which would
   otherwise SKIP raylib and SDL as a missing header), `tests/harness/absence.hero`,
   `tests/harness/probe.hero`, the `suite_golden` fixtures, `suite_compat`'s
   `known()` row for `old_package_word`; and **a new check that holds the
   spec's `Extern` production's marks to the compiler's `WORD_*` constants in
   both directions** (the spec-warden's condition 1), in the suite that already
   reads the spec's productions (`tests/harness/suite_grammar.hero`), with a
   test that makes it fire.
5. **The words around it**: `docs/design.md`'s sentences naming the mark,
   `.claude/rules/c-boundary.md`, `site/src/html/docs/the-c-boundary.html` and
   its Italian twin (the Italian written in Italian), the examples' READMEs.
   The internal names (six C-sense files, 293 identifiers) are R8's own later
   commit: leave them.
6. **The emissions**: bless **only** the cases that read red, each by name,
   `UPDATE_EMISSION=1 ./heroes-next run tests/harness/main.hero -- ./heroes-next
   emission <case>`, reading each moved file before and after: the eleven new
   cases of defects 2388, 2389 and 2395 (`run/fixedbugs-2388-*`,
   `run/fixedbugs-2389-*`, `run/fixedbugs-2395-*`) and the cases whose source the
   rename edits (their `hero_program_sources` line moves). Never the whole corpus.
7. **Format at the moment of writing**: `./heroes-next fmt <file> --in-place` on
   every `.hero` you touch.

**Verification, once, at the end** (the author's instruction of tonight: no net
twice, only the suites the change touches, aggregated in one command): rebuild
the two stages and the fixpoint, then the compiler's own tests
(`./heroes-next test selfhost/main.hero`) and, in one command, four at a time
(`xargs -P 4`), each suite into its own log file under
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/b27-scratch-round/p240/landing/`:
`check unsupported permissive full annotations fixes grammar spec special
surface probe compat corpus emission emit determinism order layout records
unseen canonical`, then `cache` alone. Then the net's own tests
(`./heroes-next test tests/harness/main.hero`). Never pipe a harness run into
`head` or `tail`. Report each count; a red is answered and its suite read
again, and only that suite.

**Then commit, by pathspec, in the lane**: before committing, `git merge
b27-round` into `b27-words` (the round will hold panel 240's own commit by
then; merge, never rebase). New files are staged by name in a separate command
from the commit; the commit is `git commit -F <msg> -- <paths>`, never `git add
-A`, `.`, `-u`, `commit -a` or `stash`. The commit's subject: `Panel 240's
landing: the mark that asks pkg-config is spelled pkgconfig, and package means
code others publish`; its body about fifteen lines: what moved, the counts,
the suites read, and that the spec's pins wait on the coordinator's refresh.

**Never**: push, amend, rebase, print a credential, start a paid run
(`claude -p`, an API call, `measure --refresh`), build or write inside
`b27-round` or another lane's worktree. Write your report as you go to
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/b27-scratch-round/p240/landing/report.md`.
Everything you write is English.

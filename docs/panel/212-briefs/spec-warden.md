# Panel 212, the spec-warden's brief

Read `00-shared.md` first, all of it (revised after the critic's pass); then
`.claude/agents/spec-warden.md`. You have a veto on budget breach (design.md §1.2, §1.6) and you hold
**Principle 0's burden of proof** (CLAUDE.md §2: a form enters v1 if the compiler needs it or provably
serves the thesis; neither, and it waits).

**Your question: is a fourteenth verb, which serves the project's records and not the language,
admissible, and what does it cost the budgets that exist?**

1. **The spec.** Build the compiler in your own worktree (as the shared brief says) and run
   `heroes measure spec/heroes-spec.md`: **offline it prints the recorded `real` row and exits 0**
   (the critic measured `real 10044 ... 2026-10-11` for the spec and `8167 ... 2026-09-29` for
   `CLAUDE.md`); only `--refresh` needs `ANTHROPIC_API_KEY`, and it is a **paid run this brief does not
   allow**. So say what you can: the proposal has no spec sentence, so the committed spec's recorded
   figure does not move; a changed spec could only be re-priced by `--refresh`, **unrun here**.
2. **The contract.** The same for `CLAUDE.md`; `DELTA_GATE` is 50 vendored tokens a commit
   (`tests/harness/suite_spec.hero:189`). Would implementing this need a sentence in `CLAUDE.md`
   (§ 3 and the cli-surface rule already name the verb's sitting)? If so its size and what pays.
3. **Principle 0 and §10's stopping rule**, honestly. List the 13 verbs (`selfhost/cli/table.hero`),
   say for each whether it serves the language (`check`, `build`, `fmt`) or the project's own
   instruments (`measure`, `mutate`, `probe`, `doctor`, `grammar`, `this`). The decisions that admitted
   the second kind are `docs/panel/016-command-surface.md`,
   `docs/panel/179-the-formatter-s-probe-is-a-verb-and-its-oracle-is-ruled-before-it-lands.md` and the
   paragraph of `.claude/rules/cli-surface.md` that says panel 179 ruled `probe` by an author decision
   *"which the stopping rule was not stretched to cover"* (the critic: a broad grep for `heroes measure`
   returns 336 files and names no panel). Quote the arguments, then say whether **this** verb stands on
   the same ground, and what would have to be true for the refusal route (R6, R3, or R11: a `.hero`
   program run with `heroes run`, no new surface) to be right. A refusal is held to the same standard
   as a feature (CLAUDE.md §12): name the program or compiler fact that would make yours wrong.
4. **The site.** A fourteenth verb needs `thirteen` to become `fourteen` in `site/src/html/start.html`
   **and `tredici` in `site/src/html/it/start.html`**, and `<code>issues</code>` on both pages
   (`site/src/lib/claims.ts:447-450, 488-491, 607-611, 690-691`). Count the lines with a command that
   does not miss the Italian (`git grep -n -iE 'thirteen|tredici' -- site/`) and say what a push that
   publishes the site costs this milestone, since the transcription is asked to be the last act.

Your report is `docs/panel/212-reports/spec-warden.md` in the lane tree. Numbers carry their command.

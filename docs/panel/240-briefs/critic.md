# Panel 240: completeness critic

Two passes. **First, before any seat starts**: read `00-shared.md` and the three
seat briefs beside it, and for every framing fact (a number, a path, a count, a
negative sentence, a word like *only* or *never*) run the command that settles
it at the base, commit `429464c94` (the round's tree at
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/b27-round/` holds that
commit, and its HEAD moves on while the sitting reads; read with `git show
429464c94:<path>` or `git grep ... 429464c94 --`). Report what you could not
verify, what is wrong, and the route or question the briefs do not list.
Write it to `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/b27-scratch-round/p240/reports/critic-briefs.md`.

**Second, after the seats**: read their reports and say what is missing: a
route nobody listed, a claim asserted and not measured, a contradiction between
seats with which is checkable, the question the sitting should have asked.
Write it to `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/b27-scratch-round/p240/reports/critic.md`.

No verdict of your own. No paid run, no credential printed, no commit, no push.

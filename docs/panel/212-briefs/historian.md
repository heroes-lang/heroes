# Panel 212, the historian's brief

Read `00-shared.md` first, all of it (revised after the critic's pass); then
`.claude/agents/historian.md`. You have no veto. **Every factual claim carries a source, read today
(2026-10-11), and a marker: `verified`, `unverified` or `inference`.** An unsourced precedent is
inadmissible. Write your report yourself to `docs/panel/212-reports/historian.md` in the lane tree,
as you go.

**Your question: who has moved thousands of issues into GitHub, how, and what happened to them?** In
order of how much it changes the design:

1. **GitHub's own documented limits**, each with the page and the sentence: the **primary** REST limit;
   the **secondary** limits on **content creation** (requests per minute and per hour, as GitHub states
   them) and on concurrent requests; what a violation returns and for how long; GitHub's published
   guidance for bulk creation (a pause between content-creating requests?); the limits on an issue
   **title**, **body**, **comment**, and on labels per issue; whether **issue types** and **sub-issues**
   have limits. The brief's arithmetic (F9, 5,918 requests) is an inference until GitHub's numbers are
   under it; say what it comes to, and how long the run takes at them.
2. **The Issue Import API, which is the author's standing question** (00-shared, item 4 of the author's
   instructions, and R4). Whether it exists today, is documented, who may call it; whether it sets
   `created_at`, `closed_at` and comment dates, **and whether anyone has used it in 2025 or 2026**; its
   status in GitHub's changelog or docs; what a read of it answers (the critic got a 404 citing
   `gist.github.com/jonmagic/5282384165e0f86ef105`). If you can show from sources that it works or does
   not, say which; if only a live write settles it, say exactly what the write would be and what it
   would leave behind, so the author can decide whether to allow it.
3. **Cross-references**: what GitHub documents about a `#N`, an `@name`, an `owner/repo#N` and a URL to
   another repository's issue in an issue body (the *mentioned* timeline event on the target, who sees
   it, whether it can be avoided without altering the text a reader reads). This matters because the
   46 Prior art sections credit other projects by link (F4).
4. **Precedent migrations and what they kept**: CPython's move from bugs.python.org to GitHub (2022:
   how `bpo-NNNN` was kept, the tool, the duration, what broke); Go's move from Google Code; others you
   can source (Mozilla, Kotlin, Jenkins, Gitea or GitLab importers, GitHub Enterprise Importer, the
   `gh-iff` extension the critic found). For each: the **old identifier** and where it went (title
   prefix, label, a link, a custom field), mentions and `#N` handling, dates, **notification volume**,
   rate-limit trouble.
5. **Issue fields and native issue types** (generally available 2026-07-02 per the changelog; the
   organisation has Task, Bug and Feature types): any project carrying an old tracker's number in a
   field, and the fields' limits.

**Attribution is part of your job**: where a precedent is what the sitting adopts, the synthesis gives
the decision issue a `## Prior art` line for it (`.claude/rules/records.md`, CL-082), `Taken from` or
`Reached first by`. Say which for each precedent and why; a precedent the sitting only compares itself
with is not prior art. **Each line you propose states only what the page itself says** (records.md, the
33 of 90 lines that said more than their page).

You make no request that changes anything. Reads of public pages only.

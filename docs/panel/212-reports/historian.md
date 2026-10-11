# Panel 212, the historian's report

Seat: historian, no veto. Written 2026-10-11 from pages read by WebFetch and WebSearch in this session.

**Method and its limits, said first.** This seat had no shell: no `gh`, no `curl`, no counting command. Every
claim below is from a public page read today; nothing was run against GitHub and nothing was counted in the
tree (so I give no count of my own). WebFetch returns a small model's reading of a page, not the page: a
sentence in quotation marks is what the tool returned as a quotation, and **each line proposed as Prior art
below must be re-read from its page by the session that files the sitting** (records.md: 33 of 90 lines said
more than their page). Markers: **verified** (a page read today says it, and I give the page), **unverified**
(a search snippet, a third party's claim, or a page I could not open), **inference** (mine, from verified
parts, labelled).

---

## verdict

**object (advisory)**, to three premises the brief carries as settled, not to the verb:

1. F9's arithmetic read as minutes of work. At GitHub's published content limit it is hours (item 1).
2. `.claude/rules/records.md`'s *a transcription cannot back-date GitHub's own dates* as a fact. A route that
   sets `created_at`, `closed_at` and comment dates existed and was reported working as late as February 2025;
   I found no report of 2026 either way (item 2).
3. `field.defect:NNN` as the double-creation guard. GitHub's search is documented as capped and rate-limited,
   GitHub's status page records a search lag in 2026, and a third-party reference calls the field qualifier
   experimental (item 1, last bullets). **inference**: the guard needs a second key that is not search.

---

## precedents

### 1. GitHub's own documented limits

All from GitHub's pages, read 2026-10-11.

**Primary REST.** "All of these requests count towards your personal rate limit of 5,000 requests per hour."
`GITHUB_TOKEN` in Actions: 1,000 per hour per repository. GraphQL: "5,000 points per hour per user." **verified**
(https://docs.github.com/en/rest/using-the-rest-api/rate-limits-for-the-rest-api and
https://docs.github.com/en/graphql/overview/rate-limits-and-node-limits-for-the-graphql-api).

**Secondary, same REST page.** "No more than 100 concurrent requests are allowed." (shared REST and GraphQL);
"No more than 900 points per minute are allowed for REST API endpoints" (most GET/HEAD/OPTIONS 1 point, most
POST/PATCH/PUT/DELETE 5); "No more than 90 seconds of CPU time per 60 seconds of real time"; and for content
creation "no more than 80 content-generating requests per minute" and "no more than 500 content-generating
requests per hour are allowed", with "Some endpoints have lower content creation limits." GraphQL: "No more than
2,000 points per minute", a mutation 5 points, and "Content creation limits include actions taken on the GitHub
web interface as well as via the REST API and GraphQL API." **verified.** So R8 (aliased GraphQL mutations) is not
a way round the content limit as the page states it; whether one aliased request counts once or N times is not on
the page. **unverified.**

**What a violation returns, and for how long.** "you will receive a 403 or 429 response" (primary also with
`x-ratelimit-remaining` 0). Wait: with `retry-after`, "you should not retry your request until after that many
seconds has elapsed"; with `x-ratelimit-remaining` 0, until `x-ratelimit-reset`; "Otherwise, wait for at least one
minute before retrying."; on repeated failure an exponentially increasing wait; "Continuing to make requests while
you are rate limited may result in the banning of your integration." And: "These secondary rate limits are subject
to change without notice." and "You may also encounter a secondary rate limit for undisclosed reasons." **verified**
(REST page above). How long a block lasts when no header is sent is **not stated** by GitHub.
Secondary blocks without `Retry-After` are reported by users: community discussions
https://github.com/orgs/community/discussions/32120 (opened 2022-09-06; a reply says the guidance promises
`Retry-After` and none appears) and https://github.com/orgs/community/discussions/50326 (2023-03-17: about
100-150 issues created from a GitLab export, 10 at a time with 90 s between batches, then a 403 "You have exceeded a
secondary rate limit and have been temporarily blocked from content creation.", `X-RateLimit-Remaining` 4820, no
`Retry-After`; a 2026-03-04 reply reports the same). **verified** that the reports exist; **unverified** as
to cause (no GitHub staff reply in either thread).

**GitHub's published guidance for bulk creation.** "To avoid exceeding secondary rate limits, you should make
requests serially instead of concurrently." and for POST, PATCH, PUT, DELETE "wait at least one second between each
request." (https://docs.github.com/en/rest/using-the-rest-api/best-practices-for-using-the-rest-api); the GraphQL
page repeats "you should pause at least 1 second between mutative requests". **verified.** The create-issue and
create-comment pages both say "This endpoint triggers notifications." and "Creating content too quickly using this
endpoint may result in secondary rate limiting." (https://docs.github.com/en/rest/issues/issues,
https://docs.github.com/en/rest/issues/comments). **verified.**

**Which requests count as "content-generating" is not defined on any page I read.** Whether the PATCH that closes
an issue counts against the 500 an hour: **unverified**. A third-party guide lists issue creation, comments and PR
changes as counting (search summary, https://fast.io/resources/github-api-rate-limiting-strategies/, unopened):
**unverified.**

**F9 at GitHub's numbers (inference, arithmetic mine).** The binding limit is the 500 an hour, not the primary 5,000
and not the 900 points a minute (180 mutations a minute at 5 points) nor the 80 a minute.
- All 5,918 requests of F9 counted as content-generating: 5,918 / 500 = **11.84 hours** (one every 7.2 s).
- Only creations and comments counted (2,014 + 1,952 = 3,966): **7.93 hours**.
- One request per issue, as an import route would be (2,014): **4.03 hours**, if that route counts at all
  (the gist says imports are "affected by regular API rate limits" and abuse limits; see item 2).
- The one-second pause alone: 5,918 s = 1.64 hours; it is not what binds.
- The primary limit is not binding: 5,918 requests is 1.18 hours of it. A second pass that edits bodies adds to the
  500 an hour if an edit counts (unverified); I did not count the cards that name a `defect NNN`.
- So a "stop at the first refusal; a rerun resumes" verb needs to **sleep** (Retry-After, else 60 s) rather than
  stop, or it stops about every hour. **inference.**

**Field limits.**
- Title: GitHub's create-issue page "does not state any maximum length for title or body" (read today). The 256
  figure is a community measurement from an error message: https://github.com/dead-claudia/github-limits ("Max length:
  256 characters", "The first verified by creating an issue with a very long title and confirmed with an error message";
  the page says not all are publicly documented). **unverified as documented, verified as a community report.**
- Body: 65,536. https://go.googlesource.com/build/+/823a5f284329c82522056e701c5b21cce25cc411%5E%21/ has the code
  comment "// As of 2025-11-27, GitHub GraphQL API limits body length to 65536."; the community page says "65536
  codepoints". The import route's gist says a body can be up to 65535 characters and a request 1 MB. Not in
  GitHub's docs. **unverified as documented** (third-party observation dated 2025-11-27). F2's 54,818-byte card plus
  the added header and permalink lines is under it by bytes, but the limit is in characters or code points, and a
  rendered body adds lines; I did not measure the rendered one.
- Comment: the same Go commit says "GitHub's limit on issue comment length applies to issue bodies too" and
  "limits comment length to 65536"; the community page says "262144 bytes" via REST v3. The two disagree.
  **unverified.** The closing comment is one line; immaterial.
- Labels per issue: no GitHub page gave a limit (the labels page states none; a search snippet cites 100 from an
  `actions/labeler` PR). **unverified.** The proposal uses at most three.
- Issue types: "You can create up to 25 issue types that your organization members can apply to issues." "The default
  types are task, bug, and feature." (https://docs.github.com/en/issues/tracking-your-work-with-issues/using-issues/managing-issue-types-in-an-organization). **verified.**
- Issue fields: "You can create up to 25 issue fields per organization." Types: Single-select, Text, Number, Date
  (https://docs.github.com/en/issues/tracking-your-work-with-issues/using-issues/managing-issue-fields-in-your-organization). **verified.**
  Creating one: `POST /orgs/{org}/issue-fields`, token "need the admin:org scope" and "the authenticated user must be
  an administrator for the organization"; `data_type` takes `text`, `date`, `single_select`, `multi_select`, `number`
  (https://docs.github.com/en/rest/orgs/issue-fields). **verified**; this matches F5's missing `admin:org`. A text
  field's length limit is not stated (same page and the fields page). **unverified.**
  Values on create: the create-issue endpoint takes `issue_field_values` (`field_id`, `value`), `type`, `milestone`,
  `labels` ("Only users with push access can set labels for new issues", "silently dropped otherwise") and
  `parent_issue_id`. **verified** (create-issue page).
- Sub-issues: "You can add up to 100 sub-issues per parent issue", "up to eight levels of nested sub-issues"
  (https://docs.github.com/en/issues/tracking-your-work-with-issues/using-issues/adding-sub-issues). **verified.**
- Issue fields GA: https://github.blog/changelog/2026-07-02-issue-fields-are-now-generally-available, dated July 2,
  2026, "Issue fields are now generally available for all GitHub organizations". The page says nothing about
  REST or GraphQL; the preview post (https://github.blog/changelog/2026-03-12-issue-fields-structured-issue-metadata-is-in-public-preview/)
  lists "REST and GraphQL APIs for field settings and values". **verified.**

**Finding a created issue again (the idempotence key).**
- Search: "you can make up to 30 requests per minute for all search endpoints"; "up to 1,000 results for each
  search" (https://docs.github.com/en/rest/search/search). **verified.** The page is silent on freshness.
- GitHub's status page: "On February 23, 2026, between 21:01 UTC and 21:30 UTC the Search service experienced degraded
  performance" and "updates to Issues and Pull Requests may not have been immediately reflected in search results."
  (https://www.githubstatus.com/incidents/47n199g2d88x). **verified**: one incident, not a guarantee either way.
- The docs' own page shows `field.priority:high` as a filter: "Filter for issues by field value: `is:open
  field.priority:high`" (https://docs.github.com/en/issues/tracking-your-work-with-issues/using-issues/filtering-and-searching-issues-and-pull-requests). **verified.**
  A third-party reference calls it "Search qualifier syntax (experimental)", says it "requires `advanced_search=true`
  on REST", and "results are inconsistent and may return 0 results even when matching issues exist"
  (https://git.cynarski.dev/Awesome/awesome-copilot/raw/branch/marketplace/skills/github-issues/references/issue-fields.md).
  **unverified** (a mirror of someone's skill file, not GitHub). The records.md rule cites "GitHub's documentation,
  read 2026-10-11" for `field.defect:235`; the page I read shows the qualifier but says nothing of the caveats.
- **inference**: a search is a hint, a GET by number is the truth. The `github:` line in the card and the `Record:`
  permalink inside the body, read with `GET /repos/.../issues/{n}`, are the stronger keys; the first creation after
  a kill is the only case that needs search, and there a retry after a pause is what the status incident argues for.

### 2. The Issue Import API (R4, the author's standing question)

**Does it exist, is it documented, who may call it.**
- It exists as an **unofficial, undocumented preview**. The only description is a gist,
  https://gist.github.com/jonmagic/5282384165e0f86ef105 (read today; it loads, so the critic's 404 was the endpoint's
  `documentation_url` pointing at the gist, not a missing gist). It says: `POST /repos/{owner}/{repo}/import/issues`,
  header `Accept: application/vnd.github.golden-comet-preview`, "admin permissions on the repository", the issue takes
  `title`, `body`, `created_at`, `closed_at`, `updated_at`, `assignee`, `milestone`, `closed`, `labels`; each comment
  takes `body` and `created_at`; max request 1 MB, body up to 65535 characters; status `pending`, `imported` or `failed`
  with `errors`; the API "hasn't been finalized yet". **verified.**
- General notes on the same page: imports "do not trigger any notifications to mentioned users or users watching the
  repository"; requests "are affected by regular API rate limits" and abuse limits; "issue numbers are handed out to
  successfully imported issues once the processing is completed", so numbers are not given at the POST (these three
  are the gist's statements, **verified as the gist's**; whether they hold in 2026 is **unverified**). It is not in
  GitHub's REST reference (the issues page I read lists no import endpoint: https://docs.github.com/en/rest/issues/issues).
- An invalid milestone, assignee, creator or label fails the import (the gist's section heading and example). So labels
  and milestones must exist first. **verified as the gist's.**

**Does it set the dates, and has anyone used it in 2025 or 2026.**
- The gist's schema carries `created_at`, `closed_at`, comment `created_at`. The package README says: "Unlike the
  official REST or GraphQL APIs, this API allows you to keep the timestamps of the imported issues and comments."
  (https://github.com/zyv/github-issues-import). **verified as claims.**
- **2025, working report (best evidence):** community discussion 150458, opened 2025-02-01, accepted answer
  2025-02-25 by the same user: the unofficial API "keeps the dates" and is "still somewhat functional"; "There is no way
  to keep reporter information as per platform policy"; issue types and resolutions "must be applied after import, e.g.
  via GraphQL"; assignees must have repository access or are not imported
  (https://github.com/orgs/community/discussions/150458). **verified** that the post says it; one user, no GitHub staff
  reply: the evidence is one person's.
- **2024, a working report:** a comment by "burner", 2024-12-16, on the gist: migrated all open dlang Bugzilla issues "with
  this API" (as summarised by the fetch tool from the gist's comment thread). **verified as a comment**, the
  tool's summary. The D project's own announcement (digitalmars.D.announce, 2024-12-01, Robert Schadek) says "the
  issues were moved by a D program I wrote" and does not name the endpoint, the dates or the rate limits
  (https://digitalmars.com/d/archives/digitalmars/D/announce/Bugzilla_Issue_migration_to_github_phobos_64481.html),
  so that the D project used this API is **unverified**; "burner" and the D project may not be the same actor.
- **2025-02-09:** a comment from zyv on the gist announcing the Python client. **verified as a comment.**
- **2026:** nothing found that shows it working or removed. A search snippet pointed to a test issue "Test Issue Import API"
  in `aa22396584/super_hello_world`, dated January 2026 in the snippet; the page returned 404 when fetched. **unverified.**
  I found no GitHub changelog or docs entry about it. Searches: "golden-comet-preview" 2025; "import/issues" with the
  preview header; unofficial import API 2026. A negative claim on the searcher's vocabulary only.
- **2023, the gist author's own warning:** jonmagic on 2023-02-03: the API "was never prioritized to be
  maintained and updated", built for the Google Code migration, team disbanded, removal possible. Same day mpdude
  notes it "still exposed (and functional as of today)". **verified as comments.** The same thread also reports, in
  2018-2019, labels making the event timestamp "now" (toddr, 2019-10-09: "If you add labels to the import, the time
  stamp on the event is now.") and an import to an organisation returning "Not Found" that worked days later
  (burner, 2023-10-15). **verified as comments**; for Heroes it means the issue's dates may be set while the
  *labeled* and *milestoned* events still read the day of the run, so the body's first line stays.

**What the live read answers.** `gh api -X GET repos/heroes-lang/heroes/import/issues/1` returning a 404 whose
`documentation_url` is the gist (the critic's read) shows the route family is known to the server. **inference**: a
generic unknown route would not name the gist. It does not say whether a write is accepted for this repository, in an
organisation, in 2026.

**If only a write settles it, exactly what the write is (the author decides whether to allow it).**
1. A throwaway repository in **the author's user account, not the organisation**, private (`POST /user/repos`, "repo
   scope to create a private repository", https://docs.github.com/en/rest/repos/repos). It must not be
   `heroes-lang/heroes`: the gist says numbers are given at completion and cannot be chosen (mpdude's question, answered
   "not possible" by the gist's author, 2023-02-03), and a test issue there would consume number 1 of a repository whose
   order is a promise. A second test in an organisation repository answers the organisation-specific "Not Found" reports,
   at the cost of an organisation-level repository.
2. One `POST .../import/issues` with the preview header: one issue with `created_at` and `closed_at` in 2026-09, `closed:
   true`, one label and one milestone that exist, two comments with `created_at` of different days; poll the status URL
   until `imported`.
3. Read back: `GET` the issue (`created_at`, `closed_at`, `state`), its comments (`created_at`), its timeline (the `closed`,
   `labeled`, `milestoned` events' dates). Four reads.
4. What it leaves: one private repository holding one issue; the author as the author of the issue and comments (every
   report says the token owner is). It notifies nobody else (private, no watchers; the gist says no notifications).
   **Deleting the repository needs `delete_repo`** ("Deleting a repository requires admin access"; "need the delete_repo
   scope", same repos page), a scope F5 says the token does not have, so deletion is a further author act (a token
   refresh); the alternative that needs nothing new is to leave it, archived or not.
5. Whether an imported body that links another project's issue writes a `cross-referenced` event there is the one side
   effect on a third party the test could cause; I would not put such a link in the test body. It can be asked later
   by reading the target's timeline of a link the real run already made. **unverified** whether a private repository's
   reference shows on the target at all: no page read says so.

### 3. Cross-references: `#N`, `@name`, `owner/repo#N`, a URL

- GitHub: "Within conversations on GitHub, references to issues and pull requests are automatically converted to
  shortened links."; `#26`, `GH-26` and `owner/repo#26` all convert; "GitHub automatically creates links from standard
  URLs", an issue URL shortening to `#26`; "Autolinked references are not created in wikis or files in a repository."
  And: "By default, references generate a backlink." (https://docs.github.com/en/get-started/writing-on-github/working-with-advanced-formatting/autolinked-references-and-urls). **verified.**
- **The documented way to avoid the backlink:** the page says "To avoid this behavior, you can use `redirect.github.com`
  instead of `github.com` when constructing the URL" in the reference; no hover pop-up appears; "not supported" on GitHub
  Enterprise Cloud with data residency (`ghe.com`). **verified** (fetch summary; re-read before citing).
  **inference**: that avoids the event without altering the words a reader reads only if the visible text is kept, as a
  Markdown link `[owner/repo#N](https://redirect.github.com/owner/repo/issues/N)`; a bare URL written with the other host
  changes the visible URL, and a bare `owner/repo#N` cannot be given the other host at all. Whether such a link still
  creates a `mentioned` or `cross-referenced` event: not on the page.
- The event: "The issue or pull request was referenced from another issue or pull request." (`cross-referenced`; "Timeline
  events only") and `mentioned`: "The `actor` was `@mentioned` in an issue or pull request body."
  (https://docs.github.com/en/rest/using-the-rest-api/issue-event-types). **verified.** Who sees a `cross-referenced`
  event on the target: not stated on the pages read. **unverified**. **inference**: the target's maintainers see it in
  that issue's timeline, as the brief says (F4).
- **Who is notified by `@name`:** "A person will only be notified about a mention if the person has read access to the
  repository" and, if the repository is owned by an organisation, is "a member of the organization"
  (https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax,
  the sentence re-fetched). **verified.** With one organisation member (F5) the 13 `@word` strings notify nobody
  but that member. **inference.** A mention in a body is also an event on *this* issue, not on the stranger's page.
- Escaping: "The text within the backticks will not be formatted."; a backslash escapes Markdown. And "The
  Markdown formatting will not be ignored in the title of an issue or a pull request." (basic-writing page).
  **verified.** Whether `#119` in a code span stays text on GitHub is **unverified** (the autolink page does not say);
  the brief's F4 already excludes code spans.
- A hash in a code span: the autolink page says "References to a commit's SHA hash are automatically converted into
  shortened links"; it is silent on code spans. **unverified**; the brief's D6 asserts it.

### 4. Precedent migrations, and what they kept

Old identifier -> where it went; dates; mentions; notifications; rate-limit trouble.

- **CPython, bugs.python.org to GitHub, April 2022.** *Tool:* GitHub's Enterprise importer fed with an export zip, with
  GitHub's engineers on call, not a public API: "importing the zip archive with the issues into a new repo on GitHub
  through the ECI (Enterprise Cloud Importer)" (https://github.com/psf/gh-migration/issues/13). **verified.**
  *Duration:* "Importing 500 issues (without attachments) ... took 13m"; "almost 60k issues, so it should take around
  25h"; testing "about 12h"; the real transfer "about 19 hours" (2022-04-09 20:45 to 04-10 15:40 UTC)
  (https://discuss.python.org/t/github-issues-migration-status-update/14573). **verified.** *Old identifier:* "doesn't
  preserve the original issue number, and simply uses the next available ID"; preserving would "have been impossible
  anyway" because issues and PRs share a number space; a banner on each bpo issue links to its GitHub issue and the lists
  gained a column of clickable GitHub IDs (same page). **verified.** On the GitHub side the number is a field in a
  body table: python/cpython#59000 has "BPO" 14795 as a link to bugs.python.org/issue14795, `created_at` 2012-05-13, author
  shown as a mannequin (https://github.com/python/cpython/issues/59000). **verified** (fetch summary). *Mentions:* "All
  nosy list members are @mentioned at the top of each issue", "there doesn't seem to be a way to automatically subscribe
  other people", mass mentions would "generate a flood of notifications"
  (https://discuss.python.org/t/github-issues-migration-is-coming-soon/13791, 2022-02-19). **verified.** *What broke:*
  "A problem during ID mapping caused several emails to be sent by mistake, affecting users subscribed to old issues from
  about 20 years ago"; formatting problems, duplicated events, users mapped to mannequins, links from bpo postponed
  (https://discuss.python.org/t/github-issues-are-now-live/14967, 2022-04-10). **verified.** GitHub's schedule slipped:
  "The Engineering team at GitHub suggested to postpone the migration due to the ongoing issues" (status-update page).
  **verified.** *Rate limits:* none mentioned.
- **Swift, JIRA to GitHub Issues, April-May 2022.** Each JIRA issue "cloned" into `apple/swift` then moved; JIRA text
  "converted into markdown"; a bot account is the creator where the JIRA creator is not in the user mapping
  (https://forums.swift.org/t/swift-bugs-are-moving-to-github-issues-and-we-need-your-help/56125, 2022-03-21;
  https://forums.swift.org/t/swift-bugs-have-migrated-to-github-issues/56960, phase one 2022-04-25, last phase
  2022-05-25). **verified.** *Old identifier:* the title opens with it, `[SR-16123] Non-escaping closure capturing ...`,
  and the body opens with a table whose "Previous ID" row reads SR-16123
  (https://github.com/swiftlang/swift/issues/58382, "Created: Apr 18, 2022", "transferred from apple/swift-issues on
  Apr 25, 2022"). **verified** (fetch summary). Whether the shown date is the JIRA date: not stated on the page.
  *Trouble:* some issues imported already closed; a user could comment but not close a cloned issue; bugs.swift.org links
  showed a login screen until the redirect (2022-05-19: "Redirect for bugs.swift.org will be available in next few
  weeks."). **verified** (forum post). No tool, rate limit or notification report on those pages.
- **Go, Google Code to GitHub, announced 2014-11-14.** "all data will be preserved"; contributors authorise a migrator app
  so issues are recreated under their own accounts, else a "gopherbot" account owns them but "still attributed"
  (https://groups.google.com/g/golang-dev/c/sckirqOWepg/m/YmyT7dWJiocJ, Rob Pike). **verified** (the post says nothing of
  issue numbers). On GitHub today: golang/go#1 is "cmd/cgo: fails with gcc 4.4.1", opened by gopherbot, "Oct 22, 2009"
  (https://github.com/golang/go/issues/1); #9000 is dated "October 25, 2014" and begins "by donald.stufft:"
  (https://github.com/golang/go/issues/9000). **verified** (fetch summary): the numbers run from 1 and the dates are the
  old ones. How it was done (which endpoint) is **unverified**: neither page says; the gist's author says the API
  "was built for the Google Code migration" without naming Go (**verified as his words**, 2023-02-03).
- **The D language, Bugzilla to GitHub, from 2024-12-01.** Phobos, then druntime (2024-12-07), DMD (2024-12-15). Numbers
  changed (Bugzilla #24288 became GitHub #10539 in a reader's example); each Bugzilla issue got a comment "THIS ISSUE HAS BEEN
  MOVED TO GITHUB" linking the new one; the migration "sent a large volume of email": "several hundred" (Manu, estimating
  about 1000), 450 for a watcher of the repository; advice for next time "disable your email notifications in bugzilla"
  (announcement page above). **verified** (fetch summary). In 2023 the author said "github is rate limited, 100 writes per
  ip per hour" (https://lists.puremagic.com/pipermail/digitalmars-d/2023-June/335541.html, via a search snippet,
  **unverified**: his figure, not GitHub's). The planning thread's "hidden API" remark was not found on any page read.
- **LLVM, Bugzilla.** The 2019-2020 proposals weighed an archive repository and a redirect script against importing by
  request, and a poster worried about "lots of references to PRxxxxx" in checked-in files
  (https://lists.llvm.org/pipermail/llvm-dev/2019-October/136167.html, search snippets). I did not find the outcome:
  **unverified**; not used.
- **Jenkins.** Per-plugin moves (search snippet, https://community.jenkins.io/t/publish-over-plugins-issues-migrating-to-github/1859).
  Whether JENKINS-NNNN was kept: not found. **unverified**; not used.
- **GitHub Enterprise Importer.** "supports migrations **to** GitHub Enterprise Cloud"; sources: Azure DevOps Cloud,
  Bitbucket Server and Data Center 5.14+, GitHub.com, GHES 3.4.1+, GHE.com, GitLab
  (https://docs.github.com/en/migrations/using-github-enterprise-importer/understanding-github-enterprise-importer/about-github-enterprise-importer). **verified.**
  **For R12: it is not a route** for a folder of Markdown files; CPython's use was a GitHub-assisted import of its own
  export, not a public path (the page lists no arbitrary-tracker source).
- **`gh-iff`.** I could not find it: `"gh-iff" github extension` returned nothing about it (the critic's `gh extension
  search` read cannot be repeated by this seat). **unverified.** The CSV importer `gavinr/github-csv-tools` "Currently
  imports title, body, labels, status (closed or open) and milestones" and its import section names no comments or dates
  (https://github.com/gavinr/github-csv-tools). **verified.** `aboutcode-org/github-import-issues-csv` (a Python script, one
  issue per row, sub-issues, project fields; search snippet only): **unverified.**
- **`git-bug`.** "can import from and export to Github, Gitlab, Jira and Launchpad"; "no files are added in your project"
  (https://github.com/git-bug/git-bug). **verified.** It is the one tool I found going tracker-in-git -> GitHub. **inference**:
  not a route for R12, because it keeps bugs in git objects, not in files of the working tree, the reverse of `issues/`.
  Whether it carries dates: not on the page.
- **Projects v2 draft items:** not researched; the critic's reasons (no number, no comments) stand. I make no claim.

### 5. Issue fields and native issue types, and an old tracker's number in a field

- Native types and fields exist and are limited (item 1). **I found no project that carries an old tracker's number in
  an issue field or a type.** Searches: GitHub issue fields custom text field Jira key/Bugzilla id/legacy tracker id
  migration 2026; "issue fields" + migration. Every precedent above put the old number in the title, in a body table, in a
  banner or in a comment, never in a field (Swift and CPython above; D by comment). A negative claim on my vocabulary.
- **inference**: the sitting's field `Defect` is, as far as I can find, first use. The risk is the search guard in item 1,
  not the field.
- Nobody asked why labels and not types (critic). Types are a classification (25 allowed; default task, bug, feature) set by
  `type` on create; `kind:defect`, `kind:feature`, `kind:task` map onto them. No precedent found for either choice.
  **inference**; for the sitting to weigh.

---

## Prior art lines the synthesis may use

Each only if the sitting adopts the thing, and in the page's own words. Form: `**Taken from** X` = adopted on their
evidence; `**Reached first by** X` = Heroes reached it on its own and the page shows X had. I cannot know which; the
coordinator knows what the sitting adopted. **Re-read each page first.** A precedent the sitting only compares itself
with is not prior art.

1. **Taken from GitHub**: [Best practices for using the REST API](https://docs.github.com/en/rest/using-the-rest-api/best-practices-for-using-the-rest-api):
   "To avoid exceeding secondary rate limits, you should make requests serially instead of concurrently."; and for POST,
   PATCH, PUT, DELETE, "wait at least one second between each request." *If the push is serial and paced.*
2. **Taken from GitHub**: [Rate limits for the REST API](https://docs.github.com/en/rest/using-the-rest-api/rate-limits-for-the-rest-api):
   with a `retry-after` header, "you should not retry your request until after that many seconds has elapsed", otherwise
   "wait for at least one minute before retrying." *If the push honours them.*
3. **Taken from GitHub**: [Autolinked references and URLs](https://docs.github.com/en/get-started/writing-on-github/working-with-advanced-formatting/autolinked-references-and-urls):
   "By default, references generate a backlink."; "To avoid this behavior, you can use `redirect.github.com` instead of
   `github.com`". *Only if the sitting rewrites a cross-project link to the other host.*
4. **Reached first by Swift**: [swiftlang/swift#58382](https://github.com/swiftlang/swift/issues/58382): the title of a
   migrated issue begins with its old tracker's key, `[SR-16123]`, and its body begins with a table whose "Previous ID" row
   reads SR-16123. *Only if the sitting finds the `Defect NNN:` title was reached without Swift (CL-083's reasoning);
   if the body-table row or the title form was taken from Swift, `Taken from`.*
5. **Taken from CPython**: [python/cpython#59000](https://github.com/python/cpython/issues/59000): the body carries the
   old tracker's number in a "BPO" field linking to bugs.python.org/issue14795. *Only if the sitting adopts the old
   identifier as a linked field of the body.*
6. **Taken from the unofficial issue-import API**: [the gist](https://gist.github.com/jonmagic/5282384165e0f86ef105): an
   issue takes `created_at` and `closed_at`, a comment takes `created_at`, and imports "do not trigger any notifications
   to mentioned users or users watching the repository". *Only if the sitting uses it, and after the live write (item 2).*
7. Not prior art, compared only: Go, the D language, LLVM, Jenkins, GitHub Enterprise Importer, `gh-iff`, `git-bug`, the CSV
   importers. (D's volume of email and CPython's mistaken emails are what the sitting learns from; the section does not
   carry "what happened to them".)

---

## argument

GitHub's pages put bulk creation at 500 content-generating requests an hour and ask for serial requests one second apart,
so 5,918 requests are about twelve hours, not minutes; the verb must sleep on `Retry-After`, not stop. The two migrations
that kept an old number (Swift, CPython) put it in a title prefix or a linked body field, none in an issue field; both
mapped some authors to a bot or mannequin, and D and CPython sent email in volume. A back-dating route exists, unofficial,
reported working in February 2025; "a transcription cannot back-date" is a premise until one throwaway write, which
deletion needing a new scope makes the author's call.

## condition

I would withdraw the objection on premise 2 if a GitHub page or a 2026 report shows the import endpoint removed (then the
rule stands), or a scratch-repository write shows `created_at`, `closed_at` and comment dates are set (then the rule is
amended, the three-request cost falls to one, and the run falls toward four hours). On premise 1, a GitHub page defining
"content-generating" so that closing and editing do not count would shorten 11.8 hours toward 7.9. On premise 3, a sentence
from GitHub, not a third party, saying `field.<name>:value` search is reliable and fresh. A project that carries an old
tracker's number in an issue field would be the missing precedent for `Defect`.

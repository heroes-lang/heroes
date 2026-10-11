# 041: the import endpoint keeps an issue's own days, and an imported reference leaves no backlink

Measured 2026-10-11 between 19:11 and 19:14 UTC (21:11 to 21:14 on this Mac's
clock) by the coordinator of panel 212, as that sitting's R6 item 2
(`docs/panel/212-the-issue-files-reach-github-through-a-program-beside-the-net-and-no-verb-and-the-rendering-rule-is-closed-first.md`),
the experiment the author allowed in conversation that evening
(`issues/2026-10/11/2026-10-11-1846-panel-212-ratify-amend-or-overturn-r1-to-r11-a-program-beside-the-net.md`).
It answers the author's standing question of 2026-10-06, the last paragraph of
`issues/2026-10/06/2026-10-06-1249-the-author-answers-1-2-3-m-issue-files-to-this-session-github.md`:
*whether GitHub's issue import endpoint still sets `created_at` and `closed_at`
is unrun: it is asked of the endpoint before the first issue is made, and the
rule in `.claude/rules/records.md`, a transcription cannot back-date GitHub's
own dates, is amended if it is answered yes.* **It is answered yes**, and the
rule is amended in the commit that writes this file.

## 1. Provenance

| what | value |
|---|---|
| the client | `gh version 2.102.0 (2026-09-30)`, logged in as the author's account with the token made after that evening's revocation; the import's response header `X-Oauth-Scopes` read `admin:public_key, gist, read:org, repo` |
| the endpoint | `POST /repos/{owner}/{repo}/import/issues` with `Accept: application/vnd.github.golden-comet-preview+json`, as the gist describes it (https://gist.github.com/jonmagic/5282384165e0f86ef105, read 2026-10-11 at 19:11 and again at about 19:40); the response named `X-Github-Api-Version-Selected: 2022-11-28` and `X-Github-Media-Type: github.v3; param=golden-comet-preview; format=json` |
| the repositories | `heroes-lang/import-probe`, private, in the organisation, because the organisation is where the transcription will import; `giuseppearici/import-probe-target`, private, in the author's account, the target of the references, a second owner as the references of our bodies to other projects have one; both made for this alone |
| what was made first | in the target, one issue (`#1`, 19:12:00); in the probe, the label `probe` and the milestone `M-probe` (number 1), which the import names |
| the payloads | `import-a.json` (686 bytes) and `import-a2.json` (601 bytes), kept in `.claude/worktrees/scratch-panel-212/probe/`, which git ignores: each an issue with `created_at`, `updated_at`, `closed_at`, `closed: true`, the label and the milestone, and one comment with its own `created_at` |
| what it cost | 15 requests of the core limit of 5,000, `rate_limit` read before the archiving |
| afterwards | both repositories archived at 19:13, `archived: true` read back for each; at 19:36 both answered 404 to every read and neither stood in the author's or the organisation's listing, the same account and scopes logged in: the author had deleted them, as they said in conversation at about 21:45 local time (this token cannot, lacking `delete_repo`) |

## 2. What was run, and what each answered

| # | the request | the answer |
|---|---|---|
| 1 | import A: created `2026-08-03T09:00:00Z`, closed `2026-08-04T10:30:00Z`, a comment at `2026-08-04T10:30:00Z`, a body referring to the target as `https://redirect.github.com/giuseppearici/import-probe-target/issues/1` | `HTTP/2.0 202 Accepted`, `"status":"pending"`; read at 19:12:24, `"status":"imported"`, `issue_url` `.../import-probe/issues/1` |
| 2 | issue 1 read back | `number 1`, `state closed`, `state_reason completed`, `created_at 2026-08-03T09:00:00Z`, `updated_at 2026-08-04T10:30:00Z`, `closed_at 2026-08-04T10:30:00Z`, `labels ["probe"]`, `milestone M-probe`, `user giuseppearici`, `comments 1` |
| 3 | its comment | `created_at 2026-08-04T10:30:00Z`, `updated_at 2026-10-11T19:12:20Z`, its body as sent |
| 4 | its timeline | `commented 2026-08-04T10:30:00Z`, `closed 2026-08-04T10:30:00Z`, `labeled 2026-10-11T19:12:21Z`; no other event |
| 5 | the target's timeline | empty |
| 6 | import A2: created `2026-08-10`, closed `2026-08-11`, the plain form `https://github.com/giuseppearici/import-probe-target/issues/1`, started after A was read `imported` | `pending`, then `imported`, `issue_url` `.../issues/2` |
| 7 | the target's timeline | still empty |
| 8 | four issues by the REST create, `POST /repos/heroes-lang/import-probe/issues`: B the plain URL, C the `redirect.github.com` URL, D the short form `giuseppearici/import-probe-target#1`, E the short form and a bare `#1`, each in a code span | numbers 3, 4, 5 and 6, each `open` |
| 9 | the target's timeline | two `cross-referenced` events, from 3 (B) at 19:12:55 and from 5 (D) at 19:12:56; none from 4 (C) or 6 (E) |
| 10 | issue 1's timeline, its `cross-referenced` events | none: E's `#1` in a code span made none in its own repository |
| 11 | `POST /markdown`, mode `gfm`, context `heroes-lang/import-probe`, one text holding eight forms: a bare `#1`, a bare `@giuseppearici`, the short form, the plain URL, the redirect URL, and `#1`, `@giuseppearici` and the short form each in a code span | five anchors: three `issue-link` (the bare `#1`, the short form, the plain URL), one `user-mention` (the bare `@`), and a plain `<a href>` for the redirect URL; the three forms in code spans gave none |

## 3. What it settles

- **The endpoint answers on 2026-10-11, in an organisation's private
  repository, and sets from the request an issue's `created_at`, `updated_at`
  and `closed_at` and a comment's `created_at`**; `closed: true` closes the
  issue `completed`. One request carries an issue whole: its days, its labels,
  its milestone, its closed state and its closing comment, where the REST route
  takes three and dates each the day it runs (panel 212's R5 counted 5,918).
- **Awaited one at a time, the numbers followed the requests' order**, 1 then 2,
  as the gist advises (*"issue numbers are handed out to successfully imported
  issues once the processing is completed"*).
- **An imported body made no backlink, in either form of the reference**; a body
  made by the REST create made one with the plain URL and with `owner/repo#N`,
  and none with `redirect.github.com` or inside a code span. So the rule of
  panel 212's R3 for bodies holds on both routes.
- **The oracle tells apart the forms the timeline does**: what `POST /markdown`
  rendered as an `issue-link` is what made an event, on the REST route.

## 4. What it does not settle, each unrun

- **A public repository.** `heroes-lang/heroes` is public and both probes were
  private; whether an import there behaves the same is unrun.
- **The scale.** Two imports, where the transcription is 2,017 (the issue files
  counted 2026-10-11 at 21:40). 2,017 requests at 500 an hour is about 4.0
  hours, an inference resting on an import counting as a content-generating
  request, which no page read says and nothing here measured; the polls of each
  import's status are reads, and how they count is unrun too.
- **The endpoint's life.** The gist says *"This API hasn't been finalized yet"*,
  and its author wrote beneath it on 2023-02-03 that the API *"was never
  prioritized to be maintained and updated"*, its removal possible
  (`docs/panel/212-reports/historian.md` § 2, verified as comments). One
  evening's yes says nothing of the next run, so the run asks it again of its
  first request.
- **The days a reader sees.** The `labeled` event and the comment's
  `updated_at` read the day of the import, not the card's, as a comment of
  2019-10-09 on the gist had reported (*"If you add labels to the import, the
  time stamp on the event is now."*, the same report); the milestone made no
  event at all. Whether GitHub's page shows the label as added today is an
  inference from the timeline, unrun on the page. So the body's first line,
  `Filed · closed`, stays on every route.
- **Notifications.** No watcher and no mention stood in the probe; the gist's
  sentence, *"Issues and comments created with this API do not trigger any
  notifications to mentioned users or users watching the repository"*, is the
  only evidence.
- **The issue field `Defect`.** The gist's schema carries no field, so the import
  cannot set it, as panel 212's R6 item 5 already said.
- **A missing label or milestone, and a real commit's hash.** Both payloads named
  ones that existed, and the comment named a hash of forty zeros, so whether a
  hash in an imported comment links to its commit is unrun.

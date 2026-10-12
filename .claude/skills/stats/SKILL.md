---
name: stats
description: The issues counted — defects open and closed by class, area and age, the days from a defect's filing to its closing, the week's pace, and every kind's open and closed counts — measured from GitHub's issues at the moment it runs. Read-only. Use whenever the author asks for statistics, counts or trends about defects, decisions or the work (they ask in Italian: "statistiche", "quanti difetti", "aperti e chiusi", "per tipo", "come stiamo andando coi difetti").
---

# /stats — the issues, counted now

Asked for by the author on 2026-10-05, meant as: *prepare a skill that gives me
the statistics on the defects, open, closed, by type and so on*. It reads
GitHub's issues (`.claude/rules/records.md` § The issues) and writes nothing:
no file in the tree, no commit, no issue.

**Every number comes from the commands below, run in this session** (CLAUDE.md
§ RUN IT). Never a number recalled from an earlier answer, a ROADMAP line or a
commit body: the tree moves between two questions, and an old count reported as
today's is the one mistake this skill exists to prevent.

## 1. Measure

From the repository's root, into the session's scratchpad (`$S`). Every
request is a GET, which `.claude/hooks/guard_bash.py` lets through:

```sh
S=<the scratchpad>; mkdir -p "$S"
# every issue, open and closed, a pull request left out, one row each:
# number, kind, area, milestone, created, closed, state, class, title
gh api --paginate 'repos/heroes-lang/heroes/issues?state=all&per_page=100' --jq '
  def tag(p): ([.labels[].name | select(startswith(p + ":"))][0] // (p + ":none")) | sub(p + ":"; "");
  .[] | select(.pull_request | not)
  | [.number, tag("kind"), tag("area"), (.milestone.title // "none"),
     .created_at[0:10], ((.closed_at // "")[0:10]), .state, tag("class"), .title]
  | @tsv' > "$S/issues.tsv"
# the program's own reader of the open issues, the one the lists use
heroes run tests/harness/issues.hero -- open --json > "$S/open.json"

# the days: an open defect's age, a closed one's days from filing to closing
today=$(date +%Y-%m-%d)
awk -F'\t' -v today="$today" '
function jd(d,   y, m, dd) { y = substr(d,1,4)+0; m = substr(d,6,2)+0; dd = substr(d,9,2)+0; if (m < 3) { y--; m += 12 } return int(365.25*(y+4716)) + int(30.6001*(m+1)) + dd }
$2 == "defect" && $7 == "open" { print "OPEN\t" jd(today) - jd($5) "\t" $8 "\t" $3 "\t" $5 "\t" $1 }
$2 == "defect" && $7 == "closed" { print "SETTLED\t" jd($6) - jd($5) "\t" $8 "\t" $3 "\t" $6 "\t" $1 }
' "$S/issues.tsv" > "$S/times.tsv"
```

**What it counts**: GitHub's issues of `heroes-lang/heroes`, the kind, area and
class read from their labels and the milestone from GitHub's. An issue opened
from outside that nobody has labelled yet counts as kind `none`, and is named
as such, never folded into a kind. The days are GitHub's: an issue transcribed
from a file holds its card's days (`docs/measurements/041`), so a closed defect's
days run from its filing to its closing, which for one closed since the batches
is the batch's close and not its repair commit's day. **The open rows must
equal the issues `open --json` lists**; a difference is reported as found, with
both commands.

Then the counts, each a one-line `awk`, `sort` or `uniq -c` over those two files:

- **every kind, open and closed**: `awk -F'\t' '{print $2, $7}' "$S/issues.tsv" | sort | uniq -c`;
- **the defects by class**, open and closed: `awk -F'\t' '$2=="defect"{print $7, $8}' …`;
  `none` is a defect closed before classes existed (2026-10-02), not a missing value;
- **the open defects by area**, and the closed ones by area;
- **what blocks the tag**: the open `blocking` and `systemic` defects by title, and
  every open `adjacent` one with its age, since an `adjacent` defect counts as
  `blocking` from the second tag placed after it was classed (CLAUDE.md
  § Verification);
- **ages**: from `times.tsv`'s `OPEN` rows, the minimum, the median, the maximum
  and the five oldest;
- **days to close**: from its `SETTLED` rows, the median, the 90th percentile and
  the maximum, by class where the class is known;
- **the pace**: per ISO week of the last six, the defects filed (by `created`) and
  the defects closed (by `closed`), from `issues.tsv`;
- **the other kinds**: open decisions by title (each waits on the author), and the
  open `feature` and `task` issues by milestone.

Where a question is narrower, filter `issues.tsv` by the column it names and
nothing else: a milestone (column 4), an area (column 3), a day (column 5).

**The questions of comprehension are not issues** (the author's *1a 2a* of
2026-10-05, said on this skill's first table), so they are in none of these
counts. Asked about them, count them apart and say so:
`find docs/learn -name '20*.md' | wc -l` and
`grep -rl '^- \[ \] ' docs/learn | wc -l` for the open ones.

## 2. Tell

In Italian, plain, alive (CLAUDE.md § 11), and **the date and time of the
measurement first**, with the commit it was taken on (`git log --oneline -1`).
Then, in this order, each as a short table:

1. **Defect aperti e chiusi**, per classe;
2. **Cosa blocca il tag**: i `blocking` e `systemic` aperti, per nome, e gli
   `adjacent` più vecchi;
3. **Età e tempi**: età dei defect aperti, giorni dalla nascita alla
   sistemazione;
4. **Il ritmo**: filati e sistemati per settimana;
5. **Per area**, i defect aperti;
6. **Tutti i tipi**, aperti e chiusi, e le decisioni che aspettano l'autore.

Numbers go in tables and not in prose; one sentence under each table says what
it means for the work, attached to the number that says it. Close with nothing
more: no offer, no plan. If the author asked one thing, answer that thing and
leave the rest out.

## What this skill does not do

It does not open, close, comment on or relabel an issue, and it does not run
a suite. A count that looks wrong is reported as found, with the command that
produced it, and the question of why goes to the author or to `/decide`.

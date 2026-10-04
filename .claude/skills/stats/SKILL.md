---
name: stats
description: The issues counted — defects open and closed by class, area and age, the days from a defect's filing to the commit that settled it, the week's pace, and every kind's open and closed counts — measured from the cards of issues/ at the moment it runs. Read-only. Use whenever the author asks for statistics, counts or trends about defects, decisions or the work (they ask in Italian: "statistiche", "quanti difetti", "aperti e chiusi", "per tipo", "come stiamo andando coi difetti").
---

# /stats — the issues, counted now

Asked for by the author on 2026-10-05, meant as: *prepare a skill that gives me
the statistics on the defects, open, closed, by type and so on*. It reads the
cards of `issues/` (`.claude/rules/records.md` § The issues) and writes nothing:
no file, no commit, no issue.

**Every number comes from the commands below, run in this session** (CLAUDE.md
§ RUN IT). Never a number recalled from an earlier answer, a ROADMAP line or a
commit body: the tree moves between two questions, and an old count reported as
today's is the one mistake this skill exists to prevent.

## 1. Measure

From the repository's root, into the session's scratchpad (`$S`):

```sh
S=<the scratchpad>; mkdir -p "$S"
# one row per issue: path, kind, area, milestone, filed, commit, open|closed, class
find issues -name '20*.md' -print0 | sort -z | xargs -0 awk '
function out() { if (f != "") print f "\t" kind "\t" area "\t" ms "\t" filed "\t" commit "\t" (open ? "open" : "closed") "\t" (cls == "" ? "none" : cls) }
FNR == 1 { out(); f = FILENAME; kind = area = ms = filed = commit = cls = ""; open = 0; seen = 0 }
FNR == 2 { kind = substr($0, 7) }
FNR == 3 { area = substr($0, 7) }
FNR == 4 { ms = substr($0, 12) }
FNR == 5 { filed = substr($0, 8) }
FNR == 6 { commit = substr($0, 9) }
FNR >= 10 && /^- \[ \] / { open = 1 }
FNR >= 10 && /^- \[[ x]\] / && !seen { seen = 1; if (match($0, /\*\*class: [a-z]+\*\*/)) cls = substr($0, RSTART + 9, RLENGTH - 11) }
END { out() }' > "$S/issues.tsv"

# the days: an open defect's age, a closed one's days from filing to the commit that settled it
today=$(date +%Y-%m-%d)
awk -F'\t' '$2 == "defect" && $7 == "closed" && $6 ~ /^[0-9a-f]{40}$/ {print $6}' "$S/issues.tsv" | sort -u > "$S/hashes"
git show -s --format='%H %ad' --date=short $(cat "$S/hashes") > "$S/days"
awk -F'\t' -v today="$today" '
function jd(d,   y, m, dd) { y = substr(d,1,4)+0; m = substr(d,6,2)+0; dd = substr(d,9,2)+0; if (m < 3) { y--; m += 12 } return int(365.25*(y+4716)) + int(30.6001*(m+1)) + dd }
NR == FNR { split($0, a, " "); settled[a[1]] = a[2]; next }
$2 == "defect" && $7 == "open" { print "OPEN\t" jd(today) - jd($5) "\t" $8 "\t" $3 "\t" $5 "\t" $1 }
$2 == "defect" && $7 == "closed" && ($6 in settled) { print "SETTLED\t" jd(settled[$6]) - jd($5) "\t" $8 "\t" $3 "\t" settled[$6] "\t" $1 }
' "$S/days" "$S/issues.tsv" > "$S/times.tsv"
```

Then the counts, each a one-line `awk`, `sort` or `uniq -c` over those two files:

- **every kind, open and closed**: `awk -F'\t' '{print $2, $7}' "$S/issues.tsv" | sort | uniq -c`;
- **the defects by class**, open and closed: `awk -F'\t' '$2=="defect"{print $7, $8}' …`;
  `none` is a defect closed before classes existed (2026-10-02), not a missing value;
- **the open defects by area**, and the closed ones by area;
- **what blocks the tag**: the open `blocking` and `systemic` defects by name, and
  every open `adjacent` one with its age, since an `adjacent` item counts as
  `blocking` from the second tag placed after it was filed (CLAUDE.md
  § Verification);
- **ages**: from `times.tsv`'s `OPEN` rows, the minimum, the median, the maximum
  and the five oldest;
- **days to settle**: from its `SETTLED` rows, the median, the 90th percentile and
  the maximum, by class where the class is known. The commit is the card's: a
  repair since the batches, and for the defects closed before 2026-09-12 the
  commit that recorded the closing, which `docs/measurements/039` says entry by
  entry;
- **the pace**: per ISO week of the last six, the defects filed (by `filed`) and
  the defects settled (by the settling commit's day), from `times.tsv` and
  `issues.tsv`;
- **the other kinds**: open decisions by name (each waits on the author), and the
  open `feature` and `task` issues by milestone.

Where a question is narrower, filter `issues.tsv` by the column it names and
nothing else: a milestone (column 4), an area (column 3), a day (column 5).

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

It does not open, tick, file or reclassify an issue, and it does not run a
suite. A count that looks wrong is reported as found, with the command that
produced it, and the question of why goes to the author or to `/decide`.

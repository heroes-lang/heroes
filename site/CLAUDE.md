# site/CLAUDE.md, the rules for the site and every outward-facing text

These rules govern all copy under `site/`, both editions, and any outward-facing
communication written for this project. Three of them bind every line, whoever
writes it and whatever they are doing, and they are the whole of this file: no
dates on the page, no assumed expert, no em dashes.

`site/README.md` keeps the site's structural rules and its register.
`site/.claude/skills/no-ai-slop/SKILL.md` is the editing pass. CLAUDE.md § 11 is
the language of the repository as a whole, and it is the reason this file exists
as a nested contract at all: it loads when something under `site/` is touched
and costs nothing in the sessions that never go there.

## No dates on the page

Author instruction 2026-08-28: *"every time the site says on the date of the
twenty-sixth of August two thousand twenty-six this was done, this was measured,
I do not like it one bit ... it reads like a log to me, and on the site I am not
interested in having that level of detail. You can say as of today, or give it
outright as a finished fact. Search the whole site for occurrences of a date, I
do not want the dates. I absolutely do not want them."*

- **A measurement never carries a date.** Not *"measured 2026-08-25"*, not
  *"re-run on 2026-08-28"*, not *"counted on"*. Write the number as a finished
  fact. Where a sentence needs a horizon, *today* or *so far* is the whole of it.
- **The build log carries none either.** `log.html` held 27 dated entries and now
  holds 27 named ones: the order of the list is the chronology and the milestone
  name is the label. Whoever wants the calendar has the repository.
- **What this does not relax**: CLAUDE.md §1 still binds, so every number on the
  page is re-measured in the session that writes it. The date leaves the *page*,
  not the practice; the commit body is where it goes instead. § Numbers on the
  site are re-run in `README.md` is the same rule with its dating clause dropped.
- **Three kinds of date are not measurements and stay.** The book's release
  wording, which the author's own brief fixes (`README.md` § The author and the
  book, rule 2); the copyright year in the footer; and years that belong to
  somebody else's history, which is Bowie's 1973 and 1977 and the language dates
  on `thanks.html`.

## The reader is never assumed to be an expert

Author instruction 2026-08-26: *"make it a rule that whoever lands on the site
is not an expert in programming languages. That is by no means a given."*
Somebody landing here may have never written a compiler, may not know what a
type checker or a garbage collector is, and may be reading about this out of
curiosity. Write for them.

- **A technical term that stays in English gets a plain-words gloss the first
  time it appears on a page**, in a few words, inline: *token* (the pieces the
  compiler chops the program's text into), *runtime* (the piece of program that
  sits underneath and keeps things working while it runs). One gloss per page,
  not per paragraph: repeating it in every section is its own kind of noise. The
  Italian edition does the same thing in Italian, which is what the two rows
  below are for.
- **Keep the English term.** Do not invent Italian translations for words the
  industry says in English (`token`, `prompt`, `runtime`, `header`, `linker`,
  `commit`, `build`, `parser`, `garbage collector`). Explaining them is the
  job; replacing them teaches the reader a word nobody else uses.
- **Italian spelling follows current usage, not archaic variants.** This is the
  one rule on this page whose subject is Italian words, which is why the words
  themselves are Italian and the rule around them is not: CLAUDE.md § 11 admits
  a translation that is a deliverable, and the Italian edition is written from
  this line. `dai`, never `dài`; `perché`, `finché`, `trentatré` with the acute
  accent; `po'` with an apostrophe. An accent a reader has to stop and look at
  is a mistake even when a dictionary admits it (author instruction 2026-08-26).
- **Never explain a term by using three more.** If the gloss needs its own
  gloss, the sentence is wrong.
- **Prefer the thing over the category.** *"A file the compiler reads to find
  out how a C library is shaped"* beats *"an interface artifact"*. This one was
  settled on the Italian page and holds in both editions.
- The Italian edition is the one the author reads, so where the two editions
  diverge the Italian is fixed to be clearer and the English is then brought up
  to match (CLAUDE.md §11). The glosses added for the Italian belong in the
  English too.

## No em dashes, and here the rule is absolute

Author instruction 2026-08-25: *"NO AI SLOP, get rid of all the em dashes"*. In
site prose there are **none at all**. The two exceptions are evidence and are
never edited: compiler output shown verbatim, and code copied byte for byte from
a repository file, which keeps whatever characters it actually contains.

## The editing pass is a skill

**The 92 lines of editing procedure that stood here until 2026-09-07 are
`site/.claude/skills/no-ai-slop/SKILL.md`.** They were a procedure with two
jobs, a workflow and a checklist, which is what a skill is, while this file is
loaded whenever anything under `site/` is touched: every page edit was paying
for a rulebook it was not using. Invoke `/no-ai-slop` to edit a draft or to
audit one without rewriting it.

What stays here is what binds **every** line of site copy, whoever writes it and
whatever they are doing: no dates, no assumed expert, no em dashes. The wider
rules are `site/README.md` § Style guide for the register and structure, and
CLAUDE.md § 11 for the language of the repository as a whole.

# 030 — M-documentation-site: the one part of the language the site could not teach

## Goal

The site had thirteen docs pages and none of them was about modules, by author
instruction of 2026-09-02: *"aggiornare il sito con queste ultime cose delle
cartelle"*. `use` appeared in passing prose only, so somebody learning Heroes
from heroes-lang.org never met the module system, let alone paths, the
qualifier, or the absence of `..`. That is the feature M-package-layout landed
and M-selfhost-nesting then moved 170 files with, and the site was silent on it.

Delivered: **chapter 13, `Modules and files` / `I moduli e i file`**, in English
and Italian, neither a translation of the other. 185 lines and 195 lines. The
docs go 13 pages per edition to 14, and the site 44 built pages to 46.

## What surprised

**The scheduling item's own complaint was already fixed, and the number that
was actually stale was a different one.** The item said `selfhost.astro`'s
description claims *"165 modules"* against a tree of 169. It says **170**, and
correctly: somebody had repaired it before this milestone opened. What was
stale is the **line count** — ten places across both editions said 49,988 while
the compiler measures **50,100**, because this evening's own repair to the
generic-instantiation key added comment lines after the last close. All ten are
re-measured. `site/README.md`'s build line said 44 pages against a measured 46,
and `docs/ROADMAP.md`'s live table carried 50,063, correct at the previous close
and overtaken by the corpus milestone.

Four stale numbers, and the one the item named was not among them. Re-measuring
what an item asserts is cheaper than trusting it, every time.

**The chapter can make a claim a documentation page usually cannot.** Every code
block is a slice of `examples/shapes/`, and that program prints `10` for the
root's `scale` and `3` for the neighbour's. If path resolution ever flips, the
program's `main.expected` changes and the corpus goes red. The page cannot
quietly stop being true, because the thing it describes is under test.

That is the payoff of the site's own rule about slices, and it was not designed
for this: the rule exists so documentation drift becomes a test failure, and
what it also buys is a page whose *claims* are pinned by a program rather than
by a proofread.

**One of the three diagnostics on the page is a rule that was ratified the same
day.** `shadowed_binding` on a local taking a module's name is panel 102's, and
the compiler enforces it because a corpus program found the hole that afternoon.
The chapter can teach the rule because two other milestones happened first, in
the right order and by accident.

**Chapter 13 rather than a chapter in the middle, and the reason is arithmetic
rather than pedagogy.** Modules arguably belong early. Inserting them would
renumber twelve pages, rewriting every crumb and every previous/next link, for
a gain no reader would notice. `the-c-boundary` stops being the last chapter and
gains a forward link in both editions, which is the whole cost of putting it
last.

## What broke and why

**Nothing broke, and the reason is worth recording**: the site's rules are
written as things a script can check, so the work was verified rather than
reviewed. Zero em dashes in the prose of either edition, measured with the
`<pre>` blocks excluded because verbatim compiler output is evidence and
evidence is never edited. Eight figures across the two editions, all eight
diffed against their `data-src` slice with entities decoded and tags stripped:
**zero drift**. Captions checked against the `data-lines` attributes in the same
pass, so the two cannot disagree. Sixteen links across the two new pages,
**zero dead**, resolved against `dist/` after the build.

**The one Italian slip was mine and the language caught nothing**: I wrote
*"togliata"*, which is not a word. A build passes over prose; only reading it
does not. That is the asymmetry every documentation page lives with, and it is
why the site's rules put as much as possible into checkable form.

## What landed, and what carried forward

**What a reader can now learn from the site that they could not this morning**:
that one file is one module; that a `use` path starts at the directory of the
file you compile, so the same line means the same file from anywhere in the
program; that a neighbour gets no shorter spelling than a stranger, and what
that buys; that there is no way up, and why a file above the root belongs to a
different program; that the qualifier is the path's last part; that two modules
may end in the same word and one file renames with `as`; and that a module's
name is a name, so nothing else in the file may take it.

**Not published.** `site/` shares this branch with the CNAME, so pushing
publishes, and that is a hard stop only the author lifts (CLAUDE.md §14).
Committed and waiting.

**What is owed and is filed rather than done**: `site/src/html/log.html` holds
28 entries against 30 closed milestones, so it is two behind — M-selfhost-nesting
and M-corpus-coverage, both closed today, and this milestone makes three.
`site/README.md` gates the log on the author asking for a refresh rather than on
a milestone closing, so the entries are filed in `docs/work/SCHEDULED.md` with
the measurement instead of written now. The same item carries the rest of a
refresh: the badge on `index.html`, `why.html`'s objections re-checked against
design.md, and every remaining number re-run.

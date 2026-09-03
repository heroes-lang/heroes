# docs/assets — the pictures on the front page, and the rules they follow

Six files, three pictures, each one a hand-written SVG with a dark and a
daylight twin. `README.md` selects between them with `<picture>` and
`prefers-color-scheme`, so a reader gets the one that matches their theme.

| picture | what it is |
|---|---|
| `banner-*.svg` | the marquee: the 1973 bolt, the kicker, the name in its gold quotation marks. The promise under it is markdown in `README.md` (rule 6) |
| `error-*.svg` | one real diagnostic, the compiler's own output |
| `fixpoint-*.svg` | the loop: C makes the compiler, the compiler makes the C, the two are the same bytes |

The art direction is the site's, and it is not re-invented here: one motif (the
bolt, red over blue at eight degrees), one light source (the red and blue wash,
behind the banner and nowhere else), one quotation colour (gold), everything
else paper and ink. The palette is copied from `site/public/style.css`, both
themes, and the rules behind it live in `site/README.md` § Style guide and
§ The visual system.

## Six rules

1. **The banner carries no measured number** (author instruction 2026-08-31).
   The four figures that used to sit along its bottom edge are markdown in
   `README.md` now, right under the picture. A number in a drawing ages in
   silence, because nobody re-measures a drawing, while CLAUDE.md §1 wants every
   count taken in the session that writes it. Markdown is also greppable, which
   an SVG label is not in practice.

2. **A number that does stay in a picture names its command and its date in the
   file comment.** The fixpoint diagram keeps three, by author decision on the
   same day: the seed build, the emission and the seed's size. Its comment says
   what was run and when, so the next reader can re-run it instead of trusting
   it. Where the figure moves at nearly every commit, the picture rounds it: the
   seed says `22 MB` rather than its byte count, which changes whenever the
   compiler regenerates it.

3. **Twins differ ONLY in their palette block.** A layout change is made twice or
   not at all. The palette block is the first thing in each file for exactly this
   reason.

4. **Path data is written `M 14 0`, with the space.** A bare capital `M` followed
   by a digit is the milestone shape the net refuses in a living file
   (`tests/harness/suite_records.hero`, `records/numbered`), and the bolt's path
   starts with one. `editors/vscode/icons/` is excluded from that walk; this
   directory is not, and should not be.

5. **Compiler output in a picture is verbatim or it is not there.** The error
   card's six lines were compared character by character against what
   `heroes check tests/golden/check/typo-not-a-new-binding.hero` printed, and
   they are identical. That is why the em dash in the message stays:
   `site/CLAUDE.md` bans em dashes in outward-facing prose and exempts evidence,
   which is never edited. Each line is one `<text xml:space="preserve">` whose
   coloured runs are `tspan`s with no `x` of their own, so the glyphs flow and
   the caret row stays under the span it marks whatever monospace font the
   reader has.

6. **The banner carries no sentence either: the promise is markdown, and it
   is the site's line** (author instruction 2026-09-03). The site's hero
   promise changed in `0367070` and the banner went on saying the old one, so a
   sentence in a drawing ages exactly the way rule 1 says a number does. The
   line under the banner in `README.md` is now the same words, with the same
   `<em>`, as the `promise` paragraph in `site/src/html/index.html`, and when
   one moves the other follows. The picture keeps what does not change: the
   bolt, the kicker and the name.

## Checking a change

Open the file in a browser, or preview `README.md` itself: nothing here needs a
build step, a font download or a network. Two things are worth a look before a
commit, because neither shows up in the source: that the SVG still parses (an
unescaped `--` inside an XML comment is a silent killer, and cost one round
here), and that the text still fits its plate under a fallback font, since
`Avenir Next Condensed` exists on macOS and resolves to something wider almost
everywhere else.

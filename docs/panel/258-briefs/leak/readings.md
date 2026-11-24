# The memory readings of the leak test, 2026-11-24

`/usr/bin/time -l`, maximum resident set size in bytes, one process each, on a Mac loaded by about ten other agents
(macOS compresses resident pages, so a low reading is the machine's). The two sources differ only by the blank lines
`heroes fmt` added; the 100,000-jump programs are `main_100k.hero`.

| jumps | source | reading |
|---|---|---|
| 1,000 | unformatted, first run (`time_1k.txt`) | 5,963,776 |
| 1,000 | formatted, first run | 6,012,928 |
| 100,000 | unformatted, first run (`time_100k.txt`) | 352,894,976 |
| 100,000 | formatted, first run | 307,429,376 |
| 100,000 | unformatted, second run | 338,821,120 |
| 100,000 | unformatted, third run | 352,911,360 |
| 100,000 | unformatted, fourth run | 352,911,360 |
| 100,000 | formatted, second run | 352,894,976 |
| 100,000 | formatted, third run | 352,894,976 |
| 100,000 | formatted, fourth run | 352,927,744 |

Eight readings at 100,000 jumps, between 307,429,376 and 352,927,744; six of them within 352,894,976 and 352,927,744. The
critic's `leaks --atExit` count on the same program at 1,000 jumps is 7,000 leaks for 3,952,000 bytes, so **7 blocks and
3,952 bytes a jump** (`docs/panel/258-reports/critic-briefs.md`, row 1.13).

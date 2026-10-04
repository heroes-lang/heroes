# Defect 085 closed: a mark's releaser is read in the mark's own module, from whichever file is checked

2026-09-25, M-agreed-retention step 11, in lane `9f813de2`, merged `62324531`:
panel 176's item 5, the per-module reading. Found by panel 176's
ffi-pragmatist (its § 5 item 4).

- [x] **085 — `unread_releaser` answers 1 for a module checked alone and 0 for the same module checked inside its program** | the rule's message says *no `extern` of this module declares it* and its lookup resolves over the whole program, so the verdict depends on which file is handed to `check` | `selfhost/check/acquiring.hero:269` · **closed 2026-09-25**

    **Origin:** panel 176's ffi-pragmatist, 2026-09-23 (its § 5 item 4);
    reproduced by the coordinator the same day before filing.

    **The reproducer.** `bio.hero` declares `h_open() -> H acquires h_close2`
    and only `main.hero` declares `h_close2`; `main.hero` uses `bio`.
    `heroes check bio.hero`: **exit 1**, `error[unread_releaser]`.
    `heroes check main.hero`: **exit 0**, and the seat measured `build` and `run`
    at 0 too.

    **Why it is a defect.** One program, two verdicts, and the message states
    the rule the lookup does not apply. Which of the two is right is the
    repair's question: the per-module reading is what the message and
    `check/acquiring.hero`'s own note say, and the program-wide one is what lets
    a libcrypto BIO mark name libssl's calls today (the seat's § 2c).

## The repair

`ends_a_life` (now `check/releasers.hero`) walks the whole resolved program,
as every program-wide rule does, and counted a declaration in ANY file while
its message said *no `extern` of this module declares it*. It reads the FILE
the mark sits in now (`source.file_of`), so a releaser declared only in
another module does not make the mark true, and `check bio.hero` and `check
main.hero` refuse the same line. The repair is to declare the releaser beside
the mark that names it; a libcrypto mark naming libssl's call declares that
call in its own group.

## The measurements

| program | before (trunk `fb0b7cb6`) | after |
|---|---|---|
| `surface-fixtures/releaser085/bio.hero` checked alone | exit 1, `unread_releaser` | exit 1, `unread_releaser` at 7:37 |
| `surface-fixtures/releaser085/main.hero`, the program | exit **0** | exit 1, the same line |

Two rows in `tests/harness/suite_surface.hero`. The checker has no platform,
and the rows run on every leg of the CI.

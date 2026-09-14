- [ ] **M-marked-acquisition 4** | Open `selfhost/print/dump.hero` and find `owned_suffix`. Read the comment directly above it — the one about `heroes fmt` comparing its own output against this dump. Then look at what the function next to it printed for a parameter, as it stood on 2026-09-13. **Before reading further: say what that comment predicts, and then say whether the file it is written in was obeying it.**

    **Where to look:** `selfhost/print/dump.hero`, the `owned_suffix` comment and
    the `signature` renderer above it; `git show bd3238cd:selfhost/print/dump.hero`
    for the state it describes;
    `docs/records/done/2026-09-14-1700-the-mark-lands-in-both-positions-and-the-dump-had-been-eating-consumes-for-a-day.md`.

    **Why it matters:** the comment says, in terms, that *a form missing HERE
    makes the formatter's self-check agree with a formatter that is deleting
    it* — and names the word that shipped that way before. One line below it,
    the renderer printed `owned` and **not** `consumes`, so for a day
    `heroes parse --dump-ast` turned
    `function curl_easy_cleanup(handle: Curl consumes)` into
    `function curl_easy_cleanup(handle: Curl)`. Exit 0. No diagnostic.

    **The shape is worth more than the instance.** A guard and the thing it
    guards can share a blind spot, and when they do, the guard reports agreement
    — which reads exactly like correctness. Here the two were the same code
    path: the formatter's self-check compares its output to the dump, so a form
    neither knows is a form both agree about.

    **And the reason no test caught it is the second half.** The dump's own
    fixture carried `owned free` and `owned drop` and neither new mark, so the
    case that would have failed did not exist. Ask, of a test suite you are
    about to trust: **which forms does its fixture NOT contain, and would you
    know?**

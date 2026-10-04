- [ ] **M-reflection-verdict mutation drill** | One program, two spellings, the same four keys: bare strings score 0 of 4 killed and `Room::width` scores 5 of 5. Predict the two numbers for a THIRD spelling before running it — the keys declared as `constant K_WIDTH: str`.

    **Where to look:** `heroes mutate <dir> --operator typo-key --survivors` and
    `--operator typo-ident`, and `selfhost/mutate/typo.hero::typo_string_key`.

    **Why it matters:** the third spelling is the one the seat guarding the
    document argued for, at **zero** cost to the language, and the milestone
    refused it for one measured reason. Working out what that reason is from the
    two numbers is the whole exercise, and the answer is a sentence about what a
    `constant` is bound to and what a field name is bound to.

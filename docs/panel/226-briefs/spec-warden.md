# Panel 226, the spec-warden's brief

Read `00-shared.md` first. Your copy: `<scratchpad>/226-spec-warden/`, `git archive` of
`a0a8d1249`; build your own compiler there from the seed. Write your report as you go to
`docs/panel/226-reports/spec-warden.md` in the repository's lane
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/thesis/`. **No paid run, no credential
printed.** The one endpoint you may call is `POST /v1/messages/count_tokens`, which is free and the
only one `.claude/hooks/guard_bash.py` lets a shell reach; the key is `ANTHROPIC_API_KEY` in the
trunk's `.env` (`set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env; set +a`, and `curl --variable
%ANTHROPIC_API_KEY --expand-header "x-api-key: {{ANTHROPIC_API_KEY}}"`, never `-H` with the value).
Never print the variable.

The proposal spends no spec token: it amends design.md Part 11 and the instrument's documents. Your
mandate is the indicator, §1.2's formula and §1.6's budget, and Principle 0's burden; grep §1.6 for
today's ceiling before you write a number.

1. **Count what the protocol sends**, each on `claude-haiku-5-5`, `claude-sonnet-5-5`,
   `claude-opus-5-5` and `claude-fable-5-1` (the coordinator counted the spec alone today: 10,294 on
   all four as one user message): the frozen template `docs/metrics/first-try.md` rendered with the
   spec and one task of about 200 words you write (a word-count program, inputs by argument, output
   by `print`), as **one user message** as the template says, and as a **`system` block holding the
   template's head and the spec with `cache_control` plus a user message holding the task** (the
   plan's cache layout); the ten shortest `.expected` of `tests/golden/full/` as a user turn each (a
   diagnostic fed back); the spec with its 46 production lines removed (`grep -vE '^    [A-Za-z]+
   += '` and the continuation lines under them: say how you cut); the spec plus one worked
   diagnostic (issue 1005: pick one `.expected` of `tests/golden/full/` and append it inside a fence).
2. **The prompt's identity.** The template's sha256 is `b7f06aab65cd9790…` today. Moving the spec
   into a `system` block changes what the model receives in shape and not in words: is that a new
   prompt series (a new sha) or the same template rendered two ways? Say which, and what the
   `prompt sha` in provenance should cover (the rendered bytes with placeholders, the request body's
   structure, or both).
3. **§1.2's formula**, real cost = tokens × (1 + rewrite rate): the instrument measures the rewrite
   rate for the first time (turns to green, tokens to green). Say how its columns map onto the two
   factors, and whether §1.6's sentence that the rewrite-rate factor *stops being an audited one
   (panel 011)* (`docs/design.md:385-387`) comes out of design.md once the instrument runs. Count
   with stage 2 where you run the compiler; `heroes measure` prints `STALE` today (the pinned count
   is an earlier digest's): say what the ledger owes.
4. **The budget arithmetic**, re-run with your counts and the prices in `00-shared.md`: the trial
   (10 tasks × 3 samples × 3 arms sharing turn 1 × Haiku, Sonnet, Opus 5.5, 5 turns, answers of
   1,500 output tokens and of 6,000 with thinking), the held-out run (40 tasks × 3 × 3 arms × 4
   models), the canary, the variant contexts (10 tasks × 3 × 2 contexts × Haiku and Sonnet). Cache
   reads where the layout allows them, writes at 1.25×, the second `check` per turn costs no token.
   Mark every price as the skill's cached figure.
5. **§1.6's consolidation block**: design.md says 1,500 real tokens are budgeted for it *before the
   thesis harness has run*. Does the first run change that sentence, and how? Read it and say.
6. **Principle 0**: the instrument enters by §1.1 (comprehension measured) and not by compiler need;
   does Part 11's amendment carry its own burden, and what would you strike from the proposal's
   paragraph as not measured by anything?

Output the structure your definition asks for; `spec_token_delta` is 0 on the spec and the counts
above are your `experiment`.

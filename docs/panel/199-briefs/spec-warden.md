# Panel 199, spec-warden

Read `00-shared.md` first, whole. Your directory `<scratchpad>/199-spec-warden/`,
your copy `tree/` inside it with its compiler from its seed, your running
notes `report.md` beside it. Read `spec/heroes-spec.md` whole first, in your
copy. Repaired 2026-10-08 on the critic's first pass; the version it read is
`first-pass/spec-warden.md`.

## The numbers, measured

`heroes measure spec/heroes-spec.md`, run at 07:54 on 2026-10-08 by `date` in
`<scratchpad>/199-briefs-work/tree` (a copy of `56def9b4`, its compiler from
its seed), exit 0:

```
  claude-legacy      7337   (64995 ranks)
  cl100k_base        7467   (100256 ranks)
  maximum            7467   a lower bound, not the reader's tokeniser
  spread              130   between the two vendored tables (1%) ...
  real               9831   claude-opus-5, 2026-10-07 — the binding number
```

and *Headroom: 409 against the 10240 ceiling — but the FFI floor mortgages 60
of it (panel 030 R3), so what is measured against the ceiling is 9891*; *an
addition needs a named removal or a pre-registered falsifiable prediction
(panel 012)*. Run it again in your copy and say whether it agrees.

## The real count of a draft

The first version said `measure --refresh` is paid and not approved. What is
known: it posts to one endpoint, `https://api.anthropic.com/v1/messages/count_tokens`
(`selfhost/cli/refresh.hero:33`), and prints a record for a person to paste,
its request bodies and answers landing under `build/` (its comments at `:1-2`
and `:40`). The critic read that
endpoint as free from Anthropic's documentation (the bundled claude-api
skill: *costs nothing and samples nothing*); **nobody in this sitting has run
it, so free is a reading of a document, not a measurement.** It needs
`ANTHROPIC_API_KEY`, which the trunk's `.env` holds and a copy does not
(`.claude/rules/records.md` § Working in lanes).

So: **if, and only if, the coordinator's launch prompt confirms it in so many
words**, run `--refresh` in your own copy, after `.
/Users/joseph/Temp/heroes/heroes-lang/.env` and printing `${#ANTHROPIC_API_KEY}`,
never the value. Otherwise price every draft with the `maximum` row, which is
a lower bound of the real count and not the reader's tokeniser, and write each
draft's real count as unrun; the landing prices it.
`.claude/rules/spec-shape.md:160` records the author's *always measure with the
real*.

## The questions

1. **Does the rule need a spec sentence** (Principle 0: the compiler needs it,
   or it serves the thesis by a measured effect)? Where would it go (`:280`'s
   *Recursion too deep aborts*, the path ends at `:238-240`, or `:267` for
   route (L)), its draft, its cost on the rows above.
2. **`:280` is false at `-O2`**, `heroes run`'s default level, for every shape
   of `00-shared.md`'s table but p1 and p2 (defect 508). Does the spec name
   the level, or does the language make the sentence true (route (G)), and
   what does each cost in tokens?
3. **Is a function whose every path calls itself a mistake an LLM plausibly
   makes?** The shape is a self-call through UFCS on a name the author
   believes is a method (`bytes`, `count`: neither is a built-in, § 11
   `:308-314`). Say what the spec already tells a reader that would prevent it
   (`:267`, `x.f(y)` is `f(x, y)`; `:113`) and what it does not.
4. **Does a diagnostic need a spec row at all?** The spec names no diagnostic
   code (`grep -c 'error\[' spec/heroes-spec.md`, 0), and
   `polymorphic_recursion`, a build-time refusal of a recursion shape, has no
   spec sentence (`grep -c polymorphic spec/heroes-spec.md`, 0). Say whether
   that is the precedent here.
5. **Warnings**: the spec has no warning class (`grep -n -i warning`, no
   line), and design.md:3725 says the language has none (`00-shared.md`).
   Does a sentence anywhere imply one?
6. If you draft a sentence the blind reader should see in variant B, write it
   exactly, with the line it replaces or follows; else say *unchanged*.

Verdict on the budget (design.md §1.6) and Principle 0. Veto on budget breach.

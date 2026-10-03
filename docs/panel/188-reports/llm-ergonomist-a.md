# Panel 188, the llm-ergonomist's blind arm A

Run by the coordinator on 2026-10-03 as one fresh `claude -p` session from `<scratchpad>/188-llm-ergonomist/a/`, outside any git tree, with the command of `docs/panel/188-briefs/llm-ergonomist.md` (`--model claude-opus-5-5 --restricted --safe-mode --strict-mcp-config`, tools Read and Write, `--max-budget-usd 1.5`). The CLI 2.1.285 reported `claude-opus-5-5` (and `claude-haiku-4-5-20251001` for its own use), 0.1767 USD, 5 turns, no error. Its brief is `docs/panel/188-briefs/blind/a-brief.md`. The coordinator built its `c.hero` with the trunk's compiler at `826ddc2f`: `check` 0, `build` 0, it printed `1.4142135623730951`, the intended output: **a one-turn repair**.

## c.hero

```
extern "math.h" link "m"
    function sqrt(x: f64) -> f64

function main()
    print(sqrt(2.0))
```

## The session's report, verbatim

# experiment

```
extern "math.h" link "m"
    function sqrt(x: f64) -> f64

function main()
    print(sqrt(2.0))
```

# reading

The decisive line was the generated C, `#include <<math.h>>`, shown twice in the clang output, together with clang's own fix-it suggesting `<math.h>`. It shows that the compiler wraps the extern string in angle brackets itself, so the string must be the bare header name. Edit: `extern "<math.h>"` became `extern "math.h"`. This matches spec section 13, whose example writes `extern "sqlite3.h"` with no brackets. I also added `link "m"` on my own judgement, not because of the message: on Linux, `sqrt` lives in libm and needs `-lm`, and on macOS `-lm` is accepted and harmless.

# confidence

High. The include error is the only error reported, and its cause is clear. `heroes check` already accepted the rest: the f64 parameter matches C's `double`, the literal `2.0` takes the type `f64` from context, and `print` takes an f64. The remaining risk is `link "m"`: it might fail if the toolchain had no libm to link against, which is unlikely on macOS or Linux. If clang turns `sqrt` into a builtin, the link is not needed but does no harm. Expected output: `1.4142135623730951`.

# argument

Yes, a one-turn repair was likely. The message is labelled "internal error", which wrongly suggests a compiler bug rather than a user mistake, and it points at a generated C file the user never wrote. It never names the Heroes source line or says "write the header name without angle brackets". Still, the doubled `<<math.h>>` makes the cause obvious to anyone who knows C, and clang's fix-it nearly spells out the answer. The spec's `"sqlite3.h"` example confirms it. One weakness: nothing in the message says whether the compiler adds quotes or angle brackets, so the include form is inferred, not stated. It also gives no hint about linking libm, which a Linux build could need next.

# choice_points

- Header string: `"math.h"` (chosen) versus keeping some bracket form. The output shows the compiler adds `<>` itself, so any brackets in the string would be doubled again and fail the same way.
- Library: `link "m"` (chosen) versus no `link`. Leaving it out compiles on macOS, where libm is part of libSystem, and usually works anywhere clang folds or inlines `sqrt(2.0)`. On Linux without folding it could fail at link time with an undefined `sqrt`. Adding it costs nothing where libm exists.
- Output format: `print(sqrt(2.0))` (kept) versus formatting with `f"..."` or `to_str`. The brief only asks to print the value, and `print` writes a float in round-trip form, `1.4142135623730951`.

# prediction

90 of 100 models produce a version that compiles and prints the square root of 2 in one turn. Almost all will remove the angle brackets. The few failures would come from misreading "internal error" as a compiler bug and leaving the source unchanged, or from making unneeded changes elsewhere that break something.

# context

Only `brief.md` and `spec.md` from this directory were read. Beyond those, my context held the standard harness system prompt and environment details (macOS platform, working directory, date) and an automatically attached user email address, which I did not use. No other files or tools were used.

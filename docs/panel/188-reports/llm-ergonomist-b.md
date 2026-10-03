# Panel 188, the llm-ergonomist's blind arm B

Run by the coordinator on 2026-10-03 as one fresh `claude -p` session from `<scratchpad>/188-llm-ergonomist/b/`, outside any git tree, with the command of `docs/panel/188-briefs/llm-ergonomist.md` (`--model claude-opus-5-5 --restricted --safe-mode --strict-mcp-config`, tools Read and Write, `--max-budget-usd 1.5`). The CLI 2.1.285 reported `claude-opus-5-5` (and `claude-haiku-4-5-20251001` for its own use), 0.171 USD, 5 turns, no error. Its brief is `docs/panel/188-briefs/blind/b-brief.md`. The coordinator built its `c.hero` with the trunk's compiler at `826ddc2f`: `check` 0, `build` 0, it printed `1.4142135623730951`, the intended output: **a one-turn repair**.

## c.hero

```
extern "math.h"
    function sqrt(x: f64) -> f64

function main()
    print(sqrt(2.0))
```

## The session's report, verbatim

# experiment

```
extern "math.h"
    function sqrt(x: f64) -> f64

function main()
    print(sqrt(2.0))
```

# reading

The message has one error, `header_name`, at `p.hero:1:8`, with a caret under `"<math.h>"`. Its explanation (the compiler writes `#include <...>` itself, so the name cannot hold `>`; "the name is `math.h`") and its line `fix (certain): replace "<math.h>" with "math.h"` led to the only edit: the string on line 1 became `"math.h"`. I changed nothing else. The message names no other problem, and nothing else in the program conflicts with the spec: `sqrt(x: f64) -> f64` matches C's `double sqrt(double)` (spec 13 lets a result be checked against the header, and `f64` is a double). `2.0` takes the type `f64` from the parameter (spec 2). `print` accepts a float (spec 11).

# confidence

High. The fix is exact and marked certain, and I checked the rest of the program against the spec without finding a second error. The only remaining risk is linking: the spec says to add `link` "when the symbols need one", and on some Linux toolchains `sqrt` needs `-lm`. But clang normally makes `sqrt` a builtin when the argument is a constant, and on macOS libm is part of libSystem. The expected output is `1.4142135623730951`, a float printed so it reads back as the same value (spec 11).

# argument

Yes. The message says what is wrong, why the language does not allow it (the compiler writes the include brackets itself), what the right name is, and gives a mechanical replacement marked certain, with a caret on the exact span. A model only has to copy the replacement. The one gap is that it does not say whether this is the program's only problem, because a check can stop at its first error. Here it is the only one, but a model cannot know that from the message alone. It has to check the rest against the spec itself. The message also says nothing about `link "m"`, which a cautious model might add or worry about.

# choice_points

- Adding `link "m"` to the group. I left it out: the fix did not ask for it, and the spec only asks for `link` when the symbols need a library. Adding it would most likely also compile and print the same result (libm exists on both macOS and Linux). On a system with no linkable `libm` it would fail at link time.
- Writing `2.0` or `to_f64(2)`. I chose `2.0`, which the context types as `f64`. `to_f64(2)` would produce the same value. Writing `2` would be refused, because a literal without a point is an integer and nothing converts it implicitly (spec 3).
- Printing the result directly or binding it first (`r = sqrt(2.0)`). Both print the same. I kept the original form.

# prediction

97 of 100 models produce a program that compiles and prints the square root of 2 in one turn. The few failures would come from extra edits that are not needed, such as a malformed `link` clause or changing the signature.

# context

Only `brief.md` and `spec.md` from this directory were read. The system context also included the harness's environment details and the user's account email. Neither affected the repair.

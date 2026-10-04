# Panel 192, the llm-ergonomist's blind arm A: A, the specification at the base

Copied by the coordinator from `<scratchpad>/192-llm-ergonomist-2/`. Each session ran as a fresh
`claude -p` session in its own folder outside any git tree, with the command of
`docs/panel/192-briefs/llm-ergonomist.md` (`--restricted --safe-mode --strict-mcp-config`, tools
`Read` and `Write` only), its cap `--max-budget-usd 0.24` for the trial `a1` and `0.25` for the
rest (see `llm-ergonomist.md`). Scored by `<scratchpad>/192-blind-score.py`, written before
any session ran. The model the CLI reports and the cost are `run.json`'s; the scorer's line
is quoted whole.

## Session a1

- **cost**: 0.2348 USD; **models**: claude-haiku-4-5-20251001, claude-opus-5-5; **ended**: `success`, 5 turns
- **scorer**: pass `True`, spelling *C through extern*, build exit 0, output `1b 5b 33 31 6d 45 52 52 4f 52 1b 5b 30 6d 0a`, *pass*

### `c.hero`

```
# Prints ERROR in red, resets the colour, ends the line.
# Heroes strings have no escape for ESC (27), so every byte goes through C.
extern "stdio.h"
    function putchar(c: i32) -> i32

function main()
    codes: [i32] = [27, '[', '3', '1', 'm', 'E', 'R', 'R', 'O', 'R', 27, '[', '0', 'm', '\n']
    for b in codes
        _ = putchar(b)
```

### `report.md`

# experiment

```
#### Prints ERROR in red, resets the colour, ends the line.
#### Heroes strings have no escape for ESC (27), so every byte goes through C.
extern "stdio.h"
    function putchar(c: i32) -> i32

function main()
    codes: [i32] = [27, '[', '3', '1', 'm', 'E', 'R', 'R', 'O', 'R', 27, '[', '0', 'm', '\n']
    for b in codes
        _ = putchar(b)
```

#### reading

- "Six escapes, and no others ... Any other escape is a compile error." So `\x1b`, `\e` and `\033` are not available, and no string literal can hold ESC.
- Section 11 lists no built-in that turns a number into a one-byte `str` (`to_str(27)` gives `"27"`). Because of that, the ESC byte cannot come from Heroes text, and I went to section 13: "Anything beyond this document ... comes from C libraries."
- "A **parameter** ... declared at the header's own width and sign: `i32` where C says int" gave `putchar(c: i32)`. "a result may be wider than C's" means `-> i32` is fine as well.
- "A group names its header, and `link` a library when the symbols need one": libc needs no `link`, so it is just `extern "stdio.h"`.
- "A character literal is an integer" and "A literal takes the type its context asks for" gave the `[i32]` array of `27` and character literals, so no conversions are needed. `'\n'` is one of the allowed escapes in a character literal.
- "an empty one needs an annotation" (this one is not empty). "`v: i64 @ 0` ... `x = 5`": `codes: [i32] = [...]` is the Binding form `":" Type "="`.
- "A line that computes a value must use it ... discard it on purpose with `_ = f(x)`" gave `_ = putchar(b)`.
- "named arguments are mandatory" only when two parameters share a type, so a positional `putchar(b)` is allowed.
- "the file you compile holds `function main()`".

#### confidence

Moderate to high that it compiles. Every construct maps to a production or an example: extern Member, typed binding, array literal, for, `_ =`. One risk is that some `stdio.h` implementations define `putchar` as a macro or an inline function, and the clang check could treat that differently. Another is that the compiler might reject a binding that is used only as a loop source (I think it counts as a read). Behaviour: all output goes through C's stdout, so nothing is interleaved with Heroes' own `print` buffer. The final newline flushes a line-buffered terminal, and normal exit flushes in any other case. Overall I expect it to work at once, at about 75%.

#### argument

The spec is complete enough to write this program, but you only get there by deduction. Nothing says outright that ESC is unreachable from a string literal. A model has to notice that only six escapes exist, look for a byte-to-str conversion, find none, and then turn to FFI. Many models will write `"\x1b[31m"` or `"\e[31m"` out of habit, which the spec says is a compile error. Once FFI is chosen, the `putchar` declaration follows directly from the width rule. Mixing `print` with C output is another trap the spec does not mention, because it says nothing about buffering. A one-turn success is plausible but not likely for a typical model.

#### choice_points

- **How to get the ESC byte.** Choice: C `putchar` for every byte. Alternatives: (a) a `\x1b`/`\e`/`\033` escape, which is a compile error; (b) `print` for the text with `putchar` only for ESC, which risks bytes appearing out of order because the two buffers are unrelated; (c) `write(2)` with a `ptr`/`cstr` buffer, which may be refused because `const void *` and `cstr` disagree, and still needs a string containing ESC.
- **Reset code.** Choice: `ESC[0m` (reset all attributes). Alternative: `ESC[39m` (default foreground only). Both put the colour back to the default and look the same here.
- **Red code.** Choice: `31` (standard red). Alternative: `91` (bright red), which is also red but not the base ANSI red.
- **Element type.** Choice: `[i32]` matching the `putchar` parameter. Alternatives: `[u8]` or a `str` indexed with `s[i]`, which yield `u8` and would need `to_i32(...).must()` or a `u8` parameter. The spec allows that only if "C converts exactly" applies to u8 against int, which it probably does but does not say.
- **Newline.** Choice: `'\n'` through `putchar`. Alternative: `print("")` for the newline, which mixes buffers.
- **Argument naming.** Choice: positional `putchar(b)`. Alternative: `putchar(c: b)`, which is equivalent.
- **Result type.** Choice: `-> i32`. Alternative: `-> i64`, which is allowed because a result may be wider.

#### prediction

About 35 of 100 models produce a program that compiles and prints the right bytes in one turn. Most failures will use a forbidden escape such as `\x1b`, `\e` or `\033`. Fewer will declare the FFI wrongly (wrong width, `link "c"`, a variadic `printf`), and some will interleave `print` with C output.

#### context

Only `brief.md` and `spec.md` from this directory were read. The system context also included an automatically attached user email address and environment details (working directory, platform, date). None of these affected the program.

## Session a2

- **cost**: 0.2387 USD; **models**: claude-haiku-4-5-20251001, claude-opus-5-5; **ended**: `success`, 5 turns
- **scorer**: pass `True`, spelling *C through extern*, build exit 0, output `1b 5b 33 31 6d 45 52 52 4f 52 1b 5b 30 6d 0a`, *pass*

### `c.hero`

```
extern "stdio.h"
    function putchar(c: i32) -> i32

# Writes every byte of `s` to standard output through C, with no newline.
function emit(s: str)
    for i in range(from: 0, to: s.len())
        _ = putchar(c: to_i32(s[i]).must())

function main()
    _ = putchar(c: 27)
    emit("[31mERROR")
    _ = putchar(c: 27)
    emit("[0m")
    _ = putchar(c: '\n')
```

### `report.md`

# experiment

```
extern "stdio.h"
    function putchar(c: i32) -> i32

#### Writes every byte of `s` to standard output through C, with no newline.
function emit(s: str)
    for i in range(from: 0, to: s.len())
        _ = putchar(c: to_i32(s[i]).must())

function main()
    _ = putchar(c: 27)
    emit("[31mERROR")
    _ = putchar(c: 27)
    emit("[0m")
    _ = putchar(c: '\n')
```

#### reading

- "Six escapes, and no others ... Any other escape is a compile error." There is no `\x1b` or `\e`, so the ESC byte cannot be written as an escape. No built-in turns a number into a one-byte `str` (`to_str(27)` gives `"27"`). This pushed me to C.
- "Anything beyond this document ... comes from C libraries" and the `extern` grammar led to `extern "stdio.h"` with no `link` (libc needs none: "`link` a library when the symbols need one").
- "A parameter and a field are declared at the header's own width and sign, `i32` where C says int" gave `c: i32`; "clang checks every result type" gave `-> i32`.
- "When two parameters ... share a type, named arguments are mandatory" made me name `from:`/`to:` in `range`; I also named `c:` at the call to `putchar`, which the `Arg` production allows.
- "A line that computes a value must use it ... discard it on purpose with `_ = f(x)`, which a `()` line refuses" gave `_ = putchar(...)` and a bare `emit(...)`.
- "A literal takes the type its context asks for" and "A character literal is an integer" let `27` and `'\n'` become `i32`.
- "`s[i]` yields a `u8`" and "`to_i8` ... `to_u64` give a `T?`" gave `to_i32(s[i]).must()`.
- `function main()`, "takes nothing and produces nothing", 4-space indentation, `#` comment.
- I routed every byte, newline included, through `putchar` so that no ordering question arises between C's stdio buffer and `print`, which the spec does not describe.

#### confidence

Moderately high that it compiles; high that, if it compiles, it prints the right bytes. Risks: whether `to_i32` accepts a `u8` (the spec lists the converters but not their argument types); whether a named argument is accepted on an extern call with one parameter; whether `putchar` being a macro in some headers bothers the clang check; whether C's stdout buffer is flushed when `main` returns (normal C exit flushes, but the spec does not say how `main` ends).

#### argument

The spec makes most of the program easy: bindings, discards, literal typing and the extern syntax are stated precisely. It fails on the one thing this task needs: a way to put the byte 27 into output. The escape list excludes `\x1b`, and no `chr`-like built-in exists, so a model must either notice that a raw ESC byte is legal inside a string ("strings any [UTF-8] but a raw carriage return or line end") or reach for FFI. Many models will write `"\x1b[31m"` or `"\033[31m"` by habit, which is a stated compile error. The FFI route works but depends on details (int width, unflushed buffers, mixing with `print`) the spec only partly settles.

#### choice_points

- Producing ESC: FFI `putchar(27)` (chosen); a raw 0x1B byte in a string literal with `print` would be shorter and likely legal, but is invisible in source and fragile to editors; `\x1b`/`\033`/`\e` would be a compile error.
- Output channel: everything via `putchar` (chosen); mixing `print` for text with `putchar` for ESC risks misordered output if the two buffer separately; `print` alone also cannot omit its newline mid-line.
- Reset code: `ESC[0m` (chosen, resets all attributes, the common idiom); `ESC[39m` resets only the foreground colour, also correct for this task.
- Parameter type of `putchar`: `i32` (chosen, matches int); `u8` or `i16` might be accepted as "C converts exactly" and would avoid `to_i32(...).must()`.
- Result type: `i32` (chosen); `i64` is allowed ("a result may be wider").
- Named argument `c:` at the extern call (chosen); positional would also be valid by the grammar.
- Byte loop over `range` with indexing (chosen); `chars()` yields `str`, which `putchar` cannot take.
- Header: `stdio.h` with no `link` (chosen); adding `link "c"` would probably also work.

#### prediction

30 of 100 one-turn programs compile and do what they should. Most will try a `\x1b`-style escape, which the spec forbids.

#### context

Only `brief.md` and `spec.md` from this directory, plus the harness's system prompt (which includes a user email address and environment details irrelevant to the task). No other files or tools beyond reading and writing here.

## Session a3

- **cost**: 0.2219 USD; **models**: claude-haiku-4-5-20251001, claude-opus-5-5; **ended**: `success`, 5 turns
- **scorer**: pass `True`, spelling *C through extern*, build exit 0, output `1b 5b 33 31 6d 45 52 52 4f 52 1b 5b 30 6d 0a`, *pass*

### `c.hero`

```
# Prints ERROR in red, then restores the default colour.
# A string has no escape for ESC (27), so every byte goes through C's putchar.

constant ESC: i32
    27

extern "stdio.h"
    function putchar(c: i32) -> i32

function emit(s: str)
    for i in range(from: 0, to: s.len())
        _ = putchar(to_i32(s[i]).must())

function main()
    _ = putchar(ESC)
    emit("[31mERROR")
    _ = putchar(ESC)
    emit("[0m\n")
```

### `report.md`

# experiment

```
#### Prints ERROR in red, then restores the default colour.
#### A string has no escape for ESC (27), so every byte goes through C's putchar.

constant ESC: i32
    27

extern "stdio.h"
    function putchar(c: i32) -> i32

function emit(s: str)
    for i in range(from: 0, to: s.len())
        _ = putchar(to_i32(s[i]).must())

function main()
    _ = putchar(ESC)
    emit("[31mERROR")
    _ = putchar(ESC)
    emit("[0m\n")
```

#### reading

- "Six escapes, and no others ... Any other escape is a compile error." So `\e`, `\x1b` and `\033` are out, and a string literal cannot hold ESC unless the byte is written raw.
- "print writes its values with no separator and exactly one trailing newline, and takes ... a number, `str` or `bool`." So `print` cannot write ESC either: an integer prints as digits, and no built-in makes a `str` from a byte. This is why all output goes through C.
- "Anything beyond this document ... comes from C libraries. A group names its header, and `link` a library when the symbols need one." This gave `extern "stdio.h"` with no `link`, because putchar is in libc.
- "A parameter ... declared at the header's own width and sign: `i32` where C says int" and "a result may be wider than C's." These gave `putchar(c: i32) -> i32`.
- "A line that computes a value must use it: ... discard it on purpose with `_ = f(x)`." This gave `_ = putchar(...)`.
- "`s[i]` yields a `u8`", "No implicit conversions, widths included" and "`to_i8` ... `to_u64` give a `T?`". These gave `to_i32(s[i]).must()`.
- "`range(from: a, to: b) -> [i64]` exclude `to`" and "`x.f(y)` is sugar for `f(x, y)`". These gave the loop over `s.len()`.
- "A literal takes the type its context asks for" and the constant example. These gave `constant ESC: i32` with body `27`.
- "`function main()`, which takes nothing and produces nothing." This gave main.

#### confidence

Moderate, around 65%. Types, discards and the extern declaration follow the text closely. The risks: (1) the spec never says whether an extern group with no `link` is accepted (libc implicit), though the "when the symbols need one" wording suggests it is; (2) `len` is listed as a built-in without a signature, and I assume it returns `i64`, which `range` needs; (3) if the runtime also writes to stdout through its own buffer, mixing would not matter here because only putchar writes, but I assume C's stdout is flushed when main returns. Output should be ESC[31mERROR ESC[0m newline.

#### argument

The specification is precise about what it forbids, which is how I knew in one pass that no string escape and no `print` call can emit byte 27. It does not say how to produce a control byte, which is the whole task. A model must infer that the answer is FFI, then get widths, the discard rule and the `u8` to `i32` conversion right. Many models will write `"\x1b[31m"` or `"\e[31m"` from habit and fail. Others will try `print` with a byte and get digits or a type error. The spec also leaves unstated whether a raw ESC byte inside a string literal is legal (the "strings any UTF-8" sentence implies yes), and whether an extern group needs `link` for libc.

#### choice_points

- How to emit ESC: through putchar via FFI (chosen). Another option was a raw 0x1B byte inside a string literal, which "strings any [UTF-8] but a raw carriage return or line end" seems to allow. That would allow `print("<ESC>[31mERROR<ESC>[0m")` but would leave an invisible byte in the source. Escapes like `\x1b` would not compile.
- Which C function to use: putchar (chosen). `printf` is variadic, and the spec gives no variadic extern form. `fputs` would need a `cstr` holding ESC, so it has the same problem.
- Mixing `print` with putchar: rejected. Using `print` for the plain text could reorder output if Heroes buffers stdout apart from C's stdio.
- Result type of putchar: `i32` (chosen). `i64` is also allowed ("a result may be wider").
- Reset code: `[0m` (chosen) resets all attributes. `[39m` resets only the foreground colour, and would also match "back to the terminal's default".
- Newline: written as `\n` inside the emitted string (chosen), or as `putchar(10)`. Both give the same output.
- The byte loop: index plus `to_i32(...).must()` (chosen). An array literal of `i32` codes would avoid the conversion but is less readable.

#### prediction

30 of 100 models produce a one-turn program that compiles and does what it should. Most failures will come from writing an escape the spec forbids, or from trying to build byte 27 with built-ins that cannot do it.

#### context

Only `brief.md` and `spec.md` from this directory were read. The system context also had harness and environment information and the user's account email, none of which bears on the program. My general knowledge of ANSI escape codes and C's stdio came from training, not from these files.

## Session a4

- **cost**: 0.2215 USD; **models**: claude-haiku-4-5-20251001, claude-opus-5-5; **ended**: `success`, 5 turns
- **scorer**: pass `True`, spelling *C through extern*, build exit 0, output `1b 5b 33 31 6d 45 52 52 4f 52 1b 5b 30 6d 0a`, *pass*

### `c.hero`

```
extern "stdio.h"
    function putchar(c: i32) -> i32

# Writes every byte of `s` through C's stdout.
function emit(s: str)
    for i in range(from: 0, to: s.len())
        _ = putchar(to_i32(s[i]).must())

function main()
    _ = putchar(27)
    emit("[31mERROR")
    _ = putchar(27)
    emit("[0m\n")
```

### `report.md`

# experiment

```
extern "stdio.h"
    function putchar(c: i32) -> i32

#### Writes every byte of `s` through C's stdout.
function emit(s: str)
    for i in range(from: 0, to: s.len())
        _ = putchar(to_i32(s[i]).must())

function main()
    _ = putchar(27)
    emit("[31mERROR")
    _ = putchar(27)
    emit("[0m\n")
```

#### reading

- "Six escapes, and no others: `\n` `\t` `\r` `\\` `\"` ... Any other escape is a compile error." No `\e`, `\x1b` or `\033`, so the ESC byte (27) cannot be written as an escape in a string. This sentence decided the whole design.
- Section 11 lists no built-in that turns an integer into a one-byte `str` (`to_str(27)` gives `"27"`). So nothing inside the language builds ESC.
- "Anything beyond this document ... comes from C libraries. A group names its header, and `link` a library when the symbols need one." This led to `extern "stdio.h"` with no `link`, since libc needs none.
- "A parameter ... declared at the header's own width and sign, `i32` where C says int." This gave `putchar(c: i32) -> i32`.
- "A literal takes the type its context asks for." So `putchar(27)` makes `27` an `i32`.
- "A line that computes a value must use it: ... discard it on purpose with `_ = f(x)`." This gave `_ = putchar(...)`.
- "`s[i]` yields a `u8`" and "`to_i8` ... `to_u64` give a `T?`". This gave `to_i32(s[i]).must()`.
- "`range(from: a, to: b) -> [i64]` exclude `to`" and "`for x in xs`, over an array or a `range`". This gave the loop in `emit`.
- I send every byte through `putchar` and never call `print`. The spec does not say whether `print` and C's stdio share one buffer, so mixing them could reorder the output.

#### confidence

Moderate. The syntax follows the grammar. Three things are unverified:
1. `s.len()` returning `i64` (implied, never stated).
2. clang accepting `putchar` against `stdio.h`. On some libcs it is also a macro, but a real function exists too.
3. C's stdout being flushed when `main` returns. This is standard if the runtime exits through C's normal exit path.

If those hold, the output is the bytes `ESC[31mERROR ESC[0m` followed by a newline.

#### argument

The specification did not make a one-turn program likely. The whole task depends on one byte, ESC, and the spec's closed escape list rules out every usual way to write it. It also gives no integer-to-character conversion. A model has to infer that the answer is FFI, then pick a C function and get its width right. It also has to guess whether `print` and C stdio interleave safely, which the spec never addresses. Many models will write `"\x1b[31m"` or `"\e[31m"` and get a compile error. The rest of the program (discarding results, `T?` conversion, `range`) is clearly specified. The FFI section is dense, but for a plain `int`-to-`int` function it is enough.

#### choice_points

- **How to produce ESC.** Choice: `putchar(27)` through FFI. Alternative 1: a raw 0x1B byte inside a string literal. "Syntax is ASCII-only" and strings forbid only CR and line end, so it might compile, and `print` would then work alone. But it is invisible in source and in this report, and the spec does not clearly allow it. Alternative 2: an invented escape like `\x1b`, which is a compile error.
- **Output path.** Choice: everything through `putchar`. Alternative: `print` for the text and `putchar` for ESC. That risks ESC appearing after the text if the two use separate buffers.
- **C function.** Choice: `putchar`. Alternatives: `write` or `fputs`. `write` needs a `ptr` from a `str`, which the spec gives no clear way to make, and `fputs` needs `stdout`, a C global the FFI cannot name. Both are likely refused.
- **Reset code.** Choice: `ESC[0m` (reset all attributes). Alternative: `ESC[39m` (default foreground only). Both meet "back to the terminal's default" for colour.
- **`link`.** Choice: omitted, because libc is linked anyway. Adding `link "c"` would probably also work.
- **Byte conversion.** Choice: `to_i32(...).must()`. Alternative: declaring `putchar(c: u8)`, which the width rule refuses.

#### prediction

25 out of 100 one-turn programs compile and do what they should. Most models will try an escape that the spec forbids, or mix `print` with C output.

#### context

Only this directory's `brief.md` and `spec.md` were read. The harness also supplied a system context (environment details and the user's account email), and none of it bears on the program. My general knowledge of ANSI codes and C's `putchar` was used as well.

## Session a5

- **cost**: 0.2242 USD; **models**: claude-haiku-4-5-20251001, claude-opus-5-5; **ended**: `success`, 5 turns
- **scorer**: pass `True`, spelling *C through extern*, build exit 0, output `1b 5b 33 31 6d 45 52 52 4f 52 1b 5b 30 6d 0a`, *pass*

### `c.hero`

```
# Prints ERROR in red, resets the colour, ends the line.
# No string escape writes ESC (27), so every byte goes through C's putchar.

extern "stdio.h"
    function putchar(c: i32) -> i32
    function fflush(stream: ptr) -> i32

function emit(s: str)
    for i in range(from: 0, to: s.len())
        _ = putchar(s[i].to_i32().must())

function main()
    _ = putchar(27)
    emit("[31mERROR")
    _ = putchar(27)
    emit("[0m\n")
    _ = fflush(nullptr)
```

### `report.md`

# experiment

```
#### Prints ERROR in red, resets the colour, ends the line.
#### No string escape writes ESC (27), so every byte goes through C's putchar.

extern "stdio.h"
    function putchar(c: i32) -> i32
    function fflush(stream: ptr) -> i32

function emit(s: str)
    for i in range(from: 0, to: s.len())
        _ = putchar(s[i].to_i32().must())

function main()
    _ = putchar(27)
    emit("[31mERROR")
    _ = putchar(27)
    emit("[0m\n")
    _ = fflush(nullptr)
```

#### reading

- "Six escapes, and no others ... Any other escape is a compile error" (section 2): no `\e`, `\x1b` or `\033`, so ESC cannot be written in a string. Nothing in section 11 turns an integer into a one-byte `str` (`to_str` writes decimal). So ESC must come from C.
- "Anything beyond this document ... comes from C libraries. A group names its header, and `link` a library when the symbols need one" (section 13): `extern "stdio.h"` with no `link`, since libc is linked anyway.
- "A parameter ... declared at the header's own width and sign: `i32` where C says int" and "a result may be wider than C's": `putchar(c: i32) -> i32`.
- "except ... what a `ptr` points at": `fflush(stream: ptr)` against `FILE *`, called with `nullptr` ("`nullptr` is the null of both") to flush all streams.
- "A character literal is an integer" and "A literal takes the type its context asks for": `putchar(27)` gives an `i32`.
- "`s[i]` yields a `u8`" and "`to_i8` ... `to_u64` give a `T?`": `s[i].to_i32().must()`.
- "`range(from: a, to: b) -> [i64]` exclude `to`", "`x.f(y)` is sugar for `f(x, y)`": the loop over `s.len()`.
- "A line that computes a value must use it: ... discard it on purpose with `_ = f(x)`": every `putchar`/`fflush` call is discarded with `_ =`.
- "the file you compile holds `function main()`": `main`.

#### confidence

Moderate. The language parts (loop, `_ =`, `.must()`, literal typing) follow the text closely. Risks: whether clang accepts `ptr` for `FILE *` (the spec says what a `ptr` points at is excepted, which I read as yes); whether the runtime flushes C stdio (I call `fflush` explicitly, so this is covered); whether `putchar` being a macro in some headers matters (it is also a real function in libc). I avoided `print` entirely so its output cannot interleave out of order with C's buffered output. If `s.len()` returns something other than `i64`, `range` would refuse it; the spec does not state `len`'s type.

#### argument

The spec made the language side easy but hid the crux: the only way to produce ESC is that "six escapes, and no others" rules it out, and no built-in converts a code to a character. A model must notice this, then reach for FFI, then reason about widths, `ptr` versus `FILE *`, and buffering between `print` and C stdio, none of which the spec addresses for this case. Many models will write `"\e[31m"` or `"\x1b[31m"` and fail to compile. The FFI section is dense and mostly about ownership, which is irrelevant here but costly to read. `len`'s result type is never stated.

#### choice_points

- How to get ESC: C `putchar` for every byte. Alternative: `print` for text plus `putchar(27)` for ESC, which risks out-of-order output if `print` does not share stdio's buffer; or `write(2)` with a `cstr lent` buffer, which may be refused against `const void *`.
- `putchar` parameter type: `i32`. Alternative `u8` might be accepted as "C converts exactly", or refused; `i64` would be refused.
- Result type: `i32`. `i64` is also allowed ("may be wider").
- Converting the byte: `to_i32().must()`. Declaring `c: u8` would avoid it if accepted.
- Flushing: explicit `fflush(nullptr)`. Omitting it relies on the runtime returning through C's `main`/`exit`; likely fine, but unstated.
- Reset sequence: `ESC[0m`. `ESC[39m` resets only the foreground; both meet "default colour".
- Newline: emitted via `putchar` as `\n` inside the string; `print("")` would add one but mixes streams.

#### prediction

30 of 100 models produce a one-turn program that compiles and does what it should. Most failures: an unsupported escape in a string literal.

#### context

Only `brief.md` and `spec.md` from this directory, plus the system/harness instructions of this session (tool descriptions, environment details, the user's account email which I did not use). No other files or sources.


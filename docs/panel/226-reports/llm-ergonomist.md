# Panel 226, the llm-ergonomist's report, run blind

**How it was run** (the coordinator, 2026-10-20): a fresh `claude -p` session from
`<scratchpad>/226-llm-ergonomist/`, outside every git tree, holding `heroes-spec.md` (the spec at
`a0a8d1249`), `brief.md`, three tasks and two feedback samples (`docs/panel/226-briefs/blind/`), with
`--model claude-opus-5-5 --restricted --safe-mode --strict-mcp-config --tools "Read,Write"
--allowedTools "Read,Write" --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage"
--max-budget-usd 4 --output-format json`. The session ran 24 turns, 218 s, 0.78 USD by the CLI's
report (`run.json`, session `8a97b7a7-72f7-4bbe-b481-f8d35daab3f1`), wrote `report.md` whole, its
`context` section last, and was then stopped by the account's session limit (*You've hit your session
limit, resets 4:30am Europe/Rome*, `is_error: true`); the report below is the file as it stood, 333
lines, copied without change. Its `context` says no instruction file, memory, rule or git status
reached it: the reading stands.

---

# Report: language ergonomics of Heroes, first try and repair

## verdict

**approve**

## experiment

### Task 1: word counts

```
constant LOWER: str
    "abcdefghijklmnopqrstuvwxyz"

function add_word(@counts: {str: i64}, w: str)
    counts[w] @ counts[w].default(0) + 1

function main()
    argv = args()
    if argv.len() != 1
        exit(code: 2)
    r = read_file(path: argv[0])
    if r.is_err()
        exit(code: 2)
    text = r.must()
    counts: {str: i64} @= {}
    word @= ""
    for ch in text.chars()
        b = ch[0]
        if b >= 'a' && b <= 'z'
            word @ word + ch
        else if b >= 'A' && b <= 'Z'
            idx = (b - 'A').to_i64().must()
            word @ word + LOWER.slice(from: idx, to: idx + 1)
        else if word != ""
            add_word(@counts, word)
            word @ ""
    if word != ""
        add_word(@counts, word)
    words = sort(keys(counts))
    seen: {i64: bool} @= {}
    for w in words
        seen[counts[w].must()] @ true
    levels = sort(keys(seen))
    i @= levels.len() - 1
    while i >= 0
        level = levels[i]
        for w in words
            if counts[w].must() == level
                print(w, " ", level)
        i @ i - 1
```

### Task 2: a ledger's balances

```
record Entry
    name: str
    amount: i64

function split_lines(text: str) -> [str]
    lines: [str] @= []
    start @= 0
    for i in range(from: 0, to: text.len())
        if text[i] == '\n'
            lines @ lines.push(text.slice(from: start, to: i))
            start @ i + 1
    if start < text.len()
        lines @ lines.push(text.slice(from: start, to: text.len()))
    return lines

function parse_int(s: str) -> i64?
    negative = s.len() > 0 && s[0] == '-'
    first @= 0
    if negative
        first @ 1
    if first == s.len()
        return fail(code: "bad_line", msg: "no digits")
    value @= 0
    for i in range(from: first, to: s.len())
        c = s[i]
        if c < '0' || c > '9'
            return fail(code: "bad_line", msg: "not a digit")
        d = (c - '0').to_i64().must()
        if negative
            value @ value * 10 - d
        else
            value @ value * 10 + d
    return ok(value)

function parse_line(line: str) -> Entry?
    space @= -1
    for i in range(from: 0, to: line.len())
        if line[i] == ' '
            if space >= 0
                return fail(code: "bad_line", msg: "more than one space")
            space @ i
    if space <= 0
        return fail(code: "bad_line", msg: "no name")
    amount = parse_int(line.slice(from: space + 1, to: line.len()))?
    return ok(Entry(name: line.slice(from: 0, to: space), amount: amount))

function main()
    argv = args()
    if argv.len() != 1
        exit(code: 1)
    r = read_file(path: argv[0])
    if r.is_err()
        exit(code: 1)
    lines = split_lines(r.must())
    totals: {str: i64} @= {}
    for n in range(from: 0, to: lines.len())
        match parse_line(lines[n])
            .ok e => totals[e.name] @ totals[e.name].default(0) + e.amount
            .err _ =>
                print(f"bad line {n + 1}")
                exit(code: 1)
    for name in sort(keys(totals))
        print(name, " ", totals[name].must())
```

### Task 3: bracket depth

```
function opener_of(c: u8) -> u8
    if c == ')'
        return '('
    if c == ']'
        return '['
    return '{'

function main()
    argv = args()
    if argv.len() != 1
        print("unbalanced")
        exit(code: 1)
    s = argv[0]
    stack: [u8] @= []
    deepest @= 0
    for i in range(from: 0, to: s.len())
        c = s[i]
        if c == '(' || c == '[' || c == '{'
            stack @ stack.push(c)
            if stack.len() > deepest
                deepest @ stack.len()
        else if c == ')' || c == ']' || c == '}'
            if stack.len() == 0 || stack[stack.len() - 1] != opener_of(c)
                print("unbalanced")
                exit(code: 1)
            stack @ stack.slice(from: 0, to: stack.len() - 1)
    if stack.len() != 0
        print("unbalanced")
        exit(code: 1)
    print(deepest)
```

### Repair 1 (`feedback/one`)

```
function total_of(xs: [i64]) -> i64
    total: i64 @= 0
    for x in xs
        total @ total + x
    return total

function main()
    print(total_of([1, 2, 3]))
```

**With `brief.txt` only:** same repair. The one-line form already has the error code, the position
and the "did you mean `total`?", which is the whole fix. What the full message added: the caret
(not needed), a note that `@` re-binds and `@=` declares (this only matters if the misspelling had
really been meant as a new cell, which here it was not), and "fix (certain)", which tells me I can
apply it without checking further. For this error the extra lines bought confidence and nothing
else.

### Repair 2 (`feedback/two`)

```
function main()
    n: u8 @= 0
    b: u8 = 255
    n @ b
    print(n)
```

**With `brief.txt` only:** probably a different repair. "expected `i64`, found `u8`" at 4:9 points
at `b`, so the obvious fix there is to convert `b`: `n @ b.to_i64().must()` (by §11 `to_i64`
returns a `T?`, so the `.must()` is required, and forgetting it would be a second refusal). That
version also compiles and prints `255`, so in this program both repairs pass. What the full message
added: (1) *why* `n` is `i64` (§2: a literal with no context defaults to `i64`, and the cell keeps
that type), (2) the place where the fix belongs, which is line 2 and not the line the error is on,
and (3) a fix labelled "guess", which is honest because a conversion would also be correct. Line 2
is the real gain: a model reading only the one-liner fixes the line it was pointed at. That is
usually fine but sometimes wrong, for example when the cell is later used as a `u8`.

## choice_points

### Task 1

1. **Wrong number of arguments** (not specified). I chose `exit(code: 2)`. Without the check,
   `argv[0]` on an empty `args()` aborts (§10). That compiles, but the exit status of an abort is
   not specified.
2. **Read failure.** `is_err()` then `exit`, then `.must()`. The other choice, a `match` whose
   `.err` arm is `exit(code: 2)`, may be refused: §8 says only a *jump* leaves a value
   unconstrained. `exit` "ends a path", but the spec does not say whether an arm whose body is a
   `()` call can sit beside a `str` arm. Writing `.must()` alone compiles but aborts instead of
   exiting 2.
3. **Turning a byte into a `str`.** The spec has no byte→`str` conversion. I walked `chars()` and
   lowercased by slicing a constant alphabet. Reaching for `to_str(b + 32)` compiles (any number
   prints) but produces `"97"` and not `"a"`. That compiles and does the wrong thing silently.
4. **`u8` → `i64` index.** `(b - 'A').to_i64().must()`. Leaving out `.must()` is refused by the §3
   "no implicit conversions" rule, since `to_i64` gives `i64?` (§11). Using the `u8` directly as
   `slice`'s `from:` is refused for the same reason.
5. **Character literals compared with `u8`.** I relied on §2 (a literal takes its context's type).
   An explicit `to_u8` would make the code longer and fallible, and still be correct.
6. **Non-ASCII bytes.** `chars()` yields a multi-byte `str` whose first byte is ≥ 0x80, so it counts
   as a separator, which is what "ASCII letters" means. Iterating bytes and slicing would also be
   correct.
7. **Sort order.** `sort` takes numbers, `str` or `bool` only and has no comparator (§11), and there
   are no anonymous functions (§9). I sorted the keys, collected the distinct counts, and walked
   them from largest down. Trying a comparator or sorting records is refused by the §11 rule
   ("never a type parameter"; no key argument). Sorting `"count word"` strings compiles but orders
   `10` before `9`.
8. **Map update in a helper.** `@counts` parameter. Inlining it twice is equivalent. A helper
   without `@` that returns nothing would be refused (§5: the computed map is unused) or would be
   a no-op on a copy (§3: values are copies).
9. **The `_ = ` rule** never came up, because every call here returns `()` or is bound.

### Task 2

1. **What a "name" is** (not specified). I chose a non-empty run of anything but a space. If names
   must be letters, `a1 5` or `é 5` would be bad lines. Such programs compile but disagree with
   mine on hidden tests.
2. **Trailing newline.** A final `\n` does not start a new line. If the empty piece after it were
   counted, every normally terminated file would print `bad line N+1`.
3. **Empty line in the middle.** Bad line (no space). Skipping it compiles and gives different
   output.
4. **CRLF.** `\r` makes the number bad, so the line is reported. Stripping it compiles and accepts
   the line.
5. **`+5`, `-`, `--5`, `007`.** Reject, reject, reject, accept. Each is a guess, and each
   alternative compiles with different output.
6. **Overflow** of a number or a total aborts (§7). Treating it as a bad line instead would need
   explicit range checks, which the task never mentions.
7. **Unreadable file / no argument** (not specified). Exit 1 and print nothing.
8. **Splitting lines.** There is no `split`, so I scan bytes and `slice`. The spec never says
   whether `slice` works on `str`, but §10 ("a slice that splits a character") implies it does.
9. **`range(from:, to:)` named arguments.** Required by §9 (two `i64` parameters). `range(0, n)`
   is refused.
10. **`match` arm with a block body ending in `exit`.** It is used as a statement, so the arm's
    type does not matter. If it were used as a value, see Task 1 point 2.
11. **Bad line on a later line after good ones.** Nothing is printed before validation finishes,
    because totals print at the end. Printing as I go would compile and break "print nothing else".

### Task 3

1. **No argument** (not specified). Print `unbalanced`, exit 1. Leaving it out aborts on `argv[0]`.
2. **Stack pop.** There is no `pop`, so `slice(from: 0, to: len - 1)`. A model that invents
   `stack.pop()` is refused (unknown name). One that tracks only a depth counter compiles but
   accepts `(]`.
3. **Bytes or `chars()`.** Bytes. Both are correct, because UTF-8 continuation bytes are never
   brackets.
4. **`opener_of` written with `if` chains, not `match` on `u8`.** §8 allows `_` on `i64`/`str` but
   says nothing about `u8` patterns. Using `match` might be refused or might compile. The spec does
   not say.
5. **Depth = stack length after a push.** Counting before the push would print `1` for the example.

### Prose and examples

- **Task 1.** No sentence names a construct. The nearest are "receives one argument" (→ `args()`)
  and "exit with status 2" (→ `exit(code:)`). The example made no difference to my code. It
  confirmed that `.` is a separator and that ties sort by word.
- **Task 2.** The same two sentences, plus "print `bad line N`" (→ string formatting, nothing
  specific). The example made no difference to my code. It does not show a trailing newline or a
  bad line, which are the two places tests would catch you out.
- **Task 3.** "The program receives one argument: a string" (→ `args()[0]`, *not* a file). The
  example made no difference to my code. It confirmed that the outermost level counts as depth 1
  and that a mismatched kind is unbalanced.

## argument

Most refusing rules are local, and the messages name the fix. The one-line form suffices for
typos; the full form matters when the fix lies away from the error's position (repair 2). Five
turns of compiler messages measure exactly that. The risk is in the tasks, not the protocol: many
failures I expect would compile and fail hidden tests on behaviour the prose leaves open
(arguments, CRLF, trailing newline, what a name is, overflow). Those measure guessing, not the
language. Missing built-ins (`split`, `pop`, byte→`str`, a sort comparator) will cost more than any
single syntax rule, and should not be blamed on the messages.

## prediction

| Task | First attempts that compile | First attempts that pass hidden tests |
|---|---|---|
| 1 word counts | 45% | 30% |
| 2 ledger | 50% | 30% |
| 3 bracket depth | 70% | 60% |

The three rules I expect models to break most often, in order:

1. **No implicit conversions, widths included (§3), together with `to_<int>` giving a `T?` (§11)**:
   `u8` from `s[i]` against `i64`, a missing `.must()` after `to_i64`, and literal defaults (§2).
2. **Unused bindings, cells nothing re-binds, and a value-returning line that must be bound (§5)**,
   including the `_` that may not drop a `T?`.
3. **Mandatory named arguments when two parameters share a type (§9)**: `range(0, n)`,
   `slice(a, b)`, and user functions with two parameters of one type.

**(a)** Turn two should get the full form. It costs a few lines, and its note and its remote fix
location ("at line 2:6") are the only parts that teach a rule or move the repair to the right
line. The label certain/guess is useful calibration.

**(b)** Re-run it and hand back the result. The model already holds that input and its expected
output, so returning "expected X, got Y" leaks nothing hidden. It does change the quantity being
measured (repair with a test oracle, not only with compiler messages), so it should be a fixed,
reported part of the protocol, the same across every language compared. Results of hidden tests
must never be returned.

## condition

I would change the verdict to **object** if, in the measured runs, most first attempts that compile
but fail hidden tests do so on behaviour the task prose leaves open (the choice points above marked
"not specified", CRLF, trailing newline) and not on the language. I would also object if the
pass-rate difference between the full and one-line message arms turns out smaller than the
run-to-run noise, because then the protocol cannot detect the effect it claims to measure.

## context

Besides the files in this directory, my context held: the agent's system instructions; environment
details (working directory, "not a git repository", platform and OS version, date); and an
automatically attached account email address for the user (not repeated here). No instruction file
such as CLAUDE.md, no memory, no project rule and no git status reached me. To find the feedback
programs I had to guess paths under `feedback/` (`01`, `1`, `a`, `A`, `case-1`, `task-01`,
`README.md` did not exist; `one` and `two` did). All the paths I tried were inside this directory,
and I ran nothing.

# Panel 156 — llm-ergonomist brief

You judge the objective: comprehension and locality. **Your verdict is an
experiment, not an opinion.**

**Read only `spec/heroes-spec.md` and this file.** Do not read design.md, the
repository, the runtime, the other briefs or the shared brief. Your value here
is that you are the only seat judging what a reader of the language actually
gets, and reading anything else destroys it.

## The situation, stated without the repository

A Heroes program calls a C library. It hands that library a handle that holds
`nullptr`, and the library reads through it. The program dies.

Here is the program. `node_open` and `node_value` come from a C header; `Node`
is the handle type.

```
extern "node.h"
    record Node tag node
    function node_open(v: i64) -> Node
    function node_value(p: Node) -> i64

function main()
    real = node_open(v: 7)
    print(node_value(p: real))
    empty: Node @ nullptr
    print(node_value(p: empty))
```

Today the same program, built the same way, behaves three different ways
depending on the machine. The three are labelled A, B and C. **Which machine is
which is deliberately withheld from you.**

| | what the program writes to its output | what it writes to its error stream |
|---|---|---|
| **A** | `7` | `panic: a null pointer was read through — a handle or ptr holding nullptr reached C where C dereferences it, at offset 0x0, called from node_value` |
| **B** | nothing | `panic: a null pointer was read through — a handle or ptr holding nullptr reached C where C dereferences it, at offset 0x0, called from main.main` |
| **C** | nothing | nothing |

All three end with a non-zero exit status.

`node_value` is the C function the program called. `main.main` is the Heroes
function that called it — the module `main`, the function `main`.

## Four tasks. Do them before forming a verdict

1. **Find the sentence.** Read `spec/heroes-spec.md` and find what it promises
   about a program that stops this way. Quote it. Then say, for each of A, B and
   C, whether that promise is kept. If the document promises nothing, say so
   plainly — a silence is a finding.

2. **The blind repair.** You are the model that wrote this program. You are
   handed **A**'s error line and nothing else. Write your next action: what do
   you change, or what do you look at? Then do the same handed **B**'s line.
   Report which of the two got you to the defect faster and why, in your own
   words, including where you guessed.

3. **The lost line.** In A the program's own output, `7`, survives; in B it does
   not. Say whether that matters to you as a reader, and construct a case where
   it changes what you would conclude about the program. Then say whether the
   specification licenses either behaviour.

4. **Name the missing one.** Without being told which machine is which: is any
   of A, B, C a behaviour you would call a defect of the language rather than of
   the program? Rank them best to worst for someone debugging, and say what the
   ideal line would say if you could write it yourself.

## What your verdict is asked to answer

- Which blame name serves a reader better, the C function that faulted or the
  Heroes function that called it — and is the answer different for a human than
  for a model?
- Is a program that says nothing when it dies acceptable under the language as
  specified?
- Should the specification say more than it does about what a stopping program
  writes? If yes, propose the sentence and keep it short — every word is paid
  for.
- You hold a veto on non-local constructs. Say plainly whether you are casting
  it and on what.

Your verdict owes a falsifiable prediction and the condition under which you
would change your vote.

Write your report to `docs/panel/156-reports/llm-ergonomist.md`.

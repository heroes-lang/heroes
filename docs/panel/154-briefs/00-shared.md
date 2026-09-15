# Panel 154 — shared brief

**Convened** 2026-09-15 on `docs/work/DEFECTS.md` 045, without asking, under
CLAUDE.md § 4's *ask once per milestone then convene again*: panels 150 to 153
convened under the same clause today. **Full panel**, five seats plus the
completeness critic. The defect was filed by panel 153's completeness critic as
the measured cost of a route it found, and it got worse when its first repair
was attacked at the shapes beside it.

## The question

> **A Heroes program hands a handle holding `nullptr` to a C function that
> dereferences it. What stops it?**

## What is true at HEAD, every line a command that was run

The shape, three lines of a header of the author's own and six of Heroes:

```c
struct node { int64_t value; struct node *next; };
static inline struct node *node_find(int64_t key) { … return NULL; }
static inline int64_t node_value(struct node *p) { return p->value; }
```
```
extern "node.h"
    record Node tag node
    function node_find(key: i64) -> Node borrows
    function node_value(p: Node) -> i64
```

| level | what the program does, measured on this Mac |
|---|---|
| `-O0` | `panic: a null pointer was read through …, called from node_value`, **exit 134** |
| `-O2` | **prints a value for a node that does not exist, exit 0**, both streams otherwise empty |
| `--sanitize` | UBSan `runtime error: member access within null pointer`, exit 0 |

**The `-O0` half is already repaired and it is the smaller half.**
`runtime/parts/stack.c` turns a read through the null page into a named abort:
the witness is the address, measured — a read through null lands in the first
page because a field's offset is smaller than a page in every struct a header
can lay out, and the null page is unmappable on all three platforms.

**The `-O2` half is the defect and no runtime guard can ever reach it.** Clang is
entitled to assume a dereferenced pointer is non-null, so it folds the fault
away entirely and there is no signal to catch. **A wrong answer at exit 0** is
what `docs/work/DEFECTS.md` exists for, and CLAUDE.md § Precedence rank 3 puts
robustness above elegance, tokens, ergonomics, compiler size and speed.

**So the repair has to be a check BEFORE the call.** That is exactly what
`guard_cstr_arguments` already does for every `cstr` argument on its way out,
and the asymmetry between `cstr` and a handle is the whole subject of this
sitting.

**The thing that makes it hard, and it is the same sentence defect 013 settled
for a different argument**: `freeaddrinfo(NULL)`, `sqlite3_close(NULL)` and
`free(NULL)` are legal C, and real programs call them. Whether NULL is legal is
a property of the C FUNCTION, not of the type, and no header states it.
`runtime/parts/stack.c` carries defect 013's paragraph on exactly this, about a
null FUNCTION pointer: `sqlite3_exec(callback: nullptr)` is correct and
`atexit(nullptr)` is not.

## Five candidate resolutions, so a seat can refuse all five and name a sixth

- **A — guard every handle argument**, as `cstr` is guarded: no surface, one
  abort site. It refuses `freeaddrinfo(NULL)` at runtime, and that is the cost
  to price rather than to assume.
- **B — a word saying NULL is legal here**, guard by default and opt out:
  `freeaddrinfo(ai: AI consumes accepts_null)`. Safe by default (§1.12), a word
  on the rarer case.
- **C — a word saying NULL is not legal**, no guard by default and opt in.
  Cheaper in words written, unsafe by default.
- **D — no new word: the marks already there decide.** A parameter marked
  `consumes` is a release and releases usually accept NULL; an unmarked handle
  parameter is a use and uses usually do not. No surface, and it rests on a
  premise about C's conventions that a seat should try to falsify against real
  headers.
- **E — refuse at compile time where the value is statically `nullptr`**, a
  diagnostic and no runtime cost, closing the witness and not the class. The
  checker has no flow analysis (`check/leasing.hero:29`, `check/consuming.hero:22`).

## Rules that bind you

Build in a COPY — `cp -r` the tree to your scratch directory, `rm -rf build`,
work there. The seed builds in about 3 s: `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`. **Never rebuild the compiler from `selfhost/`**
in a cold copy. `HEROES_RUNTIME=<copy>/runtime` to build a program elsewhere.
`.env` carries `ANTHROPIC_API_KEY`: `set -a; . ./.env; set +a`. **The working
tree is frozen from this brief to the synthesis** — and it was not at panel 153,
which the critic caught and which is not to happen twice.

**A claim enters your report only after the command that settles it has been
run.** Where you could not run it, say so in your own words. A negative claim
names what you searched for. A number carries its unit in the same sentence.

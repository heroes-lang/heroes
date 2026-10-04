# Defect 109 closed: an empty file is a program with nothing in it, and a file's end is the end of what the parser reads there

2026-09-27, M-agreed-retention step 22, in lane A (`7bcd7cc2`), merged `f08b192d`.
Found by lane 105's agent; repaired by lane A's agent, and the file-end
shapes beside it found by an audit of the degenerate inputs run in the
same lane.

- [x] **109 — every verb that reads a program panics on an empty file** | `heroes check`, `lex`, `parse` and `build` on a file of zero bytes exit 134 with `panic: string index out of range`: `source.from_files` reads the last byte of the first file, which has none; `fmt` exits 0 on the same file | `selfhost/source.hero:109` (`from_files`) · **closed 2026-09-27**

    **Origin:** lane 105's agent, 2026-09-27, met it while repairing
    `from_files` and kept it unchanged, the lane being a refactor; re-run by
    the coordinator the same day on the trunk's compiler at `2b1a1f24`:
    `check`, `lex`, `parse` and `build` exit 134, `fmt` exit 0.

    **Why it is a defect.** The compiler must not crash on an input
    (design.md §1.12); an empty file is a program with no `main`, and owes
    the diagnostic any file without one gets, or a clean exit where the verb
    has nothing to say.

## What it was

`from_files` asks whether the text so far ends inside a line before it
joins the next file, and it asked by reading the last byte of the last
file that had one. A first file of zero bytes had none, so the read was
`""[-1]`, before the library was joined and before a token was lexed.

## The degenerate files, every verb, before and after

Every verb and form the lane's sweep (`d109/sweep.sh`) ran, 21 per file,
on 16 degenerate files: zero bytes, only spaces, only newlines, CRLF alone,
a comment with and without its final newline, a `use` alone, a UTF-8 BOM
with and without a newline, one token, a `main` with no final newline, an
empty module imported by `use`, a module of blank lines and one of a
comment imported the same way. `check` in six forms, `lex` in four,
`parse` in two, `fmt` and `fmt --in-place`, `build` in three, `run`,
`test`, `measure`, `mutate`.

| | runs | exit 134 before | exit 134 after |
|---|---|---|---|
| the empty file | 21 | 17 | 0 |
| an empty module imported | 21 | 17 | 0 |
| the fourteen others | 294 | 0 | 0 |

After the repair the empty file answers as a file of spaces does: `check`
0, `build` and `run` `no_entry_point` at exit 1, `test` *no tests*. The 15
runs at exit 2, `mutate` on every file, are the same before and after:
defect 117's.

## What stood beside it

**A file's end was not the end of what the parser read there.** Every file
is lexed as its own text, and each file's end-of-file token was then
dropped so that one stream ran from the author's file through every module
into the library. A file that ended inside a declaration went on into the
next file's tokens, measured on the trunk's compiler:

| program, the trunk's compiler before, this lane's after | before | after |
|---|---|---|
| `a.hero` ends with `function two() -> i64` and no body; `b.hero` begins with an indented `return 2`; `main` prints `a.one() + b.two()` | exit **0**, prints 3: `a.two` took `b`'s first line as its body | `missing_body` in `a.hero`, `expected_declaration` in `b.hero`, exit 1 |
| a module that leaves a declaration open before the next module's declaration | `missing_body` | `missing_body` |

The two files are each refused alone on both compilers; only joined did the
trunk run them.

**An empty artifact printed one newline.** `io.print_artifact("")` printed
`print("")`, which is `\n`: `heroes fmt` on an empty file, or on one of
spaces or blank lines, wrote one byte to stdout where `--in-place` writes
none, and `build --dump-ir` of an empty program the same.

**The audit of the empty-container reads** found no second site of the
shape: 69 reads of a last element (`len() - 1`) under `selfhost/`,
listed in the lane's scratch (`audit/all_lenm1.txt`).

## The repair

- `from_files` keeps a boolean, whether the text so far ends inside a
  line, set where a file with bytes is appended and cleared by the joiner.
  An empty file ends no line.
- `lexer.lex_files` gives each file's own stream, ending in its own eof;
  `lexer.joined` makes the one stream every other reader gets, as `lex`
  did. `parse.parse_source` parses each file with its own cursor, into one
  tree, one list of declarations and one list of uses, in load order. The
  cursor's bound on the author's text is the one it had.
- `io.print_artifact` prints nothing for an empty artifact.

## The pins

Seven compiler tests: *a first file with no byte in it ends no line, so the
joiner alone follows it*; *an empty file between two others is joined like
any other*; *files that are all empty are only their joiners*; *an empty
file parses to nothing, before the library and between two modules*; *a
declaration a file leaves open is refused in that file, and the library's is
not read*; *a module's first indented lines are not the body of the module
before it*; and seventeen `surface` rows over `surface-fixtures/empty109/` and
`fileend109/`: every verb on a file of zero bytes, `fmt` and `--dump-ir`
printing zero bytes where they printed a newline, a module of zero bytes a
program imports, `mutate` over a directory holding one, and the file-end
shapes, the pair of modules that ran and printed 6 among them.

## What it does not change

`heroes mutate <file>` answers *cannot read <file>* at exit 2 for a file
it reads: it takes a directory, and the message says something false. The
same before and after, and filed as defect 117.

## The measurements

The lane's gate and platforms are its commit's body (`7bcd7cc2`): the
compiler's 778 tests, the net's own 172, every suite 0 failed, Linux arm64
and x86-64 and the Windows box at run 206, 206 and 204 with 0 failed. On
the trunk after the merge (`f08b192d`): the seed emitted again by the
trunk's compiler, fixpoint by `cmp`; the compiler's 782 tests, the net's
own 172; the full net 2741 passed and 1 failed, the annotations floor the
two lanes moved together, raised in the merge, annotations 199 and 0.

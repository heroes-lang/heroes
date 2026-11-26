# rv32

rv32: a RISC-V RV32IM emulator, with the two-pass assembler that makes
its programs, a disassembler and a debugger. The machine is one hart in
machine mode: the base integer instructions and the M extension, with
their exact 32-bit wrap-around, shifts read modulo 32 and division by zero
as the specification defines it; the CSRs of the counters and of trap
handling, traps for `ecall`, `ebreak`, illegal instructions and faulting
accesses, and `mret`; a bus that checks every address, with RAM at
0x8000_0000, a 16550 UART at 0x1000_0000 and a CLINT timer at
0x0200_0000, as QEMU's `virt` machine places them. The assembler reads GNU
as's syntax for RISC-V, its labels, directives and pseudo-instructions,
and writes a flat binary image with a small header, a checksum and the
symbols; a raw flat binary loads too. A program with no trap handler calls
`exit`, `read` and `write` by `ecall` with Linux's numbers.

Run for real, on the samples of `samples/` beside this file or a program
of one's own:

    rv32 asm primes.s                  # writes primes.img beside it
    rv32 run primes.img                # runs it; a .s is assembled first
    rv32 run strings.s --input 'hi\n'  # what its UART receives
    rv32 run fib.s --limit 1000        # at most 1000 instructions
    rv32 debug fib.img                 # the debugger, commands on stdin
    rv32 disasm fib.img                # the image as assembly

`rv32 demo <folder>` assembles the samples of the folder (primes, a sort,
a recursive Fibonacci on the stack, a string routine, the M extension's
edge cases, a timer interrupt, a crash), writes their images under
`build/rv32-demo/`, shows a source with mistakes and a damaged image
refused, reads every image back and runs it, lists one, and debugs one
under the commands of the standard input.

The packages of Rebel it uses, and what for:

- `scanner`: the assembler's tokens;
- `flags`: the command line;
- `math`: the 64-bit wrapping product `mulhu` needs;
- `numbers`, `strings`: numbers read and written in any base, text cut
  and padded;
- `bytes`, `hash`: the image's little-endian fields and its CRC-32, a
  memory dump;
- `containers/ordered`: the symbols by address and the breakpoints;
- `containers/deque`: the debugger's trail of the last instructions;
- `lists`: the symbols sorted, the debugger's lines joined;
- `fs`, `path`, `io`: images written and read, the standard streams;
- `errors`: failures told with their file and line.

## Its demo

Run from the repository's root with the words of `main.args` and `main.stdin` on its input, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run rebel/examples/rv32/main.hero -- demo rebel/examples/rv32/samples < rebel/examples/rv32/main.stdin

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 14 faces on its `packages` line, each linked here to its reference: [`bytes`](https://heroes-lang.org/rebel/bytes/), [`containers/deque`](https://heroes-lang.org/rebel/containers/deque/), [`containers/ordered`](https://heroes-lang.org/rebel/containers/ordered/), [`errors`](https://heroes-lang.org/rebel/errors/), [`flags`](https://heroes-lang.org/rebel/flags/), [`fs`](https://heroes-lang.org/rebel/fs/), [`hash`](https://heroes-lang.org/rebel/hash/), [`io`](https://heroes-lang.org/rebel/io/), [`lists`](https://heroes-lang.org/rebel/lists/), [`math`](https://heroes-lang.org/rebel/math/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`path`](https://heroes-lang.org/rebel/path/), [`scanner`](https://heroes-lang.org/rebel/scanner/), [`strings`](https://heroes-lang.org/rebel/strings/).

## Its folders

12 folders, 42 files and 6,708 lines, this page aside; of them 30 modules, `.hero` files, of 5,687 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 5 | 324 | 1 | 109 |
| `app/` | 1 | 195 | 1 | 195 |
| `asm/` | 8 | 1,418 | 8 | 1,418 |
| `cli/` | 1 | 189 | 1 | 189 |
| `cpu/` | 8 | 1,291 | 8 | 1,291 |
| `debug/` | 2 | 541 | 2 | 541 |
| `demo/` | 1 | 106 | 1 | 106 |
| `disasm/` | 1 | 191 | 1 | 191 |
| `image/` | 1 | 216 | 1 | 216 |
| `isa/` | 5 | 1,294 | 5 | 1,294 |
| `run/` | 1 | 137 | 1 | 137 |
| `samples/` | 8 | 806 | 0 | 0 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.

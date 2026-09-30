#!/usr/bin/env python3
"""Nesting shapes for panel 184's brief: each shape at a depth n, one file each.
Usage: gen.py <outdir> <n>..."""
import os, sys
out = sys.argv[1]
def body(lines):
    return "function main()\n" + "".join("    " + l + "\n" for l in lines)
def shapes(n):
    yield "paren-closed", body([f"x = {'(' * n}1{')' * n}", "print(x)"])
    yield "paren-open", body([f"x = {'(' * n}1", "print(x)"])
    yield "list-closed", body([f"x = {'[' * n}1{']' * n}", "print(len(x))"])
    yield "call-closed", ("function g(v: i64) -> i64\n    return v\n\n" + body([f"x = {'g(' * n}1{')' * n}", "print(x)"]))
    yield "unary-minus", body(["x = " + "- " * n + "1", "print(x)"])
    yield "not-chain", body(["b = " + "!" * n + "true", "print(b)"])
    yield "binary-chain", body([f"x = 1{' + 1' * n}", "print(x)"])
    lines = []
    for i in range(n):
        lines.append("    " * i + "if true")
    lines.append("    " * n + "print(1)")
    yield "if-nested", body(lines)
    yield "fstring-nested", body([f"s = {'f\"{' * n}1{'}\"' * n}", "print(s)"])
    lines = ["x = 1"]
    for i in range(n):
        lines.append(("if x == 0" if i == 0 else f"else if x == {i}"))
        lines.append(f"    print({i})")
    lines.append("else")
    lines.append("    print(0)")
    yield "else-if-chain", body(lines)
for n in map(int, sys.argv[2:]):
    for name, text in shapes(n):
        with open(os.path.join(out, f"{name}-{n}.hero"), "w") as f:
            f.write(text)

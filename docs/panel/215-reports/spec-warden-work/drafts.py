"""Panel 215, spec-warden: every candidate sentence as an exact edit of the spec.

Run: python3 -I drafts.py <base.md> <outdir>. Each draft is a list of
(old, new) pairs; each `old` must occur exactly once in the base, or the
script stops. Writes <outdir>/<name>.md for each draft.
"""
import os
import sys

R1_SENT = ("End a construction with `rest: zero` and every field\n"
           "sharing no byte with one it names is zero; only a group's record has it. A\n")
R1_UNION = "is built naming exactly one, or none when it ends with `rest: zero`, an\n"
K_OLD = "width and sign — `i32` where C says int, `i8` where it says char, `u64` where it\n"
GRAMMAR_REC = '           | "record" ident [ "tag" ident ] [ "partial" ] ( Fields | NEWLINE ) .\n'
AFTER_KEY = ("key are compile errors, for it and for any value holding it. One\n")
AT_PARAM = ("  `function advance(@l: Lex)` … `advance(@l)`. Semantics: copy in, copy\n"
            "  out (copy-out always happens, including on early return and `?`). UFCS does\n")

DRAFTS = {
    # ---- Q1: padding ----
    "q1-p-short": [(R1_SENT, R1_SENT.replace("only a group's record has it. A\n",
                                             "only a group's record has it.\nIts padding reaches C zero. A\n"))],
    "q1-p-long": [(R1_SENT, R1_SENT.replace("only a group's record has it. A\n",
                                            "only a group's record has it.\nThe bytes between and after a group's record's fields reach C zero, however\nthe record got there. A\n"))],
    "q1-e-field": [(R1_SENT, R1_SENT.replace("only a group's record has it. A\n",
                                             "only a group's record has it.\nA field named `_` is C's padding, as wide as the header leaves it: zero, and\nnever read or written. A\n"))],
    # ---- Q2: a record C forbids to copy ----
    # B: a mark the binding writes; refusal sentence + grammar word
    "q2-b-mark": [
        (AFTER_KEY, AFTER_KEY.replace(" One\n",
                                      "\n`record Lock tag lock pinned` is a place and never a value: `=`, a return,\nan argument without `@`, and holding it in an array or another record are\ncompile errors. One\n")),
        (GRAMMAR_REC, GRAMMAR_REC.replace('[ "partial" ]', '[ "partial" ] [ "pinned" ]')),
    ],
    # B with the clause saying `@` hands the place itself (spec § 9 says copy in, copy out)
    "q2-b-mark-place": [
        (AFTER_KEY, AFTER_KEY.replace(" One\n",
                                      "\n`record Lock tag lock pinned` is a place and never a value: `=`, a return,\nan argument without `@`, and holding it in an array or another record are\ncompile errors, and `@` hands a function the place itself. One\n")),
        (GRAMMAR_REC, GRAMMAR_REC.replace('[ "partial" ]', '[ "partial" ] [ "pinned" ]')),
    ],
    # C: the header decides, no word
    "q2-c-header": [
        (AFTER_KEY, AFTER_KEY.replace(" One\n",
                                      "\nA record whose C struct holds a lock or an atomic, `pthread_mutex_t` or\n`_Atomic`, is a place and never a value: `=`, a return, an argument without\n`@`, and holding it in an array or another record are compile errors. One\n")),
    ],
    # D: a check at run time
    "q2-d-runtime": [
        (AFTER_KEY, AFTER_KEY.replace(" One\n",
                                      "\nCopying a record whose C struct holds a lock or an atomic aborts. One\n")),
    ],
    # ---- Q3: R1 and K ----
    "q3-r1-out": [
        (R1_SENT, "A\n"),
        (R1_UNION, "is built naming exactly one, an\n"),
    ],
    "q3-k-out": [(K_OLD, "width and sign — `i32` where C says int, `u64` where it\n")],
    "q3-r1k-out": [
        (R1_SENT, "A\n"),
        (R1_UNION, "is built naming exactly one, an\n"),
        (K_OLD, "width and sign — `i32` where C says int, `u64` where it\n"),
    ],
    # shortened: the restriction folded into the subject
    "q3-r1-short-a": [(R1_SENT, ("End a group's construction with `rest: zero` and every field\n"
                                 "sharing no byte with one it names is zero. A\n"))],
    # shortened: drop the restriction (the checker refuses it elsewhere)
    "q3-r1-short-b": [(R1_SENT, ("End a construction with `rest: zero` and every field\n"
                                 "sharing no byte with one it names is zero. A\n"))],
    # shortened: verb-first
    "q3-r1-short-c": [(R1_SENT, ("`rest: zero` ends a group's construction, zeroing every field\n"
                                 "sharing no byte with one it names. A\n"))],
    # ---- beside Q2: the spec's own `@` sentence made true of the emitted C ----
    "q2-at-place": [(AT_PARAM, ("  `function advance(@l: Lex)` … `advance(@l)`. Semantics: the callee works on\n"
                                "  the caller's place itself. UFCS does\n"))],
}


def main():
    base_path, outdir = sys.argv[1], sys.argv[2]
    base = open(base_path, encoding="utf-8").read()
    os.makedirs(outdir, exist_ok=True)
    for name, edits in DRAFTS.items():
        text = base
        for old, new in edits:
            n = text.count(old)
            if n != 1:
                sys.exit(f"{name}: anchor occurs {n} times: {old[:60]!r}")
            text = text.replace(old, new)
        if text == base:
            sys.exit(f"{name}: no change")
        with open(os.path.join(outdir, name + ".md"), "w", encoding="utf-8") as f:
            f.write(text)
        print(name)


if __name__ == "__main__":
    main()

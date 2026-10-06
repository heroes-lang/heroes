#!/usr/bin/env python3
"""Panel 196, spec-warden: each route's section 13 draft, applied to the pristine
frozen spec. `drafts.py <name> <out>` writes the drafted spec to <out>;
`drafts.py --list` names them; `drafts.py --show <name>` prints the new text.
Every anchor must match exactly once, or the script stops."""
import sys

PRISTINE = "/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/196-spec-warden/w/pristine-spec.md"

OUT_OLD = ("A C out-parameter is an `@` parameter, and what it\n"
           "points at is held to the same width and sign — `@n: u64` where it says\n"
           "`size_t *`:\n")
# U: what an unmarked `@` lend promises, merged into the sentence that already
# defines the out-parameter (panel 122: merging beats appending).
OUT_U = ("A C out-parameter is an `@` parameter, and what it\n"
         "points at is ONE element, held to the same width and sign — `@n: u64` where it\n"
         "says `size_t *`:\n")
# U5: the same promise where S5 reads the header's own extent.
OUT_U5 = ("A C out-parameter is an `@` parameter, and what it\n"
          "points at is ONE element, held to the same width and sign — `@n: u64` where it\n"
          "says `size_t *`, and refused where the header says C writes more:\n")
# U3: S3 widened, every single-cell lend to a typed pointer says its count.
OUT_U3 = ("A C out-parameter is an `@` parameter, and what it\n"
          "points at is held to the same width and sign — `@n: u64 counted_by 1` where it\n"
          "says `size_t *`; a number or record lent to a pointer without its count is refused:\n")

FENCE_END = "    function sqlite3_close(db: Db consumes) -> i64\n```\n"

S6_AFTER = ("C writing more than one through an `@` parameter overwrites what lies beside it,\n"
            "so a buffer C fills is a group record holding it, lent whole to a function your\n"
            "own header declares taking its struct.\n")
S1_AFTER = ("Where C writes several and no argument says how many, declare the fixed array it\n"
            "fills, `@md: u8[32]`, and lend a group record's field of exactly that type: `@d.b`.\n")
S7_AFTER = ("Where C writes several and no argument says how many, `@md: [u8] counted_by N`,\n"
            "`N` a constant of the group, hands C `N` elements, the array's own then zeros,\n"
            "and leaves the array exactly those `N`.\n")
S4_AFTER = ("C writing past an `@` cell aborts when the call returns, where it wrote within\n"
            "the cell's guard; further than that is not seen.\n")

PTR_OLD = ("`f.ptr()` lends a binding's field to a `ptr` parameter declared `counted_by n`,\n"
           "naming the sibling that gives the extent, and one past the field is refused.\n")
PTR_S2 = ("`f.ptr()` lends a binding's field to a `ptr` parameter declared `counted_by n`,\n"
          "naming the sibling C reads the extent from or a constant of the group holding it,\n"
          "and one past the field is refused.\n")
# S2 with a literal too: the grammar moves.
PTR_S2L = ("`f.ptr()` lends a binding's field to a `ptr` parameter declared `counted_by n`,\n"
           "naming the sibling C reads the extent from, a constant of the group or a number,\n"
           "and one past the field is refused.\n")

CPARAM_OLD = '    CParam = [ "@" ] ident ":" Type [ "counted_by" ident ] [ "lent" ]\n'
CPARAM_INT = '    CParam = [ "@" ] ident ":" Type [ "counted_by" ( ident | integer ) ] [ "lent" ]\n'

DRAFTS = {
    # the sentence alone: what an unmarked `@` lend promises
    "U": [(OUT_OLD, OUT_U)],
    # S6: the promise, and what a buffer is bound as today
    "S6": [(OUT_OLD, OUT_U), (FENCE_END, FENCE_END + S6_AFTER)],
    # S1: a fixed-array out-parameter
    "S1": [(OUT_OLD, OUT_U), (FENCE_END, FENCE_END + S1_AFTER)],
    # S2: counted_by a constant (no grammar change), and the out-count twin named
    "S2": [(OUT_OLD, OUT_U), (PTR_OLD, PTR_S2)],
    # S2 with the literal spelling: CParam moves
    "S2L": [(OUT_OLD, OUT_U), (PTR_OLD, PTR_S2L), (CPARAM_OLD, CPARAM_INT)],
    # S7: an emitter-owned buffer, copy in and copy out, its extent a constant
    "S7": [(OUT_OLD, OUT_U), (FENCE_END, FENCE_END + S7_AFTER)],
    # S3 widened: every single-cell lend to a typed pointer states its count
    "S3": [(OUT_OLD, OUT_U3), (PTR_OLD, PTR_S2L), (CPARAM_OLD, CPARAM_INT)],
    # S5: the header's own extent read and checked
    "S5": [(OUT_OLD, OUT_U5)],
    # S4: a run-time guard
    "S4": [(OUT_OLD, OUT_U), (FENCE_END, FENCE_END + S4_AFTER)],
    # the coordinator's blind variants, verbatim, for comparison
    "vA": [(FENCE_END, FENCE_END + "An `@` parameter is ONE element. Where C writes several through it and no\n"
            "argument says how many, declare the fixed array it fills, `@md: u8[32]`, and\n"
            "lend a field of exactly that type: `@d.b`.\n")],
    "vB": [(FENCE_END, FENCE_END + "An `@` parameter is ONE element.\n"),
           ("naming the sibling that gives the extent, and one past the field is refused.\n",
            "naming the sibling that gives the extent or, where C's extent is fixed, a\n"
            "constant or a number (`md: ptr counted_by 32 lent`), and one past the field is\n"
            "refused.\n")],
}

# compositions and variants drafted after runs 2 to 4 (vendored only)
S7S_AFTER = ("Where C writes several, `@md: [u8] counted_by N`, `N` a constant of the group or\n"
             "the sibling that tells C how many, hands C `N` elements, the array's own then\n"
             "zeros, and leaves the array exactly those `N`.\n")
S7G_AFTER = S7_AFTER[:-2] + "; C writing past them aborts.\n"
S7SG_AFTER = S7S_AFTER[:-2] + "; C writing past them aborts.\n"
PTR_S2C = PTR_S2[:-2] + "; a group's record lent whole with `@` may say it too.\n"
DRAFTS["S7s"] = [(OUT_OLD, OUT_U), (FENCE_END, FENCE_END + S7S_AFTER)]
DRAFTS["S7g"] = [(OUT_OLD, OUT_U), (FENCE_END, FENCE_END + S7G_AFTER)]
DRAFTS["S7S2"] = [(OUT_OLD, OUT_U), (FENCE_END, FENCE_END + S7_AFTER), (PTR_OLD, PTR_S2)]
DRAFTS["S2C"] = [(OUT_OLD, OUT_U), (PTR_OLD, PTR_S2C)]
DRAFTS["R"] = [(OUT_OLD, OUT_U5), (FENCE_END, FENCE_END + S7G_AFTER), (PTR_OLD, PTR_S2)]
DRAFTS["Rec"] = [(OUT_OLD, OUT_U5), (FENCE_END, FENCE_END + S7SG_AFTER), (PTR_OLD, PTR_S2C)]


def apply(name):
    text = open(PRISTINE, encoding="utf-8").read()
    for old, new in DRAFTS[name]:
        n = text.count(old)
        if n != 1:
            sys.exit(f"{name}: anchor matched {n} times: {old[:60]!r}")
        text = text.replace(old, new)
    return text


if __name__ == "__main__":
    if sys.argv[1] == "--list":
        print(" ".join(DRAFTS))
    elif sys.argv[1] == "--show":
        for old, new in DRAFTS[sys.argv[2]]:
            print("--- old\n" + old + "+++ new\n" + new)
    else:
        open(sys.argv[2], "w", encoding="utf-8").write(apply(sys.argv[1]))

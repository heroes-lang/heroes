"""Panel 215, spec-warden, Q3's two sentences as exact edits of the spec.

Run: python3 -I drafts_q3.py <base.md> <outdir>. Each draft replaces BLOCK (spec
§ 13, from "elements as the type says." to "counting as one.") whole, and may
add the § 9 edit; every anchor must occur exactly once in the base.
"""
import os
import sys

BLOCK = (
    "elements as the type says. End a construction with `rest: zero` and every field\n"
    "sharing no byte with one it names is zero; only a group's record has it. A\n"
    "bit-field is none of these: leave it to `partial`.\n"
    "`record Font partial` names only some, and its size stays C's, not the field\n"
    "list's. A field in a C union, named or anonymous, shares its bytes with the\n"
    "union's other members: a record names one or more of each union, reads any, and\n"
    "is built naming exactly one, or none when it ends with `rest: zero`, an\n"
    "anonymous struct's fields counting as one.\n"
)
AT_PARAM = ("  `function advance(@l: Lex)` … `advance(@l)`. Semantics: copy in, copy\n"
            "  out (copy-out always happens, including on early return and `?`). UFCS does\n")
AT_NEW = ("  `function advance(@l: Lex)` … `advance(@l)`. Semantics: the callee works on\n"
          "  the caller's place itself. UFCS does\n")

PARTIAL = ("`record Font partial` names only some, and its size stays C's, not the field\n"
           "list's.")
UNION_OLD = (" A field in a C union, named or anonymous, shares its bytes with the\n"
             "union's other members: a record names one or more of each union, reads any, and\n"
             "is built naming exactly one, or none when it ends with `rest: zero`, an\n"
             "anonymous struct's fields counting as one.\n")
R1_OLD = ("End a construction with `rest: zero` and every field\n"
          "sharing no byte with one it names is zero; only a group's record has it. A\n")
SHARE = (" A union's members share its bytes: a record names one or more of each\n"
         "union, reads any, and is built naming exactly one, or none when it ends with\n"
         "`rest: zero`, an anonymous struct's fields counting as one.\n")

# --- R1's sentence alone, the union sentence as it is ---
R1_A = ("End a construction with `rest: zero` and every field\n"
        "of C's struct sharing no byte with one it names is zero, those `partial` leaves\n"
        "out included; only a group's record has it. A\n")
R1_B = ("End a construction with `rest: zero` and every field\n"
        "of C's struct, declared or not, sharing no byte with one it names is zero; only\n"
        "a group's record has it. A\n")
# R1 kept, the promise carried by the `partial` sentence
PARTIAL_C = ("`record Font partial` names only some; its size stays C's, not the field\n"
             "list's, and `rest: zero` zeroes what it leaves out.")

# --- the union sentence alone ---
U_A = (" A field is a name C reads as `s.name`: an anonymous union's or struct's\n"
       "members are, and so is a header macro's, but never `via.fd`, so a named union\n"
       "is a field of its typedef's record, or left to `partial`." + SHARE)
U_B = (" A field is a name C reads as `s.name`: an anonymous union's or struct's\n"
       "members are, and so is a header macro's, but never `via.fd`, so a named union\n"
       "is a field whose type is the record of its typedef, or is left to `partial`." + SHARE)
U_C = (" A field is a name C reads as `s.name`, an anonymous union's or struct's\n"
       "member or a header macro's, never `via.fd`." + SHARE)
U_D = (" A field in an anonymous C union, or one a header macro names, shares its\n"
       "bytes with the union's other members: a record names one or more of each union,\n"
       "reads any, and is built naming exactly one, or none when it ends with\n"
       "`rest: zero`, an anonymous struct's fields counting as one. A named union\n"
       "without such a macro is a field of its typedef's record, or left to `partial`.\n")

# --- the union sentence with a neutral example (`via` is the A2 task's own name) ---
U_A2 = (" A field is a name C reads as `s.name`: an anonymous union's or struct's\n"
        "members are, and so is a header macro's, but never `s.u.n`, so a named union\n"
        "is a field of its typedef's record, or left to `partial`." + SHARE)
U_E = (" A field is a name C reads as `s.name`: an anonymous union's or struct's\n"
       "member, or a header macro's, never `s.u.n`; a named union is a field typed by\n"
       "its typedef's record, or left to `partial`." + SHARE)
U_E_EX = (" A field is a name C reads as `s.name`: an anonymous union's or struct's\n"
          "member, or a header macro's such as `s6_addr`, never `s.u.n`; a named union is\n"
          "a field typed by its typedef's record, or left to `partial`." + SHARE)
U_C2 = (" A field is a name C reads as `s.name`, an anonymous union's or struct's\n"
        "member or a header macro's, never `s.u.n`." + SHARE)
SHARE2 = (" A record names one or more of each union's members, reads any, and is built\n"
          "naming exactly one, or none when it ends with `rest: zero`, an anonymous\n"
          "struct's fields counting as one.\n")
U_E_S2 = (" A field is a name C reads as `s.name`: an anonymous union's or struct's\n"
          "member, or a header macro's, never `s.u.n`; a named union is a field typed by\n"
          "its typedef's record, or left to `partial`." + SHARE2)

# the example carries the rule: where C reads `s.u.n`, `u` is the field
U_I = (" A field is a name C reads as `s.name`: an anonymous union's or struct's\n"
       "member, or a header macro's. Where C reads `s.u.n`, `u` is the field, typed by\n"
       "the record of its union's typedef, or left to `partial`." + SHARE)
U_I_NOMACRO = (" A field is a name C reads as `s.name`, an anonymous union's or struct's\n"
               "member too. Where C reads `s.u.n`, `u` is the field, typed by the record of\n"
               "its union's typedef, or left to `partial`." + SHARE)

U_K = (" A field is what C reads as `s.name`, an anonymous union's or struct's\n"
       "member or a header macro's too; where C reads `s.u.n`, the field is `u`, typed\n"
       "by its union's typedef or left to `partial`." + SHARE)

# the floor: only the misleading word out
U_ANON = (" A field in an anonymous C union shares its bytes with the\n"
          "union's other members: a record names one or more of each union, reads any, and\n"
          "is built naming exactly one, or none when it ends with `rest: zero`, an\n"
          "anonymous struct's fields counting as one.\n")

# --- merged: R1 moved after `partial` and joined to it, the union sentence after ---
HEAD = ("elements as the type says. A\n"
        "bit-field is none of these: leave it to `partial`.\n")
M_R1 = (" End a construction with `rest: zero` and every\n"
        "field of C's struct sharing no byte with one it names is zero, those `partial`\n"
        "leaves out included; only a group's record has it.")
M_R1_SHORT = (" End a construction with `rest: zero` and every\n"
              "field sharing no byte with one it names is zero, those `partial` leaves out\n"
              "included; only a group's record has it.")
M_R1_SHORT_NOGROUP = (" End a construction with `rest: zero` and every\n"
                      "field sharing no byte with one it names is zero, those `partial` leaves out\n"
                      "included.")
M_R1_NOGROUP = (" End a construction with `rest: zero` and every\n"
                "field of C's struct sharing no byte with one it names is zero, those `partial`\n"
                "leaves out included.")


def wrap(text, width=80):
    """Reflow a paragraph chunk: one space between words, lines of at most
    `width` columns, never a break inside a backticked span."""
    words, cur, inside = [], "", False
    for w in text.split():
        cur = (cur + " " + w) if cur else w
        inside ^= (w.count("`") % 2 == 1)
        if not inside:
            words.append(cur)
            cur = ""
    if cur:
        words.append(cur)
    lines, line = [], ""
    for w in words:
        if line and len(line) + 1 + len(w) > width:
            lines.append(line)
            line = w
        else:
            line = (line + " " + w) if line else w
    lines.append(line)
    return "\n".join(lines) + "\n"


def block(r1=None, partial=PARTIAL, union=UNION_OLD):
    """BLOCK with its R1 sentence, `partial` sentence and union sentence replaced."""
    b = BLOCK
    if r1 is not None:
        b = b.replace(R1_OLD, r1)
    b = b.replace(PARTIAL, partial)
    b = b.replace(UNION_OLD, union)
    return wrap(b)


def merged(r1, union):
    return wrap(HEAD + PARTIAL + r1 + union)


DRAFTS = {
    "base-rewrap": [(BLOCK, wrap(BLOCK) if wrap(BLOCK) != BLOCK else BLOCK + " ")],
    "r1-a": [(BLOCK, block(r1=R1_A))],
    "r1-b": [(BLOCK, block(r1=R1_B))],
    "r1-c": [(BLOCK, block(partial=PARTIAL_C))],
    "u-a": [(BLOCK, block(union=U_A))],
    "u-b": [(BLOCK, block(union=U_B))],
    "u-c": [(BLOCK, block(union=U_C))],
    "u-d": [(BLOCK, block(union=U_D))],
    "both-a": [(BLOCK, block(r1=R1_A, union=U_A))],
    "m-a": [(BLOCK, merged(M_R1, U_A))],
    "m-c": [(BLOCK, merged(M_R1, U_C))],
    "m-a-nogroup": [(BLOCK, merged(M_R1_NOGROUP, U_A))],
    "m-a-at": [(BLOCK, merged(M_R1, U_A)), (AT_PARAM, AT_NEW)],
    "at-alone": [(AT_PARAM, AT_NEW)],
    "u-a2": [(BLOCK, block(union=U_A2))],
    "u-e": [(BLOCK, block(union=U_E))],
    "u-e-ex": [(BLOCK, block(union=U_E_EX))],
    "u-c2": [(BLOCK, block(union=U_C2))],
    "u-e-s2": [(BLOCK, block(union=U_E_S2))],
    "m-b": [(BLOCK, merged(M_R1, U_E))],
    "m-b-short": [(BLOCK, merged(M_R1_SHORT, U_E))],
    "m-b-nogroup": [(BLOCK, merged(M_R1_NOGROUP, U_E))],
    "m-b-at": [(BLOCK, merged(M_R1, U_E)), (AT_PARAM, AT_NEW)],
    "m-b-nogroup-at": [(BLOCK, merged(M_R1_NOGROUP, U_E)), (AT_PARAM, AT_NEW)],
    "m-c2": [(BLOCK, merged(M_R1, U_C2))],
    "m-b-s2": [(BLOCK, merged(M_R1, U_E_S2))],
    "u-i": [(BLOCK, block(union=U_I))],
    "u-i-nomacro": [(BLOCK, block(union=U_I_NOMACRO))],
    "m-g": [(BLOCK, merged(M_R1_SHORT, U_I))],
    "m-g-at": [(BLOCK, merged(M_R1_SHORT, U_I)), (AT_PARAM, AT_NEW)],
    "m-g-nogroup": [(BLOCK, merged(M_R1_SHORT_NOGROUP, U_I))],
    "m-g-nogroup-at": [(BLOCK, merged(M_R1_SHORT_NOGROUP, U_I)), (AT_PARAM, AT_NEW)],
    "m-g-nomacro": [(BLOCK, merged(M_R1_SHORT, U_I_NOMACRO))],
    "m-k": [(BLOCK, merged(M_R1_SHORT, U_K))],
    "m-k-at": [(BLOCK, merged(M_R1_SHORT, U_K)), (AT_PARAM, AT_NEW)],
    "m-k-nogroup-at": [(BLOCK, merged(M_R1_SHORT_NOGROUP, U_K)), (AT_PARAM, AT_NEW)],
    "m-anon": [(BLOCK, merged(M_R1_SHORT, U_ANON))],
    "r1-short-only": [(BLOCK, merged(M_R1_SHORT, UNION_OLD))],
}


def main():
    base_path, outdir = sys.argv[1], sys.argv[2]
    base = open(base_path, encoding="utf-8").read()
    os.makedirs(outdir, exist_ok=True)
    for anchor in (BLOCK, AT_PARAM):
        if base.count(anchor) != 1:
            sys.exit(f"anchor occurs {base.count(anchor)} times: {anchor[:60]!r}")
    for name, edits in DRAFTS.items():
        text = base
        for old, new in edits:
            n = text.count(old)
            if n != 1:
                sys.exit(f"{name}: anchor occurs {n} times: {old[:60]!r}")
            if new == old:
                sys.exit(f"{name}: an edit changes nothing")
            text = text.replace(old, new)
        with open(os.path.join(outdir, name + ".md"), "w", encoding="utf-8") as f:
            f.write(text)
        print(name)


if __name__ == "__main__":
    main()

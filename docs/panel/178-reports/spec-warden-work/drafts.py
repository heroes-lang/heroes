# Panel 178 spec-warden: each draft is a list of (old, new) replacements on the pristine spec.
import sys

A13 = "array of one: `i32[4]`, never a `[T]`; build one with `[a, b, c, d]`, as many\nelements as the type says.\n"
MEMBER = '           | "record" ident [ "tag" ident ] [ "partial" ] ( Fields | NEWLINE ) .'
MEMBER_Z = '           | "record" ident [ "tag" ident ] [ "partial" ] [ "zero" ] ( Fields | NEWLINE ) .'
S9 = "- `x.f(y)` is sugar for `f(x, y)` (UFCS). There are no methods, no\n  inheritance, no overloading, no default values, no user variadics, no\n  anonymous functions."
S1 = "compile error. No braces, no semicolons, no parentheses around conditions."
PRIM = '               | "(" Expression ")" | "[" [ Expression { Sep Expression } ] "]"'
TYPE = '    Type     = Prefix { "[" integer "]" } [ "?" ] .'
VB = "fails `null_cstr`; `f.validated_bytes()` does the same for a field of bytes,\nreading to its first zero or the whole field, and either fails `not_text`.\n"
S9C = "- Record construction is a call with field names, always mandatory:\n  `Point(x: 3, y: 4)`, and a variant's case `.num(v: 7)` or `.plus`.\n"

def after13(text):
    return (A13, A13[:-1] + " " + text + "\n")

R1 = ("Or name only the fields you set and end with\n`rest: zero`: every field you do not name is zero, and so is every byte between\n"
      "fields. A record outside a group has no `rest`.")
Z1 = ("`rest: zero` is refused unless\nthe record says `zero` after its name, which claims that all zeros is a valid\n"
      "value of that struct on every platform.")
T = ("`s.to_fixed()` gives the bytes of a\n`str` as the fixed byte array its position expects, the rest zero, and fails\n"
     "when they and a terminating zero do not fit.")
T_HONEST = ("`s.to_fixed()` copies a `str`'s bytes,\nbit for bit, into the `i8` or `u8` array its position expects, the rest zero,\n"
            "and fails `too_long` when they and a terminating zero do not fit.")
Z2W = ("Zeros are not a valid value of\nevery struct: one its header initialises, such as a mutex, comes from its function.")

DRAFTS = {
  # 1. the proposal as written, and each clause alone
  "P_verbatim":      [after13(R1 + " " + Z1 + " " + T)],
  "P_honest":        [after13(R1 + " " + Z1 + " " + T), (MEMBER, MEMBER_Z)],
  "R1":              [after13(R1)],
  "R1_Z1_asworded":  [after13(R1 + " " + Z1)],
  "R1_Z1_honest":    [after13(R1 + " " + Z1), (MEMBER, MEMBER_Z)],
  "R1_Z2_warned":    [after13(R1 + " " + Z2W)],
  "T_verbatim":      [after13(T)],
  "T_honest":        [after13(T_HONEST)],
  "T_merged":        [(VB, VB[:-2] + ";\n`s.to_fixed()` is the way back, its bytes and then zeros into the `i8` or `u8`\narray its position expects, failing `too_long` where they and one zero do not fit.\n")],
  # 2. R2, A, R0
  "R2":              [after13("A construction of a group record may leave\nfields out, and each is zero, and so is every byte between fields."),
                      (S9, S9.replace("no default values,", "no default values but a\n  group record's zeros,"))],
  "A_bare_variantIV":[after13("`[x; n]` is an array of `n` copies of `x`,\nwhere `n` is an integer literal: `[0; 256]`.")],
  "A_merged_honest": [(A13, "array of one: `i32[4]`, never a `[T]`; build one with `[a, b, c, d]`, or\n`[x; 4]` for four of `x`, as many elements as the type says.\n"),
                      (PRIM, '               | "(" Expression ")" | "[" [ Expression ( ";" integer | { Sep Expression } ) ] "]"'),
                      (S1, "compile error. No braces, no semicolons but `[x; n]`'s, no parentheses around conditions.")],
  "R0":              [after13("`T.zero()` is a group record `T` whose every\nfield and byte is zero; a record outside a group has none.")],
  "R0_Z1_honest":    [after13("`T.zero()` is a group record `T` whose every\nfield and byte is zero, where the record says `zero` after its name, claiming\nthat all zeros is a valid value of that struct on every platform; a record\noutside a group has none."), (MEMBER, MEMBER_Z)],
  # the remaining routes
  "L_restated":      [after13("An element of a fixed array inside an `@`\ncell is written like any other, `u.name[0] @ 72`.")],
  "D":               [(S9C, S9C + "  A field left out is its type's zero: `0`, `false`, `\"\"`, empty, `nullptr`,\n  or a record of zeros; one whose type has none, a variant, a `T?` or a\n  function, is named.\n"),
                      (S9, S9.replace(" no default values,", ""))],
  "H_header_length": [(A13, "array of one: `i32[4]`, never a `[T]`, and `i8[_]` takes its length from the\nheader; build one with `[a, b, c, d]`, as many\nelements as the type says.\n"),
                      (TYPE, '    Type     = Prefix { "[" ( integer | "_" ) "]" } [ "?" ] .')],
  # the composition the report recommends: R1 + Z2 + L, with T merged
  "R1_Tmerged":      [after13(R1), (VB, VB[:-2] + ";\n`s.to_fixed()` is the way back, its bytes and then zeros into the `i8` or `u8`\narray its position expects, failing `too_long` where they and one zero do not fit.\n")],
}

DRAFTS.update({
  "R1_nopad":   [after13("Or name only the fields you set and end with\n`rest: zero`: every field you do not name is zero. A record outside a group has\nno `rest`.")],
  "R1_noguard": [after13("Or name only the fields you set and end with\n`rest: zero`: every field you do not name is zero, and so is every byte between\nfields.")],
  "R1_merged":  [after13("End a construction with `rest: zero` and\nevery field it does not name is zero, bytes between fields too; only a group's\nrecord has it.")],
})

DRAFTS.update({
  "R0_honest": [after13("`T.zero()` is a group record `T` whose every\nfield and byte is zero; a record outside a group has none."),
                (S9, S9.replace("is sugar for `f(x, y)` (UFCS).", "is sugar for `f(x, y)` (UFCS), but\n  `T.zero()` (section 13)."))],
})

DRAFTS.update({
  "R1_merged_allbytes": [after13("End a construction with `rest: zero` and\nevery field it does not name is zero, and so is every other byte; only a group's\nrecord has it.")],
  "R1_merged_padding":  [after13("End a construction with `rest: zero` and\nevery field it does not name is zero, padding too; only a group's record has it.")],
})

def apply(name, src):
    out = src
    for old, new in DRAFTS[name]:
        n = out.count(old)
        if n != 1:
            sys.exit(f"{name}: anchor occurs {n} times: {old[:60]!r}")
        out = out.replace(old, new)
    return out

if __name__ == "__main__":
    if sys.argv[1] == "list":
        print("\n".join(DRAFTS))
    else:
        name, pristine, target = sys.argv[1], sys.argv[2], sys.argv[3]
        src = open(pristine).read()
        out = apply(name, src)
        open(target, "w").write(out)

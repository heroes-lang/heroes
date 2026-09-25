# Critic, panel 178: drafts the seats did not price, on the spec-warden's own anchors.
import sys
sys.path.insert(0, __file__.rsplit('/', 1)[0])
from drafts_sw import A13, after13, apply as _unused, DRAFTS as SW
PRIM = '               | "(" Expression ")" | "[" [ Expression { Sep Expression } ] "]"'
S1 = "compile error. No braces, no semicolons, no parentheses around conditions."
VB = "fails `null_cstr`; `f.validated_bytes()` does the same for a field of bytes,\nreading to its first zero or the whole field, and either fails `not_text`.\n"
D = {
  # R1 in the spec-warden's merged wording, with the padding clause the historian and the ffi seat object to taken out
  "R1_merged_nopad": [after13("End a construction with `rest: zero` and\nevery field it does not name is zero; only a group's record has it.")],
  # the spec-warden's recommended text, re-measured here as a control on the instrument
  "R1_merged_padding": SW["R1_merged_padding"],
  # T as the compiler-engineer's place form, merged into the validated_bytes sentence the way T_merged was
  "T_place_merged": [(VB, VB[:-2] + ";\n`s.copy_into(@f)` is the way back, its bytes and then zeros into an `@` binding's\nbyte field `f`, failing `too_long` where they and one zero do not fit.\n")],
  # A whose length is the type's: the literal restates no N
  "A_blank_merged": [(A13, "array of one: `i32[4]`, never a `[T]`; build one with `[a, b, c, d]`, or\n`[x; _]` for as many of `x` as the type says.\n"),
                     (PRIM, '               | "(" Expression ")" | "[" [ Expression ( ";" "_" | { Sep Expression } ) ] "]"'),
                     (S1, "compile error. No braces, no semicolons but `[x; _]`'s, no parentheses around conditions.")],
  # route C said once, for the reader who would not think of it: no form, one pointer
  "C_pointer": [after13("A struct too long to spell comes from a\nfunction of the program's own header that returns one, zeroed or initialised in C.")],
}
def apply(name, src):
    out = src
    for old, new in D[name]:
        n = out.count(old)
        if n != 1:
            sys.exit(f"{name}: anchor occurs {n} times: {old[:60]!r}")
        out = out.replace(old, new)
    return out
if __name__ == "__main__":
    if sys.argv[1] == "list":
        print("\n".join(D))
    else:
        open(sys.argv[3], "w").write(apply(sys.argv[1], open(sys.argv[2]).read()))

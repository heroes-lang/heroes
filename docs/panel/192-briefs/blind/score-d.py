#!/usr/bin/env python3
"""Panel 192's fifth blind arm, D, the scorer, written 2026-10-05 before any of
its sessions ran (funded by the author's ratification of that day).

Arm D reads the base specification (4c3524fb) with route (d)'s words as R8
gives them: s1-mix (a string holds no raw control character), s2-d with the
critic's three repairs (`\\u{...}` in lowercase hex with no leading zero, never
0 or a surrogate, not in a character literal, for a character a string holds no
other way), and r1. Its task needs U+00E9 and U+200B, both of which a string
holds raw under R2, so R5 refuses `\\u{e9}` and `\\u{200b}`, each with a
`guess` fix to the raw character.

For each folder d<n> of 192-llm-ergonomist-2/:
- reads c.hero and records how it spelled each of the two characters;
- applies R5 as the landing will: an escape `\\u{HEX}` is ADMITTED only for a
  code point R2 refuses raw (C0 and C1 controls and DEL, but the three a letter
  escape writes, U+0009, U+000A, U+000D; the twelve Bidi_Control; U+2028,
  U+2029), spelled as `format(v, "x")`; any other is REFUSED and the program
  fails, named with its reason; a raw character R2 refuses inside the source is
  refused the same way;
- replaces each admitted escape by its character, builds the result with the
  base's compiler, runs it, and scores the output bytes and the exit code.

A pass: exit 0 and stdout exactly b"caf\\xc3\\xa9\\xe2\\x80\\x8b!\\n".

REGISTERED BEFORE THE RUN (the coordinator's prediction, 2026-10-05):
- `é` is written raw in at least 4 of 5 programs;
- U+200B is written as `\\u{200b}` in at least 2 of 5, which R5 refuses;
- so at most 3 of 5 pass.
Read as: 4 or 5 passes, R5's restriction costs a one-turn writer nothing on
this task; 2 or more refused escapes, the restriction is a mistake models
make, which R5 turns into a compile error whose fix is a `guess`, not
`certain`."""
import json, os, re, subprocess, sys

S = "/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad"
E = S + "/192-llm-ergonomist-2"
H = S + "/192-base/heroes"
RT = S + "/192-base/runtime"
WANT = b"caf\xc3\xa9\xe2\x80\x8b!\n"

BIDI = {0x061C, 0x200E, 0x200F, 0x202A, 0x202B, 0x202C, 0x202D, 0x202E, 0x2066, 0x2067, 0x2068, 0x2069}
REFUSED_RAW = (set(range(0x01, 0x20)) | {0x7F} | set(range(0x80, 0xA0)) | BIDI | {0x2028, 0x2029})
LETTER = {0x09, 0x0A, 0x0D}
U = re.compile(rb"\\u\{([0-9A-Za-z]*)\}")


def r5(m):
    """(admitted, reason) for one `\\u{...}` under R5."""
    digits = m.group(1).decode("ascii", "replace")
    try:
        v = int(digits, 16)
    except ValueError:
        return False, f"\\u{{{digits}}} is no hex number"
    if v == 0 or 0xD800 <= v <= 0xDFFF or v > 0x10FFFF:
        return False, f"\\u{{{digits}}} is 0, a surrogate or past U+10FFFF"
    if v in LETTER:
        return False, f"\\u{{{digits}}} is written by a letter escape (certain fix)"
    if v not in REFUSED_RAW:
        return False, f"\\u{{{digits}}}: a string holds U+{v:04X} raw (guess fix to the raw character)"
    if digits != format(v, "x"):
        return False, f"\\u{{{digits}}} is not the one spelling \\u{{{v:x}}} (certain fix)"
    return True, ""


def spelled(src):
    e = []
    if "\u00e9".encode() in src:
        e.append("é raw")
    if re.search(rb"\\u\{0*[eE]9\}", src):
        e.append("\\u{e9}")
    if b"e\xcc\x81" in src:
        e.append("e + combining accent")
    if re.search(rb"195\s*,\s*169", src) or re.search(rb"0x[cC]3\s*,\s*0x[aA]9", src):
        e.append("é as bytes")
    z = []
    if "\u200b".encode() in src:
        z.append("U+200B raw")
    if re.search(rb"\\u\{0*200[bB]\}", src):
        z.append("\\u{200b}")
    if re.search(rb"226\s*,\s*128\s*,\s*139", src) or re.search(rb"0x[eE]2\s*,\s*0x80\s*,\s*0x8[bB]", src):
        z.append("U+200B as bytes")
    if re.search(rb"^extern ", src, re.M):
        z.append("an extern")
    return ", ".join(e) or "é not found", ", ".join(z) or "U+200B not found"


def score(n):
    d = f"{E}/d{n}"
    out = {"session": f"d{n}"}
    try:
        src = open(f"{d}/c.hero", "rb").read()
    except OSError:
        out.update(verdict="FAIL", why="no c.hero")
        return out
    out["e_acute"], out["zwsp"] = spelled(src)
    refused = [why for ok, why in (r5(m) for m in U.finditer(src)) if not ok]
    try:
        text = src.decode("utf-8")
        raw = sorted({f"U+{ord(c):04X}" for c in text if ord(c) in REFUSED_RAW and c not in "\t\n\r"})
    except UnicodeDecodeError:
        raw = ["not UTF-8"]
    if raw:
        refused.append("raw " + " ".join(raw) + " refused by R2")
    out["escapes"] = [m.group(0).decode("ascii", "replace") for m in U.finditer(src)]
    if refused:
        out.update(verdict="FAIL", why="refused under (d): " + "; ".join(refused))
        return out
    sub = U.sub(lambda m: chr(int(m.group(1), 16)).encode("utf-8"), src)
    w = f"{d}/score"
    os.makedirs(w, exist_ok=True)
    open(f"{w}/c-sub.hero", "wb").write(sub)
    env = dict(os.environ, HEROES_RUNTIME=RT)
    b = subprocess.run([H, "build", "c-sub.hero", "-o", "prog"], cwd=w, env=env, capture_output=True, timeout=300)
    out["build_exit"] = b.returncode
    if b.returncode != 0:
        out.update(verdict="FAIL", why="build: " + b.stderr.decode("utf-8", "replace")[:600])
        return out
    r = subprocess.run([f"{w}/prog"], cwd=w, capture_output=True, timeout=60)
    out["run_exit"] = r.returncode
    out["stdout"] = r.stdout.hex()
    ok = r.returncode == 0 and r.stdout == WANT
    out.update(verdict="PASS" if ok else "FAIL", why="" if ok else f"printed {r.stdout!r}, exit {r.returncode}")
    return out


if __name__ == "__main__":
    for n in sys.argv[1:] or ["1", "2", "3", "4", "5"]:
        print(json.dumps(score(n), ensure_ascii=False))

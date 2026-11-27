# Grader B: a mechanical rule, written 2026-11-26 before grader A (the repair) read
# panel 259's answers whole. It labels one wrong answer (session, program) by the
# registered labels of panel 262 (several allowed), from the session's
# choice_points section and the lines of its answer section naming the program.
# Its contamination: the repair had read panel 259's report's summary of the
# readers' reasons (llm-ergonomist-blind.md:157-161, 178-185) before writing it.
import re, sys
LABELS = [
    ("W", r"(may abort|may stop|may panic)[^.]{0,160}(cover|appl|includ|any|every)[^.]{0,80}(switch|swapcontext|setcontext|jump|frame|stack)"
          r"|(switch|swapcontext|setcontext|jump|frame)[^.]{0,120}(covered|falls? (under|within)|inside)[^.]{0,60}(may abort|handler sentence|the sentence)"),
    ("N", r"may abort[^.]{0,80}(means|read as|as) (it )?(aborts|will abort)|(may|might) abort[^.]{0,40}(i|we) (took|read|treat)[^.]{0,30}abort"),
    ("L", r"exhaustive|closed list|not (on|in) (the|its) list|(list|names)[^.]{0,40}(only|none|exits)[^.]{0,80}(so|therefore)[^.]{0,40}(runs|promised)"),
    ("H", r"callback frame|any frame|frames? of c'?s|whatever a handler is|handler[^.]{0,60}(means?|meant|read as|took|taken|stands? for|refers? to)[^.]{0,60}(callback|frame|function of the program|any)"),
    ("T", r"(other|another|different|same) thread|on (its|the) thread|thread[^.]{0,40}(handler|left)"),
    ("I", r"pronoun|antecedent|\"it\"|`it`|lands within it[^.]{0,80}(stack|refers)"),
    ("P", r"(mac|darwin|macos)[^.]{0,120}(linux)[^.]{0,80}(differ|split|may|not the same)|(platform|architecture)[^.]{0,60}(differ|split|depend)"),
    ("R", r"1 gib|4,?096 bytes|the room|have not returned|large value"),
    ("S", r"running out of stack|out of stack|stack (overflow|exhaust|depth)|guard page|misjudge"),
]
def label(text):
    t = text.lower()
    got = [name for name, rx in LABELS if re.search(rx, t)]
    return got or ["O"]
def sections(md):
    m = re.search(r"choice_points(.*?)(\n#+ *context|\n\*\*context|\Z)", md, re.S | re.I)
    return m.group(1) if m else md
if __name__ == "__main__":
    base, units = sys.argv[1], sys.argv[2]
    for line in open(units):
        if not line.strip() or line.startswith("#"):
            continue
        sess, prog = line.split()[:2]
        md = open(f"{base}/{sess}/result.md").read()
        cp = sections(md)
        own = "\n".join(l for l in md.split("\n") if re.search(r"\b" + re.escape(prog) + r"\b", l, re.I))
        print(sess, prog, ",".join(label(cp + "\n" + own)), sep="\t")

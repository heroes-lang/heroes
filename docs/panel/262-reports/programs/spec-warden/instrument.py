# The instrument's change for a text: tests/harness/spec_refusals.hero with SENTENCE_OPENS set to
# the text's opening, and its own test's synthetic sentence built on SENTENCE_OPENS, so the test
# no longer hard-codes an opening a text may move.
# usage: python3 -I instrument.py <in.hero> <out.hero> <opening>
import sys

src, dst, opening = sys.argv[1], sys.argv[2], sys.argv[3]
s = open(src).read()


def swap(s, old, new):
    assert s.count(old) == 1, (old[:60], s.count(old))
    return s.replace(old, new)


s = swap(s, '    "A callback called after C left a handler of C\'s"\n', '    "' + opening + '"\n')
s = swap(
    s,
    '    spec = "text A callback called after C left a handler of C\'s by a jump, may abort,',
    '    spec = "text " + SENTENCE_OPENS + " by a jump, may abort,',
)
s = swap(
    s,
    "# Spec § 13 tells a reader that a callback called after C left a handler of\n"
    "# C's by one of five exits may abort, *saying the handler was not seen to end,\n"
    "# or that C called it from inside one*.",
    "# Spec § 13 tells a reader when a callback called on the thread of a signal\n"
    "# handler of C's may abort, *saying the handler was not seen to end, or that C\n"
    "# called it from inside one* (panel 257's R1, re-worded by panel 262).\n#",
)
open(dst, "w").write(s)

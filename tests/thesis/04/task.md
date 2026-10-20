# Task 4: run-length code

The program receives two arguments: a mode, `encode` or `decode`, and a text.

To encode, the text must be one or more lowercase ASCII letters, `a` to `z`. Each maximal run of
one letter becomes the letter followed by how many times it repeats, in decimal with no leading
zero, and the program prints the results one after another with nothing between them, then a line
feed.

To decode, the text must be one or more pairs, each a lowercase ASCII letter followed by a count:
one or more decimal digits with no leading zero, so a count is at least 1. Each pair becomes its
letter repeated count times, and the program prints them one after another, then a line feed; two
pairs in a row may carry the same letter. A text given to decode is promised, and need not be
checked, to hold no run of decimal digits standing for more than 1,000,000, and to decode to at
most 1,000,000 letters when it is of the form.

Exit with status 0. If the text is not of the form its mode asks for, the empty text included,
print `bad input` and exit with status 1. If the program does not receive exactly two arguments, or
the mode is neither `encode` nor `decode`, print nothing and exit with status 2.

Example. The arguments `encode aaabcc` print `a3b1c2`, and `decode a3b1c2` print `aaabcc`.

canary heroes-task:18a0fd51a75fb1dc9cd4bdf8bcf2c732

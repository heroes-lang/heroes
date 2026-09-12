- [ ] **M-struct-passing close — the offers** | panel 091 (a saving that costs nothing because you were going to get it anyway) | A program that prints `1` carries 27 lines of C verifying a library it never touches, and under one `.c` per module 138 of 155 modules would carry them. The obvious fix — emit them only where they are reached — was refused, and the duplication dies anyway. Say why: what is it about the LIBRARY being one module, plus one `.c` per module, that makes the multiplier disappear with no code written?

    **Where to look:** docs/panel/091 § The resolution · selfhost/source.hero:109-110 · design.md:786
    **Why it matters:** the cheapest change is the one the architecture was already going to make, and spotting it is worth more than building the clever version

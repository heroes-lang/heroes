- [ ] **M-deferral-ledger 8** | design.md says the remedy for the Windows-path wart is raw string literals. Take that claim apart: write the buggy line, then say precisely which reader a raw literal would have helped and which one it would not. Then find the three cheaper routes the sitting listed, and say why none of them was adopted.

    **Where to look:** the remedy paragraph landed under design.md Part 8 wart 15;
    the reader's sentence *"it targets I do not know how to escape this, which is
    not the failure that happens"*; and `selfhost/literals.hero:82-89`, where the
    compiler already refuses a raw carriage return in a literal for the same reason.

    **Why it matters:** the cure named in the entry aids somebody who already
    noticed the problem — and somebody who has noticed writes the double backslash
    correctly. The failure is not ignorance of escaping; it is that a string being
    transcribed gets less attention than code being composed. Python spent ten
    years making unknown escapes stricter and it changed nothing here, because the
    escapes that bite are the legal ones. Seeing that a remedy and a failure can
    aim at different people is the whole lesson.

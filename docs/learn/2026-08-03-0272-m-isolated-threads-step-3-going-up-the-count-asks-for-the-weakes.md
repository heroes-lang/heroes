- [ ] **M-isolated-threads step 3** | Going UP the count asks for the weakest order there is and going DOWN asks for a stronger one. Read the two paragraphs in `str.c` and say, in your own words, what the thread that frees the block has to be able to SEE, and why the thread that merely takes a new reference has to see nothing

    **Where to look:** runtime/parts/str.c, the comment above `hero_str_incref`
    **Why it matters:** the asymmetry looks arbitrary and is the one thing in this file that cannot be guessed

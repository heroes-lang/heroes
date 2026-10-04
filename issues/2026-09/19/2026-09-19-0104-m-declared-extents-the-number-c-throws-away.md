- [ ] **M-declared-extents walkthrough** | A C header writes `void f(char b[8])`. **Before reading: how many of those eight does the C compiler remember, and what would you expect `clang -ast-dump` to print for that parameter?** | `docs/measurements/035-not-one-byte-array-is-spelled-the-same-way-on-both.md` § The instrument

    **Where to look after answering:** it remembers **none** of them. C
    adjusts a parameter written `T a[N]` to `T *a` (C11 §6.7.6.3p7), so the
    extent is not part of the type and never reaches the tree:

    ```
    f_ptr    {"qualType": "char *"}        # void f_ptr(char *b);
    f_arr8   {"qualType": "char *"}        # void f_arr8(char b[8]);
    f_arrN   {"qualType": "char *"}        # void f_arrN(char b[]);
    f_arr2d  {"qualType": "char (*)[8]"}   # void f_arr2d(char b[4][8]);
    f_static {"qualType": "char *"}        # void f_static(char b[static 16]);
    ```

    **Why the 2D case is the one that keeps a number.** Only the OUTERMOST
    dimension decays, so `char[4][8]` becomes a pointer to `char[8]` and the
    inner 8 survives as part of the pointee's type. That single row is the
    whole rule, shown rather than stated.

    **Why it matters here.** It is the reason panel 164's route 6 is
    interesting at all: the Heroes compiler would check a number that C throws
    away, so Heroes would be stricter than the language it is binding. And it
    is the reason the obvious instrument was the wrong one — an AST-shaped
    census would have returned **zero** to a question it was not asking, and
    returned it confidently.

    **The question to carry away.** The census had to read the header's own
    bytes back through each parameter's source range. Ask what other questions
    about C a compiler's own data structures cannot answer, and how you would
    know you were asking one.

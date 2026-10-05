- [ ] **M-readable-bytes golden ratification** | A program that compiles and lies is what defeated refusing this. **Before reading: `f.name` is an `i8[8]` holding the bytes of "Darwin". Write the loop a reader would write to print it, using only what the language had. Then say what it prints.** | `docs/panel/162-the-conversion-already-existed-and-what-was-missing-was-the-extent.md`

    The loop:

    ```
    out @ out + f.name[i].to_str()
    ```

    **Where to look after answering:** it prints `6897114119105110`. Nothing in
    the specification refuses it — `i8` is a number, `to_str` on a number cannot
    fail, `str + str` concatenates — and the spec-warden confirmed it builds at
    exit 0.

    **Why it matters.** design.md Part 6 asks a refusal to name the program fact
    that would make it wrong. The fact was in the tree: a wrong program already
    compiled, and a refusal could not have reached it. That is what took
    "refuse it" off the table.

    **The question to carry away.** The judge who found this could not read the
    compiler — its seat forbids it. Ask what that restriction bought here, and
    whether any of the four seats who could read the compiler would have written
    that loop.

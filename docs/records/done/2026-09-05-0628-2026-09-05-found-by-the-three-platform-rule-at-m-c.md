- [x] **014 — every FFI diagnostic was blind on Windows whenever the path was absolute** | 2026-09-05, found by the three-platform rule at M-c-callbacks step 4, on the real box the author had just started | **repaired the same hour** | it was never anyone's change: it has been there since `location()` was written, and only a drive letter exposes it | **§4.19's own named failure** — the compiler blaming itself for a mistake in a `.hero` file

    **THE SYMPTOM.** The same program is exit 1 on macOS and **exit 2 on
    Windows**, `internal error: compiling the generated C failed`, whenever the
    source is given by an absolute path. Measured on Windows Server 2025 /
    clang 22, against a callback whose only mistake is `i64` where the header
    says `int`:

    ```
    C:/Users/Administrator/.../note.hero   before: exit 2   after: exit 1, error[ffi_callback_type]
    winpath.hero (relative)                before: exit 1   after: exit 1
    ```

    **THE CAUSE, and it is one line of arithmetic.**
    `selfhost/emit/ffi_site.hero`'s `location()` read clang's
    `<file>:<line>:<col>: <severity>: <message>` by taking the **first three**
    colons. On Windows the first colon is the **drive letter's**, so the file
    name came out as `C`, the line number as the whole rest of the path, and the
    parse refused because that is not a number. `location()` is the gate every
    `ffi_*` classifier passes through — width, sign, class, arity, format string,
    writable parameter, callback type — so **all of them fell through to exit 2
    together**, on the machine where an author is most likely to type an absolute
    path.

    **WHY NOTHING CAUGHT IT.** CI compiles relative paths, so its Windows leg
    has never produced a line with a drive letter in it. Nine sittings have
    touched this function's callers and none had a Windows box in front of it.
    It surfaced because the author started the machine for a different question
    and the coordinator ran an absolute-path probe on it.

    **THE REPAIR.** Read the location **from the right**. The seam is the `": "`
    that ends the column and opens the severity — a path may hold a colon, and
    it may not hold a colon followed by a space where clang puts one. Inside that
    prefix the last two colons are the line and the column, whatever precedes
    them. `rest` is cut at the same place it always was, so nothing downstream
    changes: this moves WHERE the location is cut, not what the remainder is, and
    the test says so in as many words.

    **THE PROOF.** Four tests over the exact strings, in `ffi_site.hero` itself —
    a drive letter survives; a relative path still reads (the shape that hid it);
    a line that is not a position report is refused rather than invented; and a
    file this compilation does not own is still refused, drive letter or not.
    Then re-measured **on the box**: absolute path exit 2 → **exit 1** with
    `error[ffi_callback_type]` on the author's own line, relative path unchanged.
    583 compiler tests, 1518 net checks, 108 harness tests green on this Mac.

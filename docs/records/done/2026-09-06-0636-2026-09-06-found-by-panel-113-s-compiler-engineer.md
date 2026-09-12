- [x] **014 — a `[T]` or `{K: V}` crosses the FFI boundary inside a callback signature, and nothing says a word** | 2026-09-06, found by panel 113's compiler-engineer seat and reproduced independently by the coordinator the same hour | **OPEN** | the sitting convened on copy-on-write's test-and-mutate; this is what it found instead, and it is the channel the corruption actually came through | **§1.12** — it is the one route by which a refcounted Heroes value reaches a thread the program did not start, so it is the door panel 111's four corruption classes were measured behind

    **The reproducer, eleven lines, run today.** A header that names a function
    pointer taking and returning a container is enough; no runtime include, no
    library:

    ```c
    /* shim.h */
    struct HeroArrayHeader;
    void hold(struct HeroArrayHeader *(*mk)(void), void (*run)(struct HeroArrayHeader *));
    ```

    ```
    extern "shim.h"
        function hold(mk: (function() -> [i64]), run: (function([i64]) -> ()))

    function make() -> [i64]
        return [1, 2, 3]

    function bump(xs: [i64])
        print(len(xs))

    function main()
        hold(mk: make, run: bump)
    ```

    `heroes build t.hero --emit-c` is **exit 0 with an empty stderr**, and the
    emitted C carries the container across in both directions:

    ```
    HeroArrayHeader * h_t_make(void);
    void h_t_bump(HeroArrayHeader * h0_xs);
    ```

    **The cause, one line.** `selfhost/check/ffi.hero:45-57` is
    `crosses_the_boundary`, the single function that decides what a C header may
    spell. It refuses containers at `:53` — `.array | .fixed | .map | .fallible
    => return false` — and at `:52` it answers `.function_ty => return true`
    **without recursing into that function type's own parameters and result**.
    So the vocabulary rule is applied to the callback and not to what the
    callback carries.

    **The spec already says otherwise, so this is the compiler's bug and not a
    design question** (CLAUDE.md §12: spec beats compiler). `spec:224`: *"A
    callback is a **parameter**, never a result; **its parameters follow the
    same rule** and `()` is `void`"*. The same rule is `crosses_the_boundary`,
    which refuses `.array`.

    **What it costs, and it is why this is filed at §1.12 rather than as a
    tidiness item.** A `[T]` handed to C is a `HeroArrayHeader *` that C may
    store and pass to a thread it made itself. Panel 111 measured four classes
    of memory corruption from threads and attributed one of them to
    copy-on-write's `if (refcount == 1)`; panel 113's compiler seat, with
    `parts/thread.c`'s guard patched down in its own copy, ran **8 threads and
    240,000 mutations** over nested `[[str]]` and `{str: i64}` under ASan and
    ThreadSanitizer and got **exit 0 with zero warnings** — because C's own
    reference holds the count above 1, so `unshare` never sees the window. The
    crash it did reproduce lands in `hero_array_incref`, not in
    `hero_array_unshare`: a C lifetime bug of panel 053's family. So the
    corruption's door is this defect, and the `refcount == 1` window is a
    separate question that may be narrower than the record says.

    **What is owed**, pending panel 113's resolution: `crosses_the_boundary`
    recurses through `.function_ty` into its parameters and result; a golden
    under `tests/golden/check/` with its `#~` annotation (CLAUDE.md §9); and the
    diagnostic names the container and the callback, because a program that
    hands C a `[T]` is doing something the author believes is allowed.

    **What must NOT be assumed while it is open.** That the guard in
    `runtime/parts/thread.c` is redundant. It is what stops this program today:
    with the guard up, the same eight-thread program is `panic: … ran on a
    thread this program did not start`, exit 134. Today's safety is the guard,
    not unreachability.

    **THE REPAIR, M-isolated-threads step 5, 2026-09-06.** `crosses_the_boundary`
    recurses through `.function_ty` into its parameters and its result — the same
    walk over the same interned table that `selfhost/check/table.hero:278-285`
    already does, so it terminates for that function's reason. The reproducer
    above is now **exit 1** with a diagnostic on the `.hero` line.

    **The message is its own, and the generic one would have been wrong.**
    `ffi_type`'s text ends by listing what a C header can declare, and the list
    includes *a function type as a parameter* — true, and here it reads as a
    contradiction, because a function type as a parameter is exactly what the
    author wrote. So `ffi_callback_carries` reuses the `ffi_type` CODE and not
    its message, on `ffi_result_position`'s precedent three functions above:

        error[ffi_type]: `(function() -> [i64])` cannot cross the FFI boundary,
        and it is an `extern`'s parameter — the callback may stand there, but
        `[i64]` inside it cannot (§4.19)

    with two notes: why it is refused inside a callback for the reason it is
    refused outside one, and what to hand C instead — a `ptr` and a length, or
    one scalar at a time, keeping the container on the Heroes side.

    **The golden went in the wrong drawer first and the harness said so.** It was
    written under `tests/golden/fixedbugs/`, and `suite_emission` refused it:
    that directory is for wrong FFI bindings that **clang** rejects at build
    time, and `--emit-c` never calls clang, so every case there emits. This one
    is refused by the checker before the emitter runs, so it belongs in
    `tests/golden/check/` with an `.expected` — written by hand after reading the
    output, because CLAUDE.md §9 forbids regenerating a golden in that directory.

    **And the repair pushed `selfhost/check/ffi.hero` past CLAUDE.md §11's ~300
    lines, which forced a cut that was worth making.** The obvious seam — *the
    position*, which the file's own module doc calls a different question — is
    the one that must NOT be cut: that doc says the two Rust files were merged
    into one Heroes module on purpose, *"because the two questions read
    together"*. The cut taken instead is the **file-wide sweep**, structurally a
    third thing: a pass over every position rather than a question about a type
    in hand, and called from `checker.hero` rather than from the signature check.
    It is now `selfhost/check/ffi_sweep.hero`. **The compiler confirmed the seam
    was already there**: after the move, `checker.hero` used nothing at all from
    `check/ffi`, and `error[unused_binding]` made the import come out.

    Green on this Mac at the repair: **583** compiler tests, **112** harness
    tests, and the net **1526 passed, 0 failed** with the determinism diff empty.

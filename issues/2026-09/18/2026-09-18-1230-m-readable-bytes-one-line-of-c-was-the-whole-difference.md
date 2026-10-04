- [ ] **M-readable-bytes walkthrough** | The conversion this milestone needed already existed in the runtime. **Before reading: `hero_str_try_from_cstr` turns a C string into a `str?`. A C `char[256]` field is a run of bytes. Name the one line that stops the first from serving the second.** | `runtime/parts/str.c`, the two functions side by side

    The line is `size_t n = strlen(p);`.

    **Where to look after answering:** the new function beside it,
    `hero_str_try_from_bytes`, which takes the extent from the caller and reads
    *to its first zero, or whole*. Everything else in the two is identical: the
    same status word, the same one-walk discipline, the same promise that on
    anything but OK the string is empty and owns nothing.

    **Why it matters.** A `strlen` walks until it meets a zero. Panel 162
    measured 16 headers: 712 struct fields, 50 of type `char[N]`, and only 13
    reliably NUL-terminated. So the sibling would walk past the field in three
    cases of four, and nothing would catch it — unlike a null handle, the pointer
    is perfectly valid.

    **The question to carry away.** Ask why the sitting spent five seats
    deciding what the failure mode should be, when `read_file` had already
    answered `not_text` on invalid UTF-8 since panel 087. Then ask what would
    have happened if nobody had run it.

# The fourth platform spells none in its C library

2026-09-19, M-declared-extents. The milestone's second open item, closed on the
box the author powered on that morning.

- [x] **M-declared-extents** | walk the census on the Windows headers and on the fourth leg, so the route is priced on four platforms rather than two | `scratchpad/extents/census.py`, `docs/ref/environment/windows/WINDOWS-MACHINE.md`

## What closed it

`docs/measurements/035-not-one-byte-array-is-spelled-the-same-way-on-both.md`
gains its Windows rows, added underneath the sentence that called the platform
unrun rather than replacing it.

| | Darwin | Linux arm64 | Linux x86-64 | Win UCRT+`shared` | Win32 `um` |
|---|---|---|---|---|---|
| `.h` walked | 3120 | 5404 | 5411 | 345 | 1516 |
| spelled as an array | 82 | 182 | 182 | **0** | 89 |
| with a fixed extent | 31 | 59 | 59 | 0 | 33 |
| fixed **and** byte-typed | 12 | 31 | 31 | **0** | **2** |

The two byte cases are `BYTE abData[SAC_MAC_LEN]` in `scclient.h` and
`scserver.h`, the smart-card API.

**The C library spells none, and structurally rather than by being small.** The
UCRT assembles declarations from macros taking the type and the name as separate
arguments:

```c
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_0(
        char*, __RETURN_POLICY_DST, _ACRTIMP, tmpnam,
        _Pre_maybenull_ _Always_(_Post_z_), char, _Buffer )
```

So there is no array spelling to find. A second, independent text instrument over
all 66 UCRT headers agrees: zero.

**And the third number.** `L_tmpnam`, compiled and run on the box:

```
Darwin 1024      glibc 20      Windows 260
```

One constant of one C standard, three platforms, three values. Odin's shipped
hand-written binding says **15**; it is stale, and panel 165's historian had
named that exact failure mode — *"family 1 needs a whole second instrument to
stay true"* — a few paragraphs before its own example proved it.

## How it was run, and what the instrument was

The headers were copied off the box and the census run on this Mac with a Windows
target: `clang -target x86_64-pc-windows-msvc -fms-extensions -fms-compatibility
-nostdlibinc -isystem ucrt -isystem shared -isystem msvc -isystem um`. **Nothing
was installed on the machine** — it has no Python beyond the Microsoft Store
stub, and it is billed by the hour.

**The Windows leg used a weaker instrument than the other three, and the record
says so.** Darwin and both Linux legs are the text screen **plus** clang AST
confirmation through each parameter's source range. Windows is the screen alone,
because one `-ast-dump=json` over `windows.h` is **86.7 MB** and the AST pass over
1516 headers ran **four hours** without finishing before it was stopped. The
screen is precise on this tree — Win32 headers are declarations rather than inline
bodies, and inline bodies are what the AST stage exists to filter — but it is not
the same instrument, and a later session widening this census should not read the
columns as equally strong.

## What it changes about the milestone's question

Nothing, and that is the finding. Route 6's whole value was the number this
census produced, and the fourth platform moves it in the direction the first
three already pointed: **the sets are disjoint**. A byte-typed, fixed-extent array
parameter exists on every platform and no two platforms spell the same ones.
Windows adds two, both in an API nothing in this project binds.

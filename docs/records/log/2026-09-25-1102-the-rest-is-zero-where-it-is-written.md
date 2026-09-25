# The rest is zero where it is written, and C says which value is valid

2026-09-25 | a construction of a group record may end with `rest: zero`, and
every field it does not name is zero, with no promise about padding; zero is
admitted on every group record as bytes and never as a claim of validity; a
header's own initialiser binds as a group constant once defect 094 is repaired;
defect 091 is repaired as a lowering; § 13 says C's `char` is `i8`; a string
into a fixed field and `[x; N]` wait behind their measurements; a zero default
for every type is refused | the repository was the wrong sample: real headers
carry 23 to 42 public structs a platform with an array longer than 8; the
padding clause is false through a by-value call on x86-64; a claimed `zero`
guards a spelling while `partial` reaches the same state, and fifty blind
readers wrote no false claim and paid its cost ten times of ten; the largest
first-try failure was C's `char` | design.md §1.11, §1.12, §4.9, §4.19,
Principle 0 | **panel 178**, full, provisional; convened on the author's
instruction of 2026-09-24, after the author's question whether the repository
was a fair sample

## What it settles, and what it leaves

It settles the form of M-buildable-structs, and files four defects found while
measuring it: 091 (an element write of a fixed field that dies saying it is a
compiler bug), 092 (a record lent to `void *` that C overruns, a stack overflow
at `check` 0), 093 (a `str` with a zero byte that C reads cut short) and 094 (a
struct initialiser constant that stops the build). It leaves to the milestone
whether records should cross calls so that C never sees uninitialised padding,
whether a mutex may be copied, the diagnostic that sends a glibc typedef to a
handle, and the measurement that would bring a string-into-field form back.

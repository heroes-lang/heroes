# Panel 178 — llm-ergonomist

You read **only** two files: `heroes-spec.md` in your directory, which is the
language's specification as it stands, and this brief. Nothing else: no other
file, no repository. Your verdict is an experiment: write what a first-time
reader of the spec writes, then say where you hesitated. Write your report to
`REPORT.md` in your directory.

The variants below are **label-stripped**. Each one is read as if added to
§ 13 of the spec, after the sentence that ends *"build one with `[a, b, c, d]`,
as many elements as the type says."* Nothing tells you which one anybody
prefers, and one of them adds nothing at all.

---

## The variants

**Variant I.** No text is added.

**Variant II.**

> Or name only the fields you set and end with `rest: zero`: every field you do
> not name is zero, and so is every byte between fields. A record outside a
> group has no `rest`. `rest: zero` is refused unless the record says `zero`
> after its name, which claims that all zeros is a valid value of that struct
> on every platform. `s.to_fixed()` gives the bytes of a `str` as the fixed
> byte array its position expects, the rest zero, and fails when they and a
> terminating zero do not fit.

**Variant III.**

> Or name only the fields you set and end with `rest: zero`: every field you do
> not name is zero, and so is every byte between fields. A record outside a
> group has no `rest`. `s.to_fixed()` gives the bytes of a `str` as the fixed
> byte array its position expects, the rest zero, and fails when they and a
> terminating zero do not fit.

**Variant IV.**

> `[x; n]` is an array of `n` copies of `x`, where `n` is an integer literal:
> `[0; 256]`. An element of a fixed array inside an `@` cell is written like
> any other, `u.name[0] @ 72`.

---

## The tasks

For **each** task and **each** variant, write the complete program a first-time
reader writes, first attempt, before looking anything up twice. Then say whether
you believe it compiles, what it prints, and where you hesitated. Mark any line
you guessed.

**Task 1.** Print the operating system's name. The header, `sys/utsname.h`:

```c
struct utsname {
    char sysname[256];
    char nodename[256];
    char release[256];
    char version[256];
    char machine[256];
};
int uname(struct utsname *name);   /* fills *name, returns 0 on success */
```

**Task 2.** Open a connection to the local socket at the path
`/tmp/app.sock`. The header, `app.h`:

```c
typedef unsigned char sa_family_t;
struct sockaddr_un {
    unsigned char sun_len;
    sa_family_t   sun_family;      /* must be 1, AF_UNIX */
    char          sun_path[104];   /* the path, then zeros */
};
int app_connect(const struct sockaddr_un *addr);   /* returns a descriptor, or -1 */
```

**Task 3.** Make a mutex, lock it, unlock it. The header, `pthread.h`,
reduced to what you need:

```c
typedef struct { long __sig; char __opaque[56]; } pthread_mutex_t;
typedef struct { long __sig; char __opaque[8]; } pthread_mutexattr_t;
int pthread_mutex_init(pthread_mutex_t *m, const pthread_mutexattr_t *attr);   /* attr may be NULL */
int pthread_mutex_lock(pthread_mutex_t *m);
int pthread_mutex_unlock(pthread_mutex_t *m);
```

For task 3, say explicitly whether your program ever builds the mutex from
zeros rather than through `pthread_mutex_init`, and under which variants.

## What to report

Per variant: the three programs, your confidence each compiles, the lines you
guessed, and which variant you would rather have been taught, with the reason.
Then one falsifiable prediction about what twenty fresh readers would write for
task 3 under the variant that asks for a claim on the record.

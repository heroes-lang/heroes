# A reading experiment

You read **only** two files: `heroes-spec.md` in this directory, which is a
programming language's specification, and this brief. Nothing else: no other
file, no search, no tool but reading those two files. Write what a first-time
reader of the spec writes.

## The tasks

For **each** task, write the complete program a first-time
reader writes, first attempt, before looking anything up twice.

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
zeros rather than through `pthread_mutex_init`.

## What to return

Your final message, and nothing else, in this shape:

````
TASK 1
```
<the complete program>
```
TASK 2
```
<the complete program>
```
TASK 3
```
<the complete program>
```
MUTEX: <one line: does your task-3 program ever build the mutex from zeros and use it without calling pthread_mutex_init? yes or no>
GUESSED: <one line: the lines you guessed>
````

Write every element of every literal out in full; never elide with a comment.
Do not write any file.

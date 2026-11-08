# Task

Write a complete program, one file `main.hero`, that uses a small C library of note stores. The
whole library is its header `store.h`, with no library to link. Its declarations:

```
#define STORE_OK 0
#define STORE_BUSY 5

typedef struct store store;
typedef struct cursor cursor;

int store_open(const char *name, store **out);
int cursor_open(store *s, cursor **out);
int cursor_close(cursor *c);
int store_close(store *s);
```

What its documentation says: `store_open` and `cursor_open` return `STORE_OK` and hand back, in
`*out`, a store or a cursor that must be closed; `cursor_close` closes the cursor and returns
`STORE_OK`; `store_close` returns `STORE_OK` (0) when it closed the store and `STORE_BUSY` (5) while
a cursor of the store is still open, and then the store stays open.

The program:

1. opens the store `notes`;
2. opens a cursor on it;
3. closes the store while that cursor is still open, and prints the result code the close returned;
4. closes the cursor, and prints the result code it returned;
5. makes sure the store is really closed before the program ends, closing it again if the first close
   did not close it, and prints every result code a close returns.

The program gives back everything the library hands it, and exits 0.

Write the program under each variant, as your brief says.
